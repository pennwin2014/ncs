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
#include "utoall.h"
#include "utoplt01.h"
#include "pasdb.h"
#include "dsutl.h"
#include "dsclt.h"
#include "ncsdef.h"
#include"dlfcn.h"

int ncsSysDispTool(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead)
{
    int i, iret, iNum;
    char caVar[32];
    char caFun[64];
    char caTemp[1024];
    int iReturn, lCount;
    char caSy[36];
    char caId[36];
    char caUsername[64];

    char caName[64];
    long lId;
    utPltDbHead *psDbHead;
    utMsgPrintMsg(psMsgHead);  // 将数据包显示出来,本函数主要用于调试。显示时先显示数据包的头部，然后显示数据包的内容。
    pasDbCursor *psCur = NULL;
    //utMsgGetSomeNVar(psMsgHead,1,"id",UT_TYPE_STRING,sizeof(caId)-1,caId);
    iReturn = dsCltGetMyInfo(2, "Userid", &lId,
                             "Username", caUsername);


    printf("lId is %d \n", lId);
    printf("caUsername is %s \n", caUsername);
    if(lId > 0)
    {
        printf("lId is %d \n", lId);
        pasDbExecSqlF("delete from dstool_v4 where id=%d ", lId);
        pasDbExecSqlF("delete from dstoolsy_v4 where id=%d ", lId);

        pasDbExecSqlF("insert into dstoolsy_v4(id) values(%d)", lId);
        for(i = 0; i < 25; i++)
        {

            sprintf(caVar, "ch%d", i);
            printf("caVar is %s \n", caVar);
            iReturn = utMsgGetSomeNVar(psMsgHead, 1, caVar, UT_TYPE_STRING, sizeof(caFun) - 1, caFun);
            printf("caFun is %s \n", caFun);
            if(iReturn == 1 && strlen(caFun) > 0)
            {

                pasDbExecSqlF("insert into dstool_v4(tname,id) values('%s',%d)", caFun, lId);

            }
        }

    }
    utPltOutToHtml(iFd, psMsgHead, psDbHead, "../html/mac/systemadmin/mytoolbar/proauth_toolbarset.htm");

    return 0;
}

