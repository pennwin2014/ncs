/*
     Modify 2015/8/4   by zyn*/
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
#include "utoall.h"
#include "utoplt01.h"
#include "pasdb.h"
#include "dsutl.h"
#include "dsclt.h"
#include "ncsdef.h"
#include"dlfcn.h"
#include <time.h>
#include "ncapi.h"

#define PLACE_TIMEOUT_SECONDS 60*60




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





//查询热点管理的mac
int  ncsSys_all_mac(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead)
{
    char     caTemp[1024] = "";
    char   caTemp_d[128] = "";
    //用于判断游标状态
    int iReturn = 0;
    //用于判断行数
    int iNum = 0;
    char cassid[128] = "";
    char mac[128] = "";
    char sql[1024] = "";
    char caSitename[64]="";
    pasDbCursor *psCur = NULL;
	long alarmDebug = utComGetVar_ld(psShmHead, "AlarmDebug", 0);
    utPltDbHead *psDbHead = utPltInitDb();  // 初始化数据模板
    utMsgPrintMsg(psMsgHead);

    iReturn = utMsgGetSomeNVar(psMsgHead, 1,
                               "ssid", UT_TYPE_STRING, 125, cassid
                              );

    if(strlen(cassid) > 0)
    {
        strcpy(caTemp_d,  pasCvtGBK(2, cassid, caTemp, 129));
        strcpy(cassid, caTemp_d);
    }
    //查询全部数据
    sprintf(sql, "select mac,sitename from  nchotmacgroup  where  nchotmacgroup.ssid=\'%s\'", cassid);
	if(alarmDebug)
	{
		printf("46 my  sql=%s\n", sql);	
	}	    
    //执行sql语句
    psCur = pasDbOpenSql(sql, 0);
	if(alarmDebug){
    printf("177 my  sql=%s\n", sql);
     }
    //把数据从游标中取出,返回值是1405时，还有数据，要继续取出
    iReturn = pasDbFetchInto(psCur, UT_TYPE_STRING, sizeof(mac) - 1,    mac,
	                                UT_TYPE_STRING,sizeof(caSitename)-1,caSitename    
                            );
    //
    while(0 == iReturn || 1405 == iReturn)
    {
        iNum++;

        if(iNum > 1)
        {
            //当循环到第二行，增加一个逗号
            utPltPutLoopVar(psDbHead, "dh", iNum, ","); // utPltPutLoopVar输出一个循环变量的值。
        }
        utPltPutLoopVar(psDbHead, "mac", iNum, mac);
		utPltPutLoopVar(psDbHead, "sitename", iNum, caSitename);
        iReturn = pasDbFetchInto(psCur, UT_TYPE_STRING, sizeof(mac) - 1,    mac,
		                                UT_TYPE_STRING,sizeof(caSitename)-1,caSitename   
                                );

    }
    //关闭游标
    pasDbCloseCursor(psCur);
    //把数据以json
    utPltOutToHtml(iFd, psMsgHead, psDbHead, "/normally/proauth_Sysoperation_mac.htm");

    return 0;


}

//查询  合规热点管理
int  ncsSysoperation(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead)
{
    long lCount = 0;
    long lTime, lstime, letime;
    char sql[1024] = "";
    char add[32] = "";
    //定义字符数组(装查询的数据)
    char  servicetype[128] = "";
    char servicequlity[128] = "";
    char ISPphone[128] = "";
    char ISPemail[128] = "";
    char security[128] = "";
    char authflag[128] = "";
    char ssid[1024] = "";
    char ISPname[128] = "";
    char mac[32] = "";
    char   caTemp_d[128] = "";
    char     caCid[1024];
    char   caTemp[1024];
    //用于判断游标状态
    int iReturn = 0;
    //用于判断行数
    int iNum = 0;
    char del[32] = "";
    char keyword[128] = "";
    char export2[12] = "";
    FILE *fp = NULL;
    char caFilename[100] = "";
    char caFile[240] = "";
    char caPath[240] = "";
    char cauthflag[128] = "";
    char caoldssid[64] = "";
    pasDbCursor *psCur = NULL;
    utPltDbHead *psDbHead = utPltInitDb();  // 初始化数据模板
    utMsgPrintMsg(psMsgHead);
    //接收数据
    //注意个数对应
    iReturn = utMsgGetSomeNVar(psMsgHead, 14,
                               "add", UT_TYPE_STRING, 30, add,
                               "ssid", UT_TYPE_STRING, 1023, ssid,
                               "security", UT_TYPE_STRING, 30, security,
                               "servicetype", UT_TYPE_STRING, 60, servicetype,
                               "servicequlity", UT_TYPE_STRING, 60, servicequlity,
                               "ISPname", UT_TYPE_STRING, 30, ISPname,
                               "ISPphone", UT_TYPE_STRING, 127, ISPphone,
                               "ISPemail", UT_TYPE_STRING, 127, ISPemail,
                               "del", UT_TYPE_STRING, 30, del,
                               "keyword", UT_TYPE_STRING, 127, keyword,
                               "export2", UT_TYPE_STRING, 10, export2,
                               "MAC", UT_TYPE_STRING, 30, mac,
                               "authflag", UT_TYPE_STRING, 127, cauthflag,
                               "oldssid", UT_TYPE_STRING, 63, caoldssid
                              );

   // printf("export2====[%s]\n", export2);

    if(strlen(mac) > 0)
    {
        strcpy(caTemp_d,  pasCvtGBK(2, mac, caTemp, 129));
        strcpy(mac, caTemp_d);
    }


    if(strlen(ssid) > 0)
    {
        strcpy(caTemp_d,  pasCvtGBK(2, ssid, caTemp, 1024));
        strcpy(ssid, caTemp_d);
    }
    if(strlen(caoldssid) > 0)
    {
        strcpy(caTemp_d,  pasCvtGBK(2, caoldssid, caTemp, 1024));
        strcpy(caoldssid, caTemp_d);
    }
    char strCondition[1024] = "";

    //执行条件查询
    if(strlen(keyword) > 0)
    {
        strcpy(caTemp_d,  pasCvtGBK(2, keyword, caTemp, 129));
        strcpy(keyword, caTemp_d);
        sprintf(strCondition + strlen(strCondition), " and nchotspotinfo.ssid  like '%c%s%c' ", '%', keyword, '%');
        sprintf(strCondition + strlen(strCondition), " or ISPname like '%c%s%c' ", '%', keyword, '%');
        sprintf(strCondition + strlen(strCondition), " or ISPphone like '%c%s%c' ", '%', keyword, '%');
        sprintf(strCondition + strlen(strCondition), " or ISPemail like '%c%s%c' ", '%', keyword, '%');
    }



    if(strlen(del) > 0)
    {
        memset(sql, 0, sizeof(sql));
        sprintf(sql, "delete from nchotspotinfo where ssid  in(%s) ", ssid);
        printf("\n173sql=%s\n", sql);
        pasDbExecSqlF(sql);
    }




    if(strlen(add) > 0)
    {
        printf("add=[%s]\n", add);
        if(strlen(ssid) > 0)
        {
            strcpy(caTemp_d,  pasCvtGBK(2, ssid, caTemp, 129));
            strcpy(ssid, caTemp_d);
        }

        if(strlen(security) > 0)
        {
            strcpy(caTemp_d,  pasCvtGBK(2, security, caTemp, 129));
            strcpy(security, caTemp_d);
        }

        if(strlen(servicequlity) > 0)
        {
            strcpy(caTemp_d,  pasCvtGBK(2, servicequlity, caTemp, 129));
            strcpy(servicequlity, caTemp_d);
        }

        if(strlen(ISPname) > 0)
        {
            strcpy(caTemp_d,  pasCvtGBK(2, ISPname, caTemp, 129));
            strcpy(ISPname, caTemp_d);
        }

        if(strlen(ISPemail) > 0)
        {
            strcpy(caTemp_d,  pasCvtGBK(2, ISPemail, caTemp, 129));
            strcpy(ISPemail, caTemp_d);
        }

        if(strlen(ISPphone) > 0)
        {
            strcpy(caTemp_d,  pasCvtGBK(2, ISPphone, caTemp, 129));
            strcpy(ISPphone, caTemp_d);
        }

        sprintf(sql, "select count(*) from nchotspotinfo where ssid=\'%s\' ", ssid);

       
        iReturn = pasDbOneRecord(sql, 0, UT_TYPE_LONG, 4, &lCount);


        if(strlen(caoldssid) > 0)
        {
            memset(sql, 0, sizeof(sql));
            sprintf(sql, "update nchotspotinfo set security='%s',servicequlity='%s',ISPname='%s',ISPphone='%s',ISPemail='%s',servicetype=\'%s\',authflag=\'%s\',ssid=\'%s\' where ssid=\'%s\'", security, servicequlity, ISPname, ISPphone, ISPemail, servicetype, cauthflag, ssid, caoldssid);
            pasDbExecSqlF(sql);
            memset(sql, 0, sizeof(sql));
            sprintf(sql, "update nchotmacgroup set mac=\'%s\',ssid=\'%s\' where ssid=\'%s\'", mac, ssid, caoldssid);
            printf("sq168=[%s]\n", sql);
            pasDbExecSqlF(sql);
            memset(sql, 0, sizeof(sql));
        }
        else
        {

            memset(sql, 0, sizeof(sql));
            sprintf(sql, " insert into nchotspotinfo(ssid,security,servicequlity,ISPname,ISPphone,ISPemail,servicetype,authflag)VALUES(\'%s\',\'%s\',\'%s\',\'%s\',\'%s\',\'%s\',\'%s\',\'s\')", ssid, security, servicequlity, ISPname, ISPphone, ISPemail, servicetype, cauthflag);
            pasDbExecSqlF(sql);
            memset(sql, 0, sizeof(sql));
            sprintf(sql, " insert into nchotmacgroup(ssid,mac)VALUES(\'%s\',\'%s\')", ssid, mac);
            pasDbExecSqlF(sql);
            memset(sql, 0, sizeof(sql));

        }

    }

    //查询全部数据
    sprintf(sql, "select security,authflag,nchotspotinfo.ssid,ISPname,servicetype,servicequlity,ISPphone,ISPemail,mac from nchotspotinfo,nchotmacgroup where  nchotspotinfo.ssid=nchotmacgroup.ssid  ");

    if(strlen(strCondition) > 0)
    {
        sprintf(sql + strlen(sql), " %s ", strCondition);
    }
    printf("174 my  sql=%s\n", sql);
    //执行sql语句
    psCur = pasDbOpenSql(sql, 0);
    printf("177 my  sql=%s\n", sql);
    if(strlen(export2) > 0)
    {
        printf("file export===============");
        sprintf(caFilename, "wifi%d.csv", utStrGetId());
        sprintf(caPath, "%s/html", "/home/ncmysql/ncs");
        sprintf(caFile, "%s/%s", caPath, caFilename);
        fp = fopen(caFile, "w");
        if(fp == NULL)
        {
            printf("fp error \n");
            return 0;
        }
        else
        {
            fprintf(fp, "热点管理\n");
            ncUtlPrintCsv(fp, 9,
                          UT_TYPE_STRING, "热点加密类型",
                          UT_TYPE_STRING, "是否合规",
                          UT_TYPE_STRING, "热点SSID",
                          UT_TYPE_STRING, "联系人",
                          UT_TYPE_STRING, "场所服务类型",
                          UT_TYPE_STRING, "场所经营性质",
                          UT_TYPE_STRING, "联系电话",
                          UT_TYPE_STRING, "运营机构邮箱",
                          UT_TYPE_STRING, "热点mac"
                         );
        }
    }
    //把数据从游标中取出,返回值是1405时，还有数据，要继续取出
    iReturn = pasDbFetchInto(psCur, UT_TYPE_STRING,  sizeof(security) - 1,      security,
                             UT_TYPE_STRING,  sizeof(authflag) - 1,      authflag,
                             UT_TYPE_STRING,  sizeof(ssid) - 1,          ssid,
                             UT_TYPE_STRING, sizeof(ISPname) - 1,      ISPname,
                             UT_TYPE_STRING, sizeof(servicetype) - 1,      servicetype,
                             UT_TYPE_STRING, sizeof(servicequlity) - 1,    servicequlity,
                             UT_TYPE_STRING, sizeof(ISPphone) - 1,    ISPphone,
                             UT_TYPE_STRING, sizeof(ISPemail) - 1,    ISPemail,
                             UT_TYPE_STRING, sizeof(mac) - 1,    mac
                            );
    //
    while(0 == iReturn || 1405 == iReturn)
    {
        iNum++;
        if(strlen(export2) > 0)
        {

            ncUtlPrintCsv(fp, 9, UT_TYPE_STRING, security,
                          UT_TYPE_STRING, authflag,
                          UT_TYPE_STRING, ssid,
                          UT_TYPE_STRING, ISPname,
                          UT_TYPE_STRING, servicetype,
                          UT_TYPE_STRING, servicequlity,
                          UT_TYPE_STRING, ISPphone,
                          UT_TYPE_STRING, ISPemail,
                          UT_TYPE_STRING, mac
                         );
        }

        if(iNum > 1)
        {
            //当循环到第二行，增加一个逗号
            utPltPutLoopVar(psDbHead, "dh", iNum, ","); // utPltPutLoopVar输出一个循环变量的值。
        }
        utPltPutLoopVar(psDbHead, "security", iNum, security);
        utPltPutLoopVar(psDbHead, "authflag", iNum, authflag);
        utPltPutLoopVar(psDbHead, "ssid", iNum, ssid);
        utPltPutLoopVar(psDbHead, "ISPname", iNum, ISPname);
        utPltPutLoopVar(psDbHead, "servicetype", iNum, servicetype);
        utPltPutLoopVar(psDbHead, "servicequlity", iNum, servicequlity);
        utPltPutLoopVar(psDbHead, "ISPphone", iNum, ISPphone);
        utPltPutLoopVar(psDbHead, "ISPemail", iNum, ISPemail);
        utPltPutLoopVar(psDbHead, "mac", iNum, mac);

        iReturn = pasDbFetchInto(psCur, UT_TYPE_STRING,  sizeof(security) - 1,      security,
                                 UT_TYPE_STRING,  sizeof(authflag) - 1,      authflag,
                                 UT_TYPE_STRING,  sizeof(ssid) - 1,          ssid,
                                 UT_TYPE_STRING, sizeof(ISPname) - 1,      ISPname,
                                 UT_TYPE_STRING, sizeof(servicetype) - 1,      servicetype,
                                 UT_TYPE_STRING, sizeof(servicequlity) - 1,    servicequlity,
                                 UT_TYPE_STRING, sizeof(ISPphone) - 1,    ISPphone,
                                 UT_TYPE_STRING, sizeof(ISPemail) - 1,    ISPemail,
                                 UT_TYPE_STRING, sizeof(mac) - 1,    mac
                                );

    }
	 utPltPutVarF(psDbHead, "TotRec", "%d", lCount);
    //关闭游标
    pasDbCloseCursor(psCur);
   // printf("after close cursor export2=[%s]", export2);
    if(strlen(export2) > 0)
    {
        fclose(fp);
        utPltFileDownload(iFd, "application/text", caPath, caFilename, caFilename);
        remove(caFile);
        utPltFreeDb(psDbHead);
        return 0;
    }
    //把数据以json
    utPltOutToHtml(iFd, psMsgHead, psDbHead, "/normally/proauth_Sysoperation.htm");

    return 0;
}


