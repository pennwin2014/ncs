#ifndef  ___DSSRV_DEF___
#define  ___DSSRV_DEF___
#define DS_MSG_CODE         990118101L
#define DS_DEFAULT_TCPPORT      8086
#define DS_DEFAULT_STATUSTIME   20
#define DS_DEFAULT_DSCLTTIMEOUT 60

#define DS_DEFAULT_MINDSCLIENTS  20    /* 最小客户端数量 */
#define DS_DEFAULT_MAXDSCLIENTS  60    /* 最大客户端数量 */
#define DS_DEFAULT_STEPCLIENTS   20    /* 每次增加的数量 */
#define DS_DEFAULT_CHECKAPPTIME  15    /* 在线信息缺省检查时间 */
#define DS_DEFAULT_KEEPAPPTIME   30    /* 在线应用确省保留时间 */
#define DS_DEFAULT_MINDUSERS    255   /* 目录服务支持的用户数量  */
#define DS_DEFAULT_MAXDUSERS    1024  /* 最大客户端数量 */
#define DS_DEFAULT_STEPUSERS    100   /* 每次增加的数量 */

#define PAS_SERVER_KEY   "pro56321123"


/* 应用服务器类型      */
#define AP_TYP_DSSRV                   1               /* 目录服务器     */
#define AP_TYP_PASSRV                  2               /* 一般的PAS服务  */
#define DSSRV_PASS_KEY                 "12pronetway"
/* 应用服务器内存结构  */
#define DS_MAX_ROLELEN                 128
#define DS_MESSAGE_OK                  0               /* 正常*/
#define DS_ERROR_APPNOEXIST            100001L         /* 服务器名字不存在      */
#define DS_ERROR_KEYERR                100002L         /* 口令出错              */
#define DS_ERROR_EXCEEDAPP             100103L         /* 超过服务器的最大数量  */
#define DS_ERROR_MSGTIMEOUT            100104L         /* 消息超时              */
#define DS_ERROR_DBERROR               200000L         /* 超过服务器的最大数量  */

/* 错误信息  */

#define DS_ERROR_DATABASE_ERROR        1000000        /* 数据库错误        */

#define DS_ERROR_LOGIN_PASSERR         1000101        /* 口令出错          */
#define DS_ERROR_LOGIN_NOUSER          1000102        /* 用户不存在        */
#define DS_ERROR_LOGIN_MEMERR          1000103        /* 内存出错          */

#define DS_ERROR_LOGIN_NODICT          1000102        /* 无数据字典        */

#define DS_ERROR_RECADD_NONAME         1000101        /* name字段没有或为空  */
#define DS_ERROR_RECADD_NOGROUP        1000102        /* group字段没有或为空 */
#define DS_ERROR_RECADD_GROUPNOEXIST   1000103        /* group不存在         */


#define DS_ERROR_USRADD_USREXIST       1000102        /* 用户已经存在      */
#define DS_ERROR_RECADD_FLSTERR        1000103        /* 字段名称不对      */
#define DS_ERROR_USRADD_PASSWORD       1000104        /* 口令为空          */
#define DS_ERROR_USRADD_LOGNAME        1000105        /* 用户名为空        */
#define DS_ERROR_USRADD_DATABASE       1100000        /* 数据库错误        */

#define DS_ERROR_GLIST_NODICT          1000201        /* 字段名称在数据字典中不存在  */
#define DS_ERROR_GLIST_INVMSG          1000202        /* 消息内容不正确    */

#define DS_ERROR_DICT_DSNOEXIST        2000101        /* 数据字典不存在    */

#define DS_ERROR_DICT_FATTRIB          2000101        /* 数据字典中属性内容不正确 */

#define DS_ERROR_MSG_VARNOEXIST        2000102        /* 消息中的变量不存在  */
typedef  struct  {  /* 数据字典  */
    char            caDname[32];          /* 字典组ID     */
    char            caName[32];           /* 字典名称     */
    unsigned short  nStart;               /* 起始序号     */
    unsigned short  nSum;                 /* 总数         */
    char            caRight[8];           /* 操作权限 0--系统管理员 1--组管理员  2--自己  3--同组 4--别人   */
} dsIndex;

typedef  struct  {  /* 数据字典属性  */
    char            caDname[32];           /* 字典ID     */
    char            caFname[32];           /* 字段名称   */
    char            cFlags;                /* 0--固定  1--基本  2--动态  */
    char            cDtype;                /* 数据类型   */
    unsigned short  nBytes;               /* 数据长度   */
    char            caRight[8];           /* 操作权限 0--系统管理员 1--组管理员  2--自己  3--同组 4--别人   */
} dsAttr;




typedef  struct  {  /* 字段属性  */
    char            caFname[32];           /* 字段名称   */
    char            cFlags;                /* 0--内部 1--固定  2--基本  3--动态  */
    char            cDtype;                /* 数据类型   */
    unsigned short  nBytes;                /* 数据长度   */
    char            caRight[8];            /* 操作权限 0--系统管理员 1--组管理员  2--自己  3--同组 4--别人   */
} dsFattr;


typedef union {
    dsIndex       psIndex;
    dsAttr        psAttr;
} dsDiction;
        

