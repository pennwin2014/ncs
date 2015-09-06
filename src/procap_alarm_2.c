/* 监控软件---有关Web的应用
     Modify 2002/9/9   by Liyunming*/
#define PAS_SRCFILE      9889     
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <string.h>
#include <locale.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/time.h>
#include "utoall.h"
#include "utoplt01.h"
#include "pasdb.h"
#include "dsutl.h"
#include "dsclt.h"
#include "ncsdef.h"
#include"dlfcn.h"

int ncsSys_WarnLog(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
	char caWhere[16];     //上网场所
	char caMac[24];       //MAC地址
	int  iStatus = 0;     //状态
	int  iTime = 0;       //告警时间
	long long lMS = 0;    //关联日志
	int  iRule = 0;       //触发规则ID
	char caName[32];      //触发规则名称
	char caMessage[256];  //告警描述
	char caSerName[64];
	int  iaCid[100] = {0};
	int  iSid = 0;
	int  iCid = 0;
	int  iNum = 0;
	int  iCount = 0;
	int  iReturn, i;
	
	int ipage = 0;//分页变量
	int istart = 0;
	int ilimit = 0;
	                   // 标志位
	int  iCidF = 0;    // 1--所有的Cid  2--对应Cid
	int  iKeywordF = 0;// 1--无查询数据  2--有查询数据
	int  iStatusF = 0; // 1--全部      2--待处理/未处理
	
	char caDel[16];//获取到对应的数据
	char caSesid[1024];
	char caUpdate[16];
	char caKeyword[64];
	char caFlags[12];
	char caSid[20];
	char caCid[20];
	char caGname[64];
	char caPage[12];
	char caLimit[16];
	char caSort[16];
	char caDir[16];
	char caStatus[8];
	char sql[256];
	char temp[1024];
	
	char *p = NULL;
	utPltDbHead *psDbHead = NULL;
	pasDbCursor *psCur = NULL;
	pasDbCursor *psCod = NULL;
	utMsgPrintMsg(psMsgHead);
	psDbHead = utPltInitDb();//初始化缓冲区
	
	iReturn = utMsgGetSomeNVar(psMsgHead,13,
					"del",UT_TYPE_STRING,10,caDel,
					"selsid",UT_TYPE_STRING,1000,caSesid,
					"update",UT_TYPE_STRING,10,caUpdate,
					"keyword",UT_TYPE_STRING,64,caKeyword,	
					"status",UT_TYPE_STRING,8,caStatus,
					"cid",UT_TYPE_STRING,20,caCid,
					"page",UT_TYPE_STRING,10,caPage,
		            "limit",UT_TYPE_STRING,10,caLimit,
		            "sort",UT_TYPE_STRING,15,caSort,
		            "dir",UT_TYPE_STRING,8,caDir);
	//分页处理
	printf("********************************************\n");
	ipage = atoi(caPage);
	ilimit = atoi(caLimit);
	istart = (ipage - 1) * ilimit;
	
	//设为已处理
	if(strlen(caSesid) >= 0) {
		strcpy(temp,caSesid);
		p = strtok(temp,",");
		while(p) {
			iReturn = pasDbExecSqlF("update ncscasemacwarnlog set flags=1 where sid=%s",p);
			p = strtok(NULL,",");
		}
	}
	
	if((!strcmp(caCid,"allWarn")) || (strlen(caCid) == 0)) iCidF = 1;
    else iCidF = 2;
    if(strlen(caKeyword) > 0) iKeywordF = 2;
    else iKeywordF = 1;
    if(strlen(caStatus) > 0) iStatusF = 2;	
	else iStatusF = 1;
	
	//根据条件进行选择执行
	sprintf(sql,"%s","select sid,servicecode,mac,flags,stime,msid,ruleid,message,cid from ncscasemacwarnlog ");
	sprintf(temp,"%s","select count(*) from ncscasemacwarnlog ");
	switch(iCidF) {
		case 1:
		    switch(iKeywordF) {
				case 1:
					switch(iStatusF) {
						case 2:
							sprintf(sql,"%s where flags=%s ",sql,caStatus);
							sprintf(temp,"%s where flags=%s  ",temp,caStatus);
							break;
						default:
							break;
					}
					break;
				case 2:
					sprintf(sql,"%s where mac like '%c%s%c' ",sql,'%',caKeyword,'%');
					sprintf(temp,"%s where mac like '%c%s%c' ",temp,'%',caKeyword,'%');
					switch(iStatusF) {
						case 2:
							sprintf(sql,"%s and flags=%s ",sql,caStatus);
							sprintf(temp,"%s and flags=%s  ",temp,caStatus);
							break;
						default:
							break;
					}
					break;
				default:
					break;
			}
			break;
		case 2:
			sprintf(sql,"%s where cid=%s ",sql,caCid);
			sprintf(temp,"%s where cid=%s ",temp,caCid);
			switch(iKeywordF) {
				case 1:
					switch(iStatusF) {
						case 2:
							sprintf(sql,"%s and flags=%s ",sql,caStatus);
							sprintf(temp,"%s and flags=%s  ",temp,caStatus);
							break;
						default:
							break;
					}
					break;
				case 2:
				sprintf(sql,"%s and mac like '%c%s%c' ",sql,'%',caKeyword,'%');
				sprintf(temp,"%s and mac like '%c%s%c' ",temp,'%',caKeyword,'%');
					switch(iStatusF) {
						case 2:
							sprintf(sql,"%s and flags=%s ",sql,caStatus);
							sprintf(temp,"%s and flags=%s  ",temp,caStatus);
							break;
						default:
							break;
					}
					break;
				default:
					break;
			}
		    break;
		default:
		    break;
	}
	iReturn = pasDbOneRecord(temp, 0, UT_TYPE_LONG, 4, &iCount);
	utPltPutVarF(psDbHead, "TotRec", "%d", iCount);
	psCur = pasDbOpenSqlF("%s order by sid desc limit %d,%d   ",sql,istart,ilimit);
	
	if(NULL == psCur) {
		printf("error!!!!!!!!!!!!!!!!!!!\n");
		return 0;
	}
	memset(caWhere, 0, sizeof(caWhere));
	memset(caMac, 0, sizeof(caMac));
	memset(caMessage, 0, sizeof(caMessage));
	memset(caSerName,0,sizeof(caSerName));
	
	
	//取当前记录的内容到输出变量
	iReturn = pasDbFetchInto(psCur, UT_TYPE_ULONG,4,&iSid,
									UT_TYPE_STRING,14,caWhere,
									UT_TYPE_STRING,20,caMac,
									UT_TYPE_ULONG,4,&iStatus,
									UT_TYPE_ULONG,4,&iTime,
									UT_TYPE_ULONG,8,&lMS,
									UT_TYPE_ULONG,4,&iRule,
									UT_TYPE_STRING,255,caMessage,
									UT_TYPE_ULONG,4,&iCid);								
	printf("iSid=%d,caWhere=%s,iRule=%d,iCid=%d\n",iSid,caWhere,iRule,iCid);
	while(0 == iReturn || 1405 == iReturn) {
		
		if(iNum > 0) {
			utPltPutLoopVar(psDbHead,"dh",iNum+1,",");
		}
		iaCid[iNum] = iCid;
		iNum ++;
		utPltPutLoopVarF(psDbHead,"sid",iNum,"%lu",iSid);
		sprintf(caSerName,"%s",getServiceNameByCode(psShmHead,caWhere));
		utPltPutLoopVar(psDbHead,"servicecode",iNum,caSerName);
		utPltPutLoopVarF(psDbHead,"flags",iNum,"%lu",iStatus);
		utPltPutLoopVar(psDbHead,"stime",iNum,utTimFormat("%Y-%m-%d %H:%M:%S",iTime));
		utPltPutLoopVarF(psDbHead,"msid",iNum,"%lu",lMS);
		utPltPutLoopVar(psDbHead,"message",iNum,caMessage);
		utPltPutLoopVar(psDbHead,"mac",iNum,caMac);
		
		//printf("caWhere=%s,caMac=%s,iStatus=%d,iTime=%d,result=%s,caWhereyy=%s\n",caSerName,caMac,iStatus,iTime,getServiceNameByCode(psShmHead,caWhere),caWhere);
		memset(caWhere, 0, sizeof(caWhere));
		memset(caSerName,0,sizeof(caSerName));
		memset(caMac, 0, sizeof(caMac));
		iStatus = 0;
		iTime = 0;
		lMS = 0;
		iRule = 0;
		iSid = 0;
		memset(caMessage, 0, sizeof(caMessage));
		
		iReturn = pasDbFetchInto(psCur,UT_TYPE_ULONG,4,&iSid, 
									UT_TYPE_STRING,14, caWhere,
									UT_TYPE_STRING,23,caMac,
									UT_TYPE_ULONG,4,&iStatus,
									UT_TYPE_ULONG,4,&iTime,
									UT_TYPE_ULONG,8,&lMS,
									UT_TYPE_ULONG,4,&iRule,
									UT_TYPE_STRING,255,caMessage,
									UT_TYPE_ULONG,4,&iCid);	
		
	}
	pasDbCloseCursor(psCur);	
	for(i=0; i<iNum; i++) {
		memset(caGname,0,sizeof(caGname));
		if(iaCid[i] > 0) {
			sprintf(sql,"select casename from ncscaseinfo where sid=%d ",iaCid[i]);
			iReturn = pasDbOneRecord(sql,0,UT_TYPE_STRING,32,caGname);
			utPltPutLoopVar(psDbHead,"ruleid",i + 1,caGname);
		}
	}
	utPltPutVarF(psDbHead,"TotRec","%d",iNum);
	printf("iNum=%d\n",iNum);
	utPltOutToHtml(iFd,psMsgHead,psDbHead,"normally/proauth_warninfo.htm");
	return 0;	
}

