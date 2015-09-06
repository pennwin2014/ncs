#ifndef  ___DSSYNC_DEF___
#define  ___DSSYNC_DEF___
#define DSSYNC_DB_UPLOAD                   0
#define DSSYNC_DB_DOWNLOAD                 1
#define DSSYNC_DEFAULT_MINSYNCCLIENTS      20    /* 最少客户数      */
#define DSSYNC_DEFAULT_STEPSYNCCLIENTS     10    /* 每次增加客户数      */
#define DSSYNC_DEFAULT_MAXSYNCCLIENTS      500   /* 最大客户数      */

#define DSSYNC_DEFAULT_CHECKCLTTIME        10    /* 客户端检查间隔  */
#define DSSYNC_CLIENT_ONLINE               1
#define DSSYNC_CLIENT_OFFLINE              2
#define DSSYNC_MAXSQLSMT                  16000
#define DSSYNC_MAXLOGLEN                  60000
typedef  struct {
    char caSrvname[48];   /* 服务器名称 srvname      --- 服务器列表
                                        srvname_srv1 --- 服务器对应的表 */
    char caValues[8192];  /* 服务器内容  */
} dsSyncServerHead;



typedef struct dsSyncServerLst_s {
    char  caSrvname[32];
    char  *psTable;
    struct dsSyncServerLst_s *psNext;
} dsSyncServerLst;


typedef struct dsSyncServer_s {
    char *pUpload;
    dsSyncServerLst *psDownload;
} dsSyncServer;

/* 同步记录  */
typedef struct dsSyncSrvRecord_s {
    char             caName[32];    /* 名字      */
    char             *pTable;       /* 表名     */   
    unsigned long    lTime;     /* 同步时间  */
    unsigned long    lPos;      /* 文件位置  */
} dsSyncSrvRecord;

typedef struct dsSyncRecord_s {
    unsigned long  lDbStartTime;      /* 数据的起始日期  */
    char           caDblogFile[128];  /* DB日志文件名称 */
    char           caDblogPath[128];  /* 转换后日志文件存放的目录  */
    char           caName[32];     /* 服务器名称      */
    char           caSid[32];      /* 通信密要钥      */
    unsigned long  lLogPos;        /* 日志文件的序号  */
    unsigned long  lLogTime;       /* 日志文件的时间 0--表示无未处理的临时文件 */
    unsigned short nNum;           /* 序号            */
    unsigned long  lServerIp;      /* 服务器的Ip地址  */
    unsigned short nServerPort;    /* 服务器的端口号  */
    unsigned short nSumSync;       /* 需要同步的服务器数量    */
    unsigned short nCurSrv;        /* 当前已完成操作的服务器  */
    short          nStatus;        /* 状态  <0 出错           */
    dsSyncSrvRecord *psSrv;
    char            *pUpload;      /* 上载的表              */
 } dsSyncRecord;
  

typedef  struct {  /* 同步客户信息  */
    char     caName[32];         /* 名称                   */
    char     caSid[32];          /* 验证的Key              */
    unsigned long  lIp;          /* IP地址                        */
    unsigned short nStatus;      /* 0--NoUse  1--在线 2--不在线       */
    unsigned char  cModiFlags;   /* 修改标志 0--未修改 1-已修改       */
    unsigned char  cLock;        /* 锁  0--正常  1--锁     */
    unsigned long  lLoginTime;   /* 登陆时间               */
    unsigned long  lLastTime;    /* 最后一次操作时间       */
    unsigned long  lRecordTime;  /* 当前文件的时间         */
    unsigned long  lRecordPos;   /* 最后保存记录的位置     */
} dsSyncOnline;

/* dssync001.c */
int dsSyncInit000(utShmHead *psShmHead);
int dsSyncTcpInit(utShmHead *psShmHead);
int dsSyncLogin(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int dsSyncCvtDownLoad(dsSyncServer *psServer,char *caValue);
dsSyncServer  *dsSyncAuth(utShmHead *psShmHead,char *pName,char *pKey,char *pSid,int *iNum);
int dsSyncRequestUpload(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int dsSyncPutLastRecord(char *pName,long lTime,unsigned long lPos);
int dsSyncDataUpload(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int dsSyncRequestDownload(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int dsSyncGetSql(char *pTable,    /* 要求的表     */
                 char *pName,     /* 服务器名称   */
                 unsigned long lTime,
                 unsigned long lPos,
                 unsigned long *lLastPos,
                 unsigned long *iSum,
                 char *pSql);
int dsSyncWriteLog(utShmHead *psShmHead,unsigned long lTime,
                      char *pName,char *pSmt);
int dsSyncWriteDbRecord(char *pSmt,int   iSum);
char *dsSyncGetOneSql(char *pSmt,char *pSql,int iMaxLen);
dsSyncOnline *dsSyncCheckAuth(utShmHead *psShmHead,int iNum,char *pSid);
                                        
/* dssync002.c */
dsSyncServer *dsSyncLoadConfig(char *pConfFile,char *pServername,char *pKey);
dsSyncServer *dsSyncReadOneServer(FILE *fp,char *pServername,char *pKey);
dsSyncServer *dsSyncReadServer(FILE *fp,char *pName,char *pKey);
int dsSyncFreeServerLst(dsSyncServerLst *psServerLst);
dsSyncServerLst *dsSyncAddServer(dsSyncServerLst **psServerLst,
           char *pSrvname,char *pTable);
char *dsSyncAddStr(char *pSrc,char *pAdd);
/* dssync003.c */
unsigned long dsSyncFileSize(char *pFile);
unsigned long dsSyncCvtLongTime(char *pInstr);
int dsSyncGetOneSqlFromFile(FILE *fp,char *pTable,char *pSql);
#endif
