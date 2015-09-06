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
#include "ncapi.h"
#include "ncmac.h"

#define output
//#define TEST
char* replaceAll(char* src, char oldChar, char newChar)
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

void removeAll(char* src, char ch)
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
int dm_list(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead)
{
    utPltDbHead *psDbHead;
    int iReturn = 0 ;
    char sql[1024];
    pasDbCursor *psCur;
    psDbHead = utPltInitDb();
    //utMsgPrintMsg(psMsgHead);
    char caCid[10], camac[32], catime[32], caservicecode[32], caservicename[32];
    long caTime;
    int iNum = 0;


    iReturn = utMsgGetSomeNVar(psMsgHead, 1, "cid", UT_TYPE_STRING, 10, caCid);
    //printf("cid = %s\n", caCid);
  
    memset(sql, 0, sizeof(sql));
    sprintf(sql, "select mac, stime, servicecode, servicename from  nctermtasklog where cid = '%s' order by stime desc", caCid);
    psCur = pasDbOpenSql(sql, 0);
	printf("sql=%s\n",sql);

    if(psCur == NULL)
    {
		pasDbCloseCursor(psCur);
        printf("cursor error\n");
    }
    else{
    iReturn = 0;
    iNum = 0;
    while((iReturn == 0 || iReturn == 1405))
    {

       caTime = 0;
        memset(catime, 0, sizeof(catime));
        memset(caservicecode, 0, sizeof(caservicecode));
        memset(camac, 0, sizeof(camac));
        memset(caservicename, 0, sizeof(caservicename));

        iReturn = pasDbFetchInto(psCur,                            
                                 UT_TYPE_STRING, 30, camac,                              
                                 UT_TYPE_LONG, 4, &caTime,                                
                                 UT_TYPE_STRING, 30, caservicecode,                                
                                 UT_TYPE_STRING, 32, caservicename
                                );


        if((iReturn != 0) && (iReturn != 1405)) break;
        iNum++;
      
        //strcpy(catime, utTimFormat("%Y/%m/%d %H:%M:%S", caTime));
        strcpy(catime, utTimFormat("%Y/%m/%d", caTime));
       
            if(iNum > 1)
            {
                utPltPutLoopVar(psDbHead, "dh", iNum, ",");
            }        
            utPltPutLoopVar(psDbHead, "mac", iNum, camac);
            utPltPutLoopVar(psDbHead, "stime", iNum, catime);
            utPltPutLoopVar(psDbHead, "servicecode", iNum, caservicecode);
            utPltPutLoopVar(psDbHead, "servicename", iNum, caservicename);
           
    }


    
  }
    pasDbCloseCursor(psCur);
   

    utPltOutToHtml(iFd, psMsgHead, psDbHead, "mac/datamining/bab/list.htm");
    return 0;
}
/*dm*/
int dm_bab(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead)
{
    utPltDbHead *psDbHead;
    int iReturn = 0 ;
    long caresult = 0;

    int camaccount = 0;
    char iMac[32], mac[32];
    long cid, groupid;
    char taskname[64];
    char taskdesc[255];
    char dids[1024];
    long maccount;
    char mac_count[32];
    long servcount;
    char macgroups[255];
    char opname[32];
    long optime;
    long modtime;
    char serv_count[32];
    char caExp[32];
    long reporttime;
    long updatetime;
    long result;
    long starttime, endtime, lCount,reportcount;
	char  reportcounts[10];
    char iTime[32];
    char istyle[32];
    char iStartTime[32];
    char iEndTime[32];
    char longitude[32], latitude[32];
    char sql[1024];
    long iCount = 0;
    int iNum = 0, iStyle = 0;
    char add[32];
    char statway[32];
    char keyword[32];
    int stat;
    char caCheck[32];
    char caStime[32] , caEtime[32];
    long lLimit = 40;
    char s_time[32];
    char time_flag[32];
    long  iTime_flag, days;
    pasDbCursor *psCur;
    char mytaskname[32];
    char mytaskdesc[255];
    char caPage[11], caLimit[11];
    int capage, calimit;
    char caUpdate[12], caDel[12];
    int caupdate = 2;
    char caCid[1024];
    char caTemp[1024];
    char temp[20];
    FILE *fp = NULL;
    int castart = 0;
    char caTemp_d[1024];
    long caservcount;
    char caFilename[100], caFile[40], caPath[40];
    psDbHead = utPltInitDb();
    //utMsgPrintMsg(psMsgHead);
    char caMac[32];
    char caEdit[12];
    char cadids[1024];
    char caUsername[32];
    long lId;


    iReturn = utMsgGetSomeNVar(psMsgHead, 20,
                               "limit", UT_TYPE_STRING, 10, caLimit,
                               "page", UT_TYPE_STRING, 10, caPage,
                               "keyword", UT_TYPE_STRING, 30, iMac,                              
                               "statway", UT_TYPE_STRING, 30, statway,
                               "mac", UT_TYPE_STRING, 30, mac,
                               "taskname", UT_TYPE_STRING, 60, mytaskname,
                               "taskdesc", UT_TYPE_STRING, 254, mytaskdesc,
                               "maccount", UT_TYPE_STRING, 30, mac_count,
                               "servcount", UT_TYPE_STRING, 30, serv_count,
                               "dids", UT_TYPE_STRING, 1023, dids,
                               "starttime", UT_TYPE_STRING, 30, iStartTime,
                               "endtime", UT_TYPE_STRING, 30, iEndTime,
                               "mytime",  UT_TYPE_STRING, 30, iTime,
                               "statway", UT_TYPE_STRING, 30, statway,
                               "exp", UT_TYPE_STRING, 30, caExp,
                               "check", UT_TYPE_STRING, 30, caCheck,
                               "del", UT_TYPE_STRING, 10, caDel,
                               "update", UT_TYPE_STRING, 10, caUpdate,
                               "cid", UT_TYPE_STRING, 1023, caCid,
                               "edit", UT_TYPE_STRING, 10, caEdit

                              );
    //printf("cid = %s\n", caCid);
    calimit = atoi(caLimit);
    capage = atoi(caPage);
    iStyle = atoi(statway);
    caupdate = atoi(caUpdate);
    //printf("%s\n", caExp);
    maccount = atoi(mac_count);
    servcount= atoi(serv_count);
		cid = atoi(caCid);
    iReturn = dsCltGetMyInfo(2, "Userid", &lId,
                             "Username", caUsername);	 
  
    printf("username = %s, id = %d \n", caUsername, lId); 

   

    //更新
    if(strlen(caEdit) > 0)
    {
    	if(cid>0){
        if(strlen(mytaskname) > 0)
        {
            strcpy(caTemp_d,  pasCvtGBK(2, mytaskname, caTemp, 63));
            strcpy(mytaskname, caTemp_d);
        }
        if(strlen(mytaskdesc) > 0)
        {
            strcpy(caTemp_d,  pasCvtGBK(2, mytaskdesc, caTemp, 63));
            strcpy(mytaskdesc, caTemp_d);
        }
        if(strlen(dids) > 0)
        {
            strcpy(caTemp_d,  pasCvtGBK(2, dids, caTemp, 63));
            strcpy(dids, caTemp_d);
				if(strstr(dids,"all"))
			{
				memset(dids, 0, sizeof(dids));
				strcpy(dids,"0");
			}
        }
        memset(sql, 0, sizeof(sql));
        sprintf(sql, "update nctermtask set taskname='%s' , taskdesc='%s' ,mac ='%s', starttime=%s, endtime=%s,servcount=%d, maccount=%d,dids='%s', modtime=%d, opname='%s' where cid in (%s) ", mytaskname, mytaskdesc, mac, iStartTime, iEndTime, servcount, maccount, dids, time(0) , caUsername,caCid);
        iReturn = pasDbExecSqlF(sql);
        printf("sql = %s  iReturn=%d", sql, iReturn);
      }
      else
      	{
      		if(strlen(mytaskname) > 0)
        {
            strcpy(caTemp_d,  pasCvtGBK(2, mytaskname, caTemp, 63));
            strcpy(mytaskname, caTemp_d);
        }
        if(strlen(mytaskdesc) > 0)
        {
            strcpy(caTemp_d,  pasCvtGBK(2, mytaskdesc, caTemp, 63));
            strcpy(mytaskdesc, caTemp_d);
        }
         if(strlen(dids) > 0)
        {
            strcpy(caTemp_d,  pasCvtGBK(2, dids, caTemp, 63));
            strcpy(dids, caTemp_d);
			if(strstr(dids,"all"))
			{
				memset(dids, 0, sizeof(dids));
				strcpy(dids,"0");
			}
        }
        memset(sql, 0, sizeof(sql));
        sprintf(sql, "insert into nctermtask (taskname, result, taskdesc,mac, starttime, endtime,servcount, maccount,dids,optime, opname, updatetime) value('%s','0','%s','%s',%s,%s,%d,%d,'%s', %d, '%s',%d)", mytaskname, mytaskdesc, mac, iStartTime, iEndTime, servcount, maccount, dids, time(0),caUsername ,time(0));
        pasDbExecSqlF(sql);
        printf("sql = %s", sql);
      	}
    }
    //更新状态
    if(strlen(caUpdate) > 0)
    {

        memset(sql, 0, sizeof(sql));
        sprintf(sql, "update nctermtask set result = %d,modtime=%d, updatetime=%d, opname='%s' where cid in (%s) and result != 2", caupdate,time(0), time(0),caUsername, caCid);
        pasDbExecSqlF(sql);
       printf("sql = %s", sql);
    }
    //删除
    if(strlen(caDel) > 0)
    {
       //printf("cadel = %s", caDel);
       //printf("cid = %s\n", caCid);
        memset(sql, 0, sizeof(sql));
        sprintf(sql, "delete from nctermtask where cid in (%s)", caCid);
        pasDbExecSqlF(sql);
       printf("sql = %s", sql);
    }

    if(strlen(caExp) <= 0)
    {
        memset(sql, 0, sizeof(sql));
        sprintf(sql, "select count(cid) from  nctermtask where 1=1 ");
        if(strlen(statway) > 0)
        {
            if(iStyle != 3)
                sprintf(sql + strlen(sql), " and  result=%ld ", iStyle);
        }
        if(strlen(iMac) != 0)
        {
           // printf("name = %s\n", iMac);
            sprintf(sql + strlen(sql), "and taskname like '%%%s%%'", iMac);

        }


        printf("sql = %s\n", sql);
        iReturn = pasDbOneRecord(sql, 0, UT_TYPE_LONG, 4, &iCount);
        //printf("iCount = %d\n", iCount);
        utPltPutVarF(psDbHead, "TotRec", "%d", iCount);
    }




    memset(sql, 0, sizeof(sql));
    sprintf(sql, "SELECT a.cid, a.taskname, a.starttime, a.endtime,  a.taskdesc,a.maccount,a.result, a.reoporttime, a.mac, a.servcount ,a.dids ,b.cc FROM  nctermtask a  LEFT JOIN  (SELECT COUNT(d.cid) cc, d.cid FROM nctermtasklog d group by d.cid ) b  ON  a.cid =b.cid  where 1=1  ");
    if(strlen(statway) > 0)
    {
        if(iStyle != 3)
            sprintf(sql + strlen(sql), " and  result=%ld ", iStyle);
    }
    if(strlen(iMac) != 0)
    {
       // printf("mac = %s\n", iMac);
        sprintf(sql + strlen(sql), "and taskname like '%%%s%%'", iMac);

    }
    if(strlen(caExp) <= 0)
    {
        castart = (capage - 1) * calimit;
        calimit = calimit < (iCount - castart) ? (capage * calimit) : iCount;
        sprintf(sql + strlen(sql), " order by cid desc limit %d, 40 ", castart);
    }
    if(strlen(caExp) > 0)
    {

        sprintf(caFilename, "sus%d.csv", utStrGetId());

        sprintf(caPath, "%s/html", "/home/ncmysql/ncs");
        sprintf(caFile, "%s/%s", caPath, caFilename);

        fp = fopen(caFile, "w");
        if(fp == NULL)
        {
            printf("fp error \n");
            return 0;
        }
        //printf("hhas\n");
        fprintf(fp, "伴随行为分析\n");
        ncUtlPrintCsv(fp, 5,
                      UT_TYPE_STRING, "任务名称",
                      UT_TYPE_STRING, "任务状态",
                      UT_TYPE_STRING, "任务描述",
                      UT_TYPE_STRING, "产生记录数",
                      UT_TYPE_STRING, "最新报表产生时间");
    }
    printf("sql = %s\n", sql);


    psCur = pasDbOpenSql(sql, 0);

    if(psCur == NULL)
    {
		pasDbCloseCursor(psCur);
        printf("cursor error\n");
    }
    else{
    iReturn = 0;

    iNum = 0;
    while((iReturn == 0 || iReturn == 1405))
    {

        cid = 0;
        camaccount = 0;
        caresult = 0;
        reporttime = 0;
        memset(taskname, 0, sizeof(taskname));
        memset(s_time, 0, sizeof(s_time));
        memset(taskdesc, 0, sizeof(taskdesc));
		 memset(reportcounts, 0, sizeof(reportcounts));
        memset(cadids, 0, sizeof(dids));

        iReturn = pasDbFetchInto(psCur,
                                 UT_TYPE_LONG, 4, &cid,
                                 UT_TYPE_STRING, 30, taskname,
								  UT_TYPE_LONG, 4, &starttime,
								   UT_TYPE_LONG, 4, &endtime,
                                 UT_TYPE_STRING, 254, taskdesc,
                                 UT_TYPE_LONG, 4, &camaccount,
                                 UT_TYPE_LONG, 4, &caresult,
                                 UT_TYPE_LONG, 4, &reporttime,
                                 UT_TYPE_STRING, 30, caMac,
                                 UT_TYPE_LONG, 4, &caservcount,
                                 UT_TYPE_STRING, 1023, cadids,
								 UT_TYPE_STRING, 8, reportcounts

                                );


        if((iReturn != 0) && (iReturn != 1405)) break;
        iNum++;
        if(caresult == 0)strcpy(temp, "生效");
        else if(caresult == 1)strcpy(temp, "失效");
        else if(caresult == 2)strcpy(temp, "完成");
        strcpy(s_time, utTimFormat("%Y/%m/%d %H:%M:%S", reporttime));
        if(strlen(caExp) > 0)
        {

            ncUtlPrintCsv(fp, 5, UT_TYPE_STRING, taskname,
                          UT_TYPE_STRING, temp,
                          UT_TYPE_STRING, taskdesc,
                          UT_TYPE_LONG, camaccount,
                          UT_TYPE_STRING, s_time);
        }
        else
        {
            if(iNum > 1)
            {
                utPltPutLoopVar(psDbHead, "dh", iNum, ",");
            }
			reportcount = atoi(reportcounts);
            utPltPutLoopVarF(psDbHead, "cid", iNum, "%ld", cid);
            utPltPutLoopVar(psDbHead, "taskname", iNum, taskname);
			  utPltPutLoopVarF(psDbHead, "starttime", iNum, "%ld", starttime);
			    utPltPutLoopVarF(psDbHead, "endtime", iNum, "%ld", endtime);
            utPltPutLoopVar(psDbHead, "taskdesc", iNum, taskdesc);
            utPltPutLoopVarF(psDbHead, "maccount", iNum, "%ld", camaccount);
            utPltPutLoopVarF(psDbHead, "result", iNum, "%ld", caresult);
            utPltPutLoopVarF(psDbHead, "reporttime", iNum, "%ld", reporttime);
            utPltPutLoopVar(psDbHead, "mac", iNum, caMac);
            utPltPutLoopVarF(psDbHead, "servcount", iNum, "%ld", caservcount);
            utPltPutLoopVar(psDbHead, "dids", iNum, cadids);
			utPltPutLoopVarF(psDbHead, "reportcount", iNum, "%ld", reportcount);
        }

    }
  }
    pasDbCloseCursor(psCur);
    if(strlen(caExp) > 0)
    {
        fclose(fp);
        utPltFileDownload(iFd, "application/text", caPath, caFilename, caFilename);
        remove(caFile);
        utPltFreeDb(psDbHead);
        return 0;
    }

    utPltOutToHtml(iFd, psMsgHead, psDbHead, "mac/datamining/bab/bab.htm");
    return 0;
}
int dm_sample(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead)
{
    utPltDbHead *psDbHead;
    int iReturn = 0 ;
    long caresult = 0;

    int camaccount = 0;
    char iMac[32], mac[32];
    long cid, groupid;
    char taskname[64];
    char taskdesc[255];
    char dids[255];
    long maccount;
    char mac_count[32];
    long servcount;
    char macgroups[255];
    char opname[32];
    long optime;
    long modtime;
    char serv_count[32];
    char caExp[32];
    long reporttime;
    long updatetime;
    long result;
    long starttime, endtime, lCount;
    char iTime[32];
    char istyle[32];
    char iStartTime[32];
    char iEndTime[32];
    char longitude[32], latitude[32];
    char sql[1024];
    long iCount = 0;
    int iNum = 0, iStyle = 0;
    char add[32];
    char statway[32];
    char keyword[32];
    int stat;
    char caCheck[32];
    char caStime[32] , caEtime[32];
    long lLimit = 40;
    char s_time[32];
    char time_flag[32];
    long  iTime_flag, days;
    pasDbCursor *psCur;
    char mytaskname[32];
    char mytaskdesc[255];
    char caPage[11], caLimit[11];
    int capage, calimit;
    char caUpdate[12], caDel[12];
    int caupdate = 2;
    char caCid[1024];
    char caTemp[1024];
    char temp[20];
    FILE *fp = NULL;
    int castart = 0;
    char caTemp_d[102];
    long caservcount;
    char caFilename[100], caFile[40], caPath[40];
    psDbHead = utPltInitDb();
    utMsgPrintMsg(psMsgHead);
    char caMac[32];
    char caEdit[12];
    


    iReturn = utMsgGetSomeNVar(psMsgHead, 21,
                               "limit", UT_TYPE_STRING, 10, caLimit,
                               "page", UT_TYPE_STRING, 10, caPage,
                               "keyword", UT_TYPE_STRING, 30, iMac,
                               "macgroups", UT_TYPE_STRING, 30, macgroups,
                               "statway", UT_TYPE_STRING, 30, statway,
                               "mac", UT_TYPE_STRING, 30, mac,
                               "taskname", UT_TYPE_STRING, 60, mytaskname,
                               "taskdesc", UT_TYPE_STRING, 254, mytaskdesc,
                               "maccount", UT_TYPE_STRING, 30, mac_count,
                               "servcount", UT_TYPE_STRING, 30, serv_count,
                               "dids", UT_TYPE_STRING, 30, dids,
                               "starttime", UT_TYPE_STRING, 30, iStartTime,
                               "endtime", UT_TYPE_STRING, 30, iEndTime,
                               "mytime",  UT_TYPE_STRING, 30, iTime,
                               "statway", UT_TYPE_STRING, 30, statway,
                               "exp", UT_TYPE_STRING, 30, caExp,
                               "check", UT_TYPE_STRING, 30, caCheck,
                               "del", UT_TYPE_STRING, 10, caDel,
                               "update", UT_TYPE_STRING, 10, caUpdate,
                               "cid", UT_TYPE_STRING, 1023, caCid,
                               "edit", UT_TYPE_STRING, 10, caEdit

                              );
   // printf("cid = %s\n", caCid);
    calimit = atoi(caLimit);
    capage = atoi(caPage);
    iStyle = atoi(statway);
    caupdate = atoi(caUpdate);
    //printf("%s\n", caExp);
    servcount=atoi(serv_count);
    maccount=atoi(mac_count);
    cid = atoi(caCid);
		


  
      
   
    //更新
    if(strlen(caEdit) > 0)
    {
    	 if(cid==0)
    	 	{
    	 		  if(strlen(mytaskname) > 0)
        {
            strcpy(caTemp_d,  pasCvtGBK(2, mytaskname, caTemp, 63));
            strcpy(mytaskname, caTemp_d);
        }
        if(strlen(mytaskdesc) > 0)
        {
            strcpy(caTemp_d,  pasCvtGBK(2, mytaskdesc, caTemp, 63));
            strcpy(mytaskdesc, caTemp_d);
        }
        memset(sql, 0, sizeof(sql));
        sprintf(sql, "insert into nctermtask (taskname, result, taskdesc,macgroups) value('%s','0','%s','%s')", mytaskname, mytaskdesc, macgroups);
        pasDbExecSqlF(sql);
        //printf("sql = %s", sql);
    	 	}
    	 	else 
    	 	{
    	 		 if(strlen(mytaskname) > 0)
        {
            strcpy(caTemp_d,  pasCvtGBK(2, mytaskname, caTemp, 63));
            strcpy(mytaskname, caTemp_d);
        }
        if(strlen(mytaskdesc) > 0)
        {
            strcpy(caTemp_d,  pasCvtGBK(2, mytaskdesc, caTemp, 63));
            strcpy(mytaskdesc, caTemp_d);
        }
        memset(sql, 0, sizeof(sql));
        sprintf(sql, "update nctermtask set taskname='%s' , taskdesc='%s' ,macgroups='%s' where cid in (%s) ", mytaskname, mytaskdesc, macgroups,caCid);
        iReturn = pasDbExecSqlF(sql);
        //printf("sql = %s  iReturn=%d", sql, iReturn);
    	 	}
       
    }
    //更新状态
    if(strlen(caUpdate) > 0)
    {

        memset(sql, 0, sizeof(sql));
        sprintf(sql, "update nctermtask set result = %d where cid in (%s) and result != 2", caupdate, caCid);
        pasDbExecSqlF(sql);
        //printf("sql = %s", sql);
    }
    //删除
    if(strlen(caDel) > 0)
    {
       //printf("cadel = %s", caDel);
       //printf("cid = %s\n", caCid);
        memset(sql, 0, sizeof(sql));
        sprintf(sql, "delete from nctermtask where cid in (%s)", caCid);
        pasDbExecSqlF(sql);
       //printf("sql = %s", sql);
    }

    if(strlen(caExp) <= 0)
    {
        memset(sql, 0, sizeof(sql));
        sprintf(sql, "select count(cid) from  nctermtask where 1=1 ");
        if(strlen(statway) > 0)
        {
            if(iStyle != 3)
                sprintf(sql + strlen(sql), " and  result=%ld ", iStyle);
        }
        if(strlen(iMac) != 0)
        {
           //printf("name = %s\n", iMac);
            sprintf(sql + strlen(sql), "and taskname like '%%%s%%'", iMac);

        }


        //printf("sql = %s\n", sql);
        iReturn = pasDbOneRecord(sql, 0, UT_TYPE_LONG, 4, &iCount);
      //  printf("iCount = %d\n", iCount);
        utPltPutVarF(psDbHead, "TotRec", "%d", iCount);
    }




    memset(sql, 0, sizeof(sql));
    sprintf(sql, "select cid, taskname, taskdesc,maccount,result, reoporttime, mac, servcount from  nctermtask where 1=1 ");
    if(strlen(statway) > 0)
    {
        if(iStyle != 3)
            sprintf(sql + strlen(sql), " and  result=%ld ", iStyle);
    }
    if(strlen(iMac) != 0)
    {
       //printf("mac = %s\n", iMac);
        sprintf(sql + strlen(sql), "and taskname like '%%%s%%'", iMac);

    }
    if(strlen(caExp) <= 0)
    {
        castart = (capage - 1) * calimit;
        calimit = calimit < (iCount - castart) ? (capage * calimit) : iCount;
        sprintf(sql + strlen(sql), " order by cid desc limit %d, %d ", castart, calimit);
    }
    if(strlen(caExp) > 0)
    {

        sprintf(caFilename, "sus%d.csv", utStrGetId());

        sprintf(caPath, "%s/html", "/home/ncmysql/ncs");
        sprintf(caFile, "%s/%s", caPath, caFilename);

        fp = fopen(caFile, "w");
        if(fp == NULL)
        {
            printf("fp error \n");
            return 0;
        }
        //printf("hhas\n");
        fprintf(fp, "抽样分析\n");
        ncUtlPrintCsv(fp, 5,
                      UT_TYPE_STRING, "任务名称",
                      UT_TYPE_STRING, "任务状态",
                      UT_TYPE_STRING, "任务描述",
                      UT_TYPE_STRING, "产生记录数",
                      UT_TYPE_STRING, "最新报表产生时间");
    }
   // printf("sql = %s\n", sql);


    psCur = pasDbOpenSql(sql, 0);

    if(psCur == NULL)
    {
		pasDbCloseCursor(psCur);
        printf("cursor error\n");
    }
    else{
    iReturn = 0;

    iNum = 0;
    while((iReturn == 0 || iReturn == 1405))
    {

        cid = 0;
        camaccount = 0;
        caresult = 0;
        reporttime = 0;
        memset(taskname, 0, sizeof(taskname));
        memset(s_time, 0, sizeof(s_time));
        memset(taskdesc, 0, sizeof(taskdesc));

        iReturn = pasDbFetchInto(psCur,
                                 UT_TYPE_LONG, 4, &cid,
                                 UT_TYPE_STRING, 30, taskname,
                                 UT_TYPE_STRING, 254, taskdesc,
                                 UT_TYPE_LONG, 4, &camaccount,
                                 UT_TYPE_LONG, 4, &caresult,
                                 UT_TYPE_LONG, 4, &reporttime,
                                 UT_TYPE_STRING, 30, caMac,
                                 UT_TYPE_LONG, 4, caservcount

                                );


        if((iReturn != 0) && (iReturn != 1405)) break;
        iNum++;


        if(caresult == 0)strcpy(temp, "生效");
        else if(caresult == 1)strcpy(temp, "失效");
        else if(caresult == 2)strcpy(temp, "完成");
        strcpy(s_time, utTimFormat("%Y/%m/%d %H:%M:%S", reporttime));
        if(strlen(caExp) > 0)
        {

            ncUtlPrintCsv(fp, 5, UT_TYPE_STRING, taskname,
                          UT_TYPE_STRING, temp,
                          UT_TYPE_STRING, taskdesc,
                          UT_TYPE_LONG, camaccount,
                          UT_TYPE_STRING, s_time);
        }
        else
        {
            if(iNum > 1)
            {
                utPltPutLoopVar(psDbHead, "dh", iNum, ",");
            }

            utPltPutLoopVarF(psDbHead, "cid", iNum, "%ld", cid);
            utPltPutLoopVar(psDbHead, "taskname", iNum, taskname);
            utPltPutLoopVar(psDbHead, "taskdesc", iNum, taskdesc);
            utPltPutLoopVarF(psDbHead, "maccount", iNum, "%ld", camaccount);
            utPltPutLoopVarF(psDbHead, "result", iNum, "%ld", caresult);
            utPltPutLoopVar(psDbHead, "reporttime", iNum, s_time);
            utPltPutLoopVar(psDbHead, "mac", iNum, caMac);
            utPltPutLoopVarF(psDbHead, "servcount", iNum, "%ld", caservcount);
        }

    }
  }
    pasDbCloseCursor(psCur);
    if(strlen(caExp) > 0)
    {
        fclose(fp);
        utPltFileDownload(iFd, "application/text", caPath, caFilename, caFilename);
        remove(caFile);
        utPltFreeDb(psDbHead);
        return 0;
    }

    utPltOutToHtml(iFd, psMsgHead, psDbHead, "mac/datamining/sample/sample.htm");
    return 0;
}
typedef struct  counttrack{
	char key[25];
};
typedef struct trackdata{
	char servicecode[15];
	char servicename[32];
	int stime[2];
};

