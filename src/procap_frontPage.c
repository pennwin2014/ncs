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

static  pattern_data  *psAcbmPat_bab = NULL;      // 伴随行为模式内容
static  pattern_tree  *psAcbmTree_bab = NULL;     // 伴随行为Acbm模式树



//#include "ncfileserver.h"
#define MAX_TIMEOUT_SECONDS (15*60)
//#define SHOWBAB_DEBUG 1

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
    char servicecode[36];
    long number;
} s_serviceData;

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
    //printf("dsiReturn=%d\n", iReturn);
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
        memset(sql, 0, sizeof(sql));
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
        if(strlen(caGroupSql) > 0)
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
        s_serviceData sdataList[5];
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
        char dispname[64] = "";
        for(iNum = 1; iNum <= lastINum; iNum++)
        {
            utPltPutLoopVarF(psDbHead, "collect_number", iNum, "%lu", sdataList[iNum - 1].number);
            memset(dispname, 0, sizeof(dispname));
            getDispnameByServicecode(psShmHead, sdataList[iNum - 1].servicecode, dispname);

            utPltPutLoopVar(psDbHead, "collect_name", iNum, dispname);

            //getServiceNameByCode(psShmHead, servicecode));
            // printf("iNum=%d,servicecode=%s\n", iNum, servicecode);
            if(iNum > 1)
            {
                utPltPutLoopVar(psDbHead, "dh", iNum, ",");
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
        char apmac[36] = "";
        memset(sql, 0, sizeof(sql));
        psDbHead = utPltInitDb();
        memset(sql, 0, sizeof(sql));
        /*
        if(strlen(caServicecodes) > 0)
        {
            sprintf(sql, "select  ifnull(sum(lcount), 0) as nn, right(apname,12) from ncmactermcount where apname!=\'\' and servicecode in (%s) group by right(apname,12) order by nn desc limit 5", caServicecodes);
        }
        */
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
        pasDbCloseCursor(psCur);

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

    iReturn = utMsgGetSomeNVar(psMsgHead, 1,
                               "groupid",  UT_TYPE_STRING, sizeof(groupid) - 1, groupid);
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
    //统计今日终端数量
    memset(sql, 0, sizeof(sql));

    /*
    if(strlen(caServicecodes) > 0)
    {
        snprintf(sql, sizeof(sql), "select ifnull(count(distinct(mac)), 0) from %s where stime>=%d and stime<=%d and servicecode in (%s)", table_name, lstime, letime, caServicecodes);

    }
    */
    if(strlen(caGroupSql) > 0)
    {
        snprintf(sql, sizeof(sql), "select ifnull(count(distinct(mac)), 0) from %s where stime>=%d and stime<=%d and %s", table_name, lstime, letime, caGroupSql);
    }
    else
    {
        snprintf(sql, sizeof(sql), "select ifnull(count(distinct(mac)), 0) from %s where stime>=%d and stime<=%d", table_name, lstime, letime);
    }
    iReturn = pasDbOneRecord(sql, 0, UT_TYPE_LONG, 4, &todayTermMac);
    if(iReturn != 0)
    {
        printf("errsql=%s,iRet=%d\n", sql, iReturn);
        utPltPutVar(psDbHead, "result", "0");
        utPltOutToHtml(iFd, psMsgHead, psDbHead, "mac/frontPage/leftBlock.htm");
        return -1;
    }
    //统计终端总数
    memset(sql, 0, sizeof(sql));
    if(strlen(caGroupSql) > 0)
    {
        snprintf(sql, sizeof(sql), "select ifnull(count(distinct(mac)),0) from nctermmacs where %s", caGroupSql);
    }
    else
    {
        snprintf(sql, sizeof(sql), "select ifnull(count(distinct(mac)),0) from nctermmacs");
    }
    /*
    if(strlen(caServicecodes) > 0)
    {
        snprintf(sql, sizeof(sql), "select ifnull(count(distinct(mac)),0) from nctermmacs where servicecode in (%s)", caServicecodes);
    }
    else
    {
        snprintf(sql, sizeof(sql), "select ifnull(count(distinct(mac)),0) from nctermmacs");
    }
    */

    //snprintf(sql, sizeof(sql), "select ifnull(sum(lcount),0) from ncmactermcount");
    iReturn = pasDbOneRecord(sql, 0, UT_TYPE_LONG, 4, &totalTermMac);
    if(iReturn != 0)
    {
        printf("errsql=%s,iRet=%d\n", sql, iReturn);
        utPltPutVar(psDbHead, "result", "0");
        utPltOutToHtml(iFd, psMsgHead, psDbHead, "mac/frontPage/leftBlock.htm");
        return -1;
    }
    totalTermMac = totalTermMac + todayTermMac;
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

            /*
            if(psClient[i].status == 1)
            {
                if(psOnline)
                {
                    onlinePlaces++;
                }
                else
                {

                    offlinePlaces++;
                }
            }
            else if(psClient[i].status == 3)
            {
                onlinePlaces++;
            }
            else if(psClient->status == 2)
            {
                offlinePlaces++;
            }
            else
            {
                //不会出现这种
                offlinePlaces++;
            }
            */
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
    utPltDbHead *psDbHead = utPltInitDb();

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

    return iReturn;
}



struct s_nctermtask
{
    uint4 cid;
    uint4 groupid;
    uint8 starttime;
    uint8 endtime;
    char dids[255];
    char mac[20];
    uint4 maccount;
    uint4 servcount;
};

typedef struct infonode
{
    char mac[19];
    char servicecode[17];
    char servicename[128];
    char address[128];//     char(128)
    uint8 stime;//       bigint
    int vtype;//       int(1)
    char vname[64];//       char(64)
    char ssid[255];//        char(255)
    char apname[21];//      char(21)
    char apmac[17];//       char(17)
    char longitude[11];//   char(11)
    char latitude[11];//    char(11)
    struct infonode* pNext;
} s_infonode;

typedef struct timenode
{
    char timestr[17];
    long count;
    s_infonode* pInfoList;
    struct timenode* pNext;
} s_timenode;
typedef struct macnode
{
    char mac[19];
    long count;
    struct macnode* pNext;
} s_macnode;

struct s_uniformtime
{
    char servicecode[14];
    s_timenode* pTimeList;//时间列表的头指针
    s_macnode* pMacList;//mac列表的头指针
};
struct db_nctermsysalarm
{
    uint4 sid;//    int   unsigned auto_increment primary key,
    char alarmcode[14];//     char(14),                                -- 告警类别码
    char alarmlevel[32];//    char(32) default ' ',                -- 告警级别 1-紧急 2-验证 3-一般
    char devtype[4];//       char(4),                             -- 01 -AP 02-场所 03-管理中心 04-上级平台
    uint4 alarmtime;//          int default 0,                         -- 告警时间
    uint4 updatetime;//         int default 0,                  -- 告警上报时间
    char description[128];//          char(128),                          -- 告警描述
    uint4 status;//           int default 0,                    -- 0未确认 1-已确认
};

typedef struct
{
    long appearCount;
    s_infonode* pInfoList;
} s_serviceappear;
static int getTimeListByNodeList(s_timenode* pnode, char* timelist)
{
    //long frontPageDebug = utComGetVar_ld(psShmHead, "FrontDebug", 0);

    s_timenode* ptmp = pnode;
    if(ptmp)
    {
        sprintf(timelist, "'%s'", ptmp->timestr);
    }
    while(ptmp->pNext)
    {
        ptmp = ptmp->pNext;
        sprintf(timelist + strlen(timelist), ",'%s'", ptmp->timestr);
    }

    // printf("timelist=[%s]\n", timelist);

    return 0;
}

static int isMacInList(s_macnode* pnode, char* mac)
{
    s_macnode* ptmp = pnode;
    while(ptmp)
    {
        if(strcmp(ptmp->mac, mac) == 0)
        {
            return 1;
        }
        ptmp = ptmp->pNext;
    }
    return 0;
}


static int findAndIncMac(s_macnode* pnode, char* mac)
{
    s_macnode* ptmp = pnode;
    while(ptmp)
    {
        if(strcmp(ptmp->mac, mac) == 0)
        {
            ptmp->count++;
            //printf("find mac=%s,count=%d\n", mac, ptmp->count);
            return 1;
        }
        ptmp = ptmp->pNext;
    }
    return 0;
}

//新增一个mac
//所以macList的头指针不能用
static int insertMac(s_macnode* pnode, char* mac)
{
    s_macnode* ptmp = pnode;
    s_macnode* pLast = pnode;
    while(ptmp)
    {
        pLast = ptmp;
        ptmp = ptmp->pNext;
    }
    ptmp = (s_macnode*)malloc(sizeof(s_macnode));
    ptmp->count = 1;
    strcpy(ptmp->mac, mac);
    //printf("recieve a mac[%s] from outside\n", mac);
    ptmp->pNext = NULL;
    pLast->pNext = ptmp;
    return 0;
}

static s_infonode* getDetailByMac(char* mac, s_timenode* ptimenode)
{
    s_timenode* pTimeNode = NULL;
    s_infonode* pInfoNode = NULL;
    if(ptimenode)
    {
        pTimeNode = ptimenode->pNext;
        pInfoNode = ptimenode->pInfoList;
    }

    while(pTimeNode)
    {
        pInfoNode = pTimeNode->pInfoList->pNext;
        while(pInfoNode)
        {
            if(strcmp(mac, pInfoNode->mac) == 0)
            {
#ifdef SHOWBAB_DEBUG
                printf("get matched infonode ,mac=[%s]->[%s]\n", mac, pInfoNode->mac);
#endif
                return pInfoNode;
            }
            pInfoNode = pInfoNode->pNext;
        }
        pTimeNode = pTimeNode->pNext;
    }
#ifdef SHOWBAB_DEBUG
    printf("did not get matched infonode ,mac=%s\n", mac);
#endif
    return NULL;
}

static int isAlreadyInAppearList(s_serviceappear* pAppearCount, long sumCount, char* mac)
{
    long i = 0;
    s_infonode* ptmp = NULL;
    for(i = 0; i < sumCount; i++)
    {
        ptmp = pAppearCount[i].pInfoList;
        while(ptmp)
        {
            if(strcmp(mac, ptmp->mac) == 0)
            {
                return 1;
            }
            ptmp = ptmp->pNext;
        }
    }
    return 0;
}


static int insertMacToInfoList(s_infonode* pnode, char* mac, s_infonode* pInfo)
{
    s_infonode* ptmp = pnode;
    s_infonode* pLast = pnode;

    while(ptmp)
    {
        pLast = ptmp;
        ptmp = ptmp->pNext;
    }
    ptmp = (s_infonode*)malloc(sizeof(s_infonode));
    strcpy(ptmp->mac, mac);
    if(pInfo)
    {
        strcpy(ptmp->servicecode, pInfo->servicecode);
        strcpy(ptmp->servicename, pInfo->servicename);
        strcpy(ptmp->address, pInfo->address);
        ptmp->stime = pInfo->stime;
        ptmp->vtype = pInfo->vtype;
        strcpy(ptmp->vname, pInfo->vname);
        strcpy(ptmp->ssid, pInfo->ssid);
        strcpy(ptmp->apname, pInfo->apname);
        strcpy(ptmp->apmac, pInfo->apmac);
        strcpy(ptmp->longitude, pInfo->longitude);
        strcpy(ptmp->latitude, pInfo->latitude);
    }
    ptmp->pNext = NULL;
    pLast->pNext = ptmp;
    return 0;
}


static int insertMacToList(s_macnode* pnode, char* mac)
{
    /*如果没有就新建一个，如果有就count+1*/
    //printf("insert mac[%s] to maclist\n", mac);
    if(!findAndIncMac(pnode, mac))
    {
        //printf("did not find mac=%s\n",mac);
        insertMac(pnode, mac);
    }

    return 0;
}

static int deleteAllMacInfoByMac(s_infonode* totalList, char* mac)
{
    s_infonode* pnode = totalList->pNext;
    s_infonode* pBeforeNode = totalList;
    while(pnode)
    {
        if(strcmp(pnode->mac, mac) == 0)
        {
            pBeforeNode->pNext = pnode->pNext;
            free(pnode);
            pnode = pBeforeNode->pNext;
        }
        pBeforeNode = pnode;
        if(pnode)
            pnode = pnode->pNext;
    }
    return 0;
}

static int deleteMacUnmatch(s_macnode* pMacNode, struct s_nctermtask* psTask, s_infonode* totalMacList)
{
    //首指针为0不算
    s_macnode* pbefore = pMacNode;
    s_macnode* ptmp = NULL;
    if(pMacNode)
    {
        ptmp = pMacNode->pNext;
    }

    while(ptmp)
    {
        if(ptmp->count < psTask->maccount)
        {
            //删除这个节点
            pbefore->pNext = ptmp->pNext;
#ifdef SHOWBAB_DEBUG
            printf("delete a mac=[%s],because count=%lu, and maccount=%lu \n", ptmp->mac, ptmp->count, psTask->maccount);
#endif
            free(ptmp);
            ptmp = pbefore;
            //删除掉totalMacList中所有mac等于这个mac的数据
            deleteAllMacInfoByMac(totalMacList, ptmp->mac);
        }
        else
        {
#ifdef SHOWBAB_DEBUG
            printf("keep a mac=[%s],because count=%lu, and maccount=%lu \n", ptmp->mac, ptmp->count, psTask->maccount);
#endif
        }
        pbefore = ptmp;
        ptmp = ptmp->pNext;
    }
    return 0;
}

static int insertToDB(s_infonode* pInfoNode, struct s_nctermtask* psTask)
{
    char sql[2048] = "";
    if(((strlen(pInfoNode->mac) > 0) && (strlen(pInfoNode->servicecode) > 0)))
    {
        memset(sql, 0, sizeof(sql));
#ifdef SHOWBAB_DEBUG
        printf("%lu,'%s','%s','%s','%s',%llu,%d,'%s','%s','%s','%s','%s','%s'\n", psTask->cid, pInfoNode->mac, pInfoNode->servicecode, pInfoNode->servicename,
               pInfoNode->address, pInfoNode->stime, pInfoNode->vtype, pInfoNode->vname, pInfoNode->ssid,
               pInfoNode->apname, pInfoNode->apmac, pInfoNode->longitude, pInfoNode->latitude);
#endif
        sprintf(sql, "insert into nctermtasklog(cid,mac,servicecode,servicename,address,stime,vtype,vname,ssid,apname,apmac,longitude,latitude) \
        	 values(%lu,'%s','%s','%s','%s',%llu,%d,'%s','%s','%s','%s','%s','%s') ",
                psTask->cid, pInfoNode->mac, pInfoNode->servicecode, pInfoNode->servicename,
                pInfoNode->address, pInfoNode->stime, pInfoNode->vtype, pInfoNode->vname, pInfoNode->ssid,
                pInfoNode->apname, pInfoNode->apmac, pInfoNode->longitude, pInfoNode->latitude);
        int iReturn = pasDbExecSqlF(sql);
#ifdef SHOWBAB_DEBUG
        printf("insert db sql = %s\n", sql);
#endif
    }
    return 0;
}

static int deleteSameRule(uint4 cid)
{
    char sql[1024] = "";
    sprintf(sql, "delete from nctermtasklog where cid=%lu", cid);
#ifdef SHOWBAB_DEBUG
    printf("delete db sql = %s\n", sql);
#endif
    return pasDbExecSqlF(sql);
}

static int updateReportTime(uint4 cid)
{
    char sql[1024] = "";
    sprintf(sql, "update nctermtask set reoporttime=%lu where cid=%lu", time(0), cid);
#ifdef SHOWBAB_DEBUG
    printf("update db sql = %s\n", sql);
#endif
    return pasDbExecSqlF(sql);
}



static int copyNode(s_infonode* toNode, s_infonode* fromNode)
{
    memcpy(toNode, fromNode, sizeof(s_infonode));
    return 0;
}

static int insertMacTotalList(s_infonode* pnode, s_infonode* pInfo)
{
    s_infonode* ptmp = pnode;
    s_infonode* pLast = pnode;
    while(ptmp)
    {
        pLast = ptmp;
        ptmp = ptmp->pNext;
    }
    ptmp = (s_infonode*)malloc(sizeof(s_infonode));
    memset(ptmp, 0, sizeof(s_infonode));
    copyNode(ptmp, pInfo);
    ptmp->pNext = NULL;
    pLast->pNext = ptmp;
}

static int insertTimeToTotalTimeList(s_timenode* pnode, char* timestr)
{
    //如果重复的就不放
    s_timenode* pLast = pnode;
    s_timenode* pTmp = pnode;
    while(pTmp)
    {
        if(strcmp(pTmp->timestr, timestr) == 0)
        {
            pTmp->count++;
            return 1;
        }
        pLast = pTmp;
        pTmp = pTmp->pNext;
    }
    s_timenode* newTime = (s_timenode*)malloc(sizeof(s_timenode));
    pLast->pNext = newTime;
    memset(newTime, 0, sizeof(s_timenode));
    strcpy(newTime->timestr, timestr);
    newTime->pNext = NULL;
    return 0;
}

static int isInSameTime(uint8 lTime, char* sTime)
{
    char tempTime[11] = {0};
    sprintf(tempTime, "%s", utTimFormat("%Y%m%d%H", lTime));
    if(strcmp(tempTime, sTime) == 0)
        return 1;
    else
        return 0;
}


static long getMacCountByTime(s_infonode* pTotalMacList, char* timestr, s_infonode* ptempNode)
{
    long macCount = 0;
    s_infonode* ptemp = pTotalMacList->pNext;
    s_infonode* pLastTemp = ptempNode;
    s_infonode* pTempTemp = ptempNode;
    while(ptemp)
    {
        if(isInSameTime(ptemp->stime, timestr))
        {
            //顺便将该node添加到临时列表以方便最后的插入数据库
            while(pTempTemp)
            {
                pLastTemp = pTempTemp;
                pTempTemp = pTempTemp->pNext;
            }
            s_infonode* newTemp = (s_infonode*)malloc(sizeof(s_infonode));
            pLastTemp->pNext = newTemp;
            memset(newTemp, 0, sizeof(s_infonode));
            copyNode(newTemp, ptemp);
            newTemp->pNext = NULL;
            macCount++;
        }
        ptemp = ptemp->pNext;
    }
    return macCount;
}

static int deleteAllMacByTime(s_infonode* pnode, char* timestr)
{
    s_infonode* pBefore = pnode;
    s_infonode* pTemp = pnode->pNext;
    while(pTemp)
    {
        pBefore = pTemp;
        if(isInSameTime(pTemp->stime, timestr))
        {
            //删掉该结点
            pBefore->pNext = pTemp->pNext;
            free(pTemp);
            pTemp = pBefore;
        }
        pTemp = pTemp->pNext;
    }
    return 0;
}

static int insertAllTempMacToDB(s_infonode* pnode, struct s_nctermtask* psTask)
{
    s_infonode* pTemp = pnode->pNext;
    while(pTemp)
    {
        insertToDB(pTemp, psTask);
        pTemp = pTemp->pNext;
    }
    return 0;
}




int ncsStatMacBab(utShmHead *psShmHead)
{
    pasDbCursor *psCur;
    char sql[2048] = "";
    long lSumTask = 0, lSumService = 0, lMacCount = 0, lAppearCount = 0;
    unsigned char* pHash_task = NULL;
    unsigned char* pHash_utime = NULL; //归一化后的时间
    char** pServicecode = NULL;
    struct s_nctermtask tmpTask;
    struct s_nctermtask* psTask = NULL;
    struct s_uniformtime* psUtime = NULL;
    struct s_uniformtime* psUtime2 = NULL;
    s_timenode* pTimeNode = NULL;
    s_timenode* pTimeNodeBefore = NULL;
    s_macnode* pMacNode = NULL;
    s_macnode* pMacNodeBefore = NULL;
    s_infonode* pInfoNode = NULL;
    s_infonode* pInfoNodeBefore = NULL;
    pasLHashInfo sHashInfo;
    char caServerlist[8024] = "";
    char caTimeList[2048] = "";
    char caMac[19] = "";
    char caUniformTime[17] = "";
    long iNum = 0, i = 0, j = 0;
    long totalAppearTimes = 0;
    /*以下为一些结构体赋值用的临时变量*/
    char caServicecode[17] = "";
    char caServicename[128] = "";
    char caAddress[128] = "";
    char caVname[64] = "";
    long lStime = 0, lVtype = 0;
    long taskCount = 0;
    char caSsid[255] = "";
    char caApname[21] = "";
    char caApmac[17] = "";
    char caLongitude[11] = "";
    char caLatitude[11] = "";
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
        sprintf(sql, "select count(cid) from nctermtask where length(mac)>0");
#ifdef SHOWBAB_DEBUG
        printf("sql=%s\n", sql);
#endif
        iReturn = pasDbOneRecord(sql, 0, UT_TYPE_LONG, 4, &lSumTask);
        printf("iReturn=%d,lSumTask=%d\n", iReturn, lSumTask);
        if(iReturn != 0 || lSumTask == 0)
        {
            return 0;
        }

        //2、取出所有规则的明细信息装入内存
        pHash_task = (unsigned char *)pasLHashInit(5000, 5000, sizeof(struct s_nctermtask), 0, 4);
        if(pHash_task == NULL)
        {
            printf("分配内存出错\n");
            return 0;
        }
        memset(sql, 0, sizeof(sql));
        sprintf(sql, "select cid,groupid,starttime,endtime,dids,mac,maccount,servcount from nctermtask");
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
                                     UT_TYPE_STRING, 18, tmpTask.mac,
                                     UT_TYPE_LONG, 4, &tmpTask.maccount,
                                     UT_TYPE_LONG, 4, &tmpTask.servcount);
#ifdef SHOWBAB_DEBUG
            printf("first sql=[%s],fetchinto ret=%d\n", sql, iReturn);
#endif
            while((iReturn == 0) || (iReturn == 1405))
            {
#ifdef SHOWBAB_DEBUG
                printf("fetch into ret=%d,cid=%d,mac=%s\n", iReturn, tmpTask.cid, tmpTask.mac);
#endif
                psTask = (struct s_nctermtask *)pasLHashLookA(pHash_task, (char*)&tmpTask.cid);
                if(psTask)
                {
                    psTask->cid = tmpTask.cid;
                    psTask->groupid = tmpTask.groupid;
                    psTask->starttime = tmpTask.starttime;
                    psTask->endtime = tmpTask.endtime;
                    strcpy(psTask->dids, tmpTask.dids);
                    strcpy(psTask->mac, tmpTask.mac);
                    psTask->maccount = tmpTask.maccount;
                    psTask->servcount = tmpTask.servcount;
                }
                memset(&tmpTask, 0, sizeof(tmpTask));
                iReturn = pasDbFetchInto(psCur,
                                         UT_TYPE_LONG, 4, &tmpTask.cid,
                                         UT_TYPE_LONG, 4, &tmpTask.groupid,
                                         UT_TYPE_LONG8, 8, &tmpTask.starttime,
                                         UT_TYPE_LONG8, 8, &tmpTask.endtime,
                                         UT_TYPE_STRING, 250, tmpTask.dids,
                                         UT_TYPE_STRING, 18, tmpTask.mac,
                                         UT_TYPE_LONG, 4, &tmpTask.maccount,
                                         UT_TYPE_LONG, 4, &tmpTask.servcount);
            }
            pasDbCloseCursor(psCur);
        }
        else
        {
            printf("sql=[%s],cur is null\n", sql);
        }
        psTask = (struct s_nctermtask*)pasLHashFirst(pHash_task, &sHashInfo);
        taskCount = 0;
        while(psTask)
        {
            //2.5、以下所有查询都基于dids,starttime和endtime为前提条件
            memset(caServerlist, 0, sizeof(caServerlist));
            // DO:根据dids获取servicecode的列表
            strcpy(caServerlist, getServicecodesByDids(psShmHead, psTask->dids));
            if(strlen(caServerlist) == 0)
            {
                psTask = (struct s_nctermtask*)pasLHashNext(&sHashInfo);
                printf("===================dids didn't get servicecode list ,pass %d task, total %d======= \n", (++taskCount), lSumTask);
                sleep(2);
                continue;
            }
#ifdef SHOWBAB_DEBUG
            printf("finish get servicecode list %s\n", caServerlist);
#endif
            //3、获取该mac在该时间段所经历的场所列表(N个)
            lSumService = 0;
            memset(sql, 0, sizeof(sql));
            //sprintf(sql, "select count(*) from (select distinct servicecode from  ncmactermatt_if_201508 where mac='%s' and stime>%llu and stime<%llu and servicecode in (%s)) as aa", psTask->mac, psTask->starttime, psTask->endtime, caServerlist);
            sprintf(sql, "select count(*) from (select distinct servicecode from  ncmactermatt_if_%s where mac='%s' and stime>%llu and stime<%llu) as aa", utTimFormat("%Y%m", time(0)), psTask->mac, psTask->starttime, psTask->endtime);
#ifdef SHOWBAB_DEBUG
            printf("do sql=[%s]\n", sql);
#endif
            iReturn = pasDbOneRecord(sql, 0, UT_TYPE_LONG, 4, &lSumService);
#ifdef SHOWBAB_DEBUG
            printf("ret=%d,lSumService=%d\n", iReturn, lSumService);
#endif
            if(iReturn != 0 || lSumTask == 0)
            {
                return 0;
            }
            pServicecode = (char **)malloc(sizeof(char *) * lSumService);
            memset(sql, 0, sizeof(sql));
            //sprintf(sql, "select distinct servicecode from  ncmactermatt_if_201508 where mac='%s' and stime>%llu and stime<%llu and servicecode in (%s)", psTask->mac, psTask->starttime, psTask->endtime, caServerlist);
            sprintf(sql, "select distinct servicecode from  ncmactermatt_if_%s where mac='%s' and stime>%llu and stime<%llu", utTimFormat("%Y%m", time(0)), psTask->mac, psTask->starttime, psTask->endtime);
#ifdef SHOWBAB_DEBUG
            printf("do get servicecodes sql=%s\n", sql);
#endif
            psCur = pasDbOpenSql(sql, 0);
            if(psCur)
            {
                iNum = 0;
                pServicecode[iNum] = (char *)malloc(20);
                memset(pServicecode[iNum], 0, 20);
                iReturn = pasDbFetchInto(psCur,
                                         UT_TYPE_STRING, 14, pServicecode[iNum]);
#ifdef SHOWBAB_DEBUG
                printf("first ret=%d,pServicecode=%s,iNum=%d,lSumService=%d\n", iReturn, pServicecode[iNum], iNum, lSumService);
#endif
                while((iReturn == 0) || (iReturn == 1405))
                {
                    iNum++;
                    if(iNum >= lSumService)
                        break;
                    pServicecode[iNum] = (char *)malloc(20);
                    memset(pServicecode[iNum], 0, 20);
                    iReturn = pasDbFetchInto(psCur,
                                             UT_TYPE_STRING, 14, pServicecode[iNum]);
#ifdef SHOWBAB_DEBUG
                    printf("fetch ret=%d,pServicecode=%s,iNum=%d,lSumService=%d\n", iReturn, pServicecode[iNum], iNum, lSumService);
#endif
                }
                pasDbCloseCursor(psCur);
            }
            else
            {
                printf("sql=%s,cur is null\n", sql);
            }
            printf("finish step 3\n\n");
            //4、归一化该mac在该些场所所经历的各个时间段并统计其次数(次数可不用)
            pHash_utime = (unsigned char *)pasLHashInit(5000, 5000, sizeof(struct s_uniformtime), 0, 14);
            if(pHash_utime == NULL)
            {
                printf("分配内存出错\n");
                return 0;
            }
            for(i = 0; i < lSumService; i++)
            {
                psUtime = (struct s_uniformtime*)pasLHashLookA(pHash_utime, pServicecode[i]);
                if(psUtime)
                {
                    strcpy(psUtime->servicecode, pServicecode[i]);
                    pTimeNode = (s_timenode*)malloc(sizeof(s_timenode));
                    memset(pTimeNode, 0, sizeof(s_timenode));
                    //为下一个mac简单列表的指针分配内存
                    pTimeNode->pInfoList = (s_infonode*)malloc(sizeof(s_infonode));
                    memset(pTimeNode->pInfoList, 0, sizeof(s_infonode));
                    pTimeNode->pInfoList->pNext = NULL;
                    //下一节点的指针置空
                    pTimeNode->pNext = NULL;
                    psUtime->pTimeList = pTimeNode;
                    //同时也创建一个空的macList
                    pMacNode = (s_macnode*)malloc(sizeof(s_macnode));
                    memset(pMacNode, 0, sizeof(s_macnode));
                    psUtime->pMacList = pMacNode;
                    memset(sql, 0, sizeof(sql));
                    sprintf(sql, "select date_format(from_unixtime(stime),'%%Y%%m%%d%%H'),count(mac) from ncmactermatt_if_%s where servicecode='%s' and mac='%s' and stime>%llu and stime<%llu group by date_format(from_unixtime(stime),'%%Y%%m%%d%%H')",
                            utTimFormat("%Y%m", time(0)), pServicecode[i], psTask->mac, psTask->starttime, psTask->endtime);
#ifdef SHOWBAB_DEBUG
                    printf("first get timestr sql=[%s]\n", sql);
#endif
                    psCur = pasDbOpenSql(sql, 0);
                    if(psCur)
                    {
                        memset(caUniformTime, 0, sizeof(caUniformTime));
                        lMacCount = 0;
                        iReturn = pasDbFetchInto(psCur,
                                                 UT_TYPE_STRING, 16, caUniformTime,
                                                 UT_TYPE_LONG, 4, &lMacCount);
                        while((iReturn == 0) || (iReturn == 1405))
                        {
#ifdef SHOWBAB_DEBUG
                            printf("fetch ret=%d,timestr=%s\n", iReturn, caUniformTime);
#endif
                            pTimeNode->count = lMacCount;
                            strcpy(pTimeNode->timestr, caUniformTime);
                            pTimeNodeBefore = pTimeNode;
                            pTimeNode = (s_timenode*)malloc(sizeof(s_timenode));
                            pTimeNodeBefore->pNext = pTimeNode;
                            memset(pTimeNode, 0, sizeof(s_timenode));
                            //为下一个明细列表的指针分配内存
                            pTimeNode->pInfoList = (s_infonode*)malloc(sizeof(s_infonode));
                            memset(pTimeNode->pInfoList, 0, sizeof(s_infonode));
                            pTimeNode->pInfoList->pNext = NULL;
                            //下一节点的指针置空
                            pTimeNode->pNext = NULL;
                            memset(caUniformTime, 0, sizeof(caUniformTime));
                            lMacCount = 0;
                            iReturn = pasDbFetchInto(psCur,
                                                     UT_TYPE_STRING, 16, caUniformTime,
                                                     UT_TYPE_LONG, 4, &lMacCount);
                        }
                        pasDbCloseCursor(psCur);
                    }
                    else
                    {
                        printf("sql=[%s],cur is null\n", sql);
                    }
                }
            }
            printf("finish step 4 \n\n");
            //5、为每个servicecode下的每个时间点的maclist填上所有出现过的mac
            //5.5、统计5里面的所有mac
            //总mac列表的头指针
            s_infonode* totalMacList = (s_infonode*)malloc(sizeof(s_infonode));
            memset(totalMacList, 0, sizeof(s_infonode));
            s_infonode* pTotalMacList = totalMacList;
            //总的时间的列表
            s_timenode* totalTimeList = (s_timenode*)malloc(sizeof(s_timenode));
            memset(totalTimeList, 0, sizeof(s_timenode));
            s_timenode* pTotalTimeList = totalTimeList;
            if(lSumService > 0)
            {
                psUtime = (struct s_uniformtime*)pasLHashLookA(pHash_utime, pServicecode[0]);
                pMacNode = psUtime->pMacList;//永远用这个作为存放mac详细信息的列表
            }
            for(i = 0; i < lSumService; i++)
            {
                psUtime = (struct s_uniformtime*)pasLHashLookA(pHash_utime, pServicecode[i]);
                if(psUtime)
                {
                    pTimeNode = psUtime->pTimeList;
                    while(pTimeNode)
                    {
                        if(strlen(pTimeNode->timestr) > 0)
                        {
                            insertTimeToTotalTimeList(totalTimeList, pTimeNode->timestr);
                            memset(sql, 0, sizeof(sql));
                            sprintf(sql, "select mac,servicecode,servicename,address,vname,stime,vtype,ssid,apname,apmac,longitude,latitude from ncmactermatt_if_%s where servicecode='%s' and mac!='%s' and stime>%llu and stime<%llu and date_format(from_unixtime(stime),'%%Y%%m%%d%%H')='%s' group by mac",
                                    utTimFormat("%Y%m", time(0)), pServicecode[i], psTask->mac, psTask->starttime, psTask->endtime, pTimeNode->timestr);
#ifdef SHOWBAB_DEBUG
                            printf("Count mac appear in time=[%s]\n sql=%s\n", pTimeNode->timestr, sql);
#endif
                            psCur = pasDbOpenSql(sql, 0);
                            if(psCur)
                            {
                                memset(caMac, 0, sizeof(caMac));
                                memset(caServicecode, 0, sizeof(caServicecode));
                                memset(caServicename, 0, sizeof(caServicename));
                                memset(caAddress, 0, sizeof(caAddress));
                                memset(caVname, 0, sizeof(caVname));
                                memset(caSsid, 0, sizeof(caSsid));
                                memset(caApname, 0, sizeof(caApname));
                                memset(caApmac, 0, sizeof(caApmac));
                                memset(caLongitude, 0, sizeof(caLongitude));
                                memset(caLatitude, 0, sizeof(caLatitude));
                                lStime = 0;
                                lVtype = 0;

                                iReturn = pasDbFetchInto(psCur,
                                                         UT_TYPE_STRING, 18, caMac,
                                                         UT_TYPE_STRING, 14, caServicecode,
                                                         UT_TYPE_STRING, 125, caServicename,
                                                         UT_TYPE_STRING, 125, caAddress,
                                                         UT_TYPE_STRING, 63, caVname,
                                                         UT_TYPE_LONG8, 8, &lStime,
                                                         UT_TYPE_LONG, 4, &lVtype,
                                                         UT_TYPE_STRING, 250, caSsid,
                                                         UT_TYPE_STRING, 20, caApname,
                                                         UT_TYPE_STRING, 16, caApmac,
                                                         UT_TYPE_STRING, 10, caLongitude,
                                                         UT_TYPE_STRING, 10, caLatitude);
                                pInfoNode = pTimeNode->pInfoList;
                                while((iReturn == 0) || (iReturn == 1405))
                                {
                                    strcpy(pInfoNode->mac, caMac);
                                    // DO: 将该mac对应的其他信息保存到内存
                                    strcpy(pInfoNode->servicecode, caServicecode);
                                    strcpy(pInfoNode->servicename, caServicename);
                                    strcpy(pInfoNode->address, caAddress);
                                    pInfoNode->stime = lStime;
                                    pInfoNode->vtype = lVtype;
                                    strcpy(pInfoNode->vname, caVname);
                                    strcpy(pInfoNode->ssid, caSsid);
                                    strcpy(pInfoNode->apname, caApname);
                                    strcpy(pInfoNode->apmac, caApmac);
                                    strcpy(pInfoNode->longitude, caLongitude);
                                    strcpy(pInfoNode->latitude, caLatitude);
                                    //随时记录并统计该mac
                                    insertMacToList(pMacNode, caMac);
                                    insertMacTotalList(totalMacList, pInfoNode);
                                    //创建下一个节点
                                    pInfoNodeBefore = pInfoNode;
                                    pInfoNode = (s_infonode*)malloc(sizeof(s_infonode));
                                    pInfoNodeBefore->pNext = pInfoNode;
                                    memset(pInfoNode, 0, sizeof(s_infonode));
                                    memset(caMac, 0, sizeof(caMac));
                                    memset(caServicecode, 0, sizeof(caServicecode));
                                    memset(caServicename, 0, sizeof(caServicename));
                                    memset(caAddress, 0, sizeof(caAddress));
                                    memset(caVname, 0, sizeof(caVname));
                                    memset(caSsid, 0, sizeof(caSsid));
                                    memset(caApname, 0, sizeof(caApname));
                                    memset(caApmac, 0, sizeof(caApmac));
                                    memset(caLongitude, 0, sizeof(caLongitude));
                                    memset(caLatitude, 0, sizeof(caLatitude));
                                    lStime = 0;
                                    lVtype = 0;
                                    iReturn = pasDbFetchInto(psCur,
                                                             UT_TYPE_STRING, 18, caMac,
                                                             UT_TYPE_STRING, 14, caServicecode,
                                                             UT_TYPE_STRING, 125, caServicename,
                                                             UT_TYPE_STRING, 125, caAddress,
                                                             UT_TYPE_STRING, 63, caVname,
                                                             UT_TYPE_LONG8, 8, &lStime,
                                                             UT_TYPE_LONG, 4, &lVtype,
                                                             UT_TYPE_STRING, 250, caSsid,
                                                             UT_TYPE_STRING, 20, caApname,
                                                             UT_TYPE_STRING, 16, caApmac,
                                                             UT_TYPE_STRING, 10, caLongitude,
                                                             UT_TYPE_STRING, 10, caLatitude);
                                }
                                pasDbCloseCursor(psCur);
                            }
                        }
                        pTimeNode = pTimeNode->pNext;
                    }
                    //6、剔除掉列表里出现时间点个数小于maccount的数据
                    //deleteMacUnmatch(pMacNode, psTask);
                }
            }
            deleteMacUnmatch(pMacNode, psTask, totalMacList);
            printf("finish step 5~6\n\n");
            if(lSumService > 0)
            {
                //7、遍历时间总列表，统计mac总列表中，
                //在该时间点出现的所有不同mac的种数,把种数小于svrcount的剔除
                //7.5、遍历mac总列表，把所有在该时间出现的mac都放进临时列表
                //如果满足条件就把该临时列表的东西存到数据，否则释放临时列表
                pTotalTimeList = totalTimeList->pNext;
                long lDiffMacCount = 0;
                s_infonode* tempInfoList = (s_infonode*)malloc(sizeof(s_infonode));
                memset(tempInfoList, 0, sizeof(s_infonode));
                s_infonode* pTempInfoList = tempInfoList;
                s_infonode* pTempBeforeInfoList = NULL;
                deleteSameRule(psTask->cid);
                while(pTotalTimeList)
                {
                    lDiffMacCount = getMacCountByTime(totalMacList, pTotalTimeList->timestr, tempInfoList);
                    if(lDiffMacCount < psTask->servcount)
                        deleteAllMacByTime(totalMacList, pTotalTimeList->timestr);
                    else
                        insertAllTempMacToDB(tempInfoList, psTask);
                    //目的是头指针不释放
                    pTempInfoList = tempInfoList->pNext;
                    //tempInfoList->pNext = NULL;
                    while(pTempInfoList)
                    {
                        free(pTempInfoList);
                        pTempInfoList = pTempInfoList->pNext;
                    }
                    tempInfoList->pNext = NULL;
                    pTotalTimeList = pTotalTimeList->pNext;
                }
                updateReportTime(psTask->cid);
                //最后再释放头指针
                free(tempInfoList);
            }
            else
            {
                printf("lSumService=0, don't need step 7 above\n");
            }

            //10、释放内存
            for(i = 0; i < lSumService; i++)
            {
                psUtime = (struct s_uniformtime*)pasLHashLookA(pHash_utime, pServicecode[i]);
                if(psUtime)
                {
                    pMacNode = psUtime->pMacList;
                    while(pMacNode)
                    {
                        pMacNodeBefore = pMacNode;
                        pMacNode = pMacNode->pNext;
                        free(pMacNodeBefore);
                    }
                    pTimeNode = psUtime->pTimeList;
                    while(pTimeNode)
                    {
                        pInfoNode = pTimeNode->pInfoList;
                        while(pInfoNode)
                        {
                            pInfoNodeBefore = pInfoNode;
                            pInfoNode = pInfoNode->pNext;
                            free(pInfoNodeBefore);
                        }
                        pTimeNodeBefore = pTimeNode;
                        pTimeNode = pTimeNode->pNext;
                        free(pTimeNodeBefore);
                    }
                }
                free(pServicecode[i]);
            }
            if(pServicecode)
                free(pServicecode);
            if(pHash_utime)
                free(pHash_utime);
            // DO: 释放totalMacList和totalTimeList
            //把头指针也一起释放
            pTotalMacList = totalMacList;
            s_infonode* pTotalBeforeMacList = pTotalMacList;
            while(pTotalMacList)
            {
                pTotalBeforeMacList = pTotalMacList;
                pTotalMacList = pTotalMacList->pNext;
                free(pTotalBeforeMacList);
            }
            pTotalTimeList = totalTimeList;
            s_timenode* pTotalBeforeTimeList = pTotalTimeList;
            while(pTotalTimeList)
            {
                pTotalBeforeTimeList = pTotalTimeList;
                pTotalTimeList = pTotalTimeList->pNext;
                free(pTotalBeforeTimeList);
            }
            psTask = (struct s_nctermtask*)pasLHashNext(&sHashInfo);
            printf("====================finish %d task, total %d=================== \n", (++taskCount), lSumTask);
            sleep(2);
        }
        if(pHash_task)
            free(pHash_task);
        printf("\n================finish one round, will start in 3*60s...===========\n\n");
        sleep(3 * 60);
    }
}
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