//查询布控管理数据
int ncsSys_controlmanage(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead)
{
    int lCount = 0;
    char caUpdate[16] = "";
    char caDel[16];
    char caSesid[1024];
    char sql[1024] = "";
    char  sid[128] = "";

    char casecode[128] = "";
    char casename[128] = "";
    char  casetype[128] = "";
    char caseclass[128] = "";
    char groupid[128] = "";
    char casedept[128] = "";
    char casedate[128] = "";
    char addby[128] = "";
    char cmemo[128] = "";
    char  lasttime[128] = "";
    char  expiretime[128] = "";
    char   flags[128] = "";
    char caKeyword[64] = "";
    //用于判断游标状态
    int iReturn = 0;
    //用于判断行数
    int iNum = 0;
    char   caTemp_d[128] = "";
    char   caTemp[1024] = "";


    char    lSid[128] = "";

    char wcount[128] = "";
    char wdate[128] = "";
    char insert[128] = "";
    char  caPage[11] = "";
    char   caLimit[11] = "";
    int capage = 0;
    int ilimit = 0;
    int castart = 0;
    long iCount = 0;


    utMsgPrintMsg(psMsgHead);
    pasDbCursor *psCur = NULL;
    utPltDbHead *psDbHead = utPltInitDb();  // 初始化数据模板

    iReturn = utMsgGetSomeNVar(psMsgHead, 16,
                               "del", UT_TYPE_STRING, 10, caDel,
                               "selsid", UT_TYPE_STRING, 1000, caSesid,
                               "update", UT_TYPE_STRING, 1000, caUpdate,
                               "keyword", UT_TYPE_STRING, 64, caKeyword,

                               "sid", UT_TYPE_STRING, 64, sid,
                               "flags", UT_TYPE_STRING, 64, flags,
                               "casename", UT_TYPE_STRING, 64, casename,
                               "casedate", UT_TYPE_STRING, 64, casedate,
                               "expiretime", UT_TYPE_STRING, 64, expiretime,
                               "groupid", UT_TYPE_STRING, 64, groupid,
                               "cmemo", UT_TYPE_STRING, 64, cmemo,
                               "wcount", UT_TYPE_STRING, 64, wcount,
                               "wdate", UT_TYPE_STRING, 64, wdate,
                               "insert", UT_TYPE_STRING, 64, insert,
                               "limit", UT_TYPE_STRING, 10, caLimit,
                               "page", UT_TYPE_STRING, 10, caPage
                              );

    ilimit = atoi(caLimit);
    capage = atoi(caPage);
   // printf("1111111calimit=[%d],capage=[%d]", ilimit, capage);
    if(strlen(sid) > 0)
    {
        strcpy(caTemp_d, pasCvtGBK(2, sid, caTemp, 60));
        strcpy(sid, caTemp_d);
    }
    if(strlen(flags) > 0)
    {
        strcpy(caTemp_d, pasCvtGBK(2, flags, caTemp, 60));
        strcpy(flags, caTemp_d);
    }
    if(strlen(casename) > 0)
    {
        strcpy(caTemp_d, pasCvtGBK(2, casename, caTemp, 60));
        strcpy(casename, caTemp_d);
    }
    if(strlen(casedate) > 0)
    {
        strcpy(caTemp_d, pasCvtGBK(2, casedate, caTemp, 60));
        strcpy(casedate, caTemp_d);
    }
    if(strlen(groupid) > 0)
    {
        strcpy(caTemp_d, pasCvtGBK(2, groupid, caTemp, 60));
        strcpy(groupid, caTemp_d);
    }
    if(strlen(cmemo) > 0)
    {
        strcpy(caTemp_d, pasCvtGBK(2, cmemo, caTemp, 60));
        strcpy(cmemo, caTemp_d);
    }
    if(strlen(wcount) > 0)
    {
        strcpy(caTemp_d, pasCvtGBK(2, wcount, caTemp, 60));
        strcpy(wcount, caTemp_d);
    }
    if(strlen(wdate) > 0)
    {
        strcpy(caTemp_d, pasCvtGBK(2, wdate, caTemp, 60));
        strcpy(wdate, caTemp_d);
    }
    if(strlen(expiretime) > 0)
    {
        strcpy(caTemp_d, pasCvtGBK(2, expiretime, caTemp, 60));
        strcpy(expiretime, caTemp_d);
    }
    if(strlen(insert) > 0)
    {
        strcpy(caTemp_d, pasCvtGBK(2, insert, caTemp, 60));
        strcpy(insert, caTemp_d);
    }



    if(strlen(caKeyword) > 0)
    {
        strcpy(caTemp_d, pasCvtGBK(2, caKeyword, caTemp, 60));
        strcpy(caKeyword, caTemp_d);
    }

   // printf(".................insert =%s.................\n", insert);
  //  printf(".................sid =%s.................\n", sid);


   // printf("keyword=%s\n", caUpdate);
    //更新状态
    if(strlen(caUpdate) > 0)
    {
        memset(sql, 0, sizeof(sql));
        // sprintf(sql, "update ncscaseinfo set result = %d where sid in (%s) and result != 2",caUpdate, caSesid);
        sprintf(sql, "update ncscaseinfo set flags = %s where sid in (%s) and flags != 2", caUpdate, caSesid);
        pasDbExecSqlF(sql);
       // printf("sql = %s", sql);
    }

    //增加或者插入
  //  printf(".................insert =%s.................\n", insert);
    if(strlen(insert) > 0)
    {

        lCount = 0;

       // printf("insertyyyyyyinsert=%s\n", insert);
        sprintf(sql, "select count(*) from ncscaseinfo where sid=\'%s\' ", sid);
       // printf("sql11111=%s\n", sql);
        //iReturn = pasDbFetchInto( psCur,UT_TYPE_LONG,  sizeof(lCount),  &lCount);
        iReturn = pasDbOneRecord(sql, 0, UT_TYPE_LONG, 4, &lCount);
        //printf("lCountyyyyyylCount=%d\n", lCount);

        if(lCount > 0)
        {

            memset(sql, 0, sizeof(sql));
            sprintf(sql, "update ncscaseinfo set flags='%s',casename='%s',casedate='%s',expiretime='%s',groupid='%s',cmemo='%s' where sid=\'%s\'", flags, casename, casedate, expiretime, groupid, cmemo, sid);
            pasDbExecSqlF(sql);
          //  printf("sql = [%s]\n", sql);
        }
        else
        {
            memset(sql, 0, sizeof(sql));
            sprintf(sql, "insert into ncscaseinfo(flags,casename,casedate,expiretime,groupid,cmemo)\
          	  values('%s','%s','%s','%s','%s','%s','%s')",
                    flags, casename, casedate, expiretime, groupid, cmemo);
            pasDbExecSqlF(sql);
           // printf("insert..............sql = [%s]\n", sql);
        }



    }



    //删除
    if(strlen(caDel) > 0)
    {
     //   printf("del start");
        memset(sql, 0, sizeof(sql));
    //    printf("delete from ncscaseinfo where groupid in(%s)\n", caSesid);
        iReturn = pasDbExecSqlF("delete from ncscaseinfo where sid in(%s)", caSesid);
        sprintf(sql, "delete from ncscaseinfo where sid in(%s) ", caSesid);
        pasDbExecSqlF(sql);
      //  printf("del sql%s", sql);
      //  printf("sql = %s", sql);
    }

    //查询记录总数
    if(1)
    {
        memset(sql, 0, sizeof(sql));
        sprintf(sql, "select count(sid) from  ncscaseinfo where 1=1 ");
        if(strlen(caKeyword) > 0)
        {
            sprintf(sql + strlen(sql), " and (casename like '%c%s%c')", '%', caKeyword, '%');
        //    printf("chaxunsql%s\n", sql);
        }
        iReturn = pasDbOneRecord(sql, 0, UT_TYPE_LONG, 4, &iCount);
      //  printf("iCount = [%d]\n", iCount);
        utPltPutVarF(psDbHead, "TotRec", "%d", iCount);
      //  printf("iCount=[%d]", iCount);
    }



    sprintf(sql, "select  sid,casecode,casename,casetype,caseclass,groupid,casedept,casedate,addby,cmemo,lasttime,expiretime,flags  from ncscaseinfo  where 1=1");
    if(strlen(caKeyword) > 0)
    {
        sprintf(sql + strlen(sql), " and (casename like '%c%s%c')", '%', caKeyword, '%');
      //  printf("chaxunsql%s\n", sql);
    }

    //分页功能的实现

    castart = (capage - 1) * ilimit;

    // ilimit = ilimit < (iCount - castart) ?  (capage * ilimit) : iCount;
 //   printf("222ilimit=[%d]\n", ilimit);
    sprintf(sql + strlen(sql), " order by  sid  desc limit %d,%d", castart, ilimit);


  //  printf("ffffffff%s\n", sql);
    //执行sql语句
    psCur = pasDbOpenSql(sql, 0);



    iReturn = pasDbFetchInto(
                  psCur, UT_TYPE_STRING,  sizeof(sid) - 1,      sid,
                  UT_TYPE_STRING,  sizeof(casecode) - 1,      casecode,
                  UT_TYPE_STRING,  sizeof(casename) - 1,      casename,
                  UT_TYPE_STRING,  sizeof(casetype) - 1,     casetype,
                  UT_TYPE_STRING,  sizeof(caseclass) - 1,    caseclass,
                  UT_TYPE_STRING,  sizeof(groupid) - 1,   groupid,
                  UT_TYPE_STRING,  sizeof(casedept) - 1,   casedept,
                  UT_TYPE_STRING,  sizeof(casedate) - 1,   casedate,
                  UT_TYPE_STRING,  sizeof(addby) - 1,  addby,
                  UT_TYPE_STRING,  sizeof(cmemo) - 1,  cmemo,
                  UT_TYPE_STRING,  sizeof(lasttime) - 1, lasttime,
                  UT_TYPE_STRING,  sizeof(expiretime) - 1, expiretime,
                  UT_TYPE_STRING,  sizeof(flags) - 1, flags
              );
    while(0 == iReturn || 1405 == iReturn)
    {
        iNum++;
        if(iNum > 1)
        {
            //当循环到第二行，增加一个逗号
            utPltPutLoopVar(psDbHead, "dh", iNum, ","); // utPltPutLoopVar输出一个循环变量的值。
        }
        utPltPutLoopVar(psDbHead, "sid", iNum, sid);
        utPltPutLoopVar(psDbHead, "casecode", iNum, casecode);
        utPltPutLoopVar(psDbHead, "casename", iNum, casename);
        utPltPutLoopVar(psDbHead, "casetype", iNum, casetype);
        utPltPutLoopVar(psDbHead, "caseclass", iNum, caseclass);
        utPltPutLoopVar(psDbHead, "groupid", iNum, groupid);
        utPltPutLoopVar(psDbHead, "casedept", iNum, casedept);
        utPltPutLoopVar(psDbHead, "casedate", iNum, casedate);
        utPltPutLoopVar(psDbHead, "addby", iNum, addby);
        utPltPutLoopVar(psDbHead, "cmemo", iNum, cmemo);
        utPltPutLoopVar(psDbHead, "lasttime", iNum, lasttime);
        utPltPutLoopVar(psDbHead, "expiretime", iNum, expiretime);
        utPltPutLoopVar(psDbHead, "flags", iNum, flags);

        iReturn = pasDbFetchInto(
                      psCur, UT_TYPE_STRING,  sizeof(sid) - 1,      sid,
                      UT_TYPE_STRING,  sizeof(casecode) - 1,      casecode,
                      UT_TYPE_STRING,  sizeof(casename) - 1,      casename,
                      UT_TYPE_STRING,  sizeof(casetype) - 1,     casetype,
                      UT_TYPE_STRING,  sizeof(caseclass) - 1,    caseclass,
                      UT_TYPE_STRING,  sizeof(groupid) - 1,   groupid,
                      UT_TYPE_STRING,  sizeof(casedept) - 1,   casedept,
                      UT_TYPE_STRING,  sizeof(casedate) - 1,   casedate,
                      UT_TYPE_STRING,  sizeof(addby) - 1,  addby,
                      UT_TYPE_STRING,  sizeof(cmemo) - 1,  cmemo,
                      UT_TYPE_STRING,  sizeof(lasttime) - 1, lasttime,
                      UT_TYPE_STRING,  sizeof(expiretime) - 1, expiretime,
                      UT_TYPE_STRING,  sizeof(flags) - 1, flags
                  );
    }
    //关闭游标
    pasDbCloseCursor(psCur);
    utPltOutToHtml(iFd, psMsgHead, psDbHead, "/normally/proauth_controlmanage.htm");
    return 0;
}

//统计数据表 接警人员管理