int dm_track(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead)
{
    utPltDbHead *psDbHead;
    int iReturn = 0 ;
    char iMac[32], mac[32];
    char address[32];
    char iTime[32];
    char istyle[32];
    char iStartTime[32];
    char iEndTime[32];
    char longitude[32], latitude[32];
    char sql[1024];
    char caExp[10];
    char time_flag[11];
    char s_time[32];
    long lTime, lstime, letime;
    long castime = 0;
	char castimes[32];
    long iCount = 0, tcount = 0;
    char caTime_flag[10];
    int iNum = 0, iStyle;
    FILE *fp = NULL;
    char caSdate_d[20], caEdate_d[20], caEtime[16], caStemptime[32] , caEtemptime[32];
    char **ptmp = NULL;
    pasDbCursor *psCur;
    char tmpc[100];

    char caFilename[128], caFile[128], caPath[128];
	char caServiceName[32], caServiceNameb[32], caServiceCode[15];
	char result[32];
	int flag, iflag=0;
	long  etimetmp=0, stimetmp =0;
	struct trackdata datas[1024], datadeal[1024];


    psDbHead = utPltInitDb();
    utMsgPrintMsg(psMsgHead);
    iReturn = utMsgGetSomeNVar(psMsgHead, 6,
                               "mac", UT_TYPE_STRING, 30, iMac,
                               "starttime", UT_TYPE_STRING, 30, iStartTime,
                               "endtime", UT_TYPE_STRING, 30, iEndTime,
                               "exp",  UT_TYPE_STRING, 30, caExp,
                               "time_flag", UT_TYPE_STRING, 10, caTime_flag,
                               "styles", UT_TYPE_STRING, 30, istyle
                              );


    iStyle = atoi(istyle);
   //printf("istyle = %s\n", istyle);
    lTime = time(0);
   //printf("time = %d\n", lTime);

    if(atol(caTime_flag) == 0)
    {
        sprintf(caStemptime, "%s 00:00", utTimFormat("%Y/%m/%d", lTime));
        lstime = utTimStrToLong("%Y/%m/%d %H:%M", caStemptime);
        letime = lTime;
    }
    else if(atol(caTime_flag) == 1)
    {
        lstime = lTime - 3 * 3600 * 24;
        letime = lTime;
    }
    else if(atol(caTime_flag) == 2)
    {
        lstime = lTime - 7 * 3600 * 24;
        letime = lTime;
    }
    else if(atol(caTime_flag) == 3)
    {
        lstime = lTime - 30 * 3600 * 24;
        letime = lTime;
    }
	   else if(atol(caTime_flag) == 4)
    {
        lstime = lTime - 3*30 * 3600 * 24;
        letime = lTime;
    }
	   else if(atol(caTime_flag) == 5)
    {
        lstime = lTime - 6*30 * 3600 * 24;
        letime = lTime;
    }
    else
    {
        ncUtlMonthDate(lTime, caSdate_d, caEdate_d);
        sprintf(caStemptime, "%s 00:00", caSdate_d);
        lstime = utTimStrToLong("%Y/%m/%d %H:%M", caStemptime);
        sprintf(caEtemptime, "%s 23:59", caEdate_d);
        letime = utTimStrToLong("%Y/%m/%d %H:%M", caEtemptime);
    }

    int minYear = atoi(utTimFormat("%Y", lstime));
    int maxYear = atoi(utTimFormat("%Y", letime));
    int minMon = atoi(utTimFormat("%m", lstime));
    int maxMon = atoi(utTimFormat("%m", letime));
    int max = maxYear * 12 + maxMon;
    int min = minYear * 12 + minMon;
    int i;

    if(strlen(caExp) > 0)
    {

        sprintf(caFilename, "track%d.csv", utStrGetId());

        sprintf(caPath, "%s/html", "/home/ncmysql/ncs");
        sprintf(caFile, "%s/%s", caPath, caFilename);

        fp = fopen(caFile, "w");
        if(fp == NULL)
        {
            printf("fp error \n");
            return 0;
        }
        if(iStyle == 0)
        {
           //printf("hhas\n");
            fprintf(fp, "轨迹地图\n");
            ncUtlPrintCsv(fp, 4,
                          UT_TYPE_STRING, "设备MAC",
                          UT_TYPE_STRING, "采集时间",
                          UT_TYPE_STRING, "经度",
                          UT_TYPE_STRING, "纬度");
        }
        else
        {
           //printf("hhas\n");
            fprintf(fp, "轨迹地图\n");
            ncUtlPrintCsv(fp, 4,
                          UT_TYPE_STRING, "设备名称",
                          UT_TYPE_STRING, "采集时间",
                          UT_TYPE_STRING, "经度",
                          UT_TYPE_STRING, "纬度");
        }
    }
	
       iNum = 0;
	   int mynum = 0;
    for(i = min; i <= max; i++)
    {
        if(iStyle == 0)
        {


            memset(sql, 0, sizeof(sql));
            sprintf(sql + strlen(sql), "select servicecode , servicename, stime ,flags from  ncmactermatt_if_%04d%02d where flags!=2", i / 12, i % 12);
            if(strlen(iMac) != 0)
            {
               //printf("mac = %s\n", iMac);
                sprintf(sql + strlen(sql), " and mac like '%%%s%%' ", iMac);

            }
            if(lstime > 0)
            {
                sprintf(sql + strlen(sql), " and stime >= %ld ", lstime);
            };
            if(letime > 0)
            {
                sprintf(sql + strlen(sql), " and stime <= %ld ", letime);
            }
            sprintf(sql + strlen(sql), " and longitude!='' and latitude!=''  order by stime");
          

        }
        else
        {
            memset(sql, 0, sizeof(sql));
            sprintf(sql, "select  max(stime), longitude, latitude from  ncmacmobileap_if_%04d%02d  where 1=1 ", i / 12, i % 12);

            if(strlen(iMac) != 0)
            {
               //printf("servicename = %s\n", iMac);
                sprintf(sql + strlen(sql), "and servicename like '%s'", iMac);

            }
            if(lstime > 0)
            {
                sprintf(sql + strlen(sql), " and stime >= %ld ", lstime);
            };
            if(letime > 0)
            {
                sprintf(sql + strlen(sql), " and stime <= %ld ", letime);
            }
            sprintf(sql + strlen(sql), " and longitude!='' and latitude!='' ");
            sprintf(sql + strlen(sql), "group by longitude , latitude  order by stime  ");
        }

	   if(iStyle == 0){
       printf("sql = %s\n", sql);
        psCur = pasDbOpenSql(sql, 0);

        if(psCur == NULL)
        {
			pasDbCloseCursor(psCur);
            printf("cursor error\n");
        }
        else{
        iReturn = 0;
  
       // printf("iCount = %d\n", iCount);
        while((iReturn == 0 || iReturn == 1405))
        {
            castime = 0;
            memset(address, 0, sizeof(address));  
			memset(result, 0, sizeof(result));

            iReturn = pasDbFetchInto(psCur,
                                     UT_TYPE_STRING, 14, caServiceCode,
									 UT_TYPE_STRING, 30, caServiceName,
                                     UT_TYPE_LONG, 4, &castime ,
									  UT_TYPE_LONG, 4, &flag
                                    );

            if((iReturn != 0) && (iReturn != 1405)) break;

			if(flag==0)
			{
				if(iflag == 0 && castime >= etimetmp)
				{
					iNum++;
					stimetmp = castime;
					strcpy(	datas[iNum].servicecode, caServiceCode);
				    strcpy(	datas[iNum].servicename, caServiceName);
					datas[iNum].stime[0] = castime;
					
			iflag ++;
				}
			}
			else
			{
				if(iflag>0)
				{
					if(castime-stimetmp>3600){
						datas[iNum].stime[1] = castime;
				
					etimetmp = castime;
					}
					else{						
					
						etimetmp =etimetmp>( stimetmp+3600) ? etimetmp: (stimetmp + 3600);
						datas[iNum].stime[1] = etimetmp;
					}
					iflag =0;
				}
			}
        }
      }
		pasDbCloseCursor(psCur);
	   }
	   else
	   {
	    printf("sql = %s\n", sql);
        psCur = pasDbOpenSql(sql, 0);

        if(psCur == NULL)
        {
			pasDbCloseCursor(psCur);
            printf("cursor error\n");
        }
        else{
        iReturn = 0;
        while((iReturn == 0 || iReturn == 1405))
        {
            
            memset(caServiceName, 0, sizeof(caServiceName));  
			memset(longitude, 0, sizeof(longitude));
			memset(latitude, 0, sizeof(latitude));

            iReturn = pasDbFetchInto(psCur,
									 UT_TYPE_LONG, 4, &castime,
                                     UT_TYPE_STRING, 20,   longitude,
									  UT_TYPE_STRING, 20,   latitude
                                    );
			
            if((iReturn != 0) && (iReturn != 1405)) break;
			mynum++;
			 if(mynum > 1)
            {
                utPltPutLoopVar(psDbHead, "dh",mynum, ",");
            }
		    //printf("servicename= %ld  stime= %s   stime= %s   inum = %d \n", castime, longitude, latitude, mynum);
		   utPltPutLoopVarF(psDbHead, "servicename", mynum, "%ld", castime);
           utPltPutLoopVar(psDbHead, "stime",mynum, longitude);
		   utPltPutLoopVar(psDbHead, "etime",  mynum , latitude);
		   
        }
      }
		pasDbCloseCursor(psCur);
	   }
    }
    if(strlen(caExp) > 0)
    {

        fclose(fp);

        utPltFileDownload(iFd, "application/text", caPath, caFilename, caFilename);

        remove(caFile);

        utPltFreeDb(psDbHead);

        return 0;
    }
	if(iStyle == 0){
	int tmps = 1, iNums = 1;
	for(tmps = 1; tmps <=iNum; tmps++)
	{
		if(tmps == 1)
		{
			strcpy(datadeal[iNums].servicecode, datas[tmps].servicecode);
		    strcpy(datadeal[iNums].servicename, datas[tmps].servicename);
			datadeal[iNums].stime[0] = datas[tmps].stime[0];
			datadeal[iNums].stime[1] = datas[tmps].stime[1];
		}
		else
		{
			if(strcmp(datadeal[iNums].servicecode, datas[tmps].servicecode)==0)
			{
				if(datas[tmps].stime[0]-datadeal[iNums].stime[1]<=1800)
				{
					datadeal[iNums].stime[1] = datas[tmps].stime[1];
				}
				else
				{
					iNums++;
					strcpy(datadeal[iNums].servicecode, datas[tmps].servicecode);
					strcpy(datadeal[iNums].servicename, datas[tmps].servicename);
					datadeal[iNums].stime[0] = datas[tmps].stime[0];
					datadeal[iNums].stime[1] = datas[tmps].stime[1];
				}
			}
			else
			{
				iNums++;
					strcpy(datadeal[iNums].servicecode, datas[tmps].servicecode);
					strcpy(datadeal[iNums].servicename, datas[tmps].servicename);
					datadeal[iNums].stime[0] = datas[tmps].stime[0];
					datadeal[iNums].stime[1] = datas[tmps].stime[1];
			}
		}
	}
	for(tmps = 1; tmps <= iNums; tmps++){
	  if(tmps > 1)
            {
                utPltPutLoopVar(psDbHead, "dh",tmps, ",");
            }
		   printf("servicename= %s  stime= %ld   stime= %ld\n", datadeal[tmps].servicename, datadeal[tmps].stime[0], datadeal[tmps].stime[1]);
		utPltPutLoopVar(psDbHead, "servicename", tmps, datadeal[tmps].servicename);
         utPltPutLoopVarF(psDbHead, "stime",tmps, "%ld", datadeal[tmps].stime[0]);
		  utPltPutLoopVarF(psDbHead, "etime", tmps, "%ld", datadeal[tmps].stime[1]);
	}
  
    utPltPutVarF(psDbHead, "TotRec", "%d", iNums);
	}
	else
	{
		printf("mynum = %d\n", mynum);
		 utPltPutVarF(psDbHead, "TotRec", "%d", mynum);
	}

    utPltOutToHtml(iFd, psMsgHead, psDbHead, "mac/datamining/track/track.htm");
    return 0;
}

