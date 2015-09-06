#ifndef  ___DSCLT1_DEF___
#define  ___DSCLT1_DEF___
#define DS_DEBUG_LOGIN           2001
#define DS_CLT_ONLINE           1
#define DS_CLT_OFFLINE          0

#define DS_CLTTYPE_WEB          3
#define DS_CLTTYPE_PAS          2
#define DS_CLT_USERADD_FLISTERR  -10002         /* 用户管理，字段列表问题   */

#define DS_CLT_SHMERR            -10001         /* 工享内存出错   */
#define DS_CLT_DBERROR           -20001         /* 数据库出错     */
#define DS_CLT_DSERROR           -30001         /* 目录服务没启动 */
#define DS_CLT_ALREADYEXIST      -40001         /* 组已存在       */

#define DS_USRLOGIN_ALREADYLOGIN     1
#define DS_USRLOGIN_NOEXIST         -1
#define DS_USRLOGIN_PASSERR         -2
#define DS_USRLOGIN_OTHERERR        -9

typedef  struct {
    unsigned long lId;           /* 客户端ID               */
    unsigned long lGroupid;      /* 组号                   */
    char     caName[32];         /* 服务器名称             */
    char     caKey[32];          /* 用Md5加密              */
    char     caDispname[32];     /* 显示的名称             */
    char     caSid[32];          /* 验证的Key              */
    unsigned long  lLoginTime;   /* 登陆时间               */
    unsigned long  lLastTime;    /* 最后一次操作时间       */
    unsigned long  lDssrvIp;     /* 目录服务器的IP地址        */
    unsigned long  lOnlineNum;   /* 在目录服务器中的序号      */
    short    nStatus;            /* 0--不在线  1--在线(主) 2--在线(备)  -1 登陆出错   */
    unsigned short nKeepTime;    /* 保留时间               */
    unsigned short nStatusTime;  /* 状态发送时间           */
    unsigned nPort;              /* 服务器端的端口号       */
    unsigned short nSumClients;  /* 当前客户总数           */
    unsigned char  cAuthflags;   /* 验证方式 0--本地 1--远程  */
    unsigned char  cCurip;       /* 当前验证服务器 0--主  1--备 */
} dsCltStatus;

typedef  struct {  /* 在线应用  */
    unsigned long lId;           /* 客户端ID 0--初试值 1--已经删除
                                  */
    char     caGroup[32];        /* 组名                   */
    char     caName[32];         /* 名称                   */
    char     caSid[24];          /* 验证的Key              */
    unsigned long  lIp;          /* IP地址                 */
    unsigned short nTcpPort;     /* Tcp端口号              */
    unsigned short nUdpPort;     /* UDP端口号              */
    unsigned short nCltType;     /* 客户端类型 1--Ds服务器 2--PAS用户  3--Web用户 */
    unsigned short nStatus;      /* 0--不在线  1--在线     */
    unsigned short nLevel;       /* 用户级别               */
    unsigned short nTimeOut;     /* 超时时间               */
    unsigned long  lOnlineNum;   /* 在目录服务器上的位置   */
    unsigned long  lLoginTime;   /* 登陆时间               */
    unsigned long  lLastTime;    /* 最后一次操作时间       */
} dsClientInfo;

typedef struct dsSessionType_s { /* Session Info */
    char            caVarName[16];  /* 变量名     */
    unsigned char   cType;          /* 数据类型   */
    unsigned char   cFlags;         /* 数据来源 0--本地 1--目录服务器 */
    unsigned short  nBytes;         /* 字节数                         */
    struct dsSessionType_s  *psNext;
} dsSessionType;



/* dsclt001.c */
int dsCltInit(utShmHead *psShmHead);
int dsCltAutoTest(char *pHead);
int dsCltUdpInit(utShmHead *psShmHead);
dsUtlConf *dsCltCurConf();
int dsCltAppLogin(utShmHead *psShmHead,
            utComSockAddr *psSockAddr,utMsgHead *psMsgHead);
int dsCltAppStatus(utShmHead *psShmHead,
            utComSockAddr *psSockAddr,utMsgHead *psMsgHead);
long dsCltGetAppAttr_l(char *pAttrName,long lDefault);
char *dsCltGetAppAttr_s(char *pAttrName,char *pDefault);
int dsUtlSetRightValue(char *pRightValue,char *pValue);
int dsCltSyncRight(unsigned long lRighttime);
/* dsclt004_1.c */
int dsCltDbGroupAdd(char *caName,char *caDispname,
                    int *iStatus,long *lGid);
int dsCltDbGroupList(int iCountMk,long lStart,long lSum,
               char *caOrder,char *caSelect,char *caWhere,long *lRsum,long *lTotal);
int dsCltDbGroupModi(long lId,char *caName,char *caDispname);
int dsCltDbGroupDel(long lId);
int dsCltUpdateRight(long lId,dsUtlAttr *psRight);
/* dsclt006.c */
int dsCltServerAdd(unsigned long *lId,int iSum,...);
int dsCltServerAddF(unsigned long *lId,char *pFlist,char *pValue);
int dsCltServerModi(unsigned long lId,int iSum,...);
int dsCltServerModiF(long lId,char *pFlist,char *pValue);
int dsCltServerList(int iStart,int iSum,int *iReturnSum,int *iTotal,
                   char *pFlist,char *pWhere,char *pOrder,char *pResult);
