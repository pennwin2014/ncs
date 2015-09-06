
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/time.h>
#include <utime.h>
#include "utoall.h"
#include "utoplt01.h"
#include "pasdb.h"
#include <pcap.h>
#include "ncdef.h"
#include "dsutl.h"
#include "dsclt.h"
#include "ncsdef.h"
#include <errno.h>

//#define debug VER_DEBUG
#define ADV_SEARCH "adv"

extern char *pHashService;


char * getService_GA(long lService)
{
    static char caReturn[100];
    memset(caReturn, 0, sizeof(caReturn));
    switch(lService)
    {
        case 1:
            strcpy(caReturn, "HTTP");
            break;
        case 2:
            strcpy(caReturn, "WAP");
            break;
        case 3:
            strcpy(caReturn, "SMTP");
            break;
        case 4:
            strcpy(caReturn, "POP3");
            break;
        case 5:
            strcpy(caReturn, "IMAP");
            break;
        case 6:
            strcpy(caReturn, "NNTP");
            break;
        case 7:
            strcpy(caReturn, "FTP");
            break;
        case 8:
            strcpy(caReturn, "SFTP");
            break;
        case 9:
            strcpy(caReturn, "TELNET");
            break;
        case 10:
            strcpy(caReturn, "HTTPS");
            break;
        case 11:
            strcpy(caReturn, "RSTP");
            break;
        case 12:
            strcpy(caReturn, "MMS");
            break;
        case 13:
            strcpy(caReturn, "WEP");
            break;
        case 14:
            strcpy(caReturn, "WPA");
            break;
        case 15:
            strcpy(caReturn, "PPTP");
            break;
        case 16:
            strcpy(caReturn, "L2TP");
            break;
        case 17:
            strcpy(caReturn, "SOCKS");
            break;
        case 18:
            strcpy(caReturn, "Compo");
            break;
        default:
            strcpy(caReturn, "");
            break;
    }

    return &caReturn[0];
}



static char **ncsUtlGetTable_t(unsigned long lSTime, unsigned long lTime, int days,
                               char *prefix, unsigned long *lStartTime, int *lCount)
{
    char sdate[33] = "";
    strcpy(sdate, utTimFormat("%Y-%m-%d", lTime));//当前日期
    strcat(sdate, " 00:00:00");
    if(lSTime)
    {
        *lStartTime = lSTime;
    }
    else
    {
        *lStartTime = utTimStrToLong("%Y/%m/%d %H:%M:%S", sdate) - (days - 1) * 86400;
    }
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
    if(NULL != tmp)
    {
        int j = 0;
        for(i = iMin; i <= iMax; i++)
        {
            tmp[j] = (char *)malloc(strLen);
            if(NULL != tmp[j])
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
    }// if (NULL != tmp) {

    return tmp;
}


// 显示验证日志

int proauthRzauthlist(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead)
{
    char caExport[32];
    FILE *fp;
    char caFilename[32], caFile[128], caPath[128];
    int iTime_flag = 0, days = 0, iret = 0, _break = 0, i = 0, lCount = 0;
    int num = 0, iNum = 0;

    unsigned long did = 0, iStart = 0, iLimit = 0, lCount1 = 0, lStartTime = 0;
    unsigned long lTime = time(0), lTotRec = 0;

    char sdate[11] = "", edate[11] = "", start[17] = "";
    char limit[17] = "", sort[17] = "", dir[9] = "";
    long lCurPg, lRowNum, lStartRec, lSstarttime;
    char sql[4096] = "", caTotPg[16], caCurPg[16];
    char caPage[12], caLimit[16], caSort[16], caDir[16];
    char caKeyword[64], caTime_flag[32], sql2[4096] = "";
    char frmfile[256] = "", *tmpgbk = NULL;
    char **ptmp = NULL, Atype[8];
    long lStatus, lStime, lEtime, lClientIp, lAPIp, lSPort, lEPort, lFlag;
    long long lSid;
    char caDid[20], caUsername[34], caMac[24], caIp[32], caServicecode[34];
    char clientMac[24], longitude[16], latitude[16], rssi[10], strSid[38];
    char caIdtype[12], caIdno[20], caFcode[20], caDispname[66], APName[20];
    char appId[34], clientIp[32];
    char * oprations[3] = {"", "上线", "下线"};

    struct sttable
    {
        char tname[32];
        unsigned long lCount;
    };
    struct sttable tableinfo[120];//10年？
    pasDbCursor *psCur = NULL;
    char caGroups[1024], caFacs[1024], caArea[256], caName2[256];
    char caTemp[1024], strSqlSrvCode[256];
    long iReturn = 0, lCurPid = 0, lGroupId = 0;
    unsigned long lIp;
    char caStime[16], caEtime[16], caTemp_d[64];

    char caGroupId[34], caAccount[34], caServicename[66];
    char  caSDate[32], caEDate[32], caSearchType[8];
    char caStemptime[32], caEtemptime[32];
    unsigned long lSTime = 0, lETime = 0;
    char caoutIp[32], catermMac[24], caAPName[20], caAPMac[24], caoprFlag[8];
    int nOprFlag = 0;
    char strSqlSrvName[256], sqlServiceCodes[1024];
    typedef struct sddata_s
    {
        char serviceCode[34];
        char dispName[66];
    } S_SDDATA_S;
    char *pHash = NULL;
    struct sddata_s *psData = NULL;


    memset(caGroupId, 0, sizeof(caGroupId));
    sprintf(caTemp, "%d", getDsGroupid());
    strcpy(caGroups, caTemp);
    utPltDbHead *psDbHead = utPltInitDb();
    utMsgPrintMsg(psMsgHead);
    iReturn = utMsgGetSomeNVar(psMsgHead, 24,
                               "TotPg", UT_TYPE_STRING, 12, caTotPg,
                               "CurPg", UT_TYPE_STRING, 12, caCurPg,
                               "keyword", UT_TYPE_STRING, 60, caKeyword,
                               "did", UT_TYPE_STRING, 15, caDid,
                               "page", UT_TYPE_STRING, 10, caPage,
                               "limit", UT_TYPE_STRING, 10, caLimit,
                               "sort", UT_TYPE_STRING, 15, caSort,
                               "dir", UT_TYPE_STRING, 8,  caDir,
                               "groupid", UT_TYPE_STRING, 11, caGroupId,
                               "stime", UT_TYPE_STRING, 8, caStime,
                               "etime", UT_TYPE_STRING, 8, caEtime,
                               "servicecode", UT_TYPE_STRING, 32, caServicecode,
                               "account", UT_TYPE_STRING, 32, caAccount,
                               "servicename", UT_TYPE_STRING, 60, caServicename,
                               "sdate", UT_TYPE_STRING, 10, caSDate,
                               "edate", UT_TYPE_STRING, 10, caEDate,
                               "searchtype", UT_TYPE_STRING, 6, caSearchType,
                               "ip", UT_TYPE_STRING, 18, caIp,
                               "outIp", UT_TYPE_STRING, 18, caoutIp,
                               "termMac", UT_TYPE_STRING, 18, catermMac,
                               "APName", UT_TYPE_STRING, 18, caAPName,
                               "APMac", UT_TYPE_STRING, 18, caAPMac,
                               "oprFlag", UT_TYPE_STRING, 6, caoprFlag,
                               "time_flag", UT_TYPE_STRING, 8, caTime_flag);

#ifdef debug
    lCurPid = getpid();
    printf("proauthRzauthlist: lCurPid = %d\n", lCurPid);
    sleep(10);
#endif

    if(0 < strlen(caKeyword))
    {
        strcpy(caTemp_d, pasCvtGBK(2, caKeyword, caTemp, 60));
        strcpy(caKeyword, caTemp_d);
    }

    if(0 < strlen(caServicename))
    {
        strcpy(caTemp_d, pasCvtGBK(2, caServicename, caTemp, 60));
        strcpy(caServicename, caTemp_d);
    }

    lCurPg = atol(caPage);
    if(lCurPg <= 0)
    {
        lCurPg = 1;
    }
    if(0 < strlen(caLimit))
    {
        lRowNum = atol(caLimit);
    }
    else
    {
        lRowNum = 40; //每一页面行数
    }
    lStartRec = (lCurPg - 1) * lRowNum;

    memset(sql2, 0, sizeof(sql2));
    if(0 == strcmp(ADV_SEARCH, caSearchType))    //advanced search
    {

        //printf("pid=%d\n",getpid());
        //sleep(15);
        if(0 < strlen(caSDate))
        {
            sprintf(caStemptime, "%s ", caSDate);
            strncat(caStemptime, caStime, 8);
            lSTime = utTimStrToLong("%Y/%m/%d %H:%M:%S", caStemptime);
        }// if (0 < strlen(caSDate)){

        if(0 < strlen(caEDate))
        {
            sprintf(caEtemptime, "%s ", caEDate);
            strncat(caStemptime, caEtime, 8);
            lETime = utTimStrToLong("%Y/%m/%d %H:%M:%S", caEtemptime);
        }// if(0 < strlen(caEDate)){
        lETime =  lTime < lETime ? lTime : lETime;
        ptmp = ncsUtlGetTable_t(lSTime, lETime, 0, "nctermlog_if_",  &lStartTime, &lCount);
        lTime = lTime + 2 * 3600;
        printf("lStartTime=%d\n", lStartTime);

        snprintf(sql2, sizeof(sql), " where A.stime >= %lu and A.stime <= %lu ",
                 lStartTime, lTime);
        if(0 < strlen(caAccount))
        {
            snprintf(sql2 + strlen(sql2), sizeof(sql2) - strlen(sql2),
                     " and A.username like '%c%s%c' ", '%', caAccount, '%');
        }
        memset(sqlServiceCodes, 0, sizeof(sqlServiceCodes));
        if((0 < strlen(caServicename))  || (0 < strlen(caServicecode)))
        {
            memset(sql, 0, sizeof(sql));
            snprintf(sql, sizeof(sql), "select count(*) from ncsuser where 1 ");
            if(0 < strlen(caServicename))
            {
                snprintf(sqlServiceCodes + strlen(sqlServiceCodes),
                         sizeof(sqlServiceCodes) - strlen(sqlServiceCodes),
                         " and dispname like '%c%s%c' ", '%', caServicename, '%');
            }
            if(0 < strlen(caServicecode))
            {
                snprintf(sqlServiceCodes + strlen(sqlServiceCodes),
                         sizeof(sqlServiceCodes) - strlen(sqlServiceCodes),
                         " and username like '%c%s%c' ", '%', caServicecode, '%');
            }
            snprintf(sql + strlen(sql), sizeof(sql) - strlen(sql),
                     " %s ", sqlServiceCodes);

            iReturn = pasDbOneRecord(sql, 0, UT_TYPE_ULONG, 4, &lCount1);
            printf("__line = %d, count = %d ,sql = %s\n", __LINE__, lCount1, sql);
            if(10 < lCount1)
            {
                printf(" proauthRzauthlist: to many groups, please input \
                    extracter group name \n ");
                utPltPutVarF(psDbHead, "TotRec", "%lu", lTotRec);
                utPltOutToHtml(iFd, psMsgHead, psDbHead,
                               "rzcx/proauth_rzcx_authlog_list.htm");
                return 1;
            }
            else if(0 == lCount1)
            {
                printf(" proauthRzauthlist: no group, please input \
                    extracter group name \n ");
                utPltOutToHtml(iFd, psMsgHead, psDbHead,
                               "rzcx/proauth_rzcx_authlog_list.htm");
                return 1;
            }
            else
            {
                pHash = (unsigned char *)pasLHashInit(10, 10,
                                                      sizeof(struct sddata_s), 0, 32);
                if(NULL == pHash)
                {
                    utWebDispMsg(iFd, psMsgHead, "nc/ncmsg_back.htm",
                                 "分配内存出错", "分配内存出错");
                    return 0;
                }
                memset(sql, 0, sizeof(sql));
                snprintf(sql, sizeof(sql), "select groupname,dispname from \
                    ncsuser where 1 ");

                snprintf(sql + strlen(sql), sizeof(sql) - strlen(sql),
                         " %s ", sqlServiceCodes);
                psCur = pasDbOpenSql(sql, 0);
                memset(sqlServiceCodes, 0, sizeof(sqlServiceCodes));

                printf("__line = %d, sql = %s\n", __LINE__, sql);
                if(NULL != psCur)
                {
                    memset(caServicecode, 0, sizeof(caServicecode));
                    memset(caDispname, 0, sizeof(caDispname));
                    iret = pasDbFetchInto(psCur,
                                          UT_TYPE_STRING, 32, caServicecode,
                                          UT_TYPE_STRING, 64, caDispname);
                    snprintf(sqlServiceCodes, sizeof(sqlServiceCodes),
                             " A.servicecode in ( ");

                    iNum = 0;
                    while((0 == iret) || (1405 == iret))
                    {
                        iNum++;
                        if(1 < iNum)
                        {
                            snprintf(sqlServiceCodes + strlen(sqlServiceCodes),
                                     sizeof(sqlServiceCodes) - strlen(sqlServiceCodes),
                                     ", ");
                        }
                        snprintf(sqlServiceCodes + strlen(sqlServiceCodes),
                                 sizeof(sqlServiceCodes) - strlen(sqlServiceCodes),
                                 " '%s' ", caServicecode);
                        psData = (struct sddata_s *)pasLHashLookA(pHash,
                                                                  caServicecode);
                        if(psData)
                        {
                            strcpy(psData->serviceCode, caServicecode);
                            strcpy(psData->dispName, caDispname);
                        }

                        memset(caServicecode, 0, sizeof(caServicecode));
                        memset(caDispname, 0, sizeof(caDispname));
                        iret = pasDbFetchInto(psCur,
                                              UT_TYPE_STRING, 32, caServicecode,
                                              UT_TYPE_STRING, 64, caDispname);
                    }// while((0 == iret) || (1405 == iret)) {
                    snprintf(sqlServiceCodes + strlen(sqlServiceCodes),
                             sizeof(sqlServiceCodes) - strlen(sqlServiceCodes), " ) ");
                }// if (NULL != psCur) {
                snprintf(sql2 + strlen(sql2), sizeof(sql2) - strlen(sql2),
                         " and  %s ", sqlServiceCodes);
            }// if (10 < lCount1) {
        }// if ((0 < strlen(caServicename))  || (0 < strlen(caServicecode))) {

        if(0 < strlen(caIp))
        {
            snprintf(sql2 + strlen(sql2), sizeof(sql2) - strlen(sql2),
                     " and INET_NTOA(A.sip) like  '%c%s%c' ", '%', caIp, '%');
        }
        if(0 < strlen(caoutIp))
        {
            snprintf(sql2 + strlen(sql2), sizeof(sql2) - strlen(sql2),
                     " and INET_NTOA(A.oip) like  '%c%s%c' ", '%', caoutIp, '%');
        }
        if(0 < strlen(catermMac))
        {
            snprintf(sql2 + strlen(sql2), sizeof(sql2) - strlen(sql2),
                     " and A.mac like '%c%s%c' ", '%', catermMac, '%');
        }
        if(0 < strlen(caAPName))
        {
            snprintf(sql2 + strlen(sql2), sizeof(sql2) - strlen(sql2),
                     " and A.apname like '%c%s%c' ", '%', caAPName, '%');
        }
        if(0 < strlen(caAPMac))
        {
            snprintf(sql2 + strlen(sql2), sizeof(sql2) - strlen(sql2),
                     " and A.apmac like '%c%s%c' ", '%', caAPMac, '%');
        }
        if(0 < strlen(caoprFlag))
        {
            nOprFlag = atoi(caoprFlag);
            if(0 != nOprFlag)
            {
                snprintf(sql2 + strlen(sql2), sizeof(sql2) - strlen(sql2),
                         " and A.flags = %d ", nOprFlag);
            }
        }
        //公安要求只查一天的数据
        sprintf(sql2 + strlen(sql2), " and A.stime >= %lu ",
                time(0) - 3600 * 24);


    }
    else
    {
        iTime_flag = atoi(caTime_flag);
        printf("iTime_flag=%lu\n", iTime_flag);
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
        }//switch(iTime_flag) {

        //get service code by id
        memset(strSqlSrvCode, 0, sizeof(strSqlSrvCode));
        if(0 < strlen(caGroupId))
        {
            lGroupId = strtol(caGroupId, NULL, 10);
            if(ERANGE != lGroupId)
            {
                memset(sql, 0, sizeof(sql));
                snprintf(sql, sizeof(sql), "select username from ncsuser where \
                       groupid = %d ", lGroupId);
                memset(caGroupId, 0, sizeof(caGroupId));
                iReturn = pasDbOneRecord(sql, 0, UT_TYPE_STRING, 32, caGroupId);
                if(0 == iReturn)
                {
                    snprintf(strSqlSrvCode, sizeof(strSqlSrvCode),
                             " and A.servicecode = '%s' ", caGroupId);
                }
            }
        }// if (0 < strlen(caGroupId)) {


        printf("pid=%d\n", getpid());
        //      sleep(15);
        ptmp = ncsUtlGetTable_t(0, lTime, days, "nctermlog_if_",  &lStartTime, &lCount);
        lTime = lTime + 2 * 3600;
        printf("lStartTime=%d\n", lStartTime);

        snprintf(sql2, sizeof(sql), " where A.stime >= %lu and A.stime <= %lu ",
                 lStartTime, lTime);
        if(0 < strlen(caKeyword))
        {
            snprintf(sql2 + strlen(sql2), sizeof(sql2) - strlen(sql2),
                     " and (A.username like '%c%s%c' or A.servicecode like '%c%s%c' or \
                A.dispname like '%c%s%c' or A.mac like '%c%s%c' or \
                A.idno like '%c%s%c' or A.appid like '%c%s%c' )",
                     '%', caKeyword, '%', '%', caKeyword, '%', '%', caKeyword, '%',
                     '%', caKeyword, '%', '%', caKeyword, '%', '%', caKeyword, '%');
        }

        if(0 < strlen(caoprFlag))
        {
            nOprFlag = atoi(caoprFlag);
            if(0 != nOprFlag)
            {
                snprintf(sql2 + strlen(sql2), sizeof(sql2) - strlen(sql2),
                         " and A.flags = %d ", nOprFlag);
            }
        }


        //公安要求只查一天的数据
        sprintf(sql2 + strlen(sql2), " and A.stime >= %lu ",
                time(0) - 3600 * 24);


        strcat(sql2, strSqlSrvCode);
    }// if ( 0 == strcmp(ADV_SEARCH, caSearchType)) {//advanced search

    printf("sql2=%s\n", sql2);
    //loop couting record numbers in tables
    for(i = 0; i < lCount; i++)
    {
        lCount1 = 0;
        memset(sql, 0, sizeof(sql));
        snprintf(sql, sizeof(sql), "select count(*) from %s as A ", ptmp[i]);
        strcat(sql, sql2);
        printf("sql = %s\n", sql);
        pasDbOneRecord(sql, 0, UT_TYPE_ULONG, 4, &lCount1);
        if(0 < lCount1)
        {
            strcpy(tableinfo[num].tname, ptmp[i]);
            tableinfo[num].lCount = lCount1;
            num++;
        }
        free(ptmp[i]);
    }// for (i = 0; i < lCount; i++) {

    free(ptmp);

    for(i = 0; i < num; i++)
    {
        lTotRec += tableinfo[i].lCount;
    }
    utPltPutVarF(psDbHead, "TotRec", "%lu", lTotRec);

    for(i = num - 1; i >= 0; i--)
    {
        printf("table[%d] = %s, _lCount1[%d] = %lu\n", i, tableinfo[i].tname,
               i, tableinfo[i].lCount);
    }

    iNum = 0;
    lStartRec =  lTotRec <  lStartRec ?
                 (lTotRec / lRowNum) * lRowNum : lStartRec ;
    iStart = lStartRec;
    iLimit = lRowNum;

    //loop seeking in tables
    for(i = num - 1; i >= 0; i--)
    {
        memset(sql, 0, sizeof(sql));
        if(tableinfo[i].lCount <= iStart)
        {
            iStart -= tableinfo[i].lCount;
            continue;
        }
        snprintf(sql, sizeof(sql), "select A.authtype, A.username, \
            A.servicecode, A.stime, A.etime, INET_NTOA(A.sip), \
            INET_NTOA(A.oip), A.sport, A.eport, A.mac, A.apname, A.apmac, \
            A.longitude, A.latitude, A.rssi, A.sessionid, A.flags, \
            A.idtype, A.idno, A.appid ");
        memset(caTemp, 0, sizeof(caTemp));
        if(0 == strcmp(ADV_SEARCH, caSearchType)
           && ((0 < strlen(caServicename))
               || (0 < strlen(caServicecode))))  //advanced search
        {
            snprintf(sql + strlen(sql), sizeof(sql) - strlen(sql),
                     ",A.dispname  ");
        }
        else
        {
            snprintf(sql + strlen(sql), sizeof(sql) - strlen(sql),
                     ",B.dispname  ");
            sprintf(caTemp, " left join ncsuser as B \
                on (B.username=A.servicecode) ");
        }
        snprintf(sql + strlen(sql), sizeof(sql) - strlen(sql),
                 "from %s as A  ", tableinfo[i].tname);
        strcat(sql, caTemp);

        strcat(sql, sql2);
        snprintf(sql + strlen(sql), sizeof(sql) - strlen(sql),
                 " order by A.stime desc limit %lu, %lu", iStart,
                 iLimit < tableinfo[i].lCount ? iLimit : tableinfo[i].lCount);

        if((tableinfo[i].lCount > iStart)
           && (tableinfo[i].lCount < iStart + iLimit))
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
        if(NULL != psCur)
        {
            memset(Atype, 0, sizeof(Atype));
            memset(caUsername, 0, sizeof(caUsername));
            memset(caServicecode, 0, sizeof(caServicecode));
            lStime = lEtime = 0;
            memset(clientIp, 0, sizeof(clientIp));
            memset(caIp, 0, sizeof(caIp));
            lSPort = lEPort = 0;
            memset(clientMac, 0, sizeof(clientMac));
            memset(APName, 0, sizeof(APName));
            memset(caMac, 0, sizeof(caMac));
            memset(longitude, 0, sizeof(longitude));
            memset(latitude, 0, sizeof(latitude));
            memset(rssi, 0, sizeof(rssi));
            memset(strSid, 0, sizeof(strSid));
            lFlag = 0;
            memset(caIdtype, 0, sizeof(caIdtype));
            memset(caIdno, 0, sizeof(caIdno));
            memset(appId, 0, sizeof(appId));
            memset(caDispname, 0, sizeof(caDispname));

            iret = pasDbFetchInto(psCur,
                                  UT_TYPE_STRING, 7, Atype,
                                  UT_TYPE_STRING, 32, caUsername,
                                  UT_TYPE_STRING, 14, caServicecode,
                                  UT_TYPE_LONG, 4, &lStime,
                                  UT_TYPE_LONG, 4, &lEtime,
                                  UT_TYPE_STRING, 15, clientIp,
                                  UT_TYPE_STRING, 15, caIp,
                                  UT_TYPE_LONG, 4, &lSPort,
                                  UT_TYPE_LONG, 4, &lEPort,
                                  UT_TYPE_STRING, 18, clientMac,
                                  UT_TYPE_STRING, 18, APName,
                                  UT_TYPE_STRING, 18, caMac,
                                  UT_TYPE_STRING, 11, longitude,
                                  UT_TYPE_STRING, 11, latitude,
                                  UT_TYPE_STRING, 8, rssi,
                                  UT_TYPE_STRING, 36, strSid,
                                  UT_TYPE_LONG, 2, &lFlag,
                                  UT_TYPE_STRING, 3, caIdtype,
                                  UT_TYPE_STRING, 18, caIdno,
                                  UT_TYPE_STRING, 32, appId,
                                  UT_TYPE_STRING, 64, caDispname);

            while((0 == iret) || (1405 == iret))
            {
                iNum++;
                if(1 < iNum)
                {
                    utPltPutLoopVar(psDbHead, "dh", iNum, ", ");
                }

                utPltPutLoopVarF(psDbHead, "atype", iNum, "%s", Atype);
                utPltPutLoopVar(psDbHead, "authAccount", iNum, caUsername);
                utPltPutLoopVar(psDbHead, "servicecode", iNum, caServicecode);
                utPltPutLoopVar(psDbHead, "stime", iNum,
                                utTimFormat("%Y/%m/%d %H:%M", lStime));
                sprintf(caTemp, "%s", 2 == lFlag ?
                        utTimFormat("%Y/%m/%d %H:%M", lEtime) : "");
                utPltPutLoopVar(psDbHead, "etime", iNum, caTemp);

                if((0 == strcmp(ADV_SEARCH, caSearchType))
                   && ((0 < strlen(caServicename))
                       || (0 < strlen(caServicecode)))
                   && pHash)  //advanced search
                {
                    psData = (struct sddata_s *)pasLHashLookA(pHash,
                                                              caServicecode);
                    if(psData)
                    {
                        strcpy(caDispname, psData->dispName);
                    }
                }
                utPltPutLoopVar(psDbHead, "servicename", iNum, caDispname);
                utPltPutLoopVar(psDbHead, "sip", iNum, clientIp);
                utPltPutLoopVar(psDbHead, "oip", iNum, caIp);
                sprintf(caTemp, "%d - %d", lSPort, lEPort);
                utPltPutLoopVar(psDbHead, "sport", iNum, caTemp);
                utPltPutLoopVar(psDbHead, "mac", iNum, clientMac);
                utPltPutLoopVar(psDbHead, "apname", iNum, APName);
                utPltPutLoopVar(psDbHead, "apmac", iNum, caMac);
                utPltPutLoopVar(psDbHead, "longitude", iNum, longitude);
                utPltPutLoopVar(psDbHead, "latitude", iNum, latitude);
                utPltPutLoopVar(psDbHead, "rssi", iNum, rssi);
                utPltPutLoopVar(psDbHead, "sessionid", iNum, strSid);
                utPltPutLoopVar(psDbHead, "flags", iNum, oprations[lFlag]);

                // fetch next record
                memset(Atype, 0, sizeof(Atype));
                memset(caUsername, 0, sizeof(caUsername));
                memset(caServicecode, 0, sizeof(caServicecode));
                lStime = lEtime = 0;
                memset(clientIp, 0, sizeof(clientIp));
                memset(caIp, 0, sizeof(caIp));
                lSPort = lEPort = 0;
                memset(clientMac, 0, sizeof(clientMac));
                memset(APName, 0, sizeof(APName));
                memset(caMac, 0, sizeof(caMac));
                memset(longitude, 0, sizeof(longitude));
                memset(latitude, 0, sizeof(latitude));
                memset(rssi, 0, sizeof(rssi));
                memset(strSid, 0, sizeof(strSid));
                lFlag = 0;
                memset(caIdtype, 0, sizeof(caIdtype));
                memset(caIdno, 0, sizeof(caIdno));
                memset(appId, 0, sizeof(appId));
                memset(caDispname, 0, sizeof(caDispname));

                iret = pasDbFetchInto(psCur,
                                      UT_TYPE_STRING, 7, Atype,
                                      UT_TYPE_STRING, 32, caUsername,
                                      UT_TYPE_STRING, 14, caServicecode,
                                      UT_TYPE_LONG, 4, &lStime,
                                      UT_TYPE_LONG, 4, &lEtime,
                                      UT_TYPE_STRING, 15, clientIp,
                                      UT_TYPE_STRING, 15, caIp,
                                      UT_TYPE_LONG, 4, &lSPort,
                                      UT_TYPE_LONG, 4, &lEPort,
                                      UT_TYPE_STRING, 18, clientMac,
                                      UT_TYPE_STRING, 18, APName,
                                      UT_TYPE_STRING, 18, caMac,
                                      UT_TYPE_STRING, 11, longitude,
                                      UT_TYPE_STRING, 11, latitude,
                                      UT_TYPE_STRING, 8, rssi,
                                      UT_TYPE_STRING, 36, strSid,
                                      UT_TYPE_LONG, 2, &lFlag,
                                      UT_TYPE_STRING, 3, caIdtype,
                                      UT_TYPE_STRING, 18, caIdno,
                                      UT_TYPE_STRING, 32, appId,
                                      UT_TYPE_STRING, 64, caDispname);
            }//while((0 == iret) || (1405 == iret)) {

            pasDbCloseCursor(psCur);
        } //if (NULL != psCur) {

        if(1 == _break)
        {
            break;
        }
    }// for (i = num - 1; i >= 0; i--)
    if(pHash)
    {
        free(pHash);
    }
    utPltOutToHtml(iFd, psMsgHead, psDbHead,
                   "rzcx/proauth_rzcx_authlog_list.htm");

    return 0;
}


