/*  Last Modify  2004/06/05  */
#define PAS_SRCFILE    3101
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/time.h>
#include "utoall.h"
#include "ncsdef.h"
#include "pasutl.h"
#include "pasdb.h"
#include "pasmessage.h"
#include "ncapi.h"
#include "ncmac.h"


#include <time.h>
#include <malloc.h>
#include <pthread.h>
#include "string.h"
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>


#include "pas_acbm.h"
#include "bab_list.h"

static  pattern_data  *psAcbmPat_bab = NULL;      // 伴随行为模式内容
static  pattern_tree  *psAcbmTree_bab = NULL;     // 伴随行为Acbm模式树



//#include "ncfileserver.h"
#define MAX_TIMEOUT_SECONDS (15*60)
//#define SHOWBAB_DEBUG 1
#define FRONTTYPE_TODAYMACS "0001"
#define FRONTTYPE_TOTALMACS "0002"
#define FRONTTYPE_TOPPLACE "0003"


struct servicedata_s
{
    unsigned long userid;
    char caServicecode[16];
    int  onuid_count;   //在线用户数
    int  endnum;      //终端用户数
    char caCname[64];
    long onstate;          //1 在线    2-离线
    char caIp[16];
    char caDate[16];
    char caEmail[64];
    char caAddress[128];
    char caHandphone[32];
    char caTel[64];
    char caContact[32];
    char caVersion[16];
    long lMonflag;
    long lUseflags;
    char caGroupname[32];
    char caDemo[256];
    char caGtype[4];
    unsigned long groupid;
    long lLasttime;
    char caAddtime[20];
    char caFcode[16];
    char servicestate[4];  //客户端状态 5-维护 2-勒令停业 3-暂停营业 4-拆机 5-维护 9-待审核    无线系统这个状态暂时拆分到别的地方
    char caJointype[12];   //01 专网真实IP地址，02-专线 03-ADSL拨号 04-ISDN 05-普通拨号 06-Cable Moderm 拨号 07-电力线 -08-无线上网 09 无线+有线 99-其它
    long lShflag;
    long lWorkflag;
    long lYyflag;
    long lDataflag;
};

static int macPrint(long isDebug, const char* fmt, ...)
{
    int retCnt = 0;
    if(isDebug)
    {
        va_list ap;
        va_start(ap, fmt);
        retCnt = vprintf(fmt, ap);
        va_end(ap);
    }
    return retCnt;
}

int  timeToStringEx(time_t now, char *year, char *mon)
{
    if((year == NULL) || (mon == NULL) || (now == 0))
    {
        return -1;
    }
    struct tm*  time1 = (struct tm *)localtime(&now);
    //年
    snprintf(year, 5, "%d", time1->tm_year + 1900);
    //月
    if((time1->tm_mon + 1 > 0) && (time1->tm_mon + 1 <= 9))
        snprintf(mon, 3, "0%d", time1->tm_mon + 1);
    else
        snprintf(mon, 3, "%d", time1->tm_mon + 1);
    return 0;
}

int doCalcByDays(int days, utMsgHead *psMsgHead, int iFd, utPltDbHead *psDbHead, char* caGroupSql)
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
        if(strlen(caGroupSql) > 0)
        {
            snprintf(sql, sizeof(sql), "select ifnull(sum(lcount),0) from ncmactermcount where sdate=\'%s\' and %s", utTimFormat("%Y%m%d", lttime), caGroupSql);
            // printf("sql=[%s]\n", sql);
        }
        else
        {
            snprintf(sql, sizeof(sql), "select ifnull(sum(lcount),0) from ncmactermcount where sdate=\'%s\' ", utTimFormat("%Y%m%d", lttime));
            // printf("sql=[%s]\n", sql);
        }

        iReturn = pasDbOneRecord(sql, 0, UT_TYPE_LONG, 4, &collect_number);
        if(iReturn != 0)
        {
            utPltPutVar(psDbHead, "result", "0");
            utPltOutToHtml(iFd, psMsgHead, psDbHead, "mac/frontPage/macTermCollectNumber.htm");
            return -1;
        }
        //算出该天的0点时间值
        memset(caStemptime, 0, sizeof(caStemptime));
        sprintf(caStemptime, "%s 00:00", utTimFormat("%Y/%m/%d", lttime));
        //printf("caStemptime=%s,leTime=%lu\n", caStemptime, letime);
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

long getMonthFirstDay(long lttime)
{
    char caStemptime[56] = "";
    snprintf(caStemptime, sizeof(caStemptime), "%s/%02d 00:00", utTimFormat("%Y/%m", lttime), 1);
    //printf("caStemptime=[%s]\n", caStemptime);
    return utTimStrToLong("%Y/%m/%d %H:%M", caStemptime);
}

long getCountInDuration(long lstime, long letime, char* caGroupSql)
{
    char sql[1024] = "";
    int iReturn = 0;
    long collect_number = 0;
    //printf("lstime=%lu, letime=%lu\n", lstime, letime);
    memset(sql, 0, sizeof(sql));
    if(strlen(caGroupSql) > 0)
    {
        snprintf(sql, sizeof(sql), "select ifnull(sum(lcount),0) from ncmactermcount where sdate>=\'%s01\' and sdate<=\'%s31\' and %s", utTimFormat("%Y%m", lstime), utTimFormat("%Y%m", letime), caGroupSql);
    }
    else
    {
        snprintf(sql, sizeof(sql), "select ifnull(sum(lcount),0) from ncmactermcount where sdate>=\'%s01\' and sdate<=\'%s31\' ", utTimFormat("%Y%m", lstime), utTimFormat("%Y%m", letime));
    }
    //printf("temp sql=[%s]\n", sql);
    iReturn = pasDbOneRecord(sql, 0, UT_TYPE_LONG, 4, &collect_number);
    //printf("iret = %d, n=%lu\n", iReturn, collect_number);
    if((iReturn != 0) && (iReturn != 1405))
    {
        return -1;
    }
    return collect_number;
}

typedef struct
{
    long rcount;
    long rtime;
} retData;