int getDispToolInfo(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead)
{
    utPltDbHead *psDbHead = utPltInitDb();  // 初始化数据模板

    pasDbCursor *psCur = NULL;
    int iret = 0;
    int iNum = 0;
    char caName[64] = "";
    char sql[1024] = "";
    char caUsername[64];
    int iReturn = 0;

    long lId;
    iReturn = dsCltGetMyInfo(2, "Userid", &lId,
                             "Username", caUsername);
    sprintf(sql, "select tname from dstool_v4 where id = %d", lId);
    printf("SQL is %s\n", sql);
    psCur = pasDbOpenSql(sql, 0);
    iret = pasDbFetchInto(psCur, UT_TYPE_STRING, sizeof(caName) - 1,      caName);
    printf("caName is %s\n", caName);

    while(0 == iret || 1405 == iret)
    {

        iNum++;
        if(iNum > 1)
        {
            utPltPutLoopVar(psDbHead, "dh", iNum, ","); // utPltPutLoopVar输出一个循环变量的值。
        }
        utPltPutLoopVar(psDbHead, "caName", iNum, caName);
        memset(caName, 0, sizeof(caName));
        iret = pasDbFetchInto(psCur, UT_TYPE_STRING, sizeof(caName) - 1,      caName);
        printf("caName is %s\n", caName);
    }
    pasDbCloseCursor(psCur);
    utPltOutToHtml(iFd, psMsgHead, psDbHead, "rzcx/proauth_DispToolInfo.htm");
    return 0;
}
//权限配置文件
int ncsSysUserRoleConfig_v9(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead)
{
    pasDbCursor *psCur;
    char caTemp[1024];
    int  iNum;
    char caFun[2048];
    char caTool[2048];
    long lLevel;
    utPltDbHead *psDbHead;
    int iReturn, i;
    long lCount;
    char caName[64], caSy[64];
    long lId, lSid, sId;
    char caUsername[64];

    dsCltGetSessionValue(1, "id", UT_TYPE_LONG, sizeof(long), &sId);
    iReturn = dsCltGetMyInfo(2, "Userid", &lId,
                             "Username", caUsername);
    utMsgPrintMsg(psMsgHead);
    memset(caFun, 0, sizeof(caFun));
    printf("sId is %d\n", sId);
    printf("lId is %d\n", lId);
    printf("caUsername is %s\n", caUsername);
    sprintf(caTemp, "select tname from dstoolsy_v4 where id=%d ", lId);
    memset(caSy, 0, sizeof(caSy));
    pasDbOneRecord(caTemp, 0, UT_TYPE_STRING, 31, caSy);
    if(strlen(caSy) == 0)
    {
        sprintf(caTemp, "select usrlevel from dsuser where id=%d ", lId);
        pasDbOneRecord(caTemp, 0, UT_TYPE_LONG, 4, &lLevel);
        strcpy(caSy, ncsGetFirstNameById(lLevel));
    }

    sprintf(caTemp, "select lright from dsuser a,dsuserrole b,dsroleattr c,dscltright d where a.id=b.id and b.roleid=c.roleid and d.name=lright and a.id=%d group by lright", lId);
    printf("caTemp is %s\n\n", caTemp);
    printf("caTemp is %s\n\n", caTemp);

    psCur = pasDbOpenSqlF(caTemp);


    if(psCur == NULL)
    {
        utWebDispMsg(iFd, psMsgHead, "ncs/ncmsg_back.htm", "部门查询", "数据库出错");
        //        free(pHash);
        return 0;
    }

    memset(caName, 0, sizeof(caName));
    iReturn = pasDbFetchInto(psCur, UT_TYPE_STRING, 31, caName);
    iNum = 0;
    while(iReturn == 0 || iReturn == 1405)
    {

        if(iNum == 0)
        {
            strcpy(caFun, caName);
        }
        else
        {
            sprintf(caFun + strlen(caFun), ",%s", caName);
        }
        iNum++;

        memset(caName, 0, sizeof(caName));
        iReturn = pasDbFetchInto(psCur, UT_TYPE_STRING, 31, caName);
    }
    pasDbCloseCursor(psCur);

    psDbHead = utPltInitDb();
    utPltPutVar(psDbHead, "sy", caSy);
    utPltPutVar(psDbHead, "fun", caFun);

    memset(caTool, 0, sizeof(caTool));
    sprintf(caTemp, "select tname from dstool_v4 where id=%d ", lId);
    psCur = pasDbOpenSqlF(caTemp);
    memset(caTool, 0, sizeof(caTool));
    if(psCur)
    {
        memset(caName, 0, sizeof(caName));
        iReturn = pasDbFetchInto(psCur, UT_TYPE_STRING, 31, caName);
        while(iReturn == 0 || iReturn == 1405)
        {

            sprintf(caTool + strlen(caTool), ",%s", caName);
            memset(caName, 0, sizeof(caName));
            iReturn = pasDbFetchInto(psCur, UT_TYPE_STRING, 31, caName);
        }
        pasDbCloseCursor(psCur);
    }
    utPltPutVar(psDbHead, "tool", caTool);


    // utPltShowDb(psDbHead);

    utPltOutToHtml(iFd, psMsgHead, psDbHead, "v4/ncs_right_config.htm");


    return 0;
}