int ncsSys_stafinfo(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead)
{
    long  lCount;
    char sql[1024] = "";
    char add[30] = "";
    char del[10] = "";
    char caKeyword[64] = "";

    char  sid[1024] = "";
    char  name[16] = "";
    char  department[128] = "";
    char   phone[128] = "";
    char  email[128] = "";
    char   caTemp_d[128] = "";
    char  caCid[1024];
    char   caTemp[1024];
    char  caPage[11] = "";
    char   caLimit[11] = "";
    char  casename[128] = "";
    int capage = 0;
    int ilimit = 0;
    long flags = 0;
    long stime = 0;
    int castart = 0;
    long iCount = 0;
    int iReturn = 0;
    char export2[12] = "";
    FILE *fp = NULL;
    char caFilename[100] = "";
    char caFile[240] = "";
    char caPath[240] = "";

    pasDbCursor *psCur = NULL;
    utPltDbHead *psDbHead = utPltInitDb();  // 初始化数据模板
    utMsgPrintMsg(psMsgHead);
    //用于判断行数
    int iNum = 0;
  //  printf("ncsSys_stafinfo  start..............");
    iReturn = utMsgGetSomeNVar(psMsgHead, 11,
                               "add", UT_TYPE_STRING, 30, add,
                               "name", UT_TYPE_STRING, 15, name,
                               "department", UT_TYPE_STRING, 30, department,
                               "phone", UT_TYPE_STRING, 60, phone,
                               "email", UT_TYPE_STRING, 60, email,
                               "del", UT_TYPE_STRING, 30, del,
                               "sid", UT_TYPE_STRING, 1024, sid,
                               "keyword", UT_TYPE_STRING, 64, caKeyword,
                               "limit", UT_TYPE_STRING, 10, caLimit,
                               "page", UT_TYPE_STRING, 10, caPage,
                               "export2", UT_TYPE_STRING, 10, export2
                              );

  //  printf("export2=[%s]\n", export2);
    ilimit = atoi(caLimit);
  //  printf("111 ilimit=[%d]\n", ilimit);
    capage = atoi(caPage);
   // printf("capage=[%d]\n", capage);
    if(strlen(caKeyword) > 0)
    {
        strcpy(caTemp_d, pasCvtGBK(2, caKeyword, caTemp, 60));
        strcpy(caKeyword, caTemp_d);
    }

    if(strlen(add) > 0)
    {
        if(strlen(name) > 0)
        {
            strcpy(caTemp_d,  pasCvtGBK(2, name, caTemp, 129));
            strcpy(name, caTemp_d);
        }
        if(strlen(department) > 0)
        {
            strcpy(caTemp_d,  pasCvtGBK(2, department, caTemp, 129));
            strcpy(department, caTemp_d);
        }
        if(strlen(phone) > 0)
        {
            strcpy(caTemp_d,  pasCvtGBK(2, phone, caTemp, 129));
            strcpy(phone, caTemp_d);
        }
        lCount = 0;

        memset(sql, 0, sizeof(sql));
        sprintf(sql, "select count(*) from stafinfo where sid=\'%s\' ", sid);


        iReturn = pasDbOneRecord(sql, 0, UT_TYPE_LONG, 4, &lCount);
     //   printf("lCount============ =========== %d\n", lCount);
        if(lCount > 0)
        {
            memset(sql, 0, sizeof(sql));
            sprintf(sql, "update stafinfo   set name=\'%s\',department=\'%s\', phone=\'%s\', email=\'%s\' where sid=\'%s\'", name, department, phone, email, sid);
         //   printf("update sql =========== %s\n", sql);
            pasDbExecSqlF(sql);
        }
        else
        {
            memset(sql, 0, sizeof(sql));
            sprintf(sql, " insert into stafinfo(name,department,phone,email)VALUE('%s','%s','%s','%s')", name, department, phone, email);
          //  printf("insert  sql ====== %s\n", sql);
            pasDbExecSqlF(sql);
        }
      //  printf("iReturn=%d\n", iReturn);
    }
    //删除
    if(strlen(del) > 0)
    {
     //   printf("del start");
        memset(sql, 0, sizeof(sql));
        sprintf(sql, "delete from stafinfo where sid in (%s)", sid);
        pasDbExecSqlF(sql);
      //  printf("del sql%s", sql);
      //  printf("sql = %s", sql);
    }
    //导出，创建打开文件

    if(strlen(export2) > 0)
    {
      //  printf("file export===============\n");
        sprintf(caFilename, "wifi%d.csv", utStrGetId());
        sprintf(caPath, "%s/html", "/home/ncmysql/ncs");
        sprintf(caFile, "%s/%s", caPath, caFilename);
        //打开文件
        fp = fopen(caFile, "w");
        //创建表头
        if(fp == NULL)
        {
      //      printf("fp error \n");
            return 0;
        }
        else
        {
            fprintf(fp, "接警人管理\n");
            ncUtlPrintCsv(fp, 5,
                          UT_TYPE_STRING, "姓名",
                          UT_TYPE_STRING, "所属部门",
                          UT_TYPE_STRING, "联系电话",
                          UT_TYPE_STRING, "联系邮箱",
                          UT_TYPE_STRING, "sid"
                         );
        }
    }

    //查询记录总数
    if(1)
    {
        memset(sql, 0, sizeof(sql));
        sprintf(sql, "select count(sid) from  stafinfo where 1=1 ");

        if(strlen(caKeyword) > 0)
        {
            sprintf(sql + strlen(sql), " and (name like '%c%s%c') or (department like '%c%s%c') or (phone like '%c%s%c')", '%', caKeyword, '%', '%', caKeyword, '%', '%', caKeyword, '%');
      //      printf("chaxunsql%s\n", sql);
         //   printf("sql = %s\n", sql);
        }
        iReturn = pasDbOneRecord(sql, 0, UT_TYPE_LONG, 4, &iCount);

        utPltPutVarF(psDbHead, "TotRec", "%d", iCount);

    }
    sprintf(sql, "select name,department,phone,email,sid from  stafinfo where 1=1");
    if(strlen(caKeyword) > 0)
    {
        sprintf(sql + strlen(sql), " and (name like '%c%s%c') or (department like '%c%s%c') or (phone like '%c%s%c')", '%', caKeyword, '%', '%', caKeyword, '%', '%', caKeyword, '%');
    }
    //分页功能的实现
    castart = (capage - 1) * ilimit;
    // ilimit = ilimit < (iCount - castart) ?  (capage * ilimit) : iCount;
    sprintf(sql + strlen(sql), " order by  sid  desc limit %d,%d", castart, ilimit);

 //   printf("000000000sql=[%s]", sql);
    //执行sql语句
    psCur = pasDbOpenSql(sql, 0);


    iReturn = pasDbFetchInto(
                  psCur, UT_TYPE_STRING,  sizeof(name) - 1,      name,
                  UT_TYPE_STRING,  sizeof(department) - 1,  department,
                  UT_TYPE_STRING,  sizeof(phone) - 1,      phone,
                  UT_TYPE_STRING,  sizeof(email) - 1,      email,
                  UT_TYPE_STRING,  sizeof(sid) - 1,      sid
              );

    while(0 == iReturn || 1405 == iReturn)
    {
        iNum++;
        //把需要导出的数据放入Csv文件中
        if(strlen(export2) > 0)
        {
            ncUtlPrintCsv(fp, 5, UT_TYPE_STRING, name,
                          UT_TYPE_STRING, department,
                          UT_TYPE_STRING, phone,
                          UT_TYPE_STRING, email,
                          UT_TYPE_STRING, sid
                         );
        }
        if(iNum > 1)
        {
            //当循环到第二行，增加一个逗号
            utPltPutLoopVar(psDbHead, "dh", iNum, ","); // utPltPutLoopVar输出一个循环变量的值。
        }
        utPltPutLoopVar(psDbHead, "name", iNum, name);
        utPltPutLoopVar(psDbHead, "department", iNum, department);
        utPltPutLoopVar(psDbHead, "phone", iNum, phone);
        utPltPutLoopVar(psDbHead, "email", iNum, email);
        utPltPutLoopVar(psDbHead, "sid", iNum, sid);

        iReturn = pasDbFetchInto(
                      psCur,  UT_TYPE_STRING,  sizeof(name) - 1,      name,
                      UT_TYPE_STRING,  sizeof(department) - 1,      department,
                      UT_TYPE_STRING,  sizeof(phone) - 1,      phone,
                      UT_TYPE_STRING,  sizeof(email) - 1,      email,
                      UT_TYPE_STRING,  sizeof(sid) - 1,     sid
                  );
    }

    //关闭游标
    pasDbCloseCursor(psCur);
   // printf("after close cursor export2=[%s]\n", export2);
    if(strlen(export2) > 0)
    {
        fclose(fp);
        utPltFileDownload(iFd, "application/text", caPath, caFilename, caFilename);
        remove(caFile);
        utPltFreeDb(psDbHead);
        return 0;
    }
    utPltOutToHtml(iFd, psMsgHead, psDbHead, "/normally/proauth_stafinfo.htm");
    return 0;

}
//查询告警处理信息
int   ncsSys_stafinfo_deal(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead)
{
    int sid = 0;
    char casid[16] = "";
    char caTemp[1024] = "";
    char caTemp_d[1024] = "";
    long flags = 0;
    long stime = 0;
    char  casename[128] = "";
    char caupdate[16] = "";
    char caflags[16] = "";
    char name[64] = "";
    char sql[1024] = "";
    char keyword[128] = "";
    pasDbCursor *psCur = NULL;
    utPltDbHead *psDbHead = utPltInitDb();  // 初始化数据模板
    utMsgPrintMsg(psMsgHead);
    int iReturn = 0;
    int iNum = 0;

    iReturn = utMsgGetSomeNVar(psMsgHead, 5,
                               "name", UT_TYPE_STRING, 63, name,
                               "update", UT_TYPE_STRING, 15, caupdate,
                               "sid", UT_TYPE_STRING, 15, casid,
                               "flags", UT_TYPE_STRING, 15, caflags,
                               "keyword", UT_TYPE_STRING, 127, keyword
                              );
    char strCondition[1024] = "";
    if(strlen(keyword) > 0)
    {
        strcpy(caTemp_d, pasCvtGBK(2, keyword, caTemp, 129));
        strcpy(keyword, caTemp_d);
        sprintf(strCondition + strlen(strCondition), " and casename  like '%c%s%c' ", '%', keyword, '%');
        sprintf(strCondition + strlen(strCondition), " or stime like '%c%s%c' ", '%', keyword, '%');

    }

    if(strlen(caupdate) > 0)
    {
        if(strlen(casid) > 0)
        {
            sid = atoi(casid);
        }
        if(strlen(caflags) > 0)
        {
            flags = atoi(caflags);
        }
        sprintf(sql, "update ncscasemacwarnlog set flags=%d", flags);
       // printf("933sql=[%s]\n", sql);
        psCur = pasDbOpenSql(sql, 0);
    }
    sprintf(sql, "SELECT ncscasemacwarnlog.sid,ncscasemacwarnlog.flags,stime,casename FROM ncscasemacwarnlog,ncscaseinfo  WHERE  ncscasemacwarnlog.cid=ncscaseinfo.casecode and ncscasemacwarnlog.opname=\'%s\' ", name);
    if(strlen(strCondition) > 0)
    {
        sprintf(sql + strlen(sql), " %s ", strCondition);
    }
 //   printf("===NO==sql=%s\n", sql);
    psCur = pasDbOpenSql(sql, 0);

    iReturn = pasDbFetchInto(
                  psCur, UT_TYPE_LONG,        4,              &sid,
                  UT_TYPE_LONG,        4,              &flags,
                  UT_TYPE_LONG,         4,             &stime,
                  UT_TYPE_STRING,  sizeof(casename) - 1,      casename
              );

    while(0 == iReturn || 1405 == iReturn)
    {
        iNum++;
        if(iNum > 1)
        {
            //当循环到第二行，增加一个逗号
            utPltPutLoopVar(psDbHead, "dh", iNum, ","); // utPltPutLoopVar输出一个循环变量的值。
        }
        utPltPutLoopVarF(psDbHead, "sid", iNum, "%lu", sid);
        utPltPutLoopVarF(psDbHead, "flags", iNum, "%lu", flags);
        utPltPutLoopVarF(psDbHead, "stime", iNum, "%lu", stime);
        utPltPutLoopVar(psDbHead, "casename", iNum, casename);

        iReturn = pasDbFetchInto(
                      psCur, UT_TYPE_LONG,     4,      &sid,
                      UT_TYPE_LONG,   4,      &flags,
                      UT_TYPE_LONG,    4,      &stime,
                      UT_TYPE_STRING,  sizeof(casename) - 1, casename
                  );

    }
  //  printf("876sql=%s", sql);
    //关闭游标
    pasDbCloseCursor(psCur);
    utPltOutToHtml(iFd, psMsgHead, psDbHead, "/normally/proauth_stafinfo_deal.htm");

    return 0;

}

//查询终端信息分组成员
int   ncsSys_terminal(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead)
{
    char sql[1024] = "";
    char keyword[128] = "";
    char mac[128] = "";
    char caGroupid[128] = "";
    char del[128] = "";
    char add[128] = "";
    char caUpdate[16] = "";
    char caTemp[256] = "";
    char caTemp_d[256] = "";
    char strCondition[1024] = "";
    char strCondition_2[1024] = "";
    char groupname[128] = "";
    char groupdesc[128] = "";
    char caMAC[640] = "";
    long  long groupid = 0;
    int  iReturn = 0;
    char  caPage[11] = "";
    char 	caKid[32] = "";
    char   caLimit[11] = "";
    int capage = 0;
    int ilimit = 0;
    int castart = 0;
    long lCount = 0;
    long iCount = 0;
    char Icid[64] = "";
    int icid = 0; //分组id
    int iNum = 0;
    char  caSid[1024] = "";
    int sid = 0;
    int  kid = 0;
    char export2[16] = "";
    char caFilename[1024] = "";
    char caPath[1024] = "";
    char caFile[1024] = "";
	long alarmDebug = utComGetVar_ld(psShmHead, "AlarmDebug", 0);
	
    FILE *fp = NULL;

    pasDbCursor   *psCur = NULL;
    utPltDbHead *psDbHead = utPltInitDb();  // 初始化数据模板
    utMsgPrintMsg(psMsgHead);


    //一点要对应的个数
    iReturn = utMsgGetSomeNVar(psMsgHead, 14,
                               "groupid", UT_TYPE_STRING, 30, caGroupid,
                               "del", UT_TYPE_STRING, 30, del,
                               "add", UT_TYPE_STRING, 30, add,
                               "keyword", UT_TYPE_STRING, 100, keyword,
                               "groupname", UT_TYPE_STRING, 100, groupname,
                               "groupdesc", UT_TYPE_STRING, 100, groupdesc,
                               "limit", UT_TYPE_STRING, 10, caLimit,
                               "page", UT_TYPE_STRING, 10, caPage,
                               "cid", UT_TYPE_STRING, 60, Icid,
                               "sid", UT_TYPE_STRING, 1023, caSid,
                               "update", UT_TYPE_STRING, 15, caUpdate,
                               "mac", UT_TYPE_STRING, 127, caMAC,
                               "kid", UT_TYPE_STRING, 31, caKid,
                               "export2", UT_TYPE_STRING, 15, export2
                              );
    if(strlen(caLimit) > 0)
    {
        ilimit = atoi(caLimit);
    }
    if(strlen(caKid) > 0)
    {
        kid = atoi(caKid);
    }
    if(strlen(caPage) > 0)
    {
        capage = atoi(caPage);
    }
    if(strlen(Icid) > 0)
    {
        icid = atoi(Icid);
    }
    if(strlen(caSid) > 0)
    {
        sid = atoi(caSid);
    }
    if(strlen(caGroupid) > 0)
    {
        groupid = atoll(caGroupid);
    }
    //用id对表格进行选择
    if(strlen(Icid) > 0)
    {
        sprintf(strCondition + strlen(strCondition), " and nctermmacgroupinfo.groupid  =%llu ", icid);
    }


    if(strlen(caUpdate) > 0)
    {
        if(strlen(caMAC) > 0)
        {
            strcpy(caTemp_d,  pasCvtGBK(2, caMAC, caTemp, 129));
            strcpy(caMAC, caTemp_d);
        }
     //   printf("882caUpdate=%s\n", caUpdate);
        lCount = 0;
        memset(sql, 0, sizeof(sql));
        sprintf(sql, "select count(*) from nctermmacgroupinfo where sid=%d ", sid);
        iReturn = pasDbOneRecord(sql, 0, UT_TYPE_LONG, 4, &lCount);

        if(lCount > 0)
        {
            memset(sql, 0, sizeof(sql));
            sprintf(sql, "update nctermmacgroupinfo   set mac=\'%s\',groupid=%llu  where sid=%d", caMAC, groupid, sid);
           // printf("update sql =========== %s\n", sql);

            pasDbExecSqlF(sql);
        }
        else
        {
            memset(sql, 0, sizeof(sql));
            sprintf(sql, " insert into nctermmacgroupinfo(mac,groupid)VALUES('%s','%llu')", caMAC, groupid);
          if( 1==alarmDebug){	            
		  printf("insert  sql ====== %s\n", sql);
		  }
            pasDbExecSqlF(sql);
        }
        sprintf(strCondition + strlen(strCondition), " and nctermmacgroupinfo.groupid  =%llu ", groupid);
       // printf("iReturn=%d\n", iReturn);

    }
    if(strlen(add) > 0)
    {
        if(strlen(groupname) > 0)
        {
            strcpy(caTemp_d,  pasCvtGBK(2, groupname, caTemp, 129));
            strcpy(groupname, caTemp_d);
        }
        if(strlen(groupdesc) > 0)
        {
            strcpy(caTemp_d,  pasCvtGBK(2, groupdesc, caTemp, 129));
            strcpy(groupdesc, caTemp_d);
        }

        memset(sql, 0, sizeof(sql));
        sprintf(sql, " insert into nctermmacgroup(groupname,groupdesc)VALUES('%s','%s')", groupname, groupdesc);
        iReturn = pasDbExecSqlF(sql);

    }


    if(strlen(del) > 0)
    {
        memset(sql, 0, sizeof(sql));
        sprintf(sql, "delete from nctermmacgroupinfo  where sid in (%s)", caSid);
        pasDbExecSqlF(sql);

    }
    //执行条件查询
    if(strlen(keyword) > 0)
    {
        strcpy(caTemp_d,  pasCvtGBK(2, keyword, caTemp, 99));
        strcpy(keyword, caTemp_d);
        if(kid != 0)
        {
          sprintf(strCondition + strlen(strCondition), " and  sid=%d  ", kid);
        }
        sprintf(strCondition + strlen(strCondition), " and (groupname like '%c%s%c' ", '%', keyword, '%');
        sprintf(strCondition + strlen(strCondition), "	or 	groupdesc like '%c%s%c' ", '%', keyword, '%');
        sprintf(strCondition + strlen(strCondition), "	or	mac like '%c%s%c' ) ", '%', keyword, '%');
       // printf("1156strCondition=%s", strCondition);
    }
    sprintf(sql, "SELECT  sid,groupname,groupdesc,mac,nctermmacgroup.groupid  FROM   nctermmacgroup,nctermmacgroupinfo   WHERE  nctermmacgroup.groupid=nctermmacgroupinfo.groupid  ");


    if(strlen(strCondition) > 0)
    {
        sprintf(sql + strlen(sql), " %s ", strCondition);
    }

    //分页功能的实现
    castart = (capage - 1) * ilimit;
    ilimit = 40;
    // ilimit = ilimit < (iCount - castart) ?  (capage * ilimit) : iCount;
    sprintf(sql + strlen(sql), " order by sid  desc ");
    //sprintf(sql + strlen(sql),"  limit %d,%d", castart,ilimit);
    //执行sql语句
    psCur = pasDbOpenSql(sql, 0);
	if( 1==alarmDebug){	
    printf("1177sql==[%s]\n", sql);
	}
    if(strlen(export2) > 0)
    {
      //  printf("file export===============");
        sprintf(caFilename, "mac_group%d.csv", utStrGetId());
        sprintf(caPath, "%s/html", "/home/ncmysql/ncs");
        sprintf(caFile, "%s/%s", caPath, caFilename);
        fp = fopen(caFile, "w");
        if(fp == NULL)
        {
          //  printf("fp error \n");
            return 0;
        }
        else
        {
            fprintf(fp, "热点管理\n");
            ncUtlPrintCsv(fp, 4,
                          UT_TYPE_STRING, "SID",
                          UT_TYPE_STRING, "终端信息分组名称",
                          UT_TYPE_STRING, "终端分组成员信息",
                          UT_TYPE_STRING, "分组描述",
                          UT_TYPE_STRING, "groupid"
                         );
        }
    }
    iReturn = pasDbFetchInto(
                  psCur,  UT_TYPE_LONG,  4,      &sid,
                  UT_TYPE_STRING,  sizeof(groupname) - 1,      groupname,
                  UT_TYPE_STRING,  sizeof(groupdesc) - 1,    groupdesc,
                  UT_TYPE_STRING,  sizeof(mac) - 1,      mac,
                  UT_TYPE_LONG8,       8,              &groupid
              );
  //  printf("sql==[%s]\n", sql);
    while(0 == iReturn || 1405 == iReturn)
    {
        iNum++;
        if(strlen(export2) > 0)
        {
            ncUtlPrintCsv(fp, 9, UT_TYPE_LONG, sid,
                          UT_TYPE_STRING, groupname,
                          UT_TYPE_STRING, groupdesc,
                          UT_TYPE_STRING, mac,
                          UT_TYPE_LONG8, groupid
                         );
        }
     //   printf("1034iNum=%d", iNum);
        if(iNum > 1)
        {
            //当循环到第二行，增加一个逗号
            utPltPutLoopVar(psDbHead, "dh", iNum, ","); // utPltPutLoopVar输出一个循环变量的值。
        }
        utPltPutLoopVarF(psDbHead, "sid", iNum, "%ld", sid);
        utPltPutLoopVar(psDbHead, "groupname", iNum, groupname);
        utPltPutLoopVar(psDbHead, "groupdesc", iNum, groupdesc);
        utPltPutLoopVar(psDbHead, "mac", iNum, mac);
        utPltPutLoopVarF(psDbHead, "groupid", iNum, "%llu", groupid);
         if( 1==alarmDebug){	
		 printf("sid=[%ld] groupname=[%s] mac=[%s]\n",sid,groupname,groupdesc);
		 
		 }
        iReturn = pasDbFetchInto(
                      psCur,   UT_TYPE_LONG,  4,      &sid,
                      UT_TYPE_STRING,  sizeof(groupname) - 1,      groupname,
                      UT_TYPE_STRING,  sizeof(groupdesc) - 1,      groupdesc,
                      UT_TYPE_STRING,  sizeof(mac) - 1,             mac,
                      UT_TYPE_LONG8,  8, &groupid
                  );
    }
    pasDbCloseCursor(psCur);
    if(strlen(export2) > 0)
    {
        fclose(fp);
        utPltFileDownload(iFd, "application/text", caPath, caFilename, caFilename);
        remove(caFile);
        utPltFreeDb(psDbHead);
        return 0;
    }


    utPltOutToHtml(iFd, psMsgHead, psDbHead, "/normally/proauth_terminal.htm");
    return 0;
}