//关联日志显示
int ncsSys_WarnLogAss(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
	char caMac[20] = "";
	char caTermtype[128] = "";
	char caSerName[128] = "";
	char caAddress[128] = "";
	char caRssi[16] = "";
	char caVname[64] = "";
	char caSSID[256] = "";
	char caApname[32] = "";
	char caApmac[20] = "";
	char caChannel[8] = "";
	char caSecurity[8] = "";
	char caXpos[8] = "";
	char caYpos[8] = "";
	char caLongitude[16] = "";
	char caLatitude[16] = "";
	char caTemp_d[64] = "";
	char caTemp[256] = "";
	long long iStime = 0;
	int  iVtype = 0;
	int  iFlags = 0;
	
	int  iReturn = 0;
	int  iNum = 0;
	char sql[255];
	char *p = NULL;
	
	char caDir[16] = "";
	char caDel[16] = "";
	char caSid[20] = "";
	char caStime[16] = "";
	char caPage[12] = "";
	char caLimit[16] = "";
	char caSort[16] = "";
	char caSesid[1000] = "";
	char caUpdate[16] = "";
	char caCid[32] = "";
	char caKeyword[64] = "";
	char caGname[64] = "";
	char caMsid[32] = "";
	
	utMsgPrintMsg(psMsgHead);
    pasDbCursor *psCur = NULL;
    utPltDbHead *psDbHead = utPltInitDb();  // 初始化数据模板
	
    iReturn = utMsgGetSomeNVar(psMsgHead, 7,
                               "del", UT_TYPE_STRING, 10, caDel,                         
                               "sid", UT_TYPE_STRING, 4, caSid,
                               "stime", UT_TYPE_STRING, 10, caStime,                      
                               "page", UT_TYPE_STRING, 10, caPage,
                               "limit", UT_TYPE_STRING, 10, caLimit,
                               "sort", UT_TYPE_STRING, 15, caSort,
                               "dir", UT_TYPE_STRING, 8,  caDir);
	p = caStime;
	p[4] = p[5];  p[5] = p[6];  p[6] = '\0';  //时间格式转换
	//printf("********caMsid=%s,caSid=%s,caStime=%s,caCid=%s>>>>>>>\n",caMsid,caSid,caStime,caCid);
	
	//获得触发规则
	sprintf(sql,"select msid from ncscasemacwarnlog where sid=%s  ",caSid);
	iReturn = pasDbOneRecord(sql,0,UT_TYPE_STRING,32,caMsid);
	
	printf("caMsid=%s,caSid=%s,caStime=%s\n",caMsid,caSid,caStime);
	psCur = pasDbOpenSqlF("select mac,termtype,servicename,address,stime,rssi,vtype,vname,ssid,apname,apmac,channel,security,xpos,ypos,longitude,latitude,flags from ncmactermatt_if_%s where sid=%s  ",caStime,caMsid);
	if(NULL == psCur) {
		return 0;
	}
	memset(caMac, 0, sizeof(caMac));
	memset(caTermtype, 0, sizeof(caTermtype));
	memset(caSerName, 0, sizeof(caSerName));
	memset(caAddress, 0, sizeof(caAddress));
	memset(caRssi, 0, sizeof(caRssi));
	memset(caVname, 0, sizeof(caVname));
	memset(caSSID, 0, sizeof(caSSID));
	memset(caApname, 0, sizeof(caApname));
	memset(caApmac, 0, sizeof(caApmac));
	memset(caChannel, 0, sizeof(caChannel));
	memset(caXpos, 0, sizeof(caXpos));
	memset(caYpos, 0, sizeof(caYpos));
	memset(caLongitude, 0, sizeof(caLongitude));
	memset(caLatitude, 0, sizeof(caLatitude));
	
	iReturn = pasDbFetchInto(psCur, UT_TYPE_STRING,17,caMac,
									UT_TYPE_STRING,128,caTermtype,
									UT_TYPE_STRING,128,caSerName,
									UT_TYPE_STRING,128,caAddress,
									UT_TYPE_ULONG,8,&iStime,
									UT_TYPE_STRING,8,caRssi,
									UT_TYPE_ULONG,4,&iVtype,
									UT_TYPE_STRING,64,caVname,
									UT_TYPE_STRING,255,caSSID,
									UT_TYPE_STRING,21,caApname,
									UT_TYPE_STRING,17,caApmac,
									UT_TYPE_STRING,2,caChannel,
									UT_TYPE_STRING,2,caSecurity,
									UT_TYPE_STRING,8,caXpos,
									UT_TYPE_STRING,8,caYpos,
									UT_TYPE_STRING,11,caLongitude,
									UT_TYPE_STRING,11,caLatitude,
									UT_TYPE_ULONG,4,&iFlags);
		//printf("caTermtype=%s,caSerName=%s,caAddress=%s,iStime=%lld,caRssi=%s,iVtype=%d,caVname=%s,caSSID=%s,caApname=%s,caApmac=%s,caChannel=%s,caSecurity=%s,caXpos=%s,caYpos=%s\n",caTermtype,caSerName,caAddress,iStime,caRssi,iVtype,caVname,caSSID,caApname,caApmac,caChannel,caSecurity,caXpos,caYpos);
	while(0 == iReturn || 1405 == iReturn) {
		if(iNum > 0) {
			utPltPutLoopVar(psDbHead,"dh",iNum+1,",");
		}
		iNum ++;
		utPltPutLoopVar(psDbHead,"mac",iNum,caMac);
		utPltPutLoopVar(psDbHead,"termtype",iNum,caTermtype);
		utPltPutLoopVar(psDbHead,"servicename",iNum,caSerName);
		utPltPutLoopVar(psDbHead,"address",iNum,caAddress);
		utPltPutLoopVar(psDbHead,"stime",iNum,utTimFormat("%Y-%m-%d %H:%M:%S",iStime));
		utPltPutLoopVar(psDbHead,"rssi",iNum,caRssi);
		utPltPutLoopVarF(psDbHead,"vtype",iNum,"%lu",iVtype);
		utPltPutLoopVar(psDbHead,"vname",iNum,caVname);
		utPltPutLoopVar(psDbHead,"ssid",iNum,caSSID);
		utPltPutLoopVar(psDbHead,"apname",iNum,caApname);
		utPltPutLoopVar(psDbHead,"apmac",iNum,caApmac);
		utPltPutLoopVar(psDbHead,"channel",iNum,caChannel);
		utPltPutLoopVar(psDbHead,"security",iNum,caSecurity);
		utPltPutLoopVar(psDbHead,"xpos",iNum,caXpos);
		utPltPutLoopVar(psDbHead,"ypos",iNum,caYpos);
		utPltPutLoopVar(psDbHead,"longitude",iNum,caLongitude);
		utPltPutLoopVar(psDbHead,"latitude",iNum,caLatitude);
		utPltPutLoopVarF(psDbHead,"flags",iNum,"%lu",iFlags);
		
	memset(caMac, 0, sizeof(caMac));
	memset(caTermtype, 0, sizeof(caTermtype));
	memset(caSerName, 0, sizeof(caSerName));
	memset(caAddress, 0, sizeof(caAddress));
	memset(caRssi, 0, sizeof(caRssi));
	memset(caVname, 0, sizeof(caVname));
	memset(caSSID, 0, sizeof(caSSID));
	memset(caApname, 0, sizeof(caApname));
	memset(caApmac, 0, sizeof(caApmac));
	memset(caChannel, 0, sizeof(caChannel));
	memset(caXpos, 0, sizeof(caXpos));
	memset(caYpos, 0, sizeof(caYpos));
	memset(caLongitude, 0, sizeof(caLongitude));
	memset(caLatitude, 0, sizeof(caLatitude));
	iFlags = 0;
	iStime = 0;
	iVtype = 0;
	
	iReturn = pasDbFetchInto(psCur, UT_TYPE_STRING,17,caMac,
									UT_TYPE_STRING,128,caTermtype,
									UT_TYPE_STRING,128,caSerName,
									UT_TYPE_STRING,128,caAddress,
									UT_TYPE_ULONG,8,&iStime,
									UT_TYPE_STRING,8,caRssi,
									UT_TYPE_ULONG,4,iVtype,
									UT_TYPE_STRING,64,caVname,
									UT_TYPE_STRING,255,caSSID,
									UT_TYPE_STRING,21,caApname,
									UT_TYPE_STRING,17,caApmac,
									UT_TYPE_STRING,2,caChannel,
									UT_TYPE_STRING,2,caSecurity,
									UT_TYPE_STRING,8,caXpos,
									UT_TYPE_STRING,8,caYpos,
									UT_TYPE_STRING,11,caLongitude,
									UT_TYPE_STRING,11,caLatitude,
									UT_TYPE_ULONG,4,&iFlags);
	
	}			
	pasDbCloseCursor(psCur);
	utPltPutVarF(psDbHead,"TotRec","%d",iNum);
	printf("iNum=%d\n",iNum);
	utPltOutToHtml(iFd,psMsgHead,psDbHead,"normally/proauth_warnlog.htm");
	return 0;
}