//显示角色列表
int ncsRolelist_v9(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead)
{
    pasDbCursor *psCur;
    utPltDbHead *psDbHead;
    long iReturn, lCount;
    char caStart[16], caTemp[2048], caCurPg[16];
    char caContent[64], caKtype[8], caCaseflags[8];
    long lCurPg, lRowNum, lStartRec, sid, nettype, flags;
    char caName[64], caNetname[32], caTemp_d[2048], caStatus[16];
    char caRightcheck[2048], caRolename[64];
    char caTid[16], caKid[16];
    long lSid, lTid;
    char caPre[36];
    long iNum;
    long lktype, lcaseflags, ltid;
    char caCid[16], caLimit[16], caPage[16], caDel[16], caDir[16], caKeyword[64], caSelsid[1024], caUpdate[16], caNtype[16], caNetid[64], caSid[16];
    long preid, lId;
    char caGroupname[32];
    long lTime, lLevel, i;

    long lPasid;
    pasLHashInfo sHashInfo;
    char *pHash;
    struct rData_s
    {
        unsigned long id;
        char caFun[2048];
        char caUser[2048];
    };
    struct rData_s *psData;
    char caOut[200][256];
    long lSum;
    char caRoleid[16];
    memset(caOut, 0, 200 * 36);





    utMsgPrintMsg(psMsgHead);
    /* 根据Roleid和Pasid取相应的权限信息  */
    pasDbOneRecord("select id from dsappsrv", 0, UT_TYPE_LONG, 4, &lPasid);



    iReturn = utMsgGetSomeNVar(psMsgHead, 11,
                               "limit",            UT_TYPE_STRING, 10, caLimit,
                               "page",          UT_TYPE_STRING, 10, caPage,
                               "dir",           UT_TYPE_STRING, 8, caDir,
                               "keyword",       UT_TYPE_STRING, 30, caKeyword,
                               "selsid", UT_TYPE_STRING, 1000, caSelsid,
                               "start", UT_TYPE_STRING, 10, caStart,
                               "del", UT_TYPE_STRING, 10, caDel,
                               "update", UT_TYPE_STRING, 10, caUpdate,
                               "rolename", UT_TYPE_STRING, 31, caRolename,
                               "rightcheck", UT_TYPE_STRING, 2000, caRightcheck,
                               "roleid", UT_TYPE_STRING, 10, caRoleid);

    if(strlen(caKeyword) > 0)
    {
        strcpy(caTemp_d, pasCvtGBK(2, caKeyword, caTemp, 63));
        strcpy(caKeyword, caTemp_d);
    }

    if(strlen(caRightcheck) > 0)
    {
        strcpy(caTemp_d, pasCvtGBK(2, caRightcheck, caTemp, 2000));
        strcpy(caRightcheck, caTemp_d);
    }
    if(strlen(caRolename) > 0)
    {
        strcpy(caTemp_d, pasCvtGBK(2, caRolename, caTemp, 63));
        strcpy(caRolename, caTemp_d);
    }


    lRowNum = atol(caLimit);
    if(lCurPg <= 0) lCurPg = 1;
    lStartRec = atol(caStart);

    psDbHead = utPltInitDb();

    //printf("caRightcheck=%s\n",caRightcheck);
    if(strcmp(caDel, "del") == 0 && strlen(caSelsid) > 0)
    {
        //     printf("delete from  dsuser  where id in(%s)\n",caSelsid);
        iReturn = pasDbExecSqlF("delete from dsuserrole where roleid  in(%s) and roleid!=101 ", caSelsid);
        iReturn = pasDbExecSqlF("delete from dsroleattr where roleid  in(%s) and roleid!=101 ", caSelsid);

        if(iReturn == 0)
        {
            pasDbExecSqlF("delete from dsrole where id  in(%s) and id!=101 ", caSelsid);
        }
    }

    if((strcmp(caUpdate, "update") == 0) && strlen(caRolename) > 0)
    {
        lCount = sepcharbydh(caRightcheck, caOut);

        sprintf(caTemp, "select id from dsrole where name='%s' ", caRolename);
        //      printf("caTemp=%s\n",caTemp);
        lId = 0;
        pasDbOneRecord(caTemp, 0, UT_TYPE_LONG, 4, &lId);
        if(lId == 0)
        {
            lId = pasGetSid("dsrole", "id");
            iReturn = pasDbExecSqlF("insert into dsrole(id,name,dispname,descr,addtime,moditime) values (%lu,'%s','%s','%s',%lu,%lu)",
                                    lId, caRolename, caRolename, caRolename, time(0), time(0));

            if(iReturn == 0)
            {
                pasDbExecSqlF("delete from dsroleattr where roleid=%d ", lId);
                //       printf("delete from dsroleattr where roleid=%d \n",lId);
                for(i = 0; i < lCount; i++)
                {
                    if(strlen(caOut[i]) > 0)
                        //           printf("caOut=%s\n",caOut[i]);
                        //            printf("insert into dsroleattr(roleid,pasid,flags,lright) values(%d,%d,0,'%s')\n",lId,lPasid,caOut[i]);
                        pasDbExecSqlF("insert into dsroleattr(roleid,pasid,flags,lright) values(%d,%d,0,'%s')", lId, lPasid, caOut[i]);
                }

            }
        }
        else
        {
            iReturn = pasDbExecSqlF("update dsrole set name='%s',dispname='%s',descr='%s',moditime=%lu where id=%d ",
                                    caRolename, caRolename, caRolename, time(0), lId);
            pasDbExecSqlF("delete from dsroleattr where roleid=%d ", lId);
            for(i = 0; i < lCount; i++)
            {
                if(strlen(caOut[i]) > 0)
                    pasDbExecSqlF("insert into dsroleattr(roleid,pasid,flags,lright) values(%d,%d,0,'%s')", lId, lPasid, caOut[i]);
            }

        }

    }
    //授权
    if((strcmp(caUpdate, "grant") == 0) && (strlen(caRoleid) > 0))
    {
        lCount = sepcharbydh(caRightcheck, caOut);
        iReturn = pasDbExecSqlF("delete from dsuserrole where roleid=%d", atol(caRoleid));
        if(lCount > 0)
        {

            if(iReturn == 0)
            {
                for(i = 0; i < lCount; i++)
                {
                    pasDbExecSqlF("insert into dsuserrole(id,roleid,addtime,addby) values(%d,%d,%d,'')", atol(caOut[i]), atol(caRoleid), time(0));
                }
            }
        }

    }


    //将权限功能列表放到HASH表中

    lCount = 0;
    pasDbOneRecord("select count(*) from dsrole ", 0, UT_TYPE_LONG, 4, &lCount);
    if(lCount < 100)
    {
        lSum = 100;
    }
    else
    {
        lSum = lCount * 1.2;
    }
    printf("lSum=%d\n", lSum);
    pHash = (unsigned char *)pasLHashInit(lSum, lSum, sizeof(struct rData_s), 0, 4);
    if(pHash == NULL)
    {
        printf("354init hash error\n");
        return 0;
    }

    sprintf(caTemp, "select roleid,lright from dsroleattr where pasid=%d order by lright", lPasid);



    psCur = pasDbOpenSql(caTemp, 0);
    //printf("caTemp=%s\n",caTemp);
    if(psCur == NULL)
    {
        utWebDispMsg(iFd, psMsgHead, "ncs/ncmsg_back.htm", "部门查询", "查询数据出错2 ");
        printf("366caTemp=%s\n", caTemp);
        return 0;
    }
    lId = 0;
    memset(caGroupname, 0, sizeof(caGroupname));
    iReturn = pasDbFetchInto(psCur, UT_TYPE_LONG, 4, &lId,
                             UT_TYPE_STRING, 31, caGroupname);


    while(iReturn == 0 || iReturn == 1405)
    {

        psData = (struct rData_s *)pasLHashLookA(pHash, &lId);
        if(psData)
        {
            if(strlen(psData->caFun) == 0)
            {
                strcpy(psData->caFun, caGroupname);
            }
            else
            {
                sprintf(psData->caFun + strlen(psData->caFun), ",%s", caGroupname);
            }
        }

        lId = 0;
        memset(caGroupname, 0, sizeof(caGroupname));
        iReturn = pasDbFetchInto(psCur, UT_TYPE_LONG, 4, &lId,
                                 UT_TYPE_STRING, 31, caGroupname);
    }

    pasDbCloseCursor(psCur);

    //将角色对象放到hash中

    sprintf(caTemp, "select a.roleid,b.dispname from dsuserrole a,dsuser b where a.id=b.id group by b.dispname,a.roleid;");
    psCur = pasDbOpenSql(caTemp, 0);
    printf("caTemp=%s\n", caTemp);
    if(psCur == NULL)
    {
        utWebDispMsg(iFd, psMsgHead, "ncs/ncmsg_back.htm", "部门查询", "查询数据出错2 ");
        printf("400caTemp=%s\n", caTemp);
        return 0;
    }
    lId = 0;
    memset(caGroupname, 0, sizeof(caGroupname));
    iReturn = pasDbFetchInto(psCur, UT_TYPE_LONG, 4, &lId,
                             UT_TYPE_STRING, 31, caGroupname);


    while(iReturn == 0 || iReturn == 1405)
    {

        psData = (struct rData_s *)pasLHashLookA(pHash, &lId);
        if(psData)
        {
            if(strlen(psData->caUser) == 0)
            {
                strcpy(psData->caUser, caGroupname);
            }
            else
            {
                sprintf(psData->caUser + strlen(psData->caUser), ",%s", caGroupname);
            }
        }

        lId = 0;
        memset(caGroupname, 0, sizeof(caGroupname));
        iReturn = pasDbFetchInto(psCur, UT_TYPE_LONG, 4, &lId,
                                 UT_TYPE_STRING, 31, caGroupname);
    }

    pasDbCloseCursor(psCur);






    long lCount2;
    iReturn = dsCltGetMyInfo(1, "Userid", &lId);
    if(iReturn != 0 || lId <= 0)
    {
        lId = 0;
    }
    if(lId > 0)
    {
        sprintf(caTemp, "select count(*) from dsuserrole where roleid=101 and id=%d ", lId);
        pasDbOneRecord(caTemp, 0, UT_TYPE_LONG, 4, &lCount2);
    }


    if(lCount2 > 0 || lId == 0)
    {
        sprintf(caTemp, "select count(*) from dsrole where 1=1 ");
    }
    else
    {
        sprintf(caTemp, "select count(distinct roleid) from dsrole,dsuserrole where dsrole.id=roleid and dsuserrole.id=%d ", lId);
    }
    if(strlen(caKeyword) > 0)
    {
        sprintf(caTemp + strlen(caTemp), " and (name like '%c%s%c') ", '%', caKeyword, '%');
    }
    lCount = 0;

    printf("caTemp=%s\n", caTemp);
    iReturn = pasDbOneRecord(caTemp, 0, UT_TYPE_LONG, 4, &lCount);

    if(lCount2 > 0 || lId == 0)
    {
        sprintf(caTemp, "select id,name from dsrole where 1=1 ");
    }
    else
    {
        sprintf(caTemp, "select distinct roleid,name from dsrole ,dsuserrole where dsrole.id=roleid and dsuserrole.id=%d ", lId);
    }

    if(strlen(caKeyword) > 0)
    {
        sprintf(caTemp + strlen(caTemp), " and (name like '%c%s%c') ", '%', caKeyword, '%');
    }
    sprintf(caTemp + strlen(caTemp), " order by name limit %d,%d", lStartRec, lRowNum);

    printf("caTemp=%s\n", caTemp);


    psCur = pasDbOpenSql(caTemp, 0);

    if(psCur == NULL)
    {
        utWebDispMsg(iFd, psMsgHead, "ncs/ncmsg_back.htm", ncsLang("0561数据统计出错"), ncsLang("0171查询数据出错"));
        printf("478caTemp=%s\n", caTemp);
        return 0;
    }


    iReturn = 0;
    iNum = 0;
    while((iReturn == 0 || iReturn == 1405) && iNum < lRowNum)
    {

        lId = 0;
        memset(caName, 0, sizeof(caName));
        iReturn = pasDbFetchInto(psCur, UT_TYPE_LONG, 4, &lId,
                                 UT_TYPE_STRING, 31, caName);




        if((iReturn != 0) && (iReturn != 1405)) break;
        iNum++;
        if(iNum > 1)
        {
            utPltPutLoopVar(psDbHead, "dh", iNum, ",");
        }
        utPltPutLoopVarF(psDbHead, "num", iNum, "%lu", iNum);
        utPltPutLoopVarF(psDbHead, "userid", iNum, "%lu", lId);
        utPltPutLoopVar(psDbHead, "username", iNum, caName);

        psData = pasLHashLook(pHash, &lId);
        if(psData)
        {
            utPltPutLoopVar(psDbHead, "fun", iNum, psData->caFun);
            utPltPutLoopVar(psDbHead, "dx", iNum, psData->caUser);
        }



    }
    pasDbCloseCursor(psCur);


    utPltPutVarF(psDbHead, "TotRec", "%d", lCount);

    //  ncsWriteSysLog(psMsgHead,caUsername2,"06","成功","查看系统用户列表信息");

    free(pHash);

    //  utPltShowDb(psDbHead);
    utPltOutToHtml(iFd, psMsgHead, psDbHead, "v4/ncs_role_list.htm");
    return 0;
}