//查询分组信息
int ncsSys_terminal_group(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead)
{
    pasDbCursor *psCur;
    char sql[1024];
    char sql_lc[1024] = "";
    char caTemp[1024];
    char sqlbuf[1024];
    long lFlag, lKid;
    int  iNum;
    char *pHash;
    typedef struct ncsKeywordStat_s
    {
        long long lId;
        char caName[64];
        char cagroupdesc[64];
        long id;
        long sum;                //总记录数
        long csum;               //已处理记录数
    } ncsKeywordStat;
    ncsKeywordStat *psData;
    utPltDbHead *psDbHead;
    int iReturn, i;
    long lCount;
    char caName[64];
    char groupdesc[64];
    long long lId, lSid;
    char caPlate[128];
    char caPid[16];
    char caPid1[16];
    long slCount = 0;
    char caGroupdesc[64] = "";
    long lPid;
    int isid = 0;
    long lSumRec = 0;
    char caDel[16], caUpdate[16], caSelsid[1024], caKeyname[64], caSid[16], caTemp_d[64];
#ifdef LDEBUG1
    utMsgPrintMsg(psMsgHead);
#endif
    utMsgPrintMsg(psMsgHead);
    iReturn = utMsgGetSomeNVar(psMsgHead, 9,
                               "keyname",     UT_TYPE_STRING, 30, caKeyname,
                               "selsid",      UT_TYPE_STRING, 1000, caSelsid,
                               "update",      UT_TYPE_STRING, 10, caUpdate,
                               "sid",         UT_TYPE_STRING, 10, caSid,
                               "del",         UT_TYPE_STRING, 10, caDel,
                               "plate",       UT_TYPE_STRING, 100, caPlate,
                               "pid",         UT_TYPE_STRING, 10, caPid,
                               "groupdesc",   UT_TYPE_STRING, 63, caGroupdesc,
                               "node",        UT_TYPE_STRING, 10, caPid1
                              );

    pHash = (unsigned char *)pasLHashInit(1000, 1000, sizeof(struct ncsKeywordStat_s), 0, 4);
    if(pHash == NULL)
    {
        return -1;
    }
    if(strlen(caGroupdesc) > 0)
    {
        strcpy(caTemp_d, pasCvtGBK(2, caGroupdesc, caTemp, 63));
        strcpy(caGroupdesc, caTemp_d);
    }
    /*
      if(strlen(caPlate) > 0)
      {

          sprintf(sqlbuf, "select count(*),flag,kid from ncskeywordlog group by kid,flag ");

          psCur = pasDbOpenSql(sqlbuf, 0);
          if(psCur == NULL)
          {
              free(pHash);
              return 0;
          }
          lCount = 0;
          lFlag = 0;
          lKid = 0;
          iReturn = pasDbFetchInto(psCur, UT_TYPE_LONG, 4, &lCount,
                                   UT_TYPE_LONG, 4, &lFlag,
                                   UT_TYPE_LONG, 4, &lKid);

          while(iReturn == 0 || iReturn == 1405)
          {

              psData = (struct ncsKeywordStat_s *)pasLHashLookA(pHash, &lKid);
              if(psData)
              {
                  if(lFlag > 0)
                  {
                      psData->csum = psData->csum + lCount;
                  }
                  psData->sum = psData->sum + lCount;
              }


              lCount = 0;
              lFlag = 0;
              lKid = 0;
              iReturn = pasDbFetchInto(psCur, UT_TYPE_LONG, 4, &lCount,
                                       UT_TYPE_LONG, 4, &lFlag,
                                       UT_TYPE_LONG, 4, &lKid);
          }
          pasDbCloseCursor(psCur);


      }
      else
      {

          sprintf(sqlbuf, "select count(*),tid from ncskeycont,ncskeyindex where tid=id and pid=%d group by tid ", atol(caPid1));
          //         sprintf(sqlbuf,"select pid from ncskeyindex limit 0,999 ");

          psCur = pasDbOpenSql(sqlbuf, 0);
          if(psCur == NULL)
          {
              free(pHash);
              return 0;
          }

          lKid = 0;
          lCount = 0;
          iReturn = pasDbFetchInto(psCur,
                                   UT_TYPE_LONG, 4, &lCount,
                                   UT_TYPE_LONG, 4, &lKid);


          while(iReturn == 0 || iReturn == 1405)
          {

              psData = (struct ncsKeywordStat_s *)pasLHashLookA(pHash, &lKid);
              if(psData)
              {
                  psData->sum = psData->sum + lCount;
                  lSumRec = lSumRec + lCount;
              }
              lKid = 0;
              lCount = 0;
              iReturn = pasDbFetchInto(psCur,
                                       UT_TYPE_LONG, 4, &lCount,
                                       UT_TYPE_LONG, 4, &lKid);
          }
          pasDbCloseCursor(psCur);



      }


    */
    if(strlen(caKeyname) > 0)
    {
        strcpy(caTemp_d, pasCvtGBK(2, caKeyname, caTemp, 63));
        strcpy(caKeyname, caTemp_d);
    }


    if(strcmp(caUpdate, "update") == 0)
    {
        if(strlen(caSid) == 0)
        {
            /*sprintf(caTemp,"select count(*) from ncskeyindex where name='%s' ",caKeyname);
            lCount=0;
            pasDbOneRecord(caTemp,0,UT_TYPE_LONG,4,&lCount);
            if(lCount==0)
            {*/

            sprintf(sql, "insert into nctermmacgroup(groupname,groupdesc) values ('%s','%s')", caKeyname, caGroupdesc);
            pasDbExecSqlF(sql);
           // printf("1265sql==qin%s", sql);
        }
        else
        {
            sprintf(sql, "update nctermmacgroup set groupname=\'%s\',groupdesc=\'%s\' where groupid=%s", caKeyname, caGroupdesc, caSid);
            pasDbExecSqlF(sql);
          //  printf("1265sql==qin%s", sql);
        }

    }
    if(strcmp(caDel, "del") == 0 && strlen(caSelsid) > 0)
    {

        sprintf(sql, "delete from nctermmacgroup where groupid in (%s)", caSelsid);
        iReturn = pasDbExecSqlF(sql);
    //    printf("  delete sql=%s", sql);
        if(iReturn == 0)
        {
            sprintf(sql, "delete from nctermmacgroupinfo where groupid in (%s)", caSelsid);
            iReturn = pasDbExecSqlF(sql);
        }


    }

    iNum = 0;

    psDbHead = utPltInitDb();
    sprintf(sql_lc, "SELECT   count(*) FROM   nctermmacgroup  WHERE  1=1 ");
  //  printf("%dsql=sql=%s\n", __LINE__, sql_lc);
    pasDbOneRecord(sql_lc, 0, UT_TYPE_LONG, 4, &slCount);

    ncsKeywordStat* pGroup = (ncsKeywordStat*)malloc(slCount * sizeof(ncsKeywordStat));
    memset(pGroup, 0, slCount * sizeof(ncsKeywordStat));

    sprintf(sql, "SELECT   groupname,nctermmacgroup.groupid,groupdesc  FROM   nctermmacgroup  WHERE  1=1 ");
    psCur = pasDbOpenSqlF(sql);
 //   printf("%dsql=sql=%s\n", __LINE__, sql);
    //     psCur = pasDbOpenSqlF("select name,id,pid from ncskeyindex where pid=%d order by name limit 0,1000",atol(caPid1));

    //    psCur = pasDbOpenSqlF("select name,id,pid from ncskeyindex  order by name limit 0,1000");

    if(psCur == NULL)
    {
        utWebDispMsg(iFd, psMsgHead, "ncs/ncmsg_back.htm", "部门查询", "数据库出错");
        //            free(pHash);
        return 0;
    }

    memset(caName, 0, sizeof(caName));
    lId = 0;
    lPid = 0;
    iReturn = pasDbFetchInto(psCur, UT_TYPE_STRING, 31, caName,
                             UT_TYPE_LONG8, 8, &lId,
                             UT_TYPE_STRING, 63, groupdesc
                             // UT_TYPE_LONG,4,&lPid
                            );
    iNum = 0;
    while(iReturn == 0 || iReturn == 1405)
    {
        sprintf(pGroup[iNum].caName, "%s", caName);
      //  printf("pGroup[iNum].caName=[%s]\n", pGroup[iNum].caName);
        pGroup[iNum].lId = lId;
        sprintf(pGroup[iNum].cagroupdesc, "%s", groupdesc);

        memset(caName, 0, sizeof(caName));
        lId = 0;
        memset(groupdesc, 0, sizeof(groupdesc));
        iReturn = pasDbFetchInto(psCur, UT_TYPE_STRING, 31, caName,
                                 UT_TYPE_LONG8, 8, &lId,
                                 UT_TYPE_STRING, 63, groupdesc
                                 // UT_TYPE_LONG,4,&lPid
                                );
        iNum++;
    }

    iNum = 0;
    /*
         sprintf(sql_lc,"select   count(*)  from   nctermmacgroupinfo where groupid=%lu ",lId);
            printf("caTemp%s\n",sql_lc);

            int lireturn=1000;
            lireturn=pasDbOneRecord(sql_lc,0,UT_TYPE_LONG,4,&slCount);
             printf("slCount=[%lu]*ireturn=[%d]\n",slCount,lireturn);
        */
   // printf("%d, slCount=%lu\n", __LINE__, slCount);
    while(iNum < slCount)
    {   
        lId = pGroup[iNum].lId;
        memset(sql_lc, 0, sizeof(sql_lc));
        sprintf(sql_lc, "select   count(*)  from   nctermmacgroupinfo where groupid=%lu ", lId);
        pasDbOneRecord(sql_lc, 0, UT_TYPE_LONG, 4, &lCount);
      //  printf("count=%d\n", lCount);
        if(iNum > 0)
        {
            utPltPutLoopVar(psDbHead, "dh", iNum + 1, ",");
        }
        
       // printf("%s,%d\n", pGroup[iNum].caName, strlen(pGroup[iNum].caName));
		//printf("%d\n", pGroup[iNum].lId);

	    utPltSetCvtHtml(1);
		utPltPutLoopVarF(psDbHead, "groupname_t", iNum+1, "%s", pGroup[iNum].caName);
	    utPltPutLoopVarF(psDbHead, "groupname", iNum+1, "%s_(%lu)", pGroup[iNum].caName, lCount);
	    utPltSetCvtHtml(0);
	    //   utPltPutLoopVarF(psDbHead,"pid",iNum,"%ld",lPid);
	    utPltPutLoopVarF(psDbHead, "groupid",   iNum+1, "%llu", pGroup[iNum].lId);
	    utPltPutLoopVar(psDbHead, "leaf", iNum+1, "true");
	    utPltPutLoopVar(psDbHead, "cls", iNum+1, "file");
	    utPltPutLoopVarF(psDbHead, "groupdesc", iNum+1, "%s", pGroup[iNum].cagroupdesc);
        iNum++;
    }
    pasDbCloseCursor(psCur);
    free(pGroup);
    if(pHash)
        free(pHash);
    // utPltShowDb(psDbHead);
   // printf("caPlate=[%s]\n", caPlate);
    if(strlen(caPlate) > 0)
    {
   		utPltOutToHtml(iFd, psMsgHead, psDbHead, caPlate);
    }
    else
    {
        //printf("total iNum=%d\n", iNum);
        utPltPutVarF(psDbHead, "sum", "%d", iNum);
        utPltOutToHtml(iFd, psMsgHead, psDbHead, "normally/ncs_keyword_checktree.htm");
    }


    return 0;
}

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

char *getServiceNameByCode(utShmHead *psShmHead, char *pCode);


static iSort_corp;
static iSortby_corp;

