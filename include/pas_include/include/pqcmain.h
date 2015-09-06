#ifndef __PQCSMAINDEF____
#define __PQCSMAINDEF____
#include "utoall.h"
#include "pqmain.h"
/* pqcm000.c */
#define PQC_MAXTCPFUN       50
#define PQC_MAXUDPFUN       50
#define PQC_LNK_MYSTATUS    11

#define PQC_STA_OFFLINE     0
#define PQC_STA_ONLINE      1

#define PQC_MSG_NORMAL      0
#define PQC_MSG_URGENCY     1
#define PQC_MSG_PAGES       2

#define PQC_AUTH_ERROFFLINE         531101   /* 联机服务器处于离线状态  */
#define PQC_AUTH_ERRTCPSOCK         531102   /* TCP Sock创建失败        */
#define PQC_AUTH_ERRCONNECT         531103   /* Tcp连接出错             */
#define PQC_AUTH_ERRSEND            531104   /* Tcp请求出错             */
#define PQC_AUTH_ERRRECEIVE         531105   /* Tcp消息接收出错         */

#define PQC_RENUM_ERRSEND           532101   /* 数据包发送出错 */

typedef struct 
{
    long   lPicqNum;             /* Picq Number */
    char   caUserName[16];       /* UserName    */
    char   caPassword[20];       /* PassWord    */
    short  nUdpPort;             
    short  nTcpPort;
    long   lMcomTime;
    long   lMuserTime;
    long   lLoginTime;
    long   lLastComTime;
    long   lServerAddress;
    long   lAutoLoginTime;
    long   lAutoStatusTime;
    long   lTimeOut;
    short  nServerTcpPort;
    short  nServerUdpPort;
    short  nNetWork;   /* 0 直接  >0 Proxy */
    short  nNetType;   /* 0 163   1  169  */
    short  nProxyPort;
    short  nStatus;              /* 用户状态     */
    long   lProxyAddress;
    char   caPublicKey[24];   
    char   caPrivKey[24];
    char   caComKey[24];
    long   lBaseTime;            /* 基准时间偏差 */
}pqcMyStatus;


typedef struct pqcUserInfo_S {
        long      picqnum;          /* Picq号码   */
        char      username[17];     /* 用户名称   */
        char      email[65];        /* E-Mail地址 */
        char      realname[21];     /* 用户全称   */
        char      micqnum[13];      /* 以色列Picq号 */
        long      power;            /* 用户权限     */
        long      userkind;         /* 用户类型     */
        long      curstatus;        /* 当前状态 0--离线  1--在线 */
        char      country[13];      /* 国家 */
        char      province[13];     /* 省   */
        char      city[13];         /* 市   */
        char      company[41];      /* 单位 */
        char      depart[21];       /* 部门 */
        char      occu[13];         /* 职业 */
        char      zipcode[7];       /* 邮编 */
        char      address[65];      /* 地址 */
        char      tele[21];         /* 电话 */
        char      fax[21];          /* 传真 */
        char      bpcall[21];       /* Bp机 */
        char      memo[65];         /* 说明 */
        char      homeurl[65];      
        long      secrecy;
        char      birthday[11];
        long      gender;
        char      htele[21];
        char      mtele[21];
        char      hzipcode[7];
        char      hcountry[13];
        char      hprovince[13];
        char      hcity[13];
        char      haddress[65];
        char      phomeurl[65];
        char      educate[9];
        char      school[65];
        char      gradudate[11];
        char      special[33];
        char      class[17];
        long      hsecrecy;
} pqcUserInfo;


/* 查找用户信息  */
typedef struct {
    long  lPicqNum;        /* 按网络字节顺序存放   */
	char  caMicqNum[12];    
	char  caUserName[16];
    char  caRealName[20];
    char  caEmail[64];
    long  lStatus;          /* 按网络字节顺序存放   */
} pqcFuserInfo;    


typedef struct {
    long lPicqNum;   /* PicqNumber */
    long lStatus;    /* 0--Offline   1---Online */
} pqcUserStatus;

/* 函数编码  
    pqcm0**   ------ 功能处理函数
    pqcm1**   ------  */
/* pqcm000.c */
int pqcInit000(utShmHead *psShmHead);
int pqcEnd000(utShmHead *psShmHead);
int pqcComTcpInit(utShmHead *psShmHead);
int pqcComUdpInit(utShmHead *psShmHead);

/* pqcm002.c  */
int pqcFunLogin(utShmHead *psShmHead, 
            utComSockAddr *psSockAddr,utMsgHead *psMsgHead);
int pqcFunStatus(utShmHead *psShmHead, 
            utComSockAddr *psSockAddr,utMsgHead *psMsgHead);
int pqcFunVerifyNet(utShmHead *psShmHead, 
            utComSockAddr *psSockAddr,utMsgHead *psMsgHead);
            
/* pqcm003.c */
int pqcComAutoLogin(utShmHead *psShmHead);
int pqComAutoStatus(utShmHead *psShmHead);
long pqcTime(utShmHead *psShmHead);
int pqcComTestServer(utShmHead *psShmHead);

/* pqcm101.c  */
int pqcFunAddContract(utShmHead *psShmHead, 
            utComSockAddr *psSockAddr,utMsgHead *psMsgHead);

            
/* pqcm301.c */
int pqcUtlAuth(utShmHead *psShmHead,
        long lPicqNum,char *pPassWord,long *lpPower,long *lpUserKind);
        
int pqcUtlSynLogin(utShmHead *psShmHead,long lPicqNum);
int pqcUtlGetUsrInfo(utShmHead *psShmHead,
        pqcUserInfo *psUserInfo,long lPicqNum,long lFlags,long lTime);

/* pqcm302.c */
int pqcUtlReqNumber(utShmHead *psShmHead,long lNumber,
    long *lRNumber,long *lMsgId);
int pqcUtlReNumber(utShmHead *psShmHead,long lNumber,
    long *lRNumber,long *lMsgId);
int pqcUtlCancelNumber(utShmHead *psShmHead,long lMsgId);
int pqcUtlRegister(utShmHead *psShmHead,long lMsgId,
    char *pPassword,pqcUserInfo *psUserInfo);    
int pqcUtlPageSend(utShmHead *psShmHead,long lPicqNum,
    int iMsgType,char *pName,char *pEEmail,char *pMessage);
int pqcUtlFindUser(utShmHead *psShmHead,long lPicqNum,
        char *pMicqNum, char *pUserName, char *pRealName,
        char *pEMail, long lStatus,
        long  lStart,
        long  lNumber,
        pqcFuserInfo *psUserInfo);
int pqcUtlReqOnline(utShmHead *psShmHead,long lPicqNum);
int pqcUtlSetRegisterKey(long lKey);
/* pqcm401.c */
int pqcComAddContract(utShmHead *psShmHead,long lPicqNum,long lFlags);

#endif

