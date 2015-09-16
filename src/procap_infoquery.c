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

} stCurInfoForty;

//ncsCurrent_FromRAM  插入记录到结构体数组中
static int InsertItemToStuct(stCurInfoForty* pStCurrentForty, int iNum, ncApSrvOnline* psAp, ncLbsMacLocation *psMacLoc)
{
    char caMacStr[20] = "", caMacStr1[20] = "";
    if(psAp && psMacLoc)
    {
        snprintf(pStCurrentForty[iNum].caServicename, sizeof(pStCurrentForty[iNum].caServicename) - 1, "%s", psAp->servicename);
        snprintf(pStCurrentForty[iNum].caDispname, sizeof(pStCurrentForty[iNum].caDispname) - 1, "%s", psAp->dispname);

        pStCurrentForty[iNum].caStime = psMacLoc->starttime;

        pStCurrentForty[iNum].lrssi = psMacLoc->lrssi;
        pStCurrentForty[iNum].urssi = psMacLoc->urssi;

        snprintf(pStCurrentForty[iNum].caMac, sizeof(pStCurrentForty[iNum].caMac) - 1, "%s", (const char*)pasCvtMacStr(psMacLoc->mac, caMacStr));
        snprintf(pStCurrentForty[iNum].caLongitude, sizeof(pStCurrentForty[iNum].caLongitude) - 1, "%s", psMacLoc->longitude);
        snprintf(pStCurrentForty[iNum].caLatitude, sizeof(pStCurrentForty[iNum].caLatitude) - 1, "%s", psMacLoc->latitude);
        snprintf(pStCurrentForty[iNum].caSsid, sizeof(pStCurrentForty[iNum].caSsid) - 1, "%s", psMacLoc->ssid);
        snprintf(pStCurrentForty[iNum].caApmac, sizeof(pStCurrentForty[iNum].caApmac) - 1, "%s", (const char*)pasCvtMacStr(psMacLoc->apmac, caMacStr1));

        pStCurrentForty[iNum].encrypt = psMacLoc->encrypt;
        snprintf(pStCurrentForty[iNum].caServiceCode, sizeof(pStCurrentForty[iNum].caServiceCode) - 1, "%s", psAp->servicecode);

#ifdef  INFOQUERY_DEBUG
        //输出测试
        /*printf("\n\n InsertItemToStuct pStCurrentForty[%d].caServicename) = %s\n", iNum, pStCurrentForty[iNum].caServicename);
        printf("InsertItemToStuct pStCurrentForty[%d].caMac) = %s\n", iNum, pStCurrentForty[iNum].caMac);
        printf("InsertItemToStuct pStCurrentForty[%d].caApmac) = %s\n", iNum, pStCurrentForty[iNum].caApmac);*/
#endif

        snprintf(pStCurrentForty[iNum].caApname, sizeof(pStCurrentForty[iNum].caApname) - 1, "%s", psAp->apname);
        snprintf(pStCurrentForty[iNum].caVname, sizeof(pStCurrentForty[iNum].caVname) - 1, "%s", psMacLoc->name);
        snprintf(pStCurrentForty[iNum].caTermtype, sizeof(pStCurrentForty[iNum].caTermtype) - 1, "%s", psMacLoc->termtype);
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
    int i, j, k;
    stCurInfoForty stTemp;


    for(i = 0; i < iLen - 1; i++)
    {
        k = i; //给记号赋值
        for(j = i + 1; j < iLen; j++)
        {
            if(pstrCur[k].caStime > pstrCur[j].caStime)
                k = j; //是k总是指向最小元素

            if(i != k)
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
        utPltPutLoopVarF(psDbHead, "servicename", i + 1, "%s", pStCurrentForty[i].caServicename);
        utPltPutLoopVarF(psDbHead, "dispname", i + 1, "%s", pStCurrentForty[i].caDispname);
        utPltPutLoopVarF(psDbHead, "stime", i + 1, "%s", utTimFormat("%Y-%m-%d %H:%M:%S", pStCurrentForty[i].caStime));

        //输出测试

#ifdef  INFOQUERY_DEBUG
        /*printf("\n\n outPutJson_current pStCurrentForty[%d].caServicename) = %s\n", i, pStCurrentForty[i].caServicename);
        printf("outPutJson_current pStCurrentForty[%d].caMac) = %s\n", i, pStCurrentForty[i].caMac);
        printf("outPutJson_current pStCurrentForty[%d].caApmac) = %s\n", i, pStCurrentForty[i].caApmac);*/
#endif


        utPltPutLoopVarF(psDbHead, "lssi", i + 1, "%5d", pStCurrentForty[i].lrssi);
        utPltPutLoopVarF(psDbHead, "ussi", i + 1, "%5d", pStCurrentForty[i].urssi);
        utPltPutLoopVarF(psDbHead, "mac", i + 1, "%s", pStCurrentForty[i].caMac);
        utPltPutLoopVarF(psDbHead, "longitude", i + 1, "%s", pStCurrentForty[i].caLongitude);
        utPltPutLoopVarF(psDbHead, "latitude", i + 1, "%s", pStCurrentForty[i].caLatitude);
        utPltPutLoopVarF(psDbHead, "ssid", i + 1, "%s", pStCurrentForty[i].caSsid);
        utPltPutLoopVarF(psDbHead, "apmac", i + 1, "%s", pStCurrentForty[i].caApmac);
        utPltPutLoopVarF(psDbHead, "security", i + 1, "%d", pStCurrentForty[i].encrypt);
        utPltPutLoopVarF(psDbHead, "servicecode", i + 1, "%s", pStCurrentForty[i].caServiceCode);
        utPltPutLoopVarF(psDbHead, "apname", i + 1, "%s", pStCurrentForty[i].caApname);
        utPltPutLoopVarF(psDbHead, "vname", i + 1, "%s", pStCurrentForty[i].caVname);
        utPltPutLoopVarF(psDbHead, "termtype", i + 1, "%s", pStCurrentForty[i].caTermtype);

        return 0;
    }

    return -1;
}

//通过 GroupId 获取其所属的 groupid
static char *myGetGroupCodeByGroupid(utShmHead *psShmHead, char *caGroupid)
{
    static char caReturn[8024];
    char caServicecode[32];
    char caGroupcode[30];
    char caIds[1024];
    char caTempid[1024], caTempcode[1024];
    int iReturn, lId, iNum;
    pasDbCursor *psCur;
    memset(caReturn, 0, sizeof(caReturn));

    iNum = 0;

    psCur = pasDbOpenSqlF("select distinct groupcode from ncsgroup where   groupid in(%s) ", caGroupid);
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
            if(iNum == 0)
            {
                sprintf(caReturn, "\"%s\"", caGroupcode);
            }
            else
            {
                sprintf(caReturn + strlen(caReturn), ",\"%s\"", caGroupcode);
            }
            iNum++;

            memset(caGroupcode, 0, sizeof(caGroupcode));
            iReturn = pasDbFetchInto(psCur,
                                     UT_TYPE_STRING, 14, caGroupcode);
        }
        pasDbCloseCursor(psCur);
    }
    //printf("*************************************************************\n");
    //printf("caReturn = %s\n\n", caReturn);

    //       sprintf(caReturn,"%s",caOpt);
    sprintf(caIds, "%s", caGroupid);
    iNum = 0;
    while(strlen(caIds) > 0)
    {
        memset(caTempid, 0, sizeof(caTempid));
        iNum = 0;

        //printf("caIds = %s\n\n", caIds);

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
                if(iNum == 0)
                {
                    sprintf(caTempid, "%lu", lId);
                    sprintf(caTempcode, "\"%s\"", caGroupcode);
                }
                else
                {
                    sprintf(caTempid + strlen(caTempid), ",%lu", lId);
                    sprintf(caTempcode + strlen(caTempcode), ",\"%s\"", caGroupcode);
                }

                memset(caGroupcode, 0, sizeof(caGroupcode));
                iReturn = pasDbFetchInto(psCur, UT_TYPE_ULONG, 4, &lId,
                                         UT_TYPE_STRING, 14, caGroupcode);

                iNum++;

            }

        }
        pasDbCloseCursor(psCur);

        strcpy(caIds, caTempid);

        //printf("caTempcode = %s\n\n", caTempcode);
        if(strlen(caTempcode) > 0)
        {
            sprintf(caReturn + strlen(caReturn), ",%s", caTempcode);
        }
    }

    //if(strlen(caTempcode)>0){
    //         sprintf(caReturn+strlen(caReturn),",%s",caTempcode);
    // }

    //printf("caReturn = %s\n\n", caReturn);
    //printf("*************************************************************\n");
    return &caReturn[0];



}


/* 功能封装：将提取日期格式中的年、月 */
static int timeToStringPlace(time_t now, char *year, char *mon)
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


/* (暂未调用 截止2015/9/6 9:21)获取当前用户选择场所的 caSelectGroups ，
             在其权限范围内的 caAuthorityGroups 所有的 outbuf(servicecodes) */
static int selectGroupsIsInAuthorityGroups(char *caSelectGroups, char *caAuthorityGroups, char* outbuf, int buflen)
{
    if(!strlen(outbuf))
    {
        memcpy(outbuf, "当前用户无权限查看所选择区域", strlen("当前用户无权限查看所选择区域"));
        return -1;
    }

    //获取有权限所有场所 servicecode
    int len = strlen(caAuthorityGroups) + 1;
    char* pos1 = caAuthorityGroups;
    char* pos2 = NULL;
    int   len1 = 0, len2 = 0;
    char  idbuf[24] = "";
    memset(outbuf, 0, buflen);

    pos2 = strchr(pos1, ',');               //获取第一个','的位置
    while(pos2 != NULL)
    {
        len1 = pos2 - pos1;                 //获取第一个','之前字符串长度
        if(len1 > 20)
            break;

        len2 = strlen(outbuf);              //定位outbuf输出起点
        if(len2 > (len - 20))
            break;

        memset(idbuf, 0, sizeof(idbuf));
        memcpy(idbuf, pos1, len1);          //拷贝当前 pos1 ',' 号之前
        if(strstr(caSelectGroups, idbuf) != NULL) //比对当前 pos1 在 caSelectGroups 中出现的位置
        {
            if(outbuf[0])
            {
                sprintf(outbuf + strlen(outbuf), ",");
                len2 = strlen(outbuf);
                memcpy(outbuf + len2, idbuf, strlen(idbuf));
            }
            else
            {
                memcpy(outbuf + len2, idbuf, strlen(idbuf));
            }
        }
        pos1 = pos2 + 1;
        pos2 = strchr(pos1, ',');
    }
    if(strlen(pos1) == 16)
    {
        memset(idbuf, 0, sizeof(idbuf));
        memcpy(idbuf, pos1, 16);
        if(strstr(caSelectGroups, idbuf) != NULL)
        {
            sprintf(outbuf + strlen(outbuf), ",");
            len2 = strlen(outbuf);
            memcpy(outbuf + len2, idbuf, strlen(idbuf));
        }
    }
    if(!strlen(outbuf))
    {
        memcpy(outbuf, "当前用户无权限查看所选择区域", strlen("当前用户无权限查看所选择区域"));
        return -1;
    }

    return 0;
}


/*
    ncsMacLog_mactermlist_v4 mac日志高级查询功能：根据安全厂商名称获取 servicecodes
    流程： ncsfactorycod 表中：caSname -> code;
          ncsfactorycod.code = ncaplist.fcode; 获取 ncaplist.servicecode;
*/
static int getServicecodeBycaSname(utShmHead *psShmHead, char *caSname, char *outbuf, int buflen)
{
    if(!psShmHead && !caSname && !outbuf && !buflen)
    {
        memcpy(outbuf, "当前传入参数有误", strlen("当前传入参数有误"));
        return -1;
    }

    //1.根据 ncsfactorycod.name 查询出 ncsfactorycod.code;
    //2.根据 ncaplist.fcode in ncsfactorycod.code 查询出 ncaplist.servicecode 并返回;
    char caSql[1024] = "";
    snprintf(caSql, sizeof(caSql) - 1, "select distinct(b.servicecode) from ncaplist b where b.fcode in (select a.code from ncsfactorycod a where a.name like '%c%s%c') ", '%', caSname, '%');
    //printf(" 安全厂商名称：%s   caSql = %s\n\n ", caSname, caSql);

    memset(outbuf, 0, buflen);

    int iReturn = 0;
    int iNum = 0;
    pasDbCursor *psCur;
    psCur = pasDbOpenSqlF(caSql);
    if(psCur)
    {
        int nRet = 0;
        char caServiceCode[16] = "";

        nRet = pasDbFetchInto(psCur, UT_TYPE_STRING, 15, &caServiceCode);
        while(0 == nRet || 1405 == nRet)
        {
            if(0 == iNum)
            {
                //snprintf(outbuf, sizeof(outbuf) - 1, "\"%s\"", caServiceCode);
                sprintf(outbuf, "\"%s\"", caServiceCode);
            }
            else
            {
                //snprintf(outbuf + strlen(outbuf), sizeof(outbuf) - 1, ",\"%s\"", caServiceCode);
                sprintf(outbuf + strlen(outbuf), ",\"%s\"", caServiceCode);
            }
            iNum++;

            memset(caServiceCode, 0, sizeof(caServiceCode));
            nRet = pasDbFetchInto(psCur, UT_TYPE_STRING, 15, caServiceCode);
        }

        pasDbCloseCursor(psCur);
    }
    //printf(" outbuf = %s\n\n", outbuf);
    return 0;
}

/*
    ncsMacLog_mactermlist_v4 mac日志查询调用功能：根据前台传来的时间标志、
*/
//static int selectMacInfoByTimeAndPage()