typedef  struct {  /* 在线应用  */
    unsigned long lId;           /* 客户端ID               */
    unsigned long lGroupId;      /* 组ID                   */
    char     caName[32];         /* 名称                   */
    char     caSid[24];          /* 验证的Key              */
    unsigned long  lIp;          /* IP地址                 */
    unsigned short nTcpPort;     /* Tcp端口号              */
    unsigned short nUdpPort;     /* UDP端口号              */
    unsigned short nCltType;     /* 客户端类型 1--Ds服务器 2--PAS用户  3--Web用户 */
    unsigned short nStatus;      /* 0--不在线  1--在线     */
    unsigned short nLeavel;      /* 用户级别               */
    unsigned short nTimeOut;     /* 超时时间               */
    unsigned long  lLoginTime;   /* 登陆时间               */
    unsigned long  lLastTime;    /* 最后一次操作时间       */
} dsAppOnline;


typedef  struct {  /* 在线应用  */
    unsigned long lId;           /* 客户端ID 0--初试值 1--已经删除 2--临时占用  */
    char     caGroup[32];        /* 组名                   */
    char     caName[32];         /* 名称                   */
    char     caSid[24];          /* 验证的Key              */
    unsigned long  lIp;          /* IP地址                 */
    unsigned short nTcpPort;     /* Tcp端口号              */
    unsigned short nUdpPort;     /* UDP端口号              */
    unsigned short nCltType;     /* 客户端类型 1--Ds服务器 2--PAS用户  3--Web用户 */
    unsigned short nStatus;      /* 0--不在线  1--在线     */
    unsigned short nLevel;      /* 用户级别               */
    unsigned short nTimeOut;     /* 超时时间               */
    unsigned long  lOnlineNum;  /* 在目录服务器上的位置   */
    unsigned long  lLoginTime;   /* 登陆时间               */
    unsigned long  lLastTime;    /* 最后一次操作时间       */
} dsSrvUserInfo;



/* 用户权限信息---用于配置文件的读入  */
typedef struct dsRightList_s {
    unsigned long  lRightId;                 /* 权限ID   */
    char           caAlias[16];              /* 权限别名 */
    char           caDesc[34];               /* 权限描述 */
    unsigned short nSumFun;                  /* 功能总数 */
    unsigned long *lpFunList;
    struct dsRightList_s    *psNext;
} dsRightList;


typedef struct {
    unsigned long  lSumRight;                /* 权限总数 */
    unsigned long  lSumBytes;                /* 字节总数 */
    dsRightList    *psRight;
} dsRightHead;


/* 用户权限信息 ----在共享内存中的结构 */
typedef struct dsShmRightList_s {
    unsigned long  lRightId;                 /* 权限ID   */
    char           caAlias[16];              /* 权限别名 */
    char           caDesc[34];               /* 权限描述 */
    unsigned short nSumFun;                  /* 功能总数 */
    unsigned long  iFunStart;
} dsShmRightList;


typedef struct {
    unsigned long  lSumRight;                /* 权限总数 */
    unsigned long  lSumBytes;                /* 字节总数 */
} dsShmRightHead;

    

/* 角色信息  */
typedef struct {
    unsigned long     lRoleId;              /* 角色ID              */
    char              caRight[128];         /* 角色对应的权限      */   
} dsRoleInfo;


/* dssrv001.c */
int dsSrvInit000(utShmHead *psShmHead);
int dsSrvTcpInit(utShmHead *psShmHead);
int dsSrvUdpInit(utShmHead *psShmHead);

/* dssrv002.c */
int dsSrvAppLogin(utShmHead *psShmHead, 
            utComSockAddr *psSockAddr,utMsgHead *psMsgHead);
int dsSrvAppStatus(utShmHead *psShmHead, 
            utComSockAddr *psSockAddr,utMsgHead *psMsgHead);            
char *dsSrvCreateSid();


/* dssrv003.c   有关数据字典的操作  */
int dsSrvSyncCltRight(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int dsSrvDicList(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int dsSrvDictModi(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int dsSrvDictAdd(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int dsSrvDictDel(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
/* dssrv004.c  有关用户组的操作    */
int dsSrvGroupList(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int dsSrvGroupModi(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int dsSrvGroupAdd(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int dsSrvGroupDel(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
dsFattr *dsDictCvtFattr(utShmHead *psShmHead,char *caFlist,char *pName,int *iSum);
dsFattr *dsDictAttr(utShmHead *psShmHead,char *pName,int *iSum);
long  dsUtlGetGroupId(char *pGname);
int dsSrvSeque(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
/* dssrv005.c  有关用户的操作      */
int dsSrvUserList(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int dsSrvUserModi(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int dsSrvUserAdd(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int dsSrvUserDel(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);

/* dssrv006.c 有关服务器的操作  */
int dsSrvServerList(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int dsSrvServerModi(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int dsSrvServerAdd(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int dsSrvServerDel(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);

/* dssrv007.c 有关记录的操作  */
int dsSrvRecordList(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int dsSrvRecordModi(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int dsSrvRecordAdd(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int dsSrvRecordDel(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int dsSrvRecordDelWhere(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int dsSrvRecordAddSome(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);

/* dssrv008.c 有关SQL的操作  */
int dsSrvSqlRecordSet(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int dsSrvSqlExec(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int dsSrvSqlExecSome(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);

/* dssrv009.c 用户登陆  */
int dsSrvUserLogin(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
char *dsSrvGenSid();
dsSrvUserInfo *dsSrvGetUserInfo(utShmHead *psShmHead,char *pSid,int *iNum);
dsSrvUserInfo *dsSrvGetFreeUserInfo(utShmHead *psShmHead,int *iNum);
int dsSrvUserLoginConfirm(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
dsSrvUserInfo *dsSrvGetUserInfoByNum(utShmHead *psShmHead,int iNum,char *pSid);
dsSrvUserInfo *dsSrvGetUserInfoByName(utShmHead *psShmHead,char *caName,char *caIp,int *iNum);
#endif