//场所信息查询
int ncsPlace_mactermlist_info2(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
	char caPage[12],caLimit[16],caSort[16],caDir[16];
	char caServicecode[32];
	char caServicename[32];
	char caKeyword[32];
	char sdate[11] = "";
   	char edate[11] = "";
	char caStime[16] = "";
   	char caEtime[16] = "";
	char caTimeFlag[32] = "";
	
	char caSerCode[20] = "";
	char caSerName[128] = "";
	long lCurPg,lRowNum,lStartRec,lSstarttime;
	int  _break = 0;
	int  iMacDis = 0;
	int  iMac = 0;
	int  iApname = 0;
	
	struct sttable
	{
		char tname[32];
		unsigned long lCount;
	};
	struct sttable tableinfo[120];
	
	//int  ipage = 0;
	//int  istart = 0;
	//int  ilimit =  0;
	//int  iCount = 0;
	//int  iTemp = 0;
    int  i = 0;
   
	char sql[1024];
	char temp[1024];
	int  idays = 0;
	int  iNum = 0;
	int  iReturn = 0;
	
	unsigned long iStart = 0;
	unsigned long iLimit = 0;
	unsigned long lCount = 0;
	unsigned long lStartTime = 0;
   	unsigned long lTime = time(0);
	
	char **ptmp = NULL;	
	
	utMsgPrintMsg(psMsgHead);
	pasDbCursor *psCur = NULL;
	utPltDbHead *psDbHead = utPltInitDb();
	
	iReturn = utMsgGetSomeNVar(psMsgHead,22,	
			"sdate",        UT_TYPE_STRING,12,sdate,
			"stime",        UT_TYPE_STRING,10,caStime,
			"edate",        UT_TYPE_STRING,12,edate,
			"etime",        UT_TYPE_STRING,10,caEtime,
			"keyword", 		UT_TYPE_STRING,60,caKeyword,
			"time_flag",    UT_TYPE_STRING,8,caTimeFlag,
			"servicecode",  UT_TYPE_STRING,15,caServicecode,
			"servicename",  UT_TYPE_STRING,60,caServicename,
			"page",         UT_TYPE_STRING,10,caPage,
			"limit",        UT_TYPE_STRING,10,caLimit,
			"sort",         UT_TYPE_STRING, 15, caSort,
			"dir",          UT_TYPE_STRING, 8,  caDir);		
	//分页变量	
	lCurPg = atol(caPage);
    if(lCurPg <= 0) lCurPg = 1;

    if(strlen(caLimit)>0)
	{
		lRowNum=atol(caLimit);
    }
    else
	{
        lRowNum =40;//每一页面行数
    }
    lStartRec = (lCurPg - 1) * lRowNum;
	//ipage = atoi(caPage);
	//ilimit = atoi(caLimit);
	//istart = (ipage - 1) * ilimit;
			
	switch(atoi(caTimeFlag)) {
   		case 1://今天
   			idays = 1;  break;
   		case 2://最近三天
   			idays = 3;  break;
   		case 3://最近一周
   			idays = 7;  break;
   		case 4://最近一月
   			idays = 30; break;
   		case 5://最近三月
   			idays = 90; break;
   		case 6://最近半年
   			idays = 180;break;
   		default:
   			idays = 1;  break;
   	}	
	//获取对应的时间
	if(strlen(sdate) > 0 && strlen(edate) > 0) {
		ptmp = (char **)ncsUtlGetTable3(sdate, edate,caStime,caEtime, "ncmachotinfo_if_", &lStartTime, &lTime, &lCount);
		printf("lStartTime=%d,sdate=%s\n",lStartTime,sdate);
	} else {
		ptmp = (char **)ncsUtlGetTable(lTime, idays, "ncmachotinfo_if_",  &lStartTime, &lCount);
		lTime=lTime+2*3600;
	}
	printf("*********************************\n");
	printf("ptmp[0]=%s,lStartTime=%ld,lTime=%ld,lCount=%ld\n",ptmp[0],lStartTime,lTime,lCount); //获取当前时间
	
	unsigned long lCount2 = 0;
	unsigned long lTotRec2 = 0;
	int num2 = 0;
	for(i=0; i<lCount; i++){
		printf("ptmp[%d]=%s\n",i,ptmp[i]);
		//strcpy(ptmp[i],"ncmachotinfo_if_201508");
	}
	
	//分页处理
	
	for(i=0; i<lCount; i++) {
		lCount2 = 0;
		memset(temp,0,sizeof(temp));
		if(strlen(sdate)>0 && strlen(edate)>0) {//高级查询
			sprintf(temp,"select count(distinct servicecode) from %s where stime>=%lu and stime<=%lu ",ptmp[i],lStartTime,lTime);
			if(strlen(caServicecode) > 0) {
				sprintf(temp,"%s and servicecode like '%c%s%c' ",temp,'%',caServicecode,'%');
			}
			if(strlen(caServicename) > 0) {
				sprintf(temp,"%s and servicename like '%c%s%c' ",temp,'%',caServicename,'%');
			}
		} else {
			if(strlen(caKeyword) > 0) {//关键字查询
				sprintf(temp,"select count(distinct servicecode) from %s where stime>=%lu and stime<=%lu and servicecode like '%c%s%c'  ",ptmp[i],lStartTime,lTime,'%',caKeyword,'%');
			} else {//无关键字查询
				sprintf(temp,"select count(distinct servicecode) from %s where stime>=%lu and stime<=%lu  ",ptmp[i],lStartTime,lTime);
				printf("*****temp=%s\n",temp);
			}
		}
		
		iReturn = pasDbOneRecord(temp, 0, UT_TYPE_LONG, 4, &lCount2);
		if(lCount2 > 0)
		{
			strcpy(tableinfo[num2].tname, ptmp[i]);
			tableinfo[num2].lCount = lCount2;
			lTotRec2 += lCount2;
			num2++;
		}
		printf("temp=%s,lCount2=%ld\n",temp,lCount2);		
	}
	
	//模拟
    //num2 = 1;
	//strcpy(tableinfo[0].tname,"ncmachotinfo_if_201508");
	//tableinfo[0].lCount = 3;
	//lTotRec2 =3;
	utPltPutVarF(psDbHead, "TotRec", "%ld", lTotRec2);
	printf("lTotRec2=%ld,num2=%d\n",lTotRec2,num2);
	
	for(i=num2-1; i>=0; i--){
		printf("***%s\n",tableinfo[i].tname);
	}
	
	iNum = 0;
	iStart = lStartRec;
	iLimit = lRowNum;
	for(i = num2-1; i >= 0; i--)
	{
		memset(sql, 0, sizeof(sql));
		if(tableinfo[i].lCount <= iStart)
		{	
			iStart-=tableinfo[i].lCount;
			continue;
		}	
		//得到sql
		if(strlen(sdate)>0 && strlen(edate)>0) {//高级查询
			sprintf(sql,"select servicecode,servicename,count(distinct(mac)),count(mac),count(distinct(apname)) from %s where stime>%lu and stime<%lu ",tableinfo[i].tname,lStartTime,lTime);
			if(strlen(caServicecode) > 0) {
				sprintf(sql,"%s and servicecode like '%c%s%c' ",sql,'%',caServicecode,'%');
			}
			if(strlen(caServicename) > 0) {
				sprintf(sql,"%s and servicename like '%c%s%c' ",sql,'%',caServicename,'%');
			}
			sprintf(sql,"%s group by servicecode  ",sql);
		} else {
			if(strlen(caKeyword) > 0) {//关键字查询
				sprintf(sql,"select servicecode,servicename,count(distinct(mac)),count(mac),count(distinct(apname)) from %s where stime>%lu and stime<%lu and servicecode like '%c%s%c' group by servicecode  ",tableinfo[i].tname,lStartTime,lTime,'%',caKeyword,'%');
			} else {//无关键字查询
				sprintf(sql,"select servicecode,servicename,count(distinct(mac)),count(mac),count(distinct(apname)) from %s where stime>%lu and stime<%lu group by servicecode  ",tableinfo[i].tname,lStartTime,lTime);
			}
		}
		
		if( tableinfo[i].lCount > iStart && tableinfo[i].lCount < iStart+iLimit )
		{
			iLimit = iLimit -  (tableinfo[i].lCount-iStart);
			iStart = 0;
		}
		else 
		{
			printf("iStart=%d,tableinfo[i].lCount=%ld,iLimit=%d,exit!!!!!!!!!!!!!!!\n",iStart,tableinfo[i].lCount,iLimit);
			_break = 1;
		}
		printf("%s\n",sql);
		//printf("__line = %d, sql = %s\n", __LINE__, sql);
		psCur = pasDbOpenSql(sql, 0);
		if(NULL != psCur) {
			printf("nnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnn\n");
			memset(caSerCode, 0, sizeof(caSerCode));
			memset(caSerName, 0, sizeof(caSerName));
			iMac = 0;
			iApname = 0;
			iReturn = pasDbFetchInto(psCur, UT_TYPE_STRING,17,caSerCode,
											UT_TYPE_STRING,128,caSerName,
											UT_TYPE_ULONG,4,&iMacDis,									
											UT_TYPE_ULONG,4,&iMac,
											UT_TYPE_ULONG,4,&iApname);
							//printf("caSerCode=%s,caSerName=%s,iApname=%d,iMac=%d,iReturn=%d\n",caSerCode,caSerName,iApname,iMac,iReturn);
	
			while(0 == iReturn || 1405 == iReturn) {
				if(iNum > 0) {
					utPltPutLoopVar(psDbHead,"dh",iNum+1,",");
				}
				iNum ++;
				utPltPutLoopVar(psDbHead,"servicecode",iNum,caSerCode);
				utPltPutLoopVar(psDbHead,"servicename",iNum,caSerName);
				utPltPutLoopVarF(psDbHead,"macnum",iNum,"%lu",iMacDis);
				utPltPutLoopVarF(psDbHead,"macsum",iNum,"%lu",iMac);
				utPltPutLoopVarF(psDbHead,"apnamesum",iNum,"%lu",iApname);
		
				memset(caSerCode, 0, sizeof(caSerCode));
				memset(caSerName, 0, sizeof(caSerName));
				iMac = 0;
				iMacDis = 0;
				iApname = 0;
		
				iReturn = pasDbFetchInto(psCur, UT_TYPE_STRING,17,caSerCode,
											UT_TYPE_STRING,128,caSerName,
											UT_TYPE_ULONG,4,&iMacDis,
											UT_TYPE_ULONG,4,&iMac,
											UT_TYPE_ULONG,4,&iApname);
				//printf("caSerCode=%s,caSerName=%s,iApname=%d,iMac=%d,iReturn=%d\n",caSerCode,caSerName,iApname,iMac,iReturn);
			}			
			pasDbCloseCursor(psCur);
		}
		if(_break == 1)  break; 
	}	
	//utPltPutVarF(psDbHead,"TotRec","%d",iNum);
	printf("iNum=%d\n",iNum);
	utPltOutToHtml(iFd,psMsgHead,psDbHead,"rzcx/proauth_mactermlist_info2.htm");
	return 0;
}

//场所信息具体查询
int ncsPlace_mactermlist_log2(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
	char instr_t[8192] = "";	
	int iTime_flag = 0;
	int days = 0;   
	int iret = 0;
	int _break = 0;
	int i = 0;
	int lCount = 0;
	int num = 0;
	int iNum = 0;
	
	unsigned long did = 0;
	unsigned long iStart = 0;
	unsigned long iLimit = 0;
	unsigned long lCount1 = 0;
	unsigned long lStartTime = 0;
   	unsigned long lTime = time(0);
   	unsigned long lTotRec = 0;
	
   	char sdate[11] = "";
   	char edate[11] = "";
   	char time_flag[2] = "";
   	char start[17] = "";
   	char limit[17] = "";
   	char sort[17] = "";
   	char dir[9] = "";
   	long lCurPg,lRowNum,lStartRec,lSstarttime;
   	char sql[40960] = "";
	char caTotPg[16],caCurPg[16];
	char caPage[12],caLimit[16],caSort[16],caDir[16];
	char caKeyword[64],caTime_flag[32];
   	char sql2[256] = "";
   	char frmfile[256] = "";
   	char *tmpgbk = NULL;
	char **ptmp = NULL;	
	
	struct sttable
	{
		char tname[32];
		unsigned long lCount;
	};
	struct sttable tableinfo[120];//10年？
   	pasDbCursor *psCur = NULL;
	char caGroups[1024];
	char caFacs[1024];
	char caArea[256],caName2[256];
    char caTemp[1024];
	long iReturn;
	unsigned long lIp;
    char caStime[16]="";
   	char caEtime[16]="";
   	char caTemp_d[64];
   	char caGroupid[36],caDid[36];
    char caSdate[24],caEdate[24];
    char caApmac[36],caMac[36],caApname[64],caPosition[64];
    char caCorpname[128];
    char caCont[2048];
    char caServicecode[16];
	char caSsid[36] = "";
	char caUserName[64] = "";	
	strcpy(caFacs, (const char*)getDsFacDid());
	strcpy(caGroups, (const char*)getDsGroupid()); 
	
	/* 选择导出的话 */
	char caExport[10] = "";
	
	
	/* 高级查询搜索字段 */
	char caSearchtype[32] = "";
	char caSname[32] = "";
	char caServicegtype[32] = "";
	char caServicearea[36] = "";
	char caCeoname[32] = "";
	char caJytype[32] = "";
    printf("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n");
	utPltDbHead *psDbHead = utPltInitDb();
	utMsgPrintMsg(psMsgHead);
	iReturn = utMsgGetSomeNVar(psMsgHead,27,
			"TotPg",      	UT_TYPE_STRING, 12, caTotPg,
			"CurPg",      	UT_TYPE_STRING, 12, caCurPg,
			"keyword", 		UT_TYPE_STRING,60,	caKeyword,
			"page",         UT_TYPE_STRING,10,	caPage,
			"limit",        UT_TYPE_STRING,10,	caLimit,
			"sort",         UT_TYPE_STRING,15, caSort,
			"dir",          UT_TYPE_STRING,8,  caDir,
			"time_flag",    UT_TYPE_STRING,8,	caTime_flag,
			"did",          UT_TYPE_STRING,10,	caDid,
			"groupid",      UT_TYPE_STRING,16,	caGroupid,
			"sdate",        UT_TYPE_STRING,12,	sdate,
			"stime",        UT_TYPE_STRING,10,	caStime,
			"edate",        UT_TYPE_STRING,12,	edate,
			"etime",        UT_TYPE_STRING,10,	caEtime,
			"apmac",        UT_TYPE_STRING,20,	caApmac,
			"mac",          UT_TYPE_STRING,20,	caMac,
			"apname",       UT_TYPE_STRING,31,	caApname,
			"username",     UT_TYPE_STRING,16,	caUserName,	
			"ssid",         UT_TYPE_STRING,30,	caSsid,			
			"servicename",  UT_TYPE_STRING,60,	caCorpname,		
			"servicecode",  UT_TYPE_STRING,15,	caServicecode,	
			
			"searchtype",   UT_TYPE_STRING,10,	caSearchtype,			
			"sname",      	UT_TYPE_STRING,31, caSname,
			"servicegtype", UT_TYPE_STRING,31, caServicegtype,
			"servicearea",  UT_TYPE_STRING, 35, caServicearea,
			"ceoname",      UT_TYPE_STRING, 31, caCeoname,
			"jytype",      	UT_TYPE_STRING, 31, caJytype);
	
	//MyremoveAll(caKeyword, '-');
	//MyremoveAll(caKeyword, ':');
   	  
	if(strlen(caSearchtype)>0)
	{
   	 	strcpy(caKeyword,"");
   	}
   	else
	{
   	  	strcpy(sdate,"");
   	  	strcpy(edate,"");
   	}
   	memset(caCont,0,sizeof(caCont));
   	 
	 
   	if(strlen(caKeyword)>0)
	{
      	strcpy(caTemp_d,pasCvtGBK(2,caKeyword,caTemp,60));
      	strcpy(caKeyword,caTemp_d);
    }
   	if(strlen(caCorpname)>0)
	{
      	strcpy(caTemp_d,pasCvtGBK(2,caCorpname,caTemp,60));
      	strcpy(caCorpname,caTemp_d);
    }   	 
   	if(strlen(caApname)>0)
	{
      	strcpy(caTemp_d,pasCvtGBK(2,caApname,caTemp,60));
      	strcpy(caApname,caTemp_d);
    }
   	if(strlen(caSsid)>0)
	{
      	strcpy(caTemp_d,pasCvtGBK(2,caSsid,caTemp,60));
      	strcpy(caSsid,caTemp_d);
    }
   	     	   	  
   	lCurPg = atol(caPage);
    if(lCurPg <= 0) lCurPg = 1;

    if(strlen(caLimit)>0)
	{
		lRowNum=atol(caLimit);
    }
    else
	{
        lRowNum =40;//每一页面行数
    }
    lStartRec = (lCurPg - 1) * lRowNum; 
   	  
   	iTime_flag = atoi(caTime_flag);
   	switch(iTime_flag)
   	{
   		case 1://今天
   			days = 1;
   			break;
   		case 2://最近三天
   			days = 3;
   			break;
   		case 3://最近一周
   			days = 7;
   			break;
   		case 4://最近一月
   			days = 30;
   			break;
   		case 5://最近三月
   			days = 90;
   			break;
   		case 6://最近半年
   			days = 180;
   			break;
   		default:
   			days = 1;
   			break;
   	}
	//新添加的测试代码
	days=180;
	
	if(strlen(sdate) > 0 && strlen(edate) > 0)
	{
		ptmp = (char **)ncsUtlGetTable3(sdate, edate,caStime,caEtime, "ncmactermatt_if_", &lStartTime, &lTime, &lCount);
		//printf("lStartTime=%d,sdate=%s\n",lStartTime,sdate);
	}
	else
	{
		ptmp = (char **)ncsUtlGetTable(lTime, days, "ncmactermatt_if_",  &lStartTime, &lCount);
		lTime=lTime+2*3600;
	}
	//printf("ptmp[0]=%s,ptmp[1]=%s,ptmp[2]=%s,ptmp[5]=%s\n",ptmp[0],ptmp[1],ptmp[2],ptmp[5]);
	memset(caCont,0,sizeof(caCont));
	
	/* 对输入框字体进行判断：如果有 @则不是关键字，否则则是 */
	char caGroupidJudge[30] = "";
	
	/* 高级查询条件 */	
	if(strlen(caSearchtype)>0)
	{
		if(strlen(caMac)>0)
		{
			sprintf(caCont+strlen(caCont)," and a.mac like '%c%s%c' ",'%',caMac,'%');
		}
		if(strlen(caApmac)>0)
		{
			sprintf(caCont+strlen(caCont)," and a.apmac like '%c%s%c' ",'%',caApmac,'%');
		}
		if(strlen(caApname)>0)
		{
			sprintf(caCont+strlen(caCont)," and a.apname like '%c%s%c' ",'%',caApname,'%');
		}		
		if(strlen(caCorpname)>0)
		{
			sprintf(caCont+strlen(caCont)," and a.servicename like '%c%s%c' ",'%',caCorpname,'%');
		}
		
		/* 高级查询条件 */
		if(strlen(caSname)>0)			//安全厂商名称查询	
		{
			//sprintf(caCont+strlen(caCont)," and servicename like '%c%s%c' ",'%',caSname,'%');
			printf(" 安全厂商名称为 %s \n\n", caSname);
		}
		if(strlen(caServicecode)>0)		//场所编号
		{
			sprintf(caCont+strlen(caCont)," and a.servicecode like '%c%s%c' ",'%',caServicecode,'%');
		}
		if(strlen(caServicegtype)>0)	//场所服务类型
		{
			//sprintf(caCont+strlen(caCont)," and servicename like '%c%s%c' ",'%',caIdname,'%');
			printf(" 场所服务类型 %s \n\n", caServicegtype);
		}
		if(strlen(caServicearea)>0)		//场所所属区域
		{
			printf(" 场所所属区域 %s \n\n", caServicearea);
		}	
		if(strlen(caCeoname)>0)			//场所法人或负责人姓名	
		{
			printf(" 场所法人或负责人姓名 %s \n\n", caCeoname);
		}
		if(strlen(caJytype)>0)			//场所经营性质	
		{
			printf(" 场所法人或负责人姓名 %s \n\n", caJytype);
		}
	}
	else
	{		
		//printf(" if outer caKeyword = %s, caGroupidJudge = %s\n\n", caKeyword, caGroupidJudge);	
		if(strlen(caKeyword)>0)
		{			
			/* 字符串中小写转大写 */
			int i = 0, j = 0;
			j = strlen(caKeyword);
			
			for(i = 0; i <= j; i++)
			{
				if(caKeyword[i]>='a' && caKeyword[i]<='z')		
					caKeyword[i] = caKeyword[i] - 32;
			}
			caKeyword[i] = '\0';						
			//printf("After strToUpper: caKeyword = %s \n", caKeyword);

			
			strncpy(caGroupidJudge, caKeyword, 1);
			if(strcmp(caGroupidJudge, "@") == 0)
			{
				//printf("caKeyword = %s, caGroupidJudge = %s\n\n", caKeyword, caGroupidJudge);					
				if(strlen(caGroupid)>0)
				{			
					//printf("In caKeyword @ caGroupid = %s\n", caGroupid);
					sprintf(caCont+strlen(caCont)," and a.servicecode in (%s)",getServicecodeByGroupid(psShmHead,caGroupid));
					//printf("caCont = %s \n", caCont);
				}
				else
				{		
					if(strlen(caUserName)>0)
					{		
						//printf("In caKeyword @ caUserName = %s\n", caUserName);
						
						//通过 caUserId 去获取相对应 lGroupId		
						sprintf(caCont+strlen(caCont)," and a.servicecode in ('%s')", caUserName);

						//printf("caCont = %s \n", caCont);
					}
				}
				

				/* 判断输入框caKeyword 是否有第二个'@'  字符: 如果有，则取其后面值作为搜索条件 */
				memset(caGroupidJudge, 0, sizeof(caGroupidJudge));
				char *pstrKeywordServiceName = NULL, *pstrKeywordMac = NULL;
				pstrKeywordServiceName = strtok_r(caKeyword, "@", &pstrKeywordMac);
				//printf("pstrKeywordServiceName, pstrKeywordMac = %s\n", pstrKeywordServiceName, pstrKeywordMac);

				/* @  场所@  后面如果输入了mac/apmac */
				if(strcmp(pstrKeywordMac, "") != 0)
					sprintf(caCont+strlen(caCont)," and (replace(a.mac,'-','') like '%c%s%c' or replace(a.apmac,'-','') like '%c%s%c') ",'%',pstrKeywordMac,'%','%',pstrKeywordMac,'%');
			}
			else
			{
				sprintf(caCont+strlen(caCont)," and (a.servicecode like '%c%s%c' or a.servicename like '%c%s%c') ",'%',caKeyword,'%','%',caKeyword,'%');			
			}			
		}
	}
	if(strlen(caDid)>0)
	{

		sprintf(caCont+strlen(caCont)," and a.servicecode='%s' ",getServicecodeByid(psShmHead,atol(caDid)));
	}
	
	/*  当前用户权限下所能查询场所*/
	if(strlen(caGroups)>0)
	{
        sprintf(caCont+strlen(caCont)," and a.servicecode in (%s)",getServicecodeByGroupid(psShmHead,caGroups));
	}
	

	char sql3[4096] = "";	
	unsigned long lCount2 = 0;
	unsigned long lTotRec2 = 0;
	int num2 = 0;
	
	/* 去重后查询结果 */
	for(i = 0; i < lCount; i++)
	{
		lCount2 = 0;
		memset(sql3, 0, sizeof(sql3));
		snprintf(sql3, sizeof(sql3), "select count(distinct a.mac) from %s a where a.stime >= %lu and a.stime <= %lu  ",  ptmp[i], lStartTime, lTime);
		strcat(sql3,caCont);
		//printf("sql3 = %s\n", sql3);
		pasDbOneRecord(sql3, 0, UT_TYPE_ULONG, 4, &lCount2);
		if(lCount2 > 0)
		{
			strcpy(tableinfo[num2].tname, ptmp[i]);
			tableinfo[num2].lCount = lCount2;
			num2++;
		}	
		//free(ptmp[i]);
	}
	//free(ptmp);	
	
	for(i = 0; i < num2; i++)
	{
		lTotRec2 += tableinfo[i].lCount;
	}
	//printf(" lCount2 = %lu  num2 = %d  lTotRec2 = %lu\n\n", lCount2, num2, lTotRec2);
	utPltPutVarF(psDbHead,"distinctCount", "%lu", lTotRec2);
	
	
	for(i = 0; i < lCount; i++)
	{
		lCount1 = 0;
		memset(sql, 0, sizeof(sql));
		snprintf(sql, sizeof(sql), "select count(*) from %s a where a.stime >= %lu and a.stime <= %lu  ", ptmp[i], lStartTime, lTime);
		strcat(sql,caCont);
		//printf("sql = %s\n", sql);
		pasDbOneRecord(sql, 0, UT_TYPE_ULONG, 4, &lCount1);
		if(lCount1 > 0)
		{
			strcpy(tableinfo[num].tname, ptmp[i]);
			tableinfo[num].lCount = lCount1;
			num++;
		}	
		free(ptmp[i]);
	}
	free(ptmp);	
	
	for(i = 0; i < num; i++)
	{
		lTotRec += tableinfo[i].lCount;
	}
	//printf(" lCount1 = %lu  num = %d  lTotRec = %lu\n\n", lCount1, num, lTotRec);
	utPltPutVarF(psDbHead,"TotRec", "%lu", lTotRec);

			
	for(i = num-1; i >= 0; i--)
	{
		//printf("table[%d] = %s, _lCount1[%d] = %lu\n", i, tableinfo[i].tname, i, tableinfo[i].lCount);
	}				
	
	/* 导出代码 */
	FILE *fp;
	char caFilename[32];
	char caFile[128];
	char caPath[128];
		
	//选择导出的话
	if(strlen(caExport) > 0)
	{	
		printf(" fp caExport  = %s\n\n", caExport);
		sprintf(caFilename,"placeLog.csv");
		sprintf(caPath,"%s", "/home/ncmysql/ncs");
		sprintf(caFile,"%s/%s",caPath,caFilename);
		fp = fopen(caFile,"w");	
		if(fp == NULL) 
		{				
			printf("数据导出出错\n\n");
			return 0;
		}
		fprintf(fp, "场所信息查询\n");	//标题
		printf(" 写入场所信息查询日志\n\n");			
		ncUtlPrintCsv(fp, 14,
				UT_TYPE_STRING, "场所名称",
				UT_TYPE_STRING, "采集设备名称",
				UT_TYPE_STRING, "安全厂商名称",
				UT_TYPE_STRING, "采集时间",				
				UT_TYPE_STRING, "采集终端MAC",
				UT_TYPE_STRING, "被采集终端场强",  
				UT_TYPE_STRING, "接入热点SSID",
				UT_TYPE_STRING, "接入热点MAC",
				UT_TYPE_STRING, "接入热点频道",
                UT_TYPE_STRING, "接入热点加密类型",				               
				UT_TYPE_STRING, "场所编号",
                UT_TYPE_STRING, "采集设备编号",				                
                UT_TYPE_STRING, "场所详细地址",											
                UT_TYPE_STRING, "安全厂商组织机构代码"); 
				
	}
	//printf(" num = %d\n\n", num);
	
	iNum = 0;
	iStart = lStartRec;
	iLimit = lRowNum;
	
	for(i = num-1; i >= 0; i--)
	{
		memset(sql, 0, sizeof(sql));
		if(tableinfo[i].lCount <= iStart)
		{	
			iStart-=tableinfo[i].lCount;
			continue;
		}	
		/* 加上关联字段 */
		//snprintf(sql, sizeof(sql), "select a.servicecode,a.servicename,d.address,a.mac,a.apname,a.apmac,a.ssid,a.stime,a.channel,a.rssi,a.security,a.longitude,a.latitude,a.termtype,a.vtype,a.vname, b.dispname, b.fcode, c.name from %s a, ncaplist b, ncsfactorycod c, ncsuser d where a.stime >= %lu and a.stime <= %lu and b.apname = RIGHT(a.apname, 12) and b.fcode = c.code and a.servicecode = d.username ", tableinfo[i].tname, lStartTime, lTime);
		
		/* 去掉字段 */
		snprintf(sql, sizeof(sql), "select a.servicecode,a.servicename,a.address,a.mac,a.apname,a.apmac,a.ssid,a.stime,a.channel,a.rssi,a.security,a.longitude,a.latitude,a.termtype,a.vtype,a.vname from %s a where a.stime >= %lu and a.stime <= %lu ", tableinfo[i].tname, lStartTime, lTime);
		
		
		strcat(sql,caCont);
		 
		snprintf(sql+strlen(sql), sizeof(sql)-strlen(sql), " order by a.stime desc limit %lu, %lu", iStart, iLimit < tableinfo[i].lCount ? iLimit : tableinfo[i].lCount);
				
		if( tableinfo[i].lCount > iStart && tableinfo[i].lCount < iStart+iLimit )
		{
			iLimit = iLimit -  (tableinfo[i].lCount-iStart);
			iStart = 0;
		}
		else 
		{
			_break = 1;
		}
	
		//printf("__line = %d, sql = %s\n", __LINE__, sql);
		psCur = pasDbOpenSql(sql, 0);
		if(psCur != NULL)
		{
         char caServicecode[16],caServicename[64],caAddress[64],caMac[20],caApname[32],caApmac[20],caSsid[64];
         long lStarttime,lEndtime,lFlags;
         char caChannel[32],caRssi[16],caSecurity[16],caLongitude[16],caLatitude[16];
         char caTermtype[128],caVname[64];
         long lVtype;
		 
		 /* 采集设备名称、安全厂商名称、厂商组织机构代码 */
		 char caDispname[32] = "";
		 char caFactoryname[32] = "";
		 char caFactorycode[32] = "";
		 
         memset(caServicecode,0,sizeof(caServicecode));
         memset(caServicename,0,sizeof(caServicename));
         memset(caAddress,0,sizeof(caAddress));
         memset(caMac,0,sizeof(caMac));
         memset(caApname,0,sizeof(caApname));
         memset(caApmac,0,sizeof(caApmac));
         memset(caSsid,0,sizeof(caSsid));
         lStarttime=0;
         memset(caChannel,0,sizeof(caChannel));
         memset(caRssi,0,sizeof(caRssi));
         memset(caSecurity,0,sizeof(caSecurity));
         memset(caLongitude,0,sizeof(caLongitude));
         memset(caLatitude,0,sizeof(caLatitude));
         memset(caTermtype,0,sizeof(caTermtype));
         memset(caVname,0,sizeof(caVname));
         lVtype=0;
			iret = pasDbFetchInto(psCur,
			     UT_TYPE_STRING,14,caServicecode,
			     UT_TYPE_STRING,63,caServicename,
			     UT_TYPE_STRING,63,caAddress,
			     UT_TYPE_STRING,18,caMac,
			     UT_TYPE_STRING,31,caApname,
			     UT_TYPE_STRING,18,caApmac,
			     UT_TYPE_STRING,63,caSsid,
			     UT_TYPE_LONG,4,&lStarttime,
			     UT_TYPE_STRING,2,caChannel,
			     UT_TYPE_STRING,8,caRssi,
			     UT_TYPE_STRING,2,caSecurity,
			     UT_TYPE_STRING,11,caLongitude,
			     UT_TYPE_STRING,11,caLatitude,
			     UT_TYPE_STRING,100,caTermtype,
			     UT_TYPE_LONG,4,&lVtype,
			     UT_TYPE_STRING,63,caVname,
				 UT_TYPE_STRING, 31, caDispname,
				 UT_TYPE_STRING, 31, caFactoryname,
				 UT_TYPE_STRING, 31, caFactorycode);
			   
					
			while((iret == 0) || (iret == 1405))
			{
				/* 选择导出的话 */
				if(strlen(caExport) > 0)
				{
					ncUtlPrintCsv(fp, 14,
						UT_TYPE_STRING, caServicename,	
						UT_TYPE_STRING, caDispname,
						UT_TYPE_STRING, caFactoryname,
						UT_TYPE_LONG, lStarttime,
						UT_TYPE_STRING, caMac,
						UT_TYPE_STRING, caRssi, 
						UT_TYPE_STRING, caSsid,
						UT_TYPE_STRING, caApmac,
						UT_TYPE_STRING, caChannel,
						UT_TYPE_STRING, caSecurity,                                                                					
						UT_TYPE_STRING, caServicecode,
						UT_TYPE_STRING, caApname,						
						UT_TYPE_STRING, caAddress,						
						UT_TYPE_STRING, caFactorycode); 
				}
				else
				{
					iNum++;
					if(iNum > 1)
					{
						utPltPutLoopVar(psDbHead,"dh",iNum,",");
					}
					utPltPutLoopVar(psDbHead,"stime",iNum,utTimFormat("%y/%m/%d %H:%M",lStarttime));      
					utPltPutLoopVar(psDbHead,"address",iNum,caAddress);
					utPltPutLoopVar(psDbHead,"apname",iNum,caApname);
					utPltPutLoopVar(psDbHead,"mac",iNum,caMac);
					utPltPutLoopVar(psDbHead,"apmac",iNum,caApmac);
					utPltPutLoopVar(psDbHead,"servicename",iNum,caServicename);
					utPltPutLoopVar(psDbHead,"channel",iNum,caChannel);
					utPltPutLoopVar(psDbHead,"rssi",iNum,caRssi);
					//printf("caSecurity=%s\n",caSecurity);
					utPltPutLoopVar(psDbHead,"security",iNum,caSecurity);
					utPltPutLoopVar(psDbHead,"longitude",iNum,caLongitude);
					utPltPutLoopVar(psDbHead,"latitude",iNum,caLatitude);		
					utPltPutLoopVar(psDbHead,"ssid",iNum,caSsid);
					utPltPutLoopVar(psDbHead,"servicecode",iNum,caServicecode);
					utPltPutLoopVar(psDbHead,"termtype",iNum,caTermtype);
					utPltPutLoopVar(psDbHead,"vname",iNum,caVname);
					utPltPutLoopVarF(psDbHead,"vtype",iNum,"%d",lVtype);
					
					utPltPutLoopVarF(psDbHead, "dispname", iNum, "%s", caDispname);
					utPltPutLoopVarF(psDbHead, "factoryname", iNum, "%s", caFactoryname);
					utPltPutLoopVarF(psDbHead, "factorycode", iNum, "%s", caFactorycode);
				}	
				
				memset(caServicecode,0,sizeof(caServicecode));
				memset(caServicename,0,sizeof(caServicename));
				memset(caAddress,0,sizeof(caAddress));
				memset(caMac,0,sizeof(caMac));
				memset(caApname,0,sizeof(caApname));
				memset(caApmac,0,sizeof(caApmac));
				memset(caSsid,0,sizeof(caSsid));
				lStarttime=0;
				memset(caChannel,0,sizeof(caChannel));
				memset(caRssi,0,sizeof(caRssi));
				memset(caSecurity,0,sizeof(caSecurity));
				memset(caLongitude,0,sizeof(caLongitude));
				memset(caLatitude,0,sizeof(caLatitude));
				memset(caTermtype,0,sizeof(caTermtype));
				memset(caVname, 0, sizeof(caVname));
				memset(caDispname, 0, sizeof(caDispname));
				memset(caFactoryname, 0, sizeof(caFactoryname));
				memset(caFactorycode, 0, sizeof(caFactorycode));
				
				lVtype=0;
				iret = pasDbFetchInto(psCur,
						UT_TYPE_STRING,14,caServicecode,
						UT_TYPE_STRING,63,caServicename,
						UT_TYPE_STRING,63,caAddress,
						UT_TYPE_STRING,18,caMac,
						UT_TYPE_STRING,31,caApname,
						UT_TYPE_STRING,18,caApmac,
						UT_TYPE_STRING,63,caSsid,
						UT_TYPE_LONG,4,&lStarttime,
						UT_TYPE_STRING,2,caChannel,
						UT_TYPE_STRING,8,caRssi,
						UT_TYPE_STRING,2,caSecurity,
						UT_TYPE_STRING,11,caLongitude,
						UT_TYPE_STRING,11,caLatitude,
						UT_TYPE_STRING,100,caTermtype,
						UT_TYPE_LONG,4,&lVtype,
						UT_TYPE_STRING,63,caVname,
						UT_TYPE_STRING, 31, caDispname,
						UT_TYPE_STRING, 31, caFactoryname,
						UT_TYPE_STRING, 31, caFactorycode);
			}			 
			pasDbCloseCursor(psCur);
		} 

		if(_break == 1)
			break; 
	}
	/* 选择导出的话 */
	if(strlen(caExport) > 0)
	{
		fclose(fp);
        utPltFileDownload(iFd,"application/text",caPath,caFilename,caFilename);
        remove(caFile);
		return 0;
	}


    utPltOutToHtml(iFd,psMsgHead,psDbHead,"rzcx/proauth_hotinfo_list.htm");
	printf("yyyyyyyyyyyyyyyyyyyyyyyyyyyy\n");
    return 0;
}

int procapAlarmSetFun_2(utShmHead *psShmHead){
	pasSetTcpFunName("ncsSys_WarnLog",ncsSys_WarnLog,0);
	pasSetTcpFunName("ncsSys_WarnLogAss",ncsSys_WarnLogAss,0);
	pasSetTcpFunName("ncsPlace_mactermlist_info2",ncsPlace_mactermlist_info2,0);
	//pasSetTcpFunName("ncsPlace_mactermlist_log2",ncsPlace_mactermlist_log2,0);	
}