static char **ncsUtlGetTable3(char *sdate, char *edate, char *stime1, char *etime1, char *prefix, unsigned long *lStartTime, unsigned long *lEndTime, int *lCount)
{
    char stime[15] = "";
    char etime[15] = "";

    snprintf(stime, sizeof(stime), "%c%c%c%c%c%c%c%c%c%c%c%c%c%c", sdate[0], sdate[1], sdate[2], sdate[3], sdate[5], sdate[6], sdate[8], sdate[9], stime1[0], stime1[1], stime1[3], stime1[4], stime1[6], stime1[7]);
    snprintf(etime, sizeof(stime), "%c%c%c%c%c%c%c%c%c%c%c%c%c%c", edate[0], edate[1], edate[2], edate[3], edate[5], edate[6], edate[8], edate[9], etime1[0], etime1[1], etime1[3], etime1[4], etime1[6], etime1[7]);
    *lStartTime = utTimStrToLong("%Y%m%d%H%M%S", stime);

    unsigned long lTime = utTimStrToLong("%Y%m%d%H%M%S", etime);
    *lEndTime = lTime;

    unsigned long lStartTime1 = *lStartTime;

    int i = 0;

    char minYear[5] = "";
    char maxYear[5] = "";
    char minMon[3] = "";
    char maxMon[3] = "";

    strcpy(minYear, utTimFormat("%Y", lStartTime1));
    strcpy(maxYear, utTimFormat("%Y", lTime));

    strcpy(minMon, utTimFormat("%m", lStartTime1));
    strcpy(maxMon, utTimFormat("%m", lTime));

    int iMin = atoi(minYear) * 12 + atoi(minMon);
    int iMax = atoi(maxYear) * 12 + atoi(maxMon);
    int strLen = strlen(prefix) + 7;

    *lCount = (iMax - iMin) + 1;

    int a;
    int b;
    char **tmp = (char **)malloc(sizeof(char *) * ((iMax - iMin) + 1));
    if(tmp != NULL)
    {
        int j = 0;
        for(i = iMin; i <= iMax; i++)
        {
            tmp[j] = (char *)malloc(strLen);
            if(tmp[j] != NULL)
            {
                a = i / 12;
                b = i % 12;
                if(b == 0)
                {
                    a--;
                    b = 12;
                }
                snprintf(tmp[j], strLen, "%s%04d%02d", prefix, a, b);
            }
            j++;
        }
    }
    return tmp;
}
static char **ncsUtlGetTable(unsigned long lTime, int days, char *prefix, unsigned long *lStartTime, int *lCount)
{
    char sdate[33] = "";
    strcpy(sdate, utTimFormat("%Y-%m-%d", lTime));//当前日期
    strcat(sdate, " 00:00:00");
    *lStartTime = utTimStrToLong("%Y/%m/%d %H:%M:%S", sdate) - (days - 1) * 86400;

    unsigned long lStartTime1 = *lStartTime;

    int i = 0;

    char minYear[5] = "";
    char maxYear[5] = "";
    char minMon[3] = "";
    char maxMon[3] = "";

    strcpy(minYear, utTimFormat("%Y", lStartTime1));
    strcpy(maxYear, utTimFormat("%Y", lTime));

    strcpy(minMon, utTimFormat("%m", lStartTime1));
    strcpy(maxMon, utTimFormat("%m", lTime));

    int iMin = atoi(minYear) * 12 + atoi(minMon);
    int iMax = atoi(maxYear) * 12 + atoi(maxMon);
    int strLen = strlen(prefix) + 7;

    *lCount = (iMax - iMin) + 1;

    int a;
    int b;
    char **tmp = (char **)malloc(sizeof(char *) * ((iMax - iMin) + 1));
    if(tmp != NULL)
    {
        int j = 0;
        for(i = iMin; i <= iMax; i++)
        {
            tmp[j] = (char *)malloc(strLen);
            if(tmp[j] != NULL)
            {
                a = i / 12;
                b = i % 12;
                if(b == 0)
                {
                    a--;
                    b = 12;
                }
                snprintf(tmp[j], strLen, "%s%04d%02d", prefix, a, b);
            }
            j++;
        }
    }
    return tmp;
}
static char *getDsGroupid()
{
    static char caReturn[1024];
    char caIds[1024], caTempid[1024];
    char caOpt[256];
    int iReturn, lId, iNum;
    pasDbCursor *psCur;
    memset(caReturn, 0, sizeof(caReturn));
    lId = 0;

    iReturn = dsCltGetMyInfo(1, "Userid", &lId);
    printf("dsiReturn=%d\n", iReturn);
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
        printf("select groupid from ncsgroup where pid in(%s)\n ", caIds);
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
//根据厂商权限,获取单位组ID组合
static char *getDsFacGroupid()
{
    static char caReturn[1024];
    char caIds[1024], caTempid[1024];
    char caOpt[256];
    int iReturn, lId, iNum;
    pasDbCursor *psCur;
    char caObj[100][50];
    long i;
    long num = 0;
    memset(caReturn, 0, sizeof(caReturn));


    iReturn = dsCltGetMyInfo(1, "Userid", &lId);
    if(iReturn != 0 || lId <= 0)
    {
        return &caReturn[0];
    }

    memset(caOpt, 0, sizeof(caOpt));
    pasDbOneRecord("select fac from dsuser where id=:id", 1, "id", UT_TYPE_LONG, lId, UT_TYPE_STRING, 255, caOpt);
    if(strstr(caOpt, "all") || strlen(caOpt) == 0)
    {
        return &caReturn[0];
    }
    num = ncs_SepChar0(caOpt, ',', caObj);
    for(i = 0; i < num; i++)
    {
        if(i == 0)
            sprintf(caReturn, "\'%s\'", caObj[0]);
        else
        {
            sprintf(caReturn + strlen(caReturn), ",\'%s\'", caObj[i]);
        }
    }

    return &caReturn[0];
}

static char *getGroupcodeSql(char *field, char *caGroupid)
{
    static char caReturn[1024];
    char caIds[1024], caTempid[1024];
    char caGroupcode[20];
    char caTempcode[1024];
    char caOpt[256];
    pasLHashInfo sHashInfo;
    int iReturn, lId, iNum;
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





    lId = 0;



    pHash = (unsigned char *)pasLHashInit(100, 100, sizeof(struct ncsDsData_s), 0, 4);
    if(pHash == NULL)
    {
        return &caReturn[0];
    }


    psCur = pasDbOpenSqlF("select groupcode from ncsgroup where   groupid in(%s) ", caGroupid);
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
    sprintf(caIds, "%s", caGroupid);
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
/* MAC日志查询：显示终端特征信息日志 */
int ncsMacLog_mactermlist_v4(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead)
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
    long lCurPg, lRowNum, lStartRec, lSstarttime;
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
    char caPhoneNumber[64] = "";
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
    iReturn = utMsgGetSomeNVar(psMsgHead, 26,
                               "TotPg",      UT_TYPE_STRING, 12, caTotPg,
                               "CurPg",      UT_TYPE_STRING, 12, caCurPg,
                               "keyword", UT_TYPE_STRING, 60, caKeyword,
                               "page",            UT_TYPE_STRING, 10, caPage,
                               "limit",           UT_TYPE_STRING, 10, caLimit,
                               "sort",            UT_TYPE_STRING, 15, caSort,
                               "dir",          UT_TYPE_STRING, 8,  caDir,
                               "time_flag",    UT_TYPE_STRING, 8, caTime_flag,
                               "did",          UT_TYPE_STRING, 10, caDid,
                               "groupid",      UT_TYPE_STRING, 10, caGroupid,
                               "username",      UT_TYPE_STRING, 63, caUserName,
                               "sdate",        UT_TYPE_STRING, 12, sdate,
                               "stime",        UT_TYPE_STRING, 10, caStime,
                               "edate",        UT_TYPE_STRING, 12, edate,
                               "etime",        UT_TYPE_STRING, 10, caEtime,
                               "apmac",        UT_TYPE_STRING, 20, caApmac,
                               "mac",          UT_TYPE_STRING, 20, caMac,
                               "macLogExport",      UT_TYPE_STRING, 9, caExport,
                               "searchtype",      UT_TYPE_STRING, 10, caSearchtype,
                               "servicecode",     UT_TYPE_STRING, 15, caServicecode,
                               "servicename",     UT_TYPE_STRING, 60, caCorpname,
                               "sname",           UT_TYPE_STRING, 31, caSname,
                               "ssid",            UT_TYPE_STRING, 30, caSsid,
                               "apname",       UT_TYPE_STRING, 31, caApname,
                               "vname",      UT_TYPE_STRING, 63, caVname,
                               "phonenumber",      UT_TYPE_STRING, 63, caPhoneNumber);

    memcpy(caMacKeyword, caKeyword, sizeof(caKeyword));
    MyremoveAll(caMacKeyword, '-');
    MyremoveAll(caMacKeyword, ':');
    MyremoveAll(caMac, '-');
    MyremoveAll(caMac, ':');
    MyremoveAll(caApmac, '-');
    MyremoveAll(caApmac, ':');
    MyremoveAll(caKeyword, '-');
    MyremoveAll(caKeyword, ':');

    if(strlen(caSearchtype) > 0)
    {
        strcpy(caKeyword, "");
    }
    else
    {
        strcpy(sdate, "");
        strcpy(edate, "");
    }
    memset(caCont, 0, sizeof(caCont));


    if(strlen(caKeyword) > 0)
    {
        strcpy(caTemp_d, pasCvtGBK(2, caKeyword, caTemp, 60));
        strcpy(caKeyword, caTemp_d);
    }
    if(strlen(caCorpname) > 0)
    {
        strcpy(caTemp_d, pasCvtGBK(2, caCorpname, caTemp, 60));
        strcpy(caCorpname, caTemp_d);
    }
    if(strlen(caApname) > 0)
    {
        strcpy(caTemp_d, pasCvtGBK(2, caApname, caTemp, 60));
        strcpy(caApname, caTemp_d);
    }
    if(strlen(caSsid) > 0)
    {
        strcpy(caTemp_d, pasCvtGBK(2, caSsid, caTemp, 60));
        strcpy(caSsid, caTemp_d);
    }

    lCurPg = atol(caPage);
    if(lCurPg <= 0)
        lCurPg = 1;

    if(strlen(caLimit) > 0)
    {
        lRowNum = atol(caLimit);
    }
    else
    {
        lRowNum = 40; //每一页面行数
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
        case 6://最近半年：即查询全部
            days = 180;
            break;
        default:
            days = 1;
            break;
    }

    if(strlen(sdate) > 0 && strlen(edate) > 0)
    {
        ptmp = ncsUtlGetTable3(sdate, edate, caStime, caEtime, "ncmactermatt_if_", &lStartTime, &lTime, &lCount);
        printf("lStartTime=%d,sdate=%s\n", lStartTime, sdate);
    }
    else
    {
        ptmp = ncsUtlGetTable(lTime, days, "ncmactermatt_if_",  &lStartTime, &lCount);
        lTime = lTime + 2 * 3600;
    }

    memset(caCont, 0, sizeof(caCont));


    char caGroupidJudge[30] = "";   //判断输入框内容有无@ 字符
    char caAdvancedPlace[4096] = "";    //保存高级查询中所输入安全厂商名称下的场所编码;
    if(strlen(caSearchtype) > 0)
    {
        if(strlen(caMac) > 0)
        {
            sprintf(caCont + strlen(caCont), " and replace(a.mac,'-','') like '%c%s%c' ", '%', caMac, '%');
        }
        if(strlen(caApmac) > 0)
        {
            sprintf(caCont + strlen(caCont), " and replace(a.apmac, '-','') like '%c%s%c' ", '%', caApmac, '%');
        }

        /* 高级查询搜索字段 */
        if(strlen(caServicecode) > 0) //场所编号
        {
            sprintf(caCont + strlen(caCont), " and a.servicecode like '%c%s%c' ", '%', caServicecode, '%');
        }
        if(strlen(caCorpname) > 0)  //场所名称
        {
            sprintf(caCont + strlen(caCont), " and a.servicename like '%c%s%c' ", '%', caCorpname, '%');
        }

        if(strlen(caSname) > 0)     //安全厂商名称查询
        {
            char caTemp_name[32] = "";
            char caTemp_name2[1024] = "";
            strcpy(caTemp_name, pasCvtGBK(2, caSname, caTemp_name2, 60));
            strcpy(caSname, caTemp_name);

            int* pCount = NULL;

            //通过安全厂商名称：获取其对应 servicecodes
            int iRet = getServicecodeBycaSname(psShmHead, caSname, caAdvancedPlace, 4096);

            printf("caSname = %s 下的场所编码为 %s \n\n", caSname, caAdvancedPlace);
            sprintf(caCont + strlen(caCont), " and a.servicecode in (%s) ", caAdvancedPlace);

        }
        if(strlen(caSsid) > 0)      //接入热点ssid
        {
            sprintf(caCont + strlen(caCont), " and a.ssid like '%c%s%c' ", '%', caSsid, '%');
        }
        if(strlen(caApname) > 0)    //采集设备编号
        {
            sprintf(caCont + strlen(caCont), " and a.apname like '%c%s%c' ", '%', caApname, '%');
        }
        if(strlen(caVname) > 0)     //上网认证账号
        {
            sprintf(caCont + strlen(caCont), " and a.vname like '%c%s%c' ", '%', caVname, '%');
        }
    }
    else
    {
        if(strlen(caPhoneNumber) > 0)
        {
            //手机号搜索：1.嗅探前台页面全局搜索框中使用; 2.高级搜索条件;
            if(strlen(caPhoneNumber) > 0)
            {
                sprintf(caCont + strlen(caCont), " and (a.vname like '%c%s%c' ) ", '%', caPhoneNumber, '%');
                printf("caPhoneNumber = %s\n\n", caPhoneNumber);

            }
        }
        if(strlen(caKeyword) > 0)
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
                if(caKeyword[i] >= 'a' && caKeyword[i] <= 'z')
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
                    if(strlen(caGroupid) > 0)
                    {
                        sprintf(caCont + strlen(caCont), " and %s", getGroupcodeSql("servicecode", caGroupid));
                    }
                    else
                    {
                        if(strlen(caUserName) > 0)
                        {
                            //printf("In caKeyword @ caUserName = %s\n", caUserName);

                            //通过 caUserId 去获取相对应 lGroupId
                            sprintf(caCont + strlen(caCont), " and a.servicecode = \'%s\'", caUserName);

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
                        sprintf(caCont + strlen(caCont), " and (replace(a.mac,'-','') like '%c%s%c' or replace(a.apmac,'-','') like '%c%s%c') ", '%', pstrKeywordMac, '%', '%', pstrKeywordMac, '%');
                    //sprintf(caCont+strlen(caCont)," and (a.mac like '%c%s%c' or a.apmac   like '%c%s%c') ",'%',pstrKeywordMac,'%','%',pstrKeywordMac,'%');

                }
                else
                {

                    sprintf(caCont + strlen(caCont), " and (replace(a.mac,'-','') like '%c%s%c' or replace(a.apmac,'-','') like '%c%s%c' or a.vname like '%c%s%c' ) ", '%', caKeyword, '%', '%', caKeyword, '%', '%', caKeyword, '%');
                }
            }

            printf("caCont = %s\n", caCont);
        }

    }



    /*  当前用户权限下所能查询场所 */
    if(strlen(caGroups) > 0)
    {
        sprintf(caCont + strlen(caCont), " and %s ", getDsGroupcodeSql("servicecode"));
    }


    if(!strlen(caKeyword) && !strlen(caPhoneNumber) && (1 == days || 6 == days))
    {
        //获取去重后mac数
        long lTodayTermMac = 0;
        long lTotalTermMac = 0;
        getMacCount(psShmHead, &lTodayTermMac, &lTotalTermMac);
        printf("今日新增去重mac数 = %lu, mac库中所有去重mac数  = %lu\n\n", lTodayTermMac, lTotalTermMac);
        if(1 == days)
            utPltPutVarF(psDbHead, "distinctCount", "%lu", lTodayTermMac);
        else
            utPltPutVarF(psDbHead, "distinctCount", "%lu", lTotalTermMac);
    }
    else
    {
        char sql3[4096] = "";
        unsigned long lCount2 = 0;
        unsigned long lTotRec2 = 0;
        int num2 = 0;

        /* 去重后查询结果 */
        for(i = 0; i < lCount; i++)
        {
            lCount2 = 0;
            memset(sql3, 0, sizeof(sql3));
            snprintf(sql3, sizeof(sql3), "select count(distinct a.mac) from %s a where a.stime >= %lu and a.stime <= %lu",  ptmp[i], lStartTime, lTime);
            strcat(sql3, caCont);
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
        utPltPutVarF(psDbHead, "distinctCount", "%lu", lTotRec2);
    }


    for(i = 0; i < lCount; i++)
    {
        lCount1 = 0;
        memset(sql, 0, sizeof(sql));
        snprintf(sql, sizeof(sql), "select count(*) from %s a where a.stime >= %lu and a.stime <= %lu",  ptmp[i], lStartTime, lTime);
        strcat(sql, caCont);

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
    utPltPutVarF(psDbHead, "TotRec", "%lu", lTotRec);

    for(i = num - 1; i >= 0; i--)
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
        sprintf(caFilename, "maclog.csv");
        sprintf(caPath, "%s", "/home/ncmysql/ncs");
        sprintf(caFile, "%s/%s", caPath, caFilename);
        fp = fopen(caFile, "w");
        if(fp == NULL)
        {
            printf("数据导出出错\n\n");
            return 0;
        }
        fprintf(fp, "mac日志查询\n");   //标题
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

    for(i = num - 1; i >= 0; i--)
    {
        memset(sql, 0, sizeof(sql));
        if(tableinfo[i].lCount <= iStart)
        {
            iStart -= tableinfo[i].lCount;
            continue;
        }
        /* 加上采集设备名称 */
        //snprintf(sql, sizeof(sql), "select a.servicecode,a.servicename,a.address,a.mac,a.apname,a.apmac,a.ssid,a.stime,a.channel,a.rssi,a.security,a.longitude,a.latitude,a.termtype,a.vtype,a.vname, b.dispname from %s a, ncaplist b where a.stime >= %lu and a.stime <= %lu and b.apname = RIGHT(a.apname, 12) ", tableinfo[i].tname, lStartTime, lTime);

        /* 去掉采集设备名称关联查询 */
        snprintf(sql, sizeof(sql), "select a.servicecode,a.servicename,a.address,a.mac,a.apname,a.apmac,a.ssid,a.stime,a.channel,a.rssi,a.security,a.longitude,a.latitude,a.termtype,a.vtype,a.vname from %s a where a.stime >= %lu and a.stime <= %lu ", tableinfo[i].tname, lStartTime, lTime);
        strcat(sql, caCont);

        //printf("\n\n sql = %s \n\n caCont = %s\n\n", sql, caCont);

        snprintf(sql + strlen(sql), sizeof(sql) - strlen(sql), " order by a.stime desc limit %lu, %lu", iStart, iLimit < tableinfo[i].lCount ? iLimit : tableinfo[i].lCount);

        if(tableinfo[i].lCount > iStart && tableinfo[i].lCount < iStart + iLimit)
        {
            iLimit = iLimit - (tableinfo[i].lCount - iStart);
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
            char caServicecode[16], caServicename[64], caAddress[64], caMac[20], caApname[32], caApmac[20], caSsid[64];
            long lStarttime, lEndtime, lFlags;
            char caChannel[32], caRssi[16], caSecurity[16], caLongitude[16], caLatitude[16];
            char caTermtype[128], caVname[64];
            long lVtype;

            /* 采集设备名称 */
            char caDispname[32] = "";

            memset(caServicecode, 0, sizeof(caServicecode));
            memset(caServicename, 0, sizeof(caServicename));
            memset(caAddress, 0, sizeof(caAddress));
            memset(caMac, 0, sizeof(caMac));
            memset(caApname, 0, sizeof(caApname));
            memset(caApmac, 0, sizeof(caApmac));
            memset(caSsid, 0, sizeof(caSsid));
            lStarttime = 0;
            memset(caChannel, 0, sizeof(caChannel));
            memset(caRssi, 0, sizeof(caRssi));
            memset(caSecurity, 0, sizeof(caSecurity));
            memset(caLongitude, 0, sizeof(caLongitude));
            memset(caLatitude, 0, sizeof(caLatitude));
            memset(caTermtype, 0, sizeof(caTermtype));
            memset(caVname, 0, sizeof(caVname));

            lVtype = 0;
            iret = pasDbFetchInto(psCur,
                                  UT_TYPE_STRING, 14, caServicecode,
                                  UT_TYPE_STRING, 63, caServicename,
                                  UT_TYPE_STRING, 63, caAddress,
                                  UT_TYPE_STRING, 18, caMac,
                                  UT_TYPE_STRING, 31, caApname,
                                  UT_TYPE_STRING, 18, caApmac,
                                  UT_TYPE_STRING, 63, caSsid,
                                  UT_TYPE_LONG, 4, &lStarttime,
                                  UT_TYPE_STRING, 2, caChannel,
                                  UT_TYPE_STRING, 8, caRssi,
                                  UT_TYPE_STRING, 2, caSecurity,
                                  UT_TYPE_STRING, 11, caLongitude,
                                  UT_TYPE_STRING, 11, caLatitude,
                                  UT_TYPE_STRING, 100, caTermtype,
                                  UT_TYPE_LONG, 4, &lVtype,
                                  UT_TYPE_STRING, 63, caVname,
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
                        utPltPutLoopVar(psDbHead, "dh", iNum, ",");
                    }

                    utPltPutLoopVar(psDbHead, "stime", iNum, utTimFormat("%Y-%m-%d %H:%M:%S", lStarttime));

                    utPltPutLoopVar(psDbHead, "address", iNum, caAddress);
                    utPltPutLoopVar(psDbHead, "apname", iNum, caApname);
                    utPltPutLoopVar(psDbHead, "mac", iNum, caMac);
                    utPltPutLoopVar(psDbHead, "apmac", iNum, caApmac);
                    utPltPutLoopVar(psDbHead, "servicename", iNum, caServicename);
                    utPltPutLoopVar(psDbHead, "channel", iNum, caChannel);
                    utPltPutLoopVar(psDbHead, "rssi", iNum, caRssi);
                    //printf("caSecurity=%s\n",caSecurity);
                    utPltPutLoopVar(psDbHead, "security", iNum, caSecurity);
                    utPltPutLoopVar(psDbHead, "longitude", iNum, caLongitude);
                    utPltPutLoopVar(psDbHead, "latitude", iNum, caLatitude);
                    utPltPutLoopVar(psDbHead, "ssid", iNum, caSsid);
                    utPltPutLoopVar(psDbHead, "servicecode", iNum, caServicecode);
                    utPltPutLoopVar(psDbHead, "termtype", iNum, caTermtype);
                    utPltPutLoopVar(psDbHead, "vname", iNum, caVname);
                    utPltPutLoopVarF(psDbHead, "vtype", iNum, "%d", lVtype);
                    utPltPutLoopVarF(psDbHead, "dispname", iNum, "%s", caDispname);
                }

                memset(caServicecode, 0, sizeof(caServicecode));
                memset(caServicename, 0, sizeof(caServicename));
                memset(caAddress, 0, sizeof(caAddress));
                memset(caMac, 0, sizeof(caMac));
                memset(caApname, 0, sizeof(caApname));
                memset(caApmac, 0, sizeof(caApmac));
                memset(caSsid, 0, sizeof(caSsid));
                memset(caServicecode, 0, sizeof(caServicecode));
                memset(caServicename, 0, sizeof(caServicename));
                memset(caAddress, 0, sizeof(caAddress));
                memset(caMac, 0, sizeof(caMac));
                memset(caApname, 0, sizeof(caApname));
                memset(caApmac, 0, sizeof(caApmac));
                memset(caSsid, 0, sizeof(caSsid));
                lStarttime = 0;
                memset(caChannel, 0, sizeof(caChannel));
                memset(caRssi, 0, sizeof(caRssi));
                memset(caSecurity, 0, sizeof(caSecurity));
                memset(caLongitude, 0, sizeof(caLongitude));
                memset(caLatitude, 0, sizeof(caLatitude));
                memset(caTermtype, 0, sizeof(caTermtype));
                memset(caVname, 0, sizeof(caVname));
                memset(caDispname, 0, sizeof(caDispname));

                lVtype = 0;

                iret = pasDbFetchInto(psCur,
                                      UT_TYPE_STRING, 14, caServicecode,
                                      UT_TYPE_STRING, 63, caServicename,
                                      UT_TYPE_STRING, 63, caAddress,
                                      UT_TYPE_STRING, 18, caMac,
                                      UT_TYPE_STRING, 31, caApname,
                                      UT_TYPE_STRING, 18, caApmac,
                                      UT_TYPE_STRING, 63, caSsid,
                                      UT_TYPE_LONG, 4, &lStarttime,
                                      UT_TYPE_STRING, 2, caChannel,
                                      UT_TYPE_STRING, 8, caRssi,
                                      UT_TYPE_STRING, 2, caSecurity,
                                      UT_TYPE_STRING, 11, caLongitude,
                                      UT_TYPE_STRING, 11, caLatitude,
                                      UT_TYPE_STRING, 100, caTermtype,
                                      UT_TYPE_LONG, 4, &lVtype,
                                      UT_TYPE_STRING, 63, caVname,
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
        utPltFileDownload(iFd, "application/text", caPath, caFilename, caFilename);
        remove(caFile);
        return 0;
    }

    utPltOutToHtml(iFd, psMsgHead, psDbHead, "rzcx/proauth_hotinfo_list.htm");
    return 0;
}

/* MAC日志查询下特征分析1：频繁出现的时间 */
int ncsMacLog_ChosenMac_TimeOftenCount(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead)
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

    if(strcmp(caTimeFlag, "null") == 0)     //查询今天：默认
    {
        snprintf(sqlTmp, sizeof(sqlTmp), " UNIX_TIMESTAMP(DATE_SUB(NOW(), INTERVAL 1 DAY)) ");
    }
    else if(strcmp(caTimeFlag, "1") == 0)   //查询今天：用户选择
    {
        snprintf(sqlTmp, sizeof(sqlTmp), " UNIX_TIMESTAMP(DATE_SUB(NOW(), INTERVAL 1 DAY)) ");
    }
    else if(strcmp(caTimeFlag, "2") == 0)   //最近三天
    {
        snprintf(sqlTmp, sizeof(sqlTmp), " UNIX_TIMESTAMP(DATE_SUB(NOW(), INTERVAL 3 DAY)) ");
    }
    else if(strcmp(caTimeFlag, "3") == 0)   //最近一周
    {
        snprintf(sqlTmp, sizeof(sqlTmp), " UNIX_TIMESTAMP(DATE_SUB(NOW(), INTERVAL 7 DAY)) ");
    }
    else if(strcmp(caTimeFlag, "4") == 0)   //最近一月
    {
        snprintf(sqlTmp, sizeof(sqlTmp), " UNIX_TIMESTAMP(DATE_SUB(NOW(), INTERVAL 1 MONTH)) ");
    }
    else if(strcmp(caTimeFlag, "5") == 0)   //最近三月
    {
        snprintf(sqlTmp, sizeof(sqlTmp), " UNIX_TIMESTAMP(DATE_SUB(NOW(), INTERVAL 3 MONTH)) ");
    }
    else if(strcmp(caTimeFlag, "6") == 0)   //最近半年
    {
        snprintf(sqlTmp, sizeof(sqlTmp), " UNIX_TIMESTAMP(DATE_SUB(NOW(), INTERVAL 6 MONTH)) ");
    }
    else    //最近半年
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
                                          UT_TYPE_LONG,  20, &dwCount,
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
int ncsMacLog_ChosenMac_PlaceOftenCount(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead)
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

    if(strcmp(caTimeFlag, "null") == 0)     //查询今天：默认
    {
        snprintf(sqlTmp, sizeof(sqlTmp), " UNIX_TIMESTAMP(DATE_SUB(NOW(), INTERVAL 1 DAY)) ");
    }
    else if(strcmp(caTimeFlag, "1") == 0)   //查询今天：用户选择
    {
        snprintf(sqlTmp, sizeof(sqlTmp), " UNIX_TIMESTAMP(DATE_SUB(NOW(), INTERVAL 1 DAY)) ");
    }
    else if(strcmp(caTimeFlag, "2") == 0)   //最近三天
    {
        snprintf(sqlTmp, sizeof(sqlTmp), " UNIX_TIMESTAMP(DATE_SUB(NOW(), INTERVAL 3 DAY)) ");
    }
    else if(strcmp(caTimeFlag, "3") == 0)   //最近一周
    {
        snprintf(sqlTmp, sizeof(sqlTmp), " UNIX_TIMESTAMP(DATE_SUB(NOW(), INTERVAL 7 DAY)) ");
    }
    else if(strcmp(caTimeFlag, "4") == 0)   //最近一月
    {
        snprintf(sqlTmp, sizeof(sqlTmp), " UNIX_TIMESTAMP(DATE_SUB(NOW(), INTERVAL 1 MONTH)) ");
    }
    else if(strcmp(caTimeFlag, "5") == 0)   //最近三月
    {
        snprintf(sqlTmp, sizeof(sqlTmp), " UNIX_TIMESTAMP(DATE_SUB(NOW(), INTERVAL 3 MONTH)) ");
    }
    else if(strcmp(caTimeFlag, "6") == 0)   //最近半年
    {
        snprintf(sqlTmp, sizeof(sqlTmp), " UNIX_TIMESTAMP(DATE_SUB(NOW(), INTERVAL 6 MONTH)) ");
    }
    else    //最近半年
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
                                          UT_TYPE_LONG,  20, &dwCount)) || 1405 == nRet)
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
int ncsMacLog_ChosenMac_PlaceTypeOftenCount(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead)
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

    if(strcmp(caTimeFlag, "null") == 0)     //查询今天：默认
    {
        snprintf(sqlTmp, sizeof(sqlTmp), " UNIX_TIMESTAMP(DATE_SUB(NOW(), INTERVAL 1 DAY)) ");
    }
    else if(strcmp(caTimeFlag, "1") == 0)   //查询今天：用户选择
    {
        snprintf(sqlTmp, sizeof(sqlTmp), " UNIX_TIMESTAMP(DATE_SUB(NOW(), INTERVAL 1 DAY)) ");
    }
    else if(strcmp(caTimeFlag, "2") == 0)   //最近三天
    {
        snprintf(sqlTmp, sizeof(sqlTmp), " UNIX_TIMESTAMP(DATE_SUB(NOW(), INTERVAL 3 DAY)) ");
    }
    else if(strcmp(caTimeFlag, "3") == 0)   //最近一周
    {
        snprintf(sqlTmp, sizeof(sqlTmp), " UNIX_TIMESTAMP(DATE_SUB(NOW(), INTERVAL 7 DAY)) ");
    }
    else if(strcmp(caTimeFlag, "4") == 0)   //最近一月
    {
        snprintf(sqlTmp, sizeof(sqlTmp), " UNIX_TIMESTAMP(DATE_SUB(NOW(), INTERVAL 1 MONTH)) ");
    }
    else if(strcmp(caTimeFlag, "5") == 0)   //最近三月
    {
        snprintf(sqlTmp, sizeof(sqlTmp), " UNIX_TIMESTAMP(DATE_SUB(NOW(), INTERVAL 3 MONTH)) ");
    }
    else if(strcmp(caTimeFlag, "6") == 0)   //最近半年
    {
        snprintf(sqlTmp, sizeof(sqlTmp), " UNIX_TIMESTAMP(DATE_SUB(NOW(), INTERVAL 6 MONTH)) ");
    }
    else    //最近半年
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
        char caGtype[36] = "";      //场所类型
        unsigned long dwCount = 0;
        int nRet = 0;
        while(0 == (nRet = pasDbFetchInto(psCur, UT_TYPE_STRING, 32, &caGtype,
                                          UT_TYPE_LONG,  20, &dwCount)) || 1405 == nRet)
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
int ncsMacLog_ChosenMac_IDCheck(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead)
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
    printf("%s\n", sql);
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

