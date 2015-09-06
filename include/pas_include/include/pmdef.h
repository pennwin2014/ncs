#ifndef __PMDEF_DEF__
#define __PMDEF_DEF__        1
#define PM_LNK_ONLINE       31    /* 在线用户信息  */
#define PM_LNK_ALLUSER      32    /* 全部用户      */
#define PM_LNK_ADDRESS      33    /* 全部地址      */
#define PM_LNK_GROUP        34    /* 全部部门      */
#define NC_LNK_PROCESS      35
#define NC_LNK_ONTIME       36
#define NC_LNK_ACCESSIP     37
#define PM_ACTION_ADMLOGIN    1
#define PM_ACTION_POPLOGIN    2
#define PM_ACTION_SMTPLOGIN   3
#define PM_ACTION_WEBLOGIN    4
#define PM_ACTION_SMTPSEND    5
#define PM_ACTION_SMTPRECV    6
#define PM_ACTION_MAXSEND     7    /* 超出限制  */
#define PM_ACTION_RECOVER     8    /* 解除禁用  */

#define PM_COND_EQUAL           1
#define PM_COND_INCLUDE         2
#define PM_COND_START           3
#define PM_COND_END             4   

#define PM_ACCESS_ALLOWED       1
#define PM_ACCESS_DENCY         0
#define PM_ACCESS_END           9

#define PM_PID_QUEUE          1
#define PM_PID_GSEND          2
#define PM_PID_POP            3
#define PM_PID_PROXY          4
#define PM_PID_ONTIME         5
#define PM_PID_CHKMAILSEND    6
#define PM_PID_VIRUS          7





/* Right 字段中，每一位 1--表示拥有该项权限  0--表示没有权限   */
#define PM_ROLE_SYSTEM        0        /* 系统管理员   */
#define PM_ROLE_DOMAIN        1        /* 域管理员     */
#define PM_ROLE_GROUP         2        /* 组管理员     */
#define PM_ROLE_USER          3        /* 一般用户     */
#define PM_ROLE_QSEND         4        /* 允许群发     */


/* 规则作用范围                         */
#define PM_OWNER_SYSTEM       0        /* 系统   */
#define PM_OWNER_USER         1        /* 个人   */

/* 处理方法                             */
#define PM_DO_DELETE          1        /* 直接删除   */
#define PM_DO_MOVE            2        /* 移到指定文件夹   */
#define PM_DO_FORWARD         4        /* 转发             */
#define PM_DO_REPLAY          8        /* 自动应答         */


/* 在线用户信息   
 */
typedef struct pmOnlineUser_s {
    char sid[24];
    char username[32];
    char maildir[63];
    char vhost[63];               /* 域名          */
    int  groupid;                 /* 组名          */
    char role[8];                 /* 用户权限      */
    unsigned char editmode;       /* 缺省编辑方式 0--文本 1--Html */
    unsigned char maxatt;         /* 最大附件数                   */
    unsigned char savecopy;       /* Save Copy      */
    unsigned char logmode;        /* 0--Web  1--Adm */
    unsigned char scanvirus;      /* 1 显示的时候自动检测病毒   0--不检测        */
    unsigned char gsend;          /* 0--不允许群发   1--允许群发        */
    unsigned char utype;          /* 0--普通用户   1--系统管理员  2--域管理员  */    
    unsigned char reverse[2];    
    unsigned long lMaxMail;       /* 可以群发的最大邮件数   */
    unsigned long lMasSize;       /* 最大限额      */
    unsigned long lIp;            /* 客户端IP地址  */
    unsigned long lLastTime;      /* 最后操作时间  */
} pmOnlineUser;

/* 文件夹列表  */
typedef struct {
    unsigned long iFileNum;
    char     cname[32];
} pmFoldLst;


struct pmTimename
{
        time_t mtime;
        char name[64];
};

typedef  struct {
	unsigned int	num;		/* total num of mail in folder */
	unsigned int	news;		/* total num of new mail in folder */
	unsigned long	size;		/* total size of mail in folder */
} pmFoldInfo;

typedef struct pmScanList_s {
    char caName[32];          /* 名称     */
    char caFile[128];          /* 文件名   */
    struct pmScanList_s *next;
} pmScanList;

/* 用户信息  */
typedef struct pmUserInfo_s {
    char   alias[32];
    char   passwd[32];
    int    groupid;
    unsigned long lMaxSize;
    char   vhost[40];
    char   userrole[20];
    char   maildir[100];
    char   address[64];         /* 默认地址          */
    unsigned long lCount;       /* 登录次数          */ 
    unsigned long lLasttime;    /* 最后访问时间      */
} pmUserInfo;