typedef struct countcrowd{
	  
    char  key[46];
  };
  
  typedef struct countcrowds{
    char  key[29];
    char   servicenamea[32];
    char   servicenameb[32];
    char    flag[10];
    long value;
     
  };
  
int dm_crowd(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead)
{ 
    utPltDbHead *psDbHead;
    int iReturn = 0 ;
    char iMac[1024], mac[32];
    char istyle[32];
    char iStartTime[32];
    char iEndTime[32];
    char longitude[32], latitude[32];
    char sql[8096];
    char caExp[10];
    char time_flag[11];
    char s_time[32];
    long lTime, lstime, letime, serv_count;
    long castime = 0;
    long iCount = 0, tcount = 0;
    char caTime_flag[10];
    int iNum = 0, iStyle;
    FILE *fp = NULL;
    char caSdate_d[20], caEdate_d[20], caEtime[16], caStemptime[32] , caEtemptime[32];
    char **ptmp = NULL;
    pasDbCursor *psCur;
    char tmpc[100];
    char caFilename[128], caFile[128], caPath[128];
    char caValue[32];
      char caTemp[1024];
       char caTemp_d[1024];
    long lBase;
    char caSdate[32], caMark[64];
    char caMacStr[20], caMacStr1[20], caServiceCode[16], caMacStr2[20];
    char caServiceName[32];
    char caGroupid[36], caUserName[36];
    char places[4096], caServiceNamea[32], caServiceNameb[32], caServiceCodea[32], caServiceCodeb[32];
    pasLHashInfo sHashInfo;
	pasLHashInfo sHashInfos;
    char result[52];
	char inputflag [5];
    psDbHead = utPltInitDb();
    utMsgPrintMsg(psMsgHead);
   ncLbsMacLocation *psMacLoc;
  iReturn = utMsgGetSomeNVar(psMsgHead, 9,
	                           "inputflag",  UT_TYPE_STRING, 4,  inputflag,
                               "mac", UT_TYPE_STRING, 1022, iMac,
                               "starttime", UT_TYPE_STRING, 30, iStartTime,
                               "endtime", UT_TYPE_STRING, 30, iEndTime,
                               "exp",  UT_TYPE_STRING, 30, caExp,
                               "time_flag", UT_TYPE_STRING, 10, caTime_flag,
                               "styles", UT_TYPE_STRING, 30, istyle,
                                "groupid",      UT_TYPE_STRING,16,caGroupid,
                                "username",       UT_TYPE_STRING,16,caUserName                                
                              );


   if(strlen(caExp) > 0)
    {

        sprintf(caFilename, "crowd%d.csv", utStrGetId());

        sprintf(caPath, "%s/html", "/home/ncmysql/ncs");
        sprintf(caFile, "%s/%s", caPath, caFilename);

        fp = fopen(caFile, "w");
        if(fp == NULL)
        {
            printf("fp error \n");
            return 0;
        }
        if(iStyle == 0)
        {
           // printf("hhas\n");
            fprintf(fp, "轨迹地图\n");
            ncUtlPrintCsv(fp, 4,
                          UT_TYPE_STRING, "设备MAC",
                          UT_TYPE_STRING, "采集时间",
                          UT_TYPE_STRING, "经度",
                          UT_TYPE_STRING, "纬度");
        }
        else
        {
          //  printf("hhas\n");
            fprintf(fp, "轨迹地图\n");
            ncUtlPrintCsv(fp, 4,
                          UT_TYPE_STRING, "设备名称",
                          UT_TYPE_STRING, "采集时间",
                          UT_TYPE_STRING, "经度",
                          UT_TYPE_STRING, "纬度");
        }
    }
    long ilCount = 0;
    int k = 0;
    lTime = time(0) - 10*60;
    memset(sql, 0, sizeof(sql));
    if(strlen(iMac)>0)
    	{
    		strcpy(caTemp_d,  pasCvtGBK(2, iMac, caTemp, 63));
            strcpy(iMac, caTemp_d);
    	}
    printf("%s\n",iMac);
		 unsigned char * pHash_suser;
        unsigned char * pHash_susers;
        struct countcrowd *psData;
        struct countcrowds *psDatas;
        pHash_suser=(unsigned char *)pasLHashInit(400000,400000,sizeof(struct countcrowd),0,46);
        pHash_susers=(unsigned char *)pasLHashInit(400000,400000,sizeof(struct countcrowds),0,29);
    	memset(sql, 0, sizeof(sql));
		memset(places,0,sizeof(places));
		if(strlen(inputflag)>0){
		if(atoi(inputflag)==0)
		{
			  sprintf(sql ,"SELECT distinct c.username  FROM ncsuser c WHERE  %s ",getGroupcodeSql("c.username",iMac)) ;
			  // strcpy( places , getServicecodeByGroupid(psShmHead,iMac));
		}
		else
		{
			  sprintf(sql ,"SELECT distinct c.username  FROM ncsuser c WHERE c.username in ('%s') ",iMac) ;
			
		}
		}
		else
		{
			 sprintf(sql ,"SELECT distinct c.username  FROM ncsuser c WHERE  c.username in ( %s )",getDsGroupcode()) ;
		}
		
  
#ifdef output
	printf("sql = %s\n", sql);
#endif
    psCur = pasDbOpenSql(sql, 0);       
        if(psCur == NULL)
        {
			pasDbCloseCursor(psCur);
            printf("cursor error\n");
        }
        else{
        iReturn = 0;
        iNum = 0;
      
        while((iReturn == 0 || iReturn == 1405))
        {
         
			memset(caServiceName,0,sizeof(caServiceName));
            iReturn = pasDbFetchInto(psCur,
                                     UT_TYPE_STRING, 30, caServiceName                               
                                   
                                    );

            if((iReturn != 0) && (iReturn != 1405)) break; 
			 memset(result,0,sizeof(result));
					 strcpy(result, caServiceName);
					 strcat(result, caServiceName);
					 if(iNum == 0)
					 sprintf(places+strlen(places), "'%s'", caServiceName);
					 else
						  sprintf(places+strlen(places), ",'%s'", caServiceName);
					   psDatas = (struct countcrowds *)pasLHashLookA(pHash_susers,&result);
                    if(psDatas){
                       }                    
           
            iNum++;
        }
		 pasDbCloseCursor(psCur);
      }
     
	  #ifdef output
     printf("places = %s\n", places);
      #endif
      memset(sql, 0, sizeof(sql));
	  long   etime = time(0);
	   int maxYear = atoi(utTimFormat("%Y", etime));    
    int maxMon = atoi(utTimFormat("%m", etime));
    int max = maxYear * 12 + maxMon;
#ifdef  TEST
	   sprintf(sql ," SELECT DISTINCT  a.servicecode, b.servicecode, a.mac FROM ncmactermatt_if_201508 a,ncmactermatt_if_201508 b WHERE a.mac = b.mac AND a.servicecode != b.`servicecode`  AND  a.stime<b.stime and  %s  and   %s ",getGroupcodeSql("a.servicecode",iMac), getGroupcodeSql("b.servicecode",iMac)) ;      
#else 
	if(strlen(inputflag)>0){
	if(atoi(inputflag)==0){
     sprintf(sql ,"  SELECT distinct  a.servicecode, b.servicecode,  a.mac FROM ncmactermatt_if_%04d%02d a,ncmactermatt_if_%04d%02d b WHERE a.mac = b.mac AND a.servicecode != b.servicecode  AND a.stime > UNIX_TIMESTAMP(SUBDATE(NOW(),INTERVAL 1 HOUR)) AND a.stime<b.stime  AND b.stime > UNIX_TIMESTAMP(SUBDATE(NOW(),INTERVAL 1 HOUR)) AND %s AND %s 	", max/12,max%12,max/12,max%12,   getGroupcodeSql("a.servicecode",iMac), getGroupcodeSql("b.servicecode",iMac)) ;
	}
	else
	{
	sprintf(sql ,"  SELECT distinct  a.servicecode, b.servicecode,  a.mac FROM ncmactermatt_if_%04d%02d a,ncmactermatt_if_%04d%02d b WHERE a.mac = b.mac AND a.servicecode != b.servicecode  AND a.stime > UNIX_TIMESTAMP(SUBDATE(NOW(),INTERVAL 1 HOUR)) AND a.stime<b.stime  AND b.stime > UNIX_TIMESTAMP(SUBDATE(NOW(),INTERVAL 1 HOUR)) AND a.servicecode in (%s)  AND  b.servicecode in (%s) 	", max/12,max%12,max/12,max%12,  places,places) ;
	}
	}
	else
	{
		   sprintf(sql ,"  SELECT distinct  a.servicecode, b.servicecode,  a.mac FROM ncmactermatt_if_%04d%02d a,ncmactermatt_if_%04d%02d b WHERE a.mac = b.mac AND a.servicecode != b.servicecode  AND a.stime > UNIX_TIMESTAMP(SUBDATE(NOW(),INTERVAL 1 HOUR)) AND a.stime<b.stime  AND b.stime > UNIX_TIMESTAMP(SUBDATE(NOW(),INTERVAL 1 HOUR)) AND a.servicecode in (%s ) AND b.servicecode in (%s) 	", max/12,max%12,max/12,max%12,   getDsGroupcode(), getDsGroupcode()) ;
	}
#endif
      #ifdef output
      printf("sql = %s\n",sql);
      #endif
      psCur = pasDbOpenSql(sql, 0);       
        if(psCur == NULL)
        {
			pasDbCloseCursor(psCur);
            printf("cursor error\n");
        }
        else{
        iReturn = 0;
        iNum = 0;
       
        while((iReturn == 0 || iReturn == 1405))
        {
           memset(result,0,sizeof(result));
            memset(caServiceCodea, 0 ,sizeof(caServiceCodea));
            memset(caServiceCodeb, 0 ,sizeof(caServiceCodeb));      
            memset(mac, 0 ,sizeof(mac));

            iReturn = pasDbFetchInto(psCur,                                   
                                     UT_TYPE_STRING, 30, caServiceCodea,
                                     UT_TYPE_STRING, 30, caServiceCodeb,
                                     UT_TYPE_STRING, 30, mac
                                    );
              
                   if((iReturn != 0) && (iReturn != 1405)) break;
                sprintf(result, "%s%s%s",caServiceCodea,caServiceCodeb,mac);                
                psData = (struct countcrowd *)pasLHashLookA(pHash_suser,&result);
                    if(psData){
                       }         
            iNum++;
        }
       pasDbCloseCursor(psCur);
         psData=(struct countcrowd *)pasLHashFirst(pHash_suser,&sHashInfo);
      while(psData){
      	    memset(result, 0, sizeof(result));
      	    strncpy(result, psData->key, 28);           
         psDatas = (struct countcrowds *)pasLHashLookA(pHash_susers,&result);
                    if(psDatas){
                    	if(strlen(psDatas->flag)==0 )
                    		{
                    			strcpy(psDatas->flag,"true");
                    			psDatas->value = 2;
                    		}
                    		else 
                    		{
                    			psDatas->value ++;
                    		}
                       }     
       
        psData=(struct countcrowd *)pasLHashNext(&sHashInfo);

     }
     iNum = 0;
        
		 uchar *pHash;
		 ncApSrvOnline *psAp;



		   pHash =  (unsigned char *)utShmHashHead(psShmHead,NC_LNK_MACLOCATION);
    if(pHash) {
        psMacLoc = (ncLbsMacLocation *)pasHashFirst(pHash,&sHashInfos);
        while(psMacLoc) {
            psAp = (ncApSrvOnline *)ncMacLogGetOnlineApByMac(psShmHead,psMacLoc->apmac);
			//printf("aaaaaservicecode = %s \n",  caServiceCode);
			memset(caServiceCode,0, sizeof(caServiceCode));
            if(psAp) {
		
                strcpy(caServiceCode,psAp->servicecode);
#ifdef output
			//	printf("sssssssssssservicecode = %s \n",  caServiceCode);
#endif
            }
            else {
                strcpy(caServiceCode,"\0");
            }
		
			char tmpcodes[15];
			int tmpi = 0;
			if(strlen(caServiceCode)>0){
				
			for(tmpi = 0; tmpi<=(strlen(places)+2)/17;tmpi++)
			{
				
				strncpy(tmpcodes, places+17*tmpi+1, 14);
				//if(tmpi == 0)printf("caServiceCode = %s, aaaaaaaaatmp = %s\n", caServiceCode, tmpcodes);
				//printf("caServiceCode = %s, aaaaaaaaatmp = %s\n", caServiceCode, tmpcodes);
				if(strcmp(tmpcodes, caServiceCode)==0)
				{
					#ifdef output
			//	printf("caServiceCode = %s, aaaaaaaaatmp = %s\n", caServiceCode, tmpcodes);
#endif
					 memset(result,0,sizeof(result));
					 strcpy(result, tmpcodes);
					 strcat(result,tmpcodes);
					   psDatas = (struct countcrowds *)pasLHashLookA(pHash_susers,&result);
                    if(psDatas){
                    	if(strlen(psDatas->flag)==0 )
                    		{
                    			strcpy(psDatas->flag,"true");
                    			psDatas->value = 1;
                    		}
                    		else 
                    		{
                    			psDatas->value ++;
                    		}
                       }    
				}
			}
			}
            psMacLoc = (ncLbsMacLocation *)pasHashNextS(&sHashInfos);
        }
    }
	

           psDatas=(struct countcrowds *)pasLHashFirst(pHash_susers,&sHashInfo);
      while(psDatas){
      	   memset(caServiceCodea, 0, sizeof(caServiceCodea));
      	   memset(caServiceCodeb, 0, sizeof(caServiceCodeb));
      	   memset(caServiceNamea, 0, sizeof(caServiceNamea));
      	   memset(caServiceNameb, 0, sizeof(caServiceNameb));
      	   strncpy(caServiceCodea, psDatas->key, 14);
      	   strncpy(caServiceCodeb, psDatas->key+14, 14);
      	   memset(sql,0,sizeof(sql));
      	   sprintf(sql ,"SELECT servicename FROM ncaplist  WHERE servicecode = '%s' ",caServiceCodea) ;
           iReturn = pasDbOneRecord(sql, 0, UT_TYPE_STRING, 30, &caServiceNamea);
#ifdef output
         //  printf("sql = %s\n", sql);
#endif
           memset(sql,0,sizeof(sql));
      	   sprintf(sql ,"SELECT servicename FROM ncaplist  WHERE servicecode = '%s' ",caServiceCodeb) ; 
      	   iReturn = pasDbOneRecord(sql, 0, UT_TYPE_STRING, 30, &caServiceNameb);
#ifdef  output
         //  printf("sql = %s\n", sql);
#endif
           if(strlen(caServiceNamea)>0&&strlen(caServiceNameb)>0){
           iNum ++;
            if(iNum > 1)
            {
                utPltPutLoopVar(psDbHead, "dh", iNum, ",");
            }
            printf("psDatas = %s,  %s,  %d\n", caServiceNamea, caServiceNameb, psDatas->value);
      	   utPltPutLoopVar(psDbHead, "addressa", iNum, caServiceNamea);
           utPltPutLoopVar(psDbHead, "addressb", iNum, caServiceNameb);
           utPltPutLoopVarF(psDbHead, "value", iNum,"%ld", psDatas->value);
		   }
           psDatas=(struct countcrowd *)pasLHashNext(&sHashInfo);

      }
     
   }

    
    if(strlen(caExp) > 0)
    {

        fclose(fp);

        utPltFileDownload(iFd, "application/text", caPath, caFilename, caFilename);

        remove(caFile);

        utPltFreeDb(psDbHead);

        return 0;
    }
   free(pHash_suser);
      free(pHash_susers);
    printf("iCount = %d\n", iNum);
    utPltPutVarF(psDbHead, "TotRec", "%d", iNum);
    //printf("hhas\n");
    utPltOutToHtml(iFd, psMsgHead, psDbHead, "mac/datamining/crowd/crowd.htm");
    return 0;
}
int dm_place(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead)
{
	utPltDbHead *psDbHead;
  psDbHead = utPltInitDb();
  utMsgPrintMsg(psMsgHead);
  char sql[1024];
  char caServiceName[32], longitude[32], latitude[32];
  sprintf(sql , "select servicename, longitude, latitude from ncaplist ");
   pasDbCursor *psCur;
   psCur = pasDbOpenSql(sql, 0);       
        if(psCur == NULL)
        {
			pasDbCloseCursor(psCur);
            printf("cursor error\n");
        }
        else{
        	
        int iReturn = 0;
        int iNum = 0;
      
        while((iReturn == 0 || iReturn == 1405))
        {         

            iReturn = pasDbFetchInto(psCur,
                                     UT_TYPE_STRING, 30, caServiceName,
                                     UT_TYPE_STRING, 30, longitude,
                                     UT_TYPE_STRING, 30, latitude                      
                                   
                                    );
                                    iNum++;
            if((iReturn != 0) && (iReturn != 1405)) break; 
             if(iNum > 1)
            {
                utPltPutLoopVar(psDbHead, "dh", iNum, ",");
            }
      	   utPltPutLoopVar(psDbHead, "servicename", iNum, caServiceName);
           utPltPutLoopVar(psDbHead, "longitude", iNum, longitude);
           utPltPutLoopVar(psDbHead, "latitude", iNum, latitude);
            
        }
		   pasDbCloseCursor(psCur);
        }
		
    utPltOutToHtml(iFd, psMsgHead, psDbHead, "mac/datamining/crowd/place.htm");
    return 0;
}
int dm_hotspot(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead)
{
	utPltDbHead *psDbHead;
  psDbHead = utPltInitDb();
  utMsgPrintMsg(psMsgHead);
  char sql[1024];
  char caServiceName[32], longitude[32], latitude[32];
    long   etime = time(0);
	   int maxYear = atoi(utTimFormat("%Y", etime));    
    int maxMon = atoi(utTimFormat("%m", etime));
    int max = maxYear * 12 + maxMon;
  sprintf(sql , "SELECT DISTINCT  servicename, longitude, latitude FROM ncmactermatt_if_%04d%02d WHERE longitude!='' AND latitude!=''  GROUP BY longitude, latitude  ", max/12, max%12);
   pasDbCursor *psCur;
   psCur = pasDbOpenSql(sql, 0);       
        if(psCur == NULL)
        {
			pasDbCloseCursor(psCur);
            printf("cursor error\n");
        }
        else{
        	
        int iReturn = 0;
        int iNum = 0;
      
        while((iReturn == 0 || iReturn == 1405))
        {         

            iReturn = pasDbFetchInto(psCur,
                                     UT_TYPE_STRING, 30, caServiceName,
                                     UT_TYPE_STRING, 30, longitude,
                                     UT_TYPE_STRING, 30, latitude                                 
                                   
                                    );
                                    iNum++;
            if((iReturn != 0) && (iReturn != 1405)) break; 
             if(iNum > 1)
            {
                utPltPutLoopVar(psDbHead, "dh", iNum, ",");
            }
      	   utPltPutLoopVar(psDbHead, "servicename", iNum, caServiceName);
           utPltPutLoopVar(psDbHead, "longitude", iNum, longitude);
           utPltPutLoopVar(psDbHead, "latitude", iNum, latitude);
            
        }
		 pasDbCloseCursor(psCur);
        }
		  
    utPltOutToHtml(iFd, psMsgHead, psDbHead, "mac/datamining/crowd/place.htm");
    return 0;
}
int dm_sus(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead)
{
	
	utPltDbHead *psDbHead;
  psDbHead = utPltInitDb();
  utMsgPrintMsg(psMsgHead);
  char sql[1024];
  char caStemptime[32];
  long letime, lstime;
   long  lTime = time(0);
   //printf("time = %d\n", lTime);

    if(atol(caTime_flag) == 0)
    {
        sprintf(caStemptime, "%s 00:00", utTimFormat("%Y/%m/%d", lTime));
        lstime = utTimStrToLong("%Y/%m/%d %H:%M", caStemptime);
        letime = lTime;
    }
    else if(atol(caTime_flag) == 1)
    {
        lstime = lTime - 3 * 3600 * 24;
        letime = lTime;
    }
    else if(atol(caTime_flag) == 2)
    {
        lstime = lTime - 7 * 3600 * 24;
        letime = lTime;
    }
    else if(atol(caTime_flag) == 3)
    {
        lstime = lTime - 30 * 3600 * 24;
        letime = lTime;
    }
	   else if(atol(caTime_flag) == 4)
    {
        lstime = lTime - 3*30 * 3600 * 24;
        letime = lTime;
    }
	   else if(atol(caTime_flag) == 5)
    {
        lstime = lTime - 6*30 * 3600 * 24;
        letime = lTime;
    }
    else
    {
        ncUtlMonthDate(lTime, caSdate_d, caEdate_d);
        sprintf(caStemptime, "%s 00:00", caSdate_d);
        lstime = utTimStrToLong("%Y/%m/%d %H:%M", caStemptime);
        sprintf(caEtemptime, "%s 23:59", caEdate_d);
        letime = utTimStrToLong("%Y/%m/%d %H:%M", caEtemptime);
    }

    int minYear = atoi(utTimFormat("%Y", lstime));
    int maxYear = atoi(utTimFormat("%Y", letime));
    int minMon = atoi(utTimFormat("%m", lstime));
    int maxMon = atoi(utTimFormat("%m", letime));
    int max = maxYear * 12 + maxMon;
    int min = minYear * 12 + minMon;
    int i;
	for(i = min;i<max; i++){
		memset(sql, 0, sizefo(sql));
  sprintf(sql , "SELECT a.ssid, a.servicecode, a.servicename  FROM ncmachotinfo_if_%04d%02d a , nchotspotinfo b  WHERE  a.ssid = b.ssid  ");
   pasDbCursor *psCur;
   psCur = pasDbOpenSql(sql, 0);       
        if(psCur == NULL)
        {
			pasDbCloseCursor(psCur);
            printf("cursor error\n");
        }
        else{
        	
        int iReturn = 0;
        int iNum = 0;
      
        while((iReturn == 0 || iReturn == 1405))
        {         

            iReturn = pasDbFetchInto(psCur,
                                     UT_TYPE_STRING, 30, caServiceName,
                                     UT_TYPE_STRING, 30, longitude,
                                     UT_TYPE_STRING, 30, latitude);
                                    iNum++;
            if((iReturn != 0) && (iReturn != 1405)) break; 
             if(iNum > 1)
            {
                utPltPutLoopVar(psDbHead, "dh", iNum, ",");
            }
      	   utPltPutLoopVar(psDbHead, "servicename", iNum, caServiceName);
           utPltPutLoopVar(psDbHead, "longitude", iNum, longitude);
           utPltPutLoopVar(psDbHead, "latitude", iNum, latitude);
            
        }
		 pasDbCloseCursor(psCur);
		}
	} 
    utPltOutToHtml(iFd, psMsgHead, psDbHead, "mac/datamining/crowd/place.htm");
    return 0;
}

int dataMiningFun(utShmHead * psShmHead)
{
    int iReturn;
   
    /*dm*/
    iReturn = pasSetTcpFunName("dm_crowd",  dm_crowd,   0);
	iReturn = pasSetTcpFunName("dm_hotspot",  dm_hotspot,   0);
    iReturn = pasSetTcpFunName("dm_place",  dm_place,   0);
    iReturn = pasSetTcpFunName("dm_track",  dm_track,   0);
    iReturn = pasSetTcpFunName("dm_bab",  dm_bab,   0);
    iReturn = pasSetTcpFunName("dm_sample",  dm_sample,   0);
    iReturn = pasSetTcpFunName("dm_list",  dm_list,   0);
	 
  
   
    return 0;

}