/* MAC日志查询加入分组1：查询终端mac组信息、mac组描述 */
int ncsMacLog_ChosenMac_GetMacGroupInfo(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead)
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
    printf("%s\n", sql);
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

/* MAC日志查询加入分组2：将所选的mac加入相应mac组名及描述 */
int ncsMacLog_ChosenMac_JoinMacGroup(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead)
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
            utPltPutVar(psDbHead, "result", "false");
            utPltPutVar(psDbHead, "message", "加入分组失败");
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
        utPltPutVar(psDbHead, "result", "true");
        utPltPutVar(psDbHead, "message", "加入分组成功");
    }

    utPltOutToHtml(iFd, psMsgHead, psDbHead, "mac/infoquery/macterm/JoinGroup.htm");
    return 0;
}

/* MAC日志查询：list 添加 "历史ssid列表" 查询选中MAC的 ssid 去重列表 */
int ncsMacLog_ChosenMac_DistinctSsid(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead)
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

    if(strcmp(caTimeFlag, "null") == 0)     //查询今天：默认
    {
        snprintf(sqlTmp, sizeof(sqlTmp), " UNIX_TIMESTAMP(DATE_SUB(NOW(), INTERVAL 1 DAY)) ");
    }
    else if(strcmp(caTimeFlag, "1") == 0)   //查询今天：用户选择
    {
        snprintf(sqlTmp, sizeof(sqlTmp), " UNIX_TIMESTAMP(DATE_SUB(NOW(), INTERVAL 1 DAY)) ");
    }
    else if(strcmp(caTimeFlag, "2") == 0)   //最近三天
    {
        snprintf(sqlTmp, sizeof(sqlTmp), " UNIX_TIMESTAMP(DATE_SUB(NOW(), INTERVAL 3 DAY)) ");
    }
    else if(strcmp(caTimeFlag, "3") == 0)   //最近一周
    {
        snprintf(sqlTmp, sizeof(sqlTmp), " UNIX_TIMESTAMP(DATE_SUB(NOW(), INTERVAL 7 DAY)) ");
    }
    else if(strcmp(caTimeFlag, "4") == 0)   //最近一月
    {
        snprintf(sqlTmp, sizeof(sqlTmp), " UNIX_TIMESTAMP(DATE_SUB(NOW(), INTERVAL 1 MONTH)) ");
    }
    else if(strcmp(caTimeFlag, "5") == 0)   //最近三月
    {
        snprintf(sqlTmp, sizeof(sqlTmp), " UNIX_TIMESTAMP(DATE_SUB(NOW(), INTERVAL 3 MONTH)) ");
    }
    else if(strcmp(caTimeFlag, "6") == 0)   //最近半年
    {
        snprintf(sqlTmp, sizeof(sqlTmp), " UNIX_TIMESTAMP(DATE_SUB(NOW(), INTERVAL 6 MONTH)) ");
    }
    else    //最近半年
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
    utPltOutToHtml(iFd, psMsgHead, psDbHead, "mac/infoquery/macterm/DistinctSsid.htm");
    return 0;
}