int proauthIPLogList(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead)
{
    char caExport[32];
    FILE *fp;
    ncsNettype *psData2;
    char caFilename[32], caFile[128], caPath[128];
    int iTime_flag = 0, days = 0, iret = 0, _break = 0, i = 0, lCount = 0;
    int num = 0, iNum = 0;

    unsigned long did = 0, iStart = 0, iLimit = 0, lCount1 = 0, lStartTime = 0;
    unsigned long lTime = time(0), lTotRec = 0;

    char sdate[11] = "", edate[11] = "", start[17] = "";
    char limit[17] = "", sort[17] = "", dir[9] = "";
    long lCurPg, lRowNum, lStartRec, lSstarttime;
    char sql[4096] = "", caTotPg[16], caCurPg[16];
    char caPage[12], caLimit[16], caSort[16], caDir[16];
    char caKeyword[64], caTime_flag[32], sql2[4096] = "";
    char frmfile[256] = "", *tmpgbk = NULL;
    char **ptmp = NULL, Atype[8];
    long lStatus, lStime, lEtime, lClientIp, lAPIp, lSPort, lEPort, lFlag;
    long long lSid;
    char caDid[20], caUsername[34], caMac[24], caIp[32], caServicecode[34];
    char clientMac[24], longitude[16], latitude[16], rssi[10], strSid[38];
    char caIdtype[12], caIdno[20], caFcode[20], caDispname[66], APName[20];
    char appId[34], clientIp[32], dip[32];
    char * oprations[3] = {"", "上线", "下线"};

    struct sttable
    {
        char tname[32];
        unsigned long lCount;
    };
    struct sttable tableinfo[120];//10年？
    pasDbCursor *psCur = NULL;
    char caGroups[1024], caFacs[1024], caArea[256], caName2[256];
    char caTemp[1024], strSqlSrvCode[256];
    long iReturn = 0, lCurPid = 0, lGroupId = 0;
    unsigned long lIp;
    char caStime[16], caEtime[16], caTemp_d[64];
    long lClientPort, lNetType, lDPort;

    char caGroupId[34], caAccount[34], caServicename[66];
    char  caSDate[32], caEDate[32], caSearchType[8];
    char caStemptime[32], caEtemptime[32];
    unsigned long lSTime = 0, lETime = 0;
    char caoutIp[32], catermMac[24], caAPName[20], caAPMac[24], caoprFlag[8];
    int nOprFlag = 0;
    char strSqlSrvName[256], sqlServiceCodes[1024];
    typedef struct sddata_s
    {
        char serviceCode[34];
        char dispName[66];
    } S_SDDATA_S;
    char *pHash = NULL;
    struct sddata_s *psData = NULL;

    memset(caGroupId, 0, sizeof(caGroupId));
    sprintf(caTemp, "%d", getDsGroupid());
    strcpy(caGroups, caTemp);
    utPltDbHead *psDbHead = utPltInitDb();
    utMsgPrintMsg(psMsgHead);
    iReturn = utMsgGetSomeNVar(psMsgHead, 24,
                               "TotPg", UT_TYPE_STRING, 12, caTotPg,
                               "CurPg", UT_TYPE_STRING, 12, caCurPg,
                               "keyword", UT_TYPE_STRING, 60, caKeyword,
                               "did", UT_TYPE_STRING, 15, caDid,
                               "page", UT_TYPE_STRING, 10, caPage,
                               "limit", UT_TYPE_STRING, 10, caLimit,
                               "sort", UT_TYPE_STRING, 15, caSort,
                               "dir", UT_TYPE_STRING, 8,  caDir,
                               "groupid", UT_TYPE_STRING, 11, caGroupId,
                               "stime", UT_TYPE_STRING, 8, caStime,
                               "etime", UT_TYPE_STRING, 8, caEtime,
                               "servicecode", UT_TYPE_STRING, 32, caServicecode,
                               "account", UT_TYPE_STRING, 32, caAccount,
                               "servicename", UT_TYPE_STRING, 60, caServicename,
                               "sdate", UT_TYPE_STRING, 10, caSDate,
                               "edate", UT_TYPE_STRING, 10, caEDate,
                               "searchtype", UT_TYPE_STRING, 6, caSearchType,
                               "ip", UT_TYPE_STRING, 18, caIp,
                               "outIp", UT_TYPE_STRING, 18, caoutIp,
                               "termMac", UT_TYPE_STRING, 18, catermMac,
                               "APName", UT_TYPE_STRING, 18, caAPName,
                               "APMac", UT_TYPE_STRING, 18, caAPMac,
                               "oprFlag", UT_TYPE_STRING, 6, caoprFlag,
                               "time_flag", UT_TYPE_STRING, 8, caTime_flag);
#ifdef debug
    lCurPid = getpid();
    printf("proauthRzauthlist: lCurPid = %d\n", lCurPid);
    sleep(10);
#endif

    if(0 < strlen(caKeyword))
    {
        strcpy(caTemp_d, pasCvtGBK(2, caKeyword, caTemp, 60));
        strcpy(caKeyword, caTemp_d);
    }
    if(0 < strlen(caServicename))
    {
        strcpy(caTemp_d, pasCvtGBK(2, caServicename, caTemp, 60));
        strcpy(caServicename, caTemp_d);
    }
    lCurPg = atol(caPage);
    if(lCurPg <= 0)
    {
        lCurPg = 1;
    }
    if(0 < strlen(caLimit))
    {
        lRowNum = atol(caLimit);
    }
    else
    {
        lRowNum = 40; //每一页面行数
    }
    lStartRec = (lCurPg - 1) * lRowNum;

    memset(sql2, 0, sizeof(sql2));
    if(0 == strcmp(ADV_SEARCH, caSearchType))    //advanced search
    {
        if(0 < strlen(caSDate))
        {
            sprintf(caStemptime, "%s ", caSDate);
            strncat(caStemptime, caStime, 8);
            lSTime = utTimStrToLong("%Y/%m/%d %H:%M:%S", caStemptime);
        }// if (0 < strlen(caSDate)){

        if(0 < strlen(caEDate))
        {
            sprintf(caEtemptime, "%s ", caEDate);
            strncat(caStemptime, caEtime, 8);
            lETime = utTimStrToLong("%Y/%m/%d %H:%M:%S", caEtemptime);
        }// if(0 < strlen(caEDate)){
        lETime =  lTime < lETime ? lTime : lETime;
        ptmp = ncsUtlGetTable_t(lSTime, lETime, 0, "nctermiplog_if_",  &lStartTime, &lCount);
        lTime = lTime + 2 * 3600;
        printf("lStartTime=%d\n", lStartTime);

        snprintf(sql2, sizeof(sql2), " where A.stime >= %lu and A.stime <= %lu ",
                 lStartTime, lTime);
        if(0 < strlen(caAccount))
        {
            snprintf(sql2 + strlen(sql2), sizeof(sql2) - strlen(sql2),
                     " and A.username like '%c%s%c' ", '%', caAccount, '%');
        }
        memset(sqlServiceCodes, 0, sizeof(sqlServiceCodes));
        if((0 < strlen(caServicename))  || (0 < strlen(caServicecode)))
        {
            memset(sql, 0, sizeof(sql));
            snprintf(sql, sizeof(sql), "select count(*) from ncsuser where 1 ");
            if(0 < strlen(caServicename))
            {
                snprintf(sqlServiceCodes + strlen(sqlServiceCodes),
                         sizeof(sqlServiceCodes) - strlen(sqlServiceCodes),
                         " and dispname like '%c%s%c' ", '%', caServicename, '%');
            }
            if(0 < strlen(caServicecode))
            {
                snprintf(sqlServiceCodes + strlen(sqlServiceCodes),
                         sizeof(sqlServiceCodes) - strlen(sqlServiceCodes),
                         " and username like '%c%s%c' ", '%', caServicecode, '%');
            }
            snprintf(sql + strlen(sql), sizeof(sql) - strlen(sql),
                     " %s ", sqlServiceCodes);

            iReturn = pasDbOneRecord(sql, 0, UT_TYPE_ULONG, 4, &lCount1);
            printf("__line = %d, count = %d ,sql = %s\n", __LINE__, lCount1, sql);
            if(10 < lCount1)
            {
                printf(" proauthRzauthlist: to many groups, please input \
                    extracter group name \n ");
                utPltPutVarF(psDbHead, "TotRec", "%lu", lTotRec);
                utPltOutToHtml(iFd, psMsgHead, psDbHead,
                               "rzcx/proauth_rzcx_authlog_list.htm");
                return 1;
            }
            else if(0 == lCount1)
            {
                printf(" proauthRzauthlist: no group, please input \
                    extracter group name \n ");
                utPltOutToHtml(iFd, psMsgHead, psDbHead,
                               "rzcx/proauth_rzcx_authlog_list.htm");
                return 1;
            }
            else
            {
                pHash = (unsigned char *)pasLHashInit(10, 10,
                                                      sizeof(struct sddata_s), 0, 32);
                if(NULL == pHash)
                {
                    utWebDispMsg(iFd, psMsgHead, "nc/ncmsg_back.htm",
                                 "分配内存出错", "分配内存出错");
                    return 0;
                }
                memset(sql, 0, sizeof(sql));
                snprintf(sql, sizeof(sql), "select groupname,dispname from \
                    ncsuser where 1 ");

                snprintf(sql + strlen(sql), sizeof(sql) - strlen(sql),
                         " %s ", sqlServiceCodes);
                psCur = pasDbOpenSql(sql, 0);
                memset(sqlServiceCodes, 0, sizeof(sqlServiceCodes));

                printf("__line = %d, sql = %s\n", __LINE__, sql);
                if(NULL != psCur)
                {
                    memset(caServicecode, 0, sizeof(caServicecode));
                    memset(caDispname, 0, sizeof(caDispname));
                    iret = pasDbFetchInto(psCur,
                                          UT_TYPE_STRING, 32, caServicecode,
                                          UT_TYPE_STRING, 64, caDispname);
                    snprintf(sqlServiceCodes, sizeof(sqlServiceCodes),
                             " A.servicecode in ( ");

                    iNum = 0;
                    while((0 == iret) || (1405 == iret))
                    {
                        iNum++;
                        if(1 < iNum)
                        {
                            snprintf(sqlServiceCodes + strlen(sqlServiceCodes),
                                     sizeof(sqlServiceCodes) - strlen(sqlServiceCodes),
                                     ", ");
                        }
                        snprintf(sqlServiceCodes + strlen(sqlServiceCodes),
                                 sizeof(sqlServiceCodes) - strlen(sqlServiceCodes),
                                 " '%s' ", caServicecode);
                        psData = (struct sddata_s *)pasLHashLookA(pHash,
                                                                  caServicecode);
                        if(psData)
                        {
                            strcpy(psData->serviceCode, caServicecode);
                            strcpy(psData->dispName, caDispname);
                        }

                        memset(caServicecode, 0, sizeof(caServicecode));
                        memset(caDispname, 0, sizeof(caDispname));
                        iret = pasDbFetchInto(psCur,
                                              UT_TYPE_STRING, 32, caServicecode,
                                              UT_TYPE_STRING, 64, caDispname);
                    }// while((0 == iret) || (1405 == iret)) {
                    snprintf(sqlServiceCodes + strlen(sqlServiceCodes),
                             sizeof(sqlServiceCodes) - strlen(sqlServiceCodes), " ) ");
                }// if (NULL != psCur) {
                snprintf(sql2 + strlen(sql2), sizeof(sql2) - strlen(sql2),
                         " and  %s ", sqlServiceCodes);
            }// if (10 < lCount1) {
        }// if ((0 < strlen(caServicename))  || (0 < strlen(caServicecode))) {

        if(0 < strlen(caIp))
        {
            snprintf(sql2 + strlen(sql2), sizeof(sql2) - strlen(sql2),
                     " and INET_NTOA(A.sip) like  '%c%s%c' ", '%', caIp, '%');
        }
        if(0 < strlen(caoutIp))
        {
            snprintf(sql2 + strlen(sql2), sizeof(sql2) - strlen(sql2),
                     " and INET_NTOA(A.oip) like  '%c%s%c' ", '%', caoutIp, '%');
        }
        if(0 < strlen(catermMac))
        {
            snprintf(sql2 + strlen(sql2), sizeof(sql2) - strlen(sql2),
                     " and A.mac like '%c%s%c' ", '%', catermMac, '%');
        }
        if(0 < strlen(caAPName))
        {
            snprintf(sql2 + strlen(sql2), sizeof(sql2) - strlen(sql2),
                     " and A.apname like '%c%s%c' ", '%', caAPName, '%');
        }
        if(0 < strlen(caAPMac))
        {
            snprintf(sql2 + strlen(sql2), sizeof(sql2) - strlen(sql2),
                     " and A.apmac like '%c%s%c' ", '%', caAPMac, '%');
        }
        if(0 < strlen(caoprFlag))
        {
            nOprFlag = atoi(caoprFlag);
            if(0 != nOprFlag)
            {
                snprintf(sql2 + strlen(sql2), sizeof(sql2) - strlen(sql2),
                         " and A.flags = %d ", nOprFlag);
            }
        }

        //公安要求只查一天的数据
        sprintf(sql2 + strlen(sql2), " and A.stime >= %lu ",
                time(0) - 3600 * 24);


        strcat(sql2, " and length(A.username)>4 ");

    }
    else
    {
        iTime_flag = atoi(caTime_flag);
        printf("iTime_flag=%lu\n", iTime_flag);
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
        }//switch(iTime_flag) {

        //get service code by id
        memset(strSqlSrvCode, 0, sizeof(strSqlSrvCode));
        if(0 < strlen(caGroupId))
        {
            lGroupId = strtol(caGroupId, NULL, 10);
            if(ERANGE != lGroupId)
            {
                memset(sql, 0, sizeof(sql));
                snprintf(sql, sizeof(sql), "select username from ncsuser where \
                       groupid = %d ", lGroupId);
                memset(caGroupId, 0, sizeof(caGroupId));
                iReturn = pasDbOneRecord(sql, 0, UT_TYPE_STRING, 32, caGroupId);
                if(0 == iReturn)
                {
                    snprintf(strSqlSrvCode, sizeof(strSqlSrvCode),
                             " and A.servicecode = '%s' ", caGroupId);
                }
            }
        }// if (0 < strlen(caGroupId)) {

        printf("days=%d\n", days);
        ptmp = ncsUtlGetTable_t(0, lTime, days, "nctermiplog_if_",  &lStartTime, &lCount);
        lTime = lTime + 2 * 3600;
        printf("lStartTime=%d\n", lStartTime);

        snprintf(sql2, sizeof(sql2), " where A.stime >= %lu and A.stime <= %lu ",
                 lStartTime, lTime);



        if(0 < strlen(caKeyword))
        {
            snprintf(sql2 + strlen(sql2), sizeof(sql2) - strlen(sql2),
                     " and (A.username like '%c%s%c' or A.servicecode like '%c%s%c' or \
                A.dispname like '%c%s%c' or A.mac like '%c%s%c' )",
                     '%', caKeyword, '%', '%', caKeyword, '%',
                     '%', caKeyword, '%', '%', caKeyword, '%');
        }
        strcat(sql2, strSqlSrvCode);

        //公安要求只查一天的数据
        sprintf(sql2 + strlen(sql2), " and A.stime >= %lu ",
                time(0) - 3600 * 24);


        strcat(sql2, " and length(A.username)>4 ");
    }// if ( 0 == strcmp(ADV_SEARCH, caSearchType)) {//advanced search


    //loop couting record numbers in tables
    for(i = 0; i < lCount; i++)
    {
        lCount1 = 0;
        memset(sql, 0, sizeof(sql));
        snprintf(sql, sizeof(sql), "select count(*) from %s as A ", ptmp[i]);
        strcat(sql, sql2);
        printf("sql = %s\n", sql);
        pasDbOneRecord(sql, 0, UT_TYPE_ULONG, 4, &lCount1);
        if(0 < lCount1)
        {
            strcpy(tableinfo[num].tname, ptmp[i]);
            tableinfo[num].lCount = lCount1;
            num++;
        }
        free(ptmp[i]);
    }// for (i = 0; i < lCount; i++) {


    free(ptmp);

    for(i = 0; i < num; i++)
    {
        lTotRec += tableinfo[i].lCount;
    }
    utPltPutVarF(psDbHead, "TotRec", "%lu", lTotRec);

    for(i = num - 1; i >= 0; i--)
    {
        printf("table[%d] = %s, _lCount1[%d] = %lu\n", i, tableinfo[i].tname,
               i, tableinfo[i].lCount);
    }

    iNum = 0;
    lStartRec =  lTotRec <  lStartRec ?
                 (lTotRec / lRowNum) * lRowNum : lStartRec ;
    iStart = lStartRec;
    iLimit = lRowNum;

    //loop seeking in tables
    for(i = num - 1; i >= 0; i--)
    {
        memset(sql, 0, sizeof(sql));
        if(tableinfo[i].lCount <= iStart)
        {
            iStart -= tableinfo[i].lCount;
            continue;
        }
        snprintf(sql, sizeof(sql), "select A.username, \
            A.servicecode, A.stime, INET_NTOA(A.sip), A.sport, \
            INET_NTOA(A.oip), A.osport, A.oeport, A.mac, A.apname, A.apmac, \
            A.longitude, A.latitude, A.sessionid, A.nettype, INET_NTOA(A.dip), \
            A.dport ");
        memset(caTemp, 0, sizeof(caTemp));
        if(0 == strcmp(ADV_SEARCH, caSearchType)
           && ((0 < strlen(caServicename))
               || (0 < strlen(caServicecode))))  //advanced search
        {
            snprintf(sql + strlen(sql), sizeof(sql) - strlen(sql),
                     ",A.dispname  ");
        }
        else
        {
            snprintf(sql + strlen(sql), sizeof(sql) - strlen(sql),
                     ",B.dispname  ");
            sprintf(caTemp, " left join ncsuser as B \
                on (B.username=A.servicecode) ");
        }
        snprintf(sql + strlen(sql), sizeof(sql) - strlen(sql),
                 "from %s as A  ", tableinfo[i].tname);
        strcat(sql, caTemp);

        strcat(sql, sql2);

        snprintf(sql + strlen(sql), sizeof(sql) - strlen(sql),
                 " order by A.stime desc limit %lu, %lu", iStart,
                 iLimit < tableinfo[i].lCount ? iLimit : tableinfo[i].lCount);

        if((tableinfo[i].lCount > iStart)
           && (tableinfo[i].lCount < iStart + iLimit))
        {
            iLimit = iLimit - (tableinfo[i].lCount - iStart);
            iStart = 0;
        }
        else
        {
            _break = 1;
        }

        printf("__line = %d, sql = %s\n", __LINE__, sql);

        printf("pid=%d\n", getpid());

        psCur = pasDbOpenSql(sql, 0);
        if(NULL != psCur)
        {
            memset(caUsername, 0, sizeof(caUsername));
            memset(caServicecode, 0, sizeof(caServicecode));
            lStime = lEtime = 0;
            memset(clientIp, 0, sizeof(clientIp));
            memset(caIp, 0, sizeof(caIp));
            lSPort = lEPort = 0;
            memset(clientMac, 0, sizeof(clientMac));
            memset(APName, 0, sizeof(APName));
            memset(caMac, 0, sizeof(caMac));
            memset(longitude, 0, sizeof(longitude));
            memset(latitude, 0, sizeof(latitude));
            memset(rssi, 0, sizeof(rssi));
            memset(strSid, 0, sizeof(strSid));
            lFlag = 0;
            memset(caIdtype, 0, sizeof(caIdtype));
            memset(caIdno, 0, sizeof(caIdno));
            memset(appId, 0, sizeof(appId));
            memset(caDispname, 0, sizeof(caDispname));
            lClientPort = lNetType = lDPort = 0;
            memset(dip, 0, sizeof(dip));

            iret = pasDbFetchInto(psCur,
                                  UT_TYPE_STRING, 32, caUsername,
                                  UT_TYPE_STRING, 14, caServicecode,
                                  UT_TYPE_LONG, 4, &lStime,
                                  UT_TYPE_STRING, 15, clientIp,
                                  UT_TYPE_LONG, 4, &lClientPort,
                                  UT_TYPE_STRING, 15, caIp,
                                  UT_TYPE_LONG, 4, &lSPort,
                                  UT_TYPE_LONG, 4, &lEPort,
                                  UT_TYPE_STRING, 18, clientMac,
                                  UT_TYPE_STRING, 18, APName,
                                  UT_TYPE_STRING, 18, caMac,
                                  UT_TYPE_STRING, 11, longitude,
                                  UT_TYPE_STRING, 11, latitude,
                                  UT_TYPE_STRING, 36, strSid,
                                  UT_TYPE_LONG, 4, &lNetType,
                                  UT_TYPE_STRING, 15, dip,
                                  UT_TYPE_LONG, 4, &lDPort,
                                  UT_TYPE_STRING, 64, caDispname);

            while((0 == iret) || (1405 == iret))
            {
                iNum++;
                if(1 < iNum)
                {
                    utPltPutLoopVar(psDbHead, "dh", iNum, ", ");
                }

                utPltPutLoopVar(psDbHead, "authAccount", iNum, caUsername);
                utPltPutLoopVar(psDbHead, "servicecode", iNum, caServicecode);
                utPltPutLoopVar(psDbHead, "stime", iNum,
                                utTimFormat("%Y/%m/%d %H:%M", lStime));
                if((0 == strcmp(ADV_SEARCH, caSearchType))
                   && ((0 < strlen(caServicename))
                       || (0 < strlen(caServicecode)))
                   && pHash)  //advanced search
                {
                    psData = (struct sddata_s *)pasLHashLookA(pHash,
                                                              caServicecode);
                    if(psData)
                    {
                        strcpy(caDispname, psData->dispName);
                    }
                }
                utPltPutLoopVar(psDbHead, "servicename", iNum, caDispname);
                utPltPutLoopVar(psDbHead, "sip", iNum, clientIp);
                utPltPutLoopVar(psDbHead, "oip", iNum, caIp);
                sprintf(caTemp, "%d - %d", lSPort, lEPort);
                utPltPutLoopVar(psDbHead, "sport", iNum, caTemp);
                utPltPutLoopVar(psDbHead, "mac", iNum, clientMac);
                utPltPutLoopVar(psDbHead, "apname", iNum, APName);
                utPltPutLoopVar(psDbHead, "apmac", iNum, caMac);
                utPltPutLoopVar(psDbHead, "longitude", iNum, longitude);
                utPltPutLoopVar(psDbHead, "latitude", iNum, latitude);
                utPltPutLoopVar(psDbHead, "sessionid", iNum, strSid);
                strcpy(caTemp, getService_GA(lNetType));
                /*
                              psData2 = (struct ncsNettype_s *)pasLHashLook(pHashService,&lNetType);
                                        if(psData2){
                                      strcpy(caTemp,psData2->caName);
                                    }
                                    else{
                                      strcpy(caTemp,"");
                                    }
                   */

                //              strcpy(caTemp, ncUtlGetServiceName(psShmHead, lNetType, "\0"));
                utPltPutLoopVar(psDbHead, "nettype", iNum, caTemp);
                utPltPutLoopVarF(psDbHead, "sport1", iNum, "%d", lClientPort);
                utPltPutLoopVar(psDbHead, "dip", iNum, dip);
                utPltPutLoopVarF(psDbHead, "dport", iNum, "%d", lDPort);

                // fetch next record
                memset(caUsername, 0, sizeof(caUsername));
                memset(caServicecode, 0, sizeof(caServicecode));
                lStime = lEtime = 0;
                memset(clientIp, 0, sizeof(clientIp));
                memset(caIp, 0, sizeof(caIp));
                lSPort = lEPort = 0;
                memset(clientMac, 0, sizeof(clientMac));
                memset(APName, 0, sizeof(APName));
                memset(caMac, 0, sizeof(caMac));
                memset(longitude, 0, sizeof(longitude));
                memset(latitude, 0, sizeof(latitude));
                memset(rssi, 0, sizeof(rssi));
                memset(strSid, 0, sizeof(strSid));
                lFlag = 0;
                memset(caIdtype, 0, sizeof(caIdtype));
                memset(caIdno, 0, sizeof(caIdno));
                memset(appId, 0, sizeof(appId));
                memset(caDispname, 0, sizeof(caDispname));
                lClientPort = lNetType = lDPort = 0;
                memset(dip, 0, sizeof(dip));

                iret = pasDbFetchInto(psCur,
                                      UT_TYPE_STRING, 32, caUsername,
                                      UT_TYPE_STRING, 14, caServicecode,
                                      UT_TYPE_LONG, 4, &lStime,
                                      UT_TYPE_STRING, 15, clientIp,
                                      UT_TYPE_LONG, 4, &lClientPort,
                                      UT_TYPE_STRING, 15, caIp,
                                      UT_TYPE_LONG, 4, &lSPort,
                                      UT_TYPE_LONG, 4, &lEPort,
                                      UT_TYPE_STRING, 18, clientMac,
                                      UT_TYPE_STRING, 18, APName,
                                      UT_TYPE_STRING, 18, caMac,
                                      UT_TYPE_STRING, 11, longitude,
                                      UT_TYPE_STRING, 11, latitude,
                                      UT_TYPE_STRING, 36, strSid,
                                      UT_TYPE_LONG, 4, &lNetType,
                                      UT_TYPE_STRING, 15, dip,
                                      UT_TYPE_LONG, 4, &lDPort,
                                      UT_TYPE_STRING, 64, caDispname);
            }//while((0 == iret) || (1405 == iret)) {

            pasDbCloseCursor(psCur);
        } //if (NULL != psCur) {

        if(1 == _break)
        {
            break;
        }
    }// for (i = num - 1; i >= 0; i--)

    if(pHash)
    {
        free(pHash);
    }

    utPltOutToHtml(iFd, psMsgHead, psDbHead, "rzcx/proauth_rzcx_iplog_list.htm");
    return 0;
}