int doCalcByMonths(int months, utMsgHead *psMsgHead, int iFd, utPltDbHead *psDbHead, char* caGroupSql)
{
    retData* retList = (retData*)malloc(sizeof(retData) * months);
    long letime = time(0);
    long lstime = getMonthFirstDay(letime);
    int i = 0;
    long tmpCount = 0;
    for(i = 0; i < months; i++)
    {
        retList[months - i - 1].rtime = lstime;
        //统计出该时间段内所有的数据
        tmpCount = getCountInDuration(lstime, letime, caGroupSql);
        if(tmpCount < 0)
        {
            utPltPutVar(psDbHead, "result", "0");
            utPltOutToHtml(iFd, psMsgHead, psDbHead, "mac/frontPage/macTermCollectNumber.htm");
            return -2;
        }
        retList[months - i - 1].rcount = tmpCount;
        letime = lstime - 3600;
        lstime = getMonthFirstDay(letime);
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


int getDispnameByApname(utShmHead *psShmHead, char* apname, char dispname[128])
{
    long frontPageDebug = utComGetVar_ld(psShmHead, "FrontDebug", 0);
    ncApSrvOnline *psAp;
    psAp = (ncApSrvOnline *)utShmHashLook(psShmHead, NC_LNK_APSRVONLINE, apname);
    if(psAp)
    {
        strcpy(dispname, psAp->dispname);
        if(strlen(dispname) == 0)
        {
            strcpy(dispname, "nullname\0");
        }
    }
    else
    {
        strcpy(dispname, "noname\0");
    }

    macPrint(frontPageDebug, "apname = %s, disp=%s\n", apname, dispname);
    /*
        char sql[1024] = "";
        int iReturn = 0;
        memset(sql, 0, sizeof(sql));
        snprintf(sql, sizeof(sql), "select dispname from ncaplist where apname=\'%s\'", apname);
        printf("temp sql=[%s]\n", sql);
        iReturn = pasDbOneRecord(sql, 0, UT_TYPE_STRING, sizeof(dispname)-1, dispname);
        printf("iret = %d, n=%s\n", iReturn, dispname);
        if((iReturn != 0) && (iReturn != 1405))
        {
            return -1;
        }
        */
    return 0;
}


int getNameByFcode(utShmHead *psShmHead, char* fcode, char dispname[64])
{
    char sql[1024] = "";
    int iReturn = 0;
    long frontPageDebug = utComGetVar_ld(psShmHead, "FrontDebug", 0);
    memset(sql, 0, sizeof(sql));
    snprintf(sql, sizeof(sql), "select name from ncsfactorycod where code=\'%s\'", fcode);
    iReturn = pasDbOneRecord(sql, 0, UT_TYPE_STRING, 63, dispname);
    if(strlen(dispname) == 0)
    {
        strcpy(dispname, "noname\0");
    }

    macPrint(frontPageDebug, "getNameByFcode iret = %d, n=[%s]\n", iReturn, dispname);

    if((iReturn != 0) && (iReturn != 1405))
    {
        return -1;
    }
    return 0;
}

int getDispnameByServicecode(utShmHead *psShmHead, char* servicecode, char dispname[64])
{
    char sql[1024] = "";
    int iReturn = 0;
    long frontPageDebug = utComGetVar_ld(psShmHead, "FrontDebug", 0);
    memset(sql, 0, sizeof(sql));
    snprintf(sql, sizeof(sql), "select dispname from ncsuser where username=\'%s\'", servicecode);
    iReturn = pasDbOneRecord(sql, 0, UT_TYPE_STRING, 63, dispname);
    if(strlen(dispname) == 0)
    {
        strcpy(dispname, "noname\0");
    }

    macPrint(frontPageDebug, "get dispnamebyserviccode sql=[%s] iret = %d, disp=[%s]\n", sql, iReturn, dispname);

    if((iReturn != 0) && (iReturn != 1405))
    {
        return -1;
    }
    return 0;
}


typedef struct
{
    char fcode[36];
    long number;
} factoryData;


static char *getDsGroupids()
{
    static char caReturn[1024];
    char caIds[1024], caTempid[1024];
    char caOpt[256];
    int iReturn, lId, iNum;
    pasDbCursor *psCur;
    memset(caReturn, 0, sizeof(caReturn));
    lId = 0;

    iReturn = dsCltGetMyInfo(1, "Userid", &lId);
    //printf("================frontpage dsiReturn=%d,lId=%d===============\n", iReturn, lId);
    if(iReturn != 0 || lId <= 0)
    {
        return &caReturn[0];
    }

    memset(caOpt, 0, sizeof(caOpt));
    pasDbOneRecord("select opt from dsuser where id=:id", 1, "id", UT_TYPE_LONG, lId, UT_TYPE_STRING, 255, caOpt);
    if(strstr(caOpt, "all"))
    {
        return &caReturn[0];
    }

    sprintf(caReturn, "%s", caOpt);
    sprintf(caIds, "%s", caOpt);

    while(strlen(caIds) > 0)
    {
        memset(caTempid, 0, sizeof(caTempid));
        iNum = 0;
        //printf("select groupid from ncsgroup where pid in(%s)\n ", caIds);
        psCur = pasDbOpenSqlF("select groupid from ncsgroup where pid in(%s) ", caIds);
        if(psCur)
        {
            iReturn = 0;
            iReturn = pasDbFetchInto(psCur, UT_TYPE_LONG, 4, &lId);
            while(iReturn == 0)
            {
                if(iNum == 0)
                {
                    sprintf(caTempid, "%d", lId);
                }
                else
                {
                    sprintf(caTempid + strlen(caTempid), ",%d", lId);
                }

                iReturn = pasDbFetchInto(psCur, UT_TYPE_LONG, 4, &lId);

                iNum++;

            }
        }
        pasDbCloseCursor(psCur);

        if(strlen(caTempid) > 0)
        {
            sprintf(caReturn + strlen(caReturn), ",%s", caTempid);
        }
        strcpy(caIds, caTempid);
    }



    return &caReturn[0];
}

static char *getServicecodesByDids(utShmHead *psShmHead, char *caGroupids)
{
    static char caReturn[8024];
    char caAllGroupIds[8024];
    char sql[8024];
    char caServicecode[32];
    int iReturn, lId, iNum;
    char caGroupId[12] = {0};
    pasDbCursor *psCur;
    memset(caReturn, 0, sizeof(caReturn));
    memset(caAllGroupIds, 0, sizeof(caAllGroupIds));
    strcpy(caAllGroupIds, caGroupids);
    if(strlen(caAllGroupIds) == 0)
        strcpy(caAllGroupIds, "0");
    int isBreak = 0;

    for(iNum = 0; iNum < 3; iNum++)
    {
        memset(sql, 0, sizeof(sql));
        sprintf(sql, "select groupid from ncsgroup where groupid in (%s) or pid in (%s)", caAllGroupIds, caAllGroupIds);
        //printf("iNum=%d,sql=[%s],groupids=[%s]\n", iNum, sql, caAllGroupIds);
        /*
            //放到sql以后就可以清空
            memset(caAllGroupIds, 0, sizeof(caAllGroupIds));
            */
        psCur = pasDbOpenSqlF(sql);
        if(psCur)
        {
            iReturn = 0;
            memset(caGroupId, 0, sizeof(caGroupId));
            iReturn = pasDbFetchInto(psCur, UT_TYPE_STRING, 11, caGroupId);
            while(iReturn == 0)
            {
                //不断壮大的groupid列表
                if(strlen(caAllGroupIds) == 0)
                {
                    sprintf(caAllGroupIds, "%s", caGroupId);
                }
                else
                {
                    if(!strstr(caAllGroupIds, caGroupId))
                        sprintf(caAllGroupIds + strlen(caAllGroupIds), ",%s", caGroupId);
                }
                memset(caGroupId, 0, sizeof(caGroupId));
                iReturn = pasDbFetchInto(psCur, UT_TYPE_STRING, 11, caGroupId);
            }
        }
        pasDbCloseCursor(psCur);
    }

    iNum = 0;
    memset(sql, 0, sizeof(sql));
    sprintf(sql, "select username from ncsuser a,ncsgroup b where a.groupid=b.groupid and (b.groupid in (%s) or b.pid in(%s)) limit 0,300 ", caAllGroupIds, caAllGroupIds);
#ifdef SHOWBAB_DEBUG
    printf("get servicecode list sql=[%s]\n", sql);
#endif
    psCur = pasDbOpenSqlF(sql);
    if(psCur)
    {
        iReturn = 0;
        memset(caServicecode, 0, sizeof(caServicecode));
        iReturn = pasDbFetchInto(psCur, UT_TYPE_STRING, 16, caServicecode);
        while(iReturn == 0)
        {
            //最终结果
            if(iNum == 0)
            {
                sprintf(caReturn, "\'%s\'", caServicecode);
            }
            else
            {
                sprintf(caReturn + strlen(caReturn), ",\'%s\'", caServicecode);
            }
            memset(caServicecode, 0, sizeof(caServicecode));
            iReturn = pasDbFetchInto(psCur, UT_TYPE_STRING, 16, caServicecode);
            iNum++;
        }
    }
    pasDbCloseCursor(psCur);

    if((strlen(caGroupids) > 0) && (strlen(caReturn) == 0))
        strcpy(caReturn, "no data");

    return &caReturn[0];
}


int macFrontPageRange(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead)
{
    long frontPageDebug = utComGetVar_ld(psShmHead, "FrontDebug", 0);

    if(frontPageDebug)
    {
        utMsgPrintMsg(psMsgHead);
    }
    utPltDbHead *psDbHead;
    char groupid[32] = "";
    char range_type[32] = "";
    char sql[8024] = "";
    //char caServicecodes[8024] = "";
    int iReturn = 0;
    pasDbCursor *psCur;
    char caGroupSql[8024] = "";
    strcpy(caGroupSql, (char*)getDsGroupcodeSql("servicecode"));
    iReturn = utMsgGetSomeNVar(psMsgHead, 2,
                               "groupid",  UT_TYPE_STRING, sizeof(groupid) - 1, groupid,
                               "range_type", UT_TYPE_STRING, sizeof(range_type) - 1, range_type);

    macPrint(frontPageDebug, "[groupid=%s,range_type=%s]\n", groupid, range_type);

    //strcpy(caServicecodes, getServicecodesByDids(psShmHead, getDsGroupids()));
    //printf("caServicecodes=[%s]\n", caServicecodes);
    //查询表里符合条件所有的点
    long collect_number = 0;
    long sumCount = 0;
    uchar caServiceName[128] = "";
    ncDeptinfo      *psDept;
    //场所采集排行
    if(strcmp(range_type, "service") == 0)
    {
        //memset(sql, 0, sizeof(sql));
        psDbHead = utPltInitDb();
        /*
        char table_name[1024] = "";
        char  year[10];
        char  mon[10];
        memset(year, 0, sizeof(year));
        memset(mon, 0 , sizeof(mon));
        time_t now;
        time(&now);
        timeToStringEx(now, year, mon);
        int syear = atoi(year);
        int smonth = atoi(mon);
        snprintf(table_name + strlen(table_name), 1024 - strlen(table_name), "ncmactermatt_if_%4u%02u", syear, smonth);

        if(strlen(caGroupSql) > 0)
        {
            snprintf(sql, sizeof(sql), "select  ifnull(count(*),0) as nn, servicecode from %s  where servicecode !=\'\' and %s group by servicecode order by nn desc limit 5", table_name, caGroupSql);
        }
        else
        {
            snprintf(sql, sizeof(sql), "select  ifnull(count(*),0) as nn, servicecode from %s  where servicecode !=\'\' group by servicecode order by nn desc limit 5", table_name);
        }
        */
        /*       if(strlen(caGroupSql) > 0)
               {
                   snprintf(sql, sizeof(sql), "select  ifnull(sum(lcount),0) as nn, servicecode from ncmactermcount where %s group by servicecode order by nn desc limit 5", caGroupSql);
               }
               else
               {
                   snprintf(sql, sizeof(sql), "select  ifnull(sum(lcount),0) as nn, servicecode from ncmactermcount group by servicecode order by nn desc limit 5");
               }
               //printf("place sql=[%s]\n", sql);
               psCur = pasDbOpenSql(sql, 0);
               /*
               //统计所有的
               psCur = pasDbOpenSql("select  ifnull(sum(lcount),0) as nn, servicecode from ncmactermcount group by servicecode order by nn desc limit 5", 0);
               */
        /*       s_serviceData sdataList[5];
               memset(sdataList, 0, sizeof(s_serviceData) * 5);
               iReturn = pasDbFetchInto(psCur,
                                        UT_TYPE_LONG, 4, &sdataList[0].number,
                                        UT_TYPE_STRING, 31, sdataList[0].servicecode);
               int iNum = 0;

               while((iReturn == 0) || (iReturn == 1405))
               {
                   sumCount += sdataList[iNum].number;
                   iNum++;
                   iReturn = pasDbFetchInto(psCur,
                                            UT_TYPE_LONG, 4, &sdataList[iNum].number,
                                            UT_TYPE_STRING, 31, sdataList[iNum].servicecode);
               }
               int lastINum = iNum;
               //printf("lastINum=%d\n", lastINum);
               pasDbCloseCursor(psCur);
        */


        uint8 lUserId = 0;


        int iReturn = dsCltGetMyInfo(1, "Userid", &lUserId);



        if(iReturn == 0 && lUserId > 0)


        {


            ncsMacFrontpageObj* psData = (ncsMacFrontpageObj *)utShmHashLookA(psShmHead, NCS_LNK_FRONTPAGE, (char*)(&lUserId));


            if(psData)


            {


                char dispname[64] = "";
                int iNum = 0;


                for(iNum = 1; iNum <= 5; iNum++)


                {
                    if(psData->sdataList[iNum - 1].flag)
                    {
                        sumCount += psData->sdataList[iNum - 1].number;

                        utPltPutLoopVarF(psDbHead, "collect_number", iNum, "%lu", psData->sdataList[iNum - 1].number);


                        memset(dispname, 0, sizeof(dispname));


                        getDispnameByServicecode(psShmHead, psData->sdataList[iNum - 1].servicecode, dispname);




                        utPltPutLoopVar(psDbHead, "collect_name", iNum, dispname);




                        //getServiceNameByCode(psShmHead, servicecode));


                        // printf("iNum=%d,servicecode=%s\n", iNum, servicecode);


                        if(iNum > 1)


                        {


                            utPltPutLoopVar(psDbHead, "dh", iNum, ",");


                        }


                    }


                }


            }
        }
        //printf("service sumcount=%lu\n", sumCount);
    }
    else if(strcmp(range_type, "factory") == 0)
    {
        psDbHead = utPltInitDb();
        memset(sql, 0, sizeof(sql));

        /*
        if(strlen(caServicecodes) > 0)
        {
            sprintf(sql, "select ifnull(count(mac), 0) as nn, fcode from ncaplist where fcode!=\'\' and servicecode in (%s) group by fcode order by nn desc limit 5", caServicecodes);
        }
        */
        if(strlen(caGroupSql) > 0)
        {
            sprintf(sql, "select ifnull(count(mac), 0) as nn, fcode from ncaplist where fcode!=\'\' and %s group by fcode order by nn desc limit 5", caGroupSql);
        }
        else
        {
            sprintf(sql, "select ifnull(count(mac), 0) as nn, fcode from ncaplist where fcode!=\'\' group by fcode order by nn desc limit 5");
        }
        psCur = pasDbOpenSql(sql, 0);
        factoryData fdataList[5];
        memset(fdataList, 0, sizeof(factoryData) * 5);
        iReturn = pasDbFetchInto(psCur,
                                 UT_TYPE_LONG, 4, &fdataList[0].number,
                                 UT_TYPE_STRING, 31, fdataList[0].fcode);
        int iNum = 0;
        while((iReturn == 0) || (iReturn == 1405))
        {
            sumCount += fdataList[iNum].number;
            iNum ++;
            iReturn = pasDbFetchInto(psCur,
                                     UT_TYPE_LONG, 4, &fdataList[iNum].number,
                                     UT_TYPE_STRING, 31, fdataList[iNum].fcode);
        }
        int lastINum = iNum;
        //printf("lastINum=%d\n", lastINum);
        pasDbCloseCursor(psCur);
        char fname[64] = "";
        for(iNum = 1; iNum <= lastINum; iNum++)
        {
            utPltPutLoopVarF(psDbHead, "collect_number", iNum, "%lu", fdataList[iNum - 1].number);
            memset(fname, 0, sizeof(fname));
            getNameByFcode(psShmHead, fdataList[iNum - 1].fcode, fname);
            utPltPutLoopVar(psDbHead, "collect_name", iNum, fname);
            if(iNum > 1)
            {
                utPltPutLoopVar(psDbHead, "dh", iNum, ",");
            }
        }
        //printf("factory sumcount=%lu\n", sumCount);
    }
    else if(strcmp(range_type, "device") == 0)
    {
        /*char apmac[36] = "";
        memset(sql, 0, sizeof(sql));
        psDbHead = utPltInitDb();
        memset(sql, 0, sizeof(sql));
        /*
        if(strlen(caServicecodes) > 0)
        {
            sprintf(sql, "select  ifnull(sum(lcount), 0) as nn, right(apname,12) from ncmactermcount where apname!=\'\' and servicecode in (%s) group by right(apname,12) order by nn desc limit 5", caServicecodes);
        }
        */
        /*
        if(strlen(caGroupSql) > 0)
        {
            sprintf(sql, "select  ifnull(sum(lcount), 0) as nn, right(apname,12) from ncmactermcount where apname!=\'\' and %s group by right(apname,12) order by nn desc limit 5", caGroupSql);
        }
        else
        {
            sprintf(sql, "select  ifnull(sum(lcount), 0) as nn, right(apname,12) from ncmactermcount where apname!=\'\'  group by right(apname,12) order by nn desc limit 5");
        }
        psCur = pasDbOpenSql(sql, 0);
        iReturn = pasDbFetchInto(psCur,
                                 UT_TYPE_LONG, 4, &collect_number,
                                 UT_TYPE_STRING, 31, apmac);
        int iNum = 0;
        char dispname[128];
        while((iReturn == 0) || (iReturn == 1405))
        {
            sumCount += collect_number;
            iNum ++;
            utPltPutLoopVarF(psDbHead, "collect_number", iNum, "%lu", collect_number);
            // 查询得到apdispname
            memset(dispname, 0, sizeof(dispname));
            getDispnameByApname(psShmHead, apmac, dispname);
            utPltPutLoopVar(psDbHead, "collect_name", iNum, dispname);
            if(iNum > 1)
            {
                utPltPutLoopVar(psDbHead, "dh", iNum, ",");
            }
            memset(apmac, 0, sizeof(apmac));
            iReturn = pasDbFetchInto(psCur,
                                     UT_TYPE_LONG, 4, &collect_number,
                                     UT_TYPE_STRING, 31, apmac);
        }
        pasDbCloseCursor(psCur);*/

        psDbHead = utPltInitDb();
        uint8 lUserId = 0;

        int iReturn = dsCltGetMyInfo(1, "Userid", &lUserId);

        if(iReturn == 0 && lUserId > 0)
        {


            ncsMacFrontpageObj* psData = (ncsMacFrontpageObj *)utShmHashLookA(psShmHead, NCS_LNK_FRONTPAGE, (char*)(&lUserId));

            if(psData)
            {


                char dispname[64] = "";
                int iNum = 0;


                for(iNum = 1; iNum <= 5; iNum++)
                {
                    if(psData->deviceList[iNum - 1].flag)
                    {

                        sumCount += psData->deviceList[iNum - 1].number;

                        utPltPutLoopVarF(psDbHead, "collect_number", iNum, "%lu", psData->deviceList[iNum - 1].number);


                        memset(dispname, 0, sizeof(dispname));


                        getDispnameByServicecode(psShmHead, psData->deviceList[iNum - 1].servicecode, dispname);

                        utPltPutLoopVar(psDbHead, "collect_name", iNum, dispname);

                        if(iNum > 1)
                        {
                            utPltPutLoopVar(psDbHead, "dh", iNum, ",");
                        }


                    }


                }


            }
        }

    }
    else if(strcmp(range_type, "warn") == 0)
    {
        long lGroupid = 0;

        memset(sql, 0, sizeof(sql));
        /*
        if(strlen(caServicecodes) > 0)
        {
            sprintf(sql, "select count(groupid) as nn,groupid from ncsuser where username in (select description from nctermsysalarm where alarmcode='10007' and status=0 and description in (%s)) and groupid!=0 group by groupid  order by nn desc limit 5", caServicecodes);
        }
        */
        if(strlen(caGroupSql) > 0)
        {
            sprintf(sql, "select count(groupid) as nn,groupid from ncsuser where username in (select description from nctermsysalarm where alarmcode='10007' and status=0 and %s) and groupid!=0 group by groupid  order by nn desc limit 5", getDsGroupcodeSql("description"));
        }
        else
        {
            sprintf(sql, "select count(groupid) as nn,groupid from ncsuser where username in (select description from nctermsysalarm where alarmcode='10007' and status=0) and groupid!=0 group by groupid  order by nn desc limit 5");
        }
        psDbHead = utPltInitDb();

        psCur = pasDbOpenSql(sql, 0);
        iReturn = pasDbFetchInto(psCur,
                                 UT_TYPE_LONG, 4, &collect_number,
                                 UT_TYPE_LONG, 4, &lGroupid);
        int iNum = 0;
        while((iReturn == 0) || (iReturn == 1405))
        {
            sumCount += collect_number;
            iNum ++;
            utPltPutLoopVarF(psDbHead, "collect_number", iNum, "%lu", collect_number);
            utPltPutLoopVar(psDbHead, "collect_name", iNum, (char*)ncsUtlGetGroupNameById(psShmHead, lGroupid, "noname"));
            if(iNum > 1)
            {
                utPltPutLoopVar(psDbHead, "dh", iNum, ",");
            }

            iReturn = pasDbFetchInto(psCur,
                                     UT_TYPE_LONG, 4, &collect_number,
                                     UT_TYPE_LONG, 4, &lGroupid);
        }
        pasDbCloseCursor(psCur);

    }
    else
    {
        //类型不对
        utPltPutVar(psDbHead, "result", "2");//2代表类型不对
        utPltOutToHtml(iFd, psMsgHead, psDbHead, "mac/frontPage/macCollectRange.htm");
        return 1;
    }
    //返回数据给前端
    utPltPutVar(psDbHead, "result", "1");
    utPltPutVarF(psDbHead, "sumCount", "%lu", sumCount);
    utPltOutToHtml(iFd, psMsgHead, psDbHead, "mac/frontPage/macCollectRange.htm");
    return 0;
}

//终端采集数量
int macFrontPageChart1(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead)
{
    long frontPageDebug = utComGetVar_ld(psShmHead, "FrontDebug", 0);
    if(frontPageDebug)
    {
        utMsgPrintMsg(psMsgHead);
    }

    utPltDbHead *psDbHead = utPltInitDb();
    //共享内存相关变量
    unsigned char *pHash;
    char caUsername[32] = "";
    char request_type[32] = "";
    //时间相关变量
    long lstime, lttime, letime;
    char caStemptime[24];
    char sDate[10];
    char sql[8024] = "";
    int iReturn = 0;
    // char caServicecodes[8024] = "";
    char caGroupSql[8024] = "";
    strcpy(caGroupSql, (char*)getDsGroupcodeSql("servicecode"));
    // strcpy(caServicecodes, getServicecodesByDids(psShmHead, getDsGroupids()));
    //printf("caServicecodes=[%s]\n", caServicecodes);

    iReturn = utMsgGetSomeNVar(psMsgHead, 2,
                               "username",  UT_TYPE_STRING, sizeof(caUsername) - 1, caUsername,
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
        timeToStringEx(now, year, mon);
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
            if(strlen(caGroupSql) > 0)
            {
                snprintf(sql, sizeof(sql), "select ifnull(count(*), 0) from %s where stime>=%d and stime<=%d and %s", table_name, lstime, lttime, caGroupSql);
            }
            else
            {
                snprintf(sql, sizeof(sql), "select ifnull(count(*), 0) from %s where stime>=%d and stime<=%d", table_name, lstime, lttime);
            }

            iReturn = pasDbOneRecord(sql, 0, UT_TYPE_LONG, 4, &collect_number);
            if(iReturn != 0)
            {
                utPltPutVar(psDbHead, "result", "0");
                utPltOutToHtml(iFd, psMsgHead, psDbHead, "mac/frontPage/macTermCollectNumber.htm");
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
            //printf("hour %d caStemptime=%s\n", hour, caStemptime);
            lttime = utTimStrToLong("%Y/%m/%d %H:%M", caStemptime);
        }
        //最后一个小时点
        memset(sql, 0, sizeof(sql));
        /*
        if(strlen(caServicecodes) > 0)
        {
            snprintf(sql, sizeof(sql), "select count(*) from %s where stime>=%d and stime<=%d and servicecode in (%s)", table_name, lstime, letime, caServicecodes);
        }
        */
        if(strlen(caGroupSql) > 0)
        {
            snprintf(sql, sizeof(sql), "select count(*) from %s where stime>=%d and stime<=%d and %s", table_name, lstime, letime, caGroupSql);
        }
        else
        {
            snprintf(sql, sizeof(sql), "select count(*) from %s where stime>=%d and stime<=%d ", table_name, lstime, letime);
        }
        iReturn = pasDbOneRecord(sql, 0, UT_TYPE_LONG, 4, &collect_number);
        if(iReturn != 0)
        {
            utPltPutVar(psDbHead, "result", "0");
            utPltOutToHtml(iFd, psMsgHead, psDbHead, "mac/frontPage/macTermCollectNumber.htm");
            return -1;
        }
        utPltPutLoopVar(psDbHead, "dh", iNum, ",");
        utPltPutLoopVarF(psDbHead, "collect_number", iNum, "%lu", collect_number);
        utPltPutLoopVarF(psDbHead, "collect_time", iNum, "%lu", letime);
    }
    else if(strcmp(request_type, "week") == 0)
    {
        iReturn = doCalcByDays(7, psMsgHead, iFd, psDbHead, caGroupSql);
        if(iReturn)
            return iReturn;
    }
    else if(strcmp(request_type, "month") == 0)
    {
        iReturn = doCalcByDays(30, psMsgHead, iFd, psDbHead, caGroupSql);
        if(iReturn)
            return iReturn;
    }
    else if(strcmp(request_type, "season") == 0)
    {
        //把本季度的
        iReturn = doCalcByMonths(3, psMsgHead, iFd, psDbHead, caGroupSql);
        if(iReturn)
            return iReturn;
    }
    else if(strcmp(request_type, "year") == 0)
    {
        iReturn = doCalcByMonths(12, psMsgHead, iFd, psDbHead, caGroupSql);
        if(iReturn)
            return iReturn;
    }
    else
    {
        //类型不对
        utPltPutVar(psDbHead, "result", "2");//2代表类型不对
        utPltOutToHtml(iFd, psMsgHead, psDbHead, "mac/frontPage/macTermCollectNumber.htm");
        return 0;
    }
    //返回数据给前端
    utPltPutVar(psDbHead, "result", "1");
    utPltOutToHtml(iFd, psMsgHead, psDbHead, "mac/frontPage/macTermCollectNumber.htm");
    return 0;
}

long getMacOnlineCount(utShmHead *psShmHead, char* caServicecodes)
{
    pasHashInfo sHashInfo;
    uchar *pHash;
    ncApSrvOnline *psAp;
    int iReturn;
    ncLbsMacLocation *psMacLoc;
    long onlineCount = 0;
    long frontPageDebug = utComGetVar_ld(psShmHead, "FrontDebug", 0);
    pHash = (unsigned char *)utShmHashHead(psShmHead, NC_LNK_MACLOCATION);
    if(pHash)
    {
        psMacLoc = (ncLbsMacLocation *)pasHashFirst(pHash, &sHashInfo);
        // printf("psMacLoc get count=0\n");
        int count = 0;
        while(psMacLoc)
        {
            if(strlen(caServicecodes) > 0)
            {
                psAp = (ncApSrvOnline *)ncMacLogGetOnlineApByMac(psShmHead, psMacLoc->apmac);
                if(psAp)
                {
                    //printf("psAp->servicecode=%s\n", psAp->servicecode);
                    if((strlen(psAp->servicecode) > 0) && strstr(caServicecodes, psAp->servicecode))
                    {
                        onlineCount ++;
                    }
                }
            }
            else
            {
                onlineCount ++;
            }


            /*
            count ++;
            printf("lasttime=%s\n", utTimFormat("%Y-%m-%d %H:%M:%S",psMacLoc->lasttime));
            if(time(0) - psMacLoc->lasttime < MAX_TIMEOUT_SECONDS)//15分钟以内的都算在线
            //if(1 == psAp->status)
            {
                onlineCount ++;
            }
            */

            /*
            psAp = (ncApSrvOnline *)ncMacLogGetOnlineApByMac(psShmHead, psMacLoc->apmac);
            if(psAp)
            {
            printf("lasttime=%s\n", utTimFormat("%Y-%m-%d %H:%M:%S",psOnline->lasttime));
            if(time(0) - psAp->lasttime < MAX_TIMEOUT_SECONDS)//15分钟以内的都算在线
            //if(1 == psAp->status)
            {
                onlineCount ++;
            }
            }
            */
            psMacLoc = (ncLbsMacLocation *)pasHashNextS(&sHashInfo);
        }
        //printf("psMacLoc count=%d\n", count);
    }
    else
    {
        macPrint(frontPageDebug, "NC_LNK_MACLOCATION is NULL\n");
    }
    return onlineCount;
}

typedef struct
{
    long onlineAp;
    long totalAp;
    long onlineMobile;
    long totalMobile;
} ApCount;

int getApCount(utShmHead *psShmHead, ApCount* pApCount, char* caServicecodes)
{
    ncApSrvOnline *psOnline;
    pasHashInfo     sHashInfo;
    ncDeptinfo      *psDept;
    int iReturn;
    uint4 lBase;
    uchar *pHash;
    lBase = utShmGetBaseAddr(psShmHead);
    pHash = utShmHashHead(psShmHead, NC_LNK_APSRVONLINE);
    if(pHash == NULL)
    {
        printf("NC_LNK_APSRVONLINE Memory Error \n");
        return (-1);
    }

    long maxTimeoutSeconds = utComGetVar_ld(psShmHead, "MaxTimeoutSecs", MAX_TIMEOUT_SECONDS);
    long frontPageDebug = utComGetVar_ld(psShmHead, "FrontDebug", 0);

    macPrint(frontPageDebug, "maxTimeoutSeconds=%lu\n", maxTimeoutSeconds);

    int isInServiceList = 0;
    psOnline = (ncApSrvOnline *)pasHashFirst(pHash, &sHashInfo);
    while(psOnline)
    {
        isInServiceList = 1;
        if(strlen(caServicecodes) > 0)
        {
            //printf("psOnline->servicecode=%s\n", psOnline->servicecode);
            if((strlen(psOnline->servicecode) > 0) && strstr(caServicecodes, psOnline->servicecode))
            {
                //printf("in servicecode=%s,caServicecodes=%s\n", psOnline->servicecode,caServicecodes);
                isInServiceList = 1;//表示在列表中
            }
            else
            {
                isInServiceList = 0;//表示没在列表中
            }
        }
        if(isInServiceList)
        {
            //总数
            pApCount->totalAp++;
            if(psOnline->mtype == 2)
            {
                pApCount->totalMobile++;
            }
            //在线数
            //if(psOnline->status == 1){
            if(time(0) - psOnline->lasttime < maxTimeoutSeconds)
            {
                pApCount->onlineAp++;
                if(psOnline->mtype == 2)
                {
                    pApCount->onlineMobile++;
                }
            }
        }
        psOnline = (ncApSrvOnline *)pasHashNextS(&sHashInfo);
    }
    return 0;
}

static int isInOnlinePlaceMemory(utShmHead *psShmHead, char* placename)
{
    char *pHash;
    pasHashInfo sHashInfo;
    int  iCount;
    char caTemp[64];
    uint4 lTime;
    unsigned long lBase;
    ncsOnline *psOnline;
    ncsClient *psClient;
    int i, iReturn;

    pHash = (unsigned char *)utShmHashHead(psShmHead, NCS_LNK_ONLINE);
    if(pHash == NULL)
    {
        printf(" NCS_LNK_ONLINE Error");
        return (-1);
    }
    lBase = utShmGetBaseAddr(psShmHead);
    lTime = time(0);
    psOnline  = (ncsOnline *)pasHashFirst(pHash, &sHashInfo);
    while(psOnline)
    {
        iCount ++;
        psClient = (ncsClient *)ncsUtlGetClientById(psShmHead, psOnline->userid);
        //printf("mem %s  === pass %s\n", utStrncpy(caTemp,psClient->username,39), placename);
        if(strcmp(utStrncpy(caTemp, psClient->username, 39), placename) == 0)
        {
            //printf("placename=%s, %lu == %lu\n", placename, psClient->lLastTime, lTime);
            return 1;
            /*
            if(psClient->lLastTime - lTime>PLACE_TIMEOUT_SECONDS)
                return 0;
            else
                return 1;
                */
        }
        psOnline = (ncsOnline *)pasHashNextS(&sHashInfo);
    }
    return 0;
}

int getMacCount(utShmHead* psShmHead, long* pTodayTermMac, long* pTotalTermMac)
{
    uint8 lUserId = 0;
    int iReturn = dsCltGetMyInfo(1, "Userid", &lUserId);
    if(iReturn == 0 && lUserId > 0)
    {
        ncsMacFrontpageObj* psData = (ncsMacFrontpageObj *)utShmHashLookA(psShmHead, NCS_LNK_FRONTPAGE, (char*)(&lUserId));
        if(psData)
        {
            psData->lastAskTime = time(0);
            (*pTodayTermMac) = psData->todayMacCount;
            (*pTotalTermMac) = psData->totalMacCount;
        }
    }
    return 0;
}


int macFrontPageLeftBlocks(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead)
{
    long frontPageDebug = utComGetVar_ld(psShmHead, "FrontDebug", 0);

    if(frontPageDebug)
    {
        utMsgPrintMsg(psMsgHead);
    }
    utPltDbHead *psDbHead = utPltInitDb();

    long onlineTerms = 0;
    long todayTermMac = 0;
    long totalTermMac = 0;
    long onlineCollectDevices = 0;
    long totalCollectDevices = 0;
    long realIdentities = 0;
    long virtualIdentities = 0;
    long currentRealAlarms = 0;
    long deployAlarms = 0;
    long onlinePlaces = 0;
    long totalPlaces = 0;
    long onlineCollectMobiles = 0;
    long totalCollectMobiles = 0;
    long illegalSpots = 0;
    long totalSpots = 0;
    //共享内存相关变量
    unsigned char *pHash;
    char groupid[32];
    struct servicedata_s *pAreastatdata;

    char sql[8024] = "";
    char caServicecodes[8024] = "";
    //char caGroupcodes[8024] = "";
    char caGroupSql[8024] = "";
    int iReturn = 0;
    //strcpy(caGroupcodes, (char*)getDsGroupcode());
    strcpy(caGroupSql, (char*)getDsGroupcodeSql("servicecode"));
    strcpy(caServicecodes, getServicecodesByDids(psShmHead, getDsGroupids()));

    //printf("caServicecodes=[%s]\n", caServicecodes);
    char askApAlarmCount[2] = "";
    iReturn = utMsgGetSomeNVar(psMsgHead, 2,
                               "groupid",  UT_TYPE_STRING, sizeof(groupid) - 1, groupid,
                               "askApAlarmCount", UT_TYPE_STRING, sizeof(askApAlarmCount) - 1, askApAlarmCount);
    if(strcmp(askApAlarmCount, "1") == 0)
    {
        //获取设备离线告警数量:
        long iApAlarmCount = 0;
        unsigned char* pHash = NULL;
        ncApSrvOnline *psDevOnline;
        pasHashInfo sHashInfo;
        long lTime = 0;
        long  maxTimeoutSeconds = utComGetVar_ld(psShmHead, "MaxTimeoutSecs", MAX_TIMEOUT_SECONDS);

        pHash = utShmHashHead(psShmHead, NC_LNK_APSRVONLINE);
        if(pHash == NULL)
        {
            printf("NC_LNK_APSRVONLINE Memory Error \n");
            return (-1);
        }
        lTime = time(0);
        psDevOnline = (ncApSrvOnline *)pasHashFirst(pHash, &sHashInfo);
        while(psDevOnline)
        {
            if(lTime - psDevOnline->lasttime > maxTimeoutSeconds)
            {
                //离线设备
                iApAlarmCount++;
            }
            else
            {
                //在线设备
            }
            psDevOnline = (ncApSrvOnline *)pasHashNextS(&sHashInfo);
        }
        utPltPutVarF(psDbHead, "ApAlarmCount", "%lu", iApAlarmCount);

        //当前实时离线告警数量
        memset(sql, 0, sizeof(sql));
        if(strlen(caGroupSql) > 0)
        {
            snprintf(sql, sizeof(sql), "select count(*) from (select description from ncsuser,nctermsysalarm where ncsuser.username=nctermsysalarm.description and  alarmcode='10007' and status=0 and %s) as aa", getDsGroupcodeSql("description"));
        }
        else
        {
            snprintf(sql, sizeof(sql), "select count(*) from (select description from ncsuser,nctermsysalarm where ncsuser.username=nctermsysalarm.description and  alarmcode='10007' and status=0) as aa");

        }

        iReturn = pasDbOneRecord(sql, 0, UT_TYPE_LONG, 4, &currentRealAlarms);
        utPltPutVarF(psDbHead, "currentRealAlarms", "%lu", currentRealAlarms);
        utPltPutVar(psDbHead, "result", "0");
        utPltOutToHtml(iFd, psMsgHead, psDbHead, "mac/frontPage/leftBlock.htm");
        //printf("&groupid@1&askApAlarmCount@1...........\n");
        return -1;
    }
    else
    {

        //在线终端数
        onlineTerms = getMacOnlineCount(psShmHead, caServicecodes);
        //1、根据当前年月生成表名
        char table_name[1024] = "";
        char  year[10];
        char  mon[10];
        char caStemptime[24];
        memset(year, 0, sizeof(year));
        memset(mon, 0 , sizeof(mon));
        time_t now;
        time(&now);
        timeToStringEx(now, year, mon);
        int syear = atoi(year);
        int smonth = atoi(mon);
        snprintf(table_name + strlen(table_name), 1024 - strlen(table_name), "ncmactermatt_if_%4u%02u", syear, smonth);
        //2、统计当天目前时间以前的所有的小时
        long letime = time(0);
        sprintf(caStemptime, "%s 00:00", utTimFormat("%Y/%m/%d", letime));
        //printf("caStemptime=%s,leTime=%lu\n", caStemptime, letime);
        //当天的0点
        long lstime = utTimStrToLong("%Y/%m/%d %H:%M", caStemptime);

        //统计今日终端数量和终端总数
        getMacCount(psShmHead, &todayTermMac, &totalTermMac);



        // DO:在线设备和设备总数,在线移动设备和移动设备总数
        ApCount mApCount;
        memset(&mApCount, 0, sizeof(ApCount));
        iReturn = getApCount(psShmHead, &mApCount, caServicecodes);
        if(iReturn == 0)
        {
            onlineCollectDevices = mApCount.onlineAp;
            onlineCollectMobiles = mApCount.onlineMobile;
            totalCollectDevices = mApCount.totalAp;
            totalCollectMobiles = mApCount.totalMobile;
        }
        /*
        // DO: 实名身份和虚拟身份
        memset(sql, 0, sizeof(sql));
        if(strlen(caGroupSql) > 0)
        {
            snprintf(sql, sizeof(sql), "select ifnull(count(*),0) from %s where vname!='' and vtype=0 and %s", table_name, caGroupSql);
        }
        else
        {
            snprintf(sql, sizeof(sql), "select ifnull(count(*),0) from %s where vname!='' and vtype=0", table_name);
        }

        iReturn = pasDbOneRecord(sql, 0, UT_TYPE_LONG, 4, &realIdentities);
        if(iReturn != 0)
        {
            printf("err sql=%s,iRet=%d\n", sql, iReturn);
            utPltPutVar(psDbHead, "result", "0");
            utPltOutToHtml(iFd, psMsgHead, psDbHead, "mac/frontPage/leftBlock.htm");
            return -1;
        }
        memset(sql, 0, sizeof(sql));
        if(strlen(caGroupSql) > 0)
        {
            snprintf(sql, sizeof(sql), "select ifnull(count(*),0) from %s where vname!='' and vtype=1 and %s", table_name, caGroupSql);
        }
        else
        {
            snprintf(sql, sizeof(sql), "select ifnull(count(*),0) from %s where vname!='' and vtype=1", table_name);
        }

        iReturn = pasDbOneRecord(sql, 0, UT_TYPE_LONG, 4, &virtualIdentities);
        if(iReturn != 0)
        {
            printf("err sql=%s,iRet=%d\n", sql, iReturn);
            utPltPutVar(psDbHead, "result", "0");
            utPltOutToHtml(iFd, psMsgHead, psDbHead, "mac/frontPage/leftBlock.htm");
            return -1;
        }
        */
        //当前实时离线告警数量
        memset(sql, 0, sizeof(sql));
        /*
        if(strlen(caServicecodes)>0)
        {
            snprintf(sql, sizeof(sql), "select ifnull(count(*), 0) from nctermsysalarm where alarmcode='10007' and status=0 and description in (%s)", caServicecodes);
        }
        */
        if(strlen(caGroupSql) > 0)
        {
            snprintf(sql, sizeof(sql), "select count(*) from (select description from ncsuser,nctermsysalarm where ncsuser.username=nctermsysalarm.description and  alarmcode='10007' and status=0 and %s) as aa", getDsGroupcodeSql("description"));
            //snprintf(sql, sizeof(sql), "select ifnull(count(*), 0) from nctermsysalarm where alarmcode='10007' and status=0 and %s", getDsGroupcodeSql("description"));
        }
        else
        {
            snprintf(sql, sizeof(sql), "select count(*) from (select description from ncsuser,nctermsysalarm where ncsuser.username=nctermsysalarm.description and  alarmcode='10007' and status=0) as aa");
            //snprintf(sql, sizeof(sql), "select ifnull(count(*), 0) from nctermsysalarm where alarmcode='10007' and status=0");
        }

        iReturn = pasDbOneRecord(sql, 0, UT_TYPE_LONG, 4, &currentRealAlarms);
        if(iReturn != 0)
        {
            printf("errsql=%s,iRet=%d\n", sql, iReturn);
            utPltPutVar(psDbHead, "result", "0");
            utPltOutToHtml(iFd, psMsgHead, psDbHead, "mac/frontPage/leftBlock.htm");
            return -1;
        }

        //设备离线告警数量
        long iApAlarmCount = 0;
        // unsigned char* pHash = NULL;
        ncApSrvOnline *psDevOnline;
        pasHashInfo sHashInfo;
        long lTime = 0;
        long  maxTimeoutSeconds = utComGetVar_ld(psShmHead, "MaxTimeoutSecs", MAX_TIMEOUT_SECONDS);

        pHash = utShmHashHead(psShmHead, NC_LNK_APSRVONLINE);
        if(pHash == NULL)
        {
            printf("NC_LNK_APSRVONLINE Memory Error \n");
            return (-1);
        }
        lTime = time(0);
        psDevOnline = (ncApSrvOnline *)pasHashFirst(pHash, &sHashInfo);
        while(psDevOnline)
        {
            if(lTime - psDevOnline->lasttime > maxTimeoutSeconds)
            {
                //离线数
                iApAlarmCount++;
            }
            else
            {
                //在线数
            }
            psDevOnline = (ncApSrvOnline *)pasHashNextS(&sHashInfo);
        }
        utPltPutVarF(psDbHead, "ApAlarmCount", "%lu", iApAlarmCount);


        //布控告警待处理数
        memset(sql, 0, sizeof(sql));
        /*
        if(strlen(caServicecodes) > 0)
        {
            snprintf(sql, sizeof(sql), "select ifnull(count(*), 0) from ncscasemacwarnlog where flags=0 and servicecode in (%s)", caServicecodes);
        }
        */
        if(strlen(caGroupSql) > 0)
        {
            snprintf(sql, sizeof(sql), "select ifnull(count(*), 0) from ncscasemacwarnlog where flags=0 and %s", caGroupSql);
        }
        else
        {
            snprintf(sql, sizeof(sql), "select ifnull(count(*), 0) from ncscasemacwarnlog where flags=0");
        }

        iReturn = pasDbOneRecord(sql, 0, UT_TYPE_LONG, 4, &deployAlarms);
        if(iReturn != 0)
        {
            utPltPutVar(psDbHead, "result", "0");
            utPltOutToHtml(iFd, psMsgHead, psDbHead, "mac/frontPage/leftBlock.htm");
            return -1;
        }
        //在线场所和总场所
        ncsOnline *psOnline;
        ncsClient *psClient;
        long offlinePlaces = 0;
        psClient = (ncsClient *)utShmArray(psShmHead, NCS_LNK_CLIENT);
        if(psClient)
        {
            int lSumUser;
            int i = 0;
            lSumUser = utShmArrayRecord(psShmHead, NCS_LNK_CLIENT);
            for(i = 0; i < lSumUser; i++)
            {

                macPrint(frontPageDebug, "place username=[%s], status=[%d]\n",  psClient[i].username,  psClient[i].status);

                psOnline = (ncsOnline *)ncsUtlGetOnlineById(psShmHead, psClient[i].userid);
                //只取该区域的如果列表为空则表示全部
                if((strlen(caServicecodes) == 0) || (strstr(caServicecodes, psClient[i].username)))
                {
                    if(isInOnlinePlaceMemory(psShmHead, psClient[i].username))
                    {
                        onlinePlaces++;
                    }
                    else
                    {
                        offlinePlaces++;
                    }

                }
            }
        }
        totalPlaces = offlinePlaces + onlinePlaces;
        //采集非合规热点数、采集热点信息总数
        memset(sql, 0, sizeof(sql));
        snprintf(sql, sizeof(sql), "select ifnull(count(*), 0) from nchotspotinfo where authflag=0");
        iReturn = pasDbOneRecord(sql, 0, UT_TYPE_LONG, 4, &illegalSpots);
        if(iReturn != 0)
        {
            printf("errsql=%s,iRet=%d\n", sql, iReturn);
            utPltPutVar(psDbHead, "result", "0");
            utPltOutToHtml(iFd, psMsgHead, psDbHead, "mac/frontPage/leftBlock.htm");
            return -1;
        }
        memset(sql, 0, sizeof(sql));
        snprintf(sql, sizeof(sql), "select ifnull(count(*), 0) from nchotspotinfo");
        iReturn = pasDbOneRecord(sql, 0, UT_TYPE_LONG, 4, &totalSpots);
        if(iReturn != 0)
        {
            printf("errsql=%s,iRet=%d\n", sql, iReturn);
            utPltPutVar(psDbHead, "result", "0");
            utPltOutToHtml(iFd, psMsgHead, psDbHead, "mac/frontPage/leftBlock.htm");
            return -1;
        }

        //返回数据给前端
        utPltPutVar(psDbHead, "result", "1");
        utPltPutVarF(psDbHead, "onlineTerms", "%lu", onlineTerms);
        utPltPutVarF(psDbHead, "todayTermMac", "%lu", todayTermMac);
        utPltPutVarF(psDbHead, "totalTermMac", "%lu", totalTermMac);
        utPltPutVarF(psDbHead, "onlineCollectDevices", "%lu", onlineCollectDevices);
        utPltPutVarF(psDbHead, "totalCollectDevices", "%lu", totalCollectDevices);
        utPltPutVarF(psDbHead, "realIdentities", "%lu", realIdentities);
        utPltPutVarF(psDbHead, "virtualIdentities", "%lu", virtualIdentities);
        utPltPutVarF(psDbHead, "currentRealAlarms", "%lu", currentRealAlarms);
        utPltPutVarF(psDbHead, "deployAlarms", "%lu", deployAlarms);
        utPltPutVarF(psDbHead, "onlinePlaces", "%lu", onlinePlaces);
        utPltPutVarF(psDbHead, "totalPlaces", "%lu", totalPlaces);
        utPltPutVarF(psDbHead, "onlineCollectMobiles", "%lu", onlineCollectMobiles);
        utPltPutVarF(psDbHead, "totalCollectMobiles", "%lu", totalCollectMobiles);
        utPltPutVarF(psDbHead, "illegalSpots", "%lu", illegalSpots);
        utPltPutVarF(psDbHead, "totalSpots", "%lu", totalSpots);
        utPltOutToHtml(iFd, psMsgHead, psDbHead, "mac/frontPage/leftBlock.htm");
    }
    return 0;
}

static char* replaceAll(char* src, char oldChar, char newChar)
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

static void removeAll(char* src, char ch)
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


void getStdFormat(char* str)
{
    removeAll(str, '-');
    removeAll(str, ':');
    // TODO:其他模式
}

static int8 isDigital(char* words)
{
    int i = 0;
    for(i = 0; i < strlen(words); i++)
    {
        if((words[i] > '9') || (words[i] < '0'))
            return 0;
    }
    return 1;
}

static int decideSearchType(char* keyword)
{
    int type = 0;
    if(strlen(keyword) == 11)
    {
        if(isDigital(keyword))
            type = 1;
    }
    return type;
}


static int doSearchPlace(char* keyword, utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead)
{
    char sql[2048] = "";
    char strCondition[1024] = "";
    long frontPageDebug = utComGetVar_ld(psShmHead, "FrontDebug", 0);
    utPltDbHead *psDbHead = utPltInitDb();
    memset(sql, 0, sizeof(sql));
    snprintf(sql, sizeof(sql), "select dispname,username from ncsuser where 1=1 ");
    if(strlen(keyword) > 0)
    {
        sprintf(strCondition + strlen(strCondition),  " and dispname like ('%c%s%c') ", '%', keyword, '%');
    }
    snprintf(sql + strlen(sql), sizeof(sql) - strlen(sql), "%s limit 100", strCondition);
    int iNum = 0;
    int iret = 0;
    pasDbCursor *psCur = NULL;

    macPrint(frontPageDebug, "search place sql=[%s]\n", sql);

    psCur = pasDbOpenSql(sql, 0);
    if(psCur != NULL)
    {
        char caUsername[16] = "";
        char caDispname[255] = "";
        while(0 == (iret = pasDbFetchInto(psCur, UT_TYPE_STRING, sizeof(caDispname) - 1, caDispname, UT_TYPE_STRING, sizeof(caUsername) - 1, caUsername)) || 1405 == iret)
        {
            iNum++;
            if(iNum > 1)
            {
                utPltPutLoopVar(psDbHead, "dh", iNum, ",");
            }
            utPltPutLoopVar(psDbHead, "username", iNum, caUsername);
            utPltPutLoopVar(psDbHead, "dispname", iNum, caDispname);
            memset(caDispname, 0, sizeof(caDispname));
            memset(caUsername, 0, sizeof(caUsername));
        }
    }
    else
    {
        return 1;
    }
    if(iNum < 1)
        return 1;
    pasDbCloseCursor(psCur);
    utPltPutVarF(psDbHead, "totalCount", "%lu", iNum);
    utPltPutVarF(psDbHead, "result", "%lu", 2);
    utPltPutVarF(psDbHead, "itemid", "%s", "id_infoquery_mac");
    utPltOutToHtml(iFd, psMsgHead, psDbHead, "mac/frontPage/placeSearch.htm");

    return 0;
}


static int doSearchPhone(char* keyword, utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead)
{
    char sql[2048] = "";
    char strCondition[1024] = "";
    long frontPageDebug = utComGetVar_ld(psShmHead, "FrontDebug", 0);
    utPltDbHead *psDbHead = utPltInitDb();
    memset(sql, 0, sizeof(sql));
    snprintf(sql, sizeof(sql), "select distinct username from nctermmacs where 1=1 ");
    if(strlen(keyword) > 0)
    {
        sprintf(strCondition + strlen(strCondition),  " and username like ('%c%s%c') ", '%', keyword, '%');
    }
    snprintf(sql + strlen(sql), sizeof(sql) - strlen(sql), "%s limit 100", strCondition);
    int iNum = 0;
    int iret = 0;
    pasDbCursor *psCur = NULL;

    macPrint(frontPageDebug, "search phone sql=[%s]\n", sql);

    psCur = pasDbOpenSql(sql, 0);
    if(psCur != NULL)
    {
        char phone[15] = "";
        while(0 == (iret = pasDbFetchInto(psCur, UT_TYPE_STRING, sizeof(phone) - 1, phone)) || 1405 == iret)
        {
            iNum++;
            if(iNum > 1)
            {
                utPltPutLoopVar(psDbHead, "dh", iNum, ",");
            }
            utPltPutLoopVar(psDbHead, "mac", iNum, phone);
            memset(phone, 0, sizeof(phone));
        }
    }
    pasDbCloseCursor(psCur);
    utPltPutVarF(psDbHead, "totalCount", "%lu", iNum);
    utPltPutVarF(psDbHead, "result", "%lu", 1);
    utPltPutVarF(psDbHead, "itemid", "%s", "id_infoquery_mac");
    utPltOutToHtml(iFd, psMsgHead, psDbHead, "mac/frontPage/macSearch.htm");
    return 0;
}


static int doSearchMac(char* keyword, utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead)
{
    char sql[2048] = "";
    char strCondition[1024] = "";
    utPltDbHead *psDbHead = utPltInitDb();

    char  year[10] = "";
    char  mon[10] = "";
    time_t now;
    time(&now);
    timeToStringEx(now, year, mon);
    int syear = atoi(year);
    int smonth = atoi(mon);
    memset(sql, 0, sizeof(sql));
    snprintf(sql, sizeof(sql), "select distinct replace(mac,'-','') from ncmactermatt_if_%4u%02u where 1=1 ", syear, smonth);

    /*
    memset(sql, 0, sizeof(sql));
    snprintf(sql, sizeof(sql), "select distinct replace(mac,'-','') from nctermmacs where 1=1 ");
    */
    long frontPageDebug = utComGetVar_ld(psShmHead, "FrontDebug", 0);

    macPrint(frontPageDebug, "before format keyword=[%s]\n", keyword);

    getStdFormat(keyword);

    macPrint(frontPageDebug, "after format keyword=[%s]\n", keyword);

    if(strlen(keyword) > 0)
    {
        sprintf(strCondition + strlen(strCondition),  " and upper(replace(mac,'-','')) like upper('%c%s%c') ", '%', keyword, '%');
    }
    snprintf(sql + strlen(sql), sizeof(sql) - strlen(sql), "%s limit 100", strCondition);
    int iNum = 0;
    int iret = 0;
    pasDbCursor *psCur = NULL;


    macPrint(frontPageDebug, "search sql=[%s]\n", sql);

    psCur = pasDbOpenSql(sql, 0);
    if(psCur != NULL)
    {
        char mac[64] = "";
        while(0 == (iret = pasDbFetchInto(psCur, UT_TYPE_STRING, sizeof(mac) - 1, mac)) || 1405 == iret)
        {
            iNum++;
            if(iNum > 1)
            {
                utPltPutLoopVar(psDbHead, "dh", iNum, ",");
            }
            utPltPutLoopVar(psDbHead, "mac", iNum, mac);
            memset(mac, 0, sizeof(mac));
        }
    }
    pasDbCloseCursor(psCur);
    utPltPutVarF(psDbHead, "totalCount", "%lu", iNum);
    utPltPutVarF(psDbHead, "result", "%lu", 0);
    utPltPutVarF(psDbHead, "itemid", "%s", "id_infoquery_mac");
    utPltOutToHtml(iFd, psMsgHead, psDbHead, "mac/frontPage/macSearch.htm");
    return 0;
}


int macFrontPageSearch(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead)
{
    long frontPageDebug = utComGetVar_ld(psShmHead, "FrontDebug", 0);

    if(frontPageDebug)
    {
        utMsgPrintMsg(psMsgHead);
    }
    //共享内存相关变量
    char groupid[32] = "";
    char keyword[256] = "";
    int iReturn = 0;

    iReturn = utMsgGetSomeNVar(psMsgHead, 2,
                               "groupid",  UT_TYPE_STRING, sizeof(groupid) - 1, groupid,
                               "keyword", UT_TYPE_STRING, sizeof(keyword) - 1, keyword);
    iReturn = doSearchPlace(keyword, psShmHead, iFd, psMsgHead);
    if(iReturn == 1)
    {
        // DO: 根据keyword的值判断是什么类型的查询
        switch(decideSearchType(keyword))
        {
            case 0://查询mac地址
                iReturn = doSearchMac(keyword, psShmHead, iFd, psMsgHead);
                break;
            case 1://查询手机号
                iReturn = doSearchPhone(keyword, psShmHead, iFd, psMsgHead);
                break;
            case 2://查询场所
                iReturn = doSearchPlace(keyword, psShmHead, iFd, psMsgHead);
                break;
            default:
                break;
        }
    }
    return iReturn;
}

struct db_nctermsysalarm
{
    uint4 sid;//    int   unsigned auto_increment primary key,
    char alarmcode[14];//     char(14),                                -- ?????
    char alarmlevel[32];//    char(32) default ' ',                -- ???? 1-?? 2-?? 3-??
    char devtype[4];//       char(4),                             -- 01 -AP 02-?? 03-???? 04-????
    uint4 alarmtime;//          int default 0,                         -- ????
    uint4 updatetime;//         int default 0,                  -- ??????
    char description[128];//          char(128),                          -- ????
    uint4 status;//           int default 0,                    -- 0??? 1-???
};

static int insertOperationAlarmToDB(struct db_nctermsysalarm* pPlaceData)
{
    char sql[2048] = "";
    long lCount = 0;
    int iReturn = 0;
    //先读取一下是否已经存在该场所的未确认告警信息
    memset(sql, 0, sizeof(sql));
    sprintf(sql, "select count(*) from nctermsysalarm where alarmcode='%s' and devtype='%s' and description='%s' and status=%lu",
            pPlaceData->alarmcode, pPlaceData->devtype, pPlaceData->description, pPlaceData->status);
    //  printf("sql=%s\n", sql);
    iReturn = pasDbOneRecord(sql, 0, UT_TYPE_LONG, 4, &lCount);
    if(iReturn != 0 || lCount > 0)
    {
        // printf("sql=%s, iret=%d, lcount=%d\n", sql, iReturn, lCount);
        return 0;
    }
    //插入到数据库
    memset(sql, 0, sizeof(sql));
    sprintf(sql, "insert into nctermsysalarm(alarmcode,alarmlevel,devtype,alarmtime,updatetime,description,status) \
        	values('%s','%s','%s',%lu,%lu,'%s',%lu) ",
            pPlaceData->alarmcode, pPlaceData->alarmlevel, pPlaceData->devtype, pPlaceData->alarmtime,
            pPlaceData->updatetime, pPlaceData->description, pPlaceData->status);
    printf("sql=%s\n", sql);
    iReturn = pasDbExecSqlF(sql);
    return 0;
}


struct s_userInfo
{
    uint8 userId;
    char name[32];
};

char *getDsGroupcodeSqlByUserId(char *field, ulong lUserId)
{
    static char caReturn[1024];
    char caIds[1024], caTempid[1024];
    char caGroupcode[20];
    char caTempcode[1024];
    char caOpt[256];
    pasLHashInfo sHashInfo;
    int iReturn, iNum;
    pasDbCursor *psCur;
    long lLen;
    typedef struct ncsDsData_s
    {
        long len;                //字段长度
        char codes[1024];
        long count;
    } ncsDsData;
    char *pHash;
    ncsDsData *psData;

    memset(caReturn, 0, sizeof(caReturn));


    if(lUserId <= 0)
    {
        return &caReturn[0];
    }

    memset(caOpt, 0, sizeof(caOpt));
    pasDbOneRecord("select opt from dsuser where id=:id", 1, "id", UT_TYPE_LONG, lUserId, UT_TYPE_STRING, 255, caOpt);
    if(strstr(caOpt, "all") || strlen(caOpt) == 0)
    {
        return &caReturn[0];
    }

    pHash = (unsigned char *)pasLHashInit(100, 100, sizeof(struct ncsDsData_s), 0, 4);
    if(pHash == NULL)
    {
        return &caReturn[0];
    }
    //
    long lId = 0;
    psCur = pasDbOpenSqlF("select groupcode from ncsgroup where   groupid in(%s) ", caOpt);
    if(psCur)
    {
        iReturn = 0;
        lId = 0;
        memset(caGroupcode, 0, sizeof(caGroupcode));
        iReturn = pasDbFetchInto(psCur,
                                 UT_TYPE_STRING, 14, caGroupcode);
        iNum = 0;
        while(iReturn == 0 || iReturn == 1405)
        {
            lLen = strlen(caGroupcode);
            psData = (ncsDsData *)pasLHashLookA(pHash, (char*)&lLen);
            if(psData)
            {
                if(psData->count == 0)
                {
                    sprintf(psData->codes, "\"%s\"", caGroupcode);
                }
                else
                {
                    sprintf(psData->codes + strlen(psData->codes), ",\"%s\"", caGroupcode);
                }
                psData->count++;
            }




            memset(caGroupcode, 0, sizeof(caGroupcode));
            iReturn = pasDbFetchInto(psCur,
                                     UT_TYPE_STRING, 14, caGroupcode);
        }
        pasDbCloseCursor(psCur);
    }


    //       sprintf(caReturn,"%s",caOpt);
    sprintf(caIds, "%s", caOpt);
    iNum = 0;
    while(strlen(caIds) > 0)
    {
        memset(caTempid, 0, sizeof(caTempid));
        iNum = 0;

        psCur = pasDbOpenSqlF("select groupid,groupcode from ncsgroup where   pid in(%s) ", caIds);
        if(psCur)
        {
            iReturn = 0;
            lId = 0;
            memset(caGroupcode, 0, sizeof(caGroupcode));
            iReturn = pasDbFetchInto(psCur, UT_TYPE_ULONG, 4, &lId,
                                     UT_TYPE_STRING, 14, caGroupcode);
            while(iReturn == 0 || iReturn == 1405)
            {
                if(iNum == 100) break;

                lLen = strlen(caGroupcode);
                psData = (ncsDsData *)pasLHashLookA(pHash, (char*)&lLen);
                if(psData)
                {
                    if(psData->count == 0)
                    {
                        sprintf(psData->codes, "\"%s\"", caGroupcode);
                    }
                    else
                    {
                        sprintf(psData->codes + strlen(psData->codes), ",\"%s\"", caGroupcode);
                    }
                    psData->count++;
                }

                if(iNum == 0)
                {
                    sprintf(caTempid, "%lu", lId);

                }
                else
                {
                    sprintf(caTempid + strlen(caTempid), ",%lu", lId);

                }

                memset(caGroupcode, 0, sizeof(caGroupcode));
                iReturn = pasDbFetchInto(psCur, UT_TYPE_ULONG, 4, &lId,
                                         UT_TYPE_STRING, 14, caGroupcode);

                iNum++;

            }
        }
        pasDbCloseCursor(psCur);
        strcpy(caIds, caTempid);
    }

    //拼装SQL语句
    psData = (ncsDsData *)pasLHashFirst(pHash, &sHashInfo);
    iNum = 0;
    memset(caReturn, 0, sizeof(caReturn));
    while(psData)
    {
        if(iNum == 0)
        {
            sprintf(caReturn, "( left(%s,%d) in (%s) ", field, psData->len, psData->codes);
        }
        else
        {
            sprintf(caReturn + strlen(caReturn), " or left(%s,%d) in (%s) ", field, psData->len, psData->codes);
        }
        iNum++;
        psData = (ncsDsData *)pasLHashNext(&sHashInfo);
    }

    if(iNum > 0)
    {
        sprintf(caReturn + strlen(caReturn), ")");
    }
    free(pHash);
    return &caReturn[0];
}


int ncsStatMacFront(utShmHead *psShmHead)
{
    char sql[8024] = "";
    unsigned char* pHash_user = NULL;
    pasDbCursor *psCur;
    struct s_userInfo tmpUser;
    struct s_userInfo* psUser = NULL;
    pasLHashInfo sHashInfo;
    char caGroupSql[8024] = "";
    int iReturn = 0;


    iReturn = pasConnect(psShmHead);
    if(iReturn < 0)
    {
        printf("conn psShmHead fail iReturn=%d\n", iReturn);
        sleep(60);
        return 0;
    }

    utShmFreeHash(psShmHead, NCS_LNK_FRONTPAGE);
    utShmHashInit(psShmHead, NCS_LNK_FRONTPAGE, 2000, 2000, sizeof(ncsMacFrontpageObj), 0, 8);
    while(1)
    {
        //1、所有用户装入内存
        pHash_user = (unsigned char *)pasLHashInit(2000, 2000, sizeof(struct s_userInfo), 0, 8);
        if(pHash_user == NULL)
        {
            printf("分配内存出错\n");
            return 0;
        }
        memset(sql, 0, sizeof(sql));
        sprintf(sql, "select id,name from dsuser");
        psCur = pasDbOpenSql(sql, 0);
        if(psCur)
        {
            memset(&tmpUser, 0, sizeof(tmpUser));
            iReturn = pasDbFetchInto(psCur,
                                     UT_TYPE_LONG8, 8, &tmpUser.userId,
                                     UT_TYPE_STRING, 32, tmpUser.name);

            while((iReturn == 0) || (iReturn == 1405))
            {
                psUser = (struct s_userInfo*)pasLHashLookA(pHash_user, (char*)&tmpUser.userId);
                if(psUser)
                {
                    psUser->userId = tmpUser.userId;
                    strcpy(psUser->name, tmpUser.name);
                }
                memset(&tmpUser, 0, sizeof(tmpUser));
                iReturn = pasDbFetchInto(psCur,
                                         UT_TYPE_LONG8, 8, &tmpUser.userId,
                                         UT_TYPE_STRING, 32, tmpUser.name);
                //printf("======%llu,%s=====\n", tmpUser.userId, tmpUser.name);
            }
            pasDbCloseCursor(psCur);
        }
        char table_name[1024] = "";
        char  year[10];
        char  mon[10];
        char caStemptime[24] = "";
        memset(year, 0, sizeof(year));
        memset(mon, 0 , sizeof(mon));
        time_t now;
        time(&now);
        timeToStringEx(now, year, mon);
        int syear = atoi(year);
        int smonth = atoi(mon);
        snprintf(table_name + strlen(table_name), 1024 - strlen(table_name), "ncmactermatt_if_%4u%02u", syear, smonth);

        //2、统计当天目前时间以前的所有的小时
        long letime = time(0);
        sprintf(caStemptime, "%s 00:00", utTimFormat("%Y/%m/%d", letime));
        //当天的0点
        long lstime = utTimStrToLong("%Y/%m/%d %H:%M", caStemptime);
        //遍历该内存
        psUser = (struct s_userInfo*)pasLHashFirst(pHash_user, &sHashInfo);
        while(psUser)
        {
            //读取共享内存该用户的信息
            ncsMacFrontpageObj* psData = (ncsMacFrontpageObj *)utShmHashLookA(psShmHead, NCS_LNK_FRONTPAGE, (char*)(&psUser->userId));
            if(psData)
            {
                psData->lastModTime = letime;
                //printf("\n====userid=%llu====asktime=%lu\n\n", psUser->userId, psData->lastAskTime);
                if(letime - psData->lastAskTime < 30)
                {
                    memset(caGroupSql, 0, sizeof(caGroupSql));
                    strcpy(caGroupSql, (char*)getDsGroupcodeSqlByUserId("servicecode", psUser->userId));
                    //统计今日终端数量
                    memset(sql, 0, sizeof(sql));
                    if(strlen(caGroupSql) > 0)
                    {
                        snprintf(sql, sizeof(sql), "select ifnull(count(distinct(mac)), 0) from %s where stime>=%d and stime<=%d and %s", table_name, lstime, letime, caGroupSql);
                    }
                    else
                    {
                        snprintf(sql, sizeof(sql), "select ifnull(count(distinct(mac)), 0) from %s where stime>=%d and stime<=%d", table_name, lstime, letime);
                    }
                    iReturn = pasDbOneRecord(sql, 0, UT_TYPE_LONG, 4, &psData->todayMacCount);
                    if(iReturn != 0)
                    {
                        printf("errsql=%s,iRet=%d\n", sql, iReturn);
                    }
                    //统计mac总数
                    memset(sql, 0, sizeof(sql));
                    if(strlen(caGroupSql) > 0)
                    {
                        snprintf(sql, sizeof(sql), "select ifnull(count(distinct(mac)),0) from nctermmacs where %s", caGroupSql);
                    }
                    else
                    {
                        snprintf(sql, sizeof(sql), "select ifnull(count(distinct(mac)),0) from nctermmacs");
                    }

                    iReturn = pasDbOneRecord(sql, 0, UT_TYPE_LONG, 4, &psData->totalMacCount);
                    if(iReturn != 0)
                    {
                        printf("errsql=%s,iRet=%d\n", sql, iReturn);
                    }
                    //printf("totay=%lu, total=%lu\n\n", psData->todayMacCount, psData->totalMacCount);

                    //统计场所采集排行
                    memset(sql, 0, sizeof(sql));

                    if(strlen(caGroupSql) > 0)
                    {
                        snprintf(sql, sizeof(sql), "select  ifnull(sum(lcount),0) as nn, servicecode from ncmactermcount where %s group by servicecode order by nn desc limit 5", caGroupSql);

                    }
                    else
                    {
                        snprintf(sql, sizeof(sql), "select  ifnull(sum(lcount),0) as nn, servicecode from ncmactermcount group by servicecode order by nn desc limit 5");
                    }
                    printf("tongji sql=%s\n", sql);
                    psCur = pasDbOpenSql(sql, 0);
                    memset(psData->sdataList, 0, sizeof(s_serviceData) * 5);
                    iReturn = pasDbFetchInto(psCur,
                                             UT_TYPE_LONG, 4, &psData->sdataList[0].number,
                                             UT_TYPE_STRING, 31, psData->sdataList[0].servicecode);

                    psData->sdataList[0].flag = 1;

                    int iNum = 0;

                    while((iReturn == 0) || (iReturn == 1405))
                    {
                        printf("number=%lu, scode=%s\n", psData->sdataList[iNum].number, psData->sdataList[iNum].servicecode);

                        iNum++;
                        iReturn = pasDbFetchInto(psCur,
                                                 UT_TYPE_LONG, 4, &psData->sdataList[iNum].number,
                                                 UT_TYPE_STRING, 31, psData->sdataList[iNum].servicecode);
                        psData->sdataList[iNum].flag = 1;
                    }
                    pasDbCloseCursor(psCur);

                    //设备采集排行
                    memset(sql, 0, sizeof(sql));

                    if(strlen(caGroupSql) > 0)

                    {

                        sprintf(sql, "select  ifnull(sum(lcount), 0) as nn, right(apname,12) from ncmactermcount where apname!=\'\' and %s group by right(apname,12) order by nn desc limit 5", caGroupSql);

                    }

                    else

                    {

                        sprintf(sql, "select  ifnull(sum(lcount), 0) as nn, right(apname,12) from ncmactermcount where apname!=\'\'  group by right(apname,12) order by nn desc limit 5");

                    }

                    psCur = pasDbOpenSql(sql, 0);

                    memset(psData->deviceList, 0, sizeof(s_serviceData) * 5);

                    iReturn = pasDbFetchInto(psCur,

                                             UT_TYPE_LONG, 4, &psData->deviceList[0].number,

                                             UT_TYPE_STRING, 31, psData->deviceList[0].servicecode);

                    psData->deviceList[0].flag = 1;

                    iNum = 0;

                    while((iReturn == 0) || (iReturn == 1405))

                    {
                        iNum++;

                        iReturn = pasDbFetchInto(psCur,

                                                 UT_TYPE_LONG, 4, &psData->deviceList[iNum].number,

                                                 UT_TYPE_STRING, 31, psData->deviceList[iNum].servicecode);

                        psData->deviceList[iNum].flag = 1;

                    }

                    pasDbCloseCursor(psCur);

                }
            }
            psUser = (struct s_userInfo*)pasLHashNext(&sHashInfo);
        }
        if(pHash_user)
            free(pHash_user);
        //printf("\n\n========休息10秒========\n\n");
        sleep(10);
    }
    return 0;
}

int ncsStatMacDevWarn(utShmHead *psShmHead)
{
    struct s_ncscasetermlim
    {
        long sid;
        long cid;
        long did;
        long countlimit;
    };

    struct s_alarmtask
    {
        char caDescription[128];
        char caDevType[32];
        uint8 alarmtime;
    };

    struct db_nctermsysalarm tmpPlaceData;

    pasLHashInfo sHashInfoTask;
    char caGroupid[6];

    long lTime = 0;


    ncsOnline *psOnline;
    ncsClient *psClient;

    long maxTimeoutSeconds = utComGetVar_ld(psShmHead, "MaxTimeoutSecs", MAX_TIMEOUT_SECONDS);
    ncApSrvOnline *psDevOnline;
    pasHashInfo     sHashInfo;
    ncDeptinfo      *psDept;
    uint4 lBase;
    uchar *pHash;
    //判断场所离线恢复
    char caDevType[32];
    uint4 alarmtime;

    char caDescription[128];
    unsigned char* pHash_alarm = NULL;

    struct s_alarmtask* psTask = NULL;
    int iReturn = pasConnect(psShmHead);
    printf("\n\n*********************************************************************\n\n");
    if(iReturn < 0)
    {
        printf("conn psShmHead fail iReturn=%d\n", iReturn);
        sleep(60);
        return 0;
    }
    while(1)
    {
        //任务1：查出离线场所并加入告警表
        psClient = (ncsClient *)utShmArray(psShmHead, NCS_LNK_CLIENT);
        if(psClient)
        {
            int lSumUser;
            int i = 0;
            lSumUser = utShmArrayRecord(psShmHead, NCS_LNK_CLIENT);
            lTime = time(0);
            for(i = 0; i < lSumUser; i++)
            {
                psOnline = (ncsOnline *)ncsUtlGetOnlineById(psShmHead, psClient[i].userid);
                if(!isInOnlinePlaceMemory(psShmHead, psClient[i].username))
                {
                    //printf("%s is offline\n", psClient[i].username);
                    memset(&tmpPlaceData, 0, sizeof(tmpPlaceData));
                    strcpy(tmpPlaceData.alarmcode, "10007");
                    strcpy(tmpPlaceData.alarmlevel, "1");
                    strcpy(tmpPlaceData.description, psClient[i].username);
                    strcpy(tmpPlaceData.devtype, "02");
                    tmpPlaceData.status = 0;
                    tmpPlaceData.updatetime = lTime;
                    tmpPlaceData.alarmtime = lTime;
                    insertOperationAlarmToDB(&tmpPlaceData);
                }
                else
                {
                }
            }
        }

        //任务2：查出每个场所在线设备总数，统计count，若超过阈值，warnlog插入数据
        char sql[10240] = "";
        unsigned char* pHash_lim = NULL;
        struct s_ncscasetermlim* psTasklim = NULL;
        struct s_ncscasetermlim templim ;
        pasLHashInfo sHashInfoLim;

        //1、统计总数
        int num = 0;
        iReturn = pasDbOneRecord("select count(*) from ncscasetermlim", 0, UT_TYPE_LONG, 4, &num);
        if(iReturn != 0 || num == 0)
        {
            sleep(3 * 60);
            continue;
        }
        printf("=====num=%d\n==========", num);
        //2、创建规则hash
        pHash_lim = (unsigned char *)pasLHashInit(5000, 5000, sizeof(struct s_ncscasetermlim), 0, 4);
        if(pHash_lim == NULL)
        {
            printf("分配内存出错\n");
            return -1;
        }

        memset(sql, 0, sizeof(sql));
        sprintf(sql, "select ncscasetermlim.cid,ncscasetermlim.countlimit,ncscasetermlim.sid,ncscaseobj.did from ncscasetermlim,ncscaseobj where ncscaseobj.cid = ncscasetermlim.cid");
        pasDbCursor *myCur = NULL;
        myCur = pasDbOpenSqlF(sql, 0);
        if(myCur == NULL)
        {
            printf("myCur error!\n");
            return -1;
        }

        int iR = 0;
        memset(&templim, 0, sizeof(struct s_ncscasetermlim));
        iR = pasDbFetchInto(myCur, UT_TYPE_LONG, 4, &templim.cid,
                            UT_TYPE_LONG, 4, &templim.countlimit,
                            UT_TYPE_LONG, 4, &templim.sid,
                            UT_TYPE_LONG, 4, &templim.did);
        //printf("=====iR=%d\n==========",iR);
        while(iR == 0 || iR == 1405)
        {
            psTasklim = (struct s_ncscasetermlim *)pasLHashLookA(pHash_lim, (char*)&templim.sid);
            if(psTasklim)
            {
                psTasklim->cid = templim.cid;
                psTasklim->countlimit = templim.countlimit;
                psTasklim->sid = templim.sid;
                psTasklim->did = templim.did;
            }
            printf("=====templim.cid=%d\n==========", templim.cid);
            printf("=====templim.countlimit=%d\n==========", templim.countlimit);
            printf("=====templim.sid=%d\n==========", templim.sid);
            printf("=====templim.did=%d\n==========", templim.did);

            memset(&templim, 0, sizeof(struct s_ncscasetermlim));
            iR = pasDbFetchInto(myCur, UT_TYPE_LONG, 4, &templim.cid,
                                UT_TYPE_LONG, 4, &templim.countlimit,
                                UT_TYPE_LONG, 4, &templim.sid,
                                UT_TYPE_LONG, 4, &templim.did);
        }
        pasDbCloseCursor(myCur);
        //printf("=====iR2222=%d\n==========",iR);
        psTasklim = (struct s_ncscasetermlim*)pasLHashFirst(pHash_lim, &sHashInfoLim);
        //printf("=====11111111111111\n==========");
        while(psTasklim)
        {
            printf("=====psTasklim->did=%d\n==========", psTasklim->did);
            //遍历一条规则，已知cid,sid,did,countlimit
            char cServiceCodes[8024] = "";
            char cDid[15];
            sprintf(cDid, "%d", psTasklim->did);
            printf("=====cDid=%s\n====", cDid);
            strcpy(cServiceCodes, getServicecodesByDids(psShmHead, cDid));
            printf("=======cid=%d\n", psTasklim->cid);
            printf("=======countlimit=%d\n", psTasklim->countlimit);
            printf("=======sid=%d\n", psTasklim->sid);
            printf("=======did=%d\n", psTasklim->did);
            printf("=======cServiceCodes=%s\n", cServiceCodes);

            char *servicecode = NULL;
            servicecode = strtok(cServiceCodes, ",");
            while(servicecode != NULL)
            {
                printf("=======servicecode=%s\n", servicecode);
                uchar * myHash;
                myHash = utShmHashHead(psShmHead, NC_LNK_APSRVONLINE);
                if(myHash == NULL)
                {
                    printf(" 退出：NC_LNK_APSRVONLINE Memory Error \n");
                    return -1;
                }
                else
                {
                    printf("NC_LNK_APSRVONLINE Memory Success \n");
                }
                ncApSrvOnline * myApOnline;
                pasHashInfo     sHashInfoAp;
                myApOnline = (ncApSrvOnline *)pasHashFirst(myHash, &sHashInfoAp);
                int myApCount = 0;
                while(myApOnline)
                {
                    printf("myApOnline 不为空 myApOnline->servicecode = %s\n", myApOnline->servicecode);
                    if(strlen(myApOnline->servicecode) > 0 && strcmp(myApOnline->servicecode, servicecode) == 0)
                    {
                        if(time(0) - myApOnline->lasttime < maxTimeoutSeconds)
                        {
                            //设备在线
                            myApCount ++ ;
                        }
                        else
                        {
                            //设备离线
                        }
                    }
                    myApOnline = (ncApSrvOnline *)pasHashNextS(&sHashInfoAp);
                }
                printf("myHash 释放前 psTasklim = %p, %d\n", psTasklim, psTasklim->countlimit);

                printf(" 释放后 psTasklim = %p, %d\n", psTasklim, psTasklim->countlimit);

                printf("=======countlimit=%d\n", psTasklim->countlimit);
                printf("=======myApCount=%d=====servicecode=%s\n", myApCount, servicecode);
                if(myApCount > psTasklim->countlimit)
                {
                    //告警：设备在线数超过阈值
                    //已知 did servicecode,
                    memset(sql, 0, sizeof(sql));
                    char dispname[64] = "";
                    sprintf(sql, "select dispname from ncsuser where username =%s", servicecode);
                    iReturn = pasDbOneRecord(sql, 0, UT_TYPE_STRING, 62, dispname);

                    printf("sql = %s, 场所编码为 %s, 场所名称为 %s\n ", sql, servicecode, dispname);
                    char caMessage[1024];
                    memset(caMessage, 0, sizeof(caMessage));
                    snprintf(caMessage , sizeof(caMessage) - 1, "%s", "场所[");
                    snprintf(caMessage + strlen(caMessage), sizeof(caMessage) - 1, "%s", dispname);
                    snprintf(caMessage + strlen(caMessage) , sizeof(caMessage) - 1, "%s", "]下的设备在线总数已经超过阈值");
                    snprintf(caMessage + strlen(caMessage), sizeof(caMessage) - 1, "%d", psTasklim->countlimit);
                    printf("=======caMessage=%s\n", caMessage);
                    iReturn = pasDbExecSqlF("insert into ncscasemacwarnlog(servicecode,mac,stime,ruleid,cid,message,flags) "
                                            " values('%s','%lu','%lu','%lu','%lu','%s','0')",
                                            servicecode,  myApCount, time(0), psTasklim->sid,  psTasklim->cid, caMessage);
                    printf("=======iReturn=%d\n", iReturn);
                }
                else
                {

                }
                if(myHash != NULL)
                {
                    free(myHash);
                }
                servicecode = strtok(NULL, ",");
            }
            psTasklim = (struct s_ncscasetermlim*)pasLHashNext(&sHashInfoLim);
        }
        if(pHash_lim != NULL)
        {
            free(pHash_lim);
        }




        //任务3：统计alarmcode ='10007'告警总数 iSumalarm
        int iSumalarm = 0;
        iReturn = pasDbOneRecord("select count(*) from nctermsysalarm where alarmcode='10007'", 0, UT_TYPE_LONG, 4, &iSumalarm);
        if(iReturn != 0 || iSumalarm == 0)
        {
            sleep(60);
            return 0;
        }

        //2、pas hash
        pHash_alarm = (unsigned char *)pasLHashInit(5000, 5000, sizeof(struct s_alarmtask), 0, 14);
        if(pHash_alarm == NULL)
        {
            printf("分配内存出错\n");
            return 0;
        }
        memset(sql, 0, sizeof(sql));
        strcpy(sql, "select description,devtype,alarmtime from nctermsysalarm where alarmcode ='10007'");
        pasDbCursor *pCur = NULL;
        pCur = pasDbOpenSql(sql, 0);
        if(pCur == NULL)
        {
            return 0;
        }
        iReturn = pasDbFetchInto(pCur, UT_TYPE_STRING, sizeof(caDescription) - 1, caDescription,
                                 UT_TYPE_STRING, sizeof(caDevType) - 1, caDevType,
                                 UT_TYPE_LONG, 4, &alarmtime);
        // printf("===1111111111111===iReturn====%d\n", iReturn); //..
        while(iReturn == 0 || iReturn == 1405)
        {
            //printf("caDescription=%s\n", caDescription);
            psTask = (struct s_alarmtask *)pasLHashLookA(pHash_alarm, caDescription);
            if(psTask)
            {
                strcpy(psTask->caDescription, caDescription);
                strcpy(psTask->caDevType, caDevType);
                psTask->alarmtime = alarmtime;
            }
            memset(caDescription, 0, sizeof(caDescription));
            memset(caDevType, 0, sizeof(caDevType));

            iReturn = pasDbFetchInto(pCur, UT_TYPE_STRING, sizeof(caDescription) - 1, caDescription,
                                     UT_TYPE_STRING, sizeof(caDevType) - 1, caDevType,
                                     UT_TYPE_LONG, 4, &alarmtime);
        }
        pasDbCloseCursor(pCur);

        //3、遍历hash
        psTask = (struct s_alarmtask*)pasLHashFirst(pHash_alarm, &sHashInfoTask);
        long icount = 0;
        while(psTask)
        {
            if(isInOnlinePlaceMemory(psShmHead, psTask->caDescription))
            {
                memset(sql, 0, sizeof(sql));
                snprintf(sql, sizeof(sql) - 1, "select count(*) from nctermsysalarm where alarmcode='10008' and description='%s'", psTask->caDescription);
                int iRes = pasDbOneRecord(sql, 0, UT_TYPE_LONG, 4, &icount);
                //printf("sql=%s,icount=%lu\n", sql, icount);
                if(iRes)
                {
                    memset(sql, 0, sizeof(sql));
                    if(icount == 0)
                    {
                        snprintf(sql, sizeof(sql) - 1, "insert into nctermsysalarm(description,alarmcode,devtype,status,alarmlevel,alarmtime,updatetime)"
                                 "values('%s','10008','%s','1','3','%llu','%llu')",
                                 psTask->caDescription, psTask->caDevType, psTask->alarmtime, time(0));
                    }
                    else
                    {
                        snprintf(sql, sizeof(sql) - 1, "update nctermsysalarm set alarmlevel='3',updatetime='%llu',status='1' where description='%s' and alarmcode='10008'", time(0), psTask->caDescription);
                    }
                    //printf("excute sql=%s\n", sql);
                    pasDbExecSqlF(sql);

                }
            }
            else
            {
            }
            psTask = (struct s_alarmtask*)pasLHashNext(&sHashInfoTask);
        }
        if(pHash_alarm)
            free(pHash_alarm);
        //任务4：查出离线设备插入告警表
        pHash = utShmHashHead(psShmHead, NC_LNK_APSRVONLINE);
        if(pHash == NULL)
        {
            printf("NC_LNK_APSRVONLINE Memory Error \n");
            return (-1);
        }
        lTime = time(0);
        psDevOnline = (ncApSrvOnline *)pasHashFirst(pHash, &sHashInfo);
        while(psDevOnline)
        {
            if(lTime - psDevOnline->lasttime > maxTimeoutSeconds)
            {
                //printf("dev %s is offline\n", psDevOnline->apname);
                memset(&tmpPlaceData, 0, sizeof(tmpPlaceData));
                strcpy(tmpPlaceData.alarmcode, "10011");
                strcpy(tmpPlaceData.alarmlevel, "1");
                strcpy(tmpPlaceData.description, psDevOnline->apname);
                strcpy(tmpPlaceData.devtype, "01");
                tmpPlaceData.status = 0;
                tmpPlaceData.updatetime = lTime;
                tmpPlaceData.alarmtime = lTime;
                insertOperationAlarmToDB(&tmpPlaceData);
            }
            // printf("in while\n");
            psDevOnline = (ncApSrvOnline *)pasHashNextS(&sHashInfo);
        }
        //printf("完成一次任务，休息60s\n");
        sleep(60);
    }
    printf("\n\n*************************************************************************\n\n");
    return 0;
}



//自定义区域树
int ncsTreeUser_v9(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead)
{
    pasDbCursor *psCur;
    char caLevel[16];
    char caPlate[128];
    char caGroupname[72], caGroupid[16];
    char caTemp[4024];
    char caName[32];
    char caWhere[256];

    char caCurPg[16], caTotRec[16];
    char caPid[16];
    unsigned long lLevel, lPid;
    int  iNum;
    char caFilter[256];
    char caNo[16];
    unsigned long lGroupid, lGroupid0, lCurPg, lTotRec, lRowNum, lStartRec;
    utPltDbHead *psDbHead;
    int iReturn, i;
    char caVar[256], caFname[64], caBfname[64];
    char caValue[256];
    char caVar1[32];
    char caOpt[256];
    char caGroups[1024];
    char caFacs[1024];
    long lId;
    char *p, *pVar;
    unsigned char *pHash;
    char caCname[72];
    char caUsername[36];
    pasLHashInfo  sHashInfo;
    struct stData
    {
        unsigned long id;
        unsigned long count;
    };
    long lCount;
    struct stData *psData;
    char caCname1[72];
    char caSql[2048];
    char caWireflag[20];
    char caFaccods[1024];
    char caGroupcode[7] = "";
    utMsgPrintMsg(psMsgHead);
    strcpy(caGroups, (const char*)getDsGroupids());
    strcpy(caFacs, (const char*)getDsFacGroupid());
    //  printf("caFacs=%s\n", caFacs);
    //      strcpy(caFaccods,getDsFacs()));
    memset(caCname, 0, sizeof(caCname));
    iReturn = utMsgGetSomeNVar(psMsgHead, 4,
                               "node",        UT_TYPE_STRING, 10, caPid,
                               "cname",       UT_TYPE_STRING, 30, caCname1,
                               "plate",       UT_TYPE_STRING, 100, caPlate,
                               "wireflag",    UT_TYPE_STRING, 10, caWireflag);
    //部门
    pHash = (unsigned char *)pasLHashInit(1000, 1000, sizeof(struct stData), 0, 4);
    if(pHash == NULL) return 0;
    //所有的非叶子结点
    psCur = pasDbOpenSql("select pid from ncsgroup where pid!=0 ", 0);
    if(psCur)
    {
        iReturn = pasDbFetchInto(psCur, UT_TYPE_ULONG, 4, &lPid);
        while(iReturn == 0)
        {
            psData = (struct stData *)pasLHashLookA(pHash, (char*)&lPid);
            if(psData)
            {
                psData->count++;
            }
            iReturn = pasDbFetchInto(psCur, UT_TYPE_ULONG, 4, &lPid);
        }
        pasDbCloseCursor(psCur);
    }

    /*
    //单位
    psCur = pasDbOpenSql("select distinct groupid from ncsuser  ", 0);
    if(psCur)
    {
        iReturn = pasDbFetchInto(psCur, UT_TYPE_ULONG, 4, &lPid);
        while(iReturn == 0)
        {
            psData = (struct stData *)pasLHashLookA(pHash, (char*)&lPid);
            if(psData)
            {
                psData->count++;
            }
            iReturn = pasDbFetchInto(psCur, UT_TYPE_ULONG, 4, &lPid);
        }
        pasDbCloseCursor(psCur);
    }
    */
    if(strlen(caCname1) > 0)
    {
        strcpy(caCname, pasCvtGBK(2, caCname1, caTemp, 30));
    }
    //    iReturn = dsCltGetSessionValue(1,"groupid",UT_TYPE_LONG,sizeof(long),&lGroupid0);
    strcpy(caWhere, "\0");
    memset(caOpt, 0, sizeof(caOpt));
    iReturn = dsCltGetMyInfo(1, "Userid", &lId);
    if(iReturn == 0)
    {
        pasDbOneRecord("select opt from dsuser where id=:id", 1, "id", UT_TYPE_LONG, lId, UT_TYPE_STRING, 255, caOpt);
    }
    lRowNum = utComGetVar_ld(psShmHead, "GroupTreeRow", 300);
    iNum = 0;
    psDbHead = utPltInitDb();
    if(strlen(caCname) == 0)
    {
        lStartRec = (lCurPg - 1) * lRowNum;
        sprintf(caTemp, "select groupname,groupid,pid,groupcode from ncsgroup where 1=1 ");
        if(strlen(caGroups) > 0)
        {
            if(strlen(caPid) == 0)
            {
                sprintf(caSql, "select pid from ncsgroup where pid not in (%s) and groupid in (%s) ", caGroups, caGroups);
                lPid = 0;
                pasDbOneRecord(caSql, 0, UT_TYPE_LONG, 4, &lPid);
                sprintf(caPid, "%d", lPid);
            }
            sprintf(caTemp + strlen(caTemp), "and groupid in (%s) ", caGroups);
        }
        sprintf(caTemp + strlen(caTemp), " and pid=%d ", atol(caPid));
        sprintf(caTemp + strlen(caTemp), " order by groupname limit %lu,%lu ", 0, lRowNum);
        //  printf("caTemp=%s\n", caTemp);
        psCur = pasDbOpenSql(caTemp, 0);

        if(psCur == NULL)
        {
            utWebDispMsg(iFd, psMsgHead, "ncs/ncmsg_back.htm", "部门查询", "数据库出错");
            //        free(pHash);
            return 0;
        }
        memset(caGroupname, 0, sizeof(caGroupname));
        memset(caGroupcode, 0, sizeof(caGroupcode));
        lGroupid = 0;
        lPid = 0;
        iReturn = pasDbFetchInto(psCur, UT_TYPE_STRING, 68, caGroupname,
                                 UT_TYPE_LONG, 4, &lGroupid,
                                 UT_TYPE_LONG, 4, &lPid,
                                 UT_TYPE_STRING, 6, caGroupcode);
        while(iReturn == 0)
        {
            psData = (struct stData *)pasLHashLook(pHash, (char*)&lGroupid);
            //if(psData)
            //{
            if(iNum > 0)
            {
                utPltPutLoopVar(psDbHead, "dh", iNum + 1, ",");
            }
            iNum++;
            utPltSetCvtHtml(1);
            utPltPutLoopVar(psDbHead, "groupname", iNum, utStrGetCharHan(caGroupname, 48));
            utPltSetCvtHtml(0);
            utPltPutLoopVarF(psDbHead, "groupid",   iNum, "%ld", lGroupid);
            if(psData)
            {
                utPltPutLoopVar(psDbHead, "leaf", iNum, "false");
            }
            else
            {
                utPltPutLoopVar(psDbHead, "leaf", iNum, "true");
            }

            utPltPutLoopVar(psDbHead, "cls", iNum, "folder");
            utPltPutLoopVar(psDbHead, "dtype", iNum, "2");
            utPltPutLoopVar(psDbHead, "code", iNum, caGroupcode);
            utPltPutLoopVarF(psDbHead, "iNum", iNum, "%d", iNum - 1);
            utPltPutLoopVarF(psDbHead, "pid", iNum, "%d", lPid);
            //}
            memset(caGroupname, 0, sizeof(caGroupname));
            memset(caGroupcode, 0, sizeof(caGroupcode));
            lGroupid = 0;
            lLevel = 0;
            lPid = 0;
            iReturn = pasDbFetchInto(psCur, UT_TYPE_STRING, 68, caGroupname,
                                     UT_TYPE_LONG, 4, &lGroupid,
                                     UT_TYPE_LONG, 4, &lPid,
                                     UT_TYPE_STRING, 6, caGroupcode);
        }
        pasDbCloseCursor(psCur);
        //     free(pHash);
        utPltPutVar(psDbHead, "rootname", utComGetVar_sd(psShmHead, "rootname", "全部区域"));
    }
    // utPltShowDb(psDbHead);
    free(pHash);
    if(strlen(caPlate) > 0)
    {
        utPltOutToHtml(iFd, psMsgHead, psDbHead, caPlate);
    }
    else
    {
        utPltOutToHtml(iFd, psMsgHead, psDbHead, "v4/ncs_user_tree.htm");
    }
    return 0;
}

/* 增加用户  */
int ncsWebClientAdd_wif_v9(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead)
{
    char caUsername[33], caGroupid[33], caIp[16], caMac[33], caMacstr[42], caUse[10];
    char caDispname[64], caLimit[32];
    char caContact[32], caTelphone[32], caAddress[128];
    char caHandphone[32];
    char caWorkflag[16], caYyflag[16];
    unsigned long lUserid, lGroupid;
    utPltDbHead *psDbHead;
    int iReturn;
    unsigned long lUse, lUserid0;
    unsigned long lIp;
    int iPass, iAddFlags;
    char caPassword[32], caPassword1[32];
    char caDate[32], caUserid[16], caUsername0[32];
    char caGtype[16];
    ncsClient *psClient;
    char caOk[16], caSave[16], caEmail[64], caWebip[16], caPost[16], caLimitid[16];

    char handphone[32], servicecode[16], postcode[16], servicestate[10], ceoname[30], areaman[30], gtype[32], addtime[30], lasttime[30];
    char joincode[16], empnum[16], area[70], areatel[30], demo[255], ip[16], servernum[12], endnum111[10], jointype[12];
    char servicestate_desc[32], pre_setdesc[32], caPreset[16];
    char sqlbuf[4048] = "";
    char caTemp[1024];
    char caUsername2[32];
    char caMsg[1024];

    char caMonflag[12];
    char caFcode[16], caDateid[16];
    char caLongitude[16], caLatitude[16];
    char caCeoidtype[8], caCeocardid[32], caCeotel[64], caSdate[20], caEdate[20], caPort_start[20], caPort_end[20], caJytype[8];

    long frontPageDebug = utComGetVar_ld(psShmHead, "FrontDebug", 0);
    memset(caMonflag, 0, sizeof(caMonflag));
    dsCltGetSessionValue(1, "dispname", UT_TYPE_STRING, 30, caUsername2);

    strcpy(caDate, utTimFormat("%Y-%m-%d %H:%M:%S", time(0)));

    utMsgPrintMsg(psMsgHead);

    iReturn = utMsgGetSomeNVar(psMsgHead, 40,
                               "userid",      UT_TYPE_STRING, 10, caUserid,
                               "username",    UT_TYPE_STRING, 30, caUsername,
                               "groupid",     UT_TYPE_STRING, 10, caGroupid,
                               "dispanme",    UT_TYPE_STRING, 63, caDispname,
                               "address",     UT_TYPE_STRING, 127, caAddress,
                               "telphone",    UT_TYPE_STRING, 15, caTelphone,
                               "contact",     UT_TYPE_STRING, 15, caContact,
                               "handphone",   UT_TYPE_STRING, 30, handphone,
                               "email_addr",  UT_TYPE_STRING, 63, caEmail,
                               "entrance_ip", UT_TYPE_STRING, 16, caIp,
                               "addtime",     UT_TYPE_STRING, 24, addtime,
                               "lasttime",    UT_TYPE_STRING, 24, lasttime,
                               "gtype",       UT_TYPE_STRING, 10, gtype,
                               "servicestate", UT_TYPE_STRING, 10, servicestate,
                               "joincode",    UT_TYPE_STRING, 6, joincode,
                               "servernum",    UT_TYPE_STRING, 4, servernum,
                               "end_num111",   UT_TYPE_STRING, 8, endnum111,
                               "jointype",    UT_TYPE_STRING, 2, jointype,
                               "empnum" ,      UT_TYPE_STRING, 6, empnum,
                               "area" ,        UT_TYPE_STRING, 70, area,
                               "areaman" ,     UT_TYPE_STRING, 30, areaman,
                               "areatel",      UT_TYPE_STRING, 30, areatel,
                               "demo" ,         UT_TYPE_STRING, 255, demo,
                               "servicecode", UT_TYPE_STRING, 15, servicecode,
                               "postcode",    UT_TYPE_STRING, 6, postcode,
                               "ceoname",     UT_TYPE_STRING, 30, ceoname,
                               "monflag",     UT_TYPE_STRING, 10, caMonflag,
                               "fcode",       UT_TYPE_STRING, 10, caFcode,
                               "dateid",      UT_TYPE_STRING, 10, caDateid,
                               "longitude",   UT_TYPE_STRING, 12, caLongitude,
                               "latitude",    UT_TYPE_STRING, 12, caLatitude,
                               "jytype",      UT_TYPE_STRING, sizeof(caJytype) - 1, caJytype,
                               "ceoidtype",   UT_TYPE_STRING, sizeof(caCeoidtype) - 1, caCeoidtype,
                               "ceocardid",   UT_TYPE_STRING, sizeof(caCeocardid) - 1, caCeocardid,
                               "sdate",       UT_TYPE_STRING, sizeof(caSdate) - 1, caSdate,
                               "edate",       UT_TYPE_STRING, sizeof(caEdate) - 1, caEdate,
                               "port_start",  UT_TYPE_STRING, sizeof(caPort_start) - 1, caPort_start,
                               "port_end",    UT_TYPE_STRING, sizeof(caPort_end) - 1, caPort_end,
                               "workflag",    UT_TYPE_STRING, sizeof(caWorkflag) - 1, caWorkflag,
                               "yyflag",      UT_TYPE_STRING, sizeof(caYyflag) - 1, caYyflag);
    utStrDelSpaces(caUsername);
    utStrDelSpaces(caIp);

    //编码转换
    if(strlen(caUsername) > 0)
    {
        pasCvtGBK(2, caUsername, caTemp, 30);
        strcpy(caUsername, caTemp);
    }
    if(strlen(caDispname) > 0)
    {
        pasCvtGBK(2, caDispname, caTemp, 63);
        strcpy(caDispname, caTemp);
    }
    if(strlen(caAddress) > 0)
    {
        pasCvtGBK(2, caAddress, caTemp, 63);
        strcpy(caAddress, caTemp);
    }
    if(strlen(caTelphone) > 0)
    {
        pasCvtGBK(2, caTelphone, caTemp, 63);
        strcpy(caTelphone, caTemp);
    }
    if(strlen(caContact) > 0)
    {
        pasCvtGBK(2, caContact, caTemp, 63);
        strcpy(caContact, caTemp);
    }
    if(strlen(handphone) > 0)
    {
        pasCvtGBK(2, handphone, caTemp, 63);
        strcpy(handphone, caTemp);
    }
    if(strlen(area) > 0)
    {
        pasCvtGBK(2, area, caTemp, 63);
        strcpy(area, caTemp);
    }
    if(strlen(areaman) > 0)
    {
        pasCvtGBK(2, areaman, caTemp, 63);
        strcpy(areaman, caTemp);
    }
    if(strlen(demo) > 0)
    {
        pasCvtGBK(2, demo, caTemp, 63);
        strcpy(demo, caTemp);
    }
    if(strlen(ceoname) > 0)
    {
        pasCvtGBK(2, ceoname, caTemp, 63);
        strcpy(ceoname, caTemp);
    }
    if(strlen(joincode) > 0)
    {
        pasCvtGBK(2, joincode, caTemp, 6);
        strcpy(joincode, caTemp);
    }


    if(utStrIsSpaces(caUserid))
    {
        lUserid = pasGetSid("ncsuser", "userid");
        iAddFlags = 1;
    }
    else
    {
        lUserid = atol(caUserid);
        iAddFlags = 0;
    }
    if(iAddFlags)   /* 增加记录  */
    {
        sprintf(sqlbuf, "添加上网场所%s,名称：%s", caUsername, caDispname);
        ncsWriteSysLog(psMsgHead, caUsername2, "05", "成功", sqlbuf);
        strcpy(caSdate, utTimFormat("%Y-%m-%d %H:%M", time(0)));
        sprintf(sqlbuf, "insert into ncsuser (userid,username,groupid,dispname,address,telphone,contact,handphone,email,ip,addtime,lasttime,\
		               gtype,servicestate,joincode,endnum,servernum,jointype,empnum,area,areaman,areatel,demo,postcode,ceoname,moditime,monflag,password,fcode,dateid,jytype,ceoidtype,ceocardid,sdate,edate,port_start,port_end,workflag,yyflag) values \
                                                 (%lu,'%s', %lu,'%s','%s','%s','%s','%s','%s','%s','%s','%s',\
                                                 %d,'%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s',%lu,%lu,'','%s',%d,%d,'%s','%s','%s','%s','%s','%s',%d,%d)", \
                lUserid, caUsername, strtoul(caGroupid, NULL, 10), caDispname, caAddress, caTelphone, caContact, handphone, caEmail, caIp, caSdate, caSdate,
                atoi(gtype), servicestate, joincode, endnum111, servernum, jointype, empnum,
                area, areaman, areatel, demo, postcode, ceoname, time(0), atol(caMonflag), caFcode, atol(caDateid), atol(caJytype), caCeoidtype, caCeocardid, caSdate, caEdate, caPort_start, caPort_end, atol(caWorkflag), atol(caYyflag));
        macPrint(frontPageDebug, "sqlbuf=%s\n", sqlbuf);
        iReturn = pasDbExecSqlF(sqlbuf);
        if(iReturn == 0)
        {
            ncsWebReqSyscorp_v4(psShmHead, sqlbuf);
        }

        sprintf(sqlbuf, "添加上网场所 场所编码%s，名称：%s", caUsername, caDispname);
        ncsWriteSysLog(psMsgHead, caUsername2, "05", "成功", sqlbuf);

        if(iReturn != 0)
        {
            strcpy(caMsg, "false");
            pasDbRollback(NULL);
        }
        else
        {
            pasDbCommit(NULL);
            strcpy(caMsg, "true");
            if(strlen(caIp) > 0)
            {
                if(utStrIsValidIp(caIp))
                {
                    lIp = ntohl(utComHostAddress(caIp));
                    sprintf(caTemp, "insert into ncsuserip (userid,lip,uip,stime) values(%lu,%lu,%lu,%lu) ", lUserid, lIp, lIp, time(0));

                    pasDbExecSql(caTemp, 0);
                }
            }

        }
        psDbHead = utPltInitDb();
        utPltPutVar(psDbHead, "mesg", caMsg);
        utPltPutVarF(psDbHead, "userid", "%d", lUserid);
        utPltOutToHtml(iFd, psMsgHead, psDbHead, "v4/ncs_msg.htm");
        return 0;

    }
    else   /* 修改记录  */
    {

        sprintf(sqlbuf, "update ncsuser set username='%s',groupid=%lu,\
      	        gtype=%d,servicestate='%s',jointype='%s',joincode='%s',endnum='%s',\
				servernum='%s',dispname='%s',address='%s',ceoname='%s',handphone='%s',postcode='%s',\
				email='%s',contact='%s',telphone='%s',areaman='%s',areatel='%s',ip='%s',empnum='%s',area='%s',demo='%s',moditime=%lu,monflag=%lu,fcode='%s',dateid=%d,longitude='%s',latitude='%s',\
				jytype=%d,ceoidtype='%s',ceocardid='%s',sdate='%s',edate='%s',port_start='%s',port_end='%s',workflag=%d,yyflag=%d where userid=%lu",
                caUsername, strtoul(caGroupid, NULL, 10), atoi(gtype), servicestate,
                jointype, joincode, endnum111, servernum, caDispname, caAddress,
                ceoname, handphone, postcode, caEmail, caContact, caTelphone, areaman,
                areatel, caIp, empnum, area, demo, time(0), atol(caMonflag), caFcode, atol(caDateid), caLongitude, caLatitude, \
                atol(caJytype), caCeoidtype, caCeocardid, caSdate, caEdate, caPort_start, caPort_end, atol(caWorkflag), atol(caYyflag), atoi(caUserid));

        macPrint(frontPageDebug, "sqlbuf=%s\n", sqlbuf);

        iReturn = pasDbExecSqlF(sqlbuf);

        if(iReturn == 0)
        {
            ncsWebReqSyscorp_v4(psShmHead, sqlbuf);
        }

        sprintf(sqlbuf, "修改上网场所信息，代码：%s,显示名：%s", caUsername, caDispname);
        ncsWriteSysLog(psMsgHead, caUsername2, "05", "成功", sqlbuf);

        if(iReturn != 0)
        {
            strcpy(caMsg, "false");
            pasDbRollback(NULL);
        }
        else
        {
            strcpy(caMsg, "true");
            pasDbCommit(NULL);
        }
        lUserid = atol(caUserid);
        psClient = (ncsClient *)ncsUtlGetClientById(psShmHead, lUserid);
        if(psClient)
        {
            strcpy(psClient->username, caUsername);
            strcpy(psClient->dispname, caDispname);
        }

        psDbHead = utPltInitDb();
        utPltPutVar(psDbHead, "mesg", caMsg);
        utPltPutVar(psDbHead, "userid", caUserid);
        utPltOutToHtml(iFd, psMsgHead, psDbHead, "v4/ncs_msg.htm");
        return 0;
    }
}

//查询该servicecode对应的场所信息
int macGetBabDeviceInfo(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead)
{
    long babDebug = utComGetVar_ld(psShmHead, "BabDebug", 0);
    if(babDebug)
    {
        utMsgPrintMsg(psMsgHead);
    }
    utPltDbHead *psDbHead = utPltInitDb();
    char sql[1024] = "";
    int iReturn = 0;
    char caServicecode[33] = "";
    long devcount = 0;
    char caGroupname[32] = "";
    char caDispname[255] = "";
    char caAddress[255] = "";
    iReturn = utMsgGetSomeNVar(psMsgHead, 1,
                               "servicecode",  UT_TYPE_STRING, sizeof(caServicecode) - 1, caServicecode);

    //布控告警待处理数
    memset(sql, 0, sizeof(sql));
    snprintf(sql, sizeof(sql), "select ifnull(count(*), 0) from ncaplist where servicecode=%s", caServicecode);
    iReturn = pasDbOneRecord(sql, 0, UT_TYPE_LONG, 4, &devcount);
    if(iReturn != 0)
    {
        utPltPutVar(psDbHead, "retcode", "1");
        utPltPutVar(psDbHead, "retmsg", "查表出错");
        utPltOutToHtml(iFd, psMsgHead, psDbHead, "mac/datamining/bab/serviceinfo.htm");
        return -1;
    }
    utPltPutVarF(psDbHead, "devcount", "%lu", devcount);
    if(strlen(caServicecode) > 7)
    {
        //经营性质
        if(caServicecode[6] == '0')
        {
            utPltPutVar(psDbHead, "business", "性质0");
        }
        else if('1' == caServicecode[6])
        {
            utPltPutVar(psDbHead, "business", "性质1");
        }
        memset(sql, 0, sizeof(sql));
        snprintf(sql, sizeof(sql), "select b.groupname,a.dispname,a.address from ncsuser a,ncsgroup b where left(a.username,6)=b.groupcode and a.username='%s'", caServicecode);
        pasDbOneRecord(sql, 0, UT_TYPE_STRING, sizeof(caGroupname) - 1, caGroupname,
                       UT_TYPE_STRING, sizeof(caDispname) - 1, caDispname,
                       UT_TYPE_STRING, sizeof(caAddress) - 1, caAddress);
        utPltPutVar(psDbHead, "area", caGroupname);
        utPltPutVar(psDbHead, "dispname", caDispname);
        utPltPutVar(psDbHead, "address", caAddress);
        utPltPutVar(psDbHead, "servicecode", caServicecode);
    }
    else
    {
        utPltPutVar(psDbHead, "retcode", "2");
        utPltPutVar(psDbHead, "retmsg", "servicecode不足7位");
        utPltOutToHtml(iFd, psMsgHead, psDbHead, "mac/datamining/bab/serviceinfo.htm");
        return -1;
    }
    utPltPutVar(psDbHead, "retcode", "0");
    utPltOutToHtml(iFd, psMsgHead, psDbHead, "mac/datamining/bab/serviceinfo.htm");
    return 0;
}
/*
struct s_babtime
{
    uint8 stime;
    uint8 etime;
};
*/
int macGetBabMacPlaceCount(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead)
{
    long babDebug = utComGetVar_ld(psShmHead, "BabDebug", 0);
    if(babDebug)
    {
        utMsgPrintMsg(psMsgHead);
    }
    utPltDbHead *psDbHead = utPltInitDb();

    char caServicecode[32] = "";
    char caCid[8] = "";
    char caMac[20] = "";
    int iReturn, iNum;
    pasDbCursor *psCur;
    long lCount = 0;
    char sql[2048] = "";

    iReturn = utMsgGetSomeNVar(psMsgHead, 2,
                               "cid",  UT_TYPE_STRING, sizeof(caCid) - 1, caCid,
                               "mac",  UT_TYPE_STRING, 17, caMac);
    if(strlen(caMac) != 17)
    {
        utPltPutVar(psDbHead, "retcode", "3");
        utPltPutVar(psDbHead, "retmsg", "mac地址长度不为17");
        utPltOutToHtml(iFd, psMsgHead, psDbHead, "mac/datamining/bab/macplacecountlist.htm");
        return -1;
    }
    if(strlen(caCid) == 0)
    {
        utPltPutVar(psDbHead, "retcode", "2");
        utPltPutVar(psDbHead, "retmsg", "cid不能为空");
        utPltOutToHtml(iFd, psMsgHead, psDbHead, "mac/datamining/bab/macplacecountlist.htm");
        return -1;
    }

    //统计该mac在每个servicecode出现的次数
    memset(sql, 0, sizeof(sql));
    snprintf(sql, sizeof(sql), "select servicecode,ifnull(count(mac),0) from ncbabplacetimemac where cid=%s and mac='%s' group by servicecode", caCid, caMac);
    macPrint(babDebug, "========bab sql=[%s]==========\n", sql);
    psCur = pasDbOpenSqlF(sql);
    if(psCur)
    {
        iReturn = 0;
        lCount = 0;
        memset(caServicecode, 0, sizeof(caServicecode));
        iReturn = pasDbFetchInto(psCur,
                                 UT_TYPE_STRING, sizeof(caServicecode) - 1, caServicecode,
                                 UT_TYPE_LONG, 4, &lCount);
        iNum = 0;
        while(iReturn == 0 || iReturn == 1405)
        {
            iNum++;
            if(iNum > 1)
            {
                utPltPutLoopVar(psDbHead, "dh", iNum, ",");
            }
            utPltPutLoopVarF(psDbHead, "maccount", iNum, "%lu", lCount);
            utPltPutLoopVarF(psDbHead, "servicecode", iNum, "%s", caServicecode);

            lCount = 0;
            memset(caServicecode, 0, sizeof(caServicecode));
            iReturn = pasDbFetchInto(psCur,
                                     UT_TYPE_STRING, sizeof(caServicecode) - 1, caServicecode,
                                     UT_TYPE_LONG, 4, &lCount);

        }
        pasDbCloseCursor(psCur);
    }

    utPltPutVarF(psDbHead, "totalcount", "%lu", iNum);
    utPltPutVar(psDbHead, "cid", caCid);
    utPltPutVar(psDbHead, "mac", caMac);
    utPltPutVar(psDbHead, "retcode", "0");
    utPltPutVar(psDbHead, "retmsg", "成功");
    utPltOutToHtml(iFd, psMsgHead, psDbHead, "mac/datamining/bab/macplacecountlist.htm");
    return 0;
}
#define TIMESTR_LENGTH 12

uint8 getUnixtimeByStr12(char* str12)
{
    int offset = 0;
    char caYear[5] = "";
    char caMonth[3] = "";
    char caDay[3] = "";
    char caHour[3] = "", caMin[3] = "";
    char caTempTime1[20] = "", caTempTime2[20] = "";
    if(strlen(str12) != TIMESTR_LENGTH)
    {
        return 0;
    }
    offset = 0;
    memcpy(caYear, str12 + offset, 4);
    offset += 4;
    memcpy(caMonth, str12 + offset, 2);
    offset += 2;
    memcpy(caDay, str12 + offset, 2);
    offset += 2;
    memcpy(caHour, str12 + offset, 2);
    offset += 2;
    memcpy(caMin, str12 + offset, 2);
    offset += 2;
    snprintf(caTempTime1, sizeof(caTempTime1), "%s/%s/%s %s:%s", caYear, caMonth, caDay, caHour, caMin);
    return utTimStrToLong("%Y/%m/%d %H:%M", caTempTime1);

}
//返回在特定时间段下每个场所的mac的个数
int macGetBabPlaceMacCount(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead)
{
    long babDebug = utComGetVar_ld(psShmHead, "BabDebug", 0);
    if(babDebug)
    {
        utMsgPrintMsg(psMsgHead);
    }
    utPltDbHead *psDbHead = utPltInitDb();

    char caServicecode[32] = "";
    char caCid[8] = "";
    char caTimePeriod1[13] = "";
    char caTimePeriod2[13] = "";
    uint8 lStime = 0, lEtime = 0;
    int iReturn, iNum = 0;
    pasDbCursor *psCur;
    long lCount = 0;
    char sql[2048] = "";

    iReturn = utMsgGetSomeNVar(psMsgHead, 3,
                               "cid",  UT_TYPE_STRING, sizeof(caCid) - 1, caCid,
                               "timeperiod1",  UT_TYPE_STRING, sizeof(caTimePeriod1) - 1, caTimePeriod1,
                               "timeperiod2",  UT_TYPE_STRING, sizeof(caTimePeriod2) - 1, caTimePeriod2);
    //201509111637
    if((strlen(caTimePeriod1) != TIMESTR_LENGTH) && (strlen(caTimePeriod1) != 0))
    {
        utPltPutVar(psDbHead, "retcode", "4");
        utPltPutVar(psDbHead, "retmsg", "起始时间段参数不为12位");
        utPltOutToHtml(iFd, psMsgHead, psDbHead, "mac/datamining/bab/placemaccountlist.htm");
        return -1;
    }
    //201509111726
    if((strlen(caTimePeriod2) != TIMESTR_LENGTH) && (strlen(caTimePeriod2) != 0))
    {
        utPltPutVar(psDbHead, "retcode", "4");
        utPltPutVar(psDbHead, "retmsg", "结束时间段参数不为12位");
        utPltOutToHtml(iFd, psMsgHead, psDbHead, "mac/datamining/bab/placemaccountlist.htm");
        return -1;
    }

    lStime = getUnixtimeByStr12(caTimePeriod1);
    lEtime = getUnixtimeByStr12(caTimePeriod2);
    lEtime = lEtime + 60;//多加一分钟

    if(strlen(caCid) == 0)
    {
        utPltPutVar(psDbHead, "retcode", "2");
        utPltPutVar(psDbHead, "retmsg", "cid不能为空");
        utPltOutToHtml(iFd, psMsgHead, psDbHead, "mac/datamining/bab/placemaccountlist.htm");
        return -1;
    }

    //统计每servicecode出现的mac个数
    memset(sql, 0, sizeof(sql));
    if((strlen(caTimePeriod1) == TIMESTR_LENGTH) && (strlen(caTimePeriod2) == TIMESTR_LENGTH))
        snprintf(sql, sizeof(sql), "select servicecode,count(distinct(mac)) from ncbabplacetimemac where cid=%s and starttime>=%llu and endtime<=%llu group by servicecode", caCid, lStime, lEtime);
    else
        snprintf(sql, sizeof(sql), "select servicecode,count(distinct(mac)) from ncbabplacetimemac where cid=%s group by servicecode", caCid);
    macPrint(babDebug, "========bab sql=[%s]==========\n", sql);
    psCur = pasDbOpenSqlF(sql);
    if(psCur)
    {
        iReturn = 0;
        lCount = 0;
        memset(caServicecode, 0, sizeof(caServicecode));
        iReturn = pasDbFetchInto(psCur,
                                 UT_TYPE_STRING, sizeof(caServicecode) - 1, caServicecode,
                                 UT_TYPE_LONG, 4, &lCount);
        iNum = 0;
        while(iReturn == 0 || iReturn == 1405)
        {
            iNum++;
            if(iNum > 1)
            {
                utPltPutLoopVar(psDbHead, "dh", iNum, ",");
            }
            utPltPutLoopVarF(psDbHead, "maccount", iNum, "%lu", lCount);
            utPltPutLoopVarF(psDbHead, "servicecode", iNum, "%s", caServicecode);

            lCount = 0;
            memset(caServicecode, 0, sizeof(caServicecode));
            iReturn = pasDbFetchInto(psCur,
                                     UT_TYPE_STRING, sizeof(caServicecode) - 1, caServicecode,
                                     UT_TYPE_LONG, 4, &lCount);

        }
        pasDbCloseCursor(psCur);
    }

    utPltPutVarF(psDbHead, "totalcount", "%lu", iNum);
    utPltPutVar(psDbHead, "cid", caCid);
    utPltPutVar(psDbHead, "retcode", "0");
    utPltPutVar(psDbHead, "retmsg", "成功");
    utPltOutToHtml(iFd, psMsgHead, psDbHead, "mac/datamining/bab/placemaccountlist.htm");
    return 0;
}

//返回某时间段，该servicecode的每个mac的伴随次数
int macGetBabMacCount(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead)
{
    long babDebug = utComGetVar_ld(psShmHead, "BabDebug", 0);
    if(babDebug)
    {
        utMsgPrintMsg(psMsgHead);
    }
    utPltDbHead *psDbHead = utPltInitDb();

    char caServicecode[32] = "";
    char caCid[8] = "";
    int iReturn, iNum;
    pasDbCursor *psCur;
    long lCount = 0;
    char caMac[20] = "";
    char caTimePeriod1[13] = "";
    char caTimePeriod2[13] = "";
    uint8 lStime = 0, lEtime = 0;
    char sql[2048] = "";

    iReturn = utMsgGetSomeNVar(psMsgHead, 4,
                               "servicecode",  UT_TYPE_STRING, sizeof(caServicecode) - 1, caServicecode,
                               "cid",  UT_TYPE_STRING, sizeof(caCid) - 1, caCid,
                               "timeperiod1",  UT_TYPE_STRING, sizeof(caTimePeriod1) - 1, caTimePeriod1,
                               "timeperiod2",  UT_TYPE_STRING, sizeof(caTimePeriod2) - 1, caTimePeriod2);
    if(strlen(caServicecode) < 7)
    {
        utPltPutVar(psDbHead, "retcode", "1");
        utPltPutVar(psDbHead, "retmsg", "servicecode不足7位");
        utPltOutToHtml(iFd, psMsgHead, psDbHead, "mac/datamining/bab/maccountlist.htm");
        return -1;
    }
    if(strlen(caCid) == 0)
    {
        utPltPutVar(psDbHead, "retcode", "2");
        utPltPutVar(psDbHead, "retmsg", "cid不能为空");
        utPltOutToHtml(iFd, psMsgHead, psDbHead, "mac/datamining/bab/maccountlist.htm");
        return -1;
    }
    if((strlen(caTimePeriod1) != TIMESTR_LENGTH) && (strlen(caTimePeriod1) != 0))
    {
        utPltPutVar(psDbHead, "retcode", "3");
        utPltPutVar(psDbHead, "retmsg", "起始时间段不足12位");
        utPltOutToHtml(iFd, psMsgHead, psDbHead, "mac/datamining/bab/maccountlist.htm");
        return -1;
    }
    if((strlen(caTimePeriod2) != TIMESTR_LENGTH) && (strlen(caTimePeriod2) != 0))
    {
        utPltPutVar(psDbHead, "retcode", "4");
        utPltPutVar(psDbHead, "retmsg", "结束时间段不足12位");
        utPltOutToHtml(iFd, psMsgHead, psDbHead, "mac/datamining/bab/maccountlist.htm");
        return -1;
    }
    lStime = getUnixtimeByStr12(caTimePeriod1);
    lEtime = getUnixtimeByStr12(caTimePeriod2) ;
    lEtime += 60;
    //统计每个mac出现的次数
    memset(sql, 0, sizeof(sql));
    if((strlen(caTimePeriod1) == TIMESTR_LENGTH) && (TIMESTR_LENGTH == strlen(caTimePeriod2)))
        snprintf(sql, sizeof(sql), "select mac,ifnull(count(mac),0) from(select mac,starttime from ncbabplacetimemac where cid=%s and servicecode='%s' and starttime>=%llu and endtime<=%llu group by mac,starttime)aa group by mac", caCid, caServicecode, lStime, lEtime);
    else
        snprintf(sql, sizeof(sql), "select mac,ifnull(count(mac),0) from(select mac,starttime from ncbabplacetimemac where cid=%s and servicecode='%s' group by mac,starttime)aa group by mac", caCid, caServicecode);
    macPrint(babDebug, "========bab sql=[%s]==========\n", sql);
    psCur = pasDbOpenSqlF(sql);
    if(psCur)
    {
        iReturn = 0;
        lCount = 0;
        memset(caMac, 0, sizeof(caMac));
        iReturn = pasDbFetchInto(psCur,
                                 UT_TYPE_STRING, 17, caMac,
                                 UT_TYPE_LONG, 4, &lCount);
        iNum = 0;
        while(iReturn == 0 || iReturn == 1405)
        {
            iNum++;
            if(iNum > 1)
            {
                utPltPutLoopVar(psDbHead, "dh", iNum, ",");
            }
            utPltPutLoopVarF(psDbHead, "maccount", iNum, "%lu", lCount);
            utPltPutLoopVarF(psDbHead, "mac", iNum, "%s", caMac);

            memset(caMac, 0, sizeof(caMac));
            iReturn = pasDbFetchInto(psCur,
                                     UT_TYPE_STRING, 17, caMac,
                                     UT_TYPE_LONG, 4, &lCount);
        }
        pasDbCloseCursor(psCur);
    }

    utPltPutVarF(psDbHead, "totalcount", "%lu", iNum);
    utPltPutVar(psDbHead, "cid", caCid);
    utPltPutVar(psDbHead, "servicecode", caServicecode);
    utPltPutVar(psDbHead, "retcode", "0");
    utPltPutVar(psDbHead, "retmsg", "成功");
    utPltOutToHtml(iFd, psMsgHead, psDbHead, "mac/datamining/bab/maccountlist.htm");
    return 0;
}


int macGetBabMacTimeDetail(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead)
{
    long babDebug = utComGetVar_ld(psShmHead, "BabDebug", 0);
    if(babDebug)
    {
        utMsgPrintMsg(psMsgHead);
    }
    utPltDbHead *psDbHead = utPltInitDb();

    char caCid[8] = "";
    int iReturn, iNum, offset = 0;
    pasDbCursor *psCur;
    long lCount = 0;
    uint8 lStime = 0, lEtime = 0;
    char caMac[20] = "";
    char caServicecode[32] = "";
    char caTimePeriod[13] = "";
    char caYear[5] = "";
    char caMonth[3] = "";
    char caDay[3] = "";
    char caShour[3] = "", caEhour[3] = "";
    char caTempTime1[20] = "", caTempTime2[20] = "";
    char sql[2048] = "";

    iReturn = utMsgGetSomeNVar(psMsgHead, 2,
                               "cid",  UT_TYPE_STRING, sizeof(caCid) - 1, caCid,
                               "timeperiod",  UT_TYPE_STRING, sizeof(caTimePeriod) - 1, caTimePeriod);

    if(strlen(caTimePeriod) != 12)
    {
        utPltPutVar(psDbHead, "retcode", "4");
        utPltPutVar(psDbHead, "retmsg", "时间段参数不为12位");
        utPltOutToHtml(iFd, psMsgHead, psDbHead, "mac/datamining/bab/timemacdetail.htm");
        return -1;
    }
    offset = 0;
    memcpy(caYear, caTimePeriod + offset, 4);
    offset += 4;
    memcpy(caMonth, caTimePeriod + offset, 2);
    offset += 2;
    memcpy(caDay, caTimePeriod + offset, 2);
    offset += 2;
    memcpy(caShour, caTimePeriod + offset, 2);
    offset += 2;
    memcpy(caEhour, caTimePeriod + offset, 2);
    snprintf(caTempTime1, sizeof(caTempTime1), "%s/%s/%s %s:00", caYear, caMonth, caDay, caShour);
    snprintf(caTempTime2, sizeof(caTempTime2), "%s/%s/%s %s:00", caYear, caMonth, caDay, caEhour);
    printf("caTempTime1=[%s],caTempTime2=[%s]\n", caTempTime1, caTempTime2);

    if(strlen(caCid) == 0)
    {
        utPltPutVar(psDbHead, "retcode", "2");
        utPltPutVar(psDbHead, "retmsg", "cid不能为空");
        utPltOutToHtml(iFd, psMsgHead, psDbHead, "mac/datamining/bab/timemacdetail.htm");
        return -1;
    }

    memset(sql, 0, sizeof(sql));
    snprintf(sql, sizeof(sql), "select mac,servicecode,ifnull(count(mac),0) from ncbabplacetimemac where cid=%s and starttime>=%llu and endtime<=%llu group by servicecode,mac", caCid, lStime, lEtime);
    macPrint(babDebug, "========time mac detail sql=[%s]==========\n", sql);
    psCur = pasDbOpenSqlF(sql);
    if(psCur)
    {
        lCount = 0;
        memset(caServicecode, 0, sizeof(caServicecode));
        memset(caMac, 0, sizeof(caMac));
        iReturn = pasDbFetchInto(psCur,
                                 UT_TYPE_STRING, sizeof(caMac) - 1, caMac,
                                 UT_TYPE_STRING, sizeof(caServicecode) - 1, caServicecode,
                                 UT_TYPE_LONG, 4, &lCount);
        iNum = 0;
        while(iReturn == 0 || iReturn == 1405)
        {
            iNum++;
            if(iNum > 1)
            {
                utPltPutLoopVar(psDbHead, "dh", iNum, ",");
            }
            utPltPutLoopVarF(psDbHead, "maccount", iNum, "%lu", lCount);
            utPltPutLoopVarF(psDbHead, "mac", iNum, "%s", caMac);
            utPltPutLoopVarF(psDbHead, "servicecode", iNum, "%s", caServicecode);
            lCount = 0;
            memset(caServicecode, 0, sizeof(caServicecode));
            memset(caMac, 0, sizeof(caMac));
            iReturn = pasDbFetchInto(psCur,
                                     UT_TYPE_STRING, sizeof(caMac) - 1, caMac,
                                     UT_TYPE_STRING, sizeof(caServicecode) - 1, caServicecode,
                                     UT_TYPE_LONG, 4, &lCount);



        }
        pasDbCloseCursor(psCur);
    }
    utPltPutVarF(psDbHead, "totalcount", "%lu", iNum);
    utPltPutVar(psDbHead, "cid", caCid);
    utPltPutVar(psDbHead, "retcode", "0");
    utPltPutVar(psDbHead, "retmsg", "成功");
    utPltOutToHtml(iFd, psMsgHead, psDbHead, "mac/datamining/bab/timemacdetail.htm");
    return 0;
}

//返回每个时间段的mac个数(包括按天统计和按小时统计)
int macGetBabMacTimeCount(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead)
{
    long babDebug = utComGetVar_ld(psShmHead, "BabDebug", 0);
    if(babDebug)
    {
        utMsgPrintMsg(psMsgHead);
    }
    utPltDbHead *psDbHead = utPltInitDb();

    char caCid[8] = "";
    int iReturn, iNum = 0;
    pasDbCursor *psCur;
    long lCount = 0;
    char caTime[17] = "";
    char caStime[20] = "", caEtime[20] = "";
    char sql[2048] = "";

    iReturn = utMsgGetSomeNVar(psMsgHead, 1,
                               "cid",  UT_TYPE_STRING, sizeof(caCid) - 1, caCid);

    if(strlen(caCid) == 0)
    {
        utPltPutVar(psDbHead, "retcode", "2");
        utPltPutVar(psDbHead, "retmsg", "cid不能为空");
        utPltOutToHtml(iFd, psMsgHead, psDbHead, "mac/datamining/bab/timemaccount.htm");
        return -1;
    }
    iNum = 0;

    //按小时统计的次数
    memset(sql, 0, sizeof(sql));
    snprintf(sql, sizeof(sql), "select from_unixtime(starttime),from_unixtime(endtime),ifnull(count(distinct(mac)),0) from ncbabplacetimemac where cid=%s group by starttime,endtime", caCid);
    macPrint(babDebug, "========time hour mac count sql=[%s]==========\n", sql);
    psCur = pasDbOpenSqlF(sql);
    if(psCur)
    {
        lCount = 0;
        memset(caStime, 0, sizeof(caStime));
        memset(caEtime, 0, sizeof(caEtime));
        iReturn = pasDbFetchInto(psCur,
                                 UT_TYPE_STRING, sizeof(caStime) - 1, caStime,
                                 UT_TYPE_STRING, sizeof(caEtime) - 1, caEtime,
                                 UT_TYPE_LONG, 4, &lCount);

        while(iReturn == 0 || iReturn == 1405)
        {
            // DO:根据caStime,caEtime 计算caTime
            //201508011455 52
            removeAll(caStime, ' ');
            removeAll(caStime, '-');
            removeAll(caStime, ':');
            //20150801 1510 25
            removeAll(caEtime, ' ');
            removeAll(caEtime, '-');
            removeAll(caEtime, ':');
            memset(caTime, 0, sizeof(caTime));
            memcpy(caTime, caStime, 12);
            memcpy(caTime + 12, caEtime + 8, 4);
            iNum++;
            if(iNum > 1)
            {
                utPltPutLoopVar(psDbHead, "dh", iNum, ",");
            }
            utPltPutLoopVar(psDbHead, "timeflag", iNum, "hour");
            utPltPutLoopVarF(psDbHead, "maccount", iNum, "%lu", lCount);
            utPltPutLoopVarF(psDbHead, "timevalue", iNum, "%s", caTime);
            lCount = 0;
            memset(caStime, 0, sizeof(caStime));
            memset(caEtime, 0, sizeof(caEtime));
            iReturn = pasDbFetchInto(psCur,
                                     UT_TYPE_STRING, sizeof(caStime) - 1, caStime,
                                     UT_TYPE_STRING, sizeof(caEtime) - 1, caEtime,
                                     UT_TYPE_LONG, 4, &lCount);
        }
        pasDbCloseCursor(psCur);
    }

    /*
    //按小时统计2
    memset(sql, 0, sizeof(sql));
    snprintf(sql, sizeof(sql), "select bb,count(distinct(mac)) from (select left(from_unixtime(starttime),13) bb,mac from ncbabplacetimemac where cid=%s)aa group by bb", caCid);
    macPrint(babDebug, "========time day mac count sql=[%s]==========\n", sql);
    psCur = pasDbOpenSqlF(sql);
    if(psCur)
    {
        lCount = 0;
        memset(caTime, 0, sizeof(caTime));
        iReturn = pasDbFetchInto(psCur,
                                 UT_TYPE_STRING, sizeof(caTime) - 1, caTime,
                                 UT_TYPE_LONG, 4, &lCount);
        while(iReturn == 0 || iReturn == 1405)
        {
            removeAll(caTime, ' ');
            removeAll(caTime, '-');
            removeAll(caTime, ':');
            iNum++;
            if(iNum > 1)
            {
                utPltPutLoopVar(psDbHead, "dh", iNum, ",");
            }
            utPltPutLoopVar(psDbHead, "timeflag", iNum, "hour");
            utPltPutLoopVarF(psDbHead, "timevalue", iNum, "%s", caTime);
            utPltPutLoopVarF(psDbHead, "maccount", iNum, "%lu", lCount);
            lCount = 0;
            memset(caTime, 0, sizeof(caTime));
            iReturn = pasDbFetchInto(psCur,
                                     UT_TYPE_STRING, sizeof(caTime) - 1, caTime,
                                     UT_TYPE_LONG, 4, &lCount);

        }
        pasDbCloseCursor(psCur);
    }
    */
    //按天统计的次数
    memset(sql, 0, sizeof(sql));
    snprintf(sql, sizeof(sql), "select bb,count(distinct(mac)) from (select left(from_unixtime(starttime),10) bb,mac from ncbabplacetimemac where cid=%s)aa group by bb", caCid);
    macPrint(babDebug, "========time day mac count sql=[%s]==========\n", sql);
    psCur = pasDbOpenSqlF(sql);
    if(psCur)
    {
        lCount = 0;
        memset(caTime, 0, sizeof(caTime));
        iReturn = pasDbFetchInto(psCur,
                                 UT_TYPE_STRING, sizeof(caTime) - 1, caTime,
                                 UT_TYPE_LONG, 4, &lCount);
        while(iReturn == 0 || iReturn == 1405)
        {
            removeAll(caTime, ' ');
            removeAll(caTime, '-');
            removeAll(caTime, ':');
            iNum++;
            if(iNum > 1)
            {
                utPltPutLoopVar(psDbHead, "dh", iNum, ",");
            }
            utPltPutLoopVar(psDbHead, "timeflag", iNum, "day");
            utPltPutLoopVarF(psDbHead, "timevalue", iNum, "%s", caTime);
            utPltPutLoopVarF(psDbHead, "maccount", iNum, "%lu", lCount);
            lCount = 0;
            memset(caTime, 0, sizeof(caTime));
            iReturn = pasDbFetchInto(psCur,
                                     UT_TYPE_STRING, sizeof(caTime) - 1, caTime,
                                     UT_TYPE_LONG, 4, &lCount);

        }
        pasDbCloseCursor(psCur);
    }
    //按月统计的次数
    memset(sql, 0, sizeof(sql));
    snprintf(sql, sizeof(sql), "select bb,count(distinct(mac)) from (select left(from_unixtime(starttime),7) bb,mac from ncbabplacetimemac where cid=%s)aa group by bb", caCid);
    macPrint(babDebug, "========time day mac count sql=[%s]==========\n", sql);
    psCur = pasDbOpenSqlF(sql);
    if(psCur)
    {
        lCount = 0;
        memset(caTime, 0, sizeof(caTime));
        iReturn = pasDbFetchInto(psCur,
                                 UT_TYPE_STRING, sizeof(caTime) - 1, caTime,
                                 UT_TYPE_LONG, 4, &lCount);
        while(iReturn == 0 || iReturn == 1405)
        {
            removeAll(caTime, ' ');
            removeAll(caTime, '-');
            removeAll(caTime, ':');
            iNum++;
            if(iNum > 1)
            {
                utPltPutLoopVar(psDbHead, "dh", iNum, ",");
            }
            utPltPutLoopVar(psDbHead, "timeflag", iNum, "month");
            utPltPutLoopVarF(psDbHead, "timevalue", iNum, "%s", caTime);
            utPltPutLoopVarF(psDbHead, "maccount", iNum, "%lu", lCount);
            lCount = 0;
            memset(caTime, 0, sizeof(caTime));
            iReturn = pasDbFetchInto(psCur,
                                     UT_TYPE_STRING, sizeof(caTime) - 1, caTime,
                                     UT_TYPE_LONG, 4, &lCount);

        }
        pasDbCloseCursor(psCur);
    }

    utPltPutVarF(psDbHead, "totalcount", "%lu", iNum);
    utPltPutVar(psDbHead, "cid", caCid);
    utPltPutVar(psDbHead, "retcode", "0");
    utPltPutVar(psDbHead, "retmsg", "成功");
    utPltOutToHtml(iFd, psMsgHead, psDbHead, "mac/datamining/bab/timemaccount.htm");
    return 0;
}

//返回特定mac在某场所某时间段内所有的时间段
int macGetBabMacDetail(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead)
{
    long babDebug = utComGetVar_ld(psShmHead, "BabDebug", 0);
    if(babDebug)
    {
        utMsgPrintMsg(psMsgHead);
    }
    utPltDbHead *psDbHead = utPltInitDb();

    char caServicecode[32] = "";
    char caCid[8] = "";
    char caMac[20] = "";
    int iReturn, iNum;
    pasDbCursor *psCur;
    long lCount = 0;
    char caTimePeriod1[13] = "";
    char caTimePeriod2[13] = "";
    uint8 lStime = 0, lEtime = 0;
    char sql[2048] = "";

    iReturn = utMsgGetSomeNVar(psMsgHead, 5,
                               "servicecode",  UT_TYPE_STRING, sizeof(caServicecode) - 1, caServicecode,
                               "mac",  UT_TYPE_STRING, sizeof(caMac) - 1, caMac,
                               "cid",  UT_TYPE_STRING, sizeof(caCid) - 1, caCid,
                               "timeperiod1",  UT_TYPE_STRING, sizeof(caTimePeriod1) - 1, caTimePeriod1,
                               "timeperiod2",  UT_TYPE_STRING, sizeof(caTimePeriod2) - 1, caTimePeriod2);

    if(strlen(caServicecode) < 7)
    {
        utPltPutVar(psDbHead, "retcode", "1");
        utPltPutVar(psDbHead, "retmsg", "servicecode不足7位");
        utPltOutToHtml(iFd, psMsgHead, psDbHead, "mac/datamining/bab/maccountdetail.htm");
        return -1;
    }
    if(strlen(caCid) == 0)
    {
        utPltPutVar(psDbHead, "retcode", "2");
        utPltPutVar(psDbHead, "retmsg", "cid不能为空");
        utPltOutToHtml(iFd, psMsgHead, psDbHead, "mac/datamining/bab/maccountdetail.htm");
        return -1;
    }
    if(strlen(caMac) != 17)
    {
        utPltPutVar(psDbHead, "retcode", "3");
        utPltPutVar(psDbHead, "retmsg", "mac地址长度不为17");
        utPltOutToHtml(iFd, psMsgHead, psDbHead, "mac/datamining/bab/maccountdetail.htm");
        return -1;
    }
    if((strlen(caTimePeriod1) != TIMESTR_LENGTH) && (strlen(caTimePeriod1) != 0))
    {
        utPltPutVar(psDbHead, "retcode", "3");
        utPltPutVar(psDbHead, "retmsg", "起始时间段不足12位");
        utPltOutToHtml(iFd, psMsgHead, psDbHead, "mac/datamining/bab/maccountdetail.htm");
        return -1;
    }
    if((strlen(caTimePeriod2) != TIMESTR_LENGTH) && (strlen(caTimePeriod2) != 0))
    {
        utPltPutVar(psDbHead, "retcode", "4");
        utPltPutVar(psDbHead, "retmsg", "结束时间段不足12位");
        utPltOutToHtml(iFd, psMsgHead, psDbHead, "mac/datamining/bab/maccountdetail.htm");
        return -1;
    }
    lStime = getUnixtimeByStr12(caTimePeriod1);
    lEtime = getUnixtimeByStr12(caTimePeriod2) ;
    lEtime += 60;

    //统计该mac出现的时间数
    memset(sql, 0, sizeof(sql));
    if((strlen(caTimePeriod1) == TIMESTR_LENGTH) && (TIMESTR_LENGTH == strlen(caTimePeriod2)))
        snprintf(sql, sizeof(sql), "select starttime,endtime from ncbabplacetimemac where cid=%s and servicecode='%s' and mac='%s' and starttime>=%llu and endtime<=%llu", caCid, caServicecode, caMac, lStime, lEtime);
    else
        snprintf(sql, sizeof(sql), "select starttime,endtime from ncbabplacetimemac where cid=%s and servicecode='%s' and mac='%s'", caCid, caServicecode, caMac);
    macPrint(babDebug, "========bab detail sql=[%s]==========\n", sql);
    psCur = pasDbOpenSqlF(sql);
    if(psCur)
    {
        lStime = 0;
        lEtime = 0;
        iReturn = pasDbFetchInto(psCur,
                                 UT_TYPE_LONG8, 8, &lStime,
                                 UT_TYPE_LONG8, 8, &lEtime);
        iNum = 0;
        while(iReturn == 0 || iReturn == 1405)
        {
            iNum++;
            if(iNum > 1)
            {
                utPltPutLoopVar(psDbHead, "dh", iNum, ",");
            }
            utPltPutLoopVarF(psDbHead, "stime", iNum, "%llu", lStime);
            utPltPutLoopVarF(psDbHead, "etime", iNum, "%llu", lEtime);
            lStime = 0;
            lEtime = 0;
            iReturn = pasDbFetchInto(psCur,
                                     UT_TYPE_LONG8, 8, &lStime,
                                     UT_TYPE_LONG8, 8, &lEtime);
        }
        pasDbCloseCursor(psCur);
    }

    utPltPutVarF(psDbHead, "totalcount", "%lu", iNum);
    utPltPutVar(psDbHead, "cid", caCid);
    utPltPutVar(psDbHead, "servicecode", caServicecode);
    utPltPutVar(psDbHead, "retcode", "0");
    utPltPutVar(psDbHead, "retmsg", "成功");
    utPltOutToHtml(iFd, psMsgHead, psDbHead, "mac/datamining/bab/maccountdetail.htm");
    return 0;
}



int macGetBabPartnerRank(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead)
{
    long babDebug = utComGetVar_ld(psShmHead, "BabDebug", 0);
    if(babDebug)
    {
        utMsgPrintMsg(psMsgHead);
    }
    utPltDbHead *psDbHead = utPltInitDb();

    char caCid[8] = "";
    int iReturn, iNum;
    pasDbCursor *psCur;
    long lCount = 0;
    char caMac[20] = "";
    char sql[2048] = "";

    iReturn = utMsgGetSomeNVar(psMsgHead, 1,
                               "cid",  UT_TYPE_STRING, sizeof(caCid) - 1, caCid);

    if(strlen(caCid) == 0)
    {
        utPltPutVar(psDbHead, "retcode", "2");
        utPltPutVar(psDbHead, "retmsg", "cid不能为空");
        utPltOutToHtml(iFd, psMsgHead, psDbHead, "mac/datamining/bab/macpartnerrank.htm");
        return -1;
    }

    //统计每个mac出现的次数
    memset(sql, 0, sizeof(sql));
    snprintf(sql, sizeof(sql), "select mac,ifnull(count(mac),0) bb from(select mac,starttime from ncbabplacetimemac where cid=%s group by mac,starttime)aa group by mac order by bb desc limit 5", caCid);
    macPrint(babDebug, "========bab sql=[%s]==========\n", sql);
    psCur = pasDbOpenSqlF(sql);
    if(psCur)
    {
        iReturn = 0;
        lCount = 0;
        memset(caMac, 0, sizeof(caMac));
        iReturn = pasDbFetchInto(psCur,
                                 UT_TYPE_STRING, 17, caMac,
                                 UT_TYPE_LONG, 4, &lCount);
        iNum = 0;
        while(iReturn == 0 || iReturn == 1405)
        {
            iNum++;
            if(iNum > 1)
            {
                utPltPutLoopVar(psDbHead, "dh", iNum, ",");
            }
            utPltPutLoopVarF(psDbHead, "maccount", iNum, "%lu", lCount);
            utPltPutLoopVarF(psDbHead, "mac", iNum, "%s", caMac);

            memset(caMac, 0, sizeof(caMac));
            iReturn = pasDbFetchInto(psCur,
                                     UT_TYPE_STRING, 17, caMac,
                                     UT_TYPE_LONG, 4, &lCount);
        }
        pasDbCloseCursor(psCur);
    }

    utPltPutVarF(psDbHead, "totalcount", "%lu", iNum);
    utPltPutVar(psDbHead, "cid", caCid);
    utPltPutVar(psDbHead, "retcode", "0");
    utPltPutVar(psDbHead, "retmsg", "成功");
    utPltOutToHtml(iFd, psMsgHead, psDbHead, "mac/datamining/bab/macpartnerrank.htm");
    return 0;
}


static int insertToDB(long lDebug, struct s_macinfo* pInfoNode)
{
    char sql[2048] = "";
    if(((strlen(pInfoNode->mac) > 0) && (strlen(pInfoNode->servicecode) > 0)))
    {
        memset(sql, 0, sizeof(sql));
        sprintf(sql, "insert into ncbabplacetimemac(cid,mac,servicecode,starttime,endtime) \
        	 values(%lu,'%s','%s',%llu,%llu) ", pInfoNode->cid,
                pInfoNode->mac, pInfoNode->servicecode, pInfoNode->starttime, pInfoNode->endtime);
        macPrint(lDebug, "insert db sql=%s", sql);
        int iReturn = pasDbExecSqlF(sql);
        macPrint(lDebug, "insert db sql = %s\n", sql);
    }
    return 0;
}

static int deleteSameRule(long lDebug, uint4 cid)
{
    char sql[1024] = "";
    sprintf(sql, "delete from ncbabplacetimemac where cid=%lu", cid);
    macPrint(lDebug, "delete db sql = %s\n", sql);
    return pasDbExecSqlF(sql);
}

static int updateReportTime(long lDebug, uint4 cid)
{
    char sql[1024] = "";
    sprintf(sql, "update ncbabtask set reoporttime=%lu where cid=%lu", time(0), cid);

    macPrint(lDebug, "update db sql = %s\n", sql);

    return pasDbExecSqlF(sql);
}

static void doDbSearchMacs(long lDebug,  unsigned char* pHash_onePointMacs, s_eachpoint* pTmpNode,
                           unsigned char* pHash_macCount)
{
    struct s_macinfo* psMacInfo = NULL;
    struct s_maccount* psMacCount = NULL;
    char sql[8023] = "";
    int iReturn = 0;
    pasDbCursor *psCur;
    char caMac[20] = "";
    memset(sql, 0, sizeof(sql));
    sprintf(sql, "select distinct(mac) from ncmactermatt_if_%s where servicecode='%s' and mac!='%s' and stime>=%llu and stime<=%llu",
            utTimFormat("%Y%m", pTmpNode->starttime), pTmpNode->servicecode, pTmpNode->mac, pTmpNode->starttime, pTmpNode->endtime);


    macPrint(lDebug, "get diff mac sql = %s\n", sql);
    psCur = pasDbOpenSql(sql, 0);
    if(psCur)
    {
        memset(caMac, 0, sizeof(caMac));
        iReturn = pasDbFetchInto(psCur, UT_TYPE_STRING, 17, caMac);
        while((iReturn == 0) || (iReturn == 1405))
        {
            //统计信息
            psMacCount = (struct s_maccount*)pasLHashLookA(pHash_macCount, (char*)&caMac);
            if(psMacCount)
            {
                memcpy(psMacCount->mac, caMac, sizeof(psMacCount->mac));
                psMacCount->count++;
            }
            //详细信息
            psMacInfo = (struct s_macinfo*)pasLHashLookA(pHash_onePointMacs, (char*)&caMac);
            if(psMacInfo)
            {
                // DO: 拷贝数据
                memcpy(psMacInfo->mac, caMac, sizeof(psMacInfo->mac));
                memcpy(psMacInfo->servicecode, pTmpNode->servicecode, sizeof(psMacInfo->servicecode));
                psMacInfo->starttime = pTmpNode->starttime;
                psMacInfo->endtime =  pTmpNode->endtime;
                psMacInfo->cid = pTmpNode->cid;
            }
            memset(caMac, 0, sizeof(caMac));
            iReturn = pasDbFetchInto(psCur, UT_TYPE_STRING, 17, caMac);
        }
        pasDbCloseCursor(psCur);
    }
    else
    {
        macPrint(lDebug, "sql=[%s],cur is null\n", sql);
    }
}

static int isInRankArray(struct s_maccount* rankArray, ulong arrayLen, char* mac)
{
    ulong i = 0;
    for(i = 0; i < arrayLen; i++)
    {
        if(strcmp(rankArray[i].mac, mac) == 0)
            return 1;
    }
    return 0;
}

static int deleteHashNotInRankArray(long babDebug, struct s_maccount* rankArray, ulong arrayLen, unsigned char* pHash_onePointMac)
{
    struct s_macinfo* psMacs = NULL;
    pasLHashInfo sHashInfo;
    psMacs = (struct s_macinfo*)pasLHashFirst(pHash_onePointMac, &sHashInfo);
    while(psMacs)
    {
        if(!isInRankArray(rankArray, arrayLen, psMacs->mac))
        {
            pasLHashDel(pHash_onePointMac, psMacs->mac);
            //macPrint(babDebug, "delete mac=[%s],servicode=[%s] from hash\n", psMacs->mac, psMacs->servicecode);
        }
        psMacs = (struct s_macinfo*)pasLHashNext(&sHashInfo);
    }
    return 0;



}

static int doDbInsertMacs(long lDebug, unsigned char* pHash_onePointMac)
{
    struct s_macinfo* psMacs = NULL;
    pasLHashInfo sHashInfo;
    psMacs = (struct s_macinfo*)pasLHashFirst(pHash_onePointMac, &sHashInfo);
    while(psMacs)
    {
        insertToDB(lDebug, psMacs);
        psMacs = (struct s_macinfo*)pasLHashNext(&sHashInfo);
    }
    return 0;
}

static int InsertToRankList(struct s_maccount* rankArray, ulong arrayLen, struct s_maccount* pMacCount)
{
    //从大到小排序前20名存到里面
    ulong i = 0;
    ulong lIndex = -1;
    for(i = 0; i < arrayLen; i++)
    {
        if(pMacCount->count > rankArray[i].count)
        {
            //当前序号后面的数据整体往后移
            lIndex = i;
            break;
        }
    }
    if(lIndex > -1)
    {
        for(i = arrayLen - 1; i > lIndex; i--)
        {
            memcpy(&rankArray[i], &rankArray[i - 1], sizeof(struct s_maccount));
        }
        memcpy(&rankArray[lIndex], pMacCount, sizeof(struct s_maccount));
    }

    return 0;
}

int incMonth(int* iYear, int* iMonth)
{
    int tYear = *iYear;
    int tMonth = (*iMonth) + 1;
    if(tMonth > 12)
    {
        tYear += 1;
        tMonth = 1;
    }
    *iYear = tYear;
    *iMonth = tMonth;
    return 0;
}

int doQueryDbInPeriod(long babDebug, struct s_babtask* psTask,  s_eachpoint* pPointList, char* caServerlist)
{
    if(psTask->starttime > psTask->endtime)
    {
        macPrint(babDebug, "starttime is larger\n");
        return 1;
    }
    char sql[2048] = "";
    pasDbCursor *psCur;
    int iReturn = 0;
    s_eachpoint tmpPoint;
    char caSyear[5] = "", caSmonth[3] = "";
    char caEyear[5] = "", caEmonth[3] = "";
    int iSyear = 0, iSmonth = 0;
    int iNyear = 0, iNmonth = 0;
    int iEyear = 0, iEmonth = 0;
    //超过六个月则只统计最近六个月
    if(psTask->endtime - psTask->starttime > 3600 * 24 * 30 * 6)
    {
        psTask->starttime = psTask->endtime - 3600 * 24 * 30 * 6;
    }
    //1 得到stime对应的年月
    snprintf(caSyear, sizeof(caSyear), "%s", utTimFormat("%Y", psTask->starttime));
    snprintf(caSmonth, sizeof(caSmonth), "%s", utTimFormat("%m", psTask->starttime));
    iSyear = atoi(caSyear);
    iSmonth = atoi(caSmonth);
    macPrint(babDebug, "starttime to [%s,%s,%d,%d]\n", caSyear, caSmonth, iSyear, iSmonth);
    //2 得到etime对应的年月
    snprintf(caEyear, sizeof(caEyear), "%s", utTimFormat("%Y", psTask->endtime));
    snprintf(caEmonth, sizeof(caEmonth), "%s", utTimFormat("%m", psTask->endtime));
    iEyear = atoi(caEyear);
    iEmonth = atoi(caEmonth);
    macPrint(babDebug, "endtime to [%s,%s,%d,%d]\n", caEyear, caEmonth, iEyear, iEmonth);
    //依次递增计算表名
    iNyear = iSyear;
    iNmonth = iSmonth;
    while(1)
    {
        memset(sql, 0, sizeof(sql));
        //e.g. select mac,servicecode,from_unixtime(stime) from ncmactermatt_if_201508 where mac='20-02-AF-C5-43-AC' group by stime
        if(strlen(caServerlist) > 0)
        {
            sprintf(sql, "select servicecode,stime from ncmactermatt_if_%04d%02d where mac='%s' and stime>%llu and stime<%llu and servicecode in (%s) group by stime", iNyear, iNmonth, psTask->mac, psTask->starttime, psTask->endtime, caServerlist);
        }
        else
        {
            sprintf(sql, "select servicecode,stime from ncmactermatt_if_%04d%02d where mac='%s' and stime>%llu and stime<%llu group by stime", iNyear, iNmonth, psTask->mac, psTask->starttime, psTask->endtime);
        }
        macPrint(babDebug, "get time list sql=%s\n", sql);
        psCur = pasDbOpenSql(sql, 0);
        if(psCur)
        {
            memset(&tmpPoint, 0, sizeof(tmpPoint));
            tmpPoint.cid = psTask->cid;
            memcpy(tmpPoint.mac, psTask->mac, sizeof(tmpPoint.mac));
            iReturn = pasDbFetchInto(psCur,
                                     UT_TYPE_STRING, 16, tmpPoint.servicecode,
                                     UT_TYPE_LONG8, 8, &tmpPoint.starttime);
            tmpPoint.endtime = tmpPoint.starttime + 3600;//离开时间默认为起始时间+1小时
            while((iReturn == 0) || (iReturn == 1405))
            {
                macPrint(babDebug, "========add pair<starttime=%llu, servicecode=%s>=======\n\n", tmpPoint.starttime, tmpPoint.servicecode);
                addToPointList(pPointList, &tmpPoint);
                memset(&tmpPoint, 0, sizeof(tmpPoint));
                tmpPoint.cid = psTask->cid;
                memcpy(tmpPoint.mac, psTask->mac, sizeof(tmpPoint.mac));
                iReturn = pasDbFetchInto(psCur,
                                         UT_TYPE_STRING, 16, tmpPoint.servicecode,
                                         UT_TYPE_LONG8, 8, &tmpPoint.starttime);
                tmpPoint.endtime = tmpPoint.starttime + 3600;
            }
            pasDbCloseCursor(psCur);
        }
        macPrint(babDebug, "N[%d,%d],E[%d,%d]\n", iNyear, iNmonth, iEyear, iEmonth);
        if((iNyear >= iEyear) && (iNmonth >= iEmonth))
            break;
        incMonth(&iNyear, &iNmonth);
    }
    return 0;
}


int ncsStatMacBab(utShmHead *psShmHead)
{
    pasDbCursor *psCur;
    char sql[2048] = "";
    long lSumTask = 0, taskCount = 0;
    unsigned char* pHash_task = NULL;
    unsigned char** pHash_allMacs = NULL;
    unsigned char* pHash_macCount = NULL;

    struct s_babtask tmpTask;
    s_eachpoint tmpPoint;
    struct s_babtask* psTask = NULL;
    struct s_maccount* psMacCount = NULL;
    pasLHashInfo sHashInfo, sHashInfoTask;
    char caServerlist[8024] = "";
    long babDebug = utComGetVar_ld(psShmHead, "BabDebug", 0);
    int iReturn = pasConnect(psShmHead);
    if(iReturn < 0)
    {
        printf("conn psShmHead fail iReturn=%d\n", iReturn);
        sleep(60);
        return 0;
    }
    while(1)
    {
        //1、统计总共有多少条规则
        memset(sql, 0, sizeof(sql));
        sprintf(sql, "select count(cid) from ncbabtask where length(mac)>0");
        iReturn = pasDbOneRecord(sql, 0, UT_TYPE_LONG, 4, &lSumTask);
        macPrint(babDebug, "iReturn=%d,lSumTask=%d\n", iReturn, lSumTask);
        if(iReturn != 0 || lSumTask == 0)
        {
            macPrint(babDebug, "不进入后面过程，休眠一段时间后继续\n\n");
            sleep(3 * 60);
            continue;
            //return 0;
        }
        //2、取出所有规则的明细信息装入哈希
        pHash_task = (unsigned char *)pasLHashInit(5000, 5000, sizeof(struct s_babtask), 0, 4);
        if(pHash_task == NULL)
        {
            printf("分配内存出错\n");
            return 0;
        }
        memset(sql, 0, sizeof(sql));
        sprintf(sql, "select cid,groupid,starttime,endtime,dids,mac from ncbabtask");
        psCur = pasDbOpenSql(sql, 0);
        if(psCur)
        {
            memset(&tmpTask, 0, sizeof(tmpTask));
            iReturn = pasDbFetchInto(psCur,
                                     UT_TYPE_LONG, 4, &tmpTask.cid,
                                     UT_TYPE_LONG, 4, &tmpTask.groupid,
                                     UT_TYPE_LONG8, 8, &tmpTask.starttime,
                                     UT_TYPE_LONG8, 8, &tmpTask.endtime,
                                     UT_TYPE_STRING, 250, tmpTask.dids,
                                     UT_TYPE_STRING, 18, tmpTask.mac);

            macPrint(babDebug, "first sql=[%s],fetchinto ret=%d\n", sql, iReturn);

            while((iReturn == 0) || (iReturn == 1405))
            {
                macPrint(babDebug, "fetch into ret=%d,cid=%d,mac=%s\n", iReturn, tmpTask.cid, tmpTask.mac);
                if(strlen(tmpTask.mac) == 17)
                {
                    psTask = (struct s_babtask *)pasLHashLookA(pHash_task, (char*)&tmpTask.cid);
                    if(psTask)
                    {
                        psTask->cid = tmpTask.cid;
                        psTask->groupid = tmpTask.groupid;
                        psTask->starttime = tmpTask.starttime;
                        psTask->endtime = tmpTask.endtime;
                        strcpy(psTask->dids, tmpTask.dids);
                        strcpy(psTask->mac, tmpTask.mac);
                        macPrint(babDebug, "insert to hash cid=%lu, starttime=%llu, endtime=%llu\n",
                                 psTask->cid, psTask->starttime, psTask->endtime);
                    }
                }
                else
                {
                    macPrint(babDebug, "主mac不为17位, mac=[%s]\n", tmpTask.mac);
                }

                memset(&tmpTask, 0, sizeof(tmpTask));
                iReturn = pasDbFetchInto(psCur,
                                         UT_TYPE_LONG, 4, &tmpTask.cid,
                                         UT_TYPE_LONG, 4, &tmpTask.groupid,
                                         UT_TYPE_LONG8, 8, &tmpTask.starttime,
                                         UT_TYPE_LONG8, 8, &tmpTask.endtime,
                                         UT_TYPE_STRING, 250, tmpTask.dids,
                                         UT_TYPE_STRING, 18, tmpTask.mac);
            }
            pasDbCloseCursor(psCur);
        }
        else
        {
            printf("error sql=[%s],cur is null\n", sql);
        }
        //3、遍历哈希表做任务对应的统计
        psTask = (struct s_babtask*)pasLHashFirst(pHash_task, &sHashInfoTask);
        taskCount = 0;
        while(psTask)
        {
            deleteSameRule(babDebug, psTask->cid);
            memset(caServerlist, 0, sizeof(caServerlist));
            // DO:根据dids获取servicecode的列表
            strcpy(caServerlist, getServicecodesByDids(psShmHead, psTask->dids));
            macPrint(babDebug, "finish get servicecode list %s\n", caServerlist);
            //创建一个单链表用于存放时间段+servicecode组合
            s_eachpoint* pPointList = createPointList();  //用于存放时间点+地点的单链表
            //遍历表里的每一个在时间段范围内的月
            doQueryDbInPeriod(babDebug, psTask, pPointList, caServerlist);
            //整理列表里相隔时间过近的点,合并为一个点
            reOrganizePointList(pPointList);
            ulong lPointCount = getPointListLength(pPointList);
            if(lPointCount > 0)
            {
                pHash_allMacs = (unsigned char**)malloc(sizeof(unsigned char*)*lPointCount);
                pHash_macCount = (unsigned char *)pasLHashInit(1000, 1000, sizeof(struct s_maccount), 0, 17);
                if(pHash_macCount == NULL)
                {
                    printf("分配pHash_macCount内存出错\n");
                    return 0;
                }
                ulong lIndex = 0;
                s_eachpoint* pTmpNode = pPointList->pNext;
                //统计所有的mac地址
                while(pTmpNode)
                {
                    pHash_allMacs[lIndex] = (unsigned char *)pasLHashInit(5000, 5000, sizeof(struct s_macinfo), 0, 17);
                    if(pHash_allMacs[lIndex] == NULL)
                    {
                        printf("分配内存出错\n");
                        return 0;
                    }
                    macPrint(babDebug, "before search pair<starttime=%llu, servicecode=%s>\n", pTmpNode->starttime, pTmpNode->servicecode);
                    doDbSearchMacs(babDebug, pHash_allMacs[lIndex], pTmpNode, pHash_macCount);
                    pTmpNode = pTmpNode->pNext;
                    lIndex ++;
                }
                //统计得到前20名的mac
                struct s_maccount rankArray[20];
                for(lIndex = 0; lIndex < 20; lIndex++)
                {
                    memset(&rankArray[lIndex], 0, sizeof(struct s_maccount));
                }

                psMacCount = (struct s_maccount*)pasLHashFirst(pHash_macCount, &sHashInfo);
                while(psMacCount)
                {
                    InsertToRankList(rankArray, 20, psMacCount);
                    psMacCount = (struct s_maccount*)pasLHashNext(&sHashInfo);
                }
                //将所有的hash存到数据库并释放hash
                for(lIndex = 0; lIndex < lPointCount; lIndex++)
                {
                    if(pHash_allMacs[lIndex])
                    {
                        //删除哈希表里不在前二十的mac
                        deleteHashNotInRankArray(babDebug, rankArray, 20, pHash_allMacs[lIndex]);
                        //遍历pHash_allPoints，将所有的点存入数据库
                        doDbInsertMacs(babDebug, pHash_allMacs[lIndex]);
                        free(pHash_allMacs[lIndex]);
                    }
                }
                if(pHash_macCount)
                    free(pHash_macCount);
                if(pHash_allMacs)
                    free(pHash_allMacs);
            }
            //释放内存
            destroyPointList(pPointList);
            updateReportTime(babDebug, psTask->cid);
            psTask = (struct s_babtask*)pasLHashNext(&sHashInfoTask);
            macPrint(babDebug, "====================finish %d task, total %d=================== \n", (++taskCount), lSumTask);
            sleep(2);
        }
        if(pHash_task)
            free(pHash_task);
        macPrint(babDebug, "\n================finish one round, will start in 3*60s...===========\n\n");
        sleep(3 * 60);
    }
}

typedef struct
{
    char tableName[30];
    char tableDesc[1024];
} s_tableInfo;

int getSqlQueryTableList(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead)
{
    long sqlDebug = utComGetVar_ld(psShmHead, "SqlDebug", 0);
    utPltDbHead *psDbHead;
    char sql[1024] = "";
    char sTableDate[9] = "";
    int iReturn = 0;
    pasDbCursor *psCur;
    if(sqlDebug)
    {
        utMsgPrintMsg(psMsgHead);
    }
    psDbHead = utPltInitDb();
    /*
    iReturn = utMsgGetSomeNVar(psMsgHead, 2,
                               "groupid",  UT_TYPE_STRING, sizeof(groupid) - 1, groupid,
                               "range_type", UT_TYPE_STRING, sizeof(range_type) - 1, range_type);
    macPrint(sqlDebug, "[groupid=%s,range_type=%s]\n", groupid, range_type);
    */
    //获取当前的年月
    snprintf(sTableDate, sizeof(sTableDate) - 1, "%s", utTimFormat("%Y%m", time(0)));
    s_tableInfo tables[3];
    memset(tables, 0, 3 * sizeof(s_tableInfo));
    snprintf(tables[0].tableName, 29, "ncmactermatt_if_%s", sTableDate);
    snprintf(tables[0].tableDesc + strlen(tables[0].tableDesc), sizeof(tables[0].tableDesc) - 1, "+-------------+------------+------+-----+---------+-------+");
    snprintf(tables[0].tableDesc + strlen(tables[0].tableDesc), sizeof(tables[0].tableDesc) - 1, "| Field       | Type       | Null | Key | Default | Extra |");
    snprintf(tables[0].tableDesc + strlen(tables[0].tableDesc), sizeof(tables[0].tableDesc) - 1, "+-------------+------------+------+-----+---------+-------+");
    snprintf(tables[0].tableDesc + strlen(tables[0].tableDesc), sizeof(tables[0].tableDesc) - 1, "| mac         | char(17)   | NO   |     | NULL    |       |");
    snprintf(tables[0].tableDesc + strlen(tables[0].tableDesc), sizeof(tables[0].tableDesc) - 1, "| termtype    | char(128)  | YES  |     |         |       |");
    snprintf(tables[0].tableDesc + strlen(tables[0].tableDesc), sizeof(tables[0].tableDesc) - 1, "| servicecode | char(14)   | NO   |     | NULL    |       |");
    snprintf(tables[0].tableDesc + strlen(tables[0].tableDesc), sizeof(tables[0].tableDesc) - 1, "| servicename | char(128)  | YES  |     |         |       |");
    snprintf(tables[0].tableDesc + strlen(tables[0].tableDesc), sizeof(tables[0].tableDesc) - 1, "| address     | char(128)  | YES  |     |         |       |");
    snprintf(tables[0].tableDesc + strlen(tables[0].tableDesc), sizeof(tables[0].tableDesc) - 1, "| stime       | bigint(20) | NO   |     | NULL    |       |");
    snprintf(tables[0].tableDesc + strlen(tables[0].tableDesc), sizeof(tables[0].tableDesc) - 1, "| rssi        | char(8)    | NO   |     | NULL    |       |");
    snprintf(tables[0].tableDesc + strlen(tables[0].tableDesc), sizeof(tables[0].tableDesc) - 1, "| vtype       | int(1)     | YES  |     | 0       |       |");
    snprintf(tables[0].tableDesc + strlen(tables[0].tableDesc), sizeof(tables[0].tableDesc) - 1, "| vname       | char(64)   | YES  |     |         |       |");
    snprintf(tables[0].tableDesc + strlen(tables[0].tableDesc), sizeof(tables[0].tableDesc) - 1, "| ssid        | char(255)  | YES  |     |         |       |");
    snprintf(tables[0].tableDesc + strlen(tables[0].tableDesc), sizeof(tables[0].tableDesc) - 1, "| apname      | char(21)   | NO   |     | NULL    |       |");
    snprintf(tables[0].tableDesc + strlen(tables[0].tableDesc), sizeof(tables[0].tableDesc) - 1, "| apmac       | char(17)   | NO   |     | NULL    |       |");
    snprintf(tables[0].tableDesc + strlen(tables[0].tableDesc), sizeof(tables[0].tableDesc) - 1, "| channel     | char(3)    | YES  |     |         |       |");
    snprintf(tables[0].tableDesc + strlen(tables[0].tableDesc), sizeof(tables[0].tableDesc) - 1, "| security    | char(2)    | YES  |     |         |       |");
    snprintf(tables[0].tableDesc + strlen(tables[0].tableDesc), sizeof(tables[0].tableDesc) - 1, "| xpos        | char(8)    | YES  |     |         |       |");
    snprintf(tables[0].tableDesc + strlen(tables[0].tableDesc), sizeof(tables[0].tableDesc) - 1, "| ypos        | char(8)    | YES  |     |         |       |");
    snprintf(tables[0].tableDesc + strlen(tables[0].tableDesc), sizeof(tables[0].tableDesc) - 1, "| longitude   | char(11)   | NO   |     | NULL    |       |");
    snprintf(tables[0].tableDesc + strlen(tables[0].tableDesc), sizeof(tables[0].tableDesc) - 1, "| latitude    | char(11)   | NO   |     | NULL    |       |");
    snprintf(tables[0].tableDesc + strlen(tables[0].tableDesc), sizeof(tables[0].tableDesc) - 1, "| flags       | int(1)     | YES  |     | 0       |       |");
    snprintf(tables[0].tableDesc + strlen(tables[0].tableDesc), sizeof(tables[0].tableDesc) - 1, "| assid       | char(255)  | YES  |     |         |       |");
    snprintf(tables[0].tableDesc + strlen(tables[0].tableDesc), sizeof(tables[0].tableDesc) - 1, "| sid         | bigint(20) | YES  |     | 0       |       |");
    snprintf(tables[0].tableDesc + strlen(tables[0].tableDesc), sizeof(tables[0].tableDesc) - 1, "+-------------+------------+------+-----+---------+-------+");
    snprintf(tables[1].tableName, 29, "ncmachotinfo_if_%s", sTableDate);
    snprintf(tables[1].tableDesc + strlen(tables[1].tableDesc), sizeof(tables[1].tableDesc) - 1, "+-------------+------------+------+-----+---------+-------+");
    snprintf(tables[1].tableDesc + strlen(tables[1].tableDesc), sizeof(tables[1].tableDesc) - 1, "| Field       | Type       | Null | Key | Default | Extra |");
    snprintf(tables[1].tableDesc + strlen(tables[1].tableDesc), sizeof(tables[1].tableDesc) - 1, "+-------------+------------+------+-----+---------+-------+");
    snprintf(tables[1].tableDesc + strlen(tables[1].tableDesc), sizeof(tables[1].tableDesc) - 1, "| mac         | char(17)   | NO   |     | NULL    |       |");
    snprintf(tables[1].tableDesc + strlen(tables[1].tableDesc), sizeof(tables[1].tableDesc) - 1, "| ssid        | char(64)   | YES  |     |         |       |");
    snprintf(tables[1].tableDesc + strlen(tables[1].tableDesc), sizeof(tables[1].tableDesc) - 1, "| channel     | char(3)    | NO   |     | NULL    |       |");
    snprintf(tables[1].tableDesc + strlen(tables[1].tableDesc), sizeof(tables[1].tableDesc) - 1, "| servicecode | char(14)   | NO   |     | NULL    |       |");
    snprintf(tables[1].tableDesc + strlen(tables[1].tableDesc), sizeof(tables[1].tableDesc) - 1, "| servicename | char(128)  | YES  |     |         |       |");
    snprintf(tables[1].tableDesc + strlen(tables[1].tableDesc), sizeof(tables[1].tableDesc) - 1, "| address     | char(128)  | YES  |     |         |       |");
    snprintf(tables[1].tableDesc + strlen(tables[1].tableDesc), sizeof(tables[1].tableDesc) - 1, "| stime       | bigint(20) | NO   |     | NULL    |       |");
    snprintf(tables[1].tableDesc + strlen(tables[1].tableDesc), sizeof(tables[1].tableDesc) - 1, "| rssi        | char(8)    | NO   |     | NULL    |       |");
    snprintf(tables[1].tableDesc + strlen(tables[1].tableDesc), sizeof(tables[1].tableDesc) - 1, "| apname      | char(21)   | NO   |     | NULL    |       |");
    snprintf(tables[1].tableDesc + strlen(tables[1].tableDesc), sizeof(tables[1].tableDesc) - 1, "| apmac       | char(17)   | NO   |     | NULL    |       |");
    snprintf(tables[1].tableDesc + strlen(tables[1].tableDesc), sizeof(tables[1].tableDesc) - 1, "| security    | char(2)    | YES  |     |         |       |");
    snprintf(tables[1].tableDesc + strlen(tables[1].tableDesc), sizeof(tables[1].tableDesc) - 1, "| xpos        | char(8)    | YES  |     |         |       |");
    snprintf(tables[1].tableDesc + strlen(tables[1].tableDesc), sizeof(tables[1].tableDesc) - 1, "| ypos        | char(8)    | YES  |     |         |       |");
    snprintf(tables[1].tableDesc + strlen(tables[1].tableDesc), sizeof(tables[1].tableDesc) - 1, "| longitude   | char(11)   | NO   |     | NULL    |       |");
    snprintf(tables[1].tableDesc + strlen(tables[1].tableDesc), sizeof(tables[1].tableDesc) - 1, "| latitude    | char(11)   | NO   |     | NULL    |       |");
    snprintf(tables[1].tableDesc + strlen(tables[1].tableDesc), sizeof(tables[1].tableDesc) - 1, "| flags       | int(1)     | YES  |     | 0       |       |");
    snprintf(tables[1].tableDesc + strlen(tables[1].tableDesc), sizeof(tables[1].tableDesc) - 1, "| sid         | bigint(20) | YES  |     | 0       |       |");
    snprintf(tables[1].tableDesc + strlen(tables[1].tableDesc), sizeof(tables[1].tableDesc) - 1, "+-------------+------------+------+-----+---------+-------+");
    snprintf(tables[2].tableName, 29, "ncmacmobileap_if_%s", sTableDate);
    snprintf(tables[2].tableDesc + strlen(tables[2].tableDesc), sizeof(tables[2].tableDesc) - 1, "+-------------+------------+------+-----+---------+-------+");
    snprintf(tables[2].tableDesc + strlen(tables[2].tableDesc), sizeof(tables[2].tableDesc) - 1, "| Field       | Type       | Null | Key | Default | Extra |");
    snprintf(tables[2].tableDesc + strlen(tables[2].tableDesc), sizeof(tables[2].tableDesc) - 1, "+-------------+------------+------+-----+---------+-------+");
    snprintf(tables[2].tableDesc + strlen(tables[2].tableDesc), sizeof(tables[2].tableDesc) - 1, "| apname      | char(21)   | NO   |     | NULL    |       |");
    snprintf(tables[2].tableDesc + strlen(tables[2].tableDesc), sizeof(tables[2].tableDesc) - 1, "| servicecode | char(14)   | NO   |     | NULL    |       |");
    snprintf(tables[2].tableDesc + strlen(tables[2].tableDesc), sizeof(tables[2].tableDesc) - 1, "| servicename | char(255)  | YES  |     |         |       |");
    snprintf(tables[2].tableDesc + strlen(tables[2].tableDesc), sizeof(tables[2].tableDesc) - 1, "| longitude   | char(11)   | NO   |     | NULL    |       |");
    snprintf(tables[2].tableDesc + strlen(tables[2].tableDesc), sizeof(tables[2].tableDesc) - 1, "| latitude    | char(11)   | NO   |     | NULL    |       |");
    snprintf(tables[2].tableDesc + strlen(tables[2].tableDesc), sizeof(tables[2].tableDesc) - 1, "| stime       | bigint(20) | NO   |     | NULL    |       |");
    snprintf(tables[2].tableDesc + strlen(tables[2].tableDesc), sizeof(tables[2].tableDesc) - 1, "| sid         | bigint(20) | YES  |     | 0       |       |");
    snprintf(tables[2].tableDesc + strlen(tables[2].tableDesc), sizeof(tables[2].tableDesc) - 1, "+-------------+------------+------+-----+---------+-------+");

    int i = 0;
    for(i = 1; i <= 3; i++)
    {
        utPltPutLoopVar(psDbHead, "tablename", i, tables[i - 1].tableName);
        utPltPutLoopVar(psDbHead, "tabledesc", i, tables[i - 1].tableDesc);
        if(i > 1)
        {
            utPltPutLoopVar(psDbHead, "dh", i, ",");
        }
    }
    //返回数据给前端
    utPltPutVar(psDbHead, "result", "1");
    utPltPutVarF(psDbHead, "totalCount", "%lu", 3);
    utPltOutToHtml(iFd, psMsgHead, psDbHead, "mac/sql/tablelist.htm");
    return 0;
}



int doUserSql(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead)
{
    long sqlDebug = utComGetVar_ld(psShmHead, "SqlDebug", 0);
    utPltDbHead *psDbHead;
    char caSql[1024] = "";
    char sql[2048] = "";
    char fileName[128] = "";
    char filePath[128] = "";
    int iReturn = 0;
    if(sqlDebug)
    {
        utMsgPrintMsg(psMsgHead);
    }
    psDbHead = utPltInitDb();

    iReturn = utMsgGetSomeNVar(psMsgHead, 1,
                               "sql",  UT_TYPE_STRING, sizeof(caSql) - 1, caSql);
    macPrint(sqlDebug, "[caSql=%s\n", caSql);
    if(!utFileIsExist("/home/ncmysql/ncs/download"))
    {
        if(mkdir("/home/ncmysql/ncs/download", 555) != 0)
        {
        	utPltPutVar(psDbHead, "result", "0");
            utPltOutToHtml(iFd, psMsgHead, psDbHead, "mac/sql/dosql.htm");
            return 0;
        }
    }

    system("chmod 777 /home/ncmysql/ncs/download && chgrp mysql  /home/ncmysql/ncs/download");
    snprintf(fileName, sizeof(fileName) - 1, "doUserSqlResult_%llu.csv", time(0));
    snprintf(filePath, sizeof(filePath) - 1, "/home/ncmysql/ncs/download/%s", fileName);

    snprintf(sql, sizeof(sql) - 1, "%s into outfile '%s' fields terminated by ',' escaped by '\\\\' lines terminated by '\\n'", caSql, filePath);
    macPrint(sqlDebug, "sql=%s", sql);
    iReturn = pasDbExecSqlF(sql);
    //返回数据给前端
    if(iReturn == 0)
    {
        utPltPutVar(psDbHead, "result", "1");
        utPltPutVarF(psDbHead, "filename", "%s", fileName);
        utPltPutVarF(psDbHead, "filepath", "%s", filePath);
    }
    else
    {
        utPltPutVar(psDbHead, "result", "0");
    }
    utPltOutToHtml(iFd, psMsgHead, psDbHead, "mac/sql/dosql.htm");
    return 0;
}


int downloadSqlResult(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead)
{
    utPltDbHead *psDbHead = utPltInitDb();
    char caPath[128] = "";
	char caDeleteCmd[256] = "";
    char caFileName[128] = "";
    sprintf(caPath, "%s/download", "/home/ncmysql/ncs");
    int iReturn = utMsgGetSomeNVar(psMsgHead, 1,
                                   "filename",  UT_TYPE_STRING, sizeof(caFileName) - 1, caFileName);

    snprintf(caDeleteCmd, sizeof(caDeleteCmd), "rm -rf %s/%s", caPath, caFileName);
	macPrint(1, "%s,%s[%s]", caPath, caFileName,caDeleteCmd);
	system(caDeleteCmd);
    utPltFileDownload(iFd, "application/text", caPath, caFileName, caFileName);
    return 0;
}
//系统刷新
int ncsSysReflesh_v9(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead)
{
    utPltDbHead *psDbHead;
    int iReturn;

    utMsgPrintMsg(psMsgHead);
    psDbHead = utPltInitDb();

    iReturn = 0;
    if(iReturn == 0)
    {
        utPltPutVar(psDbHead, "mesg", "true");
    }
    else
    {
        utPltPutVar(psDbHead, "mesg", "false");
    }

    utPltOutToHtml(iFd, psMsgHead, psDbHead, "v4/ncs_msg.htm");
    iReturn = ncsResetSystem(psShmHead);
    iReturn = ncUtlFileServerReset(psShmHead);
    return 0;
}




int procapFrontpageSetFun(utShmHead * psShmHead)
{
    int iReturn;
    iReturn = pasSetTcpFunName("macFrontPageSearch", macFrontPageSearch, 0);
    iReturn = pasSetTcpFunName("macFrontPageLeftBlocks", macFrontPageLeftBlocks, 0);
    iReturn = pasSetTcpFunName("macFrontPageChart1", macFrontPageChart1, 0);
    iReturn = pasSetTcpFunName("macFrontPageRange", macFrontPageRange, 0);
    iReturn = pasSetTcpFunName("ncsTreeUser_v9", ncsTreeUser_v9, 0);
    iReturn = pasSetTcpFunName("ncsWebClientAdd_wif_v9", ncsWebClientAdd_wif_v9, 0);
    iReturn = pasSetTcpFunName("macGetBabDeviceInfo", macGetBabDeviceInfo, 0);
    iReturn = pasSetTcpFunName("macGetBabMacCount", macGetBabMacCount, 0);
    iReturn = pasSetTcpFunName("macGetBabMacDetail", macGetBabMacDetail, 0);
    iReturn = pasSetTcpFunName("macGetBabPartnerRank", macGetBabPartnerRank, 0);
    iReturn = pasSetTcpFunName("macGetBabPlaceMacCount", macGetBabPlaceMacCount, 0);
    iReturn = pasSetTcpFunName("macGetBabMacPlaceCount", macGetBabMacPlaceCount, 0);
    iReturn = pasSetTcpFunName("macGetBabMacTimeCount", macGetBabMacTimeCount, 0);
    iReturn = pasSetTcpFunName("macGetBabMacTimeDetail", macGetBabMacTimeDetail, 0);
    iReturn = pasSetTcpFunName("getSqlQueryTableList", getSqlQueryTableList, 0);
    iReturn = pasSetTcpFunName("doUserSql", doUserSql, 0);
    iReturn = pasSetTcpFunName("downloadSqlResult", downloadSqlResult, 0);
    iReturn = pasSetTcpFunName("ncsSysReflesh_v9", ncsSysReflesh_v9, 0);
    return 0;
}