/*
    MAC日志查询：list  "上网认证账号功能"，查询 vname 相关的mac下的name、vname
    日期：2015/09/11 钱智明
*/
int ncsMacLog_ChosenMac_NetAuthID(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead)
{
    /* 初始化模板数据缓冲区 */
    utPltDbHead *psDbHead = utPltInitDb();

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

    printf("mac = %s\n\n", caMac);

    char sql[1024] = "";
    //snprintf(sql, sizeof(sql), "select mac, vname, vdisp from nctermnetlog where mac = \'%s\' ", caMac);
    snprintf(sql , sizeof(sql), "select b.mac, a.name, b.vname from ncsnettype a, nctermnetlog b where b.mac = \'%s\' and a.ntype = b.nettype ", caMac);

    printf("sql = %s\n\n", sql);

    int iNum = 0, iRet = 0;
    pasDbCursor *psCur = NULL;
    psCur = pasDbOpenSql(sql, 0);
    if(psCur != NULL)
    {
        printf("\n psCur is not null\n\n");
        char caTempMac[36] = "";
        char caName[36] = "";
        char caVname[36] = "";
        int nRet = 0;
        while(0 == (nRet = pasDbFetchInto(psCur, UT_TYPE_STRING, 35, &caTempMac,
                                          UT_TYPE_STRING, 35, &caName,
                                          UT_TYPE_STRING, 35, &caVname)) || 1405 == nRet)
        {
            iNum++;
            if(iNum > 1)
            {
                utPltPutLoopVar(psDbHead, "dh", iNum, ",");
            }
            utPltPutLoopVarF(psDbHead, "mac", iNum, "%s", caTempMac);
            utPltPutLoopVarF(psDbHead, "name", iNum, "%s", caName);
            utPltPutLoopVarF(psDbHead, "vname", iNum, "%s", caVname);

            //清空容器
            memset(caTempMac, 0, sizeof(caTempMac));
            memset(caName, 0, sizeof(caName));
            memset(caVname, 0, sizeof(caVname));
        }
    }

    pasDbCloseCursor(psCur);


    utPltOutToHtml(iFd, psMsgHead, psDbHead, "mac/infoquery/macterm/NetAuthID.html");
    return 0;
}




/*
    MAC日志查询：工具条 button 选择场所，返回前台树状菜单
    日期：2015/09/13 钱智明
*/
int ncsMacLog_ChosenMac_ChoseQueryPlace(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead)
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

#ifdef LDEBUG1
    utMsgPrintMsg(psMsgHead);
#endif
    utMsgPrintMsg(psMsgHead);
    strcpy(caGroups, getDsGroupid());
    strcpy(caFacs, getDsFacGroupid());
    printf("caFacs=%s\n", caFacs);
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
        sprintf(caTemp, "select groupname,groupid,pid from ncsgroup where 1=1 ");
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
        //      if(strlen(caFacs)>0){
        //          sprintf(caTemp+strlen(caTemp)," and groupid in (%s) ",caFacs);
        //      }
        sprintf(caTemp + strlen(caTemp), " and pid=%d ", atol(caPid));
        sprintf(caTemp + strlen(caTemp), " order by groupname limit %lu,%lu ", 0, lRowNum);
        printf("caTemp=%s\n", caTemp);
        psCur = pasDbOpenSql(caTemp, 0);

        if(psCur == NULL)
        {
            utWebDispMsg(iFd, psMsgHead, "ncs/ncmsg_back.htm", "部门查询", "数据库出错");
            //        free(pHash);
            return 0;
        }




        memset(caGroupname, 0, sizeof(caGroupname));
        lGroupid = 0;
        lPid = 0;
        iReturn = pasDbFetchInto(psCur, UT_TYPE_STRING, 68, caGroupname,
                                 UT_TYPE_LONG, 4, &lGroupid,
                                 UT_TYPE_LONG, 4, &lPid);


        while(iReturn == 0)
        {
            psData = (struct stData *)pasLHashLook(pHash, (char*)&lGroupid);
            if(psData)
            {
                if(iNum > 0)
                {
                    utPltPutLoopVar(psDbHead, "dh", iNum + 1, ",");
                }
                iNum++;
                utPltSetCvtHtml(1);
                utPltPutLoopVar(psDbHead, "groupname", iNum, utStrGetCharHan(caGroupname, 48));
                utPltSetCvtHtml(0);
                utPltPutLoopVarF(psDbHead, "groupid",   iNum, "%ld", lGroupid);


                utPltPutLoopVar(psDbHead, "leaf", iNum, "false");
                utPltPutLoopVar(psDbHead, "cls", iNum, "folder");


                //      else{
                //              utPltPutLoopVar(psDbHead,"leaf", iNum,"true");
                //                utPltPutLoopVar(psDbHead,"cls", iNum,"folder");

                //           }
                utPltPutLoopVar(psDbHead, "dtype", iNum, "2");

                utPltPutLoopVarF(psDbHead, "iNum", iNum, "%d", iNum - 1);

                utPltPutLoopVarF(psDbHead, "pid", iNum, "%d", lPid);
            }
            memset(caGroupname, 0, sizeof(caGroupname));
            lGroupid = 0;
            lLevel = 0;
            lPid = 0;
            iReturn = pasDbFetchInto(psCur, UT_TYPE_STRING, 68, caGroupname,
                                     UT_TYPE_LONG, 4, &lGroupid,
                                     UT_TYPE_LONG, 4, &lPid);
        }
        pasDbCloseCursor(psCur);
        //     free(pHash);
        utPltPutVar(psDbHead, "rootname", utComGetVar_sd(psShmHead, "rootname", "全部单位"));

    }
    sprintf(caTemp, "select dispname,userid,groupid,username from ncsuser where 1=1 ");
    if(strlen(caGroups) > 0)
    {
        sprintf(caTemp + strlen(caTemp), " and groupid in (%s) ", caGroups);
    }
    if(strlen(caFacs) > 0)
    {
        sprintf(caTemp + strlen(caTemp), " and fcode in (%s) ", caFacs);
    }
    if(strlen(caWireflag) > 0)
    {
        if(strcmp(caWireflag, "2") == 0)
        {
            sprintf(caTemp + strlen(caTemp), " and (jointype='08' or jointype='09') ");
        }
        else if(strcmp(caWireflag, "1") == 0)
        {
            sprintf(caTemp + strlen(caTemp), " and jointype!='08' ");
        }

    }


    if(strlen(caCname) == 0)
    {

        if(strlen(caPid) == 0)
        {
            sprintf(caTemp + strlen(caTemp), " and groupid not in (select groupid from ncsgroup ) ");

        }
        else
        {
            sprintf(caTemp + strlen(caTemp), " and groupid=%d ", atol(caPid));

        }
    }
    else
    {
        sprintf(caTemp + strlen(caTemp), "and (dispname like '%c%s%c' or username like '%c%s%c') ", '%', caCname, '%', '%', caCname, '%');



    }
    sprintf(caTemp + strlen(caTemp), " order by dispname limit %lu,%lu", 0, lRowNum);
    printf("caTemp=%s\n", caTemp);
    psCur = pasDbOpenSql(caTemp, 0);
    if(psCur != NULL)
    {
        memset(caGroupname, 0, sizeof(caGroupname));
        lGroupid = 0;
        lPid = 0;
        memset(caUsername, 0, sizeof(caUsername));
        iReturn = pasDbFetchInto(psCur, UT_TYPE_STRING, 68, caGroupname,
                                 UT_TYPE_LONG, 4, &lGroupid,
                                 UT_TYPE_LONG, 4, &lPid,
                                 UT_TYPE_STRING, 15, caUsername);

        while(iReturn == 0)
        {

            if(iNum > 0)
            {
                utPltPutLoopVar(psDbHead, "dh", iNum + 1, ",");
            }
            iNum++;
            utPltSetCvtHtml(1);
            utPltPutLoopVar(psDbHead, "groupname", iNum, utStrGetCharHan(caGroupname, 48));
            utPltSetCvtHtml(0);
            utPltPutLoopVarF(psDbHead, "did",   iNum, "%ld", lGroupid);

            utPltPutLoopVar(psDbHead, "leaf", iNum, "true");
            utPltPutLoopVar(psDbHead, "cls", iNum, "file");

            utPltPutLoopVar(psDbHead, "dtype", iNum, "0");
            utPltPutLoopVar(psDbHead, "code", iNum, caUsername);


            utPltPutLoopVarF(psDbHead, "iNum", iNum, "%d", iNum - 1);

            utPltPutLoopVarF(psDbHead, "pid", iNum, "%d", lPid);

            memset(caGroupname, 0, sizeof(caGroupname));
            lGroupid = 0;
            lLevel = 0;
            lPid = 0;
            memset(caUsername, 0, sizeof(caUsername));
            iReturn = pasDbFetchInto(psCur, UT_TYPE_STRING, 68, caGroupname,
                                     UT_TYPE_LONG, 4, &lGroupid,
                                     UT_TYPE_LONG, 4, &lPid,
                                     UT_TYPE_STRING, 15, caUsername);
        }
        pasDbCloseCursor(psCur);
    }
    if(pHash)
        free(pHash);

    if(strlen(caPlate) > 0)
    {
        printf("\n\n caPlate > 0 \n\n");
        utPltOutToHtml(iFd, psMsgHead, psDbHead, caPlate);
    }
    else
    {
        printf("\n\n caPlate <=0 输出json到 mac/infoquery/macterm/ncs_user_tree.htm \n\n");
        //utPltOutToHtml(iFd, psMsgHead, psDbHead, "v4/ncs_user_tree.htm");
        utPltOutToHtml(iFd, psMsgHead, psDbHead, "mac/infoquery/macterm/ncs_user_tree.htm");
    }
    return 0;
}




/* 热点查询：显示热点信息日志 */
int ncsHotSpot_infolist_v4(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead)
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
    long lCurPg, lRowNum, lStartRec, lSstarttime;
    char sql[40960] = "";
    char caTotPg[16], caCurPg[16];
    char caPage[12], caLimit[16], caSort[16], caDir[16];
    char caKeyword[64], caTime_flag[32];
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
    char caArea[256], caName2[256];
    char caTemp[1024];
    long iReturn;
    unsigned long lIp;
    char caStime[16];
    char caEtime[16];
    char caTemp_d[64];
    char caGroupid[36], caDid[36];
    char caSdate[24], caEdate[24];
    char caApmac[36], caMac[36], caApname[64], caPosition[64];
    char caCorpname[128];
    char caCont[2048], caSsid[32] = "";
    char caServicecode[16], caSearchtype[32];
    strcpy(caFacs, (const char*)getDsFacDid());
    strcpy(caGroups, (const char*)getDsGroupid());

    /* 如果导出 */
    char caExport[10] = "";



    utPltDbHead *psDbHead = utPltInitDb();
    utMsgPrintMsg(psMsgHead);
    iReturn = utMsgGetSomeNVar(psMsgHead, 22,
                               "TotPg",        UT_TYPE_STRING, 12, caTotPg,
                               "CurPg",        UT_TYPE_STRING, 12, caCurPg,
                               "keyword",      UT_TYPE_STRING, 60, caKeyword,
                               "page",         UT_TYPE_STRING, 10, caPage,
                               "limit",        UT_TYPE_STRING, 10, caLimit,
                               "sort",         UT_TYPE_STRING, 15, caSort,
                               "dir",          UT_TYPE_STRING, 8,  caDir,
                               "time_flag",    UT_TYPE_STRING, 8, caTime_flag,
                               "did",          UT_TYPE_STRING, 10, caDid,
                               "groupid",      UT_TYPE_STRING, 10, caGroupid,
                               "sdate",        UT_TYPE_STRING, 12, sdate,
                               "stime",        UT_TYPE_STRING, 10, caStime,
                               "edate",        UT_TYPE_STRING, 12, edate,
                               "etime",        UT_TYPE_STRING, 10, caEtime,
                               "mac",          UT_TYPE_STRING, 20, caMac,
                               "ssid",         UT_TYPE_STRING, 30, caSsid,
                               "searchtype",   UT_TYPE_STRING, 10, caSearchtype,
                               "hotinfoexport",      UT_TYPE_STRING, 9, caExport,
                               "servicecode",  UT_TYPE_STRING, 15, caServicecode,
                               "servicename",  UT_TYPE_STRING, 60, caCorpname,
                               "apmac",        UT_TYPE_STRING, 20, caApmac,
                               "apname",       UT_TYPE_STRING, 31, caApname);

    if(strlen(caSearchtype) > 0)
    {
        strcpy(caKeyword, "");
    }
    else
    {
        strcpy(sdate, "");
        strcpy(edate, "");
    }
    memset(caCont, 0, sizeof(caCont));

    if(strlen(caKeyword) > 0)
    {
        strcpy(caTemp_d, pasCvtGBK(2, caKeyword, caTemp, 60));
        strcpy(caKeyword, caTemp_d);
    }
    if(strlen(caCorpname) > 0)
    {
        strcpy(caTemp_d, pasCvtGBK(2, caCorpname, caTemp, 60));
        strcpy(caCorpname, caTemp_d);
    }

    if(strlen(caApname) > 0)
    {
        strcpy(caTemp_d, pasCvtGBK(2, caApname, caTemp, 60));
        strcpy(caApname, caTemp_d);
    }
    if(strlen(caSsid) > 0)
    {
        strcpy(caTemp_d, pasCvtGBK(2, caSsid, caTemp, 60));
        strcpy(caSsid, caTemp_d);
    }

    lCurPg = atol(caPage);
    if(lCurPg <= 0) lCurPg = 1;

    if(strlen(caLimit) > 0)
    {
        lRowNum = atol(caLimit);
    }
    else
    {
        lRowNum = 40; //每一页面行数
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
        ptmp = ncsUtlGetTable3(sdate, edate, caStime, caEtime, "ncmachotinfo_if_", &lStartTime, &lTime, &lCount);
        printf("lStartTime=%d,sdate=%s\n", lStartTime, sdate);
    }
    else
    {
        ptmp = ncsUtlGetTable(lTime, days, "ncmachotinfo_if_",  &lStartTime, &lCount);
        lTime = lTime + 2 * 3600;
    }

    memset(caCont, 0, sizeof(caCont));

    /* 高级查询搜索字段 */
    if(strlen(caSearchtype) > 0)
    {
        if(strlen(caApmac) > 0)         //热点MAC
        {
            sprintf(caCont + strlen(caCont), " and apmac like '%c%s%c' ", '%', caApmac, '%');
        }
        if(strlen(caApname) > 0)        //采集设备编号
        {
            sprintf(caCont + strlen(caCont), " and apname like '%c%s%c' ", '%', caApname, '%');
        }
        if(strlen(caServicecode) > 0)   //场所编号
        {
            sprintf(caCont + strlen(caCont), " and servicecode like '%c%s%c' ", '%', caServicecode, '%');
        }
        if(strlen(caCorpname) > 0)      //场所名称
        {
            sprintf(caCont + strlen(caCont), " and servicename like '%c%s%c' ", '%', caCorpname, '%');
        }
    }
    else
    {
        if(strlen(caKeyword) > 0)
        {
            sprintf(caCont, " and (mac like '%c%s%c' or ssid like '%c%s%c' or servicecode like '%c%s%c' or servicename like '%c%s%c' or apmac like '%c%s%c') ", '%', caKeyword, '%', '%', caKeyword, '%', '%', caKeyword, '%', '%', caKeyword, '%', '%', caKeyword, '%');
        }
    }

    /*  当前用户权限下所能查询场所*/
    if(strlen(caGroups) > 0)
    {
        sprintf(caCont + strlen(caCont), " and %s", getDsGroupcodeSql("servicecode"));
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
        strcat(sql3, caCont);
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
    utPltPutVarF(psDbHead, "distinctCount", "%lu", lTotRec2);

    for(i = 0; i < lCount; i++)
    {
        lCount1 = 0;
        memset(sql, 0, sizeof(sql));

        /* 关联查询 */
        //snprintf(sql, sizeof(sql), "select count(*) from %s a, nchotspotinfo b where a.stime >= %lu and a.stime <= %lu and a.ssid != \'\' AND a.ssid = b.ssid ",  ptmp[i], lStartTime, lTime);

        /* 非关联查询 */
        snprintf(sql, sizeof(sql), "select count(*) from %s a where a.stime >= %lu and a.stime <= %lu and a.ssid != \'\' ",  ptmp[i], lStartTime, lTime);
        strcat(sql, caCont);

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
    utPltPutVarF(psDbHead, "TotRec", "%lu", lTotRec);


    for(i = num - 1; i >= 0; i--)
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
        sprintf(caFilename, "hotInfoLog.csv");
        sprintf(caPath, "%s", "/home/ncmysql/ncs");
        sprintf(caFile, "%s/%s", caPath, caFilename);
        fp = fopen(caFile, "w");
        if(fp == NULL)
        {
            printf("数据导出出错\n\n");
            return 0;
        }
        fprintf(fp, "热点查询\n");  //标题
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

    for(i = num - 1; i >= 0; i--)
    {
        memset(sql, 0, sizeof(sql));
        if(tableinfo[i].lCount <= iStart)
        {
            iStart -= tableinfo[i].lCount;
            continue;
        }
        /* 加上是否合规字段 */
        //snprintf(sql, sizeof(sql), "select a.servicecode, a.servicename, a.address, a.mac, a.apname, a.apmac, a.ssid, a.stime, a.channel, a.rssi, a.security, a.longitude, a.latitude, b.authflag from %s a, nchotspotinfo b where a.stime >= %lu and a.stime <= %lu and a.ssid != \'\' AND a.ssid = b.ssid ", tableinfo[i].tname, lStartTime, lTime);

        /* 去掉是否合规字段 */
        snprintf(sql, sizeof(sql), "select a.servicecode, a.servicename, a.address, a.mac, a.apname, a.apmac, a.ssid, a.stime, a.channel, a.rssi, a.security, a.longitude, a.latitude from %s a where a.stime >= %lu and a.stime <= %lu and a.ssid != \'\' ", tableinfo[i].tname, lStartTime, lTime);

        strcat(sql, caCont);

        snprintf(sql + strlen(sql), sizeof(sql) - strlen(sql), " order by a.stime desc limit %lu, %lu", iStart, iLimit < tableinfo[i].lCount ? iLimit : tableinfo[i].lCount);


        if(tableinfo[i].lCount > iStart && tableinfo[i].lCount < iStart + iLimit)
        {
            iLimit = iLimit - (tableinfo[i].lCount - iStart);
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
            char caServicecode[16], caServicename[64], caAddress[64], caMac[20], caApname[32], caApmac[20], caSsid[64];
            long lStarttime, lEndtime, lFlags;
            char caChannel[32], caRssi[16], caSecurity[16], caLongitude[16], caLatitude[16], caAuthFlag[3] = "";
            char caHotspotfirm[32] = "", caMacBinary[16] = "";
            unsigned char *pHotspotfirm;

            memset(caServicecode, 0, sizeof(caServicecode));
            memset(caServicename, 0, sizeof(caServicename));
            memset(caAddress, 0, sizeof(caAddress));
            memset(caMac, 0, sizeof(caMac));
            memset(caApname, 0, sizeof(caApname));
            memset(caApmac, 0, sizeof(caApmac));
            memset(caSsid, 0, sizeof(caSsid));
            lStarttime = 0;
            memset(caChannel, 0, sizeof(caChannel));
            memset(caRssi, 0, sizeof(caRssi));
            memset(caSecurity, 0, sizeof(caSecurity));
            memset(caLongitude, 0, sizeof(caLongitude));
            memset(caLatitude, 0, sizeof(caLatitude));
            iret = pasDbFetchInto(psCur,
                                  UT_TYPE_STRING, 14, caServicecode,
                                  UT_TYPE_STRING, 63, caServicename,
                                  UT_TYPE_STRING, 63, caAddress,
                                  UT_TYPE_STRING, 18, caMac,
                                  UT_TYPE_STRING, 31, caApname,
                                  UT_TYPE_STRING, 18, caApmac,
                                  UT_TYPE_STRING, 63, caSsid,
                                  UT_TYPE_LONG, 4, &lStarttime,
                                  UT_TYPE_STRING, 2, caChannel,
                                  UT_TYPE_STRING, 8, caRssi,
                                  UT_TYPE_STRING, 2, caSecurity,
                                  UT_TYPE_STRING, 11, caLongitude,
                                  UT_TYPE_STRING, 11, caLatitude,
                                  UT_TYPE_STRING, 3, caAuthFlag);

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
                        utPltPutLoopVar(psDbHead, "dh", iNum, ",");
                    }

                    utPltPutLoopVar(psDbHead, "authflag", iNum, caAuthFlag);
                    utPltPutLoopVar(psDbHead, "stime", iNum, utTimFormat("%Y-%m-%d %H:%M:%S", lStarttime));
                    utPltPutLoopVar(psDbHead, "address", iNum, caAddress);
                    utPltPutLoopVar(psDbHead, "apname", iNum, caApname);
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

                    utPltPutLoopVar(psDbHead, "hotspotfirm", iNum, caHotspotfirm);


                    utPltPutLoopVar(psDbHead, "apmac", iNum, caMac); //前台热点mac显示为mac地址
                    utPltPutLoopVar(psDbHead, "servicename", iNum, caServicename);
                    utPltPutLoopVar(psDbHead, "channel", iNum, caChannel);
                    utPltPutLoopVar(psDbHead, "rssi", iNum, caRssi);
                    //printf("caSecurity=%s\n",caSecurity);
                    utPltPutLoopVar(psDbHead, "security", iNum, caSecurity);
                    utPltPutLoopVar(psDbHead, "longitude", iNum, caLongitude);
                    utPltPutLoopVar(psDbHead, "latitude", iNum, caLatitude);
                    utPltPutLoopVar(psDbHead, "ssid", iNum, caSsid);
                    utPltPutLoopVar(psDbHead, "servicecode", iNum, caServicecode);
                }

                memset(caAuthFlag, 0, sizeof(caAuthFlag));
                memset(caServicecode, 0, sizeof(caServicecode));
                memset(caServicename, 0, sizeof(caServicename));
                memset(caAddress, 0, sizeof(caAddress));
                memset(caMac, 0, sizeof(caMac));
                memset(caApname, 0, sizeof(caApname));
                memset(caApmac, 0, sizeof(caApmac));
                memset(caSsid, 0, sizeof(caSsid));
                lStarttime = 0;
                memset(caChannel, 0, sizeof(caChannel));
                memset(caRssi, 0, sizeof(caRssi));
                memset(caSecurity, 0, sizeof(caSecurity));
                memset(caLongitude, 0, sizeof(caLongitude));
                memset(caLatitude, 0, sizeof(caLatitude));
                iret = pasDbFetchInto(psCur,
                                      UT_TYPE_STRING, 14, caServicecode,
                                      UT_TYPE_STRING, 63, caServicename,
                                      UT_TYPE_STRING, 63, caAddress,
                                      UT_TYPE_STRING, 18, caMac,
                                      UT_TYPE_STRING, 31, caApname,
                                      UT_TYPE_STRING, 18, caApmac,
                                      UT_TYPE_STRING, 63, caSsid,
                                      UT_TYPE_LONG, 4, &lStarttime,
                                      UT_TYPE_STRING, 2, caChannel,
                                      UT_TYPE_STRING, 8, caRssi,
                                      UT_TYPE_STRING, 2, caSecurity,
                                      UT_TYPE_STRING, 11, caLongitude,
                                      UT_TYPE_STRING, 11, caLatitude,
                                      UT_TYPE_STRING, 3, caAuthFlag);

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
        utPltFileDownload(iFd, "application/text", caPath, caFilename, caFilename);
        remove(caFile);
        return 0;
    }

    utPltOutToHtml(iFd, psMsgHead, psDbHead, "rzcx/proauth_hotinfo_list.htm");
    return 0;
}

/* 热点查询：标记合规、不合规 */
int ncsHotSpot_MarkRuleorNot(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead)
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
            utPltPutVar(psDbHead, "result", "false");
            utPltPutVar(psDbHead, "message", "标记失败");
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
        utPltPutVar(psDbHead, "result", "true");
        utPltPutVar(psDbHead, "message", "标记成功");
    }

    utPltOutToHtml(iFd, psMsgHead, psDbHead, "mac/infoquery/hotinfo/MarkRuleorNot.htm");
    return 0;

}




/* 场所信息查询：显示终端特征信息日志表 */
int ncsPlace_mactermlist_v4(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead)
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
    long lCurPg, lRowNum, lStartRec, lSstarttime;
    char sql[40960] = "";
    char caTotPg[16], caCurPg[16];
    char caPage[12], caLimit[16], caSort[16], caDir[16];
    char caKeyword[64], caTime_flag[32];
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
    char caArea[256], caName2[256];
    char caTemp[1024];
    long iReturn;
    unsigned long lIp;
    char caStime[16] = "";
    char caEtime[16] = "";
    char caTemp_d[64];
    char caGroupid[36], caDid[36];
    char caSdate[24], caEdate[24];
    char caApmac[36], caMac[36], caApname[64], caPosition[64];
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
    iReturn = utMsgGetSomeNVar(psMsgHead, 27,
                               "TotPg",        UT_TYPE_STRING, 12, caTotPg,
                               "CurPg",        UT_TYPE_STRING, 12, caCurPg,
                               "keyword",      UT_TYPE_STRING, 60,  caKeyword,
                               "page",         UT_TYPE_STRING, 10,  caPage,
                               "limit",        UT_TYPE_STRING, 10,  caLimit,
                               "sort",         UT_TYPE_STRING, 15, caSort,
                               "dir",          UT_TYPE_STRING, 8,  caDir,
                               "time_flag",    UT_TYPE_STRING, 8,   caTime_flag,
                               "did",          UT_TYPE_STRING, 10,  caDid,
                               "groupid",      UT_TYPE_STRING, 16,  caGroupid,
                               "sdate",        UT_TYPE_STRING, 12,  sdate,
                               "stime",        UT_TYPE_STRING, 10,  caStime,
                               "edate",        UT_TYPE_STRING, 12,  edate,
                               "etime",        UT_TYPE_STRING, 10,  caEtime,
                               "apmac",        UT_TYPE_STRING, 20,  caApmac,
                               "mac",          UT_TYPE_STRING, 20,  caMac,
                               "apname",       UT_TYPE_STRING, 31,  caApname,
                               "username",     UT_TYPE_STRING, 16,  caUserName,
                               "ssid",         UT_TYPE_STRING, 30,  caSsid,
                               "servicename",  UT_TYPE_STRING, 60,  caCorpname,
                               "servicecode",  UT_TYPE_STRING, 15,  caServicecode,

                               "searchtype",   UT_TYPE_STRING, 10,  caSearchtype,
                               "sname",      UT_TYPE_STRING, 31, caSname,
                               "servicegtype",      UT_TYPE_STRING, 31, caServicegtype,
                               "servicearea",      UT_TYPE_STRING, 35, caServicearea,
                               "ceoname",      UT_TYPE_STRING, 31, caCeoname,
                               "jytype",      UT_TYPE_STRING, 31, caJytype);

    char caGroupcodeSql[1024] = "";
    strcpy(caGroupcodeSql, getGroupcodeSql("servicecode", caGroupid));

    MyremoveAll(caKeyword, '-');
    MyremoveAll(caKeyword, ':');

    if(strlen(caSearchtype) > 0)
    {
        strcpy(caKeyword, "");
    }
    else
    {
        strcpy(sdate, "");
        strcpy(edate, "");
    }
    memset(caCont, 0, sizeof(caCont));


    if(strlen(caKeyword) > 0)
    {
        strcpy(caTemp_d, pasCvtGBK(2, caKeyword, caTemp, 60));
        strcpy(caKeyword, caTemp_d);
    }
    if(strlen(caCorpname) > 0)
    {
        strcpy(caTemp_d, pasCvtGBK(2, caCorpname, caTemp, 60));
        strcpy(caCorpname, caTemp_d);
    }
    if(strlen(caApname) > 0)
    {
        strcpy(caTemp_d, pasCvtGBK(2, caApname, caTemp, 60));
        strcpy(caApname, caTemp_d);
    }
    if(strlen(caSsid) > 0)
    {
        strcpy(caTemp_d, pasCvtGBK(2, caSsid, caTemp, 60));
        strcpy(caSsid, caTemp_d);
    }

    lCurPg = atol(caPage);
    if(lCurPg <= 0) lCurPg = 1;

    if(strlen(caLimit) > 0)
    {
        lRowNum = atol(caLimit);
    }
    else
    {
        lRowNum = 40; //每一页面行数
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
        ptmp = ncsUtlGetTable3(sdate, edate, caStime, caEtime, "ncmactermatt_if_", &lStartTime, &lTime, &lCount);
        printf("lStartTime=%d,sdate=%s\n", lStartTime, sdate);
    }
    else
    {
        ptmp = ncsUtlGetTable(lTime, days, "ncmactermatt_if_",  &lStartTime, &lCount);
        lTime = lTime + 2 * 3600;
    }

    memset(caCont, 0, sizeof(caCont));

    /* 对输入框字体进行判断：如果有 @则不是关键字，否则则是 */
    char caGroupidJudge[30] = "";

    /* 高级查询条件 */
    if(strlen(caSearchtype) > 0)
    {
        if(strlen(caMac) > 0)
        {
            sprintf(caCont + strlen(caCont), " and a.mac like '%c%s%c' ", '%', caMac, '%');
        }
        if(strlen(caApmac) > 0)
        {
            sprintf(caCont + strlen(caCont), " and a.apmac like '%c%s%c' ", '%', caApmac, '%');
        }
        if(strlen(caApname) > 0)
        {
            sprintf(caCont + strlen(caCont), " and a.apname like '%c%s%c' ", '%', caApname, '%');
        }
        if(strlen(caCorpname) > 0)
        {
            sprintf(caCont + strlen(caCont), " and a.servicename like '%c%s%c' ", '%', caCorpname, '%');
        }

        /* 高级查询条件 */
        if(strlen(caSname) > 0)         //安全厂商名称查询
        {
            //sprintf(caCont+strlen(caCont)," and servicename like '%c%s%c' ",'%',caSname,'%');
            printf(" 安全厂商名称为 %s \n\n", caSname);
        }
        if(strlen(caServicecode) > 0)   //场所编号
        {
            sprintf(caCont + strlen(caCont), " and a.servicecode like '%c%s%c' ", '%', caServicecode, '%');
        }
        if(strlen(caServicegtype) > 0)  //场所服务类型
        {
            //sprintf(caCont+strlen(caCont)," and servicename like '%c%s%c' ",'%',caIdname,'%');
            printf(" 场所服务类型 %s \n\n", caServicegtype);
        }
        if(strlen(caServicearea) > 0)   //场所所属区域
        {
            printf(" 场所所属区域 %s \n\n", caServicearea);
        }
        if(strlen(caCeoname) > 0)       //场所法人或负责人姓名
        {
            printf(" 场所法人或负责人姓名 %s \n\n", caCeoname);
        }
        if(strlen(caJytype) > 0)        //场所经营性质
        {
            printf(" 场所法人或负责人姓名 %s \n\n", caJytype);
        }
    }
    else
    {
        printf(" if outer caKeyword = %s\n\n", caKeyword);
        if(strlen(caKeyword) > 0)
        {
            /* 字符串中小写转大写 */
            int i = 0, j = 0;
            j = strlen(caKeyword);

            for(i = 0; i <= j; i++)
            {
                if(caKeyword[i] >= 'a' && caKeyword[i] <= 'z')
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
                    printf("caKeyword = %s, caGroupidJudge = %s\n\n", caKeyword, caGroupidJudge);
                    if(strlen(caGroupid) > 0)
                    {
                        if(strlen(caGroupcodeSql) > 0)
                            sprintf(caCont + strlen(caCont), " and %s", caGroupcodeSql);

                        printf(" caGroupid = %s, caCont = %s", caGroupid, caCont);
                    }
                    else
                    {
                        if(strlen(caUserName) > 0)
                        {
                            //printf("In caKeyword @ caUserName = %s\n", caUserName);

                            //通过 caUserId 去获取相对应 lGroupId
                            sprintf(caCont + strlen(caCont), " and a.servicecode = \'%s\'", caUserName);

                            printf(" caUserName = %s, caCont = %s", caUserName, caCont);
                        }
                    }


                    /* 判断输入框caKeyword 是否有第二个'@'  字符: 如果有，则取其后面值作为搜索条件 */
                    memset(caGroupidJudge, 0, sizeof(caGroupidJudge));
                    char *pstrKeywordServiceName = NULL, *pstrKeywordMac = NULL;
                    pstrKeywordServiceName = strtok_r(caKeyword, "@", &pstrKeywordMac);
                    //printf("pstrKeywordServiceName, pstrKeywordMac = %s\n", pstrKeywordServiceName, pstrKeywordMac);

                    /* @  场所@  后面如果输入了mac/apmac */
                    if(strcmp(pstrKeywordMac, "") != 0)
                        sprintf(caCont + strlen(caCont), " and (replace(a.mac,'-','') like '%c%s%c' or replace(a.apmac,'-','') like '%c%s%c') ", '%', pstrKeywordMac, '%', '%', pstrKeywordMac, '%');
                }
                else
                {
                    sprintf(caCont + strlen(caCont), " and (a.servicecode like '%c%s%c' or a.servicename like '%c%s%c' or a.apname like '%c%s%c') ", '%', caKeyword, '%', '%', caKeyword, '%', '%', caKeyword, '%');
                }
            }

        }
    }

    /*if(strlen(caDid)>0)
    {

        sprintf(caCont+strlen(caCont)," and a.servicecode='%s' ",getServicecodeByid(psShmHead,atol(caDid)));
    }*/

    /*  当前用户权限下所能查询场所*/
    if(strlen(caGroups) > 0)
    {
        sprintf(caCont + strlen(caCont), " and %s", getDsGroupcodeSql("servicecode"));
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
        strcat(sql3, caCont);
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
    utPltPutVarF(psDbHead, "distinctCount", "%lu", lTotRec2);


    for(i = 0; i < lCount; i++)
    {
        lCount1 = 0;
        memset(sql, 0, sizeof(sql));
        snprintf(sql, sizeof(sql), "select count(*) from %s a where a.stime >= %lu and a.stime <= %lu  ", ptmp[i], lStartTime, lTime);
        strcat(sql, caCont);
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
    utPltPutVarF(psDbHead, "TotRec", "%lu", lTotRec);


    for(i = num - 1; i >= 0; i--)
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
        sprintf(caFilename, "placeLog.csv");
        sprintf(caPath, "%s", "/home/ncmysql/ncs");
        sprintf(caFile, "%s/%s", caPath, caFilename);
        fp = fopen(caFile, "w");
        if(fp == NULL)
        {
            printf("数据导出出错\n\n");
            return 0;
        }
        fprintf(fp, "场所信息查询\n");  //标题
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

    for(i = num - 1; i >= 0; i--)
    {
        memset(sql, 0, sizeof(sql));
        if(tableinfo[i].lCount <= iStart)
        {
            iStart -= tableinfo[i].lCount;
            continue;
        }
        /* 加上关联字段 */
        //snprintf(sql, sizeof(sql), "select a.servicecode,a.servicename,d.address,a.mac,a.apname,a.apmac,a.ssid,a.stime,a.channel,a.rssi,a.security,a.longitude,a.latitude,a.termtype,a.vtype,a.vname, b.dispname, b.fcode, c.name from %s a, ncaplist b, ncsfactorycod c, ncsuser d where a.stime >= %lu and a.stime <= %lu and b.apname = RIGHT(a.apname, 12) and b.fcode = c.code and a.servicecode = d.username ", tableinfo[i].tname, lStartTime, lTime);

        /* 去掉字段 */
        snprintf(sql, sizeof(sql), "select a.servicecode,a.servicename,a.address,a.mac,a.apname,a.apmac,a.ssid,a.stime,a.channel,a.rssi,a.security,a.longitude,a.latitude,a.termtype,a.vtype,a.vname from %s a where a.stime >= %lu and a.stime <= %lu ", tableinfo[i].tname, lStartTime, lTime);


        strcat(sql, caCont);

        snprintf(sql + strlen(sql), sizeof(sql) - strlen(sql), " order by a.stime desc limit %lu, %lu", iStart, iLimit < tableinfo[i].lCount ? iLimit : tableinfo[i].lCount);

        if(tableinfo[i].lCount > iStart && tableinfo[i].lCount < iStart + iLimit)
        {
            iLimit = iLimit - (tableinfo[i].lCount - iStart);
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
            char caServicecode[16], caServicename[64], caAddress[64], caMac[20], caApname[32], caApmac[20], caSsid[64];
            long lStarttime, lEndtime, lFlags;
            char caChannel[32], caRssi[16], caSecurity[16], caLongitude[16], caLatitude[16];
            char caTermtype[128], caVname[64];
            long lVtype;

            /* 采集设备名称、安全厂商名称、厂商组织机构代码 */
            char caDispname[32] = "";
            char caFactoryname[32] = "";
            char caFactorycode[32] = "";

            memset(caServicecode, 0, sizeof(caServicecode));
            memset(caServicename, 0, sizeof(caServicename));
            memset(caAddress, 0, sizeof(caAddress));
            memset(caMac, 0, sizeof(caMac));
            memset(caApname, 0, sizeof(caApname));
            memset(caApmac, 0, sizeof(caApmac));
            memset(caSsid, 0, sizeof(caSsid));
            lStarttime = 0;
            memset(caChannel, 0, sizeof(caChannel));
            memset(caRssi, 0, sizeof(caRssi));
            memset(caSecurity, 0, sizeof(caSecurity));
            memset(caLongitude, 0, sizeof(caLongitude));
            memset(caLatitude, 0, sizeof(caLatitude));
            memset(caTermtype, 0, sizeof(caTermtype));
            memset(caVname, 0, sizeof(caVname));
            lVtype = 0;
            iret = pasDbFetchInto(psCur,
                                  UT_TYPE_STRING, 14, caServicecode,
                                  UT_TYPE_STRING, 63, caServicename,
                                  UT_TYPE_STRING, 63, caAddress,
                                  UT_TYPE_STRING, 18, caMac,
                                  UT_TYPE_STRING, 31, caApname,
                                  UT_TYPE_STRING, 18, caApmac,
                                  UT_TYPE_STRING, 63, caSsid,
                                  UT_TYPE_LONG, 4, &lStarttime,
                                  UT_TYPE_STRING, 2, caChannel,
                                  UT_TYPE_STRING, 8, caRssi,
                                  UT_TYPE_STRING, 2, caSecurity,
                                  UT_TYPE_STRING, 11, caLongitude,
                                  UT_TYPE_STRING, 11, caLatitude,
                                  UT_TYPE_STRING, 100, caTermtype,
                                  UT_TYPE_LONG, 4, &lVtype,
                                  UT_TYPE_STRING, 63, caVname,
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
                        utPltPutLoopVar(psDbHead, "dh", iNum, ",");
                    }
                    utPltPutLoopVar(psDbHead, "stime", iNum, utTimFormat("%Y-%m-%d %H:%M:%S", lStarttime));
                    utPltPutLoopVar(psDbHead, "address", iNum, caAddress);
                    utPltPutLoopVar(psDbHead, "apname", iNum, caApname);
                    utPltPutLoopVar(psDbHead, "mac", iNum, caMac);
                    utPltPutLoopVar(psDbHead, "apmac", iNum, caApmac);
                    utPltPutLoopVar(psDbHead, "servicename", iNum, caServicename);
                    utPltPutLoopVar(psDbHead, "channel", iNum, caChannel);
                    utPltPutLoopVar(psDbHead, "rssi", iNum, caRssi);
                    //printf("caSecurity=%s\n",caSecurity);
                    utPltPutLoopVar(psDbHead, "security", iNum, caSecurity);
                    utPltPutLoopVar(psDbHead, "longitude", iNum, caLongitude);
                    utPltPutLoopVar(psDbHead, "latitude", iNum, caLatitude);
                    utPltPutLoopVar(psDbHead, "ssid", iNum, caSsid);
                    utPltPutLoopVar(psDbHead, "servicecode", iNum, caServicecode);
                    utPltPutLoopVar(psDbHead, "termtype", iNum, caTermtype);
                    utPltPutLoopVar(psDbHead, "vname", iNum, caVname);
                    utPltPutLoopVarF(psDbHead, "vtype", iNum, "%d", lVtype);

                    utPltPutLoopVarF(psDbHead, "dispname", iNum, "%s", caDispname);
                    utPltPutLoopVarF(psDbHead, "factoryname", iNum, "%s", caFactoryname);
                    utPltPutLoopVarF(psDbHead, "factorycode", iNum, "%s", caFactorycode);
                }

                memset(caServicecode, 0, sizeof(caServicecode));
                memset(caServicename, 0, sizeof(caServicename));
                memset(caAddress, 0, sizeof(caAddress));
                memset(caMac, 0, sizeof(caMac));
                memset(caApname, 0, sizeof(caApname));
                memset(caApmac, 0, sizeof(caApmac));
                memset(caSsid, 0, sizeof(caSsid));
                lStarttime = 0;
                memset(caChannel, 0, sizeof(caChannel));
                memset(caRssi, 0, sizeof(caRssi));
                memset(caSecurity, 0, sizeof(caSecurity));
                memset(caLongitude, 0, sizeof(caLongitude));
                memset(caLatitude, 0, sizeof(caLatitude));
                memset(caTermtype, 0, sizeof(caTermtype));
                memset(caVname, 0, sizeof(caVname));
                memset(caDispname, 0, sizeof(caDispname));
                memset(caFactoryname, 0, sizeof(caFactoryname));
                memset(caFactorycode, 0, sizeof(caFactorycode));

                lVtype = 0;
                iret = pasDbFetchInto(psCur,
                                      UT_TYPE_STRING, 14, caServicecode,
                                      UT_TYPE_STRING, 63, caServicename,
                                      UT_TYPE_STRING, 63, caAddress,
                                      UT_TYPE_STRING, 18, caMac,
                                      UT_TYPE_STRING, 31, caApname,
                                      UT_TYPE_STRING, 18, caApmac,
                                      UT_TYPE_STRING, 63, caSsid,
                                      UT_TYPE_LONG, 4, &lStarttime,
                                      UT_TYPE_STRING, 2, caChannel,
                                      UT_TYPE_STRING, 8, caRssi,
                                      UT_TYPE_STRING, 2, caSecurity,
                                      UT_TYPE_STRING, 11, caLongitude,
                                      UT_TYPE_STRING, 11, caLatitude,
                                      UT_TYPE_STRING, 100, caTermtype,
                                      UT_TYPE_LONG, 4, &lVtype,
                                      UT_TYPE_STRING, 63, caVname,
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
        utPltFileDownload(iFd, "application/text", caPath, caFilename, caFilename);
        remove(caFile);
        return 0;
    }


    utPltOutToHtml(iFd, psMsgHead, psDbHead, "rzcx/proauth_hotinfo_list.htm");
    return 0;
}

/* 场所信息查询下查看图表1：场所频繁出现的终端TOP10统计
    (根据用户选择的时间点、场所名称或编号) */
int ncsPlace_Chosenplace_TerminalmacTop(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead)
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

    if(strcmp(caTimeFlag, "null") == 0)     //查询今天：默认
    {
        snprintf(sqlTmp, sizeof(sqlTmp), " UNIX_TIMESTAMP(DATE_SUB(NOW(), INTERVAL 1 DAY)) ");
    }
    else if(strcmp(caTimeFlag, "1") == 0)   //查询今天：用户选择
    {
        snprintf(sqlTmp, sizeof(sqlTmp), " UNIX_TIMESTAMP(DATE_SUB(NOW(), INTERVAL 1 DAY)) ");
    }
    else if(strcmp(caTimeFlag, "2") == 0)   //最近三天
    {
        snprintf(sqlTmp, sizeof(sqlTmp), " UNIX_TIMESTAMP(DATE_SUB(NOW(), INTERVAL 3 DAY)) ");
    }
    else if(strcmp(caTimeFlag, "3") == 0)   //最近一周
    {
        snprintf(sqlTmp, sizeof(sqlTmp), " UNIX_TIMESTAMP(DATE_SUB(NOW(), INTERVAL 7 DAY)) ");
    }
    else if(strcmp(caTimeFlag, "4") == 0)   //最近一月
    {
        snprintf(sqlTmp, sizeof(sqlTmp), " UNIX_TIMESTAMP(DATE_SUB(NOW(), INTERVAL 1 MONTH)) ");
    }
    else if(strcmp(caTimeFlag, "5") == 0)   //最近三月
    {
        snprintf(sqlTmp, sizeof(sqlTmp), " UNIX_TIMESTAMP(DATE_SUB(NOW(), INTERVAL 3 MONTH)) ");
    }
    else if(strcmp(caTimeFlag, "6") == 0)   //最近半年
    {
        snprintf(sqlTmp, sizeof(sqlTmp), " UNIX_TIMESTAMP(DATE_SUB(NOW(), INTERVAL 6 MONTH)) ");
    }
    else    //最近半年
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
                                          UT_TYPE_LONG,  20, &dwCount)) || 1405 == nRet)
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

