/* 监控软件---有关Web的应用
     Modify 2002/9/9   by Liyunming*/
#define PAS_SRCFILE      9888     
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
#include <time.h>
#include "utoall.h"
#include "utoplt01.h"
#include "pasdb.h"
#include "dsutl.h"
#include "dsclt.h"
#include "ncsdef.h"
#include"dlfcn.h"

#include "pasdef.h"

/* 头文件关键数据结构 */
#include "ncapi.h"
#include "ncmac.h"

#define INFOQUERY_DEBUG

typedef struct
{
    long rcount;
    long rtime;
} retDataPlace;

//ncsCurrent_FromRAM  保存实时信息的结构体
typedef  struct stCurrentInfo								
{
	uchar caServicename[68];
	uchar caDispname[68];		
	uint4 caStime;
	char    lrssi;                                   
	char    urssi; 	
	uchar caMac[36];
	uchar caLongitude[16];
	uchar caLatitude[16];
	uchar caSsid[36];
	uchar caApmac[36];
	uchar   encrypt;	
	uchar caServiceCode[20];
	uchar caApname[24];
	uchar caVname[64];
	uchar caTermtype[36];	
	
}stCurInfoForty;

//ncsCurrent_FromRAM  插入记录到结构体数组中
static int InsertItemToStuct(stCurInfoForty* pStCurrentForty, int iNum, ncApSrvOnline* psAp, ncLbsMacLocation *psMacLoc)
{
	char caMacStr[20] = "", caMacStr1[20] = "";			
	if(psAp && psMacLoc)
	{
		snprintf(pStCurrentForty[iNum].caServicename, sizeof(pStCurrentForty[iNum].caServicename)-1, "%s", psAp->servicename);
		snprintf(pStCurrentForty[iNum].caDispname, sizeof(pStCurrentForty[iNum].caDispname)-1, "%s", psAp->dispname);	
		
		pStCurrentForty[iNum].caStime = psMacLoc->starttime;
		//snprintf(pStCurrentForty[iNum].caStime, sizeof(pStCurrentForty[iNum].caStime)-1, "%lu", psMacLoc->starttime);
		
		//printf("psMacLoc->starttime = %lu\n\n", psMacLoc->starttime);
		//printf("InsertItemToStuct pStCurrentForty[%d].caStime) = %s\n\n", iNum, utTimFormat("%Y-%m-%d %H:%M:%S", pStCurrentForty[iNum].caStime));
		
		//printf("InsertItemToStuct pStCurrentForty[%d].caStime = %lu\n\n", iNum, pStCurrentForty[iNum].caStime);
		//printf("InsertItemToStuct pStCurrentForty[%d].caStime) = %s\n\n", iNum, utTimFormat("%Y-%m-%d %H:%M:%S", pStCurrentForty[iNum].caStime));		
		
		pStCurrentForty[iNum].lrssi = psMacLoc->lrssi;
		pStCurrentForty[iNum].urssi = psMacLoc->urssi;
		//snprintf(pStCurrentForty[iNum].lrssi, sizeof(pStCurrentForty[iNum].lrssi)-1, "%5d", psMacLoc->lrssi);
		//snprintf(pStCurrentForty[iNum].urssi, sizeof(pStCurrentForty[iNum].urssi)-1, "%5d", psMacLoc->urssi);					
		
		snprintf(pStCurrentForty[iNum].caMac, sizeof(pStCurrentForty[iNum].caMac)-1, "%s", (const char*)pasCvtMacStr(psMacLoc->mac,caMacStr));
		snprintf(pStCurrentForty[iNum].caLongitude, sizeof(pStCurrentForty[iNum].caLongitude)-1, "%s", psMacLoc->longitude);		
		snprintf(pStCurrentForty[iNum].caLatitude, sizeof(pStCurrentForty[iNum].caLatitude)-1, "%s", psMacLoc->latitude);
		snprintf(pStCurrentForty[iNum].caSsid, sizeof(pStCurrentForty[iNum].caSsid)-1, "%s", psMacLoc->ssid);
		snprintf(pStCurrentForty[iNum].caApmac, sizeof(pStCurrentForty[iNum].caApmac)-1, "%s", (const char*)pasCvtMacStr(psMacLoc->apmac,caMacStr1));
		
		pStCurrentForty[iNum].encrypt = psMacLoc->encrypt;
		//snprintf(pStCurrentForty[iNum].encrypt, sizeof(pStCurrentForty[iNum].encrypt)-1, "%d", psMacLoc->encrypt);	
		
		snprintf(pStCurrentForty[iNum].caServiceCode, sizeof(pStCurrentForty[iNum].caServiceCode)-1, "%s", psAp->servicecode);
		snprintf(pStCurrentForty[iNum].caApname, sizeof(pStCurrentForty[iNum].caApname)-1, "%s", psAp->apname);
		snprintf(pStCurrentForty[iNum].caVname, sizeof(pStCurrentForty[iNum].caVname)-1, "%s", psMacLoc->name);
		snprintf(pStCurrentForty[iNum].caTermtype, sizeof(pStCurrentForty[iNum].caTermtype)-1, "%s", psMacLoc->termtype);	
	}
	else
	{
		return -1;
	}		
	
	return 0;
} 

//ncsCurrent_FromRAM  实时信息中依据时间排序
static void choiseSort(stCurInfoForty *pstrCur, int iLen)
{
	int i,j,k; 
	stCurInfoForty stTemp;
	
	
	for(i=0; i<iLen-1; i++) 
	{ 
		k=i; //给记号赋值
		for(j=i+1; j<iLen; j++)
		{
			if(pstrCur[k].caStime > pstrCur[j].caStime) 
				k=j; //是k总是指向最小元素

			if(i!=k) 
			{ 
				//当k!=i是才交换，否则a[i]即为最小
				memset(&stTemp, 0, sizeof(stTemp));
				memcpy(&stTemp, &pstrCur[i], sizeof(stCurInfoForty));
				memcpy(&pstrCur[i], &pstrCur[k], sizeof(stCurInfoForty));
				memcpy(&pstrCur[k], &stTemp, sizeof(stCurInfoForty));

			}	
		} 
	} 
}

//ncsCurrent_FromRAM  输出内容到缓冲区
static int outPutJson_current(utPltDbHead *psDbHead, stCurInfoForty* pStCurrentForty, int i)
{
	if(psDbHead)
	{
		/* 输入缓冲区 */
		utPltPutLoopVarF(psDbHead, "servicename", i, "%s", pStCurrentForty[i].caServicename);
		utPltPutLoopVarF(psDbHead, "dispname", i, "%s", pStCurrentForty[i].caDispname);	
		utPltPutLoopVarF(psDbHead, "stime", i, "%s", utTimFormat("%Y-%m-%d %H:%M:%S", pStCurrentForty[i].caStime));
		//printf("outPutJson_current pStCurrentForty[%d].caStime = %lu\n\n", i, pStCurrentForty[i].caStime);
		//printf("outPutJson_current pStCurrentForty[%d].caStime) = %s\n\n", i, utTimFormat("%Y-%m-%d %H:%M:%S", pStCurrentForty[i].caStime));
		
		utPltPutLoopVarF(psDbHead, "lssi", i, "%5d", pStCurrentForty[i].lrssi);
		utPltPutLoopVarF(psDbHead, "ussi", i, "%5d", pStCurrentForty[i].urssi);							
		utPltPutLoopVarF(psDbHead, "mac", i, "%s", pStCurrentForty[i].caMac);		
		utPltPutLoopVarF(psDbHead, "longitude", i, "%s", pStCurrentForty[i].caLongitude);
		utPltPutLoopVarF(psDbHead, "latitude", i, "%s", pStCurrentForty[i].caLatitude);
		utPltPutLoopVarF(psDbHead, "ssid", i, "%s", pStCurrentForty[i].caSsid);	
		utPltPutLoopVarF(psDbHead, "apmac", i, "%s", pStCurrentForty[i].caApmac);
		utPltPutLoopVarF(psDbHead, "security", i, "%d", pStCurrentForty[i].encrypt);
		utPltPutLoopVarF(psDbHead, "servicecode", i, "%s", pStCurrentForty[i].caServiceCode);
		utPltPutLoopVarF(psDbHead, "apname", i, "%s", pStCurrentForty[i].caApname);
		utPltPutLoopVarF(psDbHead, "vname", i, "%s", pStCurrentForty[i].caVname);
		utPltPutLoopVarF(psDbHead, "termtype", i, "%s", pStCurrentForty[i].caTermtype);	
		
		return 0;
	}		
		
	return -1;
}

//通过 GroupId 获取其所属的 groupid
static char *myGetGroupCodeByGroupid(utShmHead *psShmHead,char *caGroupid){
        static char caReturn[8024];
        char caServicecode[32];
        char caGroupcode[30];
        char caIds[1024];
        char caTempid[1024],caTempcode[1024];
        int iReturn,lId,iNum;
         pasDbCursor *psCur;
         memset(caReturn,0,sizeof(caReturn));
              
              iNum=0;
    
         psCur = pasDbOpenSqlF("select distinct groupcode from ncsgroup where   groupid in(%s) ",caGroupid);
	 	     if(psCur)
			 {
	 	     	iReturn=0;
	 	     	lId=0;
	 	     	memset(caGroupcode,0,sizeof(caGroupcode));
	 	     	iReturn=pasDbFetchInto(psCur,
	 	     	                            UT_TYPE_STRING,14,caGroupcode);
	 	     	 iNum=0;
	 	     	while(iReturn==0||iReturn==1405)
				{
	 	     		if(iNum==0)
					{	 	     			
	 	     			sprintf(caReturn,"\"%s\"",caGroupcode);
	 	     		}
	 	     		else
					{ 	     		
	 	     			sprintf(caReturn+strlen(caReturn),",\"%s\"",caGroupcode);					
	 	     		}
	 	     		iNum++;
	 	     		
	 	     			memset(caGroupcode,0,sizeof(caGroupcode));
	 	     	   iReturn=pasDbFetchInto(psCur,
	 	     	                            UT_TYPE_STRING,14,caGroupcode);
	 	     	}
	 	     	 pasDbCloseCursor(psCur);  
	 	     }
			printf("*************************************************************\n");	 
			printf("caReturn = %s\n\n", caReturn);	
	
	 	//  	 sprintf(caReturn,"%s",caOpt);
	 	  	 sprintf(caIds,"%s",caGroupid);
			iNum=0;
	 	  while(strlen(caIds)>0)
		  {
	 	  	memset(caTempid,0,sizeof(caTempid));
	 	  	iNum=0; 
			
			//printf("caIds = %s\n\n", caIds);
			
	 	     psCur = pasDbOpenSqlF("select groupid,groupcode from ncsgroup where   pid in(%s) ",caIds);
	 	     if(psCur)
			 {
	 	     	iReturn=0;
	 	     	lId=0;
	 	     	memset(caGroupcode,0,sizeof(caGroupcode));
	 	     	iReturn=pasDbFetchInto(psCur,UT_TYPE_ULONG,4,&lId,
	 	     	                            UT_TYPE_STRING,14,caGroupcode);
	 	     	while(iReturn==0||iReturn==1405)
				{
	 	     		if(iNum==100) break;
	 	     		if(iNum==0)
					{
	 	     			sprintf(caTempid,"%lu",lId);
	 	     			sprintf(caTempcode,"\"%s\"",caGroupcode);
	 	     		}
	 	     		else
					{
	 	     			sprintf(caTempid+strlen(caTempid),",%lu",lId);
	 	     			sprintf(caTempcode+strlen(caTempcode),",\"%s\"",caGroupcode);
	 	     		}
	 	     			
	 	     	memset(caGroupcode,0,sizeof(caGroupcode));
	 	     	iReturn=pasDbFetchInto(psCur,UT_TYPE_ULONG,4,&lId,
	 	     	                            UT_TYPE_STRING,14,caGroupcode);
	 	      			
	 	     			iNum++;
	 	     		
	 	     	}
					
	 	     }
	 	  	   pasDbCloseCursor(psCur);               
	  		
		 	  	strcpy(caIds,caTempid);
				
				//printf("caTempcode = %s\n\n", caTempcode);
				if(strlen(caTempcode)>0)
				{
		 	  		sprintf(caReturn+strlen(caReturn),",%s",caTempcode);
		 	   }
	 	  	}
	 	
	 	  	 //if(strlen(caTempcode)>0){
		 	 // 		sprintf(caReturn+strlen(caReturn),",%s",caTempcode);
		 	  // }
	 	 
		  printf("caReturn = %s\n\n", caReturn);
		  printf("*************************************************************\n");	 
	 	  return &caReturn[0];              


                
}

	
	
int timeToStringPlace(unsigned int now, char *year, char *mon);
int doCalcByDaysPlace(int days, char *szServicecode, utMsgHead *psMsgHead, int iFd, utPltDbHead *psDbHead);
int doCalcByMonthsPlace(int months, char *szServicecode, utMsgHead *psMsgHead, int iFd, utPltDbHead *psDbHead);
long getMonthFirstDayPlace(long lttime);
long getCountInDurationPlace(long lstime, long letime,  char *szServicecode);


static char* MyreplaceAll(char* src, char oldChar, char newChar)
{
    char * head = src;
    while(*src != '\0')
    {
        if(*src == oldChar)
            *src = newChar;
        src++;
    }
    return head;
}

static void MyremoveAll(char* src, char ch)
{
    int i, j;
    for(i = 0; src[i] != '\0'; i++)
    {
        if(src[i] == ch)
        {
            for(j = i; src[j] != '\0'; j++)
                src[j] = src[j + 1];
        }
    }
}


/* 获取当前用户选择场所的 caSelectGroups ，在其权限范围内的 caAuthorityGroups 所有的 outbuf(servicecodes) */
static int selectGroupsIsInAuthorityGroups(char *caSelectGroups, char *caAuthorityGroups, char* outbuf, int buflen)
{	
	if (!strlen(outbuf))
	{
		memcpy(outbuf, "当前用户无权限查看所选择区域", strlen("当前用户无权限查看所选择区域"));
		return -1;
	}
	
	//获取有权限所有场所 servicecode
	int len=strlen(caAuthorityGroups)+1;
	char* pos1=caAuthorityGroups;
	char* pos2 = NULL;
	int   len1 = 0, len2 = 0; 
	char  idbuf[24] = "";
	memset(outbuf,0,buflen);

	pos2=strchr(pos1,',');					//获取第一个','的位置
	while(pos2!=NULL)
	{	
		len1=pos2-pos1;						//获取第一个','之前字符串长度
		if(len1>20)
			break;
		
		len2=strlen(outbuf);				//定位outbuf输出起点
		if(len2>(len-20))
			break;
		
		memset(idbuf,0,sizeof(idbuf));
		memcpy(idbuf,pos1,len1);			//拷贝当前 pos1 ',' 号之前
		if(strstr(caSelectGroups, idbuf)!=NULL)	//比对当前 pos1 在 caSelectGroups 中出现的位置
		{
			if(outbuf[0])
			{
				sprintf(outbuf+strlen(outbuf),",");
				len2=strlen(outbuf);
				memcpy(outbuf+len2,idbuf,strlen(idbuf));
			}
			else
			{
				memcpy(outbuf+len2,idbuf,strlen(idbuf));
			}
		}
		pos1=pos2+1;
		pos2=strchr(pos1,',');				
	}
	if(strlen(pos1)==16)
	{
		memset(idbuf,0,sizeof(idbuf));
		memcpy(idbuf,pos1,16);
		if(strstr(caSelectGroups, idbuf)!=NULL)
		{			
			sprintf(outbuf+strlen(outbuf),",");
			len2=strlen(outbuf);
			memcpy(outbuf+len2,idbuf,strlen(idbuf));
		}
	}
	if (!strlen(outbuf))
	{
		memcpy(outbuf, "当前用户无权限查看所选择区域", strlen("当前用户无权限查看所选择区域"));
		return -1;
	}

	return 0;
}


static char* getGroupidFrom(char* caGroups, long lGroupid)
{
	//isInList
	//caGroups 
	
}

static char* getGroupidFromUsername(char* caGroups, char* username)
{
	//ncsuser表 groupid
	//caGroups 
	
}