static int ncsUtlSortOnCorp(char *p1, char *p2)
{
    int i;
    long long l;
    struct servicedata_s psC1, psC2;
    memcpy(&psC1, p1, sizeof(struct servicedata_s));
    memcpy(&psC2, p2, sizeof(struct servicedata_s));
    if(iSort_corp == 3)
    {
        if(iSortby_corp == 1)
            return psC2.onstate - psC1.onstate;
        else
            return psC1.onstate - psC2.onstate;
    }
    else if(iSort_corp == 1)
    {
        if(iSortby_corp == 1)
            return strcmp(psC2.caServicecode, psC1.caServicecode);
        else
            return strcmp(psC1.caServicecode, psC2.caServicecode);
    }
    else if(iSort_corp == 2)
    {
        if(iSortby_corp == 1)
            return strcmp(psC2.caCname, psC1.caCname);
        else
            return strcmp(psC1.caCname, psC2.caCname);
    }
    else if(iSort_corp == 4)
    {
        if(iSortby_corp == 1)
            return (psC2.onuid_count - psC1.onuid_count);
        else
            return (psC1.onuid_count - psC2.onuid_count);
    }
    else if(iSort_corp == 6)
    {
        if(iSortby_corp == 1)
            return strcmp(psC2.caGroupname, psC1.caGroupname);
        else
            return strcmp(psC1.caGroupname, psC2.caGroupname);
    }
    else if(iSort_corp == 7)
    {
        if(iSortby_corp == 1)
            return strcmp(psC2.caVersion, psC1.caVersion);
        else
            return strcmp(psC1.caVersion, psC2.caVersion);
    }
    else
    {

        return strcmp(psC2.caAddtime, psC1.caAddtime);

    }
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
           // printf("placename=%s, %lu == %lu\n", placename, psClient->lLastTime, lTime);
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

//显示单位信息
int ncsWebClientSearch_wif_v9(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead)
{
    char caWhere[512], caCond[64], caCond0[64];
    char caTotPg[16], caCurPg[16], caSort[16], caSortby[8];
    char caUsername[32], caGroupname[32], caMac[32], caIp[32], caDate[32];
    char caGroupid[16], caGroupid2[16];
    char caDispname[64], caAddress[128], caTel[64], caHandphone[32], caContact[32];
    char caTemp[2024], name[64];
    char caVar[16];
    long lSid;
    char *p;
    long lCount_t, js_flag;
    unsigned long lSumuser;
    unsigned long lCount_u;
    char caAll[1024];
    long index;
    ncsOnline  *psOnline;
    ncsClient * psClient;
    char caEmail[128];
    char *pSelect, *pBuf;
    unsigned long lUserid, lGroupid, lCurPg, lTotPg;
    int lCount;
    int i, iNum, lId;
    char *pSear;
    struct servicedata_s *pAreastatdata;
    pasLHashInfo sHashInfo;
    unsigned char *pHash;
    char caDir[20], caDid[16];

    unsigned char *pHash_fac;
    struct fac_s
    {
        char caCod[12];
        char caName[64];
    };
    struct fac_s *psFac;
    char caSmt[4048], caOrder[64];
    char caDel[16], caExport[16];
    utPltDbHead *psDbHead;
    int iReturn;
    long lGroupid0, lRowNum, lUseflags, lStartRec, lUseflags0, lTotRec;
    char servicecode1[24], dispname1[68], address1[130], ceoname1[32], handphone1[34], contact1[34], telphone1[68], email1[130];
    char ip1[20], servicestate1[4], jointype1[4], area1[72], areaman1[32], joincode1[8];
    pasDbCursor *psCur;
    char caUsername2[32];
    char caMsg[1024];
    char caOnlineflag[16];
    long lMonflag = 0;
    unsigned long lCurtime, lTime, lRuntime;
    unsigned long lCase;
    char caYear[30];
    char caPlate[128];
    char caOpt[256];
    char caVersion[20];
    char caPage[16], caLimit[16];
    char caDemo[256];
    char caDemo1[256];
    long lGtype = 0;
    char caCode[16], caName[64];
    char caFcod_c[16], caGtype[16], caTcode[16];
    long lSumcorp;
    char caKeyword[36];
    char caWireflag[16];
    char caUpdate[16], caUserid[16], caTelphone[24], caGroupid1[16];
    struct servicedata_s *pCorp;
    FILE *fp;
    char pFile[256], caBuf[128];
    char caLxstate[16];
    long lLxstate;
    char caTemp_d[256];
    char caSesid[1024];
    char caMove[16], caMid[16];
    char caOut[200][256];
    char caFcode[16], caFname[68];
    char caSdate[20], caAddtime[20];
    char caGroups[1024];
    char caFacs[1024];
    char caServicestate[16];
    char caLastln[20];
    char caClastime[24];
    char caRqlx[16];
    long lShflag, lWorkflag, lYyflag;
    char caJointype[20], caJointype_t[20];
    char caFlag[16], caSet[16], caSid[16];
    char caJrcs[68], caDwlb[68];
    char *pHash_lb;
    char caWorkflag[16], caShflag[16], caYyflag[16], caDataflag[16];
    //获取权限组ID和接入厂商

    //      unsigned char *psUserAtt;

    strcpy(caFacs, getDsFacGroupid());
    strcpy(caGroups, getDsGroupid());

    memset(caAll, 0, sizeof(caAll));




    //   printf("caAll=%s\n",caAll);

    pHash_fac = (unsigned char *)pasLHashInit(30, 30, sizeof(struct fac_s), 0, 12);
    if(pHash_fac == NULL)
    {
        utWebDispMsg(iFd, psMsgHead, "nc/ncmsg_back.htm", "分配内存出错", "分配内存出错");
        return 0;
    }
    //将接入厂家装入到内存
    sprintf(caTemp, "select code,name from ncsfactorycod limit 0,29 ");
    psCur = pasDbOpenSql(caTemp, 0);

    if(psCur)
    {
        memset(caCode, 0, sizeof(caCode));
        memset(caName, 0, sizeof(caName));
        iReturn = pasDbFetchInto(psCur,
                                 UT_TYPE_STRING, 10, caCode,
                                 UT_TYPE_STRING, 63, caName);



        while((iReturn == 0) || (iReturn == 1405))
        {
            psFac = (struct fac_s *)pasLHashLookA(pHash_fac, caCode);
            if(psFac)
            {
                strcpy(psFac->caName, caName);
            }
            memset(caCode, 0, sizeof(caCode));
            memset(caName, 0, sizeof(caName));
            iReturn = pasDbFetchInto(psCur,
                                     UT_TYPE_STRING, 10, caCode,
                                     UT_TYPE_STRING, 63, caName);
        }
        pasDbCloseCursor(psCur);

    }



    pHash_lb = (unsigned char *)pasLHashInit(30, 30, sizeof(struct fac_s), 0, 12);
    if(pHash_lb == NULL)
    {
        utWebDispMsg(iFd, psMsgHead, "nc/ncmsg_back.htm", "分配内存出错", "分配内存出错");
        return 0;
    }
    //将单位类别装入到内存
    sprintf(caTemp, "select code,name from ncsuser_lb limit 0,29 ");
    psCur = pasDbOpenSql(caTemp, 0);

    if(psCur)
    {
        memset(caCode, 0, sizeof(caCode));
        memset(caName, 0, sizeof(caName));
        iReturn = pasDbFetchInto(psCur,
                                 UT_TYPE_STRING, 10, caCode,
                                 UT_TYPE_STRING, 63, caName);



        while((iReturn == 0) || (iReturn == 1405))
        {
            psFac = (struct fac_s *)pasLHashLookA(pHash_lb, caCode);
            if(psFac)
            {
                strcpy(psFac->caName, caName);
            }
            memset(caCode, 0, sizeof(caCode));
            memset(caName, 0, sizeof(caName));
            iReturn = pasDbFetchInto(psCur,
                                     UT_TYPE_STRING, 10, caCode,
                                     UT_TYPE_STRING, 63, caName);
        }
        pasDbCloseCursor(psCur);

    }




    pHash = (unsigned char *)pasLHashInit(5000, 5000, sizeof(struct servicedata_s), 4, 16);
    if(pHash == NULL)
    {
        utWebDispMsg(iFd, psMsgHead, "nc/ncmsg_back.htm", "分配内存出错", "分配内存出错");
        return 0;
    }


    memset(caOpt, 0, sizeof(caOpt));
    iReturn = dsCltGetMyInfo(1, "Userid", &lId);


    lSumuser = 0;
    dsCltGetSessionValue(1, "dispname", UT_TYPE_STRING, 30, caUsername2);
   // printf("caUsername2=[%s]\n", caUsername2);

    iReturn = dsCltGetMyInfo(1, "Userid", &lId);
    js_flag = 0;



    char caSh[32];
    utMsgPrintMsg(psMsgHead);
    iReturn = utMsgGetSomeNVar(psMsgHead, 51,
                               "TotPg",            UT_TYPE_STRING, 12, caTotPg,
                               "CurPg",            UT_TYPE_STRING, 12, caCurPg,
                               "groupid",          UT_TYPE_STRING, 12, caGroupid,
                               "sort",            UT_TYPE_STRING, 15, caSort,
                               "dir",          UT_TYPE_STRING, 8,  caDir,
                               "del",             UT_TYPE_STRING, 10, caDel,
                               "export2",          UT_TYPE_STRING, 10, caExport,
                               "name",            UT_TYPE_STRING, 63, name,
                               "servicecode1",    UT_TYPE_STRING, 16, servicecode1,
                               "dispname1",       UT_TYPE_STRING, 64, dispname1,
                               "address1",        UT_TYPE_STRING, 128, address1,
                               "ceoname1",        UT_TYPE_STRING, 30, ceoname1,
                               "handphone1",      UT_TYPE_STRING, 32, handphone1,
                               "telphone1",       UT_TYPE_STRING, 64, telphone1,
                               "email1",          UT_TYPE_STRING, 128, email1,
                               "ip1",             UT_TYPE_STRING, 16, ip1,
                               "joincode1",       UT_TYPE_STRING, 7, joincode1,
                               "servicestate1",    UT_TYPE_STRING, 2, servicestate1,
                               "jointype1",       UT_TYPE_STRING, 3, jointype1,
                               "area1",           UT_TYPE_STRING, 60, area1,
                               "contact1",         UT_TYPE_STRING, 30, contact1,
                               "areaman1",        UT_TYPE_STRING, 29, areaman1,
                               "onlineflag",      UT_TYPE_STRING, 2, caOnlineflag,
                               "plate",           UT_TYPE_STRING, 120, caPlate,
                               "page",            UT_TYPE_STRING, 10, caPage,
                               "limit",           UT_TYPE_STRING, 10, caLimit,
                               "fcode",           UT_TYPE_STRING, 10, caFcod_c,
                               "lb",           UT_TYPE_STRING, 10, caGtype,
                               "lxstate",           UT_TYPE_STRING, 10, caLxstate,
                               "fcode",           UT_TYPE_STRING, 10, caFcode,
                               "selsid", UT_TYPE_STRING, 1000, caSesid,
                               "did",   UT_TYPE_STRING, 10, caDid,
                               "keyword", UT_TYPE_STRING, 30, caKeyword,
                               "move", UT_TYPE_STRING, 10, caMove,
                               "mid", UT_TYPE_STRING, 10, caMid,
                               "update", UT_TYPE_STRING, 10, caUpdate,
                               "sid", UT_TYPE_STRING, 10, caSid,
                               "set", UT_TYPE_STRING, 10, caSet,
                               "flag", UT_TYPE_STRING, 10, caFlag,
                               "lastln", UT_TYPE_STRING, 10, caLastln,
                               "jrcs",  UT_TYPE_STRING, 60, caJrcs,
                               "dwlb",  UT_TYPE_STRING, 60, caDwlb,
                               "rqlx",  UT_TYPE_STRING, 3, caRqlx,
                               "demo1", UT_TYPE_STRING, 100, caDemo1,
                               "wireflag", UT_TYPE_STRING, 8, caWireflag,
                               "jointype", UT_TYPE_STRING, 19, caJointype_t,
                               "sh",   UT_TYPE_STRING, 8, caSh,
                               "workflag", UT_TYPE_STRING, 6, caWorkflag,
                               "shflag",  UT_TYPE_STRING, 6, caShflag,
                               "yyflag",  UT_TYPE_STRING, 6, caYyflag,
                               "dataflag", UT_TYPE_STRING, 6, caDataflag);
    char caJointype_c[20];
    memset(caJointype_c, 0, sizeof(caJointype));
    if(strlen(caJointype_t) > 0)
    {
        strcpy(caTemp_d, pasCvtGBK(2, caJointype_t, caTemp, 64));
        strcpy(caJointype_t, caTemp_d);
        strcpy(caJointype_c, ncsGetJoinCodeByName(caJointype_t));
       // printf("caJointype_c=%s\n", caJointype_c);
    }


    if(strcmp(caGtype, "undefined") == 0) strcpy(caGtype, "");
    if(strcmp(caGroupid, "undefined") == 0) strcpy(caGroupid, "");
    if(strcmp(caFcode, "undefined") == 0) strcpy(caFcode, "");

    if(strcmp(caSet, "set") == 0 && strlen(caSid) > 0)
    {
        sprintf(caTemp, "update ncsuser set servicestate='%s' where userid=%s", caFlag, caSid);
     //   printf("caTemp=%s\n", caTemp);
        pasDbExecSql(caTemp, 0);
    }

    if(strlen(caJrcs) > 0)
    {
        strcpy(caTemp_d, pasCvtGBK(2, caJrcs, caTemp, 64));
        strcpy(caJrcs, caTemp_d);

        sprintf(caTemp, "select code from ncsfactorycod where name='%s' ", caJrcs);
        memset(caCode, 0, sizeof(caCode));
        pasDbOneRecord(caTemp, 0, UT_TYPE_STRING, 10, caCode);
        if(strlen(caCode) > 0)
        {
            strcpy(caJrcs, caCode);
        }
    }
    if(strlen(caDwlb) > 0)
    {
        strcpy(caTemp_d, pasCvtGBK(2, caDwlb, caTemp, 64));
        strcpy(caDwlb, caTemp_d);
        sprintf(caTemp, "select code from ncsuser_lb where name='%s' ", caDwlb);

        memset(caCode, 0, sizeof(caCode));
        pasDbOneRecord(caTemp, 0, UT_TYPE_STRING, 10, caCode);

        if(strlen(caCode) > 0)
        {
            strcpy(caDwlb, caCode);
        }


    }


    if(strlen(dispname1) > 0)
    {
        strcpy(caTemp_d, pasCvtGBK(2, dispname1, caTemp, 64));
        strcpy(dispname1, caTemp_d);
    }
    if(strlen(area1) > 0)
    {
        strcpy(caTemp_d, pasCvtGBK(2, area1, caTemp, 64));
        strcpy(area1, caTemp_d);
    }
    if(strlen(address1) > 0)
    {
        strcpy(caTemp_d, pasCvtGBK(2, address1, caTemp, 127));
        strcpy(address1, caTemp_d);
    }
    if(strlen(caKeyword) > 0)
    {
        iReturn = pasCharIsUtf8(caKeyword, strlen(caKeyword));
        if(iReturn)
        {
            strcpy(caTemp_d, pasCvtGBK(2, caKeyword, caTemp, 30));
            strcpy(caKeyword, caTemp_d);
        }

    }

    if(strcmp(caDir, "ASC") == 0)
    {
        iSortby_corp = 1;
    }
    else
    {
        iSortby_corp = 0;
    }
    if(strcmp(caSort, "dispname") == 0)
    {
        iSort_corp = 2;
    }
    else if(strcmp(caSort, "username") == 0)
    {
        iSort_corp = 1;
    }
    else if(strcmp(caSort, "status") == 0)
    {
        iSort_corp = 3;
    }
    else if(strcmp(caSort, "onuser") == 0)
    {
        iSort_corp = 4;
    }
    else if(strcmp(caSort, "groupname") == 0)
    {
        iSort_corp = 6;
    }
    else if(strcmp(caSort, "version") == 0)
    {
        iSort_corp = 7;
    }
    else
    {
        iSort_corp = 5;
    }

    //修改或增加
    if(strcmp(caUpdate, "update") == 0)
    {
        iReturn = utMsgGetSomeNVar(psMsgHead, 7,
                                   "userid",      UT_TYPE_STRING, 10, caUserid,
                                   "username",    UT_TYPE_STRING, 30, caUsername,
                                   "groupid1",     UT_TYPE_STRING, 10, caGroupid1,
                                   "dispname",    UT_TYPE_STRING, 63, caDispname,
                                   "address",     UT_TYPE_STRING, 127, caAddress,
                                   "telphone",    UT_TYPE_STRING, 15, caTelphone,
                                   "contact",     UT_TYPE_STRING, 15, caContact);
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
        if(atol(caUserid) > 0)
        {
            lUserid = atol(caUserid);
            sprintf(caTemp, "update ncsuser set username='%s',groupid=%s,dispname='%s',address='%s',contact='%s',telphone='%s',moditime=%lu,fcode='%s',gtype=%lu,jointype='%s' where userid=%s ",
                    caUsername, caGroupid1, caDispname, caAddress, caContact, caTelphone, time(0), caJrcs, atol(caDwlb), caJointype_c, caUserid);
           // printf("caTemp=%s\n", caTemp);
            iReturn = pasDbExecSqlF(caTemp);


            sprintf(caTemp, "修改上网场所信息，代码：%s,显示名：%s", caUsername, caDispname);
            ncsWriteSysLog(psMsgHead, caUsername2, "05", "成功", caTemp);
        }
        else
        {
            lUserid = utMd5Code(caUsername, strlen(caUsername), "ffff");
            if(lUserid > 2000000000) lUserid = lUserid - 2000000000;
            if(lUserid > 2000000000) lUserid = lUserid - 2000000000;
            sprintf(caTemp, "select count(*) from ncsuser where userid=%lu ", lUserid);
            lCount = 0;
            pasDbOneRecord(caTemp, 0, UT_TYPE_LONG, 4, &lCount);
            if(lCount > 0)
            {
                lUserid = lUserid + 10;
            }

            strcpy(caSdate, utTimFormat("%Y-%m-%d %H:%M", time(0)));

            sprintf(caTemp, "insert into ncsuser (userid,username,groupid,dispname,address,telphone,contact,handphone,email,ip,addtime,lasttime,\
		               servicestate,joincode,endnum,servernum,empnum,area,areaman,areatel,demo,postcode,ceoname,moditime,monflag,password,fcode,gtype,jointype) values \
                                                 (%lu,'%s', %lu,'%s','%s','%s','%s','%s','%s','%s','%s','%s',\
                                                 '%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s',%lu,%lu,'','%s',%lu,'%s')", \
                    lUserid, caUsername, strtoul(caGroupid1, NULL, 10), caDispname, caAddress, caTelphone, caContact, "", "", "", caSdate, caSdate,
                    "", "", "", "", "",
                    "", "", "", "", "", "", time(0), 0, caJrcs, atol(caDwlb), caJointype_c);
           // printf("sqlbuf=%s\n", caTemp);

            iReturn = pasDbExecSqlF(caTemp);
            if(iReturn == 0)
            {
                ncsWebReqSyscorp_v4(psShmHead, caTemp);
            }


            sprintf(caTemp, "添加上网场所 场所编码%s，名称：%s", caUsername, caDispname);
            ncsWriteSysLog(psMsgHead, caUsername2, "05", "成功", caTemp);

        }
        if(iReturn == 0)
        {
            psClient = (ncsClient *)ncsUtlGetClientById(psShmHead, lUserid);
            if(psClient)
            {
                strcpy(psClient->username, caUsername);
                strcpy(psClient->dispname, caDispname);
                psClient->groupid = strtoul(caGroupid1, NULL, 10);
            }
        }


    }




    if(!utStrIsSpaces(caDel))    /* 删除选中的项目*/
    {
        if(strlen(caSesid) > 0)
        {
            iReturn = pasDbExecSqlF("delete from ncsuser where userid in(%s)", caSesid);
            sprintf(caMsg, "delete from ncsuser where userid in(%s) ", caSesid);
            if(iReturn == 0)
            {

                ncsWriteSysLog(psMsgHead, caUsername2, "05", "成功", caMsg);


            }
            else
            {
                ncsWriteSysLog(psMsgHead, caUsername2, "05", "失败", caMsg);
            }
        }
    }

    if(!utStrIsSpaces(caSh))    /* 审核*/
    {
        if(strlen(caSesid) > 0)
        {
            iReturn = pasDbExecSqlF("update ncsuser set shflag=1 where userid in(%s)", caSesid);
            //        printf("update ncsuser set shflag=1 where userid in(%s)\n",caSesid);
            //   printf("pid=%d\n",getpid());
            //   sleep(15);
            lCount = sepcharbydh(caSesid, caOut);
            for(i = 0; i < lCount; i++)
            {

                sprintf(caMsg, "审核单位%s ", ncsUtlGetClientNameById(psShmHead, atol(caOut[i]), "未知"));
                if(iReturn == 0)
                {

                    ncsWriteSysLog(psMsgHead, caUsername2, "05", "成功", caMsg);


                }
                else
                {
                    ncsWriteSysLog(psMsgHead, caUsername2, "05", "失败", caMsg);
                }
            }
        }
    }



    if(strcmp(caMove, "move") == 0)
    {
        if(strlen(caSesid) > 0 && strlen(caMid) > 0)
        {
            iReturn = pasDbExecSqlF("update ncsuser set groupid=%s where userid in(%s)", caMid, caSesid);
            lCount = sepcharbydh(caSesid, caOut);
            for(i = 0; i < lCount; i++)
            {
                //      printf("caOut=%s\n",caOut[i]);
                psClient = (ncsClient *)ncsUtlGetClientById(psShmHead, atol(caOut[i]));
                if(psClient)
                {
                    psClient->groupid = atol(caMid);

                }
            }
        }
    }

    if(strlen(caPage) > 0)
    {
        strcpy(caCurPg, caPage);
    }
    lCurPg = atol(caCurPg);//当前页面
    if(lCurPg <= 0) lCurPg = 1 ;//如若当前页面小于0，置1

    if(strlen(caLimit) > 0)
    {
        lRowNum = atol(caLimit);
    }
    else
    {
        lRowNum = 40; //每一页面行数
    }

    lStartRec = (lCurPg - 1) * lRowNum;   //当前页面的开始第一行的行数


    sprintf(caSmt, "select useflags,userid,dispname,username,ip,lasttime,email,address,telphone,handphone,contact,monflag,version,demo,b.groupname,gtype,a.groupid,a.addtime,a.fcode,a.servicestate,a.jointype,a.shflag,a.workflag,a.yyflag from ncsuser a left join ncsgroup b on ( a.groupid=b.groupid) where 1=1 ");

    if(strlen(servicecode1) != 0)
    {
        sprintf(caSmt + strlen(caSmt), "  and username like '%c%s%c' ", '%', servicecode1, '%');
    }
    if(strlen(dispname1) != 0)
    {
        sprintf(caSmt + strlen(caSmt), "  and dispname like '%c%s%c' ", '%', dispname1, '%');
    }
    if(strlen(address1) != 0)
    {
        sprintf(caSmt + strlen(caSmt), "  and address like '%c%s%c' ", '%', address1, '%');
    }
    if(strlen(ceoname1) != 0)
    {
        sprintf(caSmt + strlen(caSmt), "  and ceoname like '%c%s%c' ", '%', ceoname1, '%');
    }
    if(strlen(handphone1) != 0)
    {
        sprintf(caSmt + strlen(caSmt), "  and handphone like '%c%s%c' ", '%', handphone1, '%');
    }
    if(strlen(contact1) != 0)
    {
        sprintf(caSmt + strlen(caSmt), "  and contact like '%c%s%c' ", '%', contact1, '%');
    }
    if(strlen(telphone1) != 0)
    {
        sprintf(caSmt + strlen(caSmt), "  and telphone like '%c%s%c' ", '%', telphone1, '%');
    }
    if(strlen(email1) != 0)
    {
        sprintf(caSmt + strlen(caSmt), "  and email like '%c%s%c' ", '%', email1, '%');
    }
    if(strlen(ip1) != 0)
    {
        sprintf(caSmt + strlen(caSmt), "  and ip like '%c%s%c' ", '%', ip1, '%');
    }

    if(strlen(joincode1) != 0)
    {
        sprintf(caSmt + strlen(caSmt), "  and joincode like '%c%s%c' ", '%', joincode1, '%');
    }
    if(strlen(servicestate1) != 0)
    {
        sprintf(caSmt + strlen(caSmt), "  and servicestate= '%s' ", servicestate1);
    }
    if(strlen(jointype1) != 0)
    {
        sprintf(caSmt + strlen(caSmt), "  and jointype= '%s' ", jointype1);
    }
    if(strlen(area1) != 0)
    {
        sprintf(caSmt + strlen(caSmt), "  and area like '%c%s%c' ", '%', area1, '%');
    }
    if(strlen(caWorkflag) > 0)
    {
        sprintf(caSmt + strlen(caSmt), " and workflag=%s ", caWorkflag);
    }
    if(strlen(caShflag) > 0)
    {
        sprintf(caSmt + strlen(caSmt), " and shflag=%s ", caShflag);
    }
    if(strlen(caYyflag) > 0)
    {
        sprintf(caSmt + strlen(caSmt), " and yyflag=%s ", caYyflag);
    }


    if(strlen(areaman1) != 0)
    {
        sprintf(caSmt + strlen(caSmt), "  and areaman like '%c%s%c' ", '%', areaman1, '%');
    }
    if(atol(caGroupid) == 99999999)
    {
        sprintf(caSmt + strlen(caSmt), " and a.groupid not in (select groupid from ncsgroup ) ");
    }
    else if(strlen(caGroupid) > 0)
    {
        sprintf(caSmt + strlen(caSmt), " and a.groupid in (%s) ", getGroupid(atol(caGroupid)));
        //      sprintf(caSmt+strlen(caSmt)," and (a.groupid=%s or a.groupid in (select groupid from ncsgroup where pid=%s)) ",caGroupid,caGroupid);
    }
    //权限
    if(strlen(caGroups) > 0)
    {
        sprintf(caSmt + strlen(caSmt), " and a.groupid in (%s) ", caGroups);
    }
    if(strlen(caFacs) > 0)
    {
        sprintf(caSmt + strlen(caSmt), " and a.fcode in (%s) ", caFacs);
    }
    if(strlen(caWireflag) > 0)
    {
        if(strcmp(caWireflag, "2") == 0)
        {
            sprintf(caSmt + strlen(caSmt), " and (a.jointype='08' or a.jointype='09') ");
        }
        else if(strcmp(caWireflag, "1") == 0)
        {
            sprintf(caSmt + strlen(caSmt), " and a.jointype!='08' ");
        }

    }
    if(strlen(caRqlx) > 0)
    {
        sprintf(caSmt + strlen(caSmt), " and gtype in (%s) ", getRqlxId(caRqlx));
    }

    if(strlen(caDemo1) > 0)
    {
        sprintf(caSmt + strlen(caSmt), " and demo like '%c%s%c' ", '%', caDemo1, '%');
    }


    if(strlen(caGtype) > 0)
    {
        if(atol(caGtype) != 9)
        {
            if(atol(caGtype) == 200)
            {
                sprintf(caSmt + strlen(caSmt), " and gtype in ('1','2','8','11','7','4')");
            }
            else if(atol(caGtype) == 300)
            {
                sprintf(caSmt + strlen(caSmt), " and gtype in ('5','6','10','3')");
            }
            else
            {
                sprintf(caSmt + strlen(caSmt), " and gtype=%s ", caGtype);
            }
        }
        else
        {
            sprintf(caSmt + strlen(caSmt), " and gtype not in ('1','2','3','4','5','6','7','8','10','11') ");
        }
        //          sprintf(caSmt+strlen(caSmt)," and gtype=%s ",caGtype);
    }
    //     if(strlen(caFcod_c)>0){
    //         sprintf(caSmt+strlen(caSmt)," and substring(username,9,2)='%s' ",caFcod_c);
    //    }
    if(strlen(caDid) > 0)
    {
        sprintf(caSmt + strlen(caSmt), " and userid=%s ", caDid);
    }

 //   printf("caKeyword=%s\n", caKeyword);
    if(strlen(caKeyword) > 0)
    {
        sprintf(caSmt + strlen(caSmt), " and (dispname like '%c%s%c' or username like '%c%s%c' or demo like '%c%s%c')", '%', caKeyword, '%', '%', caKeyword, '%', '%', caKeyword, '%');
    }
    if(strlen(caFcode) > 0)
    {
        sprintf(caSmt + strlen(caSmt), " and fcode='%s' ", caFcode);
    }
    /*
       if(strlen(caSort)>0&&strcmp(caSort,"state")!=0){
           sprintf(caSmt+strlen(caSmt),"  order by %s ",caSort);
       }
       else{
       sprintf(caSmt+strlen(caSmt),"  order by dispname ");
       }
       if(strcmp(caSortby,"desc")==0){
           sprintf(caSmt+strlen(caSmt),"  desc ");
       }
    */
  //  printf("caSmt=%s\n", caSmt);
    lLxstate = atol(caLxstate);
    psCur = pasDbOpenSql(caSmt, 0);
    if(psCur == NULL)
    {

        utWebDispMsg(iFd, psMsgHead, "nc/ncmsg_back.htm", ncsLang("0613网站访问查询"), ncsLang("0549数据库出错"));
        return 0;
    }
    memset(caDispname, 0, sizeof(caDispname));
    memset(caUsername, 0, sizeof(caUsername));
    memset(caIp, 0, sizeof(caIp));
    memset(caDate, 0, sizeof(caDate));
    memset(caEmail, 0, sizeof(caEmail));
    memset(caAddress, 0, sizeof(caAddress));
    memset(caTel, 0, sizeof(caTel));
    memset(caHandphone, 0, sizeof(caHandphone));
    memset(caContact, 0, sizeof(caContact));
    lUseflags = 0;
    lMonflag = 0;
    memset(caVersion, 0, sizeof(caVersion));
    memset(caDemo, 0, sizeof(caDemo));
    memset(caGroupname, 0, sizeof(caGroupname));
    lGtype = 0;
    lGroupid = 0;
    memset(caAddtime, 0, sizeof(caAddtime));
    memset(caFcode, 0, sizeof(caFcode));
    memset(caServicestate, 0, sizeof(caServicestate));
    memset(caJointype, 0, sizeof(caJointype));
    lShflag = 0;
    lWorkflag = 0;
    lYyflag = 0;
    iReturn = pasDbFetchInto(psCur, UT_TYPE_LONG, 4, &lUseflags,
                             UT_TYPE_ULONG, 4, &lUserid,
                             UT_TYPE_STRING, 63, caDispname,
                             UT_TYPE_STRING, 15, caUsername,
                             UT_TYPE_STRING, 15, caIp,
                             UT_TYPE_STRING, 15, caDate,
                             UT_TYPE_STRING, 63, caEmail,
                             UT_TYPE_STRING, 127, caAddress,
                             UT_TYPE_STRING, 63, caTel,
                             UT_TYPE_STRING, 31, caHandphone,
                             UT_TYPE_STRING, 31, caContact,
                             UT_TYPE_LONG, 4, &lMonflag,
                             UT_TYPE_STRING, 15, caVersion,
                             UT_TYPE_STRING, 250, caDemo,
                             UT_TYPE_STRING, 31, caGroupname,
                             UT_TYPE_STRING, 4, caGtype,
                             UT_TYPE_ULONG, 4, &lGroupid,
                             UT_TYPE_STRING, 19, caAddtime,
                             UT_TYPE_STRING, 10, caFcode,
                             UT_TYPE_STRING, 2, caServicestate,
                             UT_TYPE_STRING, 4, caJointype,
                             UT_TYPE_LONG, 4, &lShflag,
                             UT_TYPE_LONG, 4, &lWorkflag,
                             UT_TYPE_LONG, 4, &lYyflag);
    lSumcorp = 0;
    while((iReturn == 0) || (iReturn == 1405))
    {
        pAreastatdata = (struct servicedata_s *)pasLHashLookA(pHash, caUsername);
        //       pAreastatdata = (struct servicedata_s *)pasLHashLookA(pHash,&lUserid);
        if(pAreastatdata)
        {
            lSumcorp++;
            pAreastatdata->userid = lUserid;
            //       printf("caVersion=%s,caGtype=%s\n",caVersion,caGtype);
            //           strcpy(pAreastatdata->caServicecode,caUsername);
            strcpy(pAreastatdata->caCname, caDispname);
            strcpy(pAreastatdata->caIp, caIp);
            strcpy(pAreastatdata->caDate, caDate);
            strcpy(pAreastatdata->caEmail, caEmail);
            strcpy(pAreastatdata->caAddress, caAddress);
            strcpy(pAreastatdata->caHandphone, caHandphone);
            strcpy(pAreastatdata->caTel, caTel);
            strcpy(pAreastatdata->caContact, caContact);
            strcpy(pAreastatdata->caVersion, caVersion);
            strcpy(pAreastatdata->caDemo, caDemo);
            strcpy(pAreastatdata->caGroupname, caGroupname);
            strcpy(pAreastatdata->caJointype, caJointype);
            pAreastatdata->lMonflag = lMonflag;
            pAreastatdata->lUseflags = lUseflags;
            strcpy(pAreastatdata->caGtype, caGtype);
            pAreastatdata->groupid = lGroupid;
            strcpy(pAreastatdata->caAddtime, caAddtime);
            strcpy(pAreastatdata->caFcode, caFcode);
            strcpy(pAreastatdata->servicestate, caServicestate);
            pAreastatdata->lShflag = lShflag;
            pAreastatdata->lWorkflag = lWorkflag;
            pAreastatdata->lYyflag = lYyflag;
            psClient = (ncsClient *)ncsUtlGetClientById(psShmHead, lUserid);
            psOnline = (ncsOnline *)ncsUtlGetOnlineById(psShmHead, lUserid);
            if(psOnline)
            {
                strcpy(pAreastatdata->caIp, utComHostIp(htonl(psOnline->lSip)));
                pAreastatdata->onuid_count = psOnline->lUser;
                pAreastatdata->lLasttime = psOnline->lLastTime;
                lSumuser = lSumuser + psOnline->lUser;
            }
            else
            {
                pAreastatdata->lLasttime = 0;
            }


            //printf("onstate=%d\n",pAreastatdata->onstate);
            if(pAreastatdata->onstate != 9)
            {
                if(psClient)
                {
                    if(psClient->status == 1 || psClient->status == 3)
                    {
                        if(psOnline)
                        {
                            pAreastatdata->onstate = 1;
                        }
                        else
                        {

                            pAreastatdata->onstate = 2;

                        }
                    }
                    //          else if(psClient->status==3){
                    //              pAreastatdata->onstate=1;
                    //          }
                    else if(psClient->status == 2)
                    {
                        pAreastatdata->onstate = 2;

                    }

                }
            }


        }


        memset(caDispname, 0, sizeof(caDispname));
        memset(caUsername, 0, sizeof(caUsername));
        memset(caIp, 0, sizeof(caIp));
        memset(caDate, 0, sizeof(caDate));
        memset(caEmail, 0, sizeof(caEmail));
        memset(caAddress, 0, sizeof(caAddress));
        memset(caTel, 0, sizeof(caTel));
        memset(caHandphone, 0, sizeof(caHandphone));
        memset(caContact, 0, sizeof(caContact));
        lUseflags = 0;
        lMonflag = 0;
        memset(caVersion, 0, sizeof(caVersion));
        memset(caDemo, 0, sizeof(caDemo));
        memset(caGroupname, 0, sizeof(caGroupname));
        lGtype = 0;
        lGroupid = 0;
        memset(caAddtime, 0, sizeof(caAddtime));
        memset(caServicestate, 0, sizeof(caServicestate));
        lShflag = 0;
        lWorkflag = 0;
        lYyflag = 0;
        iReturn = pasDbFetchInto(psCur, UT_TYPE_LONG, 4, &lUseflags,
                                 UT_TYPE_ULONG, 4, &lUserid,
                                 UT_TYPE_STRING, 63, caDispname,
                                 UT_TYPE_STRING, 15, caUsername,
                                 UT_TYPE_STRING, 15, caIp,
                                 UT_TYPE_STRING, 15, caDate,
                                 UT_TYPE_STRING, 63, caEmail,
                                 UT_TYPE_STRING, 127, caAddress,
                                 UT_TYPE_STRING, 63, caTel,
                                 UT_TYPE_STRING, 31, caHandphone,
                                 UT_TYPE_STRING, 31, caContact,
                                 UT_TYPE_LONG, 4, &lMonflag,
                                 UT_TYPE_STRING, 15, caVersion,
                                 UT_TYPE_STRING, 250, caDemo,
                                 UT_TYPE_STRING, 31, caGroupname,
                                 UT_TYPE_STRING, 4, caGtype,
                                 UT_TYPE_ULONG, 4, &lGroupid,
                                 UT_TYPE_STRING, 19, caAddtime,
                                 UT_TYPE_STRING, 10, caFcode,
                                 UT_TYPE_STRING, 2, caServicestate,
                                 UT_TYPE_STRING, 4, caJointype,
                                 UT_TYPE_LONG, 4, &lShflag,
                                 UT_TYPE_LONG, 4, &lWorkflag,
                                 UT_TYPE_LONG, 4, &lYyflag);
    }
    pasDbCloseCursor(psCur);

    //各单位用户数
    sprintf(caTemp, "select count(b.did ),a.username from ncsuser a left join ncsenduser b on ( a.userid=b.did) where 1=1 ");
    if(!strstr(caOpt, "all") && (strlen(caOpt) > 0))
    {
        sprintf(caTemp + strlen(caTemp), " and a.groupid in (%s) ", caOpt);
    }
    sprintf(caTemp + strlen(caTemp), " group by a.username order by a.username ");
    psCur = pasDbOpenSql(caTemp, 0);
    if(psCur == NULL)
    {
        if(pHash) free(pHash);
        utWebDispMsg(iFd, psMsgHead, "nc/ncmsg_back.htm", ncsLang("0613网站访问查询"), ncsLang("0549数据库出错"));
        return 0;
    }

    lCount_u = 0;
    lUserid = 0;
    i = 0;
    memset(caUsername, 0, sizeof(caUsername));
    iReturn = pasDbFetchInto(psCur,
                             UT_TYPE_LONG, 4, &lCount_u,
                             UT_TYPE_STRING, 15, caUsername);

    while((iReturn == 0) || (iReturn == 1405))
    {
        pAreastatdata = (struct servicedata_s *)pasLHashLook(pHash, caUsername);
        if(pAreastatdata)
        {
            pAreastatdata->endnum = lCount_u;
            //              printf("endnu=%d,lUserid=%d\n",lCount_u,lUserid);
        }
        lCount_u = 0;
        lUserid = 0;
        i++;
        memset(caUsername, 0, sizeof(caUsername));
        iReturn = pasDbFetchInto(psCur,
                                 UT_TYPE_LONG, 4, &lCount_u,
                                 UT_TYPE_STRING, 15, caUsername);
    }
    pasDbCloseCursor(psCur);

    //统计是否有数据
    char caDataOffTime[12];
    long lDataTime;
    char caToday[32], caTable[32];
    strcpy(caDataOffTime, utComGetVar_sd(psShmHead, "DataTimeOut", "86400"));
    lDataTime = atol(caDataOffTime);
    lTime = time(0);
    strcpy(caToday, utTimFormat("%Y%m", lTime));
    sprintf(caTable, "nctermlog_if_%s", caToday);
    sprintf(caTemp, "select distinct servicecode from %s where stime>%d ", caTable, lTime - lDataTime);
   // printf("caTemp=%s\n", caTemp);
    psCur = pasDbOpenSql(caTemp, 0);
    if(psCur)
    {
        memset(caUsername, 0, sizeof(caUsername));
        iReturn = pasDbFetchInto(psCur, UT_TYPE_STRING, 15, caUsername);
        while((iReturn == 0) || (iReturn == 1405))
        {

            pAreastatdata = (struct servicedata_s *)pasLHashLook(pHash, caUsername);
            if(pAreastatdata)
            {
                pAreastatdata->lDataflag = 1;
            }

            memset(caUsername, 0, sizeof(caUsername));
            iReturn = pasDbFetchInto(psCur, UT_TYPE_STRING, 15, caUsername);

        }
        pasDbCloseCursor(psCur);
    }


    //判断MAC日志是否有
    sprintf(caTable, "ncmactermatt_if_%s", caToday);
    sprintf(caTemp, "select distinct servicecode from %s where stime>%d ", caTable, lTime - lDataTime);
    printf("caTemp=%s\n", caTemp);
    psCur = pasDbOpenSql(caTemp, 0);
    if(psCur)
    {
        memset(caUsername, 0, sizeof(caUsername));
        iReturn = pasDbFetchInto(psCur, UT_TYPE_STRING, 15, caUsername);
        while((iReturn == 0) || (iReturn == 1405))
        {

            pAreastatdata = (struct servicedata_s *)pasLHashLook(pHash, caUsername);
            if(pAreastatdata)
            {

                pAreastatdata->lDataflag = 1;
            }

            memset(caUsername, 0, sizeof(caUsername));
            iReturn = pasDbFetchInto(psCur, UT_TYPE_STRING, 15, caUsername);

        }
        pasDbCloseCursor(psCur);
    }


    //  printf("lSumcorp=%d\n",lSumcorp);
    pCorp = (struct servicedata_s*)malloc((lSumcorp + 1) * sizeof(struct servicedata_s));
    if(pCorp == NULL)
    {
        utWebDispMsg(iFd, psMsgHead, "nc/ncmsg_back.htm", "分配内存出错", "分配内存出错");
        return 0;
    }
    memset(pCorp, 0, (lSumcorp + 1)*sizeof(struct servicedata_s));


    pAreastatdata  = (struct servicedata_s *)pasLHashFirst(pHash, &sHashInfo);
    iNum = 0;
    lLxstate = atol(caLxstate);
    long lDataflag = atol(caDataflag);
    sprintf(caTable, "ncmactermatt_if_%s", caToday);
    int isInFlag = 0;
    while(pAreastatdata)
    {
        isInFlag = 0;
        if(lLxstate == 0)
        {
            isInFlag = 1;
        }
        else if(lLxstate == 1)
        {
            if(isInOnlinePlaceMemory(psShmHead, pAreastatdata->caServicecode))
                isInFlag = 1;
        }
        else if(lLxstate == 2)
        {
            if(!isInOnlinePlaceMemory(psShmHead, pAreastatdata->caServicecode))
                isInFlag = 1;
        }
        //if(strlen(caLxstate) == 0 )|| pAreastatdata->onstate == lLxstate)
        if(isInFlag)
        {
            if(strlen(caDataflag) == 0 || pAreastatdata->lDataflag == lDataflag)
            {
                pCorp[iNum].userid = pAreastatdata->userid;
                strcpy(pCorp[iNum].caServicecode, pAreastatdata->caServicecode);
                strcpy(pCorp[iNum].caCname, pAreastatdata->caCname);
                strcpy(pCorp[iNum].caIp, pAreastatdata->caIp);
                strcpy(pCorp[iNum].caDate, pAreastatdata->caDate);
                strcpy(pCorp[iNum].caEmail, pAreastatdata->caEmail);
                strcpy(pCorp[iNum].caAddress, pAreastatdata->caAddress);
                strcpy(pCorp[iNum].caHandphone, pAreastatdata->caHandphone);
                strcpy(pCorp[iNum].caTel, pAreastatdata->caTel);
                strcpy(pCorp[iNum].caContact, pAreastatdata->caContact);
                strcpy(pCorp[iNum].caVersion, pAreastatdata->caVersion);
                strcpy(pCorp[iNum].caDemo, pAreastatdata->caDemo);
                strcpy(pCorp[iNum].caGroupname, pAreastatdata->caGroupname);
                strcpy(pCorp[iNum].caJointype, pAreastatdata->caJointype);
                pCorp[iNum].lMonflag = pAreastatdata->lMonflag;
                pCorp[iNum].lUseflags = pAreastatdata->lUseflags;
                pCorp[iNum].onuid_count = pAreastatdata->onuid_count;
                pCorp[iNum].onstate = pAreastatdata->onstate;
                //      printf("state=%d\n",pAreastatdata->onstate);
                strcpy(pCorp[iNum].caGtype, pAreastatdata->caGtype);
                pCorp[iNum].endnum = pAreastatdata->endnum;
                pCorp[iNum].groupid = pAreastatdata->groupid;
                pCorp[iNum].lShflag = pAreastatdata->lShflag;
                pCorp[iNum].lWorkflag = pAreastatdata->lWorkflag;
                pCorp[iNum].lYyflag = pAreastatdata->lYyflag;

                pCorp[iNum].lDataflag = pAreastatdata->lDataflag;
                strcpy(pCorp[iNum].caAddtime, pAreastatdata->caAddtime);
                strcpy(pCorp[iNum].caFcode, pAreastatdata->caFcode);
                pCorp[iNum].lLasttime = pAreastatdata->lLasttime;
                if(pCorp[iNum].endnum < pCorp[iNum].onuid_count)
                {
                    pCorp[iNum].endnum = pCorp[iNum].onuid_count;
                }
                iNum++;
            }
        }

        pAreastatdata = (struct servicedata_s *)pasLHashNext(&sHashInfo);
    }

    lSumcorp = iNum;

    qsort(pCorp, lSumcorp, sizeof(struct servicedata_s), ncsUtlSortOnCorp);

    //  printf("lSumcopr=%d\n",lSumcorp);
    if(!utStrIsSpaces(caExport))
    {
        FILE *fp;
        char caFilename[32];
        char caFile[128];
        char caPath[128];
        char caState_desc[32];
        sprintf(caFilename, "ncwarn%d.csv", utStrGetId());
        sprintf(caPath, "%s/html", "/home/ncmysql/ncs");
        sprintf(caFile, "%s/%s", caPath, caFilename);
        fp = fopen(caFile, "w");
        if(fp == NULL)
        {
            pasDbCloseCursor(psCur);
            utWebDispMsg(iFd, psMsgHead, "ncs/ncmsg_back.htm", ncsLang("0893监控单位"), ncsLang("0546数据导出出错"));
            return 0;
        }
        ncsWriteSysLog(psMsgHead, caUsername2, "06", "成功", "导出上网服务场所信息");

        fprintf(fp, (char *)ncsLang("0893监控单位\n"));
        ncUtlPrintCsv(fp, 12, UT_TYPE_STRING, "场所编码",
                      UT_TYPE_STRING, ncsLang("0906单位名称"),
                      UT_TYPE_STRING, ncsLang("0054IP地址"),
                      UT_TYPE_STRING, ncsLang("0907E-Mail"),
                      UT_TYPE_STRING, ncsLang("0908地址"),
                      UT_TYPE_STRING, ncsLang("0910电话"),
                      UT_TYPE_STRING, ncsLang("0909手机"),
                      UT_TYPE_STRING, ncsLang("0911联系人"),
                      UT_TYPE_STRING, ncsLang("0912最后上线时间"),
                      UT_TYPE_STRING, "单位组",
                      UT_TYPE_STRING, "状态",
                      UT_TYPE_STRING, "备注");

        for(i = 0; i < lSumcorp; i++)
        {
            if(pCorp[i].onstate == 1)
            {
                strcpy(caState_desc, "在线");
            }
            else if(pCorp[i].onstate == 2)
            {
                strcpy(caState_desc, "离线");
            }
            else if(pCorp[i].onstate == 4)
            {
                strcpy(caState_desc, "停业");
            }
            else if(pCorp[i].onstate == 5)
            {
                strcpy(caState_desc, "维护");
            }
            else if(pCorp[i].onstate == 9)
            {
                strcpy(caState_desc, "待审核");
            }
            else
            {
                strcpy(caState_desc, "");
            }
            sprintf(caTemp, "\r%s", pCorp[i].caServicecode);
            ncUtlPrintCsv(fp, 12, UT_TYPE_STRING, caTemp,
                          UT_TYPE_STRING, pCorp[i].caCname,
                          UT_TYPE_STRING, pCorp[i].caIp,
                          UT_TYPE_STRING, pCorp[i].caEmail,
                          UT_TYPE_STRING, pCorp[i].caAddress,
                          UT_TYPE_STRING, pCorp[i].caTel,
                          UT_TYPE_STRING, pCorp[i].caHandphone,
                          UT_TYPE_STRING, pCorp[i].caContact,
                          UT_TYPE_STRING, pCorp[i].caDate,
                          UT_TYPE_STRING, pCorp[i].caGroupname,
                          UT_TYPE_STRING, caState_desc,
                          UT_TYPE_STRING, pCorp[i].caDemo);

        }

        fclose(fp);
        utPltFileDownload(iFd, "application/text", caPath, caFilename, caFilename);
        remove(caFile);
        free(pHash);
        free(pHash_fac);
        free(pCorp);
        return 0;
    }

    psDbHead = utPltInitDb();


    iNum = 0;

    for(i = 0; i < lSumcorp; i++)
    {
        if(i >= lStartRec && iNum < lRowNum)
        {
            if(iNum > 0)
            {
                utPltPutLoopVar(psDbHead, "dh", iNum + 1, ",");
            }
            iNum++;
            utPltPutLoopVarF(psDbHead, "useflags", iNum, "%d", pCorp[i].lUseflags);
            utPltPutLoopVarF(psDbHead, "userid",   iNum, "%lu", pCorp[i].userid);
            utPltPutLoopVar(psDbHead, "username", iNum, pCorp[i].caServicecode);
            utPltPutLoopVar(psDbHead, "dispname", iNum, pCorp[i].caCname);
            utPltPutLoopVar(psDbHead, "groupname", iNum, pCorp[i].caGroupname);
            utPltPutLoopVarF(psDbHead, "num", iNum, "%d", iNum - 1);
            utPltPutLoopVar(psDbHead, "email", iNum, pCorp[i].caEmail);
            utPltPutLoopVar(psDbHead, "addr", iNum, pCorp[i].caAddress);
            utPltPutLoopVar(psDbHead, "contact", iNum, pCorp[i].caContact);
            utPltPutLoopVar(psDbHead, "ip", iNum, pCorp[i].caIp);
            utPltPutLoopVar(psDbHead, "version", iNum, pCorp[i].caVersion);
            utPltPutLoopVarF(psDbHead, "groupid", iNum, "%lu", pCorp[i].groupid);
            utPltPutLoopVar(psDbHead, "telphone", iNum, pCorp[i].caTel);
            utPltPutLoopVar(psDbHead, "jointype", iNum, ncsGetJoinTypeByCode(pCorp[i].caJointype));
            //              if(strlen(pCorp[i].caServicecode)>10){
            //          memset(caCode,0,sizeof(caCode));
            //          char *p=&pCorp[i].caServicecode[0];
            //          memcpy(caCode,p+7,1);
            //        printf("caCode=%s\n",caCode);
            //            utPltPutLoopVar(psDbHead,"dwlx",iNum,ncsGetUserTnameByType(caCode));
            //       }
            memset(caCode, 0, sizeof(caCode));
            strcpy(caCode, pCorp[i].caFcode);
            psFac = (struct fax_s *)pasLHashLook(pHash_fac, caCode);
            if(psFac)
            {
                utPltPutLoopVar(psDbHead, "jrcj", iNum, psFac->caName);
            }


            memset(caCode, 0, sizeof(caCode));
            sprintf(caCode, "%s", pCorp[i].caGtype);
            //        printf("caCode=%s\n",caCode);
            psFac = (struct fax_s *)pasLHashLook(pHash_lb, caCode);
            if(psFac)
            {
                //         printf("aaaaaaaa=%s\n",psFac->caName);
                utPltPutLoopVar(psDbHead, "dwlb", iNum, psFac->caName);
            }

            utPltPutLoopVarF(psDbHead, "shflag", iNum, "%d", pCorp[i].lShflag);
            utPltPutLoopVarF(psDbHead, "workflag", iNum, "%d", pCorp[i].lWorkflag);
            utPltPutLoopVarF(psDbHead, "yyflag", iNum, "%d", pCorp[i].lYyflag);


            utPltPutLoopVar(psDbHead, "xxdesc", iNum, "详细");
            utPltPutLoopVarF(psDbHead, "endnum", iNum, "%d", pCorp[i].endnum);
            utPltPutLoopVarF(psDbHead, "onuser", iNum, "%lu", pCorp[i].onuid_count);

            if(isInOnlinePlaceMemory(psShmHead, pCorp[i].caServicecode))
            {
                utPltPutLoopVarF(psDbHead, "dataflag", iNum, "%d", 1);//有数据1,无数据2
                utPltPutLoopVar(psDbHead, "status", iNum, "<font color=green>正常</font>");
            }
            else
            {
                utPltPutLoopVarF(psDbHead, "dataflag", iNum, "%d", 2);
                utPltPutLoopVar(psDbHead, "status", iNum, "<font color=red>离线</font>");
            }
            /*
            if(pCorp[i].onstate == 1)
            {
                if(pCorp[i].lDataflag != 1)
                {
                    //utPltPutLoopVar(psDbHead,"status", iNum,"<font color=blue>异常</font>");
                    utPltPutLoopVar(psDbHead,"status", iNum,"<font color=green>正常</font>");
                }
                else
                {
                    utPltPutLoopVar(psDbHead, "status", iNum, "<font color=green>在线</font>");
                }
            }
            else if(pCorp[i].onstate == 2)
            {
                utPltPutLoopVar(psDbHead, "status", iNum, "<font color=red>离线</font>");
            }
            else if(pCorp[i].onstate == 4)
            {
                utPltPutLoopVar(psDbHead, "status", iNum, "<font color=blue>停业</font>");
            }
            else if(pCorp[i].onstate == 5)
            {
                utPltPutLoopVar(psDbHead, "status", iNum, "维护");
            }
            else if(pCorp[i].onstate == 9)
            {
                utPltPutLoopVar(psDbHead, "status", iNum, "待审核");
            }
            */
            utPltPutLoopVar(psDbHead, "xxdesc", iNum, "详细");

            if(pCorp[i].lLasttime > 0)
            {
                utPltPutLoopVar(psDbHead, "lasttime", iNum, utTimFormat("%Y-%m-%d %H:%M:%S", pCorp[i].lLasttime));
            }

            sprintf(caTemp, "%s", pCorp[i].caGtype);
            utPltPutLoopVar(psDbHead, "gtype", iNum, ncsGetUserTnameByType(caTemp));
            utPltPutLoopVar(psDbHead, "demo", iNum, pCorp[i].caDemo);

            utPltPutLoopVarF(psDbHead, "CurPg", iNum, "%lu", lCurPg);

            if(atol(caLastln) == 1 || (pCorp[i].onstate != 1))
            {
                //获取上次客户端连接时间
                sprintf(caTemp, "select sdate from ncscltlog where userid=%lu order by sdate desc limit 0,1 ", pCorp[i].userid);
                //printf("pid=%d\n",getpid());
                //sleep(15);

               // printf("caTemp=%s\n", caTemp);
                memset(caClastime, 0, sizeof(caClastime));

                pasDbOneRecord(caTemp, 0, UT_TYPE_STRING, 20, caClastime);
                utPltPutLoopVar(psDbHead, "clastime", iNum, caClastime);
            }

        }
    }


    lCount = lSumcorp;
    lTotPg = (lCount - 1) / lRowNum + 1;
    lTotRec = lCount;
    i = 0;
    //    while(iNum < lRowNum + 1) {
    //       i++;

    //       utPltPutLoopVarF(psDbHead,"snum", i,"%d",iNum);
    //        iNum++;
    //    }
    utPltPutVar(psDbHead, "sort", caSort);
    utPltPutVar(psDbHead, "sortby", caSortby);

    utPltPutVarF(psDbHead, "TotRec", "%lu", lTotRec);
    utPltPutVarF(psDbHead, "TotPg", "%lu", lTotPg);
    utPltPutVarF(psDbHead, "CurPg", "%lu", lCurPg);
    utPltPutVar(psDbHead, "groupid", caGroupid);
    utPltPutVar(psDbHead, "did", caDid);
    utPltPutVar(psDbHead, "name", name);

    utPltPutVar(psDbHead, "servicecode1", servicecode1);
    utPltPutVar(psDbHead, "dispname1", dispname1);
    utPltPutVar(psDbHead, "ceoname1", ceoname1);
    utPltPutVar(psDbHead, "contact1", contact1);
    utPltPutVar(psDbHead, "ip1", ip1);
    utPltPutVar(psDbHead, "servicestate1", servicestate1);
    utPltPutVar(psDbHead, "gtype", caGtype);
    utPltPutVar(psDbHead, "fcode", caFcod_c);
    utPltPutVar(psDbHead, "lxstate", caLxstate);
    if(pHash) free(pHash);
    if(pCorp) free(pCorp);

    free(pHash_fac);
    free(pHash_lb);
    //  utPltShowDb(psDbHead);

    utPltOutToHtml(iFd, psMsgHead, psDbHead, "v4/ncs_corp_list.htm");

    return 0;
}
//保存AP信息
int proauth_apset_Save_v9(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead)
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
    char caOldApname[32] = "";
    char caDispname[128] = "";
    memset(caUsername2, 0, sizeof(caUsername2));
    dsCltGetSessionValue(1, "dispname", UT_TYPE_STRING, 30, caUsername2);


    utMsgPrintMsg(psMsgHead);

    psDbHead = utPltInitDbHead();

    utMsgGetSomeNVar(psMsgHead, 120,
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
                     "oldApname", UT_TYPE_STRING, sizeof(caOldApname) - 1, caOldApname,
                     "dispname", UT_TYPE_STRING, sizeof(caDispname) - 1, caDispname

                    );


    if(strlen(caDispname) > 0)
    {
        pasCvtGBK(2, caDispname, caTemp, 128);
        strcpy(caDispname, caTemp);
    }
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
    if(strlen(caOldApname) > 0)
    {
        sprintf(caTemp, "select count(*) from ncaplist where apname='%s' ", caOldApname);
    }
    else
    {
        sprintf(caTemp, "select count(*) from ncaplist where apname='%s' ", caGwid);
    }

   // printf("1831 caTemp=%s\n", caTemp);
    lCount = 0;
    pasDbOneRecord(caTemp, 0, UT_TYPE_LONG, 4, &lCount);
    if(lCount > 0)
    {
        sprintf(caTemp, "修改AP%s信息", caGwid);
        ncsWriteSysLog(psMsgHead, caUsername2, "03", "成功", caTemp);
        if(strlen(caOldApname) > 0)
        {
            printf("update ncaplist set mac='%s',did=%s,address='%s',version='%s',servicecode='%s',plate='%s',line='%s',mapid='%s',ssid='%s',longitude='%s',latitude='%s',lasttime=%d,flags=%d,servicename='%s',aptype=%d,fcode='%s',uptimestep=%d,apname='%s',dispname='%s' where apname='%s' \n",
                   caMac, caGroupid, caAddress, caVersion, caServicecode, caPlate, caLine, caMapid, caSsid, caLongitude, caLatitude, time(0), atol(caFlags), caServicename, atol(caAptype), caFcode, atol(caUptimestep), caGwid, caDispname, caOldApname);


            iReturn = pasDbExecSqlF("update ncaplist set mac='%s',did=%s,address='%s',version='%s',servicecode='%s',plate='%s',line='%s',mapid='%s',ssid='%s',longitude='%s',latitude='%s',lasttime=%d,flags=%d,servicename='%s',aptype=%d,fcode='%s',uptimestep=%d,radius=%d,apname='%s',dispname='%s' where apname='%s' ",
                                    caMac, caGroupid, caAddress, caVersion, caServicecode, caPlate, caLine, caMapid, caSsid, caLongitude, caLatitude, time(0), atol(caFlags), caServicename, atol(caAptype), caFcode, atol(caUptimestep), atol(caRadius), caGwid, caDispname, caOldApname);

        }
        else
        {
            printf("update ncaplist set mac='%s',did=%s,address='%s',version='%s',servicecode='%s',plate='%s',line='%s',mapid='%s',ssid='%s',longitude='%s',latitude='%s',lasttime=%d,flags=%d,servicename='%s',aptype=%d,fcode='%s',uptimestep=%d,dispname='%s' where apname='%s' \n",
                   caMac, caGroupid, caAddress, caVersion, caServicecode, caPlate, caLine, caMapid, caSsid, caLongitude, caLatitude, time(0), atol(caFlags), caServicename, atol(caAptype), caFcode, atol(caUptimestep), caDispname, caGwid);

            iReturn = pasDbExecSqlF("update ncaplist set mac='%s',did=%s,address='%s',version='%s',servicecode='%s',plate='%s',line='%s',mapid='%s',ssid='%s',longitude='%s',latitude='%s',lasttime=%d,flags=%d,servicename='%s',aptype=%d,fcode='%s',uptimestep=%d,radius=%d,dispname='%s' where apname='%s' ",
                                    caMac, caGroupid, caAddress, caVersion, caServicecode, caPlate, caLine, caMapid, caSsid, caLongitude, caLatitude, time(0), atol(caFlags), caServicename, atol(caAptype), caFcode, atol(caUptimestep), atol(caRadius), caDispname, caGwid);

        }
    }
    else
    {
		/*
        printf("======insert into ncaplist (apname,mac,did,address,version,servicecode,plate,line,mapid,ssid,longitude,latitude,starttime,lasttime,flags,servicename,aptype,fcode,uptimestep) \
				  values('%s','%s',%s,'%s','%s','%s','%s','%s','%s','%s','%s','%s',%d,%d,%d,'%s',%d,'%s',%d)\n", \
               caGwid, caMac, caGroupid, caAddress, caVersion, caServicecode, caPlate, caLine, caMapid, caSsid, caLongitude, caLatitude, time(0), time(0), atol(caFlags), caServicename, atol(caAptype), caFcode, atol(caUptimestep));
              */
        iReturn = pasDbExecSqlF("insert into ncaplist (apname,mac,did,address,version,servicecode,plate,line,mapid,ssid,longitude,latitude,starttime,lasttime,flags,servicename,aptype,fcode,uptimestep,radius,dispname) \
				  values('%s','%s',%s,'%s','%s','%s','%s','%s','%s','%s','%s','%s',%d,%d,%d,'%s',%d,'%s',%d,%d,'%s')", \
                                caGwid, caMac, caGroupid, caAddress, caVersion, caServicecode, caPlate, caLine, caMapid, caSsid, caLongitude, caLatitude, time(0), time(0), atol(caFlags), caServicename, atol(caAptype), caFcode, atol(caUptimestep), atol(caRadius), caDispname);

        sprintf(caTemp, "添加AP%s信息", caGwid);
        ncsWriteSysLog(psMsgHead, caUsername2, "03", "成功", caTemp);
    }

    //printf("iReturn=%d\n", iReturn);

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

//
//显示AP信息
int proauthWebAplist_v9(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead)
{
    char caTemp[2024];
    char caEdate[20];
    unsigned long lCurPg, lTotPg;
    unsigned long lTime, l, lTime1, lSip, lDip;
    int i, iNum;
    int iReturn;
    long lRowNum, lflags, lStartRec, lCount, lSid;
    char sqlbuf[2048] = "";
    long lRadius;
    pasDbCursor *psCur;
    char cadispname[64] = "";
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
    char caGroupids[2048] = "", caDel[20] = "", caSelsid[512] = "";
    char caGwid[36], caMac[68], caAddress[256], caVersion[32], caMark[68];
    char caPlate[36], caLine[36], caMapid[36], caSsid[36];
    unsigned long lGroupid, lFlags;
    double fLongitude, fLatitude;
    char caServicecode[20], caServicename[128], caLongitude[20], caLatitude[20];
    char caUsername2[36];
    char caSh[16];
    char caServicecodes[8024] = "";
    memset(caUsername2, 0, sizeof(caUsername2));
    dsCltGetSessionValue(1, "dispname", UT_TYPE_STRING, 30, caUsername2);
   // printf("proauthWebAplist_v9..start ");
    utPltDbHead *psDbHead = utPltInitDb();  // 初始化数据模板
    utMsgPrintMsg(psMsgHead);
    //该用户所管辖的区域列表
    strcpy(caServicecodes, getServicecodesByDids(psShmHead, getDsGroupids()));
    //printf("caServicecodes==[%s]\n",caServicecodes);

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
   // printf("caSelsid=[%s]\n", caSelsid);
   // printf("  proauthWebAplist_v9  start.......\n");
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

   // printf("caDel=%s\n", caDel);
    if(strcmp(caDel, "del") == 0)
    {
        pasDbExecSqlF("delete from ncaplist where apname in (%s) ", caSelsid);
       // printf("sql='delete from ncaplist where apname in (%s)' \n", caSelsid);
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
        sprintf(sqlbuf + strlen(sqlbuf), "and (apname like '%c%s%c' or mac like '%c%s%c' or address like '%c%s%c' or plate like '%c%s%c' or line like '%c%s%c' or servicecode like '%c%s%c' or servicename like '%c%s%c' or dispname like '%c%s%c' )", '%', caKeyword, '%', '%', caKeyword, '%', '%', caKeyword, '%', '%', caKeyword, '%', '%', caKeyword, '%', '%', caKeyword, '%', '%', caKeyword, '%', '%', caKeyword, '%');
       // printf("1529sqlbuf=[%s]\n", sqlbuf);
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
    if(strlen(caServicecodes) == 0)
    {
        sprintf(sqlbuf, "select apname,mac,did,address,version,servicecode,plate,line,mapid,ssid,longitude,latitude,a.flags,servicename,fcode,aptype,uptimestep,b.name,a.radius,dispname from ncaplist a left join ncsfactorycod b on (fcode=b.code) where 1=1 ");
    }
    else
    {
        sprintf(sqlbuf, "select apname,mac,did,address,version,servicecode,plate,line,mapid,ssid,longitude,latitude,a.flags,servicename,fcode,aptype,uptimestep,b.name,a.radius,dispname from ncaplist a left join ncsfactorycod b on (fcode=b.code) where 1=1 and servicecode  in (%s) ", caServicecodes);


    }
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

   // printf("sqlbuf=%s\n,calDel=[%s]\n", sqlbuf, caDel);
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
                             UT_TYPE_LONG, 4, &lRadius,
                             UT_TYPE_STRING, sizeof(cadispname) - 1, cadispname

                            );




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
        utPltPutLoopVarF(psDbHead, "dispname", iNum, cadispname);
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
                                 UT_TYPE_LONG, 4, &lRadius,
                                 UT_TYPE_STRING, sizeof(cadispname) - 1, cadispname
                                );
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

int procapAlarmSetFun(utShmHead *psShmHead)
{
    pasSetTcpFunName("ncsSys_all_mac", ncsSys_all_mac, 0);
    pasSetTcpFunName("ncsSys_terminal_group", ncsSys_terminal_group, 0);
    pasSetTcpFunName("ncsSysoperation", ncsSysoperation, 0);
    pasSetTcpFunName("ncsSys_controlmanage", ncsSys_controlmanage, 0);
    pasSetTcpFunName("ncsSys_stafinfo", ncsSys_stafinfo, 0);
    pasSetTcpFunName("ncsSys_terminal", ncsSys_terminal, 0);
    pasSetTcpFunName("ncsSys_stafinfo_deal", ncsSys_stafinfo_deal, 0);
    pasSetTcpFunName("ncsWebClientSearch_wif_v9", ncsWebClientSearch_wif_v9, 0);
    pasSetTcpFunName("proauth_apset_Save_v9", proauth_apset_Save_v9, 0);
    pasSetTcpFunName("proauthWebAplist_v9", proauthWebAplist_v9, 0);

}