/* 地址信息   */
typedef struct pmAddress_s {
    char  address[64];
    char  alias[32];
} pmAddress;

typedef struct pmGroup_s {
    long  groupid;
    char  groupname[20];
} pmGroup;

typedef struct ncProcessInfo_s {
    int iPid;
    unsigned long lStartTime;      /* 启动时间  */
    unsigned long lTimeOut;        /* 超时时间  */
    unsigned long lOntime;         /* 在每天指定时间重新启动  */
    unsigned char caControl[32];   /* 控制信息  */
    char     caName[32];           /* 功能名称  */
    int   (*fFunName)(utShmHead *); /* 函数名   */
    int   iFlags;                   /* 0--无  1--内部进程  2--外部  */
} ncProcessInfo;

typedef struct ncOnTimeFun_s {
    unsigned long lLastTime;            /* 最后一次执行时间  */
    unsigned long lUsedTime;            /* 最后一次执行使用时间  */
    unsigned long lCount;               /* 执行次数      */
    unsigned long lStepTime;            /* 间隔时间      */
    unsigned long lOnTime;              /* 指定时间 hhmmss       */
    int      iStatus;                   /* 0--空闲  1--等待 2--正在执行  */
    char     caName[32];                /* 功能名称      */
    int   (*fFunName)(utShmHead *);     /* 执行函数名称  */
} ncOnTimeFun;


/* 验证限制     */
/*  Include     pronetway.com       192.168.0.1  192.168.0.255   Allowed     */
/*  StartWith   lym                 192.168.0.1  192.168.0.255   Allowed     */
/*  EndOf       .com                192.168.1.0  192.168.1.255   Dency       */
/*  Equal       lym@pronetway.com   192.168.1.0  192.168.1.255   Dency       */
/*  Equal       any                 0.0.0.0     255.255.255.255 Allowed      */

typedef struct pmAccessIp_s {
    char     caDomain[64];             /* 域名                                         */
    unsigned long lSip;                /* 起始IP地址                                   */
    unsigned long lEip;                /* 终止IP地址                                   */
    unsigned short  iCond;             /* 条件  1-- Equal 2--Include  3--Start  4--End */
    unsigned short  iFlags;            /* 0--禁止  1--允许   */
} pmAccessIp;

/* 邮件发送限制                                           */
/*  From include lym To startwith pronetway.com  allowed  */
/*  From Equal   any to include   pronetway.com  allowed  */

typedef struct pmSmtpSend_s {
    char     caFrom[64];
    char     caTo[64];
    unsigned char     cFromCond;
    unsigned char     cToCond;
    unsigned short    nFlags;
} pmSmtpSend;
/* pasutl_vir.c */
int pmUtlSock(char *pPath);
int pmUtlKavStart(utShmHead *psShmHead);
int pmUtlKavTest(int iSock);
int pmUtlKavInfo(int iSock,char *pDate,char *pVersion);
int pmUtlKavKeyInfo(int iSock,char *pSn,char *pExpire);
int pmUtlKavScan(int iSock,char *pFile,int iOpt);
int pmUtlKavEnd(int iSock);
int pmUtlScanDir(int iSock,char *pDir,int iOpt);
int pmUtlScanMailDir(int iSock,char *pDir);
pmScanList *pmUtlKavScanMail(int iSock,char *pFile);
int pmUtlKavDoMail(char *caFile,pmScanList *psList,int iOpt,int iSender);
int pmUtlGetStr322(char *p,char *caName,char *caFile);
pmScanList *pmUtlAddScanList(pmScanList *psList,char *pName,char *pFile);
int pmUtlFreeScanList(pmScanList *psList);
int pmUtlClamDoMail(char *caFile,char *caVname,int iOpt,int iSender);
int pmUtlClamSock(char *pPath);
int pmUtlClamSend(char *pComm,char *pResult,int iMax);
int pmUtlClamTest();
int pmUtlClamInfo(char *pVersion,char *pDate);
int pmUtlClamReload();
int pmUtlClamScan(char *pFile,char *pOut);
int pmUtlClamScanStart();
int pmUtlClamScanEnd(int iSock);
int pmUtlClamScanOneFile(int iSock,char *pFile,char *pOut);
char *pmStrGetWord(char *p,char *pOut,
                   short nMaxLen,char *pTerm);
char *pmUtlGetLine(char *pBuf);
int pmStrIsSpaces(char *pIn);
int pmUtlReturnMail555(char *caFile);
char *pmUtlCvtVdate(char *pDate);
#endif