/* MAC日志查询：显示终端特征信息日志 */
int ncsMacLog_mactermlist_v4(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
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
	char caTotPg[16] = "", caCurPg[16] = "";
	char caPage[12] = "", caLimit[16] = "", caSort[16] = "", caDir[16] = "";
	char caKeyword[64] = "", caMacKeyword[64] = "", caTime_flag[32] = "";
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
	char caGroups[1024] = "";
	char caFacs[1024] = "";
	char caArea[256] = "", caName2[256] = "";
    char caTemp[1024] = "";
	long iReturn;
	unsigned long lIp;
    char caStime[16] = "";
   	char caEtime[16] = "";
   	char caTemp_d[64] = "";
   	char caGroupid[36] = "", caDid[36] = "";
    char caSdate[24] = "", caEdate[24] = "";
    char caApmac[36] = "", caMac[36] = "", caApname[64] = "", caPosition[64] = "";
    char caCorpname[128] = "";
    char caCont[2048] = "";
    char caServicecode[16] = "", caSearchtype[32] = "";
	
	/* 用户点击左侧菜单树中的场所名称 */
	char caUserName[64] = "";
	
	/* 高级查询搜索字段 */
	char caSname[32] = "";	
	char caSsid[36] = "";
	
	/* 手机号搜索 */
	char caVname[64] = "";
	
	/* 导出 */
	char caExport[10] = "";	

	memset(caKeyword, 0, sizeof(caKeyword));
	
	strcpy(caFacs, (const char*)getDsFacDid());
	strcpy(caGroups, (const char*)getDsGroupid());  
	//printf("*********************************************************************");
	//printf("caGroups = %s\n\n", caGroups);
	//printf("*********************************************************************");
   
	utPltDbHead *psDbHead = utPltInitDb();
	utMsgPrintMsg(psMsgHead);
	iReturn = utMsgGetSomeNVar(psMsgHead,24,
			"TotPg",      UT_TYPE_STRING, 12, caTotPg,
			"CurPg",      UT_TYPE_STRING, 12, caCurPg,
			"keyword", UT_TYPE_STRING,60,caKeyword,
			"page",            UT_TYPE_STRING,10,caPage,
			"limit",           UT_TYPE_STRING,10,caLimit,
			"sort",            UT_TYPE_STRING, 15, caSort,
			"dir",          UT_TYPE_STRING, 8,  caDir,
			"time_flag",    UT_TYPE_STRING,8,caTime_flag,
			"did",          UT_TYPE_STRING,10,caDid,
			"groupid",      UT_TYPE_STRING,10,caGroupid,
			"username",      UT_TYPE_STRING,63,caUserName,
			"sdate",        UT_TYPE_STRING,12,sdate,
			"stime",        UT_TYPE_STRING,10,caStime,
			"edate",        UT_TYPE_STRING,12,edate,
			"etime",        UT_TYPE_STRING,10,caEtime,
			"apmac",        UT_TYPE_STRING,20,caApmac,
			"mac",          UT_TYPE_STRING,20,caMac,			
			"macLogExport",      UT_TYPE_STRING,9,caExport,							
			"searchtype",      UT_TYPE_STRING,10,caSearchtype,
			"servicecode",     UT_TYPE_STRING,15,caServicecode,			
			"servicename",     UT_TYPE_STRING,60,caCorpname,	
			"sname",      	   UT_TYPE_STRING,31,caSname,
			"ssid",            UT_TYPE_STRING,30,caSsid,
			"apname",       UT_TYPE_STRING,31,caApname,	
			"vname",      UT_TYPE_STRING,63,caVname);
	
	memcpy(caMacKeyword, caKeyword, sizeof(caKeyword));
	MyremoveAll(caMacKeyword, '-');
   	MyremoveAll(caMacKeyword, ':');
	MyremoveAll(caMac,'-');
	MyremoveAll(caMac,':');
	MyremoveAll(caApmac, '-');
	MyremoveAll(caApmac, ':');
	MyremoveAll(caKeyword, '-');
	MyremoveAll(caKeyword, ':');
	
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
    if(lCurPg <= 0) 
		lCurPg = 1;
	
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
	
	if(strlen(sdate) > 0 && strlen(edate) > 0)
	{
		ptmp = ncsUtlGetTable3(sdate, edate,caStime,caEtime, "ncmactermatt_if_", &lStartTime, &lTime, &lCount);
		printf("lStartTime=%d,sdate=%s\n",lStartTime,sdate);
	}
	else
	{
		ptmp = ncsUtlGetTable(lTime, days, "ncmactermatt_if_",  &lStartTime, &lCount);
		lTime=lTime+2*3600;
	}

	memset(caCont,0,sizeof(caCont));
	

	char caGroupidJudge[30] = ""; //判断输入框内容有无@ 字符
	if(strlen(caSearchtype)>0)
	{		
		if(strlen(caMac)>0)
		{
			sprintf(caCont+strlen(caCont)," and replace(a.mac,'-','') like '%c%s%c' ",'%',caMac,'%');
		}
		if(strlen(caApmac)>0)
		{
			sprintf(caCont+strlen(caCont)," and replace(a.apmac, '-','') like '%c%s%c' ",'%',caApmac,'%');
		}	
		
		/* 高级查询搜索字段 */		
		if(strlen(caServicecode)>0)	//场所编号 	
		{
			sprintf(caCont+strlen(caCont)," and a.servicecode like '%c%s%c' ",'%',caServicecode,'%');
		}
		if(strlen(caCorpname)>0)	//场所名称
		{
			sprintf(caCont+strlen(caCont)," and a.servicename like '%c%s%c' ",'%',caCorpname,'%');
		}
		
		if(strlen(caSname)>0)		//安全厂商名称查询	
		{
			//sprintf(caCont+strlen(caCont)," and servicename like '%c%s%c' ",'%',caSname,'%');
			printf(" 安全厂商名称为 %s \n\n", caSname);
		}		
		if(strlen(caSsid)>0)		//接入热点ssid
		{
			sprintf(caCont+strlen(caCont)," and a.ssid like '%c%s%c' ",'%',caSsid,'%');
		}
		if(strlen(caApname)>0)		//采集设备编号
		{
			sprintf(caCont+strlen(caCont)," and a.apname like '%c%s%c' ",'%',caApname,'%');
		}
		if(strlen(caVname)>0)		//上网认证账号
		{			
			printf(" 上网认证账号 %s \n\n", caVname);
		}		
	}
	else
	{
		if(strlen(caKeyword)>0)
		{
			/*char caTempKeyword[64] = "";
			caTempKeyword = convmac(caKeyword);
			printf("caTempKeyword = %s, caKeyword = %s\n", caTempKeyword, caKeyword);
			*/
			
			/* 字符串中小写转大写 */
			int i = 0, j = 0;
			j = strlen(caKeyword);
			
			for(i = 0; i <= j; i++)
			{
				if(caKeyword[i]>='a' && caKeyword[i]<='z')		
					caKeyword[i] = caKeyword[i] - 32;
			}
			//Keyword[i] = '\0';						
			//printf("After strToUpper: caKeyword = %s \n", caKeyword);

			if(strcmp(caGroupid, "all") == 0)
			{
				printf("查询所有场所\n\n");
			}
			else
			{
				/* 如果caKeyword第一个字符为 '@' */
				strncpy(caGroupidJudge, caKeyword, 1);
				if(strcmp(caGroupidJudge, "@") == 0)
				{
					if(strlen(caGroupid)>0)	
					{						
						sprintf(caCont+strlen(caCont)," and %s", getGroupcodeSql("servicecode", caGroupid));				
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
					printf("pstrKeywordServiceName, pstrKeywordMac = %s\n", pstrKeywordServiceName, pstrKeywordMac);

					/* @  场所@  后面如果输入了mac/apmac */
					if(strcmp(pstrKeywordMac, "") != 0)
						sprintf(caCont+strlen(caCont)," and (replace(a.mac,'-','') like '%c%s%c' or replace(a.apmac,'-','') like '%c%s%c') ",'%',pstrKeywordMac,'%','%',pstrKeywordMac,'%');
						//sprintf(caCont+strlen(caCont)," and (a.mac like '%c%s%c' or a.apmac	like '%c%s%c') ",'%',pstrKeywordMac,'%','%',pstrKeywordMac,'%');
				
				}
				else
				{
					sprintf(caCont," and (replace(a.mac,'-','') like '%c%s%c' or replace(a.apmac,'-','') like '%c%s%c') ",'%',caKeyword,'%','%',caKeyword,'%');			
				
					//sprintf(caCont+strlen(caCont)," and (a.mac like '%c%s%c' or a.apmac	like '%c%s%c') ",'%',pstrKeywordMac,'%','%',pstrKeywordMac,'%');								
				}	
			}						
			
			printf("caCont = %s\n", caCont);		
		}

	}
  	  
	if(strlen(caVname)>0)
	{
		sprintf(caCont+strlen(caCont)," and a.vname like '%c%s%c' ",'%',caVname,'%');
	}
	if(strlen(caDid)>0)
	{

		//sprintf(caCont+strlen(caCont)," and a.servicecode='%s' ",getServicecodeByid(psShmHead,atol(caDid)));
	}
 	
	/*  当前用户权限下所能查询场所*/
	if(strlen(caGroups)>0)
	{
		sprintf(caCont+strlen(caCont)," and %s ", getDsGroupcodeSql("servicecode"));
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
		printf("sql3 = %s\n", sql3);
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
	printf(" lCount2 = %lu  num2 = %d  lTotRec2 = %lu\n\n", lCount2, num2, lTotRec2);
	utPltPutVarF(psDbHead,"distinctCount", "%lu", lTotRec2);
	

	for(i = 0; i < lCount; i++)
	{
		lCount1 = 0;
		memset(sql, 0, sizeof(sql));
		snprintf(sql, sizeof(sql), "select count(*) from %s a where a.stime >= %lu and a.stime <= %lu  ",  ptmp[i], lStartTime, lTime);
		strcat(sql,caCont);

		printf("i=%d, sql = %s\n", i, sql);
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
	utPltPutVarF(psDbHead,"TotRec", "%lu", lTotRec);
		
	for(i = num-1; i >= 0; i--)
	{
		printf("table[%d] = %s, _lCount1[%d] = %lu\n", i, tableinfo[i].tname, i, tableinfo[i].lCount);
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
		sprintf(caFilename,"maclog.csv");
		sprintf(caPath,"%s", "/home/ncmysql/ncs");
		sprintf(caFile,"%s/%s",caPath,caFilename);
		fp = fopen(caFile,"w");	
		if(fp == NULL) 
		{				
			printf("数据导出出错\n\n");
			return 0;
		}
		fprintf(fp, "mac日志查询\n");	//标题
		printf(" 写入mac日志查询\n\n");			
		ncUtlPrintCsv(fp, 18,
				UT_TYPE_STRING, "终端MAC地址",
                UT_TYPE_STRING, "上网认证账号",
                UT_TYPE_STRING, "采集时间",
                UT_TYPE_STRING, "终端场强",
                UT_TYPE_STRING, "场所名称",
				UT_TYPE_STRING, "采集设备名称",
                UT_TYPE_STRING, "场所编号",
                UT_TYPE_STRING, "采集设备编号",
                UT_TYPE_STRING, "采集设备经度",
                UT_TYPE_STRING, "采集设备纬度",
				UT_TYPE_STRING, "终端品牌",
                UT_TYPE_STRING, "终端历史SSID列表",
                UT_TYPE_STRING, "相对采集设备X坐标",
                UT_TYPE_STRING, "相对采集设备Y坐标",
				UT_TYPE_STRING, "接入热点SSID",
                UT_TYPE_STRING, "接入热点MAC",
                UT_TYPE_STRING, "接入热点频道",
                UT_TYPE_STRING, "接入热点加密类型"); 
	}
	printf(" num = %d\n\n", num);
	
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
		/* 加上采集设备名称 */
		//snprintf(sql, sizeof(sql), "select a.servicecode,a.servicename,a.address,a.mac,a.apname,a.apmac,a.ssid,a.stime,a.channel,a.rssi,a.security,a.longitude,a.latitude,a.termtype,a.vtype,a.vname, b.dispname from %s a, ncaplist b where a.stime >= %lu and a.stime <= %lu and b.apname = RIGHT(a.apname, 12) ", tableinfo[i].tname, lStartTime, lTime);
		
		/* 去掉采集设备名称关联查询 */
		snprintf(sql, sizeof(sql), "select a.servicecode,a.servicename,a.address,a.mac,a.apname,a.apmac,a.ssid,a.stime,a.channel,a.rssi,a.security,a.longitude,a.latitude,a.termtype,a.vtype,a.vname from %s a where a.stime >= %lu and a.stime <= %lu ", tableinfo[i].tname, lStartTime, lTime);
		strcat(sql,caCont);
			
		//printf("\n\n sql = %s \n\n caCont = %s\n\n", sql, caCont);
		
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
	
		printf("__line = %d, sql = %s\n", __LINE__, sql);
		psCur = pasDbOpenSql(sql, 0);		
		
		if(psCur != NULL)
		{
			char caServicecode[16],caServicename[64],caAddress[64],caMac[20],caApname[32],caApmac[20],caSsid[64];
			long lStarttime,lEndtime,lFlags;
			char caChannel[32],caRssi[16],caSecurity[16],caLongitude[16],caLatitude[16];
			char caTermtype[128],caVname[64];
			long lVtype;
			
			/* 采集设备名称 */
			char caDispname[32] = "";
			
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
				 UT_TYPE_STRING, 31, caDispname);			   
					
			while((iret == 0) || (iret == 1405))
			{
				/* 选择导出的话 */
				if(strlen(caExport) > 0)
				{
					ncUtlPrintCsv(fp, 18,
                      UT_TYPE_STRING, caMac,
                      UT_TYPE_STRING, caVname,
                      UT_TYPE_LONG, lStarttime,
                      UT_TYPE_STRING, caRssi,
                      UT_TYPE_STRING, caServicename,
					  UT_TYPE_STRING, caDispname,
                      UT_TYPE_STRING, caServicecode,
                      UT_TYPE_STRING, caApname,
                      UT_TYPE_STRING, caLongitude,
                      UT_TYPE_STRING, caLatitude,
					  UT_TYPE_STRING, caTermtype,
                      UT_TYPE_STRING, "终端历史ssid列表",
                      UT_TYPE_STRING, "X坐标",
                      UT_TYPE_STRING, "Y坐标",
					  UT_TYPE_STRING, caSsid,
                      UT_TYPE_STRING, caApmac,
                      UT_TYPE_STRING, caChannel,
                      UT_TYPE_STRING, caSecurity); 
				}
				else
				{
					iNum++;
					if(iNum > 1)
					{
						utPltPutLoopVar(psDbHead,"dh",iNum,",");
					}

					utPltPutLoopVar(psDbHead,"stime",iNum,utTimFormat("%Y-%m-%d %H:%M:%S",lStarttime));
      
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
					utPltPutLoopVarF(psDbHead,"dispname",iNum,"%s",caDispname);
				}	
				
				memset(caServicecode,0,sizeof(caServicecode));
				memset(caServicename,0,sizeof(caServicename));
				memset(caAddress,0,sizeof(caAddress));
				memset(caMac,0,sizeof(caMac));
				memset(caApname,0,sizeof(caApname));
				memset(caApmac,0,sizeof(caApmac));
				memset(caSsid,0,sizeof(caSsid));
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
				memset(caDispname,0,sizeof(caDispname));
				
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
						UT_TYPE_STRING, 31, caDispname);				
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
	return 0;	   
}

/* MAC日志查询下特征分析1：频繁出现的时间 */
int ncsMacLog_ChosenMac_TimeOftenCount(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
	/* 初始化模板数据缓冲区 */
	utPltDbHead *psDbHead = utPltInitDb();
	
	/* 将数据包显示出来,本函数主要用于调试。
	显示时先显示数据包的头部，然后显示数据包的内容。*/
	utMsgPrintMsg(psMsgHead);
	
	/* 场所编号、查询时间类型(天、周、月、三月、季度、年) */	
	char caTimeFlag[10] = "";
	char caMac[32] = "";
	int iReturn = 0;
	/* 从消息数据包 psMsgHead 中提取指定数量的变量 */
	iReturn = utMsgGetSomeNVar(psMsgHead, 2, 		
		"timevalue", UT_TYPE_STRING, sizeof(caTimeFlag) - 1, caTimeFlag,
		"mac", UT_TYPE_STRING, sizeof(caMac) - 1, caMac);
		
	MyremoveAll(caMac, '-');
	MyremoveAll(caMac, ':');
	
	
	printf("timevalue = %s\n", caTimeFlag);	
	printf("mac = %s\n", caMac);
			
	//1.根据当前年月生成表名
    char table_name[1024] = "";
    char  year[10];
    char  mon[10];
    memset(year, 0, sizeof(year));
    memset(mon, 0 , sizeof(mon));
    time_t now;
    time(&now);
    timeToStringPlace(now, year, mon);
    int syear = atoi(year);
    int smonth = atoi(mon);		
    snprintf(table_name + strlen(table_name), 1024 - strlen(table_name), "ncmactermatt_if_%4u%02u", syear, smonth);
			
	//2.根据查询时间类型选定相应时间条件
	char caStartDate[32] = "";
   	char caEndDate[32] = "";
	
	char sql[4096] = "";	
	char sqlTmp[4096] = "";
	snprintf(sql, sizeof(sql), "select mac, count(*) AS '出现次数',HOUR(FROM_UNIXTIME(d.stime)) as hourtime  from %s d where d.stime >= ", table_name);
	
	if(strcmp(caTimeFlag, "null") ==0 )		//查询今天：默认
	{
		snprintf(sqlTmp, sizeof(sqlTmp), " UNIX_TIMESTAMP(DATE_SUB(NOW(), INTERVAL 1 DAY)) ");
	}
	else if(strcmp(caTimeFlag, "1") ==0)	//查询今天：用户选择
	{
		snprintf(sqlTmp, sizeof(sqlTmp), " UNIX_TIMESTAMP(DATE_SUB(NOW(), INTERVAL 1 DAY)) ");
	}
	else if(strcmp(caTimeFlag, "2") ==0 )	//最近三天
	{
		snprintf(sqlTmp, sizeof(sqlTmp), " UNIX_TIMESTAMP(DATE_SUB(NOW(), INTERVAL 3 DAY)) ");
	}
	else if(strcmp(caTimeFlag, "3") ==0 )	//最近一周
	{
		snprintf(sqlTmp, sizeof(sqlTmp), " UNIX_TIMESTAMP(DATE_SUB(NOW(), INTERVAL 7 DAY)) ");
	}
	else if(strcmp(caTimeFlag, "4") ==0 )	//最近一月
	{
		snprintf(sqlTmp, sizeof(sqlTmp), " UNIX_TIMESTAMP(DATE_SUB(NOW(), INTERVAL 1 MONTH)) ");
	}
	else if(strcmp(caTimeFlag, "5") ==0 )	//最近三月
	{
		snprintf(sqlTmp, sizeof(sqlTmp), " UNIX_TIMESTAMP(DATE_SUB(NOW(), INTERVAL 3 MONTH)) ");
	}
	else if(strcmp(caTimeFlag, "6") ==0 )	//最近半年
	{
		snprintf(sqlTmp, sizeof(sqlTmp), " UNIX_TIMESTAMP(DATE_SUB(NOW(), INTERVAL 6 MONTH)) ");
	}
	else 	//最近半年
	{
		snprintf(sqlTmp, sizeof(sqlTmp), " UNIX_TIMESTAMP(DATE_SUB(NOW(), INTERVAL 6 MONTH)) ");
	}
	
	//printf(sqlTmp);
	printf("\n");
	
	/* 在原来的sql语句有效字符结尾处，追加上 sqlTmp */
	snprintf(sql + strlen(sql), sizeof(sql) - strlen(sql), "%s", sqlTmp);
	snprintf(sql + strlen(sql), sizeof(sql) - strlen(sql), " and d.stime <= (UNIX_TIMESTAMP(NOW())) and replace(d.mac,'-','') like '%c%s%c' GROUP BY hourtime ORDER BY count(*) desc limit 10 ", '%', caMac, '%');
	
	printf("\n");
	printf("\n");
	printf("%s", sql);
	printf("\n");
	
	int iNum = 0, iRet = 0;
	unsigned long dwCountMax = 0;
	pasDbCursor *psCur = NULL;
	psCur = pasDbOpenSql(sql, 0);
	if(psCur != NULL)
	{
		printf("\n psCur is not null\n\n");
		char caMac[36] = "";		
		unsigned long dwCount = 0;		
		char caHour[36] = "";
		
		int nRet = 0;
		while(0 == (nRet = pasDbFetchInto(psCur, UT_TYPE_STRING, 32, &caMac,
												 UT_TYPE_LONG, 	20, &dwCount,
												 UT_TYPE_STRING, 32, &caHour)) || 1405 == nRet)
		{
			iNum++;
			if(iNum > 1)
			{
				utPltPutLoopVar(psDbHead, "dh", iNum, ",");
			}
			utPltPutLoopVarF(psDbHead, "mac", iNum, "%s", caMac);
			utPltPutLoopVarF(psDbHead, "count", iNum, "%lu", dwCount);
			utPltPutLoopVarF(psDbHead, "hour", iNum, "%s", caHour);
			
			if(dwCount >= dwCountMax)
			{
				dwCountMax = dwCount;				
			}	
			
			//清空容器
			memset(caMac, 0, sizeof(caMac));
			memset(caHour, 0, sizeof(caHour));
			dwCount = 0;
		}
	}
	
	pasDbCloseCursor(psCur);
	utPltPutVarF(psDbHead, "result", "%d\n", iNum);
	utPltPutVarF(psDbHead, "max", "%lu\n", dwCountMax);
	printf("final iNum = %d\n", iNum);
	utPltOutToHtml(iFd, psMsgHead, psDbHead, "mac/infoquery/macterm/ChosenMacTimeOftenCount.htm");	
	return 0;
}

/* MAC日志查询下特征分析2：频繁出现的场所 */
int ncsMacLog_ChosenMac_PlaceOftenCount(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{			
	/* 初始化模板数据缓冲区 */
	utPltDbHead *psDbHead = utPltInitDb();
	
	/* 将数据包显示出来,本函数主要用于调试。
	显示时先显示数据包的头部，然后显示数据包的内容。*/
	utMsgPrintMsg(psMsgHead);
	
	/* 场所编号、查询时间类型(天、周、月、三月、季度、年) */	
	char caTimeFlag[10] = "";
	char caMac[32] = "";
	int iReturn = 0;
	/* 从消息数据包 psMsgHead 中提取指定数量的变量 */
	iReturn = utMsgGetSomeNVar(psMsgHead, 2, 		
		"timevalue", UT_TYPE_STRING, sizeof(caTimeFlag) - 1, caTimeFlag,
		"mac", UT_TYPE_STRING, sizeof(caMac) - 1, caMac);
		
	MyremoveAll(caMac, '-');
	MyremoveAll(caMac, ':');
	
	printf("\n");	
	printf("timevalue = %s\n", caTimeFlag);	
	printf("mac = %s\n", caMac);
			
	//1.根据当前年月生成表名
    char table_name[1024] = "";
    char  year[10];
    char  mon[10];
    memset(year, 0, sizeof(year));
    memset(mon, 0 , sizeof(mon));
    time_t now;
    time(&now);
    timeToStringPlace(now, year, mon);
    int syear = atoi(year);
    int smonth = atoi(mon);		
    snprintf(table_name + strlen(table_name), 1024 - strlen(table_name), "ncmactermatt_if_%4u%02u", syear, smonth);
			
	//2.根据查询时间类型选定相应时间条件
	char caStartDate[32] = "";
   	char caEndDate[32] = "";
	
	char sql[4096] = "";	
	char sqlTmp[4096] = "";
	snprintf(sql, sizeof(sql), "select servicename, servicecode, count(stime) from %s where stime >= ", table_name);
	
	if(strcmp(caTimeFlag, "null") ==0 )		//查询今天：默认
	{
		snprintf(sqlTmp, sizeof(sqlTmp), " UNIX_TIMESTAMP(DATE_SUB(NOW(), INTERVAL 1 DAY)) ");
	}
	else if(strcmp(caTimeFlag, "1") ==0)	//查询今天：用户选择
	{
		snprintf(sqlTmp, sizeof(sqlTmp), " UNIX_TIMESTAMP(DATE_SUB(NOW(), INTERVAL 1 DAY)) ");
	}
	else if(strcmp(caTimeFlag, "2") ==0 )	//最近三天
	{
		snprintf(sqlTmp, sizeof(sqlTmp), " UNIX_TIMESTAMP(DATE_SUB(NOW(), INTERVAL 3 DAY)) ");
	}
	else if(strcmp(caTimeFlag, "3") ==0 )	//最近一周
	{
		snprintf(sqlTmp, sizeof(sqlTmp), " UNIX_TIMESTAMP(DATE_SUB(NOW(), INTERVAL 7 DAY)) ");
	}
	else if(strcmp(caTimeFlag, "4") ==0 )	//最近一月
	{
		snprintf(sqlTmp, sizeof(sqlTmp), " UNIX_TIMESTAMP(DATE_SUB(NOW(), INTERVAL 1 MONTH)) ");
	}
	else if(strcmp(caTimeFlag, "5") ==0 )	//最近三月
	{
		snprintf(sqlTmp, sizeof(sqlTmp), " UNIX_TIMESTAMP(DATE_SUB(NOW(), INTERVAL 3 MONTH)) ");
	}
	else if(strcmp(caTimeFlag, "6") ==0 )	//最近半年
	{
		snprintf(sqlTmp, sizeof(sqlTmp), " UNIX_TIMESTAMP(DATE_SUB(NOW(), INTERVAL 6 MONTH)) ");
	}
	else 	//最近半年
	{
		snprintf(sqlTmp, sizeof(sqlTmp), " UNIX_TIMESTAMP(DATE_SUB(NOW(), INTERVAL 6 MONTH)) ");
	}
	
	//printf(sqlTmp);
	printf("\n");
	
	/* 在原来的sql语句有效字符结尾处，追加上 sqlTmp */
	snprintf(sql + strlen(sql), sizeof(sql) - strlen(sql), "%s", sqlTmp);
	snprintf(sql + strlen(sql), sizeof(sql) - strlen(sql), " and stime <= (UNIX_TIMESTAMP(NOW())) and servicename != '' and replace(mac,'-','') like '%c%s%c' GROUP BY servicecode ORDER BY 3 desc limit 10 ", '%', caMac, '%');
	
	printf("\n");
	printf("\n");
	printf("%s", sql);
	printf("\n");
	
	int iNum = 0, iRet = 0;
	unsigned long dwCountMax = 0;
	pasDbCursor *psCur = NULL;
	psCur = pasDbOpenSql(sql, 0);
	if(psCur != NULL)
	{
		printf("\n psCur is not null\n\n");
		char caServicename[36] = "";
		char caServicecode[36] = "";
		unsigned long dwCount = 0;		
		int nRet = 0;
		while(0 == (nRet = pasDbFetchInto(psCur, UT_TYPE_STRING, 32, &caServicename,
												 UT_TYPE_STRING, 32, &caServicecode,	
												 UT_TYPE_LONG, 	20, &dwCount)) || 1405 == nRet)
		{
			iNum++;
			if(iNum > 1)
			{
				utPltPutLoopVar(psDbHead, "dh", iNum, ",");
			}
			utPltPutLoopVarF(psDbHead, "servicename", iNum, "%s", caServicename);
			utPltPutLoopVarF(psDbHead, "servicecode", iNum, "%s", caServicecode);
			utPltPutLoopVarF(psDbHead, "count", iNum, "%lu", dwCount);
			
			if(dwCount >= dwCountMax)
			{
				dwCountMax = dwCount;				
			}	
			
			//清空容器
			memset(caServicename, 0, sizeof(caServicename));
			dwCount = 0;
		}
	}
	
	pasDbCloseCursor(psCur);
	utPltPutVarF(psDbHead, "result", "%d\n", iNum);
	utPltPutVarF(psDbHead, "max", "%lu\n", dwCountMax);
	printf("final iNum = %d\n", iNum);
	utPltOutToHtml(iFd, psMsgHead, psDbHead, "mac/infoquery/macterm/ChosenMacPlaceOftenCount.htm");	
	return 0;	
}

/* MAC日志查询下特征分析3：频繁出现的场所类型 */
int ncsMacLog_ChosenMac_PlaceTypeOftenCount(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{			
	/* 初始化模板数据缓冲区 */
	utPltDbHead *psDbHead = utPltInitDb();
	
	/* 将数据包显示出来,本函数主要用于调试。
	显示时先显示数据包的头部，然后显示数据包的内容。*/
	utMsgPrintMsg(psMsgHead);
	
	/* 场所编号、查询时间类型(天、周、月、三月、季度、年) */	
	char caTimeFlag[10] = "";
	char caMac[32] = "";
	int iReturn = 0;
	/* 从消息数据包 psMsgHead 中提取指定数量的变量 */
	iReturn = utMsgGetSomeNVar(psMsgHead, 2, 		
		"timevalue", UT_TYPE_STRING, sizeof(caTimeFlag) - 1, caTimeFlag,
		"mac", UT_TYPE_STRING, sizeof(caMac) - 1, caMac);
	MyremoveAll(caMac, '-');
	MyremoveAll(caMac, ':');
	
	printf("\n");		
	printf("timevalue = %s\n", caTimeFlag);	
	printf("mac = %s\n", caMac);
			
	//1.根据当前年月生成表名
    char table_name[1024] = "";
    char  year[10];
    char  mon[10];
    memset(year, 0, sizeof(year));
    memset(mon, 0 , sizeof(mon));
    time_t now;
    time(&now);
    timeToStringPlace(now, year, mon);
    int syear = atoi(year);
    int smonth = atoi(mon);		
    snprintf(table_name + strlen(table_name), 1024 - strlen(table_name), "ncmactermatt_if_%4u%02u", syear, smonth);
			
	//2.根据查询时间类型选定相应时间条件
	char caStartDate[32] = "";
   	char caEndDate[32] = "";
	
	char sql[4096] = "";	
	char sqlTmp[4096] = "";
	snprintf(sql, sizeof(sql), "SELECT CASE a.gtype WHEN 1 THEN '旅店宾馆类' WHEN 2 THEN '图书馆阅览室' WHEN 3 THEN '电脑培训中心类' WHEN 4 THEN '娱乐场所类' WHEN 5 THEN '交通枢纽' WHEN 6 THEN '公共交通工具' WHEN 7 THEN '餐饮服务场所' WHEN 8 THEN '金融服务场所' WHEN 10 THEN '购物场所' WHEN 11 THEN '公共服务场所' WHEN 12 THEN '文化服务场所' WHEN 13 THEN '公共休闲场所' WHEN 14 THEN '其他' END AS '服务类型', SUM( times) times FROM ncsuser a, (SELECT servicecode, COUNT(stime) AS times from %s where stime >= ", table_name);
	
	if(strcmp(caTimeFlag, "null") ==0 )		//查询今天：默认
	{
		snprintf(sqlTmp, sizeof(sqlTmp), " UNIX_TIMESTAMP(DATE_SUB(NOW(), INTERVAL 1 DAY)) ");
	}
	else if(strcmp(caTimeFlag, "1") ==0)	//查询今天：用户选择
	{
		snprintf(sqlTmp, sizeof(sqlTmp), " UNIX_TIMESTAMP(DATE_SUB(NOW(), INTERVAL 1 DAY)) ");
	}
	else if(strcmp(caTimeFlag, "2") ==0 )	//最近三天
	{
		snprintf(sqlTmp, sizeof(sqlTmp), " UNIX_TIMESTAMP(DATE_SUB(NOW(), INTERVAL 3 DAY)) ");
	}
	else if(strcmp(caTimeFlag, "3") ==0 )	//最近一周
	{
		snprintf(sqlTmp, sizeof(sqlTmp), " UNIX_TIMESTAMP(DATE_SUB(NOW(), INTERVAL 7 DAY)) ");
	}
	else if(strcmp(caTimeFlag, "4") ==0 )	//最近一月
	{
		snprintf(sqlTmp, sizeof(sqlTmp), " UNIX_TIMESTAMP(DATE_SUB(NOW(), INTERVAL 1 MONTH)) ");
	}
	else if(strcmp(caTimeFlag, "5") ==0 )	//最近三月
	{
		snprintf(sqlTmp, sizeof(sqlTmp), " UNIX_TIMESTAMP(DATE_SUB(NOW(), INTERVAL 3 MONTH)) ");
	}
	else if(strcmp(caTimeFlag, "6") ==0 )	//最近半年
	{
		snprintf(sqlTmp, sizeof(sqlTmp), " UNIX_TIMESTAMP(DATE_SUB(NOW(), INTERVAL 6 MONTH)) ");
	}
	else 	//最近半年
	{
		snprintf(sqlTmp, sizeof(sqlTmp), " UNIX_TIMESTAMP(DATE_SUB(NOW(), INTERVAL 6 MONTH)) ");
	}
	
	//printf(sqlTmp);
	printf("\n");
	
	/* 在原来的sql语句有效字符结尾处，追加上 sqlTmp */
	snprintf(sql + strlen(sql), sizeof(sql) - strlen(sql), "%s", sqlTmp);
	snprintf(sql + strlen(sql), sizeof(sql) - strlen(sql), " and stime <= (UNIX_TIMESTAMP(NOW())) and replace(mac,'-','') like '%c%s%c' GROUP BY servicecode ) b WHERE b.servicecode =a.username ", '%', caMac, '%');
	
	printf("\n");
	printf("\n");
	printf("%s", sql);
	printf("\n");
	
	int iNum = 0, iRet = 0;
	unsigned long dwCountMax = 0;
	pasDbCursor *psCur = NULL;
	psCur = pasDbOpenSql(sql, 0);
	if(psCur != NULL)
	{
		printf("\n psCur is not null\n\n");
		//char caServicename[36] = "";		
		char caGtype[36] = "";		//场所类型
		unsigned long dwCount = 0;		
		int nRet = 0;
		while(0 == (nRet = pasDbFetchInto(psCur, UT_TYPE_STRING, 32, &caGtype,	
												 UT_TYPE_LONG, 	20, &dwCount)) || 1405 == nRet)
		{
			iNum++;
			if(iNum > 1)
			{
				utPltPutLoopVar(psDbHead, "dh", iNum, ",");
			}
			utPltPutLoopVarF(psDbHead, "gtype", iNum, "%s", caGtype);
			utPltPutLoopVarF(psDbHead, "count", iNum, "%lu", dwCount);
			
			if(dwCount >= dwCountMax)
			{
				dwCountMax = dwCount;				
			}	
			
			//清空容器
			memset(caGtype, 0, sizeof(caGtype));
			dwCount = 0;
		}
	}
	
	pasDbCloseCursor(psCur);
	utPltPutVarF(psDbHead, "result", "%d\n", iNum);
	utPltPutVarF(psDbHead, "max", "%lu\n", dwCountMax);
	printf("final iNum = %d\n", iNum);
	utPltOutToHtml(iFd, psMsgHead, psDbHead, "mac/infoquery/macterm/ChosenMacPlaceTypeOftenCount.htm");	
	return 0;	
}

/* MAC日志查询身份查看：查询指定mac的虚拟身份类型、内容 */
int ncsMacLog_ChosenMac_IDCheck(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{			
	/* 初始化模板数据缓冲区 */
	utPltDbHead *psDbHead = utPltInitDb();
	
	/* 将数据包显示出来,本函数主要用于调试。
	显示时先显示数据包的头部，然后显示数据包的内容。*/
	utMsgPrintMsg(psMsgHead);
	
	char caMac[32] = "";
	memset(caMac, 0, sizeof(caMac));
	int iReturn = 0;
	/* 从消息数据包 psMsgHead 中提取指定数量的变量 */
	iReturn = utMsgGetSomeNVar(psMsgHead, 1, 				
		"mac", UT_TYPE_STRING, sizeof(caMac) - 1, caMac);
		
	printf("\n");			
	
#ifdef INFOQUERY_DEBUG	
	printf("mac = %s\n", caMac);
#endif	
	
	
	char sql[4096] = "";	
	snprintf(sql, sizeof(sql), "select a.name, b.vname from ncsnettype a, nctermnetlog b where b.mac like '%c%s%c' and a.ntype = b.nettype", '%', caMac, '%');
		
#ifdef INFOQUERY_DEBUG	
	printf("\n");
	printf("%s\n",sql);
	printf("\n");
#endif
	
	
	int iNum = 0, iRet = 0;
	unsigned long dwCountMax = 0;
	pasDbCursor *psCur = NULL;
	psCur = pasDbOpenSql(sql, 0);
	if(psCur != NULL)
	{
		printf("\n psCur is not null\n\n");
		char caVtype[36] = "";	
		char caVname[36] = "";	
		
		int nRet = 0;
		while(0 == (nRet = pasDbFetchInto(psCur, UT_TYPE_STRING, 32, &caVtype,	
												 UT_TYPE_STRING, 32, &caVname)) || 1405 == nRet)
		{
			iNum++;
			if(iNum > 1)
			{
				utPltPutLoopVar(psDbHead, "dh", iNum, ",");
			}
			utPltPutLoopVarF(psDbHead, "vtype", iNum, "%s", caVtype);
			utPltPutLoopVarF(psDbHead, "vname", iNum, "%s", caVname);
						
			//清空容器
			memset(caVtype, 0, sizeof(caVtype));
			memset(caVname, 0, sizeof(caVname));		
		}
	}
	
	pasDbCloseCursor(psCur);
	utPltPutVarF(psDbHead, "result", "%d\n", iNum);
	
#ifdef INFOQUERY_DEBUG		
	printf("final iNum = %d\n", iNum);
#endif

	utPltOutToHtml(iFd, psMsgHead, psDbHead, "mac/infoquery/macterm/IdCheck.htm");	
	return 0;	
}

/* MAC日志查询加入分组：查询终端mac组信息、mac组描述 */
int ncsMacLog_ChosenMac_GetMacGroupInfo(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{			
	/* 初始化模板数据缓冲区 */
	utPltDbHead *psDbHead = utPltInitDb();
	
	/* 将数据包显示出来,本函数主要用于调试。
	显示时先显示数据包的头部，然后显示数据包的内容。*/
	utMsgPrintMsg(psMsgHead);
	
	char caMac[32] = "";
	memset(caMac, 0, sizeof(caMac));
	int iReturn = 0;		
	char sql[1024] = "";	
	
	snprintf(sql, sizeof(sql), "select groupid, groupname, groupdesc from nctermmacgroup");

#ifdef INFOQUERY_DEBUG	
	printf("\n");
	printf("%s\n",sql);
	printf("\n");
#endif
	
	int iNum = 0, iRet = 0;
	unsigned long dwCountMax = 0;
	pasDbCursor *psCur = NULL;
	psCur = pasDbOpenSql(sql, 0);
	if(psCur != NULL)
	{
		
#ifdef INFOQUERY_DEBUG		
		printf("\n psCur is not null\n\n");
#endif		
		
		char caGrouid[12] = "";	
		char caGroupname[36] = "";	
		char caGroupdesc[256] = "";	
		
		int nRet = 0;
		while(0 == (nRet = pasDbFetchInto(psCur, UT_TYPE_STRING, 12, &caGrouid, 
												 UT_TYPE_STRING, 36, &caGroupname,	
												 UT_TYPE_STRING, 256, &caGroupdesc)) || 1405 == nRet)
		{
			iNum++;
			if(iNum > 1)
			{
				utPltPutLoopVar(psDbHead, "dh", iNum, ",");
			}
			
			utPltPutLoopVarF(psDbHead, "groupid", iNum, "%s", caGrouid);
			utPltPutLoopVarF(psDbHead, "groupname", iNum, "%s", caGroupname);
			utPltPutLoopVarF(psDbHead, "groupdesc", iNum, "%s", caGroupdesc);
						
			//清空容器
			memset(caGrouid, 0, sizeof(caGrouid));
			memset(caGroupname, 0, sizeof(caGroupname));
			memset(caGroupdesc, 0, sizeof(caGroupdesc));		
		}
	}
	
	pasDbCloseCursor(psCur);
	utPltPutVarF(psDbHead, "result", "%d\n", iNum);
	
#ifdef INFOQUERY_DEBUG		
	printf("final iNum = %d\n", iNum);
#endif
		
	utPltOutToHtml(iFd, psMsgHead, psDbHead, "mac/infoquery/macterm/GetMacGroupInfo.htm");	
	return 0;	
}

/* MAC日志查询加入分组：将所选的mac加入相应mac组名及描述 */
int ncsMacLog_ChosenMac_JoinMacGroup(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{			
	/* 初始化模板数据缓冲区 */
	utPltDbHead *psDbHead = utPltInitDb();
	
	/* 将数据包显示出来,本函数主要用于调试。
	显示时先显示数据包的头部，然后显示数据包的内容。*/
	utMsgPrintMsg(psMsgHead);
	
	char caMac[1024] = "";
	char caGroupId[12] = "";	
	int iReturn = 0;
	
	/* 从消息数据包 psMsgHead 中提取指定数量的变量 */
	iReturn = utMsgGetSomeNVar(psMsgHead, 2, 				
		"mac", UT_TYPE_STRING, sizeof(caMac) - 1, caMac,
		"groupid", UT_TYPE_STRING, sizeof(caGroupId) - 1, caGroupId);	
	
	printf("\n");	
	
#ifdef INFOQUERY_DEBUG		
	/* iReturn 的值输出打印会导致下面程序执行异常 */
	//printf("iReturn = %s\n", iReturn);	
	printf("mac = %s\n", caMac);
	printf("groupid = [%s]\n", caGroupId);
#endif	
	
	int iRet = 0;
	char sql[1024] = "";	
		
	/* 循环截取出 caMac 中的多个mac值 */
	char *pMac = NULL;
	pMac = strtok(caMac, ",");
	while(pMac)
	{
		//printf("%s\n", pMac);
		
		snprintf(sql, sizeof(sql), "insert into nctermmacgroupinfo(mac, groupid, modtime) values(\'%s\', \'%s\', UNIX_TIMESTAMP(NOW())) ", pMac, caGroupId);
		
		iRet = pasDbExecSqlF(sql);
		if(iRet)
		{
			utPltPutVar(psDbHead,"result", "false");
			utPltPutVar(psDbHead,"message", "加入分组失败");
		}
		
#ifdef INFOQUERY_DEBUG	
		printf("sql = %s\n", sql);		
#endif

		memset(sql, 0 , sizeof(sql));		
		pMac = NULL;
		pMac = strtok(NULL, ",");
	}
	if(!iRet)
	{
		utPltPutVar(psDbHead,"result", "true");
		utPltPutVar(psDbHead,"message", "加入分组成功");
	}
				
	utPltOutToHtml(iFd, psMsgHead, psDbHead, "mac/infoquery/macterm/JoinGroup.htm");		
	return 0;	
}

/* MAC日志查询：list 添加 "历史ssid列表" 查询选中MAC的 ssid 去重列表 */
int ncsMacLog_ChosenMac_DistinctSsid(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
	/* 初始化模板数据缓冲区 */
	utPltDbHead *psDbHead = utPltInitDb();
	//long infoqueryDebug = utComGetVar_ld(psShmHead, "InfoqueryDebug", 0);
	
	/* 将数据包显示出来,本函数主要用于调试。
	显示时先显示数据包的头部，然后显示数据包的内容。*/
	utMsgPrintMsg(psMsgHead);
	
	char caMac[32] = "";
	char caTimeFlag[10] = "";	
	int iReturn = 0;
	
	/* 从消息数据包 psMsgHead 中提取指定数量的变量 */
	iReturn = utMsgGetSomeNVar(psMsgHead, 2, 				
		"mac", UT_TYPE_STRING, sizeof(caMac) - 1, caMac,
		"timevalue", UT_TYPE_STRING, sizeof(caTimeFlag) - 1, caTimeFlag);		
		
	//MyremoveAll(caMac, '-');
	//MyremoveAll(caMac, ':');
	
	printf("mac = %s\n", caMac);
	printf("timevalue = %s\n", caTimeFlag);
	
	/* iReturn 的值输出打印会导致下面程序执行异常 */
	//printf("iReturn = %s\n", iReturn);		

	//1.根据当前年月生成表名
    char table_name[1024] = "";
    char  year[10];
    char  mon[10];
    memset(year, 0, sizeof(year));
    memset(mon, 0 , sizeof(mon));
    time_t now;
    time(&now);
    timeToStringPlace(now, year, mon);
    int syear = atoi(year);
    int smonth = atoi(mon);		
    snprintf(table_name + strlen(table_name), 1024 - strlen(table_name), "ncmactermatt_if_%4u%02u", syear, smonth);
			
	//2.根据查询时间类型选定相应时间条件
	//printf("table_name = %s\n\n", table_name);
	
	char sql[4096] = "";	
	char sqlTmp[4096] = "";
	snprintf(sql, sizeof(sql), "select distinct ssid, servicename from %s where ", table_name);
	
	if(strcmp(caTimeFlag, "null") ==0 )		//查询今天：默认
	{
		snprintf(sqlTmp, sizeof(sqlTmp), " UNIX_TIMESTAMP(DATE_SUB(NOW(), INTERVAL 1 DAY)) ");
	}
	else if(strcmp(caTimeFlag, "1") ==0)	//查询今天：用户选择
	{
		snprintf(sqlTmp, sizeof(sqlTmp), " UNIX_TIMESTAMP(DATE_SUB(NOW(), INTERVAL 1 DAY)) ");
	}
	else if(strcmp(caTimeFlag, "2") ==0 )	//最近三天
	{
		snprintf(sqlTmp, sizeof(sqlTmp), " UNIX_TIMESTAMP(DATE_SUB(NOW(), INTERVAL 3 DAY)) ");
	}
	else if(strcmp(caTimeFlag, "3") ==0 )	//最近一周
	{
		snprintf(sqlTmp, sizeof(sqlTmp), " UNIX_TIMESTAMP(DATE_SUB(NOW(), INTERVAL 7 DAY)) ");
	}
	else if(strcmp(caTimeFlag, "4") ==0 )	//最近一月
	{
		snprintf(sqlTmp, sizeof(sqlTmp), " UNIX_TIMESTAMP(DATE_SUB(NOW(), INTERVAL 1 MONTH)) ");
	}
	else if(strcmp(caTimeFlag, "5") ==0 )	//最近三月
	{
		snprintf(sqlTmp, sizeof(sqlTmp), " UNIX_TIMESTAMP(DATE_SUB(NOW(), INTERVAL 3 MONTH)) ");
	}
	else if(strcmp(caTimeFlag, "6") ==0 )	//最近半年
	{
		snprintf(sqlTmp, sizeof(sqlTmp), " UNIX_TIMESTAMP(DATE_SUB(NOW(), INTERVAL 6 MONTH)) ");
	}
	else 	//最近半年
	{
		//snprintf(sqlTmp, sizeof(sqlTmp), " UNIX_TIMESTAMP(DATE_SUB(NOW(), INTERVAL 6 MONTH)) ");
	}
	
			
	/* 在原来的sql语句有效字符结尾处，追加上 sqlTmp */
	snprintf(sql + strlen(sql), sizeof(sql) - strlen(sql), "%s", sqlTmp);
	snprintf(sql + strlen(sql), sizeof(sql) - strlen(sql), " mac = \'%s\' and ssid != '' ORDER BY ssid asc", caMac);
	
	printf("sql = %s\n\n", sql);
		
	int iNum = 0, iRet = 0;	
	pasDbCursor *psCur = NULL;
	psCur = pasDbOpenSql(sql, 0);
	if(psCur != NULL)
	{
		printf("\n psCur is not null\n\n");
		char caSsid[36] = "";
		char caServicename[36] = "";
		int nRet = 0;	
		while(0 == (nRet = pasDbFetchInto(psCur, UT_TYPE_STRING, 35, &caSsid,
												 UT_TYPE_STRING, 35, &caServicename)) || 1405 == nRet)
		{
			iNum++;
			if(iNum > 1)
			{
				utPltPutLoopVar(psDbHead, "dh", iNum, ",");
			}
			utPltPutLoopVarF(psDbHead, "ssid", iNum, "%s", caSsid);						
			utPltPutLoopVarF(psDbHead, "servicename", iNum, "%s", caServicename);	

			//清空容器
			memset(caSsid, 0, sizeof(caSsid));			
			memset(caServicename, 0, sizeof(caServicename));	
		}
	}
	
	pasDbCloseCursor(psCur);
	
	
	utPltPutVarF(psDbHead, "totalCount", "%d\n", iNum);	
	utPltOutToHtml(iFd, psMsgHead, psDbHead, "mac/infoquery/macterm/ncsMacLog_ChosenMac_DistinctSsid.htm");		
	return 0;
}



/* 热点查询：显示热点信息日志 */
int ncsHotSpot_infolist_v4(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
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
    char caStime[16];
   	char caEtime[16];
   	char caTemp_d[64];
   	char caGroupid[36],caDid[36];
    char caSdate[24],caEdate[24];
    char caApmac[36],caMac[36],caApname[64],caPosition[64];
    char caCorpname[128];
    char caCont[2048], caSsid[32] = "";
    char caServicecode[16], caSearchtype[32];
	strcpy(caFacs, (const char*)getDsFacDid());
	strcpy(caGroups, (const char*)getDsGroupid());         
	
	/* 如果导出 */	
	char caExport[10] = "";	
	
	
	
	utPltDbHead *psDbHead = utPltInitDb();
	utMsgPrintMsg(psMsgHead);
	iReturn = utMsgGetSomeNVar(psMsgHead,22,
			"TotPg",      	UT_TYPE_STRING, 12, caTotPg,
			"CurPg",      	UT_TYPE_STRING, 12, caCurPg,
			"keyword", 		UT_TYPE_STRING,60,caKeyword,
			"page",         UT_TYPE_STRING,10,caPage,
			"limit",        UT_TYPE_STRING,10,caLimit,
			"sort",         UT_TYPE_STRING, 15, caSort,
			"dir",          UT_TYPE_STRING, 8,  caDir,
			"time_flag",    UT_TYPE_STRING,8,caTime_flag,
			"did",          UT_TYPE_STRING,10,caDid,
			"groupid",      UT_TYPE_STRING,10,caGroupid,
			"sdate",        UT_TYPE_STRING,12,sdate,
			"stime",        UT_TYPE_STRING,10,caStime,
			"edate",        UT_TYPE_STRING,12,edate,
			"etime",        UT_TYPE_STRING,10,caEtime,			
			"mac",          UT_TYPE_STRING,20,caMac,			
			"ssid",         UT_TYPE_STRING,30,caSsid,
			"searchtype",   UT_TYPE_STRING,10,caSearchtype,
			"hotinfoexport",      UT_TYPE_STRING,9,caExport,
			"servicecode",  UT_TYPE_STRING,15,caServicecode,
			"servicename",  UT_TYPE_STRING,60,caCorpname,
			"apmac",        UT_TYPE_STRING,20,caApmac,
			"apname",       UT_TYPE_STRING,31,caApname);
   	  
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
	
	if(strlen(sdate) > 0 && strlen(edate) > 0)
	{
		ptmp = ncsUtlGetTable3(sdate, edate,caStime,caEtime, "ncmachotinfo_if_", &lStartTime, &lTime, &lCount);
		printf("lStartTime=%d,sdate=%s\n",lStartTime,sdate);
	}
	else
	{
		ptmp = ncsUtlGetTable(lTime, days, "ncmachotinfo_if_",  &lStartTime, &lCount);
		lTime=lTime+2*3600;
	}

	memset(caCont,0,sizeof(caCont));
	
	/* 高级查询搜索字段 */
	if(strlen(caSearchtype)>0)
	{		
		if(strlen(caApmac)>0)			//热点MAC
		{
			sprintf(caCont+strlen(caCont)," and apmac like '%c%s%c' ",'%',caApmac,'%');
		}
		if(strlen(caApname)>0)			//采集设备编号
		{
			sprintf(caCont+strlen(caCont)," and apname like '%c%s%c' ",'%',caApname,'%');
		}
		if(strlen(caServicecode)>0)		//场所编号
		{
			sprintf(caCont+strlen(caCont)," and servicecode like '%c%s%c' ",'%',caServicecode,'%');
		}
		if(strlen(caCorpname)>0)		//场所名称
		{
			sprintf(caCont+strlen(caCont)," and servicename like '%c%s%c' ",'%',caCorpname,'%');
		}		
	}
	else
	{
		if(strlen(caKeyword)>0)
		{
			sprintf(caCont," and (mac like '%c%s%c' or ssid like '%c%s%c' or servicecode like '%c%s%c' or servicename like '%c%s%c' or apmac like '%c%s%c') ",'%',caKeyword,'%','%',caKeyword,'%','%',caKeyword,'%','%',caKeyword,'%','%',caKeyword,'%');		
		}
	}
	if(strlen(caDid)>0)
	{
		sprintf(caCont+strlen(caCont)," and servicecode='%s' ",getServicecodeByid(psShmHead,atol(caDid)));
	}
	if(strlen(caGroupid)>0)
	{
		sprintf(caCont+strlen(caCont)," and servicecode in (%s)",getServicecodeByGroupid(psShmHead,caGroupid));
	}
	if(strlen(caGroups)>0)
	{
		sprintf(caCont+strlen(caCont)," and servicecode in (%s)",getServicecodeByGroupid(psShmHead,caGroups));
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
		printf("sql3 = %s\n", sql3);
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
	printf(" lCount2 = %lu  num2 = %d  lTotRec2 = %lu\n\n", lCount2, num2, lTotRec2);
	utPltPutVarF(psDbHead,"distinctCount", "%lu", lTotRec2);
	
	for(i = 0; i < lCount; i++)
	{
		lCount1 = 0;			
		memset(sql, 0, sizeof(sql));	
		
		/* 关联查询 */	
		//snprintf(sql, sizeof(sql), "select count(*) from %s a, nchotspotinfo b where a.stime >= %lu and a.stime <= %lu and a.ssid != \'\' AND a.ssid = b.ssid ",  ptmp[i], lStartTime, lTime);
		
		/* 非关联查询 */
		snprintf(sql, sizeof(sql), "select count(*) from %s a where a.stime >= %lu and a.stime <= %lu and a.ssid != \'\' ",  ptmp[i], lStartTime, lTime);
		strcat(sql,caCont);
				
		printf("sql = %s\n", sql);				
		
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
	
	printf(" num = %d\n", num);
	for(i = 0; i < num; i++)
	{
		lTotRec += tableinfo[i].lCount;
	}
	utPltPutVarF(psDbHead,"TotRec", "%lu", lTotRec);
	
	
	for(i = num-1; i >= 0; i--)
	{
		printf("table[%d] = %s, _lCount1[%d] = %lu\n", i, tableinfo[i].tname, i, tableinfo[i].lCount);
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
		sprintf(caFilename,"hotInfoLog.csv");
		sprintf(caPath,"%s", "/home/ncmysql/ncs");
		sprintf(caFile,"%s/%s",caPath,caFilename);
		fp = fopen(caFile,"w");	
		if(fp == NULL) 
		{				
			printf("数据导出出错\n\n");
			return 0;
		}
		fprintf(fp, "热点查询\n");	//标题
		printf(" 写入热点查询\n\n");			
		ncUtlPrintCsv(fp, 14,
				UT_TYPE_STRING, "是否合规",
                UT_TYPE_STRING, "热点SSID",
				UT_TYPE_STRING, "场所名称",
				UT_TYPE_STRING, "热点频道",
				UT_TYPE_STRING, "热点加密类型",				
                UT_TYPE_STRING, "采集时间",
				UT_TYPE_STRING, "热点MAC地址", 				
                UT_TYPE_STRING, "热点场强",    
				UT_TYPE_STRING, "采集设备经度",
                UT_TYPE_STRING, "采集设备纬度",
				UT_TYPE_STRING, "场所编号",
                UT_TYPE_STRING, "采集设备编号",                								             
                UT_TYPE_STRING, "X坐标",
                UT_TYPE_STRING, "Y坐标"); 
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
		/* 加上是否合规字段 */
	   //snprintf(sql, sizeof(sql), "select a.servicecode, a.servicename, a.address, a.mac, a.apname, a.apmac, a.ssid, a.stime, a.channel, a.rssi, a.security, a.longitude, a.latitude, b.authflag from %s a, nchotspotinfo b where a.stime >= %lu and a.stime <= %lu and a.ssid != \'\' AND a.ssid = b.ssid ", tableinfo[i].tname, lStartTime, lTime);
	   
	   /* 去掉是否合规字段 */
	   snprintf(sql, sizeof(sql), "select a.servicecode, a.servicename, a.address, a.mac, a.apname, a.apmac, a.ssid, a.stime, a.channel, a.rssi, a.security, a.longitude, a.latitude from %s a where a.stime >= %lu and a.stime <= %lu and a.ssid != \'\' ", tableinfo[i].tname, lStartTime, lTime);
	   
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
	
		printf("__line = %d, sql = %s\n", __LINE__, sql);
		psCur = pasDbOpenSql(sql, 0);
		if(psCur != NULL)
		{
		char caServicecode[16],caServicename[64],caAddress[64],caMac[20],caApname[32],caApmac[20],caSsid[64];
         long lStarttime,lEndtime,lFlags; 
         char caChannel[32],caRssi[16],caSecurity[16],caLongitude[16],caLatitude[16], caAuthFlag[3] = "";
		 char caHotspotfirm[32] = "", caMacBinary[16] = ""; unsigned char *pHotspotfirm;
		 
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
				 UT_TYPE_STRING,3,caAuthFlag);
			   			
			//printf("\n\n**********************************************");
			while(((iret == 0) || (iret == 1405)))
			{
				/* 选择导出的话 */
				if(strlen(caExport) > 0)
				{
					if(strcmp(caAuthFlag, "0"))
					{
						strcpy(caAuthFlag, "非合规");
					}
					else if(strcmp(caAuthFlag, "1"))
					{
						strcpy(caAuthFlag, "合规");
					}
					else
					{
						strcpy(caAuthFlag, "其他");
					}
					
					ncUtlPrintCsv(fp, 14,
						UT_TYPE_STRING, caAuthFlag,
						UT_TYPE_STRING, caSsid,
						UT_TYPE_STRING, caServicename,
						UT_TYPE_STRING, caChannel,
						UT_TYPE_STRING, caSecurity,
						UT_TYPE_LONG, lStarttime,
						UT_TYPE_STRING, caApmac,   
						UT_TYPE_STRING, caRssi, 
						UT_TYPE_STRING, caLongitude,
						UT_TYPE_STRING, caLatitude,
						UT_TYPE_STRING, caServicecode,
						UT_TYPE_STRING, caApname,                                                              					  
						UT_TYPE_STRING, "X坐标",
						UT_TYPE_STRING, "Y坐标"); 
				}
				else
				{
					iNum++;
					if(iNum > 1)
					{
						utPltPutLoopVar(psDbHead,"dh",iNum,",");
					}
				 
					utPltPutLoopVar(psDbHead,"authflag",iNum,caAuthFlag);		 
					utPltPutLoopVar(psDbHead,"stime",iNum,utTimFormat("%Y-%m-%d %H:%M:%S",lStarttime));         
					utPltPutLoopVar(psDbHead,"address",iNum,caAddress);
					utPltPutLoopVar(psDbHead,"apname",iNum,caApname);
					//utPltPutLoopVar(psDbHead,"mac",iNum,caMac);
					
					//写入热点场商
					memset(caHotspotfirm, 0, sizeof(caHotspotfirm));
					memset(caMacBinary, 0, sizeof(caMacBinary));
					pasCvtMacI(caMac, caMacBinary);
					
					pHotspotfirm = ncUtlCvtMac2Vendor(caMacBinary, caHotspotfirm);
					if(pHotspotfirm == NULL)
					{
						snprintf(caHotspotfirm, sizeof(caHotspotfirm) - 1, "%s", "其他");
					}					
					//printf("\n caMac = %s, caMacBinary = %s, pHotspotfirm = %p, caHotspotfirm = %s\n", caMac, caMacBinary, pHotspotfirm, caHotspotfirm);
					
					utPltPutLoopVar(psDbHead,"hotspotfirm", iNum, caHotspotfirm);
					
					
					utPltPutLoopVar(psDbHead,"apmac",iNum,caMac);	//前台热点mac显示为mac地址
					utPltPutLoopVar(psDbHead,"servicename",iNum,caServicename);
					utPltPutLoopVar(psDbHead,"channel",iNum,caChannel);
					utPltPutLoopVar(psDbHead,"rssi",iNum,caRssi);
					//printf("caSecurity=%s\n",caSecurity);
					utPltPutLoopVar(psDbHead,"security",iNum,caSecurity);
					utPltPutLoopVar(psDbHead,"longitude",iNum,caLongitude);
					utPltPutLoopVar(psDbHead,"latitude",iNum,caLatitude);
					utPltPutLoopVar(psDbHead,"ssid",iNum,caSsid);
					utPltPutLoopVar(psDbHead,"servicecode",iNum,caServicecode);
				}	
							
				memset(caAuthFlag,0,sizeof(caAuthFlag));
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
						UT_TYPE_STRING,3,caAuthFlag);
							
			}		
			//printf("**********************************************\n\n");	
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
    return 0;
}

/* 热点查询：标记合规、不合规 */
int ncsHotSpot_MarkRuleorNot(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
	/* 初始化模板数据缓冲区 */
	utPltDbHead *psDbHead = utPltInitDb();
	
	/* 将数据包显示出来,本函数主要用于调试。
	显示时先显示数据包的头部，然后显示数据包的内容。*/
	utMsgPrintMsg(psMsgHead);
	
	char caAuthflag[60] = "";	
	char caSsid[1024] = "";	
	char caMac[1024] = "";
	int iReturn = 0;
	
	/* 从消息数据包 psMsgHead 中提取指定数量的变量 */
	iReturn = utMsgGetSomeNVar(psMsgHead, 3, 						
		"authflag", UT_TYPE_STRING, sizeof(caAuthflag) - 1, caAuthflag,
		"ssid", UT_TYPE_STRING, sizeof(caSsid) - 1, caSsid,
		"mac", UT_TYPE_STRING, sizeof(caMac) - 1, caMac);	
	
	printf("\n");	
	
	//caSsid[1024] = "MERCURY_661788,MERCURY_661788,Test_HKPLAZA,Test_HKPLAZA";
	//caMac[1024]  = "14-CF-92-66-17-88,14-CF-92-66-17-88,02-34-CB-C0-A1-5F,02-34-CB-C0-A1-5F";
	
#ifdef INFOQUERY_DEBUG		

	/* iReturn 的值输出打印会导致下面程序执行异常 */	
	//printf("authflag = [%s]\n", caAuthflag);
	printf("ssid = %s\n", caSsid);
	printf("mac = %s\n", caMac);
#endif	
	
	int iRet = 0;
	char sql[1024] = "";
	
	char *pSsidNow = NULL;
	char *pSsidLeft = NULL;
	char *pMacNow = NULL;
	char *pMacLeft = NULL;
			
			
	pSsidNow = strtok_r(caSsid, ",", &pSsidLeft);	
	pMacNow = strtok_r(caMac, ",", &pMacLeft);
	//while(pSsidNow)
	{						
		//snprintf(sql, sizeof(sql), "update nchotspotinfo set authflag = \'%s\' where ssid = \'%s\' ", caAuthflag, pSsid);
		
		iRet = pasDbExecSqlF("select ssid from nchotspotinfo where ssid = '1234'");
		printf("iRet = %d\n", iRet);
		if(iRet)
		{
			utPltPutVar(psDbHead,"result", "false");
			utPltPutVar(psDbHead,"message", "标记失败");
		}
		
#ifdef INFOQUERY_DEBUG	
		printf("sql = %s\n", sql);		
#endif
		memset(sql, 0 , sizeof(sql));	
		
		//printf("pMacNow = %s\n", pMacNow);
		pMacNow = strtok_r(NULL, ",", &pMacLeft);
		
		//printf("pSsidNow = %s\n", pSsidNow);
		pSsidNow = strtok_r(NULL, ",", &pSsidLeft);
		
				
	}
	if(!iRet)
	{
		utPltPutVar(psDbHead,"result", "true");
		utPltPutVar(psDbHead,"message", "标记成功");
	}
				
	utPltOutToHtml(iFd, psMsgHead, psDbHead, "mac/infoquery/hotinfo/MarkRuleorNot.htm");		
	return 0;
	
}

//功能封装：(暂未用到) 根据UserID来查询获取GroupID
char* getGroupIdFromUserName(char* caUserName)
{
	char sql[40960] = "";
	static char caGroupid[64]="";
	
	snprintf(sql, sizeof(sql), "select groupid from ncsuser where username = /'%s/' ", caUserName);
	pasDbOneRecord(sql, 0, UT_TYPE_STRING, 63, &caGroupid);	
	return caGroupid;
}



/* 场所信息查询：显示终端特征信息日志表 */
int ncsPlace_mactermlist_v4(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
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
   
	utPltDbHead *psDbHead = utPltInitDb();
	utMsgPrintMsg(psMsgHead);
	iReturn = utMsgGetSomeNVar(psMsgHead,27,
			"TotPg",      	UT_TYPE_STRING, 12, caTotPg,
			"CurPg",      	UT_TYPE_STRING, 12, caCurPg,
			"keyword", 		UT_TYPE_STRING,60,	caKeyword,
			"page",         UT_TYPE_STRING,10,	caPage,
			"limit",        UT_TYPE_STRING,10,	caLimit,
			"sort",         UT_TYPE_STRING, 15, caSort,
			"dir",          UT_TYPE_STRING, 8,  caDir,
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
			"sname",      UT_TYPE_STRING,31, caSname,
			"servicegtype",      UT_TYPE_STRING,31, caServicegtype,
			"servicearea",      UT_TYPE_STRING, 35, caServicearea,
			"ceoname",      UT_TYPE_STRING, 31, caCeoname,
			"jytype",      UT_TYPE_STRING, 31, caJytype);
	
	char caGroupcodeSql[1024] = "";	
	strcpy(caGroupcodeSql, getGroupcodeSql("servicecode", caGroupid));	
	
	MyremoveAll(caKeyword, '-');
	MyremoveAll(caKeyword, ':');
   	  
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
	
	if(strlen(sdate) > 0 && strlen(edate) > 0)
	{
		ptmp = ncsUtlGetTable3(sdate, edate,caStime,caEtime, "ncmactermatt_if_", &lStartTime, &lTime, &lCount);
		printf("lStartTime=%d,sdate=%s\n",lStartTime,sdate);
	}
	else
	{
		ptmp = ncsUtlGetTable(lTime, days, "ncmactermatt_if_",  &lStartTime, &lCount);
		lTime=lTime+2*3600;
	}

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
			
			if(strcmp(caGroupid, "all") == 0)
			{
				printf("查询所有场所\n\n");
			}
			else
			{
				strncpy(caGroupidJudge, caKeyword, 1);
				if(strcmp(caGroupidJudge, "@") == 0)
				{
					//printf("caKeyword = %s, caGroupidJudge = %s\n\n", caKeyword, caGroupidJudge);					
					if(strlen(caGroupid)>0)
					{			
						if(strlen(caGroupcodeSql) > 0)						
							sprintf(caCont+strlen(caCont)," and %s", caGroupcodeSql);					
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
	}
	if(strlen(caDid)>0)
	{

		sprintf(caCont+strlen(caCont)," and a.servicecode='%s' ",getServicecodeByid(psShmHead,atol(caDid)));
	}
	
	/*  当前用户权限下所能查询场所*/
	if(strlen(caGroups)>0)
	{
        sprintf(caCont+strlen(caCont)," and %s", getDsGroupcodeSql("servicecode"));
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
		printf("sql3 = %s\n", sql3);
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
	printf(" lCount2 = %lu  num2 = %d  lTotRec2 = %lu\n\n", lCount2, num2, lTotRec2);
	utPltPutVarF(psDbHead,"distinctCount", "%lu", lTotRec2);
	
	
	for(i = 0; i < lCount; i++)
	{
		lCount1 = 0;
		memset(sql, 0, sizeof(sql));
		snprintf(sql, sizeof(sql), "select count(*) from %s a where a.stime >= %lu and a.stime <= %lu  ", ptmp[i], lStartTime, lTime);
		strcat(sql,caCont);
		printf("sql = %s\n", sql);
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
	printf(" lCount1 = %lu  num = %d  lTotRec = %lu\n\n", lCount1, num, lTotRec);
	utPltPutVarF(psDbHead,"TotRec", "%lu", lTotRec);

			
	for(i = num-1; i >= 0; i--)
	{
		printf("table[%d] = %s, _lCount1[%d] = %lu\n", i, tableinfo[i].tname, i, tableinfo[i].lCount);
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
	
		printf("__line = %d, sql = %s\n", __LINE__, sql);
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
					utPltPutLoopVar(psDbHead,"stime",iNum,utTimFormat("%Y-%m-%d %H:%M:%S",lStarttime));      
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
    return 0;
}

/* (未使用)场所信息查询下查看图表1：指定场所采集终端数统计(根据用户选择的时间点) */ 
int ncsPlace_Chosenplace_MacCount(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{
 //utMsgPrintMsg(psMsgHead);
    utPltDbHead *psDbHead = utPltInitDb();
    //共享内存相关变量
    unsigned char *pHash;
    char caServicecode[32] = "";
    char request_type[32] = "";
    //时间相关变量
    long lstime, lttime, letime;
    char caStemptime[24];
    char sDate[10];
    char sql[1024] = "";
    int iReturn = 0;

    iReturn = utMsgGetSomeNVar(psMsgHead, 2,
                               "servicecode",  UT_TYPE_STRING, sizeof(caServicecode) - 1, caServicecode,
                               "request_type", UT_TYPE_STRING, sizeof(request_type) - 1, request_type);

    //查询表里符合条件所有的点
    long collect_number = 0;
    if(strcmp(request_type, "day") == 0)
    {
        //1、根据当前年月生成表名
        char table_name[1024] = "";
        char  year[10];
        char  mon[10];
        memset(year, 0, sizeof(year));
        memset(mon, 0 , sizeof(mon));
        time_t now;
        time(&now);
        timeToStringPlace(now, year, mon);
        int syear = atoi(year);
        int smonth = atoi(mon);
        snprintf(table_name + strlen(table_name), 1024 - strlen(table_name), "ncmactermatt_if_%4u%02u", syear, smonth);
        //2、统计当天目前时间以前的所有的小时
        letime = time(0);
        sprintf(caStemptime, "%s 00:00", utTimFormat("%Y/%m/%d", letime));
        //printf("zero time caStemptime%s\n", caStemptime);
        //当天的0点
        lstime = utTimStrToLong("%Y/%m/%d %H:%M", caStemptime);
        //统计每个小时点
        lttime = lstime;
        int iNum = 0;
        int hour = 0;
        while(lttime < letime)
        {
            memset(sql, 0, sizeof(sql));
            snprintf(sql, sizeof(sql), "select ifnull(count(*), 0) from %s where stime>=%d and stime<=%d and servicecode like '%c%s%c'", table_name, lstime, lttime, '%', caServicecode, '%');
            printf("temp sql=[%s]\n", sql);
            iReturn = pasDbOneRecord(sql, 0, UT_TYPE_LONG, 4, &collect_number);
            if(iReturn != 0)
            {
                utPltPutVar(psDbHead, "result", "0");
                utPltOutToHtml(iFd, psMsgHead, psDbHead, "mac/infoquery/place/ChosenPlacemacCount.htm");
                return -1;
            }
            if(iNum > 1)
            {
                utPltPutLoopVar(psDbHead, "dh", iNum, ",");
            }
            iNum ++;
            utPltPutLoopVarF(psDbHead, "collect_number", iNum, "%lu", collect_number);
            utPltPutLoopVarF(psDbHead, "collect_time", iNum, "%lu", lttime);
            lstime = lttime;
            hour ++;
            memset(caStemptime, 0, sizeof(caStemptime));
            sprintf(caStemptime, "%s %02d:00", utTimFormat("%Y/%m/%d", letime), hour);
            printf("hour %d caStemptime=%s\n", hour, caStemptime);
            lttime = utTimStrToLong("%Y/%m/%d %H:%M", caStemptime);
        }
        //最后一个小时点
        memset(sql, 0, sizeof(sql));
        snprintf(sql, sizeof(sql), "select count(*) from %s where stime>=%d and stime<=%d and servicecode = \'%s\' ", table_name, lstime, letime,caServicecode );
        iReturn = pasDbOneRecord(sql, 0, UT_TYPE_LONG, 4, &collect_number);
        if(iReturn != 0)
        {
            utPltPutVar(psDbHead, "result", "0");
            utPltOutToHtml(iFd, psMsgHead, psDbHead, "mac/infoquery/place/ChosenPlacemacCount.htm");
            return -1;
        }
        utPltPutLoopVar(psDbHead, "dh", iNum, ",");
        utPltPutLoopVarF(psDbHead, "collect_number", iNum, "%lu", collect_number);
        utPltPutLoopVarF(psDbHead, "collect_time", iNum, "%lu", letime);
    }
    else if(strcmp(request_type, "week") == 0)
    {
		printf("caServicecode = %s\n", caServicecode);
        iReturn = doCalcByDaysPlace(7, caServicecode, psMsgHead, iFd, psDbHead);
        if(iReturn)
            return iReturn;
    }
    else if(strcmp(request_type, "month") == 0)
    {
        iReturn = doCalcByDaysPlace(30, caServicecode, psMsgHead, iFd, psDbHead);
        if(iReturn)
            return iReturn;
    }
    else if(strcmp(request_type, "season") == 0)
    {
        //把本季度的
        iReturn = doCalcByMonthsPlace(3, caServicecode, psMsgHead, iFd, psDbHead);
        if(iReturn)
            return iReturn;
    }
    else if(strcmp(request_type, "year") == 0)
    {
        iReturn = doCalcByMonthsPlace(12, caServicecode, psMsgHead, iFd, psDbHead);
        if(iReturn)
            return iReturn;
    }
    else
    {
        //类型不对
        utPltPutVar(psDbHead, "result", "2");//2代表类型不对
        utPltOutToHtml(iFd, psMsgHead, psDbHead, "mac/infoquery/place/ChosenPlacemacCount.htm");
        return 0;
    }
    //返回数据给前端
    utPltPutVar(psDbHead, "result", "1");
    utPltOutToHtml(iFd, psMsgHead, psDbHead, "mac/infoquery/place/ChosenPlacemacCount.htm");
    return 0;

}

/* 功能封装：将提取日期格式中的年、月 */
int timeToStringPlace(unsigned int now, char *year, char *mon)
{
    if((year == NULL) || (mon == NULL) || (now == 0))
    {
        return -1;
    }
    struct tm *time1;
    time1 = (struct tm *)localtime(&now);
    //年
    snprintf(year, 5, "%d", time1->tm_year + 1900);
    //月
    if((time1->tm_mon + 1 > 0) && (time1->tm_mon + 1 <= 9))
        snprintf(mon, 3, "0%d", time1->tm_mon + 1);
    else
        snprintf(mon, 3, "%d", time1->tm_mon + 1);
    return 0;
}

int doCalcByDaysPlace(int days, char *szServicecode, utMsgHead *psMsgHead, int iFd, utPltDbHead *psDbHead)
{
    long lstime, lttime;
    int iReturn = 0;
    long collect_number;
    char sql[1024] = "";
    char caStemptime[24];
    int i = 0;
    long letime = time(0);
    for(i = 0; i <= days; i++)
    {
        lttime = letime - 3600 * 24 * (days - i + 1);
        //统计出该天的总数
        memset(sql, 0, sizeof(sql));
        snprintf(sql, sizeof(sql), "select ifnull(sum(lcount),0) from ncmactermcount where sdate=\'%s\' and servicecode = \'%s\' ", utTimFormat("%Y%m%d", lttime), szServicecode);
        printf("sql=[%s]\n", sql);
        iReturn = pasDbOneRecord(sql, 0, UT_TYPE_LONG, 4, &collect_number);
        if(iReturn != 0)
        {
            utPltPutVar(psDbHead, "result", "0");
            utPltOutToHtml(iFd, psMsgHead, psDbHead, "mac/infoquery/place/ChosenPlacemacCount.htm");
            return -1;
        }
        //算出该天的0点时间值
        memset(caStemptime, 0, sizeof(caStemptime));
        sprintf(caStemptime, "%s 00:00", utTimFormat("%Y/%m/%d", lttime));
        printf("caStemptime=%s,leTime=%lu\n", caStemptime, letime);
        lstime = utTimStrToLong("%Y/%m/%d %H:%M", caStemptime);
        if(i > 1)
        {
            utPltPutLoopVar(psDbHead, "dh", i, ",");
        }
        utPltPutLoopVarF(psDbHead, "collect_number", i, "%lu", collect_number);
        utPltPutLoopVarF(psDbHead, "collect_time", i, "%lu", lstime);
    }
    return 0;
}

int doCalcByMonthsPlace(int months, char *szServicecode, utMsgHead *psMsgHead, int iFd, utPltDbHead *psDbHead)
{
    retDataPlace* retList = (retDataPlace*)malloc(sizeof(retDataPlace) * months);
    long letime = time(0);
    long lstime = getMonthFirstDayPlace(letime);
    int i = 0;
    long tmpCount = 0;
    for(i = 0; i < months; i++)
    {
        retList[months - i - 1].rtime = lstime;
        //统计出该时间段内所有的数据
        tmpCount = getCountInDurationPlace(lstime, letime, szServicecode);
        if(tmpCount < 0)
        {
            utPltPutVar(psDbHead, "result", "0");
            utPltOutToHtml(iFd, psMsgHead, psDbHead, "mac/infoquery/place/ChosenPlacemacCount.htm");
            return -2;
        }
        retList[months - i - 1].rcount = tmpCount;
        letime = lstime - 3600;
        lstime = getMonthFirstDayPlace(letime);
    }
    //遍历数据给返回数据赋值
    for(i = 0; i < months; i++)
    {
        if(i > 0)
        {
            utPltPutLoopVar(psDbHead, "dh", i + 1, ",");
        }
        utPltPutLoopVarF(psDbHead, "collect_number", i + 1, "%lu", retList[i].rcount);
        utPltPutLoopVarF(psDbHead, "collect_time", i + 1, "%lu", retList[i].rtime);
    }
    free(retList);
    return 0;
}

long getMonthFirstDayPlace(long lttime)
{
    char caStemptime[56] = "";
    snprintf(caStemptime, sizeof(caStemptime), "%s/%02d 00:00", utTimFormat("%Y/%m", lttime), 1);
   // printf("caStemptime=[%s]\n", caStemptime);
    return utTimStrToLong("%Y/%m/%d %H:%M", caStemptime);
}

long getCountInDurationPlace(long lstime, long letime,  char *szServicecode)
{
    char sql[1024] = "";
    int iReturn = 0;
    long collect_number = 0;
   // printf("lstime=%lu, letime=%lu\n", lstime, letime);
    memset(sql, 0, sizeof(sql));
    snprintf(sql, sizeof(sql), "select ifnull(sum(lcount),0) from ncmactermcount where sdate>=\'%s01\' and sdate<=\'%s31\' and servicecode = '%s'", utTimFormat("%Y%m", lstime), utTimFormat("%Y%m", letime), szServicecode);
   // printf("temp sql=[%s]\n", sql);
    iReturn = pasDbOneRecord(sql, 0, UT_TYPE_LONG, 4, &collect_number);
   // printf("iret = %d, n=%lu\n", iReturn, collect_number);
    if((iReturn != 0) && (iReturn != 1405))
    {
        return -1;
    }
    return collect_number;
}

/* 场所信息查询下查看图表2：场所频繁出现的终端TOP10统计(根据用户选择的时间点、场所名称或编号) */
int ncsPlace_Chosenplace_TerminalmacTop(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{			
	/* 初始化模板数据缓冲区 */
	utPltDbHead *psDbHead = utPltInitDb();
	
	/* 将数据包显示出来,本函数主要用于调试。
	显示时先显示数据包的头部，然后显示数据包的内容。*/
	utMsgPrintMsg(psMsgHead);
	
	/* 场所编号、查询时间类型(天、周、月、三月、季度、年) */	
	char caTimeFlag[10] = "";
	char caServicename[32] = "";
	int iReturn = 0;
	/* 从消息数据包 psMsgHead 中提取指定数量的变量 */
	iReturn = utMsgGetSomeNVar(psMsgHead, 2, 		
		"timevalue", UT_TYPE_STRING, sizeof(caTimeFlag) - 1, caTimeFlag,
		"servicename", UT_TYPE_STRING, sizeof(caServicename) - 1, caServicename);
		
	printf("\n");		
	printf("timevalue = %s\n", caTimeFlag);	
	printf("servicename = %s\n", caServicename);
	
	if(strlen(caServicename) <= 0)
	{
		int a = 0;
		utPltPutVarF(psDbHead, "result", "%d\n", a);			
		utPltOutToHtml(iFd, psMsgHead, psDbHead, "mac/infoquery/place/PlaceTerminalmacTop.htm");		
		return 0;
	}
		
	/* 起始时间、结束时间 */	
	/*char caStemptime[24] = "";
	long lstime, lttime, letime;	
	//查询表里符合条件所有的点
    long collect_number = 0; */
	
	//1.根据当前年月生成表名
    char table_name[1024] = "";
    char  year[10];
    char  mon[10];
    memset(year, 0, sizeof(year));
    memset(mon, 0 , sizeof(mon));
    time_t now;
    time(&now);
    timeToStringPlace(now, year, mon);
    int syear = atoi(year);
    int smonth = atoi(mon);		
    snprintf(table_name + strlen(table_name), 1024 - strlen(table_name), "ncmactermatt_if_%4u%02u", syear, smonth);
			
	//2.根据查询时间类型选定相应时间条件
	char caStartDate[32] = "";
   	char caEndDate[32] = "";
	
	char sql[4096] = "";	
	char sqlTmp[4096] = "";
	snprintf(sql, sizeof(sql), "select mac , count(stime) from %s where stime >= ", table_name);
	
	if(strcmp(caTimeFlag, "null") ==0 )		//查询今天：默认
	{
		snprintf(sqlTmp, sizeof(sqlTmp), " UNIX_TIMESTAMP(DATE_SUB(NOW(), INTERVAL 1 DAY)) ");
	}
	else if(strcmp(caTimeFlag, "1") ==0)	//查询今天：用户选择
	{
		snprintf(sqlTmp, sizeof(sqlTmp), " UNIX_TIMESTAMP(DATE_SUB(NOW(), INTERVAL 1 DAY)) ");
	}
	else if(strcmp(caTimeFlag, "2") ==0 )	//最近三天
	{
		snprintf(sqlTmp, sizeof(sqlTmp), " UNIX_TIMESTAMP(DATE_SUB(NOW(), INTERVAL 3 DAY)) ");
	}
	else if(strcmp(caTimeFlag, "3") ==0 )	//最近一周
	{
		snprintf(sqlTmp, sizeof(sqlTmp), " UNIX_TIMESTAMP(DATE_SUB(NOW(), INTERVAL 7 DAY)) ");
	}
	else if(strcmp(caTimeFlag, "4") ==0 )	//最近一月
	{
		snprintf(sqlTmp, sizeof(sqlTmp), " UNIX_TIMESTAMP(DATE_SUB(NOW(), INTERVAL 1 MONTH)) ");
	}
	else if(strcmp(caTimeFlag, "5") ==0 )	//最近三月
	{
		snprintf(sqlTmp, sizeof(sqlTmp), " UNIX_TIMESTAMP(DATE_SUB(NOW(), INTERVAL 3 MONTH)) ");
	}
	else if(strcmp(caTimeFlag, "6") ==0 )	//最近半年
	{
		snprintf(sqlTmp, sizeof(sqlTmp), " UNIX_TIMESTAMP(DATE_SUB(NOW(), INTERVAL 6 MONTH)) ");
	}
	else 	//最近半年
	{
		snprintf(sqlTmp, sizeof(sqlTmp), " UNIX_TIMESTAMP(DATE_SUB(NOW(), INTERVAL 6 MONTH)) ");
	}
	
	//printf(sqlTmp);
	printf("\n");
	
	/* 在原来的sql语句有效字符结尾处，追加上 sqlTmp */
	snprintf(sql + strlen(sql), sizeof(sql) - strlen(sql), "%s", sqlTmp);
	snprintf(sql + strlen(sql), sizeof(sql) - strlen(sql), " and stime <= (UNIX_TIMESTAMP(NOW())) and servicename like '%c%s%c' GROUP BY mac ORDER BY 2 desc limit 10 ", '%', caServicename, '%');
	
	
	/* snprintf(sql, sizeof(sql), "select mac , count(stime) from %s where stime >= UNIX_TIMESTAMP('2015-08-05 00:00:00') and stime <= (UNIX_TIMESTAMP(NOW())) and servicecode = \'%s\' GROUP BY mac ORDER BY 2 desc limit 10 ", table_name, caServiceCode); */
	printf("\n");
	printf("\n");
	printf("%s", sql);
	printf("\n");
	
	int iNum = 0, iRet = 0;
	unsigned long dwCountMax = 0;
	pasDbCursor *psCur = NULL;
	psCur = pasDbOpenSql(sql, 0);
	if(psCur != NULL)
	{
		printf("\n psCur is not null\n\n");
		char caMac[36] = "";
		unsigned long dwCount = 0;		
		int nRet = 0;
		while(0 == (nRet = pasDbFetchInto(psCur, UT_TYPE_STRING, 32, &caMac,
												 UT_TYPE_LONG, 	20, &dwCount)) || 1405 == nRet)
		{
			iNum++;
			if(iNum > 1)
			{
				utPltPutLoopVar(psDbHead, "dh", iNum, ",");
			}
			utPltPutLoopVarF(psDbHead, "mac", iNum, "%s", caMac);
			utPltPutLoopVarF(psDbHead, "count", iNum, "%lu", dwCount);
			
			if(dwCount >= dwCountMax)
			{
				dwCountMax = dwCount;				
			}	
			
			//清空容器
			memset(caMac, 0, sizeof(caMac));
			dwCount = 0;
		}
	}
	
	pasDbCloseCursor(psCur);
	utPltPutVarF(psDbHead, "result", "%d\n", iNum);
	utPltPutVarF(psDbHead, "max", "%lu\n", dwCountMax);
	printf("final iNum = %d\n", iNum);
	utPltOutToHtml(iFd, psMsgHead, psDbHead, "mac/infoquery/place/PlaceTerminalmacTop.htm");
	return 0;
}

/* 场所信息查询下查看图表3：指定场所的人流密集时段(根据用户选择的时间点、场所名称或编号) */
int ncsPlace_Chosenplace_PeopleCrowedTime(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{			
	/* 初始化模板数据缓冲区 */
	utPltDbHead *psDbHead = utPltInitDb();
	
	/* 将数据包显示出来,本函数主要用于调试。
	显示时先显示数据包的头部，然后显示数据包的内容。*/
	utMsgPrintMsg(psMsgHead);
	
	/* 场所编号、查询时间类型(天、周、月、三月、季度、年) */	
	char caTimeFlag[10] = "";
	char caServicename[32] = "";
	int iReturn = 0;
	/* 从消息数据包 psMsgHead 中提取指定数量的变量 */
	iReturn = utMsgGetSomeNVar(psMsgHead, 2, 		
		"timevalue", UT_TYPE_STRING, sizeof(caTimeFlag) - 1, caTimeFlag,
		"servicename", UT_TYPE_STRING, sizeof(caServicename) - 1, caServicename);
	
	printf("\n");	
	printf("timevalue = %s\n", caTimeFlag);	
	printf("servicename = %s\n", caServicename);
			
	//1.根据当前年月生成表名
    char table_name[1024] = "";
    char  year[10];
    char  mon[10];
    memset(year, 0, sizeof(year));
    memset(mon, 0 , sizeof(mon));
    time_t now;
    time(&now);
    timeToStringPlace(now, year, mon);
    int syear = atoi(year);
    int smonth = atoi(mon);		
    snprintf(table_name + strlen(table_name), 1024 - strlen(table_name), "ncmactermatt_if_%4u%02u", syear, smonth);
			
	//2.根据查询时间类型选定相应时间条件
	char caStartDate[32] = "";
   	char caEndDate[32] = "";
	
	char sql[4096] = "";	
	char sqlTmp[4096] = "";
	snprintf(sql, sizeof(sql), "select servicename, count(*) as '出现次数', HOUR(FROM_UNIXTIME(d.stime)) as hourtime from %s d where d.stime >= ", table_name);
	
	if(strcmp(caTimeFlag, "null") ==0 )		//查询今天：默认
	{
		snprintf(sqlTmp, sizeof(sqlTmp), " UNIX_TIMESTAMP(DATE_SUB(NOW(), INTERVAL 1 DAY)) ");
	}
	else if(strcmp(caTimeFlag, "1") ==0)	//查询今天：用户选择
	{
		snprintf(sqlTmp, sizeof(sqlTmp), " UNIX_TIMESTAMP(DATE_SUB(NOW(), INTERVAL 1 DAY)) ");
	}
	else if(strcmp(caTimeFlag, "2") ==0 )	//最近三天
	{
		snprintf(sqlTmp, sizeof(sqlTmp), " UNIX_TIMESTAMP(DATE_SUB(NOW(), INTERVAL 3 DAY)) ");
	}
	else if(strcmp(caTimeFlag, "3") ==0 )	//最近一周
	{
		snprintf(sqlTmp, sizeof(sqlTmp), " UNIX_TIMESTAMP(DATE_SUB(NOW(), INTERVAL 7 DAY)) ");
	}
	else if(strcmp(caTimeFlag, "4") ==0 )	//最近一月
	{
		snprintf(sqlTmp, sizeof(sqlTmp), " UNIX_TIMESTAMP(DATE_SUB(NOW(), INTERVAL 1 MONTH)) ");
	}
	else if(strcmp(caTimeFlag, "5") ==0 )	//最近三月
	{
		snprintf(sqlTmp, sizeof(sqlTmp), " UNIX_TIMESTAMP(DATE_SUB(NOW(), INTERVAL 3 MONTH)) ");
	}
	else if(strcmp(caTimeFlag, "6") ==0 )	//最近半年
	{
		snprintf(sqlTmp, sizeof(sqlTmp), " UNIX_TIMESTAMP(DATE_SUB(NOW(), INTERVAL 6 MONTH)) ");
	}
	else 	//最近半年
	{
		snprintf(sqlTmp, sizeof(sqlTmp), " UNIX_TIMESTAMP(DATE_SUB(NOW(), INTERVAL 6 MONTH)) ");
	}
	
	//printf(sqlTmp);
	printf("\n");
	
	/* 在原来的sql语句有效字符结尾处，追加上 sqlTmp */
	snprintf(sql + strlen(sql), sizeof(sql) - strlen(sql), "%s", sqlTmp);
	snprintf(sql + strlen(sql), sizeof(sql) - strlen(sql), " and d.stime <= (UNIX_TIMESTAMP(NOW())) and d.servicename like '%c%s%c' GROUP BY hourtime ORDER BY d.stime asc ", '%', caServicename, '%');
	
	
	printf("\n");
	printf("\n");
	printf("%s", sql);
	printf("\n");
	
	int iNum = 0, iRet = 0;
	unsigned long dwCountMax = 0;
	pasDbCursor *psCur = NULL;
	psCur = pasDbOpenSql(sql, 0);
	if(psCur != NULL)
	{
		printf("\n psCur is not null\n\n");
		char caServicename[36] = "";				
		unsigned long dwCount = 0;		
		char caHour[36] = "";	
		
		int nRet = 0;
		while(0 == (nRet = pasDbFetchInto(psCur, UT_TYPE_STRING, 32, &caServicename, 
												 UT_TYPE_LONG, 	20, &dwCount,
												 UT_TYPE_STRING, 32, &caHour)) || 1405 == nRet)
		{
			iNum++;
			if(iNum > 1)
			{
				utPltPutLoopVar(psDbHead, "dh", iNum, ",");
			}
			
			utPltPutLoopVarF(psDbHead, "servicename", iNum, "%lu", caServicename);
			utPltPutLoopVarF(psDbHead, "count", iNum, "%lu", dwCount);
			utPltPutLoopVarF(psDbHead, "hour", iNum, "%s", caHour);			
			
			if(dwCount >= dwCountMax)
			{
				dwCountMax = dwCount;				
			}	
			
			//清空容器
			memset(caServicename, 0, sizeof(caServicename));
			memset(caHour, 0, sizeof(caHour));
			dwCount = 0;
		}
	}
	
	pasDbCloseCursor(psCur);
	utPltPutVarF(psDbHead, "result", "%d\n", iNum);
	utPltPutVarF(psDbHead, "max", "%lu\n", dwCountMax);
	printf("final iNum = %d\n", iNum);
	utPltOutToHtml(iFd, psMsgHead, psDbHead, "mac/infoquery/place/ChosenPlacePeopleCrowedTime.htm");	
	return 0;	
}



/* 移动采集设备：显示终端特征信息日志 */
int ncsMobileap_mactermlist_v4(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
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
    char caStime[16];
   	char caEtime[16];
   	char caTemp_d[64];
   	char caGroupid[36],caDid[36];
    char caSdate[24],caEdate[24];
    char caApmac[36],caMac[36],caApname[64],caPosition[64];
    char caCorpname[128];
    char caCont[2048];
	char caSsid[36] = "";
    char caServicecode[16],caSearchtype[32];
	strcpy(caFacs, (const char*)getDsFacDid());
	strcpy(caGroups, (const char*)getDsGroupid()); 

	/* 选择导出的话 */
	char caExport[10] = "";
	
	/* 高级查询搜索字段 */
	char caModel[32] = "";
	char caDevid[32] = "";	
	char caPlate[32] = "";
	char caLine[32] = "";
	char caAptype[32] = "";	
   
   utPltDbHead *psDbHead = utPltInitDb();
	utMsgPrintMsg(psMsgHead);
	iReturn = utMsgGetSomeNVar(psMsgHead,27,
	    "TotPg",      UT_TYPE_STRING, 12, caTotPg,
			"CurPg",      UT_TYPE_STRING, 12, caCurPg,
			"keyword", UT_TYPE_STRING,60,caKeyword,
   	  "page",            UT_TYPE_STRING,10,caPage,
   	  "limit",           UT_TYPE_STRING,10,caLimit,
   	  "sort",            UT_TYPE_STRING, 15, caSort,
   	  "dir",          UT_TYPE_STRING, 8,  caDir,
   	  "time_flag",    UT_TYPE_STRING,8,caTime_flag,
   	  "did",          UT_TYPE_STRING,10,caDid,
   	  "groupid",      UT_TYPE_STRING,10,caGroupid,
   	  "sdate",        UT_TYPE_STRING,12,sdate,
   	  "stime",        UT_TYPE_STRING,10,caStime,
   	  "edate",        UT_TYPE_STRING,12,edate,
   	  "etime",        UT_TYPE_STRING,10,caEtime,
   	  "apmac",        UT_TYPE_STRING,20,caApmac,
   	  "mac",          UT_TYPE_STRING,20,caMac,
   	  "apname",       UT_TYPE_STRING,31,caApname,
   	  "servicecode",     UT_TYPE_STRING,15,caServicecode,
   	  "servicename",     UT_TYPE_STRING,60,caCorpname,
   	  "ssid",            UT_TYPE_STRING,30,caSsid,
	  "searchtype",      UT_TYPE_STRING,10,caSearchtype,
	  "model",       UT_TYPE_STRING,31, caModel,
	  "devid",      UT_TYPE_STRING,31, caDevid, 
	  "plate",      UT_TYPE_STRING,31, caPlate, 	  
 	  "line",       UT_TYPE_STRING,31, caLine,
	  "aptype",      UT_TYPE_STRING,31, caAptype, 
	  "mobileexport",      UT_TYPE_STRING, 9, caExport);
   	  
   	  if(strlen(caSearchtype)>0){
   	  	strcpy(caKeyword,"");
   	  }
   	  else{
   	  	strcpy(sdate,"");
   	  	strcpy(edate,"");
   	  }
   	  memset(caCont,0,sizeof(caCont));
   	  
   	  
   	    if(strlen(caKeyword)>0){
      	strcpy(caTemp_d,pasCvtGBK(2,caKeyword,caTemp,60));
      	strcpy(caKeyword,caTemp_d);
      }
   	  if(strlen(caCorpname)>0){
      	strcpy(caTemp_d,pasCvtGBK(2,caCorpname,caTemp,60));
      	strcpy(caCorpname,caTemp_d);
      }
   	  
   	 if(strlen(caApname)>0){
      	strcpy(caTemp_d,pasCvtGBK(2,caApname,caTemp,60));
      	strcpy(caApname,caTemp_d);
      }
   	 if(strlen(caSsid)>0){
      	strcpy(caTemp_d,pasCvtGBK(2,caSsid,caTemp,60));
      	strcpy(caSsid,caTemp_d);
      }
   	  
   	
   	  
   	 lCurPg = atol(caPage);
    if(lCurPg <= 0) lCurPg = 1;

 
      if(strlen(caLimit)>0){
    		lRowNum=atol(caLimit);
    	}
    	else{
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
	
	if(strlen(sdate) > 0 && strlen(edate) > 0)
	{
		ptmp = ncsUtlGetTable3(sdate, edate,caStime,caEtime, "ncmactermatt_if_", &lStartTime, &lTime, &lCount);
		printf("lStartTime=%d,sdate=%s\n",lStartTime,sdate);
	}
	else{
		ptmp = ncsUtlGetTable(lTime, days, "ncmactermatt_if_",  &lStartTime, &lCount);
		lTime=lTime+2*3600;
	}

  memset(caCont,0,sizeof(caCont));
  if(strlen(caSearchtype)>0)
  {
  	if(strlen(caMac)>0){
  		sprintf(caCont+strlen(caCont)," and a.mac like '%c%s%c' ",'%',caMac,'%');
  	}
  	if(strlen(caApmac)>0){
  		sprintf(caCont+strlen(caCont)," and a.apmac like '%c%s%c' ",'%',caApmac,'%');
  	}
  	if(strlen(caApname)>0){
  		sprintf(caCont+strlen(caCont)," and a.apname like '%c%s%c' ",'%',caApname,'%');
  	}
  	if(strlen(caServicecode)>0){
  		sprintf(caCont+strlen(caCont)," and a.servicecode like '%c%s%c' ",'%',caServicecode,'%');
  	}
  	if(strlen(caCorpname)>0){
  		sprintf(caCont+strlen(caCont)," and a.servicename like '%c%s%c' ",'%',caCorpname,'%');
  	}
	
	/* 高级查询搜索字段 */
	if(strlen(caModel)>0)	//车厢编号	
	{		
		printf(" 车厢编号 %s \n\n", caModel);
	}
	if(strlen(caDevid)>0)	//车牌号码
	{
		
		printf(" 车牌号码 %s \n\n", caDevid);
	}
	if(strlen(caPlate)>0)	//站点名称
	{
		printf(" 站点名称 %s \n\n", caPlate);
	}
	if(strlen(caLine)>0)	//线路名称
	{
		
		printf(" 线路名称 %s \n\n", caLine);
	}
	if(strlen(caAptype)>0)	//设备类型
	{
		printf(" 设备类型 %s \n\n", caAptype);
	}
	
  }
  else{
  	if(strlen(caKeyword)>0){
  		sprintf(caCont," and (a.apname like '%c%s%c' or a.apmac like '%c%s%c') ",'%',caKeyword,'%','%',caKeyword,'%');		
  	}
  }
		 if(strlen(caDid)>0){

          sprintf(caCont+strlen(caCont)," and a.servicecode='%s' ",getServicecodeByid(psShmHead,atol(caDid)));
	    }
	    if(strlen(caGroupid)>0){
	        sprintf(caCont+strlen(caCont)," and a.servicecode in (%s)",getServicecodeByGroupid(psShmHead,caGroupid));
	    }
	    if(strlen(caGroups)>0){
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
		printf("sql3 = %s\n", sql3);
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
	printf(" lCount2 = %lu  num2 = %d  lTotRec2 = %lu\n\n", lCount2, num2, lTotRec2);
	utPltPutVarF(psDbHead,"distinctCount", "%lu", lTotRec2);


	for(i = 0; i < lCount; i++)
	{

			lCount1 = 0;
			memset(sql, 0, sizeof(sql));
			snprintf(sql, sizeof(sql), "select count(*) from %s a where a.stime >= %lu and a.stime <= %lu  ",  ptmp[i], lStartTime, lTime);
		  strcat(sql,caCont);

				printf("sql = %s\n", sql);
				pasDbOneRecord(sql, 0, UT_TYPE_ULONG, 4, &lCount1);
			if(lCount1 > 0)
			{
				strcpy(tableinfo[num].tname, ptmp[i]);
				tableinfo[num].lCount = lCount1;
				num++;
			}
	//	}
		free(ptmp[i]);
	}
	free(ptmp);	
	for(i = 0; i < num; i++)
	{
		lTotRec += tableinfo[i].lCount;
	}
	utPltPutVarF(psDbHead,"TotRec", "%lu", lTotRec);
		
	for(i = num-1; i >= 0; i--)
	{
		printf("table[%d] = %s, _lCount1[%d] = %lu\n", i, tableinfo[i].tname, i, tableinfo[i].lCount);
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
		sprintf(caFilename,"mobileLog.csv");
		sprintf(caPath,"%s", "/home/ncmysql/ncs");
		sprintf(caFile,"%s/%s",caPath,caFilename);
		fp = fopen(caFile,"w");	
		if(fp == NULL) 
		{				
			printf("数据导出出错\n\n");
			return 0;
		}
		fprintf(fp, "移动信息查询\n");	//标题
		printf(" 写入移动信息查询\n\n");			
		
		ncUtlPrintCsv(fp, 15,
				UT_TYPE_STRING, "采集设备名称",
				UT_TYPE_STRING, "采集设备编号",				
                UT_TYPE_STRING, "安全厂商名称",
				UT_TYPE_STRING, "采集设备经度",
                UT_TYPE_STRING, "采集设备纬度",				
                UT_TYPE_STRING, "采集时间",				
                UT_TYPE_STRING, "终端MAC",				
                UT_TYPE_STRING, "被采集终端场强",				
				UT_TYPE_STRING, "身份类型",
                UT_TYPE_STRING, "身份内容",
                UT_TYPE_STRING, "接入热点SSID",
                UT_TYPE_STRING, "接入热点MAC",
                UT_TYPE_STRING, "接入热点频道",
                UT_TYPE_STRING, "接入热点加密类型",                
				UT_TYPE_STRING, "安全厂商组织机构代码"); 
	}
	
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
	
		printf("__line = %d, sql = %s\n", __LINE__, sql);
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
					ncUtlPrintCsv(fp, 15,
						UT_TYPE_STRING, caDispname,
						UT_TYPE_STRING, caApname,
						UT_TYPE_STRING, caFactoryname,
						UT_TYPE_STRING, caLongitude,
						UT_TYPE_STRING, caLatitude,
						UT_TYPE_LONG, lStarttime,					
						UT_TYPE_STRING, caMac,
						UT_TYPE_STRING, caRssi,						
						UT_TYPE_LONG, lVtype,
						UT_TYPE_STRING, caVname,
						UT_TYPE_STRING, caSsid,
						UT_TYPE_STRING, caApmac,
						UT_TYPE_STRING, caChannel,
						UT_TYPE_STRING, caSecurity,                                                                                       
						UT_TYPE_STRING, caFactorycode); 
				}
				else
				{
					iNum++;
					if(iNum > 1)
					{
						utPltPutLoopVar(psDbHead,"dh",iNum,",");
					}

					utPltPutLoopVar(psDbHead,"stime",iNum,utTimFormat("%Y-%m-%d %H:%M:%S",lStarttime));
      
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
         memset(caVname,0,sizeof(caVname));
		 
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
    return 0;
}

/* 移动采集设备下查看图表1：指定AP 频繁出现的终端次数(根据用户选择的时间点、采集设备名称或编号) */
int ncsMobileap_Chosenplace_TerminalmacTop(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{			
	/* 初始化模板数据缓冲区 */
	utPltDbHead *psDbHead = utPltInitDb();
	
	/* 将数据包显示出来,本函数主要用于调试。
	显示时先显示数据包的头部，然后显示数据包的内容。*/
	utMsgPrintMsg(psMsgHead);
	
	/* 场所编号、查询时间类型(天、周、月、三月、季度、年) */	
	char caTimeFlag[10] = "";
	char caApname[32] = "";
	int iReturn = 0;
	/* 从消息数据包 psMsgHead 中提取指定数量的变量 */
	iReturn = utMsgGetSomeNVar(psMsgHead, 2, 		
		"timevalue", UT_TYPE_STRING, sizeof(caTimeFlag) - 1, caTimeFlag,
		"apname", UT_TYPE_STRING, sizeof(caApname) - 1, caApname);
			
	printf("timevalue = %s\n", caTimeFlag);	
	printf("apname = %s\n", caApname);
	
	//1.根据当前年月生成表名
    char table_name[1024] = "";
    char  year[10];
    char  mon[10];
    memset(year, 0, sizeof(year));
    memset(mon, 0 , sizeof(mon));
    time_t now;
    time(&now);
    timeToStringPlace(now, year, mon);
    int syear = atoi(year);
    int smonth = atoi(mon);		
    snprintf(table_name + strlen(table_name), 1024 - strlen(table_name), "ncmactermatt_if_%4u%02u", syear, smonth);
			
	//2.根据查询时间类型选定相应时间条件
	char caStartDate[32] = "";
   	char caEndDate[32] = "";
	
	char sql[4096] = "";	
	char sqlTmp[4096] = "";
	snprintf(sql, sizeof(sql), "select mac , count(stime) from %s where stime >= ", table_name);
	
	if(strcmp(caTimeFlag, "null") ==0 )		//查询今天：默认
	{
		snprintf(sqlTmp, sizeof(sqlTmp), " UNIX_TIMESTAMP(DATE_SUB(NOW(), INTERVAL 1 DAY)) ");
	}
	else if(strcmp(caTimeFlag, "1") ==0)	//查询今天：用户选择
	{
		snprintf(sqlTmp, sizeof(sqlTmp), " UNIX_TIMESTAMP(DATE_SUB(NOW(), INTERVAL 1 DAY)) ");
	}
	else if(strcmp(caTimeFlag, "2") ==0 )	//最近三天
	{
		snprintf(sqlTmp, sizeof(sqlTmp), " UNIX_TIMESTAMP(DATE_SUB(NOW(), INTERVAL 3 DAY)) ");
	}
	else if(strcmp(caTimeFlag, "3") ==0 )	//最近一周
	{
		snprintf(sqlTmp, sizeof(sqlTmp), " UNIX_TIMESTAMP(DATE_SUB(NOW(), INTERVAL 7 DAY)) ");
	}
	else if(strcmp(caTimeFlag, "4") ==0 )	//最近一月
	{
		snprintf(sqlTmp, sizeof(sqlTmp), " UNIX_TIMESTAMP(DATE_SUB(NOW(), INTERVAL 1 MONTH)) ");
	}
	else if(strcmp(caTimeFlag, "5") ==0 )	//最近三月
	{
		snprintf(sqlTmp, sizeof(sqlTmp), " UNIX_TIMESTAMP(DATE_SUB(NOW(), INTERVAL 3 MONTH)) ");
	}
	else if(strcmp(caTimeFlag, "6") ==0 )	//最近半年
	{
		snprintf(sqlTmp, sizeof(sqlTmp), " UNIX_TIMESTAMP(DATE_SUB(NOW(), INTERVAL 6 MONTH)) ");
	}
	else 	//最近半年
	{
		snprintf(sqlTmp, sizeof(sqlTmp), " UNIX_TIMESTAMP(DATE_SUB(NOW(), INTERVAL 6 MONTH)) ");
	}
	
	//printf(sqlTmp);
	printf("\n");
	
	/* 在原来的sql语句有效字符结尾处，追加上 sqlTmp */
	snprintf(sql + strlen(sql), sizeof(sql) - strlen(sql), "%s", sqlTmp);
	snprintf(sql + strlen(sql), sizeof(sql) - strlen(sql), " and stime <= (UNIX_TIMESTAMP(NOW())) and apname like '%c%s%c' GROUP BY mac ORDER BY 2 desc limit 10 ", '%', caApname, '%');
	
	printf("\n");
	printf("\n");
	printf("%s", sql);
	printf("\n");
	
	int iNum = 0, iRet = 0;
	unsigned long dwCountMax = 0;
	pasDbCursor *psCur = NULL;
	psCur = pasDbOpenSql(sql, 0);
	if(psCur != NULL)
	{
		printf("\n psCur is not null\n\n");
		char caMac[36] = "";
		unsigned long dwCount = 0;		
		int nRet = 0;
		while(0 == (nRet = pasDbFetchInto(psCur, UT_TYPE_STRING, 32, &caMac,
												 UT_TYPE_LONG, 	20, &dwCount)) || 1405 == nRet)
		{
			iNum++;
			if(iNum > 1)
			{
				utPltPutLoopVar(psDbHead, "dh", iNum, ",");
			}
			utPltPutLoopVarF(psDbHead, "mac", iNum, "%s", caMac);
			utPltPutLoopVarF(psDbHead, "count", iNum, "%lu", dwCount);
			
			if(dwCount >= dwCountMax)
			{
				dwCountMax = dwCount;				
			}	
			
			//清空容器
			memset(caMac, 0, sizeof(caMac));
			dwCount = 0;
		}
	}
	
	pasDbCloseCursor(psCur);
	utPltPutVarF(psDbHead, "result", "%d\n", iNum);
	utPltPutVarF(psDbHead, "max", "%lu\n", dwCountMax);
	printf("final iNum = %d\n", iNum);
	utPltOutToHtml(iFd, psMsgHead, psDbHead, "mac/infoquery/mobileap/ChosenApTerminalmacTop.htm");
	return 0;		
}

/* 移动采集设备下查看图表2：指定AP的人流密集时段(根据用户选择的时间点、场所名称或编号) */
int ncsMobileap_Chosenplace_PeopleCrowedTime(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead)
{			
	/* 初始化模板数据缓冲区 */
	utPltDbHead *psDbHead = utPltInitDb();
	
	/* 将数据包显示出来,本函数主要用于调试。
	显示时先显示数据包的头部，然后显示数据包的内容。*/
	utMsgPrintMsg(psMsgHead);
	
	/* 场所编号、查询时间类型(天、周、月、三月、季度、年) */	
	char caTimeFlag[10] = "";
	char caApname[32] = "";
	int iReturn = 0;
	/* 从消息数据包 psMsgHead 中提取指定数量的变量 */
	iReturn = utMsgGetSomeNVar(psMsgHead, 2, 		
		"timevalue", UT_TYPE_STRING, sizeof(caTimeFlag) - 1, caTimeFlag,
		"apname", UT_TYPE_STRING, sizeof(caApname) - 1, caApname);
	
	printf("\n");	
	printf("timevalue = %s\n", caTimeFlag);	
	printf("apname = %s\n", caApname);
			
	//1.根据当前年月生成表名
    char table_name[1024] = "";
    char  year[10];
    char  mon[10];
    memset(year, 0, sizeof(year));
    memset(mon, 0 , sizeof(mon));
    time_t now;
    time(&now);
    timeToStringPlace(now, year, mon);
    int syear = atoi(year);
    int smonth = atoi(mon);		
    snprintf(table_name + strlen(table_name), 1024 - strlen(table_name), "ncmactermatt_if_%4u%02u", syear, smonth);
			
	//2.根据查询时间类型选定相应时间条件
	char caStartDate[32] = "";
   	char caEndDate[32] = "";
	
	char sql[4096] = "";	
	char sqlTmp[4096] = "";
	snprintf(sql, sizeof(sql), "select apname, count(*) as '出现次数', HOUR(FROM_UNIXTIME(d.stime)) as hourtime from %s d where d.stime >= ", table_name);
	
	if(strcmp(caTimeFlag, "null") ==0 )		//查询今天：默认
	{
		snprintf(sqlTmp, sizeof(sqlTmp), " UNIX_TIMESTAMP(DATE_SUB(NOW(), INTERVAL 1 DAY)) ");
	}
	else if(strcmp(caTimeFlag, "1") ==0)	//查询今天：用户选择
	{
		snprintf(sqlTmp, sizeof(sqlTmp), " UNIX_TIMESTAMP(DATE_SUB(NOW(), INTERVAL 1 DAY)) ");
	}
	else if(strcmp(caTimeFlag, "2") ==0 )	//最近三天
	{
		snprintf(sqlTmp, sizeof(sqlTmp), " UNIX_TIMESTAMP(DATE_SUB(NOW(), INTERVAL 3 DAY)) ");
	}
	else if(strcmp(caTimeFlag, "3") ==0 )	//最近一周
	{
		snprintf(sqlTmp, sizeof(sqlTmp), " UNIX_TIMESTAMP(DATE_SUB(NOW(), INTERVAL 7 DAY)) ");
	}
	else if(strcmp(caTimeFlag, "4") ==0 )	//最近一月
	{
		snprintf(sqlTmp, sizeof(sqlTmp), " UNIX_TIMESTAMP(DATE_SUB(NOW(), INTERVAL 1 MONTH)) ");
	}
	else if(strcmp(caTimeFlag, "5") ==0 )	//最近三月
	{
		snprintf(sqlTmp, sizeof(sqlTmp), " UNIX_TIMESTAMP(DATE_SUB(NOW(), INTERVAL 3 MONTH)) ");
	}
	else if(strcmp(caTimeFlag, "6") ==0 )	//最近半年
	{
		snprintf(sqlTmp, sizeof(sqlTmp), " UNIX_TIMESTAMP(DATE_SUB(NOW(), INTERVAL 6 MONTH)) ");
	}
	else 	//最近半年
	{
		snprintf(sqlTmp, sizeof(sqlTmp), " UNIX_TIMESTAMP(DATE_SUB(NOW(), INTERVAL 6 MONTH)) ");
	}
	
	//printf(sqlTmp);
	printf("\n");
	
	/* 在原来的sql语句有效字符结尾处，追加上 sqlTmp */
	snprintf(sql + strlen(sql), sizeof(sql) - strlen(sql), "%s", sqlTmp);
	snprintf(sql + strlen(sql), sizeof(sql) - strlen(sql), " and d.stime <= (UNIX_TIMESTAMP(NOW())) and d.apname like '%c%s%c' GROUP BY hourtime ORDER BY d.stime asc ", '%', caApname, '%');
	
	
	printf("\n");
	printf("\n");
	printf("%s", sql);
	printf("\n");
	
	int iNum = 0, iRet = 0;
	unsigned long dwCountMax = 0;
	pasDbCursor *psCur = NULL;
	psCur = pasDbOpenSql(sql, 0);
	if(psCur != NULL)
	{
		printf("\n psCur is not null\n\n");
		char caApname[36] = "";				
		unsigned long dwCount = 0;		
		char caHour[36] = "";	
		
		int nRet = 0;
		while(0 == (nRet = pasDbFetchInto(psCur, UT_TYPE_STRING, 32, &caApname, 
												 UT_TYPE_LONG, 	20, &dwCount,
												 UT_TYPE_STRING, 32, &caHour)) || 1405 == nRet)
		{
			iNum++;
			if(iNum > 1)
			{
				utPltPutLoopVar(psDbHead, "dh", iNum, ",");
			}
			
			utPltPutLoopVarF(psDbHead, "apname", iNum, "%lu", caApname);
			utPltPutLoopVarF(psDbHead, "count", iNum, "%lu", dwCount);
			utPltPutLoopVarF(psDbHead, "hour", iNum, "%s", caHour);			
			
			if(dwCount >= dwCountMax)
			{
				dwCountMax = dwCount;				
			}	
			
			//清空容器
			memset(caApname, 0, sizeof(caApname));
			memset(caHour, 0, sizeof(caHour));
			dwCount = 0;
		}
	}
	
	pasDbCloseCursor(psCur);
	utPltPutVarF(psDbHead, "result", "%d\n", iNum);
	utPltPutVarF(psDbHead, "max", "%lu\n", dwCountMax);
	printf("final iNum = %d\n", iNum);
	utPltOutToHtml(iFd, psMsgHead, psDbHead, "mac/infoquery/mobileap/ChosenApPeopleCrowedTime.htm");	
	return 0;	
}



/* 实时信息查询(内存读取)：从内存读取页面表格信息 */
int ncsCurrent_FromRAM(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead)
{
	/* 初始化模板数据缓冲区 */
	utPltDbHead *psDbHead = utPltInitDb();
	utMsgPrintMsg(psMsgHead);
	
	/* 左侧场所选择 groupid、username */	 /* 二次查询 mac 条件 */
	char caGroupId[32] = "";
	char caUsernamesByID[1024] = "";
	char caUsername[32] = "";
	char caMacPage[32]= "", caApmacPage[32] = "";
	
	/* 关键字查询 */
	char caKeyword[64] = "", caTemp_d[64] = "", caTemp[1024] = "";
	
	/* 选择导出的话 */
	char caExport[10] = "";
	long lRet = 0;
	lRet = utMsgGetSomeNVar(psMsgHead,4,
		"currentexport", UT_TYPE_STRING, 9, caExport,
		"groupid", UT_TYPE_STRING, 31, caGroupId,
		"username", UT_TYPE_STRING, 31, caUsername,
		"keyword", UT_TYPE_STRING, 63, caKeyword);
	
	//编码转换：转换成GBK，后台通用字符集
	if(strlen(caKeyword)>0)
	{
		strcpy(caTemp_d, pasCvtGBK(2,caKeyword,caTemp,60));
      	strcpy(caKeyword, caTemp_d);
    }	
			
	printf(" ncsCurrent_FromRAM：groupid = %s, username = %s, keyword = %s\n", caGroupId, caUsername, caKeyword);
	/* 获取所有场所下的记录数 */	
	int iServiceRecord = utShmHashRecord(psShmHead, NC_LNK_MACLOCATION);	
	printf("所有场所下实时信息数为：iServiceRecord = %d\n\n", iServiceRecord);
	
	/* 导出代码 */
	FILE *fp;
	char caFilename[32];
	char caFile[128];
	char caPath[128];		
	
	//选择导出的话
	if(strlen(caExport) > 0)
	{	
		printf(" fp caExport  = %s\n\n", caExport);
		sprintf(caFilename,"currentInfoLog.csv");
		sprintf(caPath,"%s", "/home/ncmysql/ncs");
		sprintf(caFile,"%s/%s",caPath,caFilename);
		fp = fopen(caFile,"w");	
		if(fp == NULL) 
		{				
			printf("数据导出出错\n\n");
			return 0;
		}
		fprintf(fp, "实时信息\n");	//标题
		printf(" 写入实时信息\n\n");			
		ncUtlPrintCsv(fp, 15,
				UT_TYPE_STRING, "场所名称",
				UT_TYPE_STRING, "采集设备名称",
				UT_TYPE_STRING, "采集时间",
				UT_TYPE_STRING, "最早终端场强",
				UT_TYPE_STRING, "最近终端场强",				
				UT_TYPE_STRING, "终端MAC地址",
				UT_TYPE_STRING, "采集设备经度",
                UT_TYPE_STRING, "采集设备纬度",
				UT_TYPE_STRING, "接入热点SSID",
                UT_TYPE_STRING, "接入热点MAC",                
                UT_TYPE_STRING, "接入热点加密类型",				                             				                                		
                UT_TYPE_STRING, "场所编号",
                UT_TYPE_STRING, "采集设备编号",
                UT_TYPE_STRING, "身份内容",
				UT_TYPE_STRING, "终端品牌"); 
	}	
	
	pasHashInfo sHashInfo;
    uchar *pHash;
    ncApSrvOnline *psAp;
    long lBase;
    char caSdate[32] = "", caMark[64] = "";
    char caMacStr[20] = "", caMacStr1[20] = "", caMacStr2[20] = "";
	
	//场所名称、采集设备名称、采集时间、判断采集时间是否有效
	char caServicename[32] = "", caDispname[32] = "", caStime[32] = "", caTempYear[32] = "";
	
	//最早采集场强、最近采集场强	//终端MAC、采集设备MAC
	long lRssi = 0, lUrssi = 0;								
	char caMac[32]= "", caApmac[32] = "";	
	
	//采集设备经度、纬度	//热点ssid
	char caLongitude[32] = "", caLatitude[32] = "";		
	char caSsid[32] = ""; 								
	long lSecurity = 0;									
	
	//场所编号、采集设备编号 //虚拟身份、终端品牌	
	char caServiceCode[32] = "", caApname[32] = "";		
	char caVname[32] = "", caTermtype[32] = "";	
    char caSplitServiceCode[32] = "";
	
	//返回前端40条实时信息、当前或所有场所实时信息总数、当前或所有场所有效实时信息总数	
	int iNum = 0;			
	int iNumControl = 0;	
	int iMacCount = 0;			
		
	// 用户菜单选择的 servicecodes 、用户登陆权限下可查看场所范围 servicecodes 
	char caAuthorityGroupcodes[1024] = "";	
	char caGrouidCurrent[32] = "";

	strcpy(caAuthorityGroupcodes, (const char*)getDsGroupcode()); 
		
	printf("用户权限下 caAuthorityGroupcodes = %s\n", caAuthorityGroupcodes);	
	
	int iControlTime = 0;	
	long iUserAuth = 0;						//用户权限判断
	
	stCurInfoForty pStCurrentForty[40];		//统计40条或以下实时信息并按时间排序返回前台
	memset(pStCurrentForty,0,sizeof(stCurInfoForty)*40);
	
	//如果选择左侧 groupid 获取当前groupid下所有username到 caUsernamesByID
	if(strlen(caGroupId) > 0)
	{		
		//可以获取多级菜单下所有username
		snprintf(caUsernamesByID, sizeof(caUsernamesByID)-1, "%s", myGetGroupCodeByGroupid(psShmHead, caGroupId));	
		printf("caGroupId = %s\n\n", caGroupId);
		printf("caUsernamesByID = %s\n\n", caUsernamesByID);

	}
	
    ncLbsMacLocation *psMacLoc;
    pHash =  (unsigned char *)utShmHashHead(psShmHead, NC_LNK_MACLOCATION);
    if(pHash) 
	{	
        psMacLoc = (ncLbsMacLocation *)pasHashFirst(pHash,&sHashInfo);		
		//printf("pHash = %p\n", pHash);		
        //printf("psMacLoc = %p\n", psMacLoc);		
		while(psMacLoc)
		{
			psAp = (ncApSrvOnline *)ncMacLogGetOnlineApByMac(psShmHead,psMacLoc->apmac);
            if(psAp) 
			{
				strcpy(caServicename, psAp->servicename);	//场所名称                
				strcpy(caDispname, psAp->dispname);			//采集设备名称
				strcpy(caStime, utTimFormat("%Y-%m-%d %H:%M:%S", psMacLoc->starttime));	//采集时间			
				
				strcpy(caMac, (const char*)pasCvtMacStr(psMacLoc->mac,caMacStr));		//终端MAC
				stpcpy(caLongitude, psMacLoc->longitude);
				stpcpy(caLatitude, psMacLoc->latitude);				
				stpcpy(caSsid, psMacLoc->ssid);
				strcpy(caApmac, (const char*)pasCvtMacStr(psMacLoc->apmac,caMacStr1));	//采集设备MAC												
				stpcpy(caVname, psMacLoc->name);
				stpcpy(caTermtype, psMacLoc->termtype);																              
				strcpy(caServiceCode, psAp->servicecode);	//场所编号
				strcpy(caApname, psAp->apname);				//采集设备编号				
				
			
				if((strlen(caAuthorityGroupcodes) > 0))		//非admin用户，获取用户权限下管理区域不为空
				{
					if(strlen(caServiceCode) > 0)
					{														
						iUserAuth = isInGroupCode(caServiceCode, caAuthorityGroupcodes);
						if(0 == iUserAuth)					//权限不在指定区域
						{
							psMacLoc = (ncLbsMacLocation *)pasHashNextS(&sHashInfo);
							continue;
						}
					}
				}						
				
				//截取转换后的时间日期：前四位为1970，作为垃圾数据不获取
				memset(caTempYear, 0, sizeof(caTempYear));
				strncpy(caTempYear, caStime, 4);
				
				if((strlen(caServiceCode) > 0) && (strcmp(caTempYear, "1970") != 0))
				{
					/* 选择导出的话 */
					if(strlen(caExport) > 0)
					{
						iNum++;					 
						if(iNum >= 40)
						{
							iControlTime = iNum;
						}						
						ncUtlPrintCsv(fp, 15,
							UT_TYPE_STRING, caServicename,
							UT_TYPE_STRING, caDispname,
							UT_TYPE_STRING, caStime,
							UT_TYPE_LONG, psMacLoc->lrssi,
							UT_TYPE_LONG, psMacLoc->urssi,												
							UT_TYPE_STRING, caMac,
							UT_TYPE_STRING, psMacLoc->longitude,
							UT_TYPE_STRING, psMacLoc->latitude,
							UT_TYPE_STRING, psMacLoc->ssid,
							UT_TYPE_STRING, caApmac,
							UT_TYPE_LONG, 	psMacLoc->encrypt,					
							UT_TYPE_STRING, caServiceCode,
							UT_TYPE_STRING, caApname,						
							UT_TYPE_STRING, psMacLoc->name,
							UT_TYPE_STRING, psMacLoc->termtype); 
					}
					else
					{
						//输入框内容为空时显示(iNum) 40条、所有场所 iNumControl 条、所有场所有效数据 iMacCount 条
						if(strcmp(caKeyword, "") == 0 || (strcmp(caGroupId, "all") == 0))	
						{						
							iNumControl++;		//遍历当前场所下实时信息个数
							if(strcmp(caMac, "00:00:00:00:00:00") != 0)
							{
								iMacCount++;	//遍历当前场所下有效实时信息个数，绘制跑马表数据						
							}
					
							if(iNumControl <= 40)						
							{						
								iNum++;							
								//将符合需要的40条数据插入到结构体 pStCurrentForty 
								InsertItemToStuct(pStCurrentForty, iNum - 1, psAp, psMacLoc);
							}
							if(iNumControl <= 5)
							{
								//printf("psMacLoc->starttime = %lu, caStime = %s\n\n", psMacLoc->starttime, utTimFormat("%Y-%m-%d %H:%M:%S", psMacLoc->starttime));						
							
							}
												
						}					
						else
						{						
							if((strlen(caGroupId) > 0))
							{									
								//获取当前场所编号前6位(groupcode)与groupid下所有groupcode比较
								memset(caSplitServiceCode, 0, sizeof(caSplitServiceCode));
								strncpy(caSplitServiceCode, caServiceCode, 6);															
								char *p = strstr(caUsernamesByID, caSplitServiceCode);
							
								//int iServicecode = 0;	
								//iServicecode = isInGroupCode(caUsernamesByID, caServiceCode);
								//printf("caUsernamesByID = %s, caServiceCode = %s, caSplitServiceCode = %s\n\n", caUsernamesByID, caServiceCode, caSplitServiceCode);
							
								if(p)
								{															
									iNumControl++;		//遍历当前场所下实时信息个数
									if(strcmp(caMac, "00:00:00:00:00:00") != 0)
									{
										iMacCount++;	//遍历当前场所下有效实时信息个数，绘制跑马表数据						
									}								
									//将符合需要的40条数据输出到缓冲区
									if(iNumControl <= 40)						
									{						
										iNum++;							
										InsertItemToStuct(pStCurrentForty, iNum - 1, psAp, psMacLoc);
									}
									if(iNumControl <= 5)
									{
										//printf("iNumControl <= 5 caUsernamesByID = %s, caServiceCode = %s, caSplitServiceCode = %s\n\n", caUsernamesByID, caServiceCode,caSplitServiceCode);
									}
								}
							}
							else
							{
								//用户点选择左侧菜单树下 username：(iNum) 40条、共 iNumControl 条、有效数据 iMacCount 条
								if((strcmp(caUsername, caServiceCode) == 0))
								{						
									iNumControl++;		//遍历当前场所下实时信息个数
									if(strcmp(caMac, "00:00:00:00:00:00") != 0)
									{
										iMacCount++;	//遍历当前场所下有效实时信息个数，绘制跑马表数据			
									}					
									//将符合需要的40条数据输出到缓冲区
									if(iNumControl <= 40)						
									{						
										iNum++;							
										InsertItemToStuct(pStCurrentForty, iNum - 1, psAp, psMacLoc);
									}					
								}
							}
						}		
						
					}
				}									
            }
            else 
			{
                //strcpy(caServiceCode,"is null");
				//printf("psAp = %p, caServiceCode = %s\n\n", psAp, caServiceCode);
            }
																	
			psMacLoc = (ncLbsMacLocation *)pasHashNextS(&sHashInfo);
		}
		
		int iRet = 0;
		if(strcmp(caKeyword, "") == 0)	
		{			
			printf("输入框为空时输出 JSON");
			//依据时间排序并保存回 pStCurrentForty			
			choiseSort(pStCurrentForty, iNum);
			int i;	
			for(i = 0; i < iNum; i++)
			{			
				if(i > 1)
				{	
					utPltPutLoopVar(psDbHead, "dh", i, ",");
				}	
								
				//将符合需要的40条数据输出到缓冲区
				outPutJson_current(psDbHead, pStCurrentForty, i);
			}
			iRet = 1;
			printf("当前所有场所实时数量为 = %d, 有效数量为 = %d\n\n", iNumControl, iMacCount);
		}
		printf("iRet = %d\n\n", iRet);
		if(!iRet)
		{
			printf("输入框非空时输出 JSON\n\n");			
			if((strlen(caGroupId) > 0))
			{				
				printf("输入框非空时 caGroupId 大于0 输出 JSON\n\n");
				printf("caUsername = %s, caServiceCode = %s\n", caUsername, caServiceCode);
					//依据时间排序并保存回 pStCurrentForty			
					choiseSort(pStCurrentForty, iNum);
					int i;	
					for(i = 0; i < iNum; i++)
					{			
						if(i > 1)
						{	
							utPltPutLoopVar(psDbHead, "dh", i, ",");
						}	
								
						//将符合需要的40条数据输出到缓冲区
						outPutJson_current(psDbHead, pStCurrentForty, i);
					}
					printf("caGroupId = %s 实时数量为 = %d, 有效数量为 = %d\n\n", caGroupId, iNumControl, iMacCount);
			}
			else
			{
				if((strlen(caUsername) > 0))
				{
					printf("输入框非空时 caUsername 大于0 输出 JSON\n\n");
					//依据时间排序并保存回 pStCurrentForty			
					choiseSort(pStCurrentForty, iNum);
					int i;	
					for(i = 0; i < iNum; i++)
					{			
						if(i > 1)
						{	
							utPltPutLoopVar(psDbHead, "dh", i, ",");
						}	
								
						//将符合需要的40条数据输出到缓冲区
						outPutJson_current(psDbHead, pStCurrentForty, i);
					}
					printf("当前指定场所 caUsername = %s 实时数量为 = %d, 有效数量为 = %d\n\n", caUsername, iNumControl, iMacCount);
				}
			}
		}
	}
	else
	{
		printf(" pHash is NULL\n\n");
	}
	
	/* 选择导出的话 */
	if(strlen(caExport) > 0)
	{
		fclose(fp);
        utPltFileDownload(iFd,"application/text",caPath,caFilename,caFilename);
        remove(caFile);
		return 0;
	}
	else
	{		
		utPltPutVarF(psDbHead, "result", "%d", iNumControl);
		utPltPutVarF(psDbHead, "maccount", "%d", iMacCount);									
		utPltOutToHtml(iFd, psMsgHead, psDbHead, "mac/infoquery/currentinfo/CurrentInfo.htm");	
	}
	
    return 0;	
}


//注册所写函数到系统
int procapWeb001Fun_infoquery(utShmHead *psShmHead)
{
	
	/* MAC日志查询 */ 
	pasSetTcpFunName("ncsMacLog_mactermlist_v4", ncsMacLog_mactermlist_v4, 0);
	/* MAC日志查询下特征分析1：频繁出现的时间 */
	pasSetTcpFunName("ncsMacLog_ChosenMac_TimeOftenCount", ncsMacLog_ChosenMac_TimeOftenCount, 0);
	/* MAC日志查询下特征分析2：频繁出现的场所 */
	pasSetTcpFunName("ncsMacLog_ChosenMac_PlaceOftenCount", ncsMacLog_ChosenMac_PlaceOftenCount, 0);
	/* MAC日志查询下特征分析3：频繁出现的场所类型 */
	pasSetTcpFunName("ncsMacLog_ChosenMac_PlaceTypeOftenCount", ncsMacLog_ChosenMac_PlaceTypeOftenCount, 0);
	/* MAC日志查询：身份查看 */
	pasSetTcpFunName("ncsMacLog_ChosenMac_IDCheck", ncsMacLog_ChosenMac_IDCheck, 0);
	/* MAC日志查询：获取分组信息 */
	pasSetTcpFunName("ncsMacLog_ChosenMac_GetMacGroupInfo", ncsMacLog_ChosenMac_GetMacGroupInfo, 0);
	/* MAC日志查询：加入分组 */
	pasSetTcpFunName("ncsMacLog_ChosenMac_JoinMacGroup", ncsMacLog_ChosenMac_JoinMacGroup, 0);
	/* MAC日志查询：终端mac 历史ssid列表 */
	pasSetTcpFunName("ncsMacLog_ChosenMac_DistinctSsid", ncsMacLog_ChosenMac_DistinctSsid, 0);
	
	
	/* 热点查询 */
	pasSetTcpFunName("ncsHotSpot_infolist_v4", ncsHotSpot_infolist_v4, 0);
	//标记合规、非合规
	pasSetTcpFunName("ncsHotSpot_MarkRuleorNot", ncsHotSpot_MarkRuleorNot, 0);
	pasUtlMac2VendorInit("ncoui.dat", 30000);
	
	/* 场所信息查询 */
	pasSetTcpFunName("ncsPlace_mactermlist_v4", ncsPlace_mactermlist_v4, 0);
	//(前台暂未用到)查看图表1：指定场所采集终端数统计(根据用户选择的时间点)  
	pasSetTcpFunName("ncsPlace_Chosenplace_MacCount", ncsPlace_Chosenplace_MacCount, 0);
	
	
	//查看图表2：场所频繁出现的终端TOP10统计(根据用户选择的时间点、采集设备名称或编号)
	pasSetTcpFunName("ncsPlace_Chosenplace_TerminalmacTop", ncsPlace_Chosenplace_TerminalmacTop, 0);
	//查看图表3：指定场所的人流密集时段(根据用户选择的时间点、场所名称或编号) 
	pasSetTcpFunName("ncsPlace_Chosenplace_PeopleCrowedTime", ncsPlace_Chosenplace_PeopleCrowedTime, 0);
	
	
	
	/* 移动采集设备 */
	pasSetTcpFunName("ncsMobileap_mactermlist_v4", ncsMobileap_mactermlist_v4, 0);
	//查看图表1：场所频繁出现的终端TOP10统计(根据用户选择的时间点、采集设备名称或编号)
	pasSetTcpFunName("ncsMobileap_Chosenplace_TerminalmacTop", ncsMobileap_Chosenplace_TerminalmacTop, 0);
	//查看图表2：指定场所的人流密集时段(根据用户选择的时间点、场所名称或编号)
	pasSetTcpFunName("ncsMobileap_Chosenplace_PeopleCrowedTime", ncsMobileap_Chosenplace_PeopleCrowedTime, 0);
	

	/* 实时信息 */
	pasSetTcpFunName("ncsCurrent_FromRAM", ncsCurrent_FromRAM, 0);
	
	return 0;
}