//显示AP信息
int proauthWebAplist(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead)
{
    char caTemp[2024];
    char caEdate[20];
    unsigned long lCurPg, lTotPg;
    unsigned long lTime, l, lTime1, lSip, lDip;
    int i, iNum;
    utPltDbHead *psDbHead;
    int iReturn;
    long lRowNum, lflags, lStartRec, lCount, lSid;
    char sqlbuf[2048] = "";
    long lRadius;
    pasDbCursor *psCur;

    char caTotPg[20], caCurPg[20];
    char caKeyword[64], caTime_flag[16];
    char caPage[12], caLimit[16], caSort[16], caDir[16];

    char caSdate_d[20], caEdate_d[20], caEtime[16];
    char caTemp_d[68];
    char caFacs[2048], caGroups[2048];
    char caCode[16], caName[64];
    char caGroupid[20], caDid[20];
    char caApname[36];
    char caFcode[32], caFname[64];
    long lAptype, lUptimestep;
    char caGroupids[2048]="", caDel[20]="", caSelsid[512]="";
    char caGwid[36], caMac[68], caAddress[256], caVersion[32], caMark[68];
    char caPlate[36], caLine[36], caMapid[36], caSsid[36];
    unsigned long lGroupid, lFlags;
    double fLongitude, fLatitude;
    char caServicecode[20], caServicename[128], caLongitude[20], caLatitude[20];
    char caUsername2[36];
    char caSh[16];
    memset(caUsername2, 0, sizeof(caUsername2));
    dsCltGetSessionValue(1, "dispname", UT_TYPE_STRING, 30, caUsername2);


    lCurPg = 1;

    utMsgPrintMsg(psMsgHead);
    iReturn = utMsgGetSomeNVar(psMsgHead, 12,
                               "TotPg",      UT_TYPE_STRING, 12, caTotPg,
                               "CurPg",      UT_TYPE_STRING, 12, caCurPg,
                               "keyword", UT_TYPE_STRING, 60, caKeyword,
                               "page",            UT_TYPE_STRING, 10, caPage,
                               "limit",           UT_TYPE_STRING, 10, caLimit,
                               "sort",            UT_TYPE_STRING, 15, caSort,
                               "dir",          UT_TYPE_STRING, 8,  caDir,
                               "groupid",      UT_TYPE_STRING, 10, caGroupid,
                               "did",          UT_TYPE_STRING, 10, caDid,
                               "selsid",       UT_TYPE_STRING, 500, caSelsid,
                               "del",          UT_TYPE_STRING, 10, caDel,
                               "sh",           UT_TYPE_STRING, 10, caSh);
	printf("caSelsid=[%s]\n", caSelsid);
    if(strlen(caSelsid) > 0)
    {
        pasCvtGBK(2, caSelsid, caTemp, 1000);
        strcpy(caSelsid, caTemp);
    }

    strcpy(caGroups, getDsGroupid());
    if(strlen(caGroupid) > 0)
    {
        strcpy(caGroupids, getGroupid(atol(caGroupid)));
    }
    else
    {
        strcpy(caGroupids, "");
    }

    utStrDelSpaces(caKeyword);


    if(strlen(caKeyword) > 0)
    {
        strcpy(caTemp_d, pasCvtGBK(2, caKeyword, caTemp, 60));
        strcpy(caKeyword, caTemp_d);
    }

    lCurPg = atol(caPage);
    if(lCurPg <= 0) lCurPg = 1;

	printf("caDel=%s\n", caDel);
    if(strcmp(caDel, "del") == 0)
    {
        pasDbExecSqlF("delete from ncaplist where apname in (%s) ", caSelsid);
		printf("sql='delete from ncaplist where apname in (%s)' \n", caSelsid);
    }
    if(strcmp(caSh, "sh") == 0)
    {
        char caMsg[1000];
        sprintf(caMsg, "审核AP%s ", caSelsid);
        pasDbExecSqlF("update ncaplist set flags=1 where apname in (%s) ", caSelsid);
        ncsWriteSysLog(psMsgHead, caUsername2, "05", "成功", caMsg);

    }
    if(strlen(caLimit) > 0)
    {
        lRowNum = atol(caLimit);
    }
    else
    {
        lRowNum = 40; //每一页面行数
    }
    lStartRec = (lCurPg - 1) * lRowNum;



    sprintf(sqlbuf, "select count(*) from ncaplist where 1=1 ");
    if(strlen(caGroups) > 0)
    {
        //         sprintf(sqlbuf+strlen(sqlbuf)," and groupid in (select groupid from ncsrvgroup where groupid in (%s))) ",caGroups);
    }

    if(strlen(caKeyword) > 0)
    {
        sprintf(sqlbuf + strlen(sqlbuf), "and (apname like '%c%s%c' or mac like '%c%s%c' or address like '%c%s%c' or plate like '%c%s%c' or line like '%c%s%c' or servicecode like '%c%s%c' or servicename like '%c%s%c' or dispname like '%c%s%c' )", '%', caKeyword, '%', '%', caKeyword, '%', '%', caKeyword, '%', '%', caKeyword, '%', '%', caKeyword, '%', '%', caKeyword, '%', '%', caKeyword, '%','%', caKeyword, '%');
       printf("1529sqlbuf=[%s]\n",sqlbuf);
	  }
    if(strlen(caGroupid) > 0)
    {
        sprintf(sqlbuf + strlen(sqlbuf), " and servicecode in (select username from ncsuser where groupid=%s ", caGroupid);
    }
    //      if(strlen(caGroupids)>0){
    //          sprintf(sqlbuf+strlen(sqlbuf)," and groupid in (select groupid from ncsrvgroup where groupid in (%s))) ",caGroupids);
    //      }
    if(strlen(caDid) > 0)
    {
        sprintf(sqlbuf + strlen(sqlbuf), " and did=%s ", caDid);
    }

    lCount = 0;
    pasDbOneRecord(sqlbuf, 0, UT_TYPE_LONG, 4, &lCount);

    sprintf(sqlbuf, "select apname,mac,did,address,version,servicecode,plate,line,mapid,ssid,longitude,latitude,a.flags,servicename,fcode,aptype,uptimestep,b.name,a.radius from ncaplist a left join ncsfactorycod b on (fcode=b.code) where 1=1 ");
    if(strlen(caGroups) > 0)
    {
        //         sprintf(sqlbuf+strlen(sqlbuf)," and groupid in (select groupid from ncsrvgroup where groupid in (%s))) ",caGroups);
    }

    if(strlen(caKeyword) > 0)
    {
        sprintf(sqlbuf + strlen(sqlbuf), "and (apname like '%c%s%c' or mac like '%c%s%c' or address like '%c%s%c' or plate like '%c%s%c' or line like '%c%s%c' or servicecode like '%c%s%c' or servicename like '%c%s%c' or dispname like '%c%s%c' )", '%', caKeyword, '%', '%', caKeyword, '%', '%', caKeyword, '%', '%', caKeyword, '%', '%', caKeyword, '%', '%', caKeyword, '%', '%', caKeyword, '%', '%', caKeyword, '%');
    }
    if(strlen(caGroupids) > 0)
    {
        //        sprintf(sqlbuf+strlen(sqlbuf)," and groupid in (select groupid from ncsrvgroup where groupid in (%s))) ",caGroupids);
    }
    if(strlen(caGroupid) > 0)
    {
        sprintf(sqlbuf + strlen(sqlbuf), " and servicecode in (select username from ncsuser where groupid=%s) ", caGroupid);
    }
    if(strlen(caDid) > 0)
    {
        sprintf(sqlbuf + strlen(sqlbuf), " and did=%s ", caDid);
    }
    if(strcmp(caSort, "gwid") == 0)
    {
        strcpy(caSort, "apname");
    }
    sprintf(sqlbuf + strlen(sqlbuf), " order by %s %s limit %lu,%lu", caSort, caDir, lStartRec, lRowNum);

    printf("sqlbuf=%s,calDel=[%s]\n", sqlbuf, caDel);
    psCur = pasDbOpenSql(sqlbuf, 0);
    if(psCur == NULL)
    {
        utWebDispMsg(iFd, psMsgHead, "ncmsg_pasmsg.htm", "用户查询", "查询数据出错 %s", pasDbErrorMsg(NULL));
        return 0;
    }


    psDbHead = utPltInitDb();

    iReturn = 0;
    memset(caGwid, 0, sizeof(caGwid));
    memset(caMac, 0, sizeof(caMac));
    lGroupid = 0;
    memset(caAddress, 0, sizeof(caAddress));
    memset(caVersion, 0, sizeof(caVersion));
    memset(caMark, 0, sizeof(caMark));
    memset(caPlate, 0, sizeof(caPlate));
    memset(caLine, 0, sizeof(caLine));
    memset(caMapid, 0, sizeof(caMapid));
    memset(caSsid, 0, sizeof(caSsid));
    memset(caServicecode, 0, sizeof(caServicecode));
    memset(caServicename, 0, sizeof(caServicename));
    memset(caLongitude, 0, sizeof(caLongitude));
    memset(caLatitude, 0, sizeof(caLatitude));
    lFlags = 0;
    memset(caFcode, 0, sizeof(caFcode));
    lAptype = 0;
    lUptimestep = 0;
    lRadius = 0;
    memset(caFname, 0, sizeof(caFname));
    iReturn = pasDbFetchInto(psCur, UT_TYPE_STRING, sizeof(caGwid) - 1, caGwid,
                             UT_TYPE_STRING, sizeof(caMac) - 1, caMac,
                             UT_TYPE_LONG, 4, &lGroupid,
                             UT_TYPE_STRING, sizeof(caAddress) - 1, caAddress,
                             UT_TYPE_STRING, sizeof(caVersion) - 1, caVersion,
                             UT_TYPE_STRING, sizeof(caServicecode) - 1, caServicecode,
                             UT_TYPE_STRING, sizeof(caPlate) - 1, caPlate,
                             UT_TYPE_STRING, sizeof(caLine) - 1, caLine,
                             UT_TYPE_STRING, sizeof(caMapid) - 1, caMapid,
                             UT_TYPE_STRING, sizeof(caSsid) - 1, caSsid,
                             UT_TYPE_STRING, sizeof(caLongitude) - 1, caLongitude,
                             UT_TYPE_STRING, sizeof(caLatitude) - 1, caLatitude,
                             UT_TYPE_LONG, 4, &lFlags,
                             UT_TYPE_STRING, sizeof(caServicename) - 1, caServicename,
                             UT_TYPE_STRING, sizeof(caFcode) - 1, caFcode,
                             UT_TYPE_LONG, 4, &lAptype,
                             UT_TYPE_LONG, 4, &lUptimestep,
                             UT_TYPE_STRING, sizeof(caFname) - 1, caFname,
                             UT_TYPE_LONG, 4, &lRadius);




    iNum = 0;
    while(iReturn == 0 || iReturn == 1405)
    {

        if(iNum > 0)
        {
            utPltPutLoopVar(psDbHead, "dh", iNum + 1, ",");
        }
        else
        {
            utPltPutLoopVar(psDbHead, "dh", iNum + 1, "");
        }
        iNum++;

        utPltPutLoopVarF(psDbHead, "num", iNum, "%lu", iNum);
        utPltPutLoopVarF(psDbHead, "groupid", iNum, "%lu", lGroupid);
        utPltPutLoopVar(psDbHead, "gwid", iNum, caGwid);
        utPltPutLoopVar(psDbHead, "mac", iNum, caMac);
        utPltPutLoopVar(psDbHead, "address", iNum, caAddress);
        utPltPutLoopVar(psDbHead, "version", iNum, caVersion);
        utPltPutLoopVar(psDbHead, "servicecode", iNum, caServicecode);
        utPltPutLoopVar(psDbHead, "plate", iNum, caPlate);
        utPltPutLoopVar(psDbHead, "line", iNum, caLine);
        utPltPutLoopVar(psDbHead, "mapid", iNum, caMapid);
        utPltPutLoopVar(psDbHead, "ssid", iNum, caSsid);
        utPltPutLoopVarF(psDbHead, "longitude", iNum, "%s", caLongitude);
        utPltPutLoopVarF(psDbHead, "latitude", iNum, "%s", caLatitude);
        utPltPutLoopVarF(psDbHead, "flags", iNum, "%d", lFlags);
        utPltPutLoopVar(psDbHead, "servicename", iNum, caServicename);
        utPltPutLoopVar(psDbHead, "fcode", iNum, caFcode);
        utPltPutLoopVar(psDbHead, "fname", iNum, caFname);
        utPltPutLoopVarF(psDbHead, "aptype", iNum, "%d", lAptype);
        utPltPutLoopVarF(psDbHead, "uptimestep", iNum, "%d", lUptimestep);
        utPltPutLoopVarF(psDbHead, "radius", iNum, "%d", lRadius);
        //        utPltPutLoopVar(psDbHead,"corpname",iNum,authGetClientName(psShmHead,lGroupid));

        memset(caGwid, 0, sizeof(caGwid));
        memset(caMac, 0, sizeof(caMac));
        lGroupid = 0;
        memset(caAddress, 0, sizeof(caAddress));
        memset(caVersion, 0, sizeof(caVersion));
        memset(caMark, 0, sizeof(caMark));
        memset(caPlate, 0, sizeof(caPlate));
        memset(caLine, 0, sizeof(caLine));
        memset(caMapid, 0, sizeof(caMapid));
        memset(caSsid, 0, sizeof(caSsid));
        memset(caServicecode, 0, sizeof(caServicecode));
        memset(caServicename, 0, sizeof(caServicename));
        memset(caLongitude, 0, sizeof(caLongitude));
        memset(caLatitude, 0, sizeof(caLatitude));
        lRadius = 0;
        lFlags = 0;

        memset(caFcode, 0, sizeof(caFcode));
        lAptype = 0;
        lUptimestep = 0;
        memset(caFname, 0, sizeof(caFname));
        iReturn = pasDbFetchInto(psCur, UT_TYPE_STRING, sizeof(caGwid) - 1, caGwid,
                                 UT_TYPE_STRING, sizeof(caMac) - 1, caMac,
                                 UT_TYPE_LONG, 4, &lGroupid,
                                 UT_TYPE_STRING, sizeof(caAddress) - 1, caAddress,
                                 UT_TYPE_STRING, sizeof(caVersion) - 1, caVersion,
                                 UT_TYPE_STRING, sizeof(caServicecode) - 1, caServicecode,
                                 UT_TYPE_STRING, sizeof(caPlate) - 1, caPlate,
                                 UT_TYPE_STRING, sizeof(caLine) - 1, caLine,
                                 UT_TYPE_STRING, sizeof(caMapid) - 1, caMapid,
                                 UT_TYPE_STRING, sizeof(caSsid) - 1, caSsid,
                                 UT_TYPE_STRING, sizeof(caLongitude) - 1, caLongitude,
                                 UT_TYPE_STRING, sizeof(caLatitude) - 1, caLatitude,
                                 UT_TYPE_LONG, 4, &lFlags,
                                 UT_TYPE_STRING, sizeof(caServicename) - 1, caServicename,
                                 UT_TYPE_STRING, sizeof(caFcode) - 1, caFcode,
                                 UT_TYPE_LONG, 4, &lAptype,
                                 UT_TYPE_LONG, 4, &lUptimestep,
                                 UT_TYPE_STRING, sizeof(caFname) - 1, caFname,
                                 UT_TYPE_LONG, 4, &lRadius);
    }



    lTotPg = (lCount - 1) / lRowNum + 1;

    utPltPutVarF(psDbHead, "TotRec", "%lu", lCount);
    utPltPutVarF(psDbHead, "TotPg", "%lu", lTotPg);
    utPltPutVarF(psDbHead, "CurPg", "%lu", lCurPg);

    pasDbCloseCursor(psCur);

    ncsWriteSysLog(psMsgHead, caUsername2, "06", "成功", "查看AP信息");
    //     utPltShowDb(psDbHead);
    utPltOutToHtml(iFd, psMsgHead, psDbHead, "systemset/proauth_systemapset_list.htm");
    return 0;
}