int dsCltServerInfo(unsigned long lGid,char *pFlist,char *pResult);
int dsCltServerDel(long lGid);
int dsCltWebServerList(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int dsCltWebServerForm(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int dsCltWebServerModi(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int dsCltWebServerDel(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
/* dsweb001.c */
int dsCltWebMyStatus(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
/* dsclt002.c */

char *dsCltGetPasId();
int dsCltSetPasId(char *p);
int dsCltSetDsConfig(dsUtlConf *ps);
dsUtlConf *dsCltGetDsConfig();
int dsCltSetSessionType(char *pVarname,
                        int iType,
                        int iLen,
                        int iFlags);
int dsCltSetMySessionNumber(int iNum);
int dsCltGetMySessionNumber();
int dsCltUtlSessionOffset(char *pVar,int *iSize);
int dsCltUtlCvtSessionFlist(char *fList);
int dsCltSetSessionDsValue_a(char *pValue);
int dsCltUtlSessionSize();
int dsCltSetSessionValue(int iSum,...);
int dsCltGetSessionValue(int iSum,...);
int dsCltUserLogin(char *pUserName,
                   char *pPassword,
                   char *pIp);
char *dsCltGenSid();
dsClientInfo *dsCltGetSpacesClientInfo(utShmHead *psShmHead,
                                       unsigned long lId,
                                       int *iNum);
int dsCltCheckClient(char *pHead);
dsClientInfo *dsCltGetClientInfoBySid(utShmHead *psShmHead,char *pSid,int *iNum);
int dsCltDelClientInfoByNum(utShmHead *psShmHead,int iNum);
int dsCltDelClientInfoBySid(utShmHead *psShmHead,char *pSid);
int dsCltCvtCookiesSidNum(char *pValue,int *iPasid,int *iNum1,int *iNum2,char *pSid);
dsClientInfo *dsCltCheckClientInfo(utShmHead *psShmHead,
                                         int iNum,
                                         char *pSid,
                                         unsigned long lIp,
                                         int *iRet);
int dsCltClientIsLogin(utShmHead *psShmHead,utMsgHead *psMsgHead);
dsClientInfo *dsCltGetClientInfoByName(utShmHead *psShmHead,char *pName,
            unsigned long lIp,int *iNum);
int pasCheckMyRight(char *pFunName);
int dsCltGetMyInfo(int iSum,...);
long dsCltGetSeque();
/* dsclt004.c */
int dsCltGroupAdd(char *pName,char *pDispname, unsigned long *lGid);
int dsCltGroupList(int iStart,int iSum,int *iReturnSum,int *iTotal,
                   char *pFlist,char *pWhere,char *pOrder,char *pResult);
int dsCltGroupInfo(unsigned long lGid,char *pFlist,char *pResult);
int dsCltGroupModi(long lGid,char *pName,char *pDispname);
int dsCltGroupDel(long lGid);
int dsCltWebGroupList(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int dsCltWebGroupForm(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int dsCltWebGroupModi(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int dsCltWebGroupDel(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
/* dsclt007.c */
int dsCltRecordAdd(char *pDsname,unsigned long *lId,int iSum,...);
int dsCltRecordAddF(char *pDsname,unsigned long *lId,char *pFlist,char *pValue);
int dsCltRecordModi(char *pDsname,unsigned long lId,int iSum,...);
int dsCltRecordModiF(char *pDsname,long lId,char *pFlist,char *pValue);
int dsCltRecordList(char *pDsname,int iStart,int iSum,int *iReturnSum,int *iTotal,
                   char *pFlist,char *pWhere,char *pOrder,char *pResult);
int dsCltRecordInfo(char *pDsname,unsigned long lGid,char *pFlist,char *pResult);
int dsCltRecordDel(char *pDsname,long lGid);
int dsCltRecordDelSome(char *pDsname,char *pWhere);
int dsCltRecordAddSome(char *pDsname,char *pFlist,int iSum,char *pFvalue);
/* dsclt008.c */
int dsCltSqlRecordSet(char *pSmt,int iStart,int iSum,int *iReturnSum,char *pResult);
int dsCltSqlRecordSetCount(char *pCount,char *pSmt,int iStart,int iSum,int *iReturnSum,int *iRetCount,char *pResult);
int dsCltSqlExec(char *pSmt);
int dsCltSqlExecSome(char *pSmt);
/* dsutl001.c */
int dsUtlLoadConfig(char *pConfFile,dsUtlConf *psConf);
int dsUtlReadOneAttr(FILE *fp,dsUtlConf *psConf);
dsUtlAttr *dsUtlReadFunList(FILE *fp,dsUtlAttr *psAttr,char *pName);
dsUtlAttr *dsUtlReadRightList(FILE *fp,dsUtlAttr *psAttr,char *pName);
dsUtlAttr *dsUtlReadApp(FILE *fp,dsUtlAttr *psAttr,char *pName);
dsUtlAttrList *dsUtlAttrAppend(dsUtlAttrList *psFun,char *pName,int iType,int iBytes,char *pValue);
dsUtlAttrList *dsUtlGetAttrByName(dsUtlAttrList *psFun,char *caName);
dsUtlAttrList *dsUtlGetAttrByCode(dsUtlAttrList *psFun,unsigned long lCode);
int dsUtlPrintAttr(dsUtlAttr *psAttr);
dsUtlAttr *dsUtlGetAttrInfoByName(dsUtlAttr *psAttr,char *pName);
/* dsweb002.c */
int dsCltWebDsroleList(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int dsCltWebDsroleForm(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int dsCltWebDsroleModi(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int dsCltWebDsroleDel(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int dsCltWebDispDsright(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int dsCltWebRightSave(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int dsCltWebDsRoleList_main(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int dsCltWebDsPasRoleList(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int dsCltWebDsPasRoleAdd(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int dsCltWebDsPasRoleDel(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int dsCltWebDispUsrRole(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int dsCltWebUserRoleSave(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
/* dsclt003.c */
int dsCltWebDictList(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int dsCltWebDictForm(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int dsCltWebDictModi(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int dsCltWebDictDel(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
char *dsUtlGetDtype(int iType);
char *dsUtlGetFtype(int iType);
char *dsUtlGerRname(char *cR);
char *dsUtlGetUsrLevel(long lUsrlevel);
char *dsUtlGetCltType(int iType);
/* dsclt005.c */
int dsCltUserAdd(unsigned long *lId,int iSum,...);
int dsCltUserAddF(unsigned long *lId,char *pFlist,char *pValue);
int dsCltUserModi(unsigned long lId,int iSum,...);
int dsCltUserModiF(long lId,char *pFlist,char *pValue);
int dsCltUserList(int iStart,int iSum,int *iReturnSum,int *iTotal,
                   char *pFlist,char *pWhere,char *pOrder,char *pResult);
int dsCltUserInfo(unsigned long lGid,char *pFlist,char *pResult);
int dsCltUserDel(long lGid);
int dsCltWebUserList(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int dsCltWebUserForm(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int dsCltWebUserModi(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int dsCltWebUserDel(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
/* dsclt008.c */
int dsCltSqlRecordSet(char *pSmt,int iStart,int iSum,int *iReturnSum,char *pResult);
int dsCltSqlRecordSetCount(char *pCount,char *pSmt,int iStart,int iSum,int *iReturnSum,int *iRetCount,char *pResult);
int dsCltSqlExec(char *pSmt);
int dsCltSqlExecSome(char *pSmt);
/* dsclt002.c */
char *dsCltGetPasId();
int dsCltSetPasId(char *p);
int dsCltSetDsConfig(dsUtlConf *ps);
dsUtlConf *dsCltGetDsConfig();
int dsCltSetSessionType(char *pVarname,
                        int iType,
                        int iLen,
                        int iFlags);
int dsCltSetMySessionNumber(int iNum);
int dsCltGetMySessionNumber();
int dsCltUtlSessionOffset(char *pVar,int *iSize);
int dsCltUtlCvtSessionFlist(char *fList);
int dsCltSetSessionDsValue_a(char *pValue);
int dsCltUtlSessionSize();
int dsCltSetSessionValue(int iSum,...);
int dsCltGetSessionValue(int iSum,...);
int dsCltUserLogin(char *pUserName,
                   char *pPassword,
                   char *pIp);
char *dsCltGenSid();
dsClientInfo *dsCltGetSpacesClientInfo(utShmHead *psShmHead,
                                       unsigned long lId,
                                       int *iNum);
int dsCltCheckClient(char *pHead);
dsClientInfo *dsCltGetClientInfoBySid(utShmHead *psShmHead,char *pSid,int *iNum);
int dsCltDelClientInfoByNum(utShmHead *psShmHead,int iNum);
int dsCltDelClientInfoBySid(utShmHead *psShmHead,char *pSid);
int dsCltCvtCookiesSidNum(char *pValue,int *iPasid,int *iNum1,int *iNum2,char *pSid);
dsClientInfo *dsCltCheckClientInfo(utShmHead *psShmHead,
                                         int iNum,
                                         char *pSid,
                                         unsigned long lIp,
                                         int *iRet);
int dsCltClientIsLogin(utShmHead *psShmHead,utMsgHead *psMsgHead);
dsClientInfo *dsCltGetClientInfoByName(utShmHead *psShmHead,char *pName,
            unsigned long lIp,int *iNum);
int pasCheckMyRight(char *pFunName);
int dsCltGetMyInfo(int iSum,...);
long dsCltGetSeque();
/* dsweb003.c */
int dsCltWebFunlist(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int dsCltWebLogin(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int dsCltWebTestCookies(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int dsCltWebFunGen(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);

                  
#endif
