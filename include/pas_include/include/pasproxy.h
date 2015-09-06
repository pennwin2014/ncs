#ifndef  ___PASC_DEF___
#define  ___PASC_DEF___ 
#define PASC_FUN_HEAT            11    /* 心跳请求  */
#define PASC_FUN_ERROR           12    /* 错误      */
#define PASC_FUN_NEWHEAT         13    /* 新的心跳请求  */
#define PASC_ERR_CONNECT         101   /* 数据库连接出错  */
#define PASC_ERR_NORESP          102   /* 没有应答        */
#define PASC_FLAGS_NOKEEP  0     /* 不需要保存    */
#define PASC_FLAGS_KEEP    1     /* 需要长期连接  */
#define PASC_FLAGS_ACTIVE  2     /* 连接正在活动  */
#define PASC_STATUS_BUSY   1     /* 正在忙        */
#define PASC_STATUS_FREE   0     /* 正在忙        */
#define PASC_STATUS_ERROR  9     /* 连接出错      */
#define PAS_MAX_PASSERVER  8
typedef struct pasIpAddress_s {
    unsigned long    lIp;
    unsigned short   nPort;
    unsigned char    cFlags;
    unsigned char    cRev;
} pasIpAddress;


/* 本地Tcp连接 Msg->proxy */
typedef struct pasSLocal_s {
    int iFd;                    /* Sock Id             */
    int iLocalSock;             /* 本地SockId          */
    unsigned long  lIp;         /* 连接IP地址         */
    unsigned short nPort;       /* 端口号             */
    unsigned char  cFlags; 
    unsigned char  cStatus;      /* 状态               */
    unsigned long  lPid;         /* PasId, 若为0 本地  */
    unsigned long  lLastTime;    /* 最后操作时间       */
    char           caKey[32];    /* 加密Key            */
    utMsgHead      *psMsgHead;   /* 队列消息           */
} pasSLocal;


typedef struct pasSrvInfo_s {
    int iMaxThread;                   /* 最大线程数量  */
    int iSumThread;                   /* 当前线程数    */
    int iMaxTcp;
    int iThisMaxTcp;                  /* 当前用到的最大Tcp连接数 */
    unsigned long  lLastTime;         /* 最后操作时间  */
    unsigned long  lStartTime;        /* 启动时间 */
    char     caPort[64];
    pasSLocal  *psLocal;
    int     iTcpSock[PAS_MAX_TCPPORT];                     /* Proxy端口        */
    int (*_pasSGetOnlineInfo)(utShmHead *,unsigned long ,char *);     /* 取得在线信息  */
    int (*_pasSSetOnlineInfo)(utShmHead *,unsigned long);             /* 设置在线信息  */
    utShmHead  *psShmHead;
} pasSrvInfo;

typedef struct pasSrvThread_s {
    pasSrvInfo *psSrv;
    int        iFd;
} pasSrvThread;


/* Pas proxy客户端信息  */

typedef struct pasCSrvList_s {
    unsigned long  lPid;           /* 客户端ID         */
    unsigned long  lIp;            /* Ip地址, 网络顺序  */
    unsigned short nPort;          /* 端口, 网络顺序    */
    unsigned short nStatus;        /* 状态              */
    int            iSock;          /* 连接Sock          */
    unsigned long  lLastTime;      /* 最后连接时间      */
    char           caKey[32];      /* 通信密钥          */
} pasCSrvList;

/* 客户端进程信息   */
typedef struct pasCltProcess_s {
    unsigned long iPid;        // 进程Id
    unsigned long lStartTime;  // 时间
    unsigned long lLastTime;   // 最后访问时间
} pasCltProcess;


typedef struct pasCltInfo_s {
    unsigned long  lLocalIp;    /* 本地Pas地址  */
    unsigned short nLocalPort;  /* 本地端口     */
    unsigned short nSumServer;  /* 服务器总数  */
    unsigned short nMaxServer;  
    unsigned short nMaxThread;
    unsigned short nSumThread;
    unsigned short nCurServer;
    unsigned long  lTimeStep;      /* 心跳周期  */
    unsigned long  lTimeOut;       /* 超时      */
    unsigned long  lStartTime;
    unsigned long  lLastTime;
    pasCSrvList    *psSrv;     /* 服务器列表  */
    pasCltProcess  *psProcess;
} pasCltInfo;

typedef struct pasCltThread_s {
    pasCltInfo  *psClt;
    pasCSrvList *psSrv;
    int        iFd;
} pasCltThread;

typedef struct pasConfig_s {
    char caVar[32];
    char caValue[128];
    struct pasConfig_s *next;
} pasConfig;
    

/* pasproxy.c */
int pasProxyServer(utShmHead *psShmHead,
                    int _pasSGetOnlineInfo(utShmHead *,unsigned long ,char *),
                    int _pasSSetOnlineInfo(utShmHead *,unsigned long));
int pasProxySrv(utShmHead *psShmHead,int *iTcpSock,int _pasSGetOnlineInfo(utShmHead *,unsigned long ,char *),
    int _pasSSetOnlineInfo(utShmHead *,unsigned long));
int pasProxySendMsg(unsigned long lIpAddress,unsigned short nTcpPort,
                unsigned long lFunCode,unsigned long lSid,unsigned long lRid);
int pasProxySendMsgToSock(int iSock,unsigned long lFunCode,unsigned long lSid,unsigned long lRid,int iCode);
int _pasCCvtIpPort(char *caSip,unsigned long *_lSip,unsigned short *_nSport);
/* pasproxy_c.c */
pasCltInfo *_pasProxyCltInit(utShmHead *psShmHead);
int pasProxyClt(utShmHead *psShmHead,int iFlags);
int pasProxyCltProcessIsFree(pasCltInfo *psClt,pasCltProcess *psProcess,int iTimeOut);
int pasProxyDOServerMsg(utMsgHead *psMsgHead,int iFd,pasCSrvList *psSrv,pasCltInfo *psClt);
int pasProxySetServerIp(utShmHead *psShmHead,unsigned long lIp,unsigned short nPort,unsigned long lProId,char *pKey);
pasConfig *pasUtlLoadConfig(char *pFile);
int pasUtlConfigFree(pasConfig *ps);
char *pasUtlGetConfVarValue(pasConfig *ps,char *pVar,char *pDef);
char *pasUtlGetConfVarValueNum(pasConfig *ps,int n,char *pVar,char *pDef);
unsigned long pasUtlGetConfVarValue_l(pasConfig *ps,char *pVar,unsigned long lValue);
int pasProxySetSynApi(int (*fFunName)(utShmHead *));
    
#endif
