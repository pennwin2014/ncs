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
#include "pasutl.h"
#include "pasmessage.h"
#include "ncmac.h"

//单位树AJAX通讯
int ncsTreeUser_v4_v9(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead)
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
    char caLongitude[16];
    char caLatitude[16];
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
            psData = (struct stData *)pasLHashLookA(pHash, &lPid);
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
            psData = (struct stData *)pasLHashLookA(pHash, &lPid);
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
            psData = (struct stData *)pasLHashLook(pHash, &lGroupid);
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
    sprintf(caTemp, "select dispname,userid,groupid,username,longitude,latitude  from ncsuser where 1=1 ");
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
    printf("%dcaTemp=%s\n", __LINE__, caTemp);
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
                                 UT_TYPE_STRING, 15, caUsername,
                                 UT_TYPE_STRING, 15, caLongitude,
                                 UT_TYPE_STRING, 15, caLatitude
                                );

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
            utPltPutLoopVar(psDbHead, "longitude", iNum, caLongitude);
            printf("caLongitude=[%s]\n", caLongitude);
            // utPltPutLoopVar(psDbHead,"latitude",iNum,'caLatitude');
            utPltPutLoopVar(psDbHead, "latitude", iNum, '12');
            printf("caLatitude=[%s]\n", caLatitude);
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
                                     UT_TYPE_STRING, 15, caUsername,
                                     UT_TYPE_STRING, 15, caLongitude,
                                     UT_TYPE_STRING, 15, caLatitude
                                    );
        }
        pasDbCloseCursor(psCur);
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


int procapTreeSetFun(utShmHead * psShmHead)
{
    pasSetTcpFunName("ncsTreeUser_v4_v9", ncsTreeUser_v4_v9, 0);

}