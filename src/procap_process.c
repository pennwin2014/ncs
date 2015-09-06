
#define ncmax(a,b) ((a) > (b) ? (a) : (b))
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <fcntl.h>
#include <netdb.h>
#include <errno.h>
#include <string.h>
#include <pthread.h>
#include <sys/stat.h>
#include <time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/time.h>
#include "utoall.h"
#include "pasdb.h"
#include "ncsdef.h"
#include "pasutl.h"
#define PAS_SRCFILE  9989
int ncsStatMacWarn(utShmHead *psShmHead);
int ncsStatMacBab(utShmHead *psShmHead);
int ncsStatMacDevWarn(utShmHead *psShmHead);
static int ncsStartOnTimeDo(utShmHead *psShmHead)
{
    /* 设置定时进程  */
    int iReturn;
    pasUtlInitOntimeFun(psShmHead, NCS_LNK_ONTIME, 50);
    pasUtlSetOntimeFun(psShmHead, "CheckProc", pasProcessCheck, 60, 0);
    //    pasUtlSetOntimeFun(psShmHead,"ncsSrvCheckSubAdm", ncsSrvCheckSubAdm, 30,0);
    iReturn = pasConnect(psShmHead);
    if(iReturn != 0)
    {
        pasSysLog(PAS_LOG_ERROR, "Connect Database Error Sqlcode is %d", iReturn);
        exit(9);
    }
    pasUtlStartOnTimeFun(psShmHead);
    return 0;

}
/* 后台生成每日的MAC统计数据  */
int ncsStatTermMacPerDay_v4(utShmHead *psShmHead, long lStart, long lEnd)
{
    char caTemp[2048], caTemp1[1024];
    char caYM[12];
    strcpy(caYM, utTimFormat("%Y%m", lEnd));
    strcpy(caTemp, "insert into ncmactermcount(servicecode,apname,lcount,sdate) select servicecode,apname,count(*),from_unixtime(stime,'\%Y\%m\%d') aa ");

    sprintf(caTemp + strlen(caTemp), " from  ncmactermatt_if_%s where stime>=%d and stime<%d ", caYM, lStart, lEnd);

    strcat(caTemp, " group by servicecode,apname,aa ");
    printf("caTemp=%s\n", caTemp);
    pasDbExecSql(caTemp, 0);


}
int ncsStatMaccount(utShmHead *psShmHead)
{

    pasDbCursor *psCur;
    char caReportname[64], caGname_rp[64], caWorktime_rp[16];
    int iReturn;
    long lLasttime, lTime, lYtime;
    char caNoWH[16], caSdate[20];
    long lDid;
    long long lSid;
     char caYM[12];
     char caTemp[2048];
    iReturn = pasConnect(psShmHead);
    if(iReturn < 0)
    {

        sleep(60);
        return 0;
    }

    while(1)
    {

        lTime = time(0);
        strcpy(caNoWH, utTimFormat("%H", lTime));

        if(strcmp(caNoWH, "02") == 0)
        {
            lYtime = lTime - 3600 * 15;
            sprintf(caSdate, "%s 23:59:59", utTimFormat("%Y/%m/%d", lYtime));
            lYtime = utTimStrToLong("%Y/%m/%d %H:%M:%S", caSdate);
            lLasttime = ncsUtlGetLastInfo("TermmacStatTime");


            //统计每天MAC数量
            if(lYtime > lLasttime)
            {
                ncsStatTermMacPerDay_v4(psShmHead, lLasttime, lYtime);
                ncsUtlSetLastInfo("TermmacStatTime", lYtime);
            }

        }
//Mac入库
       //获取最大的ID
        strcpy(caYM, utTimFormat("%Y%m", time(0)));
        unsigned long long lLastId;
       ncsUtlGetLastInfo_v4("TermmacStatMac",&lLastId);
       sprintf(caTemp,"select max(sid) from ncmactermatt_if_%s where sid>=%llu ",caYM,lLastId );
       lSid=0;
       pasDbOneRecord(caTemp,0,UT_TYPE_LONG8,8,&lSid);
       sprintf(caTemp,"insert into nctermmacs(mac,servicecode,username,starttime,moditime) select mac,servicecode,vname,min(stime),max(stime)  from  ncmactermatt_if_%s where sid>=%llu and sid<=%llu group by mac,servicecode  ON DUPLICATE KEY UPDATE moditime=values(moditime) ",caYM,lLastId,lSid);
       printf("caTemp=%s\n",caTemp);
       iReturn=pasDbExecSql(caTemp,0);
       if(iReturn==0){
       	ncsUtlSetLastInfo_v4("TermmacStatMac", lSid);
       }

        ncsUtlGetLastInfo_v4("TermmacStatMacAp",&lLastId);
       sprintf(caTemp,"select max(sid) from ncmactermatt_if_%s where sid>=%llu ",caYM,lLastId );
       lSid=0;
       pasDbOneRecord(caTemp,0,UT_TYPE_LONG8,8,&lSid);
       sprintf(caTemp,"insert into nctermmacs_ap(mac,apname,starttime,moditime) select mac,apname,min(stime),max(stime)  from  ncmactermatt_if_%s where sid>=%llu and sid<=%llu group by apname,mac  ON DUPLICATE KEY UPDATE moditime=values(moditime) ",caYM,lLastId,lSid);
       printf("caTemp=%s\n",caTemp);
       iReturn=pasDbExecSql(caTemp,0);
       if(iReturn==0){
       	ncsUtlSetLastInfo_v4("TermmacStatMacAp", lSid);
       }    
       
        sleep(600);
    }
}







/* 启动各种进程  */
int ncsStartSomeProcess_mac(utShmHead *psShmHead)
{
    int iReturn, iLicense;
    pasUtlInitProcess(psShmHead, NCS_LNK_PROCESS, 60);
    pasUtlSetProcessName(psShmHead, "OnTime", ncsStartOnTimeDo, 0, 0);
    /* 注册执行的进程  */
    //启动生成MAC统计信息进程
    pasUtlSetProcessName(psShmHead, "MacCount", ncsStatMaccount, 68400L, 0);
    //启动告警日志进程
    pasUtlSetProcessName(psShmHead, "MacWarn", ncsStatMacWarn, 68400L, 0);
    //启动伴随行为分析进程
    pasUtlSetProcessName(psShmHead, "MacBab", ncsStatMacBab, 68400L, 0);
    //启动运维告警进程
    pasUtlSetProcessName(psShmHead, "MacDevWarn", ncsStatMacDevWarn, 68400L, 0);
    iReturn = pasUtlStartProcess(psShmHead);
    return iReturn;
}