int ncsStatMacDevWarn(utShmHead *psShmHead)
{
    char sql[2048] = "";
    long lTime = 0;
    int iReturn = pasConnect(psShmHead);
    if(iReturn < 0)
    {
        printf("conn psShmHead fail iReturn=%d\n", iReturn);
        sleep(60);
        return 0;
    }
    struct db_nctermsysalarm tmpPlaceData;
    ncsOnline *psOnline;
    ncsClient *psClient;

    long maxTimeoutSeconds = utComGetVar_ld(psShmHead, "MaxTimeoutSecs", MAX_TIMEOUT_SECONDS);
    ncApSrvOnline *psDevOnline;
    pasHashInfo     sHashInfo;
    ncDeptinfo      *psDept;
    uint4 lBase;
    uchar *pHash;
    while(1)
    {
        //查出离线场所并加入告警表
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
                    // printf("%s is offline\n", psClient[i].username);
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
                    // printf("%s is online\n", psClient[i].username);
                }
            }
        }
        //查出离线设备插入告警表
        lBase = utShmGetBaseAddr(psShmHead);
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
            psDevOnline = (ncApSrvOnline *)pasHashNextS(&sHashInfo);
        }
        sleep(60);
    }
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



int procapFrontpageSetFun(utShmHead * psShmHead)
{
    int iReturn;
    iReturn = pasSetTcpFunName("macFrontPageSearch", macFrontPageSearch, 0);
    iReturn = pasSetTcpFunName("macFrontPageLeftBlocks", macFrontPageLeftBlocks, 0);
    iReturn = pasSetTcpFunName("macFrontPageChart1", macFrontPageChart1, 0);
    iReturn = pasSetTcpFunName("macFrontPageRange", macFrontPageRange, 0);
    iReturn = pasSetTcpFunName("ncsTreeUser_v9", ncsTreeUser_v9, 0);
    iReturn = pasSetTcpFunName("ncsWebClientAdd_wif_v9", ncsWebClientAdd_wif_v9, 0);
    return 0;
}