int ncsChangePass(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead)
{
    utPltDbHead *psDbHead;
    printf("hhas");
    char caUsername[33], caPassword1[33], caPassword2[33], oldPassword[33];
    char caPass0[33], caOldpass[33], caUserid[33];

    char caKey[32], caKey0[64], sql[512], caKey1[64];
    long lId;
    int iReturn;
    pasDbCursor *psCur;
    iReturn = dsCltGetMyInfo(2, "Userid", &lId,
                             "Username", caUsername);
    psDbHead = utPltInitDbHead();
    printf("Userid is %d\n", lId);
    printf("iReturn is %d\n", iReturn);
    if(iReturn < 0)
    {
        utWebDispMsg(iFd, psMsgHead, "nc/ncmsg_back.htm", "用户登录", "用户没有登录 ");
        return 0;
    }

    iReturn = utMsgGetSomeNVar(psMsgHead, 3,
                               "rname", UT_TYPE_STRING, 30, oldPassword,
                               "password11",     UT_TYPE_STRING, 31, caPassword1,
                               "password21",    UT_TYPE_STRING, 31, caPassword2);
    printf("caUsername is %s\n", caUsername);
    printf("oldPassword is %s\n", oldPassword);
    printf("caPassword1 is %s\n", caPassword1);
    printf("caPassword2 is %s\n", caPassword2);

    utStrDelSpaces(caPassword2);
    utStrDelSpaces(caPassword1);
    utStrDelSpaces(caUsername);
    utStrDelSpaces(oldPassword);

    sprintf(caKey0, "%s%s", caUsername, oldPassword);

    utMd5Ascii22(caKey0, strlen(caKey0), NULL, caKey);

    sprintf(sql, "select lKey from dsuser where name='%s'", caUsername);
    printf("sql is %s\n", sql);
    psCur = pasDbOpenSql(sql, 0);

    if(psCur != NULL)
    {
        iReturn = pasDbFetchInto(psCur, UT_TYPE_STRING, 63, caKey1);
    }
    pasDbCloseCursor(psCur);

    printf("caKey1 is %s\n", caKey1);
    printf("caKey is %s\n", caKey);
    int iret1, iret2;
    iret1 = strcmp(caKey, caKey1);
    iret2 = strcmp(caPassword1, caPassword2);
    printf("iret1---%d\n", iret1);
    printf("iret2----%d\n", iret2);
    iReturn = 1;


    if(iret1 == 0 && iret2 == 0)
    {
        sprintf(caKey0, "%s%s", caUsername, caPassword1);

        utMd5Ascii22(caKey0, strlen(caKey0), NULL, caKey);
        printf("caKey is %s\n", caKey);
        printf("caUsername is %s\n", caUsername);
        iReturn = pasDbExecSqlF("update dsuser set lKey='%s' where name='%s' ", caKey, caUsername);
    }
    if(iReturn == 0)
    {
        printf("成功\n");
        utPltPutVar(psDbHead, "title", "true");
        //      utPltPutVarF(psDbHead,"sid","%s",caSid);
        printf("成功\n");
        utPltPutVar(psDbHead, "message", "保存成功");
    }
    else
    {
        printf("失败\n");
        utPltPutVar(psDbHead, "title", "false");
        //      utPltPutVarF(psDbHead,"sid","%s",caSid);
        utPltPutVar(psDbHead, "message", "保存失败，信息填写错误");
    }

    utPltOutToHtml(iFd, psMsgHead, psDbHead, "systemset/nc_adfile.htm");

    return 0;


}



int procap_myaccount_setfun(utShmHead *psShmHead)
{
    pasSetTcpFunName("ncsSysDispTool", ncsSysDispTool, 0);
    pasSetTcpFunName("getDispToolInfo", getDispToolInfo, 0);
    pasSetTcpFunName("ncsSysUserRoleConfig_v9", ncsSysUserRoleConfig_v9, 0);
    pasSetTcpFunName("ncsRolelist_v9", ncsRolelist_v9, 0);
    pasSetTcpFunName("ncsChangePass", ncsChangePass, 0);


}