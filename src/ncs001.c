/*  Last Modify  2004/06/05  */
#define PAS_SRCFILE    3101
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/time.h>
#include "utoall.h"
#include "ncsdef.h"
#include "pasutl.h"
#include "pasdb.h"
#include "pasmessage.h"
#include "ncfileserver.h"
#define max(a,b) ((a) > (b) ? (a) : (b))

int ncsUtlCheckOnlineClt(utShmHead *psShmHead);

/* 网络督察初始化功能  */
int ncsInitProeim(utShmHead *psShmHead)
{
    int iReturn;
    long lCount;
    ncsSystemInfo *psInfo;
    /* 初试化序列号  */
    if( (iReturn = ncsInitSeque(psShmHead)) < 0) {
        return iReturn;
    }
    ncsInitSysBuf(psShmHead);
//    pasTcpSetOntimeDo(ncsUtlCheckOnlineClt,30,(char *)psShmHead);
    return 0;
}



int ncsInitSysBuf(utShmHead *psShmHead)
{
    int iReturn;
    long lCount;
    ncsSystemInfo *psInfo;
    unsigned long lSid,lLastTime;
    utShmFreeArray(psShmHead,NCS_LNK_SYSINFO);  
    iReturn = utShmInitArray(psShmHead,NCS_LNK_SYSINFO,sizeof(ncsSystemInfo),1);
    if(iReturn < 0) {
        pasSysLog(PAS_LOG_ERROR,"NCS_LNK_SYSINFO init error");
        return (-1);
    }        
    psInfo = (ncsSystemInfo *)utShmArray(psShmHead,NCS_LNK_SYSINFO);
    if(psInfo) {
        char caCom[128],caFile[128];
        memset(psInfo,0,sizeof(ncsSystemInfo));
        psInfo->lStartTime = psInfo->lLastTime = time(0);
        strcpy(psInfo->password,utComGetVar_sd(psShmHead,"pubkey","ProNetWay"));
        strcpy(psInfo->caHome,utComGetVar_sd(psShmHead,"MailDir","/home/ncmysql/ncs/data"));
        if(!utFileIsExist(psInfo->caHome)) {
            sprintf(caCom,"/bin/mkdir -p  %s",psInfo->caHome);
            system(caCom);
        }
        sprintf(caFile,"%s/%s",psInfo->caHome,"Download/Case");
        printf("caFile=%s\n",caFile);
        if(!utFileIsExist(caFile)) {
            sprintf(caCom,"/bin/mkdir -p  %s",caFile);
            system(caCom);
        }
        strcpy(psInfo->caMinVar,NCS_CLT_MINVERSION);
    }
    psInfo->lStartTime = time(0);
    psInfo->psShmHead = psShmHead;
    ncsLoadGroupInfo(psInfo);
    ncsLoadClientInfo(psInfo);
    utShmFreeHash(psShmHead,NCS_LNK_ONLINE);
    iReturn = utShmHashInit(psShmHead,NCS_LNK_ONLINE,psInfo->iSumClient+1,psInfo->iSumClient+1,
                            sizeof(ncsOnline),0,4);
    if(iReturn < 0) {
        pasSysLog(PAS_LOG_ERROR,"NCS_LNK_ONLINE init error");
        return (-1);
    }
    ncsUtlInitSynBuf(psShmHead);
    ncsUtlInitOnlineBuf(psShmHead);
    ncsUtlLoadCaseCtl(psInfo);
    ncsUtlSetLastMsgIdFromDb(psShmHead);
    if(strcasecmp(utComGetVar_sd(psShmHead,"SubAdmMgr","No"),"Yes")==0) {
        ncsSrvInit(psShmHead);
    }
    /*装载告警邮件地址*/
    ncsUtlLoadCaseWarnFromDb(psShmHead);   
    lLastTime = ncsLoadDownLoadCom(psShmHead);     
    psInfo->lComTime = lLastTime;    
    return 0;
}

/* 装载用户信息，返回当前用户的数量*/
int ncsLoadClientInfo(ncsSystemInfo *psStatus)
{
    int iReturn;
    utShmHead *psShmHead;
    pasDbCursor *psCur;
    unsigned long lUserid,lGroupid,lLastTime,lUseflags,lCount;
    char caUsername[32],caDispname[64],caPassword[32],caVersion[16];
    char caTime[32];
    ncsClient  *psClient;
    ncsGroup *psGroup;
    pasHashInfo sHashInfo;
    int i,iNew=0;
    long long llX,llY;
    unsigned long lLasttime,lUsertype;
    char *pHash;
    psShmHead = psStatus->psShmHead;
    utShmFreeArray(psShmHead,NCS_LNK_CLIENT);  
    iNew = pasDbFieldIsExist("ncsuser","longitude");
    iReturn = pasDbOneRecord("select count(*) from ncsuser",0,UT_TYPE_LONG,4,&lCount);
    if(iReturn != 0) {
        pasLog(PAS_LOG_ERROR,"ncsuser error");
        return (-1);
    }            
    if(lCount == 0) lCount = 1;
    iReturn = utShmInitArray(psShmHead,NCS_LNK_CLIENT,sizeof(ncsClient),lCount);
    if(iReturn != 0) {
        pasLog(PAS_LOG_ERROR,"NCS_LNK_CLIENT init Error");
        return (-1);
    }
    psClient = (ncsClient *)utShmArray(psShmHead,NCS_LNK_CLIENT);
    if(psClient == NULL) {
        pasLog(PAS_LOG_ERROR,"NCS_LNK_CLIENT memory error");
        return (-1);
    }

    memset(psClient,0,sizeof(ncsClient) * lCount);
    /* 将客户信息读入到内存  */
    if(iNew) {
        psCur = pasDbOpenSqlF("select userid,username,dispname,password,groupid,gtype,lasttime,useflags,version,latitude,longitude from ncsuser order by userid");
    }
    else {
        psCur = pasDbOpenSqlF("select userid,username,dispname,password,groupid,gtype,lasttime,useflags,version,0,0  from ncsuser order by userid");
    }
    if(psCur == NULL) {
        pasSysLog(PAS_LOG_ERROR,"Load client info error %s",pasDbErrorMsg(NULL));
        return (-1);
    }
    i = 0;
    iReturn = pasDbFetchInto(psCur,
      	                           UT_TYPE_LONG,  4,&lUserid,
  	                               UT_TYPE_STRING,31,caUsername,
  	                               UT_TYPE_STRING,63,caDispname,
  	                               UT_TYPE_STRING,31,caPassword,
  	                               UT_TYPE_LONG,  4, &lGroupid,
  	                               UT_TYPE_LONG,  4, &lUsertype,
  	                               UT_TYPE_STRING,16,caTime,
  	                               UT_TYPE_LONG,  4, &lUseflags,
  	                               UT_TYPE_STRING,11,caVersion,
  	                               UT_TYPE_LONG8,8,&llY,
  	                               UT_TYPE_LONG8,8,&llX);
  	                               
   	while((iReturn == 0 || iReturn == 1405 )&& i < lCount) {
   	    psClient[i].num = i;
        psClient[i].userid = lUserid;
        psClient[i].groupid = lGroupid;
        psClient[i].usertype = lUsertype;
        psGroup = (ncsGroup *)ncsUtlGetGroupById(psShmHead,lGroupid);
        if(psGroup) {
            psClient[i].gnum = psGroup->num;
        }            
        psClient[i].useflags = lUseflags;
        psClient[i].lLastTime = utTimStrToLong("%Y-%m-%d %H:%M",caTime);
        psClient[i].status = NCS_CLTSTA_OFFLINE;
        strcpy(psClient[i].username,caUsername);
        strcpy(psClient[i].dispname,caDispname);
        strcpy(psClient[i].password,caPassword);
        strcpy(psClient[i].caVer,caVersion);
        psClient[i].fx = (double ) llX / 100000000.00L;
        psClient[i].fy = (double ) llY / 100000000.00L;
        i++;
        iReturn = pasDbFetchInto(psCur,
      	                           UT_TYPE_LONG,  4,&lUserid,
  	                               UT_TYPE_STRING,31,caUsername,
  	                               UT_TYPE_STRING,63,caDispname,
  	                               UT_TYPE_STRING,31,caPassword,
  	                               UT_TYPE_LONG,  4, &lGroupid,
  	                               UT_TYPE_LONG,  4, &lUsertype,
  	                               UT_TYPE_STRING,16,caTime,
  	                               UT_TYPE_LONG,  4, &lUseflags,
  	                               UT_TYPE_STRING,11,caVersion,
  	                               UT_TYPE_LONG8,8,&llY,
  	                               UT_TYPE_LONG8,8,&llX);
    }
    pasDbCloseCursor(psCur);
    /* 在线用户同步  */
    psStatus->psClient = psClient;
    psStatus->iSumClient = i;
    return 0;
}




/* 装载告警邮件地址和初始化邮件告警记录 */
int ncsUtlLoadCaseWarnFromDb(utShmHead *psShmHead)
{
    int iReturn;
    long lHashSize,lMaxMail;
    pasDbCursor *psCur;
    unsigned long lCid,lWtype,lCount;
    char caWaddr[64];
    char caTime[32];
    ncsCaseMob  *psCaseMob;
  
    int i;
    unsigned long lLasttime,lUsertype;
    char *psHash;
 
    utShmFreeArray(psShmHead,NCS_LNK_CASEMOB);  
    iReturn = pasDbOneRecord("select count(*) from ncscasewarn",0,UT_TYPE_LONG,4,&lCount);
    if(iReturn != 0) {
        pasSysLog(PAS_LOG_ERROR,"open ncscasewarn error");
        return (-1);
    }            
    if(lCount == 0) lCount = 1;
    iReturn = utShmInitArray(psShmHead,NCS_LNK_CASEMOB,sizeof(ncsCaseWarn),lCount);
    if(iReturn != 0) {
        pasSysLog(PAS_LOG_ERROR,"NCS_LNK_CASEMOB init Error");
        return (-1);
    }
    psCaseMob = (ncsCaseMob *)utShmArray(psShmHead,NCS_LNK_CASEMOB);
    if(psCaseMob == NULL) {
        pasSysLog(PAS_LOG_ERROR,"NCS_LNK_CASEMOB memory error");
        return (-1);
    }

    memset(psCaseMob,0,sizeof(psCaseMob) * lCount);
    /* 将告警邮件地址读入到内存  */
    psCur = pasDbOpenSqlF("select cid,waddr,wtype from ncscasewarn order by cid ");
    if(psCur == NULL) {
        pasSysLog(PAS_LOG_ERROR,"Load ncscasewarn info error %s",pasDbErrorMsg(NULL));
        return (-1);
    }
    i = 0;
    lCid=0;
    memset(caWaddr,0,sizeof(caWaddr));
    lWtype=0;
    iReturn = pasDbFetchInto(psCur,
      	                           UT_TYPE_LONG,  4,&lCid,
  	                               UT_TYPE_STRING,63,caWaddr,
  	                               UT_TYPE_LONG,4,&lWtype);
  	                             
  	                               
   	while(iReturn == 0 && i < lCount) {
   	    psCaseMob[i].cid = lCid;
   	    psCaseMob[i].wtype = lWtype;
   	    strcpy(psCaseMob[i].waddr,caWaddr);
   	    
       i++;
    lCid=0;
    memset(caWaddr,0,sizeof(caWaddr));
    lWtype=0;
    iReturn = pasDbFetchInto(psCur,
      	                           UT_TYPE_LONG,  4,&lCid,
  	                               UT_TYPE_STRING,63,caWaddr,
  	                               UT_TYPE_LONG,4,&lWtype);
    }
    pasDbCloseCursor(psCur);
 
 
//初始化邮件告警
 lMaxMail = utComGetVar_ld(psShmHead,"MaxMail",10000);

 utShmFreeHash(psShmHead,NCS_LNK_MAILWARN);
 iReturn=utShmHashInit(psShmHead,NCS_LNK_MAILWARN,lMaxMail+1,lMaxMail+1,sizeof(ncsMailWarnRecord),0,4);

    if(iReturn < 0) {
        utSysLog(" Init Memory Error ncUserLoadUserInfo");
        return NULL;
    }
   
    return 0;
}


/* 检查在线用户，是否超时  */
int ncsUtlCheckOnlineClt(utShmHead *psShmHead)
{
    pasHashInfo sHashInfo;
    unsigned char *pHash;
    ncsOnline *psOnline,*psOnline0;
    ncsClient   *psClient;
    char  caDataIp[32];
    unsigned long lTimeOut;
    unsigned long lTime,lSip,lLastSynTime,lSumClient;
    unsigned short nPort;
    int iReturn,iStartServer,iSumClient=0,iCount,iSock;
    ncFSSClientStatus *psStatus=NULL;
    
    if(strcasecmp(utComGetVar_sd(psShmHead,"StartDataServer","No"),"Yes")==0) {
        iStartServer = 1;
        pasMsgSetMyInfo(1000,"ProEIm001");
        strcpy(caDataIp,utComGetVar_sd(psShmHead,"DataServerIp","127.0.0.1:8001"));
        utStr2IpPort(caDataIp,&lSip,&nPort);
    }
    else {
        iStartServer = 0;
    }
    
    iReturn = pasConnect(psShmHead);
    if(iReturn != 0) {
        pasLog(PAS_LOG_ERROR,"Connect Db ERror");
        sleep(60);
        return 0;
    }
    sleep(30);
//    printf("StartServer==%d \n",iStartServer);
    if(iStartServer) {
        lSumClient = utShmArrayRecord(psShmHead,NCS_LNK_CLIENT);
        psStatus = (ncFSSClientStatus *)malloc(lSumClient * sizeof(ncFSSClientStatus));
        if(psStatus == NULL) {
            printf(" Memory Alloc Error \n");
        }
    }
    lLastSynTime = 0;
    while(1) {
        lTimeOut = utComGetVar_ld(psShmHead,"NcTimeOut",180);
        lTime = time(0);
        psClient = (ncsClient *)utShmArray(psShmHead,NCS_LNK_CLIENT);
        if(psClient == NULL) {
            sleep(10);
            return 0;
        }
        pHash = (unsigned char *)utShmHashHead(psShmHead,NCS_LNK_ONLINE);
        if(pHash) {
            iCount = 0;
            psOnline  = (ncsOnline *)pasHashFirst(pHash,&sHashInfo);
            while(psOnline) {
                if(psClient[psOnline->num].cmod != 0) { /* 更新数据库中相关记录  */
                    iReturn = pasDbExecSqlF("update ncsuser set lasttime='%s',version='%s' where  userid = %lu",
                          utTimFormat("%Y-%m-%d %H:%M",psClient[psOnline->num].lLastTime),psClient[psOnline->num].caVer,psClient[psOnline->num].userid);
                    pasLogs(PAS_SRCFILE,1010,"[%d]update ncsuser set lasttime='%s',version='%s' where  userid = %lu",iReturn,
                          utTimFormat("%Y-%m-%d %H:%M",psClient[psOnline->num].lLastTime),psClient[psOnline->num].caVer,psClient[psOnline->num].userid);
                    psClient[psOnline->num].cmod = 0;
                }
                if(lTime > psOnline->lLastTime + lTimeOut) {
                    if(psStatus) {
                        psStatus[iCount].lDid = psOnline->userid;
                        strcpy(psStatus[iCount].caName,psClient[psOnline->num].username);
                        strcpy(psStatus[iCount].caDispName,psClient[psOnline->num].dispname);
                        strcpy(psStatus[iCount].caDoTime,"\0");
                        psStatus[iCount].lSip = psOnline->lSip;
                        ncFSSGenAuthCode(psOnline->userid,psOnline->caKey,psStatus[iCount].AuthCode);
                        psStatus[iCount].cStatus = 0;
                        psStatus[iCount].cKeep = 0;
                        psStatus[iCount].cDo = 1;
                        iCount ++;
                    }
                    psClient[psOnline->num].status = NCS_CLTSTA_OFFLINE;
                    psOnline0 = psOnline;
                    psOnline = (ncsOnline *)pasHashNextS(&sHashInfo);
                    ncsUtlClientLogout(psShmHead,psOnline0,"超时离线");  /* 用户离线  */
                }
                else {    /* 没有超时  */
                    /*
                    if(psOnline->lHeatRecTime < lTime - 60) {
                        if(psOnline->lHeatCount < 4 && psOnline->lLastTime < lTime - 20) {
                            psClient[psOnline->num].status = NCS_CLTSTA_ABNORMITY;
                            strcpy(psClient[psOnline->num].caMsg,"心跳不正常");
                        }
                        else {
                            strcpy(psClient[psOnline->num].caMsg,"\0");
                        }
                        psOnline->lHeatCount = 0;
                        psOnline->lHeatRecTime = lTime;
                    }
                    */
                    psClient[psOnline->num].status = NCS_CLTSTA_ONLINE;
                    if(psStatus) {
                        psStatus[iCount].lDid = psOnline->userid;
                        strcpy(psStatus[iCount].caName,psClient[psOnline->num].username);
                        strcpy(psStatus[iCount].caDispName,psClient[psOnline->num].dispname);
                        strcpy(psStatus[iCount].caDoTime,"\0");
                        psStatus[iCount].lSip = psOnline->lSip;
                        ncFSSGenAuthCode(psOnline->userid,psOnline->caKey,psStatus[iCount].AuthCode);
                        psStatus[iCount].cStatus = 1;
                        psStatus[iCount].cKeep = 0;
                        psStatus[iCount].cDo = 1;
                        iCount ++;
                    }
                    psOnline = (ncsOnline *)pasHashNextS(&sHashInfo);
                }
            }
            lTime = time(0);
            if(iCount > 0 && lTime > lLastSynTime + 120) {
                int iSock;
                iSock =   utTcpConnect0(lSip,nPort);
                if(iSock > 0) {
                    iReturn = ncFSCSynClientInfo(iSock,psStatus,iCount);
                    pasLogs(PAS_SRCFILE,1006,"Conneect Tcp Sucess %s:%lu iReturn = %d iCount=%d\n",
                         utComHostIp(lSip),ntohs(nPort),iReturn,iCount);
                    close(iSock);
                    lLastSynTime = time(0);
                }
                else {
                    pasLogs(PAS_SRCFILE,1006,"Conneect Tcp Error %s:%lu iCount=%d\n",
                         utComHostIp(lSip),ntohs(nPort),iCount);
                }
            }
        }
        sleep(10);
    }
    return 0;
}


/* 备份在线信息  */

ncsBackOnline *ncsUtlBackOnline(utShmHead *psShmHead)
{
    pasHashInfo sHashInfo;
    unsigned char *pHash;
    ncsOnline *psOnline;
    ncsBackOnline *psBack=NULL,*psBack0;
    pHash = (unsigned char *)utShmHashHead(psShmHead,NCS_LNK_ONLINE);
    if(pHash) {
        psOnline  = (ncsOnline *)pasHashFirst(pHash,&sHashInfo);
        while(psOnline) {
            if( (psBack0 = malloc(sizeof(ncsBackOnline))) == NULL) {
                pasLog(PAS_LOG_ERROR,"Memory Error");
                return NULL;
            }
            memcpy(&psBack0->sOnline,psOnline,sizeof(ncsOnline));
            psBack0->next = psBack;
            psBack = psBack0;            
            psOnline = (ncsOnline *)pasHashNextS(&sHashInfo);
        }
        return psBack;
    }
    return NULL;
}

/* 恢复在线信息   */
int ncsUtlRestoreOnline(utShmHead *psShmHead,ncsBackOnline *psBack0)
{
    ncsBackOnline *psBack;
    ncsClient *psClient;
    ncsGroup  *psGroup;
    ncsOnline *psOnline;
    ncsSystemInfo *psSysInfo;
    
    psSysInfo = (ncsSystemInfo *)utShmArray(psShmHead,NCS_LNK_SYSINFO);
    psGroup = (ncsGroup *)utShmArray(psShmHead,NCS_LNK_GROUP);
    
    if(psSysInfo && psGroup) {
        psBack = psBack0;
        while(psBack) {
            psClient = ncsUtlGetClientById(psShmHead,psBack->sOnline.userid);
            if(psClient) {
                psOnline = (ncsOnline *)utShmHashLookA(psShmHead,NCS_LNK_ONLINE,&psClient->userid);
                if(psOnline) {
                    memcpy(psOnline,&psBack->sOnline,sizeof(ncsOnline));
                    psOnline->num = psClient->num;
//                    strcpy(psClient->caVer,psSysInfo->caMinVar);
                    psClient->status = NCS_CLTSTA_ONLINE;

                    /*
                    if(strcasecmp(psClient->caVer,psSysInfo->caMinVar) < 0) {
                        psClient->status = NCS_CLTSTA_ABNORMITY;                        
                        strcpy(psClient->caMsg,"客户端版本太低");
                    }
                    else {
                    }
                    */
                    psOnline->lLastTime = time(0);
                    psOnline->lSynCheck = ntohl(psClient->syninfo.allchsum) +
                            ntohl(psGroup[psClient->gnum].syninfo.allchsum) +
                            ntohl(psSysInfo->syncomm.allchsum) +
                            ntohl(psSysInfo->synall.allchsum);
                }
            }
            psBack = psBack->next;
        }
    }
    psBack = psBack0;
    while(psBack) {
        psBack0 = psBack->next;
        free(psBack);
        psBack = psBack0;
    }
    return 0;
}



/* 做用户离线的工作  */
int ncsUtlClientLogout(utShmHead *psShmHead,ncsOnline *psOnline,char *pMsg)
{
    ncsUtlClientLog(NCS_CLTLOG_LOGOUT,psOnline->userid,psOnline->lSip,pMsg);
    utShmHashDel(psShmHead,NCS_LNK_ONLINE,psOnline);
    return 0;
}

/* 客户端登录  */
ncsOnline *ncsUtlDoLogin(utShmHead *psShmHead,ncsClient *psClient,
                               unsigned long lIp,unsigned short nPort,int iFlags)
{
    unsigned long lTime;
    ncsOnline *psOnline;
    char caKey[32];
    ncsSystemInfo *psSysInfo;
    ncsGroup *psGroup;
    lTime = time(0);
    psOnline = (ncsOnline *)utShmHashLookA(psShmHead,NCS_LNK_ONLINE,&psClient->userid);
    if(psOnline == NULL) {
        pasLog(PAS_LOG_ERROR,"NCS_LNK_ONLINE, Max user limit");
        return NULL;
    }
    if(psOnline->lLastTime > 0) {
        if(psOnline->cflags < iFlags) {
            return psOnline;    /* 已登陆  */
        }
    }
    psClient->status = NCS_CLTSTA_ONLINE;
    psClient->errcount = 0;
    psClient->lLastTime = lTime;
    psOnline->num = psClient->num;
    psOnline->lSip = lIp;
    psOnline->nPort = nPort;
    if(psOnline->lStartTime != 0) psOnline->lStartTime = lTime;
    psOnline->lLastTime = lTime;
    psOnline->cflags = iFlags;
    psOnline->psClient = psClient;
    psGroup = (ncsGroup *)utShmArray(psShmHead,NCS_LNK_GROUP);
    psSysInfo = (ncsSystemInfo *)utShmArray(psShmHead,NCS_LNK_SYSINFO);
    psOnline->lSynCheck = ntohl(psClient->syninfo.allchsum) +
                          ntohl(psGroup[psClient->gnum].syninfo.allchsum) +
                          ntohl(psSysInfo->syncomm.allchsum) +
                          ntohl(psSysInfo->synall.allchsum);
    pasLogs(PAS_SRCFILE,1001,"[ncsUtlDoLogin]CheckSum=%lu   %lu + %lu + %lu + %lu \n",
                        psOnline->lSynCheck,ntohl(psClient->syninfo.allchsum),
                        ntohl(psGroup[psClient->gnum].syninfo.allchsum),
                        ntohl(psSysInfo->syncomm.allchsum),
                        ntohl(psSysInfo->synall.allchsum));
    sprintf(caKey,"%s%lu",psClient->username,lTime);
    utMd5Ascii22(caKey,strlen(caKey),NULL,psOnline->caKey);
    ncsUtlClientLog(NCS_CLTLOG_LOGIN,psClient->userid,psOnline->lSip,"Client %s login success",psClient->username);
    return psOnline;
}


/* 装载部门信息  */
int ncsLoadGroupInfo(ncsSystemInfo *psStatus)
{
    utShmHead *psShmHead;
    pasDbCursor *psCur;
    ncsGroup    *psGroup;
    unsigned long lGroupid;
    char caGroupname[32];
    int i,iReturn;
    unsigned long lCount;
    psShmHead = psStatus->psShmHead;
    iReturn = pasDbOneRecord("select count(*) from ncsgroup",0,UT_TYPE_LONG,4,&lCount);
    if(iReturn != 0) {
        pasSysLog(PAS_LOG_ERROR,"ncsgroup error");
        return (-1);
    }
    utShmFreeArray(psShmHead,NCS_LNK_GROUP);  
    if(lCount == 0) lCount = 1;
    iReturn = utShmInitArray(psShmHead,NCS_LNK_GROUP,sizeof(ncsGroup),lCount);
    if(iReturn != 0) {
        pasSysLog(PAS_LOG_ERROR,"NCS_LNK_GROUP init Error");
        return (-1);
    }
    psGroup = (ncsGroup *)utShmArray(psShmHead,NCS_LNK_GROUP);
    if(psGroup == NULL) {
        pasSysLog(PAS_LOG_ERROR,"NCS_LNK_GROUP memory error");
        return (-1);
    }

    memset(psGroup,0,sizeof(ncsGroup) * lCount);
    /* 将客户信息读入到内存  */
    psCur = pasDbOpenSqlF("select groupid,groupname from ncsgroup order by groupid");
    if(psCur == NULL) {
        pasSysLog(PAS_LOG_ERROR,"Load ncsgroup info error %s",pasDbErrorMsg(NULL));
        return (-1);
    }
    i = 0;
    iReturn = pasDbFetchInto(psCur,
      	                           UT_TYPE_LONG,  4,&lGroupid,
  	                               UT_TYPE_STRING,31,caGroupname);
   	while(iReturn == 0 && i < lCount) {
   	    psGroup[i].groupid = lGroupid;
   	    psGroup[i].num = i;
        strcpy(psGroup[i].groupname,caGroupname);
        i++;
        iReturn = pasDbFetchInto(psCur,
      	                           UT_TYPE_LONG,  4,&lGroupid,
  	                               UT_TYPE_STRING,31,caGroupname);
    }
    pasDbCloseCursor(psCur);
    psStatus->psGroup = psGroup;
    psStatus->iSumGroup = i;
    /* 在线用户同步  */
    return 0;
}



/* 检查在线用户，是否超时  */
int ncsUtShowOnlineUser(utShmHead *psShmHead)
{
    pasHashInfo sHashInfo;
    unsigned char *pHash;
    ncsOnline *psOnline;
    ncsClient   *psClient;
    int  iCount;
    psClient = (ncsClient *)utShmArray(psShmHead,NCS_LNK_CLIENT);
    if(psClient == NULL) {
        printf(" Online user Does not exist \n");
        return 0;
    }
    pHash = (unsigned char *)utShmHashHead(psShmHead,NCS_LNK_ONLINE);
    if(pHash) {
        iCount = 0;
        psOnline  = (ncsOnline *)pasHashFirst(pHash,&sHashInfo);
        while(psOnline) {
            printf("%3d Id:%6lu Name:[%15s] Disp:[%15s] Key:[%s] Ip:%15s User:%3d  Tcp:%6d  LastTime:%s  Flags:%d \n",
                  iCount+1,
                  psOnline->userid,
                  psClient[psOnline->num].username,
                  psClient[psOnline->num].dispname,
                  psOnline->caKey,
                  utComHostIp(htonl(psOnline->lSip)),
                  psOnline->lUser,
                  psOnline->lTcp,
                  utTimFormat("%Y-%m-%d %H:%M:%S",psOnline->lLastTime),
                  psOnline->cflags);
             psOnline = (ncsOnline *)pasHashNextS(&sHashInfo);
             iCount++;
        }
    }
    return 0;
}


/* 装载下发命令    */
int ncsLoadDownLoadCom(utShmHead *psShmHead)
{
    ncsDownloadCom *psCom;
    ncsDownloadComHead *psHead;
    char           *pBuf,*p;
    int iReturn;
    pasDbCursor *psCur;
    unsigned long lId,lCount=0,lAddTime,lExpireTime,lType,lMax,lPos=0;
    char caName[64],caCom[2048];
    int i;
    unsigned long lLastTime=0,lTime,lSize;
    lTime = time(0);

    
    iReturn = pasDbOneRecord("select count(*) from ncsdowncom where flags = 1 and (expiretime =0 or expiretime > :exp)",1,
                                "exp",  UT_TYPE_ULONG,  lTime,
                                UT_TYPE_LONG,4,&lCount);
    if(iReturn != 0) {                                 
        pasLog(PAS_LOG_ERROR,"ncsdowncom error %s",pasDbErrorMsg(NULL));
        return (-1);
    }
    if(lCount > 0) {
        psCom = (ncsDownloadCom *)malloc(sizeof(ncsDownloadCom) * lCount);
        if(psCom == NULL) {
            pasLog(PAS_LOG_ERROR,"Memory Error ");
            return (-1);
        }
        lMax = lCount * 2048;
        pBuf = malloc(lMax);
        if(pBuf == NULL) {
            free(psCom);
            pasLog(PAS_LOG_ERROR,"Memory Error ");
            return (-1);
        }
        lPos = 0;
        psCur = pasDbOpenSqlF("select gtype,id,name,commstr,addtime,expiretime from ncsdowncom where flags = 1 and (expiretime =0 or expiretime > %lu) order by addtime desc",lTime);
        if(psCur == NULL) {
            free(pBuf);
            free(psCom);
            pasLog(PAS_LOG_ERROR,"Load ncsdowncom  error %s",pasDbErrorMsg(NULL));
            return (-1);
        }
        i = 0;
        iReturn = pasDbFetchInto(psCur,
      	                           UT_TYPE_ULONG,  4,&lType,
      	                           UT_TYPE_ULONG,  4,&lId,
  	                               UT_TYPE_STRING,63,caName,
  	                               UT_TYPE_STRING,2047,caCom,
  	                               UT_TYPE_ULONG,  4,&lAddTime,
  	                               UT_TYPE_ULONG,  4,&lExpireTime);
   	    while(iReturn == 0 && i < lCount) {
   	        if(!utStrIsSpaces(caCom)) {
                if(lLastTime < lAddTime) {
                    lLastTime = lAddTime;
                }
       	        psCom[i].cGtype = lType;
   	            psCom[i].lId = lId;
   	            psCom[i].lPos = lPos;
   	            psCom[i].lBytes = strlen(caCom)+1;
   	            psCom[i].lTime = lAddTime;
   	            psCom[i].lExpireTime = lExpireTime;
   	            memcpy(pBuf+lPos,caCom,psCom[i].lBytes);
   	            lPos += caCom,psCom[i].lBytes;
   	            i++;
   	        }
            iReturn = pasDbFetchInto(psCur,
      	                           UT_TYPE_ULONG,  4,&lType,
      	                           UT_TYPE_ULONG,  4,&lId,
  	                               UT_TYPE_STRING,63,caName,
  	                               UT_TYPE_STRING,2047,caCom,
  	                               UT_TYPE_ULONG,  4,&lAddTime,
  	                               UT_TYPE_ULONG,  4,&lExpireTime);
   	    
   	    }
   	    lCount = i;
   	    pasDbCloseCursor(psCur);
   	}
    lSize = sizeof(ncsDownloadComHead) + lCount*sizeof(ncsDownloadCom) + lPos;
    utShmFreeArray(psShmHead,NCS_LNK_DOWNCOM);  
    iReturn = utShmInitArray(psShmHead,NCS_LNK_DOWNCOM,lSize,1);
    if(iReturn != 0) {
        pasLog(PAS_LOG_ERROR,"NCS_LNK_DOWNCOM init Error");
        if(lCount > 0) {
            free(psCom);
            free(pBuf);
        }
        return (-1);
    }
    p = (char *)utShmArray(psShmHead,NCS_LNK_DOWNCOM);
    if(p == NULL) {
        pasLog(PAS_LOG_ERROR,"NCS_LNK_DOWNCOM memory error");
        if(lCount > 0) {
            free(psCom);
            free(pBuf);
        }
        return (-1);
    }
    memset(p,0,lSize);
    psHead = (ncsDownloadComHead *)p;
    psHead->lSumBytes = lSize;
    psHead->lLoadTime = lTime;
    psHead->lLastTime = lLastTime;
    psHead->lSumCom = lCount;
    if(lCount > 0) {
        memcpy(p+sizeof(ncsDownloadComHead),psCom,lCount*sizeof(ncsDownloadCom));
        memcpy(p+sizeof(ncsDownloadComHead)+lCount*sizeof(ncsDownloadCom),pBuf,lPos);
        free(psCom);
        free(pBuf);
    }
    psHead->psCom = (ncsDownloadCom *) (p+sizeof(ncsDownloadComHead));
    psHead->pBase = p + sizeof(ncsDownloadComHead) + lCount * sizeof(ncsDownloadCom);
    
    return lLastTime;
}


/* 自动清除临时文件
 */
int ncsNcsAutoCleanTemp(utShmHead *psShmHead)
{
    char caPath[128],caKeep[128];
    unsigned long lStartTime;
    char *p;
    int iFlags;
    unsigned long lTime0;
    unsigned long lTime1;
    while(1) {
        lTime0 = time(0);
        if(strcasecmp(utComGetVar_sd(psShmHead,"XFssKeepSend","No"),"Yes")==0) {
            iFlags = 1;
        }
        else {
            iFlags = 0;
        } 
        p = utComGetVar_s(psShmHead,"XLogPath");
        if(p) {
            sprintf(caPath,"%s",p);
            ncUtlImpCheckPath1(caPath);
            lStartTime = utComGetVar_ld(psShmHead,"XLogKeepTime",30);
            lStartTime = time(0) - lStartTime * 86400L;
            utFilDeleteDir_1(caPath,0,lStartTime);
        }
        if(iFlags == 1) {
            lStartTime = utComGetVar_ld(psShmHead,"XKeepTime",7);
            lStartTime = time(0) - lStartTime * 86400L;
            utFilDeleteDir_1(caKeep,0,lStartTime);
            p = utComGetVar_s(psShmHead,"XLogPathKeep");
            if(p) {
                sprintf(caKeep,"%s",p);
                lStartTime = utComGetVar_ld(psShmHead,"XKeepTime",7);
                lStartTime = time(0) - lStartTime * 86400L;
                utFilDeleteDir_1(caKeep,0,lStartTime);
            }
        }
        lTime1 = time(0);
        if(lTime1 < lTime0 + 3600) {
            sleep(lTime0 + 3600 - lTime1);
        }
    }
    return 0;
}