//保存AP信息
int proauth_apset_Save(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead)
{
    utPltDbHead *psDbHead = NULL;

    long iReturn, lSid;
    char caPlateindex[64];;
    char caSid[12], caAdname[64], caAdcode[32], caDatarang[1024], caSsid[1024];
    unsigned long lTime = time(0);
    char caPlateid[32], caPid[20];
    char caTemp[1024];
    char caGwid[32], caGroupid[16], caMac[64], caAddress[256], caVersion[16];
    char caMark[64], caPlate[32], caLine[32], caMapid[32];
    char caLongitude[20], caLatitude[20], caFlags[12];
    char caServicename[128], caServicecode[20];
    char caDel[16];
    char caApname[36];
    char caFcode[32], caAptype[16], caUptimestep[16];
    long lCount;
    char caUsername2[36];
    char caRadius[16];
	char caOldApname[32]="";
    memset(caUsername2, 0, sizeof(caUsername2));
    dsCltGetSessionValue(1, "dispname", UT_TYPE_STRING, 30, caUsername2);


    utMsgPrintMsg(psMsgHead);

    psDbHead = utPltInitDbHead();

    utMsgGetSomeNVar(psMsgHead, 19,
                     "gwid",   UT_TYPE_STRING, sizeof(caGwid) - 1, caGwid,
                     "groupid", UT_TYPE_STRING, sizeof(caGroupid) - 1, caGroupid,
                     "mac", UT_TYPE_STRING, sizeof(caMac) - 1, caMac,
                     "address", UT_TYPE_STRING, sizeof(caAddress) - 1, caAddress,
                     "version", UT_TYPE_STRING, sizeof(caVersion) - 1, caVersion,
                     "servicecode", UT_TYPE_STRING, sizeof(caServicecode) - 1, caServicecode,
                     "plate", UT_TYPE_STRING, sizeof(caPlate) - 1, caPlate,
                     "line", UT_TYPE_STRING, sizeof(caLine) - 1, caLine,
                     "mapid", UT_TYPE_STRING, sizeof(caMapid) - 1, caMapid,
                     "ssid", UT_TYPE_STRING, sizeof(caSsid) - 1, caSsid,
                     "longitude", UT_TYPE_STRING, sizeof(caLongitude) - 1, caLongitude,
                     "latitude", UT_TYPE_STRING, sizeof(caLatitude) - 1, caLatitude,
                     "flags", UT_TYPE_STRING, sizeof(caFlags) - 1, caFlags,
                     "servicename", UT_TYPE_STRING, sizeof(caServicename) - 1, caServicename,
                     "fcode",      UT_TYPE_STRING, sizeof(caFcode) - 1, caFcode,
                     "aptype",    UT_TYPE_STRING, sizeof(caAptype) - 1, caAptype,
                     "uptimestep", UT_TYPE_STRING, sizeof(caUptimestep) - 1, caUptimestep,
                     "radius", UT_TYPE_STRING, sizeof(caRadius) - 1, caRadius,
					  "oldApname", UT_TYPE_STRING, sizeof(caOldApname) - 1, caOldApname
					 );

    if(strlen(caGroupid) == 0)
    {
        strcpy(caGroupid, "0");
    }
    if(strlen(caServicename) > 0)
    {
        pasCvtGBK(2, caServicename, caTemp, 100);
        strcpy(caServicename, caTemp);
    }
    if(strlen(caServicecode) > 0)
    {
        pasCvtGBK(2, caServicecode, caTemp, 16);
        strcpy(caServicecode, caTemp);
    }

    if(strlen(caGwid) > 0)
    {
        pasCvtGBK(2, caGwid, caTemp, 31);
        strcpy(caGwid, caTemp);
    }

    if(strlen(caAddress) > 0)
    {
        pasCvtGBK(2, caAddress, caTemp, 255);
        strcpy(caAddress, caTemp);
    }


    if(strlen(caMark) > 0)
    {
        pasCvtGBK(2, caMark, caTemp, 63);
        strcpy(caMark, caTemp);
    }

    if(strlen(caPlate) > 0)
    {
        pasCvtGBK(2, caPlate, caTemp, 31);
        strcpy(caPlate, caTemp);
    }


    if(strlen(caSsid) > 0)
    {
        pasCvtGBK(2, caSsid, caTemp, 31);
        strcpy(caSsid, caTemp);
    }

    if(strlen(caLine) > 0)
    {
        pasCvtGBK(2, caLine, caTemp, 31);
        strcpy(caLine, caTemp);
    }
    if(strlen(caMapid) > 0)
    {
        pasCvtGBK(2, caMapid, caTemp, 31);
        strcpy(caMapid, caTemp);
    }
	if(strlen(caOldApname)>0)
	{
	   sprintf(caTemp, "select count(*) from ncaplist where apname='%s' ", caOldApname);		
	}else{
		sprintf(caTemp, "select count(*) from ncaplist where apname='%s' ", caGwid);
	}
    
	printf("1831 caTemp=%s\n",caTemp);
    lCount = 0;
    pasDbOneRecord(caTemp, 0, UT_TYPE_LONG, 4, &lCount);
    if(lCount > 0)
    {
        printf("update ncaplist set mac='%s',did=%s,address='%s',version='%s',servicecode='%s',plate='%s',line='%s',mapid='%s',ssid='%s',longitude='%s',latitude='%s',lasttime=%d,flags=%d,servicename='%s',aptype=%d,fcode='%s',uptimestep=%d where apname='%s' \n",
               caMac, caGroupid, caAddress, caVersion, caServicecode, caPlate, caLine, caMapid, caSsid, caLongitude, caLatitude, time(0), atol(caFlags), caServicename, atol(caAptype), caFcode, atol(caUptimestep), caGwid);
        sprintf(caTemp, "修改AP%s信息", caGwid);
        ncsWriteSysLog(psMsgHead, caUsername2, "03", "成功", caTemp);
		if(strlen(caOldApname)>0)
		{
			
			        iReturn = pasDbExecSqlF("update ncaplist set mac='%s',did=%s,address='%s',version='%s',servicecode='%s',plate='%s',line='%s',mapid='%s',ssid='%s',longitude='%s',latitude='%s',lasttime=%d,flags=%d,servicename='%s',aptype=%d,fcode='%s',uptimestep=%d,radius=%d where apname='%s' ",
                                caMac, caGroupid, caAddress, caVersion, caServicecode, caPlate, caLine, caMapid, caSsid, caLongitude, caLatitude, time(0), atol(caFlags), caServicename, atol(caAptype), caFcode, atol(caUptimestep), atol(caRadius), caOldApname);
    
		}else{
			        iReturn = pasDbExecSqlF("update ncaplist set mac='%s',did=%s,address='%s',version='%s',servicecode='%s',plate='%s',line='%s',mapid='%s',ssid='%s',longitude='%s',latitude='%s',lasttime=%d,flags=%d,servicename='%s',aptype=%d,fcode='%s',uptimestep=%d,radius=%d where apname='%s' ",
                                caMac, caGroupid, caAddress, caVersion, caServicecode, caPlate, caLine, caMapid, caSsid, caLongitude, caLatitude, time(0), atol(caFlags), caServicename, atol(caAptype), caFcode, atol(caUptimestep), atol(caRadius), caGwid);
    
		}
    }
    else
    {
        printf("======insert into ncaplist (apname,mac,did,address,version,servicecode,plate,line,mapid,ssid,longitude,latitude,starttime,lasttime,flags,servicename,aptype,fcode,uptimestep) \
				  values('%s','%s',%s,'%s','%s','%s','%s','%s','%s','%s','%s','%s',%d,%d,%d,'%s',%d,'%s',%d)\n", \
               caGwid, caMac, caGroupid, caAddress, caVersion, caServicecode, caPlate, caLine, caMapid, caSsid, caLongitude, caLatitude, time(0), time(0), atol(caFlags), caServicename, atol(caAptype), caFcode, atol(caUptimestep));

        iReturn = pasDbExecSqlF("insert into ncaplist (apname,mac,did,address,version,servicecode,plate,line,mapid,ssid,longitude,latitude,starttime,lasttime,flags,servicename,aptype,fcode,uptimestep,radius) \
				  values('%s','%s',%s,'%s','%s','%s','%s','%s','%s','%s','%s','%s',%d,%d,%d,'%s',%d,'%s',%d,%d)", \
                                caGwid, caMac, caGroupid, caAddress, caVersion, caServicecode, caPlate, caLine, caMapid, caSsid, caLongitude, caLatitude, time(0), time(0), atol(caFlags), caServicename, atol(caAptype), caFcode, atol(caUptimestep), atol(caRadius));

        sprintf(caTemp, "添加AP%s信息", caGwid);
        ncsWriteSysLog(psMsgHead, caUsername2, "03", "成功", caTemp);
    }

    printf("iReturn=%d\n", iReturn);

    if(iReturn == 0)
    {
        utPltPutVar(psDbHead, "titel", "true");
        //      utPltPutVarF(psDbHead,"sid","%s",caSid);
        utPltPutVar(psDbHead, "message", "保存成功");
    }
    else
    {
        utPltPutVar(psDbHead, "titel", "false");
        //      utPltPutVarF(psDbHead,"sid","%s",caSid);
        utPltPutVar(psDbHead, "message", "保存失败");
    }

    utPltOutToHtml(iFd, psMsgHead, psDbHead, "systemset/nc_adfile.htm");

}
//显示liecene数
int lan_showstatus(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead)
{

    utPltDbHead *psDbHead;


    psDbHead = utPltInitDb();

    //  printf("caTemp11=%s\n",caTemp);
    char caProSn[32];
    unsigned long lExpire, lUsers, lStart, lCount;
    strcpy(caProSn, utComGetVar_sd(psShmHead, "ProductSN", ""));
    pasInstGetLicense(caProSn, &lExpire, &lUsers, &lStart);

    lCount = 0;

    utPltPutVar(psDbHead, "psn", caProSn);
    utPltPutVar(psDbHead, "nwversion", NCS_VERSION);
    utPltOutToHtml(iFd, psMsgHead, psDbHead, "proauth_show_firststatus.htm");

    return 0;
}
/* 增加或修改记录信息  */
int ncAdminUserSave_v8(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead)
{
    char caUserid[16];
    char caUsername[32];
    char caPassword1[32], caPassword2[32];
    char caGroupname[68], caDispname[32], caGroupid[68];
    char caLevel[16], caMyname[32];
    char caEmail[64];
    char caKey[32], caKey0[64];
    char caSum[16];
    char caSql[8192];
    char caVar[32];
    char *p;
    char caGroup_gbk[64];
    char caTemp[1024];
    char caHandphone[32];
    char caUserflags[8];
    char caOpt1[256];
    char caAddress[128], caUseflags[16];
    unsigned long lTime, lUserid, lUsrlevel, lGroupid, lRoleid;
    int iReturn, i, iSum;
    char caUsername2[32];
    char caKey2[64], caKey22[64];
    char caFac[256];
    char caMsg[1024];

    char *keyword_gbk;
    utPltDbHead *psDbHead;
    memset(caUsername2, 0, sizeof(caUsername2));
    dsCltGetSessionValue(1, "dispname", UT_TYPE_STRING, 30, caUsername2);
    memset(caGroupname, 0, sizeof(caGroupname));
    lTime = time(0);
    utMsgPrintMsg(psMsgHead);
    char caFacid[1024];
    iReturn = utMsgGetSomeNVar(psMsgHead, 13,
                               "userid",      UT_TYPE_STRING, 10, caUserid,
                               "username",        UT_TYPE_STRING, 31, caUsername,
                               "dispname",    UT_TYPE_STRING, 31, caDispname,
                               "password1",    UT_TYPE_STRING, 31, caPassword1,
                               "password2",   UT_TYPE_STRING, 31, caPassword2,
                               "groupname",     UT_TYPE_STRING, 64, caGroupid,
                               "logcount",    UT_TYPE_STRING, 31, caLevel,
                               "email",       UT_TYPE_STRING, 63, caEmail,
                               "sumright",    UT_TYPE_STRING, 10, caSum,
                               "useflags",    UT_TYPE_STRING, 2, caUseflags,
                               "optid",        UT_TYPE_STRING, 255, caOpt1,
                               "facid",        UT_TYPE_STRING, 1000, caFacid,
                               "level",        UT_TYPE_STRING, 10, caLevel);
    if(strlen(caDispname) > 0)
    {
        keyword_gbk = convert("UTF-8", "GBK", caDispname);
        if(keyword_gbk)
        {
            strcpy(caDispname, keyword_gbk);
            free(keyword_gbk);
        }
    }

    if(strlen(caGroupid) > 0)
    {
        keyword_gbk = convert("UTF-8", "GBK", caGroupid);
        if(keyword_gbk)
        {
            strcpy(caGroupid, keyword_gbk);
            free(keyword_gbk);
        }
    }
    if(strlen(caUsername) > 0)
    {
        keyword_gbk = convert("UTF-8", "GBK", caUsername);
        if(keyword_gbk)
        {
            strcpy(caUsername, keyword_gbk);
            free(keyword_gbk);
        }
    }

    lUserid = atol(caUserid);
    lGroupid = 0;
    sprintf(caTemp, "select groupid from ncsgroup where groupname='%s' or descr='%s' ", caGroupid, caGroupid);
    printf("caTemp=%s\n", caTemp);
    pasDbOneRecord(caTemp, 0, UT_TYPE_LONG, sizeof(long), &lGroupid);
    iSum = atol(caSum);
    if(strlen(caLevel) == 0)
    {
        strcpy(caLevel, "1");
    }
    lUsrlevel = atol(caLevel);
    iReturn = dsCltGetMyInfo(1, "USERNAME", caMyname);
    if(iReturn < 0)
    {
        strcpy(caMyname, "\0");
    }
    if(strcmp(caPassword1, caPassword2) != 0)
    {
        utWebDispMsg(iFd, psMsgHead, "v8/ncmsg_pasmsg.htm", "false", "口令不相等 ");
        return 0;
    }

    if(utStrIsSpaces(caUserid) || lUserid == 0)   /* 新增记录  */
    {
        lUserid = dsCltGetSeque();
        if(lUserid > 0)
        {
            if(utStrIsSpaces(caPassword1))
            {
                utWebDispMsg(iFd, psMsgHead, "v8/ncmsg_pasmsg.htm", "false", "口令不能为空 ");
                return 0;
            }
            sprintf(caKey0, "%s%s", caUsername, caPassword1);
            utMd5Ascii22(caKey0, strlen(caKey0), NULL, caKey); /* 口令加密  */
            sprintf(caSql, "<DSSQL>insert into dsuser (id,name,dispname,groupid,groupname,addtime,\
                              moditime,lkey,lasttime,useflags,status,logcount,email,opt,fac,usrlevel) values \
                           (%lu,'%s','%s',%lu,'%s',%lu,\
                           %lu,'%s',%lu,0,0,\
                           %d,'%s','%s','%s',%d)</DSSQL>\n",
                    lUserid, caUsername, caDispname, lGroupid, caGroupid, lTime, lTime,
                    caKey, lTime, lUsrlevel, caEmail, caOpt1, caFacid, atol(caLevel));

            for(i = 0; i < iSum; i++)
            {
                sprintf(caVar, "v%d", i);
                p = utMsgGetVar_s(psMsgHead, caVar);
                if(p)
                {
                    lRoleid = atol(p);
                    sprintf(caSql + strlen(caSql), "<DSSQL>insert into dsuserrole(id,roleid,addtime,addby) values \
                              (%lu,%lu,%lu,'%s')</DSSQL>\n",
                            lUserid, lRoleid, lTime, caMyname);
                }
            }
            printf("caSql=%s\n", caSql);

            iReturn = dsCltSqlExecSome(caSql);
            if(iReturn == 0)
            {
                utWebDispMsg(iFd, psMsgHead, "v8/ncmsg_pasmsg.htm", "true", "添加用户成功");
                ncsWriteSysLog(psMsgHead, caUsername2, "05", "成功", caSql);
                return 0;
                //           ncsWebAdminUserList(psShmHead, iFd,psMsgHead);
                ncsWriteSysLog(psMsgHead, caUsername2, "05", "成功", caSql);
            }
            else
            {
                ncsWriteSysLog(psMsgHead, caUsername2, "05", "失败", caSql);
                utWebDispMsg(iFd, psMsgHead, "v8/ncmsg_pasmsg.htm", "false", "添加用户失败，数据库操作出错");
                return 0;
            }
        }
        else
        {
            ncsWriteSysLog(psMsgHead, caUsername2, "05", "失败", caSql);
            utWebDispMsg(iFd, psMsgHead, "v8/ncmsg_pasmsg.htm", "false", "添加用户失败，数据库操作出错");
            return 0;
        }
    }
    else   /* 修改记录  */
    {
        memset(caSql, 0, sizeof(caSql));
        utStrAddF(caSql, "<DSSQL>update dsuser set name='%s',dispname='%s',groupid=%lu,\
                            groupname='%s',opt='%s',logcount=%d",
                  caUsername, caDispname, lGroupid, caGroupid, caOpt1, lUsrlevel);

        if(!utStrIsSpaces(caPassword1))
        {
            sprintf(caKey0, "%s%s", caUsername, caPassword1);
            utMd5Ascii22(caKey0, strlen(caKey0), NULL, caKey); /* 口令加密  */
            utStrAddF(caSql, ",lkey='%s'", caKey);
        }



        utStrAddF(caSql, ",moditime=%lu,lasttime=%lu,useflags=%lu,email='%s',fac='%s',usrlevel=%d where id=%lu </DSSQL>\n",
                  lTime, lTime, atol(caUseflags), caEmail, caFacid, atol(caLevel), lUserid);

        utStrAddF(caSql, "<DSSQL>delete from dsuserrole where id = %lu </DSSQL>\n", lUserid);
        for(i = 0; i < iSum; i++)
        {
            sprintf(caVar, "v%d", i);
            p = utMsgGetVar_s(psMsgHead, caVar);
            if(p)
            {
                lRoleid = atol(p);
                utStrAddF(caSql, "<DSSQL>insert into dsuserrole(id,roleid,addtime,addby) values \
                          (%lu,%lu,%lu,'%s')</DSSQL>\n",
                          lUserid, lRoleid, lTime, caMyname);
            }
        }
        printf("caSql=%s\n", caSql);
        iReturn = dsCltSqlExecSome(caSql);
        if(iReturn == 0)
        {
            ncsWriteSysLog(psMsgHead, caUsername2, "05", "成功", caSql);
            utWebDispMsg(iFd, psMsgHead, "v8/ncmsg_pasmsg.htm", "true", "修改用户成功");
            return 0;
            //           ncsWriteSysLog(psMsgHead,caUsername2,"05","成功",caSql);
        }
        else
        {
            ncsWriteSysLog(psMsgHead, caUsername2, "05", "失败", caSql);
            utWebDispMsg(iFd, psMsgHead, "v8/ncmsg_pasmsg.htm", "false", "修改用户失败，数据库操作出错%d", iReturn);
            return 0;

        }
    }
    return 0;
}