/* 场所信息查询下查看图表2：指定场所的人流密集时段(根据用户选择的时间点、场所名称或编号) */
int ncsPlace_Chosenplace_PeopleCrowedTime(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead)
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

    if(strcmp(caTimeFlag, "null") == 0)     //查询今天：默认
    {
        snprintf(sqlTmp, sizeof(sqlTmp), " UNIX_TIMESTAMP(DATE_SUB(NOW(), INTERVAL 1 DAY)) ");
    }
    else if(strcmp(caTimeFlag, "1") == 0)   //查询今天：用户选择
    {
        snprintf(sqlTmp, sizeof(sqlTmp), " UNIX_TIMESTAMP(DATE_SUB(NOW(), INTERVAL 1 DAY)) ");
    }
    else if(strcmp(caTimeFlag, "2") == 0)   //最近三天
    {
        snprintf(sqlTmp, sizeof(sqlTmp), " UNIX_TIMESTAMP(DATE_SUB(NOW(), INTERVAL 3 DAY)) ");
    }
    else if(strcmp(caTimeFlag, "3") == 0)   //最近一周
    {
        snprintf(sqlTmp, sizeof(sqlTmp), " UNIX_TIMESTAMP(DATE_SUB(NOW(), INTERVAL 7 DAY)) ");
    }
    else if(strcmp(caTimeFlag, "4") == 0)   //最近一月
    {
        snprintf(sqlTmp, sizeof(sqlTmp), " UNIX_TIMESTAMP(DATE_SUB(NOW(), INTERVAL 1 MONTH)) ");
    }
    else if(strcmp(caTimeFlag, "5") == 0)   //最近三月
    {
        snprintf(sqlTmp, sizeof(sqlTmp), " UNIX_TIMESTAMP(DATE_SUB(NOW(), INTERVAL 3 MONTH)) ");
    }
    else if(strcmp(caTimeFlag, "6") == 0)   //最近半年
    {
        snprintf(sqlTmp, sizeof(sqlTmp), " UNIX_TIMESTAMP(DATE_SUB(NOW(), INTERVAL 6 MONTH)) ");
    }
    else    //最近半年
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
                                          UT_TYPE_LONG,  20, &dwCount,
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
int ncsMobileap_mactermlist_v4(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead)
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
    long lCurPg, lRowNum, lStartRec, lSstarttime;
    char sql[40960] = "";
    char caTotPg[16], caCurPg[16];
    char caPage[12], caLimit[16], caSort[16], caDir[16];
    char caKeyword[64], caTime_flag[32];
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
    char caArea[256], caName2[256];
    char caTemp[1024];
    long iReturn;
    unsigned long lIp;
    char caStime[16];
    char caEtime[16];
    char caTemp_d[64];
    char caGroupid[36], caDid[36];
    char caSdate[24], caEdate[24];
    char caApmac[36], caMac[36], caApname[64], caPosition[64];
    char caCorpname[128];
    char caCont[2048];
    char caSsid[36] = "";
    char caServicecode[16], caSearchtype[32];
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
    iReturn = utMsgGetSomeNVar(psMsgHead, 27,
                               "TotPg",      UT_TYPE_STRING, 12, caTotPg,
                               "CurPg",      UT_TYPE_STRING, 12, caCurPg,
                               "keyword", UT_TYPE_STRING, 60, caKeyword,
                               "page",            UT_TYPE_STRING, 10, caPage,
                               "limit",           UT_TYPE_STRING, 10, caLimit,
                               "sort",            UT_TYPE_STRING, 15, caSort,
                               "dir",          UT_TYPE_STRING, 8,  caDir,
                               "time_flag",    UT_TYPE_STRING, 8, caTime_flag,
                               "did",          UT_TYPE_STRING, 10, caDid,
                               "groupid",      UT_TYPE_STRING, 10, caGroupid,
                               "sdate",        UT_TYPE_STRING, 12, sdate,
                               "stime",        UT_TYPE_STRING, 10, caStime,
                               "edate",        UT_TYPE_STRING, 12, edate,
                               "etime",        UT_TYPE_STRING, 10, caEtime,
                               "apmac",        UT_TYPE_STRING, 20, caApmac,
                               "mac",          UT_TYPE_STRING, 20, caMac,
                               "apname",       UT_TYPE_STRING, 31, caApname,
                               "servicecode",     UT_TYPE_STRING, 15, caServicecode,
                               "servicename",     UT_TYPE_STRING, 60, caCorpname,
                               "ssid",            UT_TYPE_STRING, 30, caSsid,
                               "searchtype",      UT_TYPE_STRING, 10, caSearchtype,
                               "model",       UT_TYPE_STRING, 31, caModel,
                               "devid",      UT_TYPE_STRING, 31, caDevid,
                               "plate",      UT_TYPE_STRING, 31, caPlate,
                               "line",       UT_TYPE_STRING, 31, caLine,
                               "aptype",      UT_TYPE_STRING, 31, caAptype,
                               "mobileexport",      UT_TYPE_STRING, 9, caExport);

    if(strlen(caSearchtype) > 0)
    {
        strcpy(caKeyword, "");
    }
    else
    {
        strcpy(sdate, "");
        strcpy(edate, "");
    }
    memset(caCont, 0, sizeof(caCont));

    if(strlen(caKeyword) > 0)
    {
        strcpy(caTemp_d, pasCvtGBK(2, caKeyword, caTemp, 60));
        strcpy(caKeyword, caTemp_d);
    }
    if(strlen(caCorpname) > 0)
    {
        strcpy(caTemp_d, pasCvtGBK(2, caCorpname, caTemp, 60));
        strcpy(caCorpname, caTemp_d);
    }

    if(strlen(caApname) > 0)
    {
        strcpy(caTemp_d, pasCvtGBK(2, caApname, caTemp, 60));
        strcpy(caApname, caTemp_d);
    }
    if(strlen(caSsid) > 0)
    {
        strcpy(caTemp_d, pasCvtGBK(2, caSsid, caTemp, 60));
        strcpy(caSsid, caTemp_d);
    }

    lCurPg = atol(caPage);
    if(lCurPg <= 0) lCurPg = 1;


    if(strlen(caLimit) > 0)
    {
        lRowNum = atol(caLimit);
    }
    else
    {
        lRowNum = 40; //每一页面行数
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
        ptmp = ncsUtlGetTable3(sdate, edate, caStime, caEtime, "ncmactermatt_if_", &lStartTime, &lTime, &lCount);
        printf("lStartTime=%d,sdate=%s\n", lStartTime, sdate);
    }
    else
    {
        ptmp = ncsUtlGetTable(lTime, days, "ncmactermatt_if_",  &lStartTime, &lCount);
        lTime = lTime + 2 * 3600;
    }

    memset(caCont, 0, sizeof(caCont));
    if(strlen(caSearchtype) > 0)
    {
        if(strlen(caMac) > 0)
        {
            sprintf(caCont + strlen(caCont), " and a.mac like '%c%s%c' ", '%', caMac, '%');
        }
        if(strlen(caApmac) > 0)
        {
            sprintf(caCont + strlen(caCont), " and a.apmac like '%c%s%c' ", '%', caApmac, '%');
        }
        if(strlen(caApname) > 0)
        {
            sprintf(caCont + strlen(caCont), " and a.apname like '%c%s%c' ", '%', caApname, '%');
        }
        if(strlen(caServicecode) > 0)
        {
            sprintf(caCont + strlen(caCont), " and a.servicecode like '%c%s%c' ", '%', caServicecode, '%');
        }
        if(strlen(caCorpname) > 0)
        {
            sprintf(caCont + strlen(caCont), " and a.servicename like '%c%s%c' ", '%', caCorpname, '%');
        }

        /* 高级查询搜索字段 */
        if(strlen(caModel) > 0) //车厢编号
        {
            printf(" 车厢编号 %s \n\n", caModel);
        }
        if(strlen(caDevid) > 0) //车牌号码
        {
            printf(" 车牌号码 %s \n\n", caDevid);
        }
        if(strlen(caPlate) > 0) //站点名称
        {
            printf(" 站点名称 %s \n\n", caPlate);
        }
        if(strlen(caLine) > 0)  //线路名称
        {
            printf(" 线路名称 %s \n\n", caLine);
        }
        if(strlen(caAptype) > 0) //设备类型
        {
            printf(" 设备类型 %s \n\n", caAptype);
        }

    }
    else
    {
        if(strlen(caKeyword) > 0)
        {
            sprintf(caCont, " and (a.apname like '%c%s%c' or a.apmac like '%c%s%c') ", '%', caKeyword, '%', '%', caKeyword, '%');
        }
    }

    /*  当前用户权限下所能查询场所*/
    if(strlen(caGroups) > 0)
    {
        sprintf(caCont + strlen(caCont), " and %s", getDsGroupcodeSql("servicecode"));
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
        strcat(sql3, caCont);
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
    utPltPutVarF(psDbHead, "distinctCount", "%lu", lTotRec2);


    for(i = 0; i < lCount; i++)
    {

        lCount1 = 0;
        memset(sql, 0, sizeof(sql));
        snprintf(sql, sizeof(sql), "select count(*) from %s a where a.stime >= %lu and a.stime <= %lu  ",  ptmp[i], lStartTime, lTime);
        strcat(sql, caCont);

        printf("sql = %s\n", sql);
        pasDbOneRecord(sql, 0, UT_TYPE_ULONG, 4, &lCount1);
        if(lCount1 > 0)
        {
            strcpy(tableinfo[num].tname, ptmp[i]);
            tableinfo[num].lCount = lCount1;
            num++;
        }
        //  }
        free(ptmp[i]);
    }
    free(ptmp);
    for(i = 0; i < num; i++)
    {
        lTotRec += tableinfo[i].lCount;
    }
    utPltPutVarF(psDbHead, "TotRec", "%lu", lTotRec);

    for(i = num - 1; i >= 0; i--)
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
        sprintf(caFilename, "mobileLog.csv");
        sprintf(caPath, "%s", "/home/ncmysql/ncs");
        sprintf(caFile, "%s/%s", caPath, caFilename);
        fp = fopen(caFile, "w");
        if(fp == NULL)
        {
            printf("数据导出出错\n\n");
            return 0;
        }
        fprintf(fp, "移动信息查询\n");  //标题
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

    for(i = num - 1; i >= 0; i--)
    {
        memset(sql, 0, sizeof(sql));
        if(tableinfo[i].lCount <= iStart)
        {
            iStart -= tableinfo[i].lCount;
            continue;
        }
        snprintf(sql, sizeof(sql), "select a.servicecode,a.servicename,a.address,a.mac,a.apname,a.apmac,a.ssid,a.stime,a.channel,a.rssi,a.security,a.longitude,a.latitude,a.termtype,a.vtype,a.vname from %s a where a.stime >= %lu and a.stime <= %lu ", tableinfo[i].tname, lStartTime, lTime);
        strcat(sql, caCont);

        snprintf(sql + strlen(sql), sizeof(sql) - strlen(sql), " order by a.stime desc limit %lu, %lu", iStart, iLimit < tableinfo[i].lCount ? iLimit : tableinfo[i].lCount);


        if(tableinfo[i].lCount > iStart && tableinfo[i].lCount < iStart + iLimit)
        {
            iLimit = iLimit - (tableinfo[i].lCount - iStart);
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
            char caServicecode[16], caServicename[64], caAddress[64], caMac[20], caApname[32], caApmac[20], caSsid[64];
            long lStarttime, lEndtime, lFlags;
            char caChannel[32], caRssi[16], caSecurity[16], caLongitude[16], caLatitude[16];
            char caTermtype[128], caVname[64];
            long lVtype;

            /* 采集设备名称、安全厂商名称、厂商组织机构代码 */
            char caDispname[32] = "";
            char caFactoryname[32] = "";
            char caFactorycode[32] = "";

            memset(caServicecode, 0, sizeof(caServicecode));
            memset(caServicename, 0, sizeof(caServicename));
            memset(caAddress, 0, sizeof(caAddress));
            memset(caMac, 0, sizeof(caMac));
            memset(caApname, 0, sizeof(caApname));
            memset(caApmac, 0, sizeof(caApmac));
            memset(caSsid, 0, sizeof(caSsid));
            lStarttime = 0;
            memset(caChannel, 0, sizeof(caChannel));
            memset(caRssi, 0, sizeof(caRssi));
            memset(caSecurity, 0, sizeof(caSecurity));
            memset(caLongitude, 0, sizeof(caLongitude));
            memset(caLatitude, 0, sizeof(caLatitude));
            memset(caTermtype, 0, sizeof(caTermtype));
            memset(caVname, 0, sizeof(caVname));
            lVtype = 0;
            iret = pasDbFetchInto(psCur,
                                  UT_TYPE_STRING, 14, caServicecode,
                                  UT_TYPE_STRING, 63, caServicename,
                                  UT_TYPE_STRING, 63, caAddress,
                                  UT_TYPE_STRING, 18, caMac,
                                  UT_TYPE_STRING, 31, caApname,
                                  UT_TYPE_STRING, 18, caApmac,
                                  UT_TYPE_STRING, 63, caSsid,
                                  UT_TYPE_LONG, 4, &lStarttime,
                                  UT_TYPE_STRING, 2, caChannel,
                                  UT_TYPE_STRING, 8, caRssi,
                                  UT_TYPE_STRING, 2, caSecurity,
                                  UT_TYPE_STRING, 11, caLongitude,
                                  UT_TYPE_STRING, 11, caLatitude,
                                  UT_TYPE_STRING, 100, caTermtype,
                                  UT_TYPE_LONG, 4, &lVtype,
                                  UT_TYPE_STRING, 63, caVname,
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
                        utPltPutLoopVar(psDbHead, "dh", iNum, ",");
                    }

                    utPltPutLoopVar(psDbHead, "stime", iNum, utTimFormat("%Y-%m-%d %H:%M:%S", lStarttime));

                    utPltPutLoopVar(psDbHead, "address", iNum, caAddress);
                    utPltPutLoopVar(psDbHead, "apname", iNum, caApname);
                    utPltPutLoopVar(psDbHead, "mac", iNum, caMac);
                    utPltPutLoopVar(psDbHead, "apmac", iNum, caApmac);
                    utPltPutLoopVar(psDbHead, "servicename", iNum, caServicename);
                    utPltPutLoopVar(psDbHead, "channel", iNum, caChannel);
                    utPltPutLoopVar(psDbHead, "rssi", iNum, caRssi);
                    //printf("caSecurity=%s\n",caSecurity);
                    utPltPutLoopVar(psDbHead, "security", iNum, caSecurity);
                    utPltPutLoopVar(psDbHead, "longitude", iNum, caLongitude);
                    utPltPutLoopVar(psDbHead, "latitude", iNum, caLatitude);
                    utPltPutLoopVar(psDbHead, "ssid", iNum, caSsid);
                    utPltPutLoopVar(psDbHead, "servicecode", iNum, caServicecode);
                    utPltPutLoopVar(psDbHead, "termtype", iNum, caTermtype);
                    utPltPutLoopVar(psDbHead, "vname", iNum, caVname);
                    utPltPutLoopVarF(psDbHead, "vtype", iNum, "%d", lVtype);

                    utPltPutLoopVarF(psDbHead, "dispname", iNum, "%s", caDispname);
                    utPltPutLoopVarF(psDbHead, "factoryname", iNum, "%s", caFactoryname);
                    utPltPutLoopVarF(psDbHead, "factorycode", iNum, "%s", caFactorycode);
                }

                memset(caServicecode, 0, sizeof(caServicecode));
                memset(caServicename, 0, sizeof(caServicename));
                memset(caAddress, 0, sizeof(caAddress));
                memset(caMac, 0, sizeof(caMac));
                memset(caApname, 0, sizeof(caApname));
                memset(caApmac, 0, sizeof(caApmac));
                memset(caSsid, 0, sizeof(caSsid));
                lStarttime = 0;
                memset(caChannel, 0, sizeof(caChannel));
                memset(caRssi, 0, sizeof(caRssi));
                memset(caSecurity, 0, sizeof(caSecurity));
                memset(caLongitude, 0, sizeof(caLongitude));
                memset(caLatitude, 0, sizeof(caLatitude));
                memset(caTermtype, 0, sizeof(caTermtype));
                memset(caVname, 0, sizeof(caVname));

                memset(caDispname, 0, sizeof(caDispname));
                memset(caFactoryname, 0, sizeof(caFactoryname));
                memset(caFactorycode, 0, sizeof(caFactorycode));

                lVtype = 0;
                iret = pasDbFetchInto(psCur,
                                      UT_TYPE_STRING, 14, caServicecode,
                                      UT_TYPE_STRING, 63, caServicename,
                                      UT_TYPE_STRING, 63, caAddress,
                                      UT_TYPE_STRING, 18, caMac,
                                      UT_TYPE_STRING, 31, caApname,
                                      UT_TYPE_STRING, 18, caApmac,
                                      UT_TYPE_STRING, 63, caSsid,
                                      UT_TYPE_LONG, 4, &lStarttime,
                                      UT_TYPE_STRING, 2, caChannel,
                                      UT_TYPE_STRING, 8, caRssi,
                                      UT_TYPE_STRING, 2, caSecurity,
                                      UT_TYPE_STRING, 11, caLongitude,
                                      UT_TYPE_STRING, 11, caLatitude,
                                      UT_TYPE_STRING, 100, caTermtype,
                                      UT_TYPE_LONG, 4, &lVtype,
                                      UT_TYPE_STRING, 63, caVname,
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
        utPltFileDownload(iFd, "application/text", caPath, caFilename, caFilename);
        remove(caFile);
        return 0;
    }

    utPltOutToHtml(iFd, psMsgHead, psDbHead, "rzcx/proauth_hotinfo_list.htm");
    return 0;
}

/* 移动采集设备下查看图表1：指定AP 频繁出现的终端次数(根据用户选择的时间点、采集设备名称或编号) */
int ncsMobileap_Chosenplace_TerminalmacTop(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead)
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

    if(strcmp(caTimeFlag, "null") == 0)     //查询今天：默认
    {
        snprintf(sqlTmp, sizeof(sqlTmp), " UNIX_TIMESTAMP(DATE_SUB(NOW(), INTERVAL 1 DAY)) ");
    }
    else if(strcmp(caTimeFlag, "1") == 0)   //查询今天：用户选择
    {
        snprintf(sqlTmp, sizeof(sqlTmp), " UNIX_TIMESTAMP(DATE_SUB(NOW(), INTERVAL 1 DAY)) ");
    }
    else if(strcmp(caTimeFlag, "2") == 0)   //最近三天
    {
        snprintf(sqlTmp, sizeof(sqlTmp), " UNIX_TIMESTAMP(DATE_SUB(NOW(), INTERVAL 3 DAY)) ");
    }
    else if(strcmp(caTimeFlag, "3") == 0)   //最近一周
    {
        snprintf(sqlTmp, sizeof(sqlTmp), " UNIX_TIMESTAMP(DATE_SUB(NOW(), INTERVAL 7 DAY)) ");
    }
    else if(strcmp(caTimeFlag, "4") == 0)   //最近一月
    {
        snprintf(sqlTmp, sizeof(sqlTmp), " UNIX_TIMESTAMP(DATE_SUB(NOW(), INTERVAL 1 MONTH)) ");
    }
    else if(strcmp(caTimeFlag, "5") == 0)   //最近三月
    {
        snprintf(sqlTmp, sizeof(sqlTmp), " UNIX_TIMESTAMP(DATE_SUB(NOW(), INTERVAL 3 MONTH)) ");
    }
    else if(strcmp(caTimeFlag, "6") == 0)   //最近半年
    {
        snprintf(sqlTmp, sizeof(sqlTmp), " UNIX_TIMESTAMP(DATE_SUB(NOW(), INTERVAL 6 MONTH)) ");
    }
    else    //最近半年
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
                                          UT_TYPE_LONG,  20, &dwCount)) || 1405 == nRet)
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
int ncsMobileap_Chosenplace_PeopleCrowedTime(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead)
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

    if(strcmp(caTimeFlag, "null") == 0)     //查询今天：默认
    {
        snprintf(sqlTmp, sizeof(sqlTmp), " UNIX_TIMESTAMP(DATE_SUB(NOW(), INTERVAL 1 DAY)) ");
    }
    else if(strcmp(caTimeFlag, "1") == 0)   //查询今天：用户选择
    {
        snprintf(sqlTmp, sizeof(sqlTmp), " UNIX_TIMESTAMP(DATE_SUB(NOW(), INTERVAL 1 DAY)) ");
    }
    else if(strcmp(caTimeFlag, "2") == 0)   //最近三天
    {
        snprintf(sqlTmp, sizeof(sqlTmp), " UNIX_TIMESTAMP(DATE_SUB(NOW(), INTERVAL 3 DAY)) ");
    }
    else if(strcmp(caTimeFlag, "3") == 0)   //最近一周
    {
        snprintf(sqlTmp, sizeof(sqlTmp), " UNIX_TIMESTAMP(DATE_SUB(NOW(), INTERVAL 7 DAY)) ");
    }
    else if(strcmp(caTimeFlag, "4") == 0)   //最近一月
    {
        snprintf(sqlTmp, sizeof(sqlTmp), " UNIX_TIMESTAMP(DATE_SUB(NOW(), INTERVAL 1 MONTH)) ");
    }
    else if(strcmp(caTimeFlag, "5") == 0)   //最近三月
    {
        snprintf(sqlTmp, sizeof(sqlTmp), " UNIX_TIMESTAMP(DATE_SUB(NOW(), INTERVAL 3 MONTH)) ");
    }
    else if(strcmp(caTimeFlag, "6") == 0)   //最近半年
    {
        snprintf(sqlTmp, sizeof(sqlTmp), " UNIX_TIMESTAMP(DATE_SUB(NOW(), INTERVAL 6 MONTH)) ");
    }
    else    //最近半年
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
                                          UT_TYPE_LONG,  20, &dwCount,
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

    // 左侧场所选择 groupid、username   // 二次查询 mac 条件
    char caGroupId[32] = "";
    char caUsernamesByID[1024] = "";
    char caUsername[32] = "";

    char caMacPage[32] = "", caApmacPage[32] = "";

    /* 关键字查询 */
    char caKeyword[64] = "", caTemp_d[64] = "", caTemp[1024] = "";

    /* 选择导出的话 */
    char caExport[10] = "";
    long lRet = 0;
    lRet = utMsgGetSomeNVar(psMsgHead, 4,
                            "currentexport", UT_TYPE_STRING, 9, caExport,
                            "groupid", UT_TYPE_STRING, 31, caGroupId,
                            "username", UT_TYPE_STRING, 31, caUsername,
                            "keyword", UT_TYPE_STRING, 63, caKeyword);

    //编码转换：转换成GBK，后台通用字符集
    if(strlen(caKeyword) > 0)
    {
        strcpy(caTemp_d, pasCvtGBK(2, caKeyword, caTemp, 60));
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
        sprintf(caFilename, "currentInfoLog.csv");
        sprintf(caPath, "%s", "/home/ncmysql/ncs");
        sprintf(caFile, "%s/%s", caPath, caFilename);
        fp = fopen(caFile, "w");
        if(fp == NULL)
        {
            printf("数据导出出错\n\n");
            return 0;
        }
        fprintf(fp, "实时信息\n");  //标题
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

    //最早采集场强、最近采集场强    //终端MAC、采集设备MAC
    long lRssi = 0, lUrssi = 0;
    char caMac[32] = "", caApmac[32] = "";

    //采集设备经度、纬度    //热点ssid
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
    char caUserAuthJudge[20] = "";
    long iUserAuth = 0;                     //用户权限判断

    strcpy(caAuthorityGroupcodes, (const char*)getDsGroupcode());
    printf("用户权限下 caAuthorityGroupcodes = %s\n", caAuthorityGroupcodes);

    char caKeywordJudge[30] = "";           //判断输入框内容有无 '@' 字符

    stCurInfoForty pStCurrentForty[40];     //统计40条或以下实时信息并按时间排序返回前台
    memset(pStCurrentForty, 0, sizeof(stCurInfoForty) * 40);

    //如果选择左侧 groupid 获取当前groupid下所有username到 caUsernamesByID
    if(strlen(caGroupId) > 0)
    {
        //可以获取多级菜单下所有username
        snprintf(caUsernamesByID, sizeof(caUsernamesByID) - 1, "%s", myGetGroupCodeByGroupid(psShmHead, caGroupId));
    }

    ncLbsMacLocation *psMacLoc;
    pHash = (unsigned char *)utShmHashHead(psShmHead, NC_LNK_MACLOCATION);
    if(pHash)
    {
        psMacLoc = (ncLbsMacLocation *)pasHashFirst(pHash, &sHashInfo);
        //printf("pHash = %p\n", pHash);
        //printf("psMacLoc = %p\n", psMacLoc);
        while(psMacLoc)
        {
            psAp = (ncApSrvOnline *)ncMacLogGetOnlineApByMac(psShmHead, psMacLoc->apmac);
            if(psAp)
            {
                strcpy(caServicename, psAp->servicename);   //场所名称
                strcpy(caDispname, psAp->dispname);         //采集设备名称
                strcpy(caStime, utTimFormat("%Y-%m-%d %H:%M:%S", psMacLoc->starttime)); //采集时间

                strcpy(caMac, (const char*)pasCvtMacStr(psMacLoc->mac, caMacStr));      //终端MAC
                stpcpy(caLongitude, psMacLoc->longitude);
                stpcpy(caLatitude, psMacLoc->latitude);
                stpcpy(caSsid, psMacLoc->ssid);
                strcpy(caApmac, (const char*)pasCvtMacStr(psMacLoc->apmac, caMacStr1)); //采集设备MAC
                stpcpy(caVname, psMacLoc->name);
                stpcpy(caTermtype, psMacLoc->termtype);
                strcpy(caServiceCode, psAp->servicecode);   //场所编号
                strcpy(caApname, psAp->apname);             //采集设备编号


                if((strlen(caAuthorityGroupcodes) > 0))     //非admin用户，获取用户权限下管理区域不为空
                {
                    if(strlen(caServiceCode) > 0)
                    {
                        memset(caUserAuthJudge, 0, sizeof(caUserAuthJudge));
                        strncpy(caUserAuthJudge, caServiceCode, 6);

                        //iUserAuth = isInGroupCode(caUserAuthJudge, caAuthorityGroupcodes);
                        char *p = strstr(caAuthorityGroupcodes, caUserAuthJudge);
                        if(!p)                  //权限不在指定区域
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
                            iNumControl = iNum;
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
                                      UT_TYPE_LONG,   psMacLoc->encrypt,
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
                            iNumControl++;      //遍历当前场所下实时信息个数
                            if(strcmp(caMac, "00:00:00:00:00:00") != 0)
                            {
                                iMacCount++;    //遍历当前场所下有效实时信息个数，绘制跑马表数据
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
                            /* 如果caKeyword第一个字符为 '@' */
                            strncpy(caKeywordJudge, caKeyword, 1);
                            if(strcmp(caKeywordJudge, "@") == 0)
                            {
                                if((strlen(caGroupId) > 0))
                                {
                                    //获取当前场所编号前6位(groupcode)与groupid下所有groupcode比较
                                    memset(caSplitServiceCode, 0, sizeof(caSplitServiceCode));
                                    strncpy(caSplitServiceCode, caServiceCode, 6);
                                    char *p = strstr(caUsernamesByID, caSplitServiceCode);
                                    if(p)
                                    {
                                        iNumControl++;      //遍历当前场所下实时信息个数
                                        if(strcmp(caMac, "00:00:00:00:00:00") != 0)
                                        {
                                            iMacCount++;    //遍历当前场所下有效实时信息个数，绘制跑马表数据
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
                                        iNumControl++;      //遍历当前场所下实时信息个数
                                        if(strcmp(caMac, "00:00:00:00:00:00") != 0)
                                        {
                                            iMacCount++;    //遍历当前场所下有效实时信息个数，绘制跑马表数据
                                        }
                                        //将符合需要的40条数据输出到缓冲区
                                        if(iNumControl <= 40)
                                        {
                                            iNum++;
                                            InsertItemToStuct(pStCurrentForty, iNum - 1, psAp, psMacLoc);
                                        }
                                    }
                                }

                                /* 判断输入框caKeyword 是否有第二个'@'  字符: 如果有，则取其后面值作为搜索条件 */
                                char *pKeywordServiceName = NULL, *pKeywordMac = NULL;
                                pKeywordServiceName = strtok_r(caKeyword, "@", &pKeywordMac);
                                if(strcmp(pKeywordMac, "") != 0)
                                {
                                    if(strstr(caServicename, caKeyword) || strstr(caMac, pKeywordMac) || strstr(caApmac, pKeywordMac))
                                    {
                                        iNumControl++;      //遍历当前场所下实时信息个数
                                        if(strcmp(caMac, "00:00:00:00:00:00") != 0)
                                        {
                                            iMacCount++;    //遍历当前场所下有效实时信息个数，绘制跑马表数据
                                        }
                                        //将符合需要的40条数据输出到缓冲区
                                        if(iNumControl <= 40)
                                        {
                                            iNum++;
                                            InsertItemToStuct(pStCurrentForty, iNum - 1, psAp, psMacLoc);
                                        }
                                        if(iNumControl <= 5)
                                        {
                                            printf("有'@'字符用户关键字搜索\n");
                                            printf("pKeywordServiceName = %s\n", pKeywordServiceName);
                                            printf("pKeywordMac = %s, caMac = %s, caApmac = %s\n\n", pKeywordMac, caMac, caApmac);
                                        }
                                    }
                                }
                            }
                            else    //用户输入关键字搜索
                            {
                                /*if(iNumControl <= 5)
                                {
                                    printf("用户输入关键字搜索\n");
                                    printf("caKeyword = %s, caServicename = %s, caMac = %s, caApmac = %s\n\n", caKeyword, caServicename, caMac, caApmac);
                                } */

                                if(strstr(caServicename, caKeyword) || strstr(caMac, caKeyword) || strstr(caApmac, caKeyword) || (strcmp(caKeyword, caServicename) == 0) || (strcmp(caKeyword, caMac) == 0) || (strcmp(caKeyword, caApmac) == 0))
                                {
                                    iNumControl++;      //遍历当前场所下实时信息个数
                                    if(strcmp(caMac, "00:00:00:00:00:00") != 0)
                                    {
                                        iMacCount++;    //遍历当前场所下有效实时信息个数，绘制跑马表数据
                                    }
                                    //将符合需要的40条数据输出到缓冲区
                                    //if(iNumControl <= 40)
                                    if(iNumControl <= 40)
                                    {
                                        iNum++;
                                        InsertItemToStuct(pStCurrentForty, iNum - 1, psAp, psMacLoc);
                                    }
                                    if(iNumControl <= 5)
                                    {
                                        printf("用户关键字搜索\n");
                                        printf("caKeyword = %s, caServicename = %s, caMac = %s, caApmac = %s\n\n", caKeyword, caServicename, caMac, caApmac);
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
                if(i > 0)
                {
                    utPltPutLoopVar(psDbHead, "dh", i + 1, ",");
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
            printf("输入框非空时查询结果输出到 JSON\n\n");

            /* 如果caKeyword第一个字符为 '@' */
            strncpy(caKeywordJudge, caKeyword, 1);
            if(strcmp(caKeywordJudge, "@") == 0)
            {

                if((strlen(caGroupId) > 0))
                {
                    printf("输入框非空时 caGroupId 大于0 输出 JSON\n\n");
                    printf("caUsername = %s, caServiceCode = %s\n", caUsername, caServiceCode);
                    //依据时间排序并保存回 pStCurrentForty
                    choiseSort(pStCurrentForty, iNum);
                    int i;
                    for(i = 0; i < iNum; i++)
                    {
                        if(i > 0)
                        {
                            utPltPutLoopVar(psDbHead, "dh", i + 1, ",");
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
                            if(i > 0)
                            {
                                utPltPutLoopVar(psDbHead, "dh", i + 1, ",");
                            }

                            //将符合需要的40条数据输出到缓冲区
                            outPutJson_current(psDbHead, pStCurrentForty, i);
                        }
                        printf("当前指定场所 caUsername = %s 实时数量为 = %d, 有效数量为 = %d\n\n", caUsername, iNumControl, iMacCount);
                    }
                }
            }
            else
            {
                printf("用户关键字查询：\n\n");
                int i;
                for(i = 0; i < iNum; i++)
                {
                    if(strstr(pStCurrentForty[i].caServicename, caKeyword) || strstr(pStCurrentForty[i].caMac, caKeyword) || strstr(pStCurrentForty[i].caApmac, caKeyword) || (strcmp(caKeyword, pStCurrentForty[i].caServicename) == 0) || (strcmp(caKeyword, pStCurrentForty[i].caMac) == 0) || (strcmp(caKeyword, pStCurrentForty[i].caApmac) == 0))
                    {
                        if(i > 0)
                        {
                            utPltPutLoopVar(psDbHead, "dh", i + 1, ",");
                        }

                        //将符合需要的40条数据输出到缓冲区
                        outPutJson_current(psDbHead, pStCurrentForty, i);

                        if(i <= 5)
                        {
                            printf("用户关键字查询时输出结果 iNum = %d, i = %d\n\n", iNum, i);
                        }
                    }
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
        utPltFileDownload(iFd, "application/text", caPath, caFilename, caFilename);
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
    /* MAC日志查询：list 中上网认证账号功能：查询vname相关的mac下的vname、vdisp */
    pasSetTcpFunName("ncsMacLog_ChosenMac_NetAuthID", ncsMacLog_ChosenMac_NetAuthID, 0);
    /* MAC日志查询：点击工具条 "场所选择"，返回前台树状菜单 */
    pasSetTcpFunName("ncsMacLog_ChosenMac_ChoseQueryPlace", ncsMacLog_ChosenMac_ChoseQueryPlace, 0);


    /* 热点查询 */
    pasSetTcpFunName("ncsHotSpot_infolist_v4", ncsHotSpot_infolist_v4, 0);
    //标记合规、非合规
    pasSetTcpFunName("ncsHotSpot_MarkRuleorNot", ncsHotSpot_MarkRuleorNot, 0);
    pasUtlMac2VendorInit("ncoui.dat", 30000);




    /* 场所信息查询 */
    pasSetTcpFunName("ncsPlace_mactermlist_v4", ncsPlace_mactermlist_v4, 0);
    //查看图表1：场所频繁出现的终端TOP10统计(根据用户选择的时间点、采集设备名称或编号)
    pasSetTcpFunName("ncsPlace_Chosenplace_TerminalmacTop", ncsPlace_Chosenplace_TerminalmacTop, 0);
    //查看图表2：指定场所的人流密集时段(根据用户选择的时间点、场所名称或编号)
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