//系统管理员退出登录
int proauthExit(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead)
{
    char caApp[20];
    char caMsg[256];
    int iReturn;
    char caUsername2[36];
    char caTemp[1024];
    utPltDbHead *psDbHead = NULL;
    dsCltGetSessionValue(1, "dispname", UT_TYPE_STRING, 30, caUsername2);

    psDbHead = utPltInitDbHead();
    sprintf(caTemp, "%s退出系统", caUsername2);
    ncsWriteSysLog(psMsgHead, caUsername2, "01", "成功", caTemp);
    utPltOutToHtml(iFd, psMsgHead, psDbHead, "ds/proauth_exit.htm");

    return 0;
}

//状态参数设置
int proauth_statusparset(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead)
{
    char *pConfig;
    char *pFile;
    pasConfList *psConfig;
    utPltDbHead *psDbHead;
    int iReturn;
    psDbHead = utPltInitDb();
    pConfig = pasGetConfigFileName();
    psConfig = pasUtlReadConfig(pConfig);
    if(psConfig == NULL)
    {
        utWebDispMsg(iFd, psMsgHead, "ncmsg_pasmsg.htm", "参数设置", "参数设置出错 %s", pasDbErrorMsg(NULL));
        return 0;
    }
    utPltPutVar(psDbHead, "NcTimeOut",   pasUtlLookConfig(psConfig, "NcTimeOut", "600"));
    utPltPutVar(psDbHead, "DataTimeOut",     pasUtlLookConfig(psConfig, "DataTimeOut", "86400"));


    pasUtlFreeConfig(psConfig);
    //    utPltShowDb(psDbHead);
    utPltOutToHtml(iFd, psMsgHead, psDbHead, "systemset/proauth_statuspar.htm");
    return 0;
}


//保存状态参数设置
int proauth_statusparsave(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead)
{
    char caApp[20];
    char caMsg[256];
    int iReturn;
    char *pConfig;
    char *p, *pFile, *pNat, *pRoute;
    FILE *fp;
    pasConfList *psConfig;
    char caUsername2[36];
    utPltDbHead *psDbHead = NULL;
    dsCltGetSessionValue(1, "dispname", UT_TYPE_STRING, 30, caUsername2);
    utMsgPrintMsg(psMsgHead);
    psDbHead = utPltInitDbHead();

    pConfig = pasGetConfigFileName();
    psConfig = pasUtlReadConfig(pConfig);
    if(psConfig == NULL)
    {
        utWebDispMsg(iFd, psMsgHead, "ncmsg_pasmsg.htm", "参数设置", "参数设置出错 %s", pasDbErrorMsg(NULL));
        return 0;
    }

    if(p = utMsgGetVar_s(psMsgHead, "NcTimeOut"))
    {
        pasUtlModiConfig(psConfig, "NcTimeOut", p);
    }
    if(p = utMsgGetVar_s(psMsgHead, "DataTimeOut"))
    {
        pasUtlModiConfig(psConfig, "DataTimeOut", p);
    }




    pasUtlSaveConfig(pConfig, psConfig);
    pasUtlFreeConfig(psConfig);
    //    p = utMsgGetVar_s(psMsgHead,"app");
    //    if (p)
    //    {
    //       if (!utStrIsSpaces(p))
    //       {
    //            ncUtlReReadConFig(psShmHead);

    //        }
    //    }
    utPltPutVar(psDbHead, "titel", "true");
    ncsWriteSysLog(psMsgHead, caUsername2, "06", "成功", "修改状态参数");
    utPltPutVar(psDbHead, "message", "保存成功");
    utPltOutToHtml(iFd, psMsgHead, psDbHead, "v8/nc_adfile.htm");

    return 0;
}

//日志留存参数设置
int proauth_logparset(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead)
{
    char *pConfig;
    char *pFile;
    pasConfList *psConfig;
    utPltDbHead *psDbHead;
    int iReturn;
    psDbHead = utPltInitDb();
    pConfig = pasGetConfigFileName();
    psConfig = pasUtlReadConfig(pConfig);
    if(psConfig == NULL)
    {
        utWebDispMsg(iFd, psMsgHead, "ncmsg_pasmsg.htm", "参数设置", "参数设置出错 %s", pasDbErrorMsg(NULL));
        return 0;
    }
    utPltPutVar(psDbHead, "StartAutoClean",   pasUtlLookConfig(psConfig, "StartAutoClean", "No"));
    utPltPutVar(psDbHead, "termlog",     pasUtlLookConfig(psConfig, "termlog", "6"));
    utPltPutVar(psDbHead, "termiplog",     pasUtlLookConfig(psConfig, "termiplog", "6"));

    utPltPutVar(psDbHead, "termmaclog",     pasUtlLookConfig(psConfig, "termmaclog", "6"));
    utPltPutVar(psDbHead, "hotinfolog",     pasUtlLookConfig(psConfig, "hotinfolog", "6"));
    utPltPutVar(psDbHead, "mobileaplog",     pasUtlLookConfig(psConfig, "mobileaplog", "6"));

    utPltPutVar(psDbHead, "termiplogfile",     pasUtlLookConfig(psConfig, "termiplogfile", "30"));
    utPltPutVar(psDbHead, "termlogfile",     pasUtlLookConfig(psConfig, "termlogfile", "30"));
    utPltPutVar(psDbHead, "operlog",     pasUtlLookConfig(psConfig, "operlog", "30"));
    pasUtlFreeConfig(psConfig);
    //    utPltShowDb(psDbHead);
    utPltOutToHtml(iFd, psMsgHead, psDbHead, "systemset/proauth_logpar.htm");
    return 0;
}

//保存状态参数设置
int proauth_logparsave(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead)
{
    char caApp[20];
    char caMsg[256];
    int iReturn;
    char *pConfig;
    char *p, *pFile, *pNat, *pRoute;
    FILE *fp;
    pasConfList *psConfig;
    char caUsername2[36];
    utPltDbHead *psDbHead = NULL;
    dsCltGetSessionValue(1, "dispname", UT_TYPE_STRING, 30, caUsername2);
    utMsgPrintMsg(psMsgHead);
    psDbHead = utPltInitDbHead();

    pConfig = pasGetConfigFileName();
    psConfig = pasUtlReadConfig(pConfig);
    if(psConfig == NULL)
    {
        utWebDispMsg(iFd, psMsgHead, "ncmsg_pasmsg.htm", "参数设置", "参数设置出错 %s", pasDbErrorMsg(NULL));
        return 0;
    }

    if(p = utMsgGetVar_s(psMsgHead, "StartAutoClean"))
    {
        pasUtlModiConfig(psConfig, "StartAutoClean", p);
    }
    if(p = utMsgGetVar_s(psMsgHead, "termlog"))
    {
        pasUtlModiConfig(psConfig, "termlog", p);
    }
    if(p = utMsgGetVar_s(psMsgHead, "termiplogfile"))
    {
        pasUtlModiConfig(psConfig, "termiplogfile", p);
    }
    if(p = utMsgGetVar_s(psMsgHead, "termlogfile"))
    {
        pasUtlModiConfig(psConfig, "termlogfile", p);
    }
    if(p = utMsgGetVar_s(psMsgHead, "operlog"))
    {
        pasUtlModiConfig(psConfig, "operlog", p);
    }
    if(p = utMsgGetVar_s(psMsgHead, "termmaclog"))
    {
        pasUtlModiConfig(psConfig, "termmaclog", p);
    }

    if(p = utMsgGetVar_s(psMsgHead, "hotinfolog"))
    {
        pasUtlModiConfig(psConfig, "hotinfolog", p);
    }
    if(p = utMsgGetVar_s(psMsgHead, "mobileaplog"))
    {
        pasUtlModiConfig(psConfig, "mobileaplog", p);
    }

    pasUtlSaveConfig(pConfig, psConfig);
    pasUtlFreeConfig(psConfig);
    //    p = utMsgGetVar_s(psMsgHead,"app");
    //    if (p)
    //    {
    //       if (!utStrIsSpaces(p))
    //       {
    //            ncUtlReReadConFig(psShmHead);

    //        }
    //    }
    utPltPutVar(psDbHead, "titel", "true");
    ncsWriteSysLog(psMsgHead, caUsername2, "06", "成功", "修改留存日志参数");
    utPltPutVar(psDbHead, "message", "保存成功");
    utPltOutToHtml(iFd, psMsgHead, psDbHead, "v8/nc_adfile.htm");

    return 0;
}


//日志上传参数设置
int proauth_logftpparset(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead)
{
    char *pConfig;
    char *pFile;
    pasConfList *psConfig;
    utPltDbHead *psDbHead;
    int iReturn;
    psDbHead = utPltInitDb();
    pConfig = pasGetConfigFileName();
    psConfig = pasUtlReadConfig(pConfig);
    if(psConfig == NULL)
    {
        utWebDispMsg(iFd, psMsgHead, "ncmsg_pasmsg.htm", "参数设置", "参数设置出错 %s", pasDbErrorMsg(NULL));
        return 0;
    }
    utPltPutVar(psDbHead, "StartDb2Bcp",   pasUtlLookConfig(psConfig, "StartDb2Bcp", "No"));
    utPltPutVar(psDbHead, "Db2BcpOutPath",     pasUtlLookConfig(psConfig, "Db2BcpOutPath", "/data/bcpout"));
    utPltPutVar(psDbHead, "Db2BcpStep",     pasUtlLookConfig(psConfig, "Db2BcpStep", "10"));

    utPltPutVar(psDbHead, "DB2BcpZipPass",     pasUtlLookConfig(psConfig, "DB2BcpZipPass", "Ac123ASfgq"));
    utPltPutVar(psDbHead, "Db2BcpScode",     pasUtlLookConfig(psConfig, "Db2BcpScode", "310012"));
    utPltPutVar(psDbHead, "Db2BcpDcode",     pasUtlLookConfig(psConfig, "Db2BcpDcode", "310000"));

    utPltPutVar(psDbHead, "FtpKeep",     pasUtlLookConfig(psConfig, "FtpKeep", "No"));
    utPltPutVar(psDbHead, "FtpKeepPath",     pasUtlLookConfig(psConfig, "FtpKeepPath", "/data/ftpkeep"));
    utPltPutVar(psDbHead, "Db2BcpSendType",     pasUtlLookConfig(psConfig, "Db2BcpSendType", "FTP"));

    utPltPutVar(psDbHead, "StartSendFtp",     pasUtlLookConfig(psConfig, "StartSendFtp", "No"));
    utPltPutVar(psDbHead, "FtpTimeStep",     pasUtlLookConfig(psConfig, "FtpTimeStep", "10"));
    utPltPutVar(psDbHead, "FtpLocalPath",     pasUtlLookConfig(psConfig, "FtpLocalPath", "/data/bcpload"));

    utPltPutVar(psDbHead, "FtpFileKey",     pasUtlLookConfig(psConfig, "FtpFileKey", "zip"));
    utPltPutVar(psDbHead, "FtpRemotePath",     pasUtlLookConfig(psConfig, "FtpRemotePath", "/data/proeim/"));
    utPltPutVar(psDbHead, "FtpUser",     pasUtlLookConfig(psConfig, "FtpUser", "proeim"));
    utPltPutVar(psDbHead, "FtpServer",     pasUtlLookConfig(psConfig, "FtpServer", ""));
    utPltPutVar(psDbHead, "FtpPass",     pasUtlLookConfig(psConfig, "FtpPass", ""));

    pasUtlFreeConfig(psConfig);

    utPltOutToHtml(iFd, psMsgHead, psDbHead, "systemset/proauth_ftppar.htm");
    utPltShowDb(psDbHead);
    return 0;
}

//保存FTP参数设置
int proauth_logftpparsave(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead)
{
    char caApp[20];
    char caMsg[256];
    int iReturn;
    char *pConfig;
    char *p, *pFile, *pNat, *pRoute;
    FILE *fp;
    pasConfList *psConfig;
    char caUsername2[36];
    utPltDbHead *psDbHead = NULL;
    dsCltGetSessionValue(1, "dispname", UT_TYPE_STRING, 30, caUsername2);
    utMsgPrintMsg(psMsgHead);
    psDbHead = utPltInitDbHead();

    pConfig = pasGetConfigFileName();
    psConfig = pasUtlReadConfig(pConfig);
    if(psConfig == NULL)
    {
        utWebDispMsg(iFd, psMsgHead, "ncmsg_pasmsg.htm", "参数设置", "参数设置出错 %s", pasDbErrorMsg(NULL));
        return 0;
    }

    if(p = utMsgGetVar_s(psMsgHead, "StartDb2Bcp"))
    {
        pasUtlModiConfig(psConfig, "StartDb2Bcp", p);
    }
    if(p = utMsgGetVar_s(psMsgHead, "Db2BcpOutPath"))
    {
        pasUtlModiConfig(psConfig, "Db2BcpOutPath", p);
    }
    if(p = utMsgGetVar_s(psMsgHead, "Db2BcpStep"))
    {
        pasUtlModiConfig(psConfig, "Db2BcpStep", p);
    }
    if(p = utMsgGetVar_s(psMsgHead, "DB2BcpZipPass"))
    {
        pasUtlModiConfig(psConfig, "DB2BcpZipPass", p);
    }

    if(p = utMsgGetVar_s(psMsgHead, "Db2BcpScode"))
    {
        pasUtlModiConfig(psConfig, "Db2BcpScode", p);
    }
    if(p = utMsgGetVar_s(psMsgHead, "Db2BcpDcode"))
    {
        pasUtlModiConfig(psConfig, "Db2BcpDcode", p);
    }
    if(p = utMsgGetVar_s(psMsgHead, "FtpKeep"))
    {
        pasUtlModiConfig(psConfig, "FtpKeep", p);
    }

    if(p = utMsgGetVar_s(psMsgHead, "FtpKeepPath"))
    {
        pasUtlModiConfig(psConfig, "FtpKeepPath", p);
    }
    if(p = utMsgGetVar_s(psMsgHead, "Db2BcpSendType"))
    {
        pasUtlModiConfig(psConfig, "Db2BcpSendType", p);
    }


    if(p = utMsgGetVar_s(psMsgHead, "StartSendFtp"))
    {
        pasUtlModiConfig(psConfig, "StartSendFtp", p);
    }
    if(p = utMsgGetVar_s(psMsgHead, "FtpTimeStep"))
    {
        pasUtlModiConfig(psConfig, "FtpTimeStep", p);
    }

    if(p = utMsgGetVar_s(psMsgHead, "FtpLocalPath"))
    {
        pasUtlModiConfig(psConfig, "FtpLocalPath", p);
    }
    if(p = utMsgGetVar_s(psMsgHead, "FtpFileKey"))
    {
        pasUtlModiConfig(psConfig, "FtpFileKey", p);
    }

    if(p = utMsgGetVar_s(psMsgHead, "FtpRemotePath"))
    {
        pasUtlModiConfig(psConfig, "FtpRemotePath", p);
    }
    if(p = utMsgGetVar_s(psMsgHead, "FtpUser"))
    {
        pasUtlModiConfig(psConfig, "FtpUser", p);
    }

    if(p = utMsgGetVar_s(psMsgHead, "FtpServer"))
    {
        pasUtlModiConfig(psConfig, "FtpServer", p);
    }
    if(p = utMsgGetVar_s(psMsgHead, "FtpPass"))
    {
        pasUtlModiConfig(psConfig, "FtpPass", p);
    }

    pasUtlSaveConfig(pConfig, psConfig);
    pasUtlFreeConfig(psConfig);
    //    p = utMsgGetVar_s(psMsgHead,"app");
    //    if (p)
    //    {
    //       if (!utStrIsSpaces(p))
    //       {
    //            ncUtlReReadConFig(psShmHead);

    //        }
    //    }
    utPltPutVar(psDbHead, "titel", "true");
    ncsWriteSysLog(psMsgHead, caUsername2, "06", "成功", "修改日志上传参数");
    utPltPutVar(psDbHead, "message", "保存成功");
    utPltOutToHtml(iFd, psMsgHead, psDbHead, "v8/nc_adfile.htm");

    return 0;
}



//场所离线告警参数设置
int proauth_warnoffparset(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead)
{
    char *pConfig;
    char *pFile;
    pasConfList *psConfig;
    utPltDbHead *psDbHead;
    int iReturn;
    psDbHead = utPltInitDb();
    pConfig = pasGetConfigFileName();
    psConfig = pasUtlReadConfig(pConfig);
    if(psConfig == NULL)
    {
        utWebDispMsg(iFd, psMsgHead, "ncmsg_pasmsg.htm", "参数设置", "参数设置出错 %s", pasDbErrorMsg(NULL));
        return 0;
    }
    utPltPutVar(psDbHead, "StartWarnOff",   pasUtlLookConfig(psConfig, "StartWarnOff", "No"));
    utPltPutVar(psDbHead, "warncont",   pasUtlLookConfig(psConfig, "warncont", "上网管理系统故障告警，请及时报修！"));
    utPltPutVar(psDbHead, "bbmobno", pasUtlLookConfig(psConfig, "bbmobno", ""));

    utPltPutVar(psDbHead, "fzrmobno",   pasUtlLookConfig(psConfig, "fzrmobno", "0"));
    utPltPutVar(psDbHead, "aqymobno",   pasUtlLookConfig(psConfig, "aqymobno", "0"));
    utPltPutVar(psDbHead, "wjmobno",   pasUtlLookConfig(psConfig, "wjmobno", "0"));
    utPltPutVar(psDbHead, "bbmobflag",   pasUtlLookConfig(psConfig, "bbmobflag", "0"));

    pasUtlFreeConfig(psConfig);

    utPltOutToHtml(iFd, psMsgHead, psDbHead, "systemset/proauth_warnoffpar.htm");
    utPltShowDb(psDbHead);
    return 0;
}

//场所离线告警设置保存
int proauth_warnoffparsave(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead)
{
    char caApp[20];
    char caMsg[256];
    int iReturn;
    char *pConfig;
    char *p, *pFile, *pNat, *pRoute;
    FILE *fp;
    pasConfList *psConfig;
    char caUsername2[36];
    char caTemp_d[64], caTemp[1024];
    utPltDbHead *psDbHead = NULL;
    dsCltGetSessionValue(1, "dispname", UT_TYPE_STRING, 30, caUsername2);
    utMsgPrintMsg(psMsgHead);
    psDbHead = utPltInitDbHead();

    pConfig = pasGetConfigFileName();
    psConfig = pasUtlReadConfig(pConfig);
    if(psConfig == NULL)
    {
        utWebDispMsg(iFd, psMsgHead, "ncmsg_pasmsg.htm", "参数设置", "参数设置出错 %s", pasDbErrorMsg(NULL));
        return 0;
    }

    if(p = utMsgGetVar_s(psMsgHead, "StartWarnOff"))
    {
        pasUtlModiConfig(psConfig, "StartWarnOff", p);
    }

    if(p = utMsgGetVar_s(psMsgHead, "warncont"))
    {

        strcpy(caTemp_d, pasCvtGBK(2, p, caTemp, 60));
        pasUtlModiConfig(psConfig, "warncont", caTemp_d);
    }

    if(p = utMsgGetVar_s(psMsgHead, "bbmobno"))
    {
        pasUtlModiConfig(psConfig, "bbmobno", p);
    }
    if(p = utMsgGetVar_s(psMsgHead, "fzrmobno"))
    {
        pasUtlModiConfig(psConfig, "fzrmobno", p);
    }
    else
    {
        pasUtlModiConfig(psConfig, "fzrmobno", "0");
    }
    if(p = utMsgGetVar_s(psMsgHead, "aqymobno"))
    {
        pasUtlModiConfig(psConfig, "aqymobno", p);
    }
    else
    {
        pasUtlModiConfig(psConfig, "aqymobno", "0");
    }
    if(p = utMsgGetVar_s(psMsgHead, "wjmobno"))
    {
        pasUtlModiConfig(psConfig, "wjmobno", p);
    }
    else
    {
        pasUtlModiConfig(psConfig, "wjmobno", "0");
    }
    if(p = utMsgGetVar_s(psMsgHead, "bbmobflag"))
    {
        pasUtlModiConfig(psConfig, "bbmobflag", p);
    }
    else
    {
        pasUtlModiConfig(psConfig, "bbmobflag", "0");
    }

    pasUtlSaveConfig(pConfig, psConfig);
    pasUtlFreeConfig(psConfig);
    //    p = utMsgGetVar_s(psMsgHead,"app");
    //    if (p)
    //    {
    //       if (!utStrIsSpaces(p))
    //       {
    //            ncUtlReReadConFig(psShmHead);

    //        }
    //    }
    utPltPutVar(psDbHead, "titel", "true");
    ncsWriteSysLog(psMsgHead, caUsername2, "06", "成功", "修改场所离线告警参数");
    utPltPutVar(psDbHead, "message", "保存成功");
    utPltOutToHtml(iFd, psMsgHead, psDbHead, "v8/nc_adfile.htm");

    return 0;
}

/* 添加维护记录  */
int proauth_whsave_v8(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead)
{
    char *p;
    char caGroup_gbk[64];
    char caTemp[2024];

    unsigned long lTime;
    int iReturn, i, iSum;
    char caUsername2[32];
    char caKey2[64], caKey22[64];
    char caFac[256];
    char caDesc[1024];
    char caCorpids[1024], caSdate[16], caStime[16], caWhman[32], caFcode[16];
    char caDate_all[32];
    char *keyword_gbk;
    utPltDbHead *psDbHead;
    char caDispname[68], caServicecode[16];
    lTime = time(0);
    utMsgPrintMsg(psMsgHead);
    char caFacid[1024];
    char caObj[100][50];
    char caFlag[12];
    long num;
    psDbHead = utPltInitDbHead();
    iReturn = utMsgGetSomeNVar(psMsgHead, 7,
                               "corpid",      UT_TYPE_STRING, 1000, caCorpids,
                               "sdate",        UT_TYPE_STRING, 10, caSdate,
                               "stime",    UT_TYPE_STRING, 10, caStime,
                               "desc",    UT_TYPE_STRING, 254, caDesc,
                               "whman",   UT_TYPE_STRING, 15, caWhman,
                               "fcode",     UT_TYPE_STRING, 15, caFcode,
                               "flag",    UT_TYPE_STRING, 6, caFlag);
    if(strlen(caSdate) > 0)
    {
        sprintf(caDate_all, "%s %s", caSdate, caStime);
        lTime = utTimStrToLong("%Y/%m/%d %H:%M:%S", caDate_all);
    }
    if(strlen(caDesc) > 0)
    {
        keyword_gbk = convert("UTF-8", "GBK", caDesc);
        if(keyword_gbk)
        {
            strcpy(caDesc, keyword_gbk);
            free(keyword_gbk);
        }
    }

    if(strlen(caWhman) > 0)
    {
        keyword_gbk = convert("UTF-8", "GBK", caWhman);
        if(keyword_gbk)
        {
            strcpy(caWhman, keyword_gbk);
            free(keyword_gbk);
        }
    }
    num = ncs_SepChar0(caCorpids, ',', caObj);

    for(i = 0; i < num; i++)
    {
        sprintf(caTemp, "select username,dispname from ncsuser where userid=%s ", caObj[i]);
        pasDbOneRecord(caTemp, 0, UT_TYPE_STRING, 15, caServicecode,
                       UT_TYPE_STRING, 63, caDispname);
        printf("insert into ncswhlog(stime,servicecode,servicename,did,mesg,fcode,wman) \
   	      values(%d,'%s','%s',%d,'%s','%s','%s') \n", lTime, caServicecode, caDispname, atol(caObj[i]), caDesc, caFcode, caWhman);


        iReturn = pasDbExecSqlF("insert into ncswhlog(stime,servicecode,servicename,did,mesg,fcode,wman,flag) \
   	      values(%d,'%s','%s',%d,'%s','%s','%s',%d) ", lTime, caServicecode, caDispname, atol(caObj[i]), caDesc, caFcode, caWhman, atol(caFlag));
        if(iReturn == 0)
        {
            pasDbExecSqlF("update ncsuser set workflag=%d where userid=%d ", atol(caFlag), atol(caObj[i]));
        }

    }
    if(iReturn == 0)
    {
        utPltPutVar(psDbHead, "titel", "true");
        utPltPutVar(psDbHead, "message", "保存成功");
    }
    else
    {
        utPltPutVar(psDbHead, "titel", "false");
        utPltPutVar(psDbHead, "message", "保存失败");
    }
    utPltOutToHtml(iFd, psMsgHead, psDbHead, "v8/nc_adfile.htm");
    return 0;
}

//显示维护日志
int proauthWhloglist(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead)
{
    char caTemp[2024];
    char caEdate[20];
    unsigned long lCurPg, lTotPg;
    unsigned long lTime, l, lTime1, lSip, lDip;
    int i, iNum;
    utPltDbHead *psDbHead;
    int iReturn;
    long lRowNum, lflags, lStartRec, lCount;
    char sqlbuf[2048] = "";

    pasDbCursor *psCur;

    char caTotPg[20], caCurPg[20];
    char caKeyword[64], caTime_flag[16];
    char caPage[12], caLimit[16], caSort[16], caDir[16];

    char caSdate_d[20], caEdate_d[20], caEtime[16];
    char caTemp_d[68];
    char caFacs[2048], caGroups[2048];
    char caCode[16], caName[64];
    char caGroupid[20], caDid[20];
    char caServicecode[20], caServicename[68], caMesg[1024], caFcode[32], caWman[32];
    long lSid, lFlag, lStime;
    unsigned long lGroupid, lFlags;
    char caFname[68];
    char caUsername2[36];
    char caSh[16];
    char caFlag[16];
    memset(caUsername2, 0, sizeof(caUsername2));
    dsCltGetSessionValue(1, "dispname", UT_TYPE_STRING, 30, caUsername2);


    lCurPg = 1;

    utMsgPrintMsg(psMsgHead);
    iReturn = utMsgGetSomeNVar(psMsgHead, 10,
                               "TotPg",      UT_TYPE_STRING, 12, caTotPg,
                               "CurPg",      UT_TYPE_STRING, 12, caCurPg,
                               "keyword", UT_TYPE_STRING, 60, caKeyword,
                               "page",            UT_TYPE_STRING, 10, caPage,
                               "limit",           UT_TYPE_STRING, 10, caLimit,
                               "sort",            UT_TYPE_STRING, 15, caSort,
                               "dir",          UT_TYPE_STRING, 8,  caDir,
                               "groupid",      UT_TYPE_STRING, 10, caGroupid,
                               "did",          UT_TYPE_STRING, 10, caDid,
                               "flag",         UT_TYPE_STRING, 8, caFlag);


    utStrDelSpaces(caKeyword);


    if(strlen(caKeyword) > 0)
    {
        strcpy(caTemp_d, pasCvtGBK(2, caKeyword, caTemp, 60));
        strcpy(caKeyword, caTemp_d);
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

    strcpy(caFacs, getDsFacGroupid());

    sprintf(sqlbuf, "select count(*) from ncswhlog where 1=1 ");
    if(strlen(caFacs) > 0)
    {
        sprintf(sqlbuf + strlen(sqlbuf), " and fcode in (%s) ", caFacs);
    }

    if(strlen(caKeyword) > 0)
    {
        sprintf(sqlbuf + strlen(sqlbuf), "and (servicecode like '%c%s%c' or servicename like '%c%s%c' or mesg like '%c%s%c' )", '%', caKeyword, '%', '%', caKeyword, '%', '%', caKeyword, '%');
    }

    if(strlen(caDid) > 0)
    {
        sprintf(sqlbuf + strlen(sqlbuf), " and did=%s ", caDid);
    }
    if(strlen(caFlag) > 0)
    {
        sprintf(sqlbuf + strlen(sqlbuf), " and flag=%s ", caFlag);
    }
    lCount = 0;
    pasDbOneRecord(sqlbuf, 0, UT_TYPE_LONG, 4, &lCount);

    sprintf(sqlbuf, "select a.sid,stime,servicecode,servicename,mesg,fcode,wman,flag,name from ncswhlog a left join ncsfactorycod on (fcode=code) where 1=1 ");
    if(strlen(caFacs) > 0)
    {
        sprintf(sqlbuf + strlen(sqlbuf), " and fcode in (%s) ", caFacs);
    }

    if(strlen(caKeyword) > 0)
    {
        sprintf(sqlbuf + strlen(sqlbuf), "and (servicecode like '%c%s%c' or servicename like '%c%s%c' or mesg like '%c%s%c' )", '%', caKeyword, '%', '%', caKeyword, '%', '%', caKeyword, '%');
    }

    if(strlen(caDid) > 0)
    {
        sprintf(sqlbuf + strlen(sqlbuf), " and did=%s ", caDid);
    }
    if(strlen(caFlag) > 0)
    {
        sprintf(sqlbuf + strlen(sqlbuf), " and flag=%s ", caFlag);
    }
    sprintf(sqlbuf + strlen(sqlbuf), " order by sid desc limit %lu,%lu", lStartRec, lRowNum);

    printf("sqlbuf=%s\n", sqlbuf);
    psCur = pasDbOpenSql(sqlbuf, 0);
    if(psCur == NULL)
    {
        utWebDispMsg(iFd, psMsgHead, "ncmsg_pasmsg.htm", "用户查询", "查询数据出错 %s", pasDbErrorMsg(NULL));
        return 0;
    }


    psDbHead = utPltInitDb();

    iReturn = 0;
    lSid = 0;
    lStime = 0;
    memset(caServicecode, 0, sizeof(caServicecode));
    memset(caServicename, 0, sizeof(caServicename));
    memset(caMesg, 0, sizeof(caMesg));
    memset(caFcode, 0, sizeof(caFcode));
    memset(caWman, 0, sizeof(caWman));
    memset(caFname, 0, sizeof(caFname));
    lFlag = 0;
    iReturn = pasDbFetchInto(psCur, UT_TYPE_LONG, 4, &lSid,
                             UT_TYPE_LONG, 4, &lStime,
                             UT_TYPE_STRING, 15, caServicecode,
                             UT_TYPE_STRING, sizeof(caServicename) - 1, caServicename,
                             UT_TYPE_STRING, sizeof(caMesg) - 1, caMesg,
                             UT_TYPE_STRING, sizeof(caFcode) - 1, caFcode,
                             UT_TYPE_STRING, sizeof(caWman) - 1, caWman,
                             UT_TYPE_LONG, 4, &lFlag,
                             UT_TYPE_STRING, 63, caFname);




    iNum = 0;
    utPltSetCvtHtml(1);
    while(iReturn == 0 || iReturn == 1405)
    {

        if(iNum > 0)
        {
            utPltPutLoopVar(psDbHead, "dh", iNum + 1, ",");
        }
        else
        {
            utPltPutLoopVar(psDbHead, "dh", iNum + 1, "");
        }
        iNum++;

        utPltPutLoopVarF(psDbHead, "num", iNum, "%lu", iNum);
        utPltPutLoopVarF(psDbHead, "sid", iNum, "%lu", lSid);
        utPltPutLoopVar(psDbHead, "stime", iNum, utTimFormat("%Y/%m/%d %H:%M", lStime));
        utPltPutLoopVar(psDbHead, "servicecode", iNum, caServicecode);
        utPltPutLoopVar(psDbHead, "servicename", iNum, caServicename);
        utPltPutLoopVar(psDbHead, "mesg", iNum, caMesg);
        utPltPutLoopVar(psDbHead, "fcode", iNum, caFcode);
        utPltPutLoopVar(psDbHead, "servicecode", iNum, caServicecode);
        utPltPutLoopVar(psDbHead, "wman", iNum, caWman);
        utPltPutLoopVarF(psDbHead, "flag", iNum, "%d", lFlag);
        utPltPutLoopVar(psDbHead, "fname", iNum, caFname);

        lStime = 0;
        memset(caServicecode, 0, sizeof(caServicecode));
        memset(caServicename, 0, sizeof(caServicename));
        memset(caMesg, 0, sizeof(caMesg));
        memset(caFcode, 0, sizeof(caFcode));
        memset(caWman, 0, sizeof(caWman));
        memset(caFname, 0, sizeof(caFname));
        lFlag = 0;
        iReturn = pasDbFetchInto(psCur, UT_TYPE_LONG, 4, &lSid,
                                 UT_TYPE_LONG, 4, &lStime,
                                 UT_TYPE_STRING, 15, caServicecode,
                                 UT_TYPE_STRING, sizeof(caServicename) - 1, caServicename,
                                 UT_TYPE_STRING, sizeof(caMesg) - 1, caMesg,
                                 UT_TYPE_STRING, sizeof(caFcode) - 1, caFcode,
                                 UT_TYPE_STRING, sizeof(caWman) - 1, caWman,
                                 UT_TYPE_LONG, 4, &lFlag,
                                 UT_TYPE_STRING, 63, caFname);

    }



    lTotPg = (lCount - 1) / lRowNum + 1;
    utPltSetCvtHtml(0);
    utPltPutVarF(psDbHead, "TotRec", "%lu", lCount);
    utPltPutVarF(psDbHead, "TotPg", "%lu", lTotPg);
    utPltPutVarF(psDbHead, "CurPg", "%lu", lCurPg);

    pasDbCloseCursor(psCur);

    ncsWriteSysLog(psMsgHead, caUsername2, "06", "成功", "查看维护日志");
    //     utPltShowDb(psDbHead);
    utPltOutToHtml(iFd, psMsgHead, psDbHead, "rzcx/proauth_whlog_list.htm");
    return 0;
}


//显示消息日志
int proauthMesgloglist(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead)
{
    char caTemp[2024];
    char caEdate[20];
    unsigned long lCurPg, lTotPg;
    unsigned long lTime, l, lTime1, lSip, lDip;
    int i, iNum;
    utPltDbHead *psDbHead;
    int iReturn;
    long lRowNum, lflags, lStartRec, lCount;
    char sqlbuf[2048] = "";

    pasDbCursor *psCur;

    char caTotPg[20], caCurPg[20];
    char caKeyword[64], caTime_flag[16];
    char caPage[12], caLimit[16], caSort[16], caDir[16];

    char caSdate_d[20], caEdate_d[20], caEtime[16];
    char caTemp_d[68];
    char caFacs[2048], caGroups[2048];
    char caCode[16], caName[64];
    char caGroupid[20], caDid[20];
    char caServicecode[20], caServicename[68], caMesg[1024], caFcode[32], caWman[32];
    char caMessage_title[256], caMessage_content[1024], caMfrom[32], caMto[32], caMfrom_name[64], caMto_name[64];
    long lSid, lFlag, lStime;
    unsigned long lGroupid, lFlags;
    char caFname[68];
    char caUsername2[36], caDel[20], caSelsid[1024];
    char caSh[16];
    char caFlag[16];
    memset(caUsername2, 0, sizeof(caUsername2));
    dsCltGetSessionValue(1, "dispname", UT_TYPE_STRING, 30, caUsername2);


    lCurPg = 1;

    utMsgPrintMsg(psMsgHead);
    iReturn = utMsgGetSomeNVar(psMsgHead, 12,
                               "TotPg",      UT_TYPE_STRING, 12, caTotPg,
                               "CurPg",      UT_TYPE_STRING, 12, caCurPg,
                               "keyword", UT_TYPE_STRING, 60, caKeyword,
                               "page",            UT_TYPE_STRING, 10, caPage,
                               "limit",           UT_TYPE_STRING, 10, caLimit,
                               "sort",            UT_TYPE_STRING, 15, caSort,
                               "dir",          UT_TYPE_STRING, 8,  caDir,
                               "groupid",      UT_TYPE_STRING, 10, caGroupid,
                               "did",          UT_TYPE_STRING, 10, caDid,
                               "flag",         UT_TYPE_STRING, 8, caFlag,
                               "del",          UT_TYPE_STRING, 8, caDel,
                               "selsid",       UT_TYPE_STRING, 1000, caSelsid);

    if(strlen(caDel) > 0 && strlen(caSelsid) > 0)
    {
        pasDbExecSqlF("delete from ncsmessage where sid in (%s) ", caSelsid);
    }

    utStrDelSpaces(caKeyword);


    if(strlen(caKeyword) > 0)
    {
        strcpy(caTemp_d, pasCvtGBK(2, caKeyword, caTemp, 60));
        strcpy(caKeyword, caTemp_d);
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

    strcpy(caFacs, getDsFacGroupid());

    sprintf(sqlbuf, "select count(*) from ncsmessage where 1=1 ");
    if(strlen(caFacs) > 0)
    {
        sprintf(sqlbuf + strlen(sqlbuf), " and (mfrom in (%s) or mto in (%s) or mto='')", caFacs, caFacs);
    }

    if(strlen(caKeyword) > 0)
    {
        sprintf(sqlbuf + strlen(sqlbuf), "and (message_title like '%c%s%c' or message_content like '%c%s%c' or mfrom like '%c%s%c' or mto like '%c%s%c' )", '%', caKeyword, '%', '%', caKeyword, '%', '%', caKeyword, '%', '%', caKeyword, '%');
    }


    if(strlen(caFlag) > 0)
    {
        if(atol(caFlag) == 1)
        {
            if(strlen(caFacs) == 0)
            {
                sprintf(sqlbuf + strlen(sqlbuf), " and mto ='admin' ");
            }
            else
            {
                sprintf(sqlbuf + strlen(sqlbuf), " and mto in (%s) ", caFacs);
            }
        }
        else if(atol(caFlag) == 2)
        {
            if(strlen(caFacs) == 0)
            {
                sprintf(sqlbuf + strlen(sqlbuf), " and mfrom ='admin' ");
            }
            else
            {
                sprintf(sqlbuf + strlen(sqlbuf), " and mfrom in (%s) ", caFacs);
            }

        }

    }
    lCount = 0;
    pasDbOneRecord(sqlbuf, 0, UT_TYPE_LONG, 4, &lCount);

    sprintf(sqlbuf, "select a.sid,send_time,message_title,message_content,mfrom,mto,b.name,c.name from ncsmessage a left join ncsfactorycod b on (a.mfrom=b.code) left join ncsfactorycod c on (a.mto=c.code) where 1=1 ");
    if(strlen(caFacs) > 0)
    {
        sprintf(sqlbuf + strlen(sqlbuf), " and (mfrom in (%s) or mto in (%s) or mto='')", caFacs, caFacs);
    }

    if(strlen(caKeyword) > 0)
    {
        sprintf(sqlbuf + strlen(sqlbuf), "and (message_title like '%c%s%c' or message_content like '%c%s%c' or mfrom like '%c%s%c' or mto like '%c%s%c' )", '%', caKeyword, '%', '%', caKeyword, '%', '%', caKeyword, '%', '%', caKeyword, '%');
    }
    if(strlen(caFlag) > 0)
    {
        if(atol(caFlag) == 1)
        {
            if(strlen(caFacs) == 0)
            {
                sprintf(sqlbuf + strlen(sqlbuf), " and mto ='admin' ");
            }
            else
            {
                sprintf(sqlbuf + strlen(sqlbuf), " and mto in (%s) ", caFacs);
            }
        }
        else if(atol(caFlag) == 2)
        {
            if(strlen(caFacs) == 0)
            {
                sprintf(sqlbuf + strlen(sqlbuf), " and mfrom ='admin' ");
            }
            else
            {
                sprintf(sqlbuf + strlen(sqlbuf), " and mfrom in (%s) ", caFacs);
            }

        }

    }
    sprintf(sqlbuf + strlen(sqlbuf), " order by a.sid desc limit %lu,%lu", lStartRec, lRowNum);

    printf("sqlbuf=%s\n", sqlbuf);


    psCur = pasDbOpenSql(sqlbuf, 0);
    if(psCur == NULL)
    {
        utWebDispMsg(iFd, psMsgHead, "ncmsg_pasmsg.htm", "用户查询", "查询数据出错 %s", pasDbErrorMsg(NULL));
        return 0;
    }


    psDbHead = utPltInitDb();
    iNum = 0;
    lSid = 0;
    lStime = 0;
    memset(caMessage_title, 0, sizeof(caMessage_title));
    memset(caMessage_content, 0, sizeof(caMessage_content));
    memset(caMfrom, 0, sizeof(caMfrom));
    memset(caMto, 0, sizeof(caMto));
    memset(caMto_name, 0, sizeof(caMto_name));
    memset(caMfrom_name, 0, sizeof(caMfrom_name));
    lFlag = 0;
    iReturn = pasDbFetchInto(psCur, UT_TYPE_LONG, 4, &lSid,
                             UT_TYPE_LONG, 4, &lStime,
                             UT_TYPE_STRING, 250, caMessage_title,
                             UT_TYPE_STRING, 1000, caMessage_content,
                             UT_TYPE_STRING, 15, caMfrom,
                             UT_TYPE_STRING, 15, caMto,
                             UT_TYPE_STRING, 30, caMfrom_name,
                             UT_TYPE_STRING, 30, caMto_name);

    while(iReturn == 0 || iReturn == 1405)
    {
        if(strlen(caMfrom_name) == 0 && strlen(caMfrom) > 0)
        {
            strcpy(caMfrom_name, caMfrom);
        }
        if(strlen(caMto_name) == 0 && strlen(caMto) > 0)
        {
            strcpy(caMto_name, caMto);
        }
        if(iNum > 0)
        {
            utPltPutLoopVar(psDbHead, "dh", iNum + 1, ",");
        }
        else
        {
            utPltPutLoopVar(psDbHead, "dh", iNum + 1, "");
        }
        iNum++;
        utPltPutLoopVarF(psDbHead, "num", iNum, "%lu", iNum);
        utPltPutLoopVarF(psDbHead, "sid", iNum, "%d", lSid);
        utPltPutLoopVar(psDbHead, "stime", iNum, utTimFormat("%Y/%m/%d %H:%M", lStime));
        utPltPutLoopVar(psDbHead, "message_title", iNum, caMessage_title);
        utPltPutLoopVar(psDbHead, "message_content", iNum, caMessage_content);
        utPltPutLoopVar(psDbHead, "mfrom", iNum, caMfrom);
        utPltPutLoopVar(psDbHead, "mto", iNum, caMto);
        utPltPutLoopVar(psDbHead, "mfrom_name", iNum, caMfrom_name);
        utPltPutLoopVar(psDbHead, "mto_name", iNum, caMto_name);
        iReturn = 0;
        lSid = 0;
        lStime = 0;
        memset(caMessage_title, 0, sizeof(caMessage_title));
        memset(caMessage_content, 0, sizeof(caMessage_content));
        memset(caMfrom, 0, sizeof(caMfrom));
        memset(caMto, 0, sizeof(caMto));
        memset(caMto_name, 0, sizeof(caMto_name));
        memset(caMfrom_name, 0, sizeof(caMfrom_name));
        lFlag = 0;
        iReturn = pasDbFetchInto(psCur, UT_TYPE_LONG, 4, &lSid,
                                 UT_TYPE_LONG, 4, &lStime,
                                 UT_TYPE_STRING, 250, caMessage_title,
                                 UT_TYPE_STRING, 1000, caMessage_content,
                                 UT_TYPE_STRING, 15, caMfrom,
                                 UT_TYPE_STRING, 15, caMto,
                                 UT_TYPE_STRING, 30, caMfrom_name,
                                 UT_TYPE_STRING, 30, caMto_name);

    }



    lTotPg = (lCount - 1) / lRowNum + 1;
    utPltSetCvtHtml(0);
    utPltPutVarF(psDbHead, "TotRec", "%lu", lCount);
    utPltPutVarF(psDbHead, "TotPg", "%lu", lTotPg);
    utPltPutVarF(psDbHead, "CurPg", "%lu", lCurPg);

    pasDbCloseCursor(psCur);

    ncsWriteSysLog(psMsgHead, caUsername2, "06", "成功", "查看消息日志");
    //     utPltShowDb(psDbHead);
    utPltOutToHtml(iFd, psMsgHead, psDbHead, "rzcx/proauth_mesglog_list.htm");
    return 0;
}

/* 添加消息  */
int proauth_mesgsave_v8(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead)
{
    char *p;
    char caGroup_gbk[64];
    char caTemp[2024];

    unsigned long lTime;
    int iReturn, i, iSum;
    char caUsername2[32];
    char caKey2[64], caKey22[64];
    char caFac[256];
    char caDesc[1024];
    char caCorpids[1024], caSdate[16], caStime[16], caWhman[32], caFcode[16];
    char caDate_all[32];
    char *keyword_gbk;
    utPltDbHead *psDbHead;
    char caDispname[68], caServicecode[16];
    lTime = time(0);
    utMsgPrintMsg(psMsgHead);
    char caFacid[1024];
    char caObj[100][50];
    char caMto[16], caMessage_title[256], caMessage_content[1024];
    char caFlag[12];
    long num;
    psDbHead = utPltInitDbHead();
    iReturn = utMsgGetSomeNVar(psMsgHead, 3,
                               "mto",      UT_TYPE_STRING, 15, caMto,
                               "message_title", UT_TYPE_STRING, 255, caMessage_title,
                               "message_content", UT_TYPE_STRING, 1000, caMessage_content);


    if(strlen(caMessage_title) > 0)
    {
        keyword_gbk = convert("UTF-8", "GBK", caMessage_title);
        if(keyword_gbk)
        {
            strcpy(caMessage_title, keyword_gbk);
            free(keyword_gbk);
        }
    }

    if(strlen(caMessage_content) > 0)
    {
        keyword_gbk = convert("UTF-8", "GBK", caMessage_content);
        if(keyword_gbk)
        {
            strcpy(caMessage_content, keyword_gbk);
            free(keyword_gbk);
        }
    }
    strcpy(caFac, getDsFacGroupid());
    printf("caFac=%s\n", caFac);
    num = ncs_SepChar0(caFac, ',', caObj);
    printf("num=%d\n", num);
    if(num > 0)
    {
        strcpy(caFac, caObj[0]);
    }
    else
    {
        strcpy(caFac, "\'admin\'");
    }

    printf("insert into ncsmessage(send_time,message_title,message_content,mfrom,mto) \
   	      values(%d,'%s','%s','%s','%s') \n", lTime, caMessage_title, caMessage_content, caFac, caMto);

    iReturn = pasDbExecSqlF("insert into ncsmessage(send_time,message_title,message_content,mfrom,mto) \
   	      values(%d,'%s','%s',%s,'%s') ", lTime, caMessage_title, caMessage_content, caFac, caMto);



    if(iReturn == 0)
    {
        utPltPutVar(psDbHead, "titel", "true");
        utPltPutVar(psDbHead, "message", "保存成功");
    }
    else
    {
        utPltPutVar(psDbHead, "titel", "false");
        utPltPutVar(psDbHead, "message", "保存失败");
    }
    utPltOutToHtml(iFd, psMsgHead, psDbHead, "v8/nc_adfile.htm");
    return 0;
}

// 显示热点信息日志
int ncsHotinfolist_v4(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead)
{
    char caExport[32];
    FILE *fp;
    char caFilename[32];
    char caFile[128];
    char caPath[128];
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
    char caServicecode[16], caSsid[36], caSearchtype[32];
    strcpy(caFacs, getDsFacDid());
    strcpy(caGroups, getDsGroupid());

    utPltDbHead *psDbHead = utPltInitDb();
    utMsgPrintMsg(psMsgHead);
    iReturn = utMsgGetSomeNVar(psMsgHead, 21,
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
                               "searchtype",      UT_TYPE_STRING, 10, caSearchtype);

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
    if(strlen(caSearchtype) > 0)
    {
        if(strlen(caMac) > 0)
        {
            sprintf(caCont + strlen(caCont), " and mac like '%c%s%c' ", '%', caMac, '%');
        }
        if(strlen(caApmac) > 0)
        {
            sprintf(caCont + strlen(caCont), " and apmac like '%c%s%c' ", '%', caApmac, '%');
        }
        if(strlen(caApname) > 0)
        {
            sprintf(caCont + strlen(caCont), " and apname like '%c%s%c' ", '%', caApname, '%');
        }
        if(strlen(caServicecode) > 0)
        {
            sprintf(caCont + strlen(caCont), " and servicecode like '%c%s%c' ", '%', caServicecode, '%');
        }
        if(strlen(caCorpname) > 0)
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
    if(strlen(caDid) > 0)
    {

        sprintf(caCont + strlen(caCont), " and servicecode='%s' ", getServicecodeByid(psShmHead, atol(caDid)));
    }
    if(strlen(caGroupid) > 0)
    {
        sprintf(caCont + strlen(caCont), " and servicecode in (%s)", getServicecodeByGroupid(psShmHead, caGroupid));
    }
    if(strlen(caGroups) > 0)
    {
        sprintf(caCont + strlen(caCont), " and servicecode in (%s)", getServicecodeByGroupid(psShmHead, caGroups));
    }




    for(i = 0; i < lCount; i++)
    {

        lCount1 = 0;
        memset(sql, 0, sizeof(sql));
        snprintf(sql, sizeof(sql), "select count(*) from %s where stime >= %lu and stime <= %lu  ",  ptmp[i], lStartTime, lTime);
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

    //
    for(i = num - 1; i >= 0; i--)
    {
        printf("table[%d] = %s, _lCount1[%d] = %lu\n", i, tableinfo[i].tname, i, tableinfo[i].lCount);
    }
    //

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
        snprintf(sql, sizeof(sql), "select servicecode,servicename,address,mac,apname,apmac,ssid,stime,channel,rssi,security,longitude,latitude from %s where stime >= %lu and stime <= %lu  ", tableinfo[i].tname, lStartTime, lTime);
        strcat(sql, caCont);

        snprintf(sql + strlen(sql), sizeof(sql) - strlen(sql), " order by stime desc limit %lu, %lu", iStart, iLimit < tableinfo[i].lCount ? iLimit : tableinfo[i].lCount);


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
                                  UT_TYPE_STRING, 11, caLatitude);


            while((iret == 0) || (iret == 1405))
            {
                iNum++;
                if(iNum > 1)
                {
                    utPltPutLoopVar(psDbHead, "dh", iNum, ",");
                }

                utPltPutLoopVar(psDbHead, "stime", iNum, utTimFormat("%y/%m/%d %H:%M", lStarttime));

                utPltPutLoopVar(psDbHead, "address", iNum, caAddress);
                utPltPutLoopVar(psDbHead, "apname", iNum, caApname);
                utPltPutLoopVar(psDbHead, "mac", iNum, caMac);
                utPltPutLoopVar(psDbHead, "apmac", iNum, caApmac);
                utPltPutLoopVar(psDbHead, "servicename", iNum, caServicename);
                utPltPutLoopVar(psDbHead, "channel", iNum, caChannel);
                utPltPutLoopVar(psDbHead, "rssi", iNum, caRssi);
                //       printf("caSecurity=%s\n",caSecurity);
                utPltPutLoopVar(psDbHead, "security", iNum, caSecurity);
                utPltPutLoopVar(psDbHead, "longitude", iNum, caLongitude);
                utPltPutLoopVar(psDbHead, "latitude", iNum, caLatitude);
                utPltPutLoopVar(psDbHead, "ssid", iNum, caSsid);
                utPltPutLoopVar(psDbHead, "servicecode", iNum, caServicecode);
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
                                      UT_TYPE_STRING, 11, caLatitude);
            }
            pasDbCloseCursor(psCur);
        }

        if(_break == 1)
            break;
    }



    utPltOutToHtml(iFd, psMsgHead, psDbHead, "rzcx/proauth_hotinfo_list.htm");
    return 0;
}

// 显示终端特征信息日志
int ncsMactermlist_v4(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead)
{
    char caExport[32];
    FILE *fp;
    char caFilename[32];
    char caFile[128];
    char caPath[128];
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
    char caServicecode[16], caSsid[36], caSearchtype[32];
    strcpy(caFacs, getDsFacDid());
    strcpy(caGroups, getDsGroupid());

    utPltDbHead *psDbHead = utPltInitDb();
    utMsgPrintMsg(psMsgHead);
    iReturn = utMsgGetSomeNVar(psMsgHead, 21,
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
                               "searchtype",      UT_TYPE_STRING, 10, caSearchtype);

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
            sprintf(caCont + strlen(caCont), " and mac like '%c%s%c' ", '%', caMac, '%');
        }
        if(strlen(caApmac) > 0)
        {
            sprintf(caCont + strlen(caCont), " and apmac like '%c%s%c' ", '%', caApmac, '%');
        }
        if(strlen(caApname) > 0)
        {
            sprintf(caCont + strlen(caCont), " and apname like '%c%s%c' ", '%', caApname, '%');
        }
        if(strlen(caServicecode) > 0)
        {
            sprintf(caCont + strlen(caCont), " and servicecode like '%c%s%c' ", '%', caServicecode, '%');
        }
        if(strlen(caCorpname) > 0)
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
    if(strlen(caDid) > 0)
    {

        sprintf(caCont + strlen(caCont), " and servicecode='%s' ", getServicecodeByid(psShmHead, atol(caDid)));
    }
    if(strlen(caGroupid) > 0)
    {
        sprintf(caCont + strlen(caCont), " and servicecode in (%s)", getServicecodeByGroupid(psShmHead, caGroupid));
    }
    if(strlen(caGroups) > 0)
    {
        sprintf(caCont + strlen(caCont), " and servicecode in (%s)", getServicecodeByGroupid(psShmHead, caGroups));
    }




    for(i = 0; i < lCount; i++)
    {

        lCount1 = 0;
        memset(sql, 0, sizeof(sql));
        snprintf(sql, sizeof(sql), "select count(*) from %s where stime >= %lu and stime <= %lu  ",  ptmp[i], lStartTime, lTime);
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

    //
    for(i = num - 1; i >= 0; i--)
    {
        printf("table[%d] = %s, _lCount1[%d] = %lu\n", i, tableinfo[i].tname, i, tableinfo[i].lCount);
    }
    //

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
        snprintf(sql, sizeof(sql), "select servicecode,servicename,address,mac,apname,apmac,ssid,stime,channel,rssi,security,longitude,latitude,termtype,vtype,vname from %s where stime >= %lu and stime <= %lu  ", tableinfo[i].tname, lStartTime, lTime);
        strcat(sql, caCont);

        snprintf(sql + strlen(sql), sizeof(sql) - strlen(sql), " order by stime desc limit %lu, %lu", iStart, iLimit < tableinfo[i].lCount ? iLimit : tableinfo[i].lCount);


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
                                  UT_TYPE_STRING, 63, caVname);


            while((iret == 0) || (iret == 1405))
            {
                iNum++;
                if(iNum > 1)
                {
                    utPltPutLoopVar(psDbHead, "dh", iNum, ",");
                }

                utPltPutLoopVar(psDbHead, "stime", iNum, utTimFormat("%y/%m/%d %H:%M", lStarttime));

                utPltPutLoopVar(psDbHead, "address", iNum, caAddress);
                utPltPutLoopVar(psDbHead, "apname", iNum, caApname);
                utPltPutLoopVar(psDbHead, "mac", iNum, caMac);
                utPltPutLoopVar(psDbHead, "apmac", iNum, caApmac);
                utPltPutLoopVar(psDbHead, "servicename", iNum, caServicename);
                utPltPutLoopVar(psDbHead, "channel", iNum, caChannel);
                utPltPutLoopVar(psDbHead, "rssi", iNum, caRssi);
                //       printf("caSecurity=%s\n",caSecurity);
                utPltPutLoopVar(psDbHead, "security", iNum, caSecurity);
                utPltPutLoopVar(psDbHead, "longitude", iNum, caLongitude);
                utPltPutLoopVar(psDbHead, "latitude", iNum, caLatitude);
                utPltPutLoopVar(psDbHead, "ssid", iNum, caSsid);
                utPltPutLoopVar(psDbHead, "servicecode", iNum, caServicecode);
                utPltPutLoopVar(psDbHead, "termtype", iNum, caTermtype);
                utPltPutLoopVar(psDbHead, "vname", iNum, caVname);
                utPltPutLoopVarF(psDbHead, "vtype", iNum, "%d", lVtype);
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
                                      UT_TYPE_STRING, 63, caVname);
            }
            pasDbCloseCursor(psCur);
        }

        if(_break == 1)
            break;
    }



    utPltOutToHtml(iFd, psMsgHead, psDbHead, "rzcx/proauth_hotinfo_list.htm");
    return 0;
}


// 显示移动终端轨迹日志
int ncsMobileAplist_v4(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead)
{
    char caExport[32];
    FILE *fp;
    char caFilename[32];
    char caFile[128];
    char caPath[128];
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
    char caServicecode[16], caSsid[36], caSearchtype[32];
    strcpy(caFacs, getDsFacDid());
    strcpy(caGroups, getDsGroupid());

    utPltDbHead *psDbHead = utPltInitDb();
    utMsgPrintMsg(psMsgHead);
    iReturn = utMsgGetSomeNVar(psMsgHead, 21,
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
                               "searchtype",      UT_TYPE_STRING, 10, caSearchtype);

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
        ptmp = ncsUtlGetTable3(sdate, edate, caStime, caEtime, "ncmacmobileap_if_", &lStartTime, &lTime, &lCount);
        printf("lStartTime=%d,sdate=%s\n", lStartTime, sdate);
    }
    else
    {
        ptmp = ncsUtlGetTable(lTime, days, "ncmacmobileap_if_",  &lStartTime, &lCount);
        lTime = lTime + 2 * 3600;
    }

    memset(caCont, 0, sizeof(caCont));
    if(strlen(caSearchtype) > 0)
    {

        if(strlen(caApname) > 0)
        {
            sprintf(caCont + strlen(caCont), " and apname like '%c%s%c' ", '%', caApname, '%');
        }
        if(strlen(caServicecode) > 0)
        {
            sprintf(caCont + strlen(caCont), " and servicecode like '%c%s%c' ", '%', caServicecode, '%');
        }
        if(strlen(caCorpname) > 0)
        {
            sprintf(caCont + strlen(caCont), " and servicecode in (select username from ncsuser where dispname like '%c%s%c') ", '%', caCorpname, '%');
        }
    }
    else
    {
        if(strlen(caKeyword) > 0)
        {
            sprintf(caCont, " and (apname like '%c%s%c' or servicecode like '%c%s%c' ) ", '%', caKeyword, '%', '%', caKeyword, '%');
        }
    }

    if(strlen(caGroupid) > 0)
    {
        sprintf(caCont + strlen(caCont), " and servicecode in (%s)", getServicecodeByGroupid(psShmHead, caGroupid));
    }
    if(strlen(caGroups) > 0)
    {
        sprintf(caCont + strlen(caCont), " and servicecode in (%s)", getServicecodeByGroupid(psShmHead, caGroups));
    }




    for(i = 0; i < lCount; i++)
    {

        lCount1 = 0;
        memset(sql, 0, sizeof(sql));
        snprintf(sql, sizeof(sql), "select count(*) from %s where stime >= %lu and stime <= %lu  ",  ptmp[i], lStartTime, lTime);
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

    //
    for(i = num - 1; i >= 0; i--)
    {
        printf("table[%d] = %s, _lCount1[%d] = %lu\n", i, tableinfo[i].tname, i, tableinfo[i].lCount);
    }
    //

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
        snprintf(sql, sizeof(sql), "select servicecode,apname,longitude,latitude,stime from %s where stime >= %lu and stime <= %lu  ", tableinfo[i].tname, lStartTime, lTime);
        strcat(sql, caCont);

        snprintf(sql + strlen(sql), sizeof(sql) - strlen(sql), " order by stime desc limit %lu, %lu", iStart, iLimit < tableinfo[i].lCount ? iLimit : tableinfo[i].lCount);


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
            memset(caServicecode, 0, sizeof(caServicecode));
            memset(caServicename, 0, sizeof(caServicename));
            memset(caApname, 0, sizeof(caApname));
            lStarttime = 0;
            memset(caLongitude, 0, sizeof(caLongitude));
            memset(caLatitude, 0, sizeof(caLatitude));
            iret = pasDbFetchInto(psCur,
                                  UT_TYPE_STRING, 14, caServicecode,
                                  UT_TYPE_STRING, 31, caApname,
                                  UT_TYPE_STRING, 11, caLongitude,
                                  UT_TYPE_STRING, 11, caLatitude,
                                  UT_TYPE_LONG, 4, &lStarttime);



            while((iret == 0) || (iret == 1405))
            {
                iNum++;
                if(iNum > 1)
                {
                    utPltPutLoopVar(psDbHead, "dh", iNum, ",");
                }

                utPltPutLoopVar(psDbHead, "stime", iNum, utTimFormat("%y/%m/%d %H:%M", lStarttime));

                utPltPutLoopVar(psDbHead, "apname", iNum, caApname);
                //        utPltPutLoopVar(psDbHead,"servicename",iNum,caServicename);
                utPltPutLoopVar(psDbHead, "longitude", iNum, caLongitude);
                utPltPutLoopVar(psDbHead, "latitude", iNum, caLatitude);
                utPltPutLoopVar(psDbHead, "servicecode", iNum, caServicecode);



                memset(caServicecode, 0, sizeof(caServicecode));
                memset(caServicename, 0, sizeof(caServicename));
                memset(caApname, 0, sizeof(caApname));
                lStarttime = 0;
                memset(caLongitude, 0, sizeof(caLongitude));
                memset(caLatitude, 0, sizeof(caLatitude));

                lVtype = 0;
                iret = pasDbFetchInto(psCur,
                                      UT_TYPE_STRING, 14, caServicecode,
                                      UT_TYPE_STRING, 31, caApname,
                                      UT_TYPE_STRING, 11, caLongitude,
                                      UT_TYPE_STRING, 11, caLatitude,
                                      UT_TYPE_LONG, 4, &lStarttime);
            }
            pasDbCloseCursor(psCur);
        }

        if(_break == 1)
            break;
    }



    utPltOutToHtml(iFd, psMsgHead, psDbHead, "rzcx/proauth_mobileap_list.htm");
    return 0;
}




int ncsWebFacComp_mesg(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead);

int hhas(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead)
{
	 utPltDbHead *psDbHead = utPltInitDb();
	int  iNum = 1;
    utMsgPrintMsg(psMsgHead);
	utPltPutLoopVar(psDbHead, "longitude", iNum, "121");
                utPltPutLoopVar(psDbHead, "latitude", iNum, "31");
	 utPltOutToHtml(iFd, psMsgHead, psDbHead, "rzcx/proauth_mobileap_list.htm");
	return 0;
}
int proauthInitWeb001Fun(utShmHead *psShmHead)
{
    pasSetTcpFunName("proauthRzauthlist", proauthRzauthlist, 0);
	pasSetTcpFunName("hhas", hhas, 0);
    pasSetTcpFunName("proauthIPLogList", proauthIPLogList, 0);
    pasSetTcpFunName("proauthWebAplist", proauthWebAplist, 0);
    pasSetTcpFunName("proauth_apset_Save", proauth_apset_Save, 0);
    pasSetTcpFunName("lan_showstatus", lan_showstatus, 0);
    pasSetTcpFunName("ncAdminUserSave_v8", ncAdminUserSave_v8, 0);
    pasSetTcpFunName("proauthExit", proauthExit, 0);
    pasSetTcpFunName("proauth_logparsave", proauth_logparsave, 0);
    pasSetTcpFunName("proauth_logparset", proauth_logparset, 0);

    pasSetTcpFunName("proauth_logftpparset", proauth_logftpparset, 0);
    pasSetTcpFunName("proauth_logftpparsave", proauth_logftpparsave, 0);

    pasSetTcpFunName("proauth_warnoffparset", proauth_warnoffparset, 0);
    pasSetTcpFunName("proauth_warnoffparsave", proauth_warnoffparsave, 0);

    pasSetTcpFunName("proauth_whsave_v8", proauth_whsave_v8, 0);
    pasSetTcpFunName("proauthWhloglist", proauthWhloglist, 0);
    pasSetTcpFunName("proauthMesgloglist", proauthMesgloglist, 0);
    pasSetTcpFunName("ncsWebFacComp_mesg", ncsWebFacComp_mesg, 0);
    pasSetTcpFunName("proauth_mesgsave_v8", proauth_mesgsave_v8, 0);
    pasSetTcpFunName("ncsHotinfolist_v4", ncsHotinfolist_v4, 0);
    pasSetTcpFunName("ncsMactermlist_v4", ncsMactermlist_v4, 0);
    pasSetTcpFunName("ncsMobileAplist_v4", ncsMobileAplist_v4, 0);
    return 0;
}
