#ifndef  ___PAS_DEF___
#define  ___PAS_DEF___   
#define	PAS_LISTENQ		1024
#ifdef  IRIX_DEF
#include "../include/sgiconfig.h"
#define SCM_RIGHTS          1
#endif
#ifdef  LINUX_DEF
#define  PAS_TCP_PASSSOCK         1
#define  HAVE_MSGHDR_MSG_CONTROL 1
#endif
#ifndef CMSG_LEN
/* $$.Ic CMSG_LEN$$ */
#define CMSG_LEN(size)          (sizeof(struct cmsghdr) + (size))
#endif
#ifndef CMSG_SPACE
/* $$.Ic CMSG_SPACE$$ */
#define CMSG_SPACE(size)        (sizeof(struct cmsghdr) + (size))
#endif
#define PAS_DEF_KEY        "p#ronetwayItsE23"
#define PAS_FUNKEY         "pronetway"
#define PAS_SERVER_TCP     1
#define PAS_SERVER_UDP     2
#define PAS_MAX_RUN_TIME   300    /* 进程最长运行时间    */
#define PAS_MAX_PRO_TIME   14400  /* 一个进程的生命周期  */
#define PAS_MAX_REQ_COUNT  50000  /* 最多请求次数        */
#define PAS_LNK_UDPPROCESS  1      /* Udp进程信息         */
#define PAS_LNK_TCPPROCESS  2      /* Tcp进程信息         */
#define PAS_LNK_PROCESS     3      /* 其它进程信息        */
#include <stdarg.h>

#define PAS_DEBUG_ON        1
#define PAS_DEBUG_OFF       0

#define PAS_CHECKSUM_NONE   0
#define PAS_CHECKSUM_MD5    1
#define PAS_CRYPT_NONE      0
#define PAS_CRYPT_DES       1
#define PAS_CRYPT_TEA       2
#define PAS_AUTH_NONE       0
#define PAS_AUTH_ONLY       1
#define PAS_AUTH_CHECKRIGHT 2
#define PAS_FUN_CANTUSE     1
#define PAS_FUN_NORMAL      0

#define PAS_RETURN_DEBUGON    1018
#define PAS_RETURN_DEBUGOFF   2018
#define PAS_FUN_TEST        1      /* 测试信息            */
#define PAS_FUN_VERSION     2      /* 当前版本号          */
#define PAS_FUN_MESSAGE     3      /* 取得服务器当前状态  */
#define PAS_FUN_SETDEBUG    4      /* Set Debug           */
#define PAS_FUN_ONTIMEDO    5      /* 定时处理函数        */
#define PAS_FUN_DOCOMMAND   6      /* 定时处理函数        */

#define PAS_ONTIMEDOCODE   19651129L  /* 定时处理校验数据  */
#define PAS_FUN_SHOW        0       /* 在功能列表上显示    */
#define PAS_FUN_NOSHOW      1       /* 在功能列表上不显示  */
#define PAS_AUTH_NOAUTH     0       /* 不验证              */
#define PAS_AUTH_AUTHPASS   1       /* 验证通过即可        */
#define PAS_AUTH_CHECKRIGHT 2       /* 检查权限            */

#define PAS_FUN_USE         1       /* 功能使用标志            */
#define PAS_FUN_SECRET      2       /* 安全标志                */
#define PAS_FUN_CHECKSUM    3       /* 校验标志                */
#define PAS_FUN_AUTH        4       /* 验证                    */
#define PAS_FUN_MAXRUNTIME  5       /* 函数允许执行的最大长度  */

#define PAS_MAX_TCPPORT     8
#define PAS_DEBUG_ERROR     1
#define PAS_DEBUG_WARN      2
#define PAS_DEBUG_LOG       4
#define PAS_DEBUG_DEBUG     8


    
/* TCP进程信息  */
typedef struct {
    int             iPid;           /* 进程号  */
    int             iStatus;        /* 状态  0--空闲  1--忙 */
    unsigned long   lStartTime;     /* 进程开始运行时间     */
    unsigned long   lLastTime;      /* 进程开始运行时间     */
    unsigned long   lMaxRunTime;    /* 允许运行的最大时间   */
    int             iFun;           /* 功能代码             */
    int             iFd;            /* 句柄                 */
    unsigned long   lReqBytes;      /* 请求字节数           */
    unsigned long   lResBytes;      /* 应答字节数           */
    unsigned long   lSumCount;      /* 总的次数             */
    unsigned long   lCount;         /* 处理次数             */
} pasProcess;


/* 进程信息  */
struct pasProcessInfo_s {
    int iMinProcess;
    int iMaxProcess;
    pasProcess *psProcess;
};

/* 功能描述  */
typedef struct  {
    int   iFunCode;                 /* 功能代码                    */
    int   (*fFunName)(utShmHead *,int,utMsgHead *);
    int   iMaxTime;                 /* 函数执行的最长时间          */
    char  caFunName[32];            /* 功能说明                    */
    unsigned char cUseFlags;        /* 使用标志 0--可用 1--不可用  2--自定义 */
    unsigned char cSecretFlags;     /* 数据包加密标志 0--根据数句包 1--DES加密      */   
    unsigned char cCheckSumFlags;   /* 数据包校验标志 0--不校验     1--Md5Code校验 2--CrC校验  */   
    unsigned char cAuthFlags;       /* 验证方式 0--不验证  1--通过passid验证(用户信息)
                                                2--通过passid验证(应用服务器验证)
                                                3--必须要有权限 
                                     */
    int   (*fGetKey)(utShmHead *,unsigned long,char *);
} pasTcpFunList;

/* 功能代码保存的格式  */
typedef struct  {
    int   iFunCode;                 /* 功能代码                        */
    char  caFunName[32];            /* 功能说明                        */
    unsigned char cUseFlags;        /* 使用标志 0--可用 1--不可用      */
    unsigned char cFunType;         /* 类别  0--程序内部  1--手工定义  */
    short iAuthFlags;               /* 验证方式 0--不验证 1--验证通过  2--必须要有权限 */
} pasTcpFunList1;


typedef struct  {
    unsigned long lFileId;          /* 文件标识                        */
    unsigned long lVersion;         /* 版本号                          */
    int   iSumFun;                  /* 功能总数                        */
    char  caMd5;                    /* 校验代码                        */
    unsigned long lModiTime;        /* 最后修改时间                    */
} pasTcpFunHead;

/* 角色的结构  */


typedef struct  {
    unsigned long lRoleId;          /* 角色Id                        */
    char     *pRight;               /* 权限字                        */
} pasTcpRoleList;
    

typedef struct  {
    int   iMaxFun;
    int   iNumFun;
    pasTcpFunList *psFunList;
} pasTcpFun;

typedef struct  {
    int   iType;
    int   iFunCode;
    int   (*fFunName)(utShmHead *,utComSockAddr *,utMsgHead *);
    int   iMaxTime;                 /* 函数执行的最长时间  */
    int   iReserve;                 /* 保留                */
} pasUdpFunList;
    
typedef struct  {
    int   iMaxFun;
    int   iNumFun;
    pasUdpFunList *psFunList;
} pasUdpFun;


/* 定时处理函数  */
typedef struct  {
    int iSum;   /* 总共数量  */
    int iMax;   /* 最大允许数量  */
    struct pasComTime_S {
        short iUseflags;   /* 使用标志  0--未用      1--正在使用              */
        short iRunFlags;   /* 执行位置  1--TCP请求   2--UDP请求  3--本地执行  */
        long lTimeOut;    /* 间隔间隔      */
        long lLastTime;   /* 上次访问时间  */
        char *pData;      /* 传递参数      */
        int   (*fFunName)(char *pData);   /* 超时处理函数  */
    } *psTime;
} pasTimeFun; 


typedef struct pasStrVarList_s_s {
    char    *value;
    int4    len; 
} pasStrVarList;


typedef struct pasVarValue_s {
    char            *pVar;              /* 变量名        */
    char            *pValue;            /* 变量值        */
    struct pasVarValue_s *next;
} pasVarValue;    
    

/* utostr14.c */
int8 pasGetLongTimeId();
char *pasGetStringTimeId();
char *pasUtlDbCvtStr2Db(char *pIn,char *pOut,int iMax);
char *pasUtlDbCvtStr2Db1(char *pIn,char *pOut,int iMax);
int pasStrGetVarByChar(char *buf,char cTerm,pasStrVarList *psValue,int iMax);
int pasStrFreeVarlist(pasStrVarList *psValue,int iMax);
int pasStrInitVarlist(pasStrVarList *psValue,int iMax);
int pasStrClearVarlist(pasStrVarList *psValue,int iMax);
                

/* pasutl_hash.c */
unsigned long pasHashBufSize(unsigned long lMaxHash,unsigned long lMaxRec,unsigned long lRecLen);
char *pasHashInit(unsigned long lMaxHash,unsigned long lMaxRec,unsigned long lRecLen,
                    unsigned short nKeyStart,unsigned short nKeyLen);
int pasHashInit0(char *pBase,unsigned long lMaxHash,unsigned long lMaxRec,
            unsigned long lRecLen,unsigned short nKeyStart,unsigned short nKeyLen);
int pasHashReset(char *pHash);
int pasHashAdd(char *pHash,char *pCont,int iFlags);
char *pasHashLook(char *pHash,char *pKey);
char *pasHashLookA(char *pHash,char *pKey);
char *pasHashLookA0(char *pHash,char *pKey,int *iStatus);
int pasHashDel(char *pHash,char *pKey);
int pasHashPrint(char *pHash,long lStart,long lSum);
int pasHashAll(char *pHash,char *pHead,unsigned long lStart,unsigned long lNum,
                           int pasHashFun(char *pHead,char *p));
long pasHashRecord(char *pHash);
long pasHashMaxRecord(char *pHash);
void *pasHashFirst(char *pHash,pasHashInfo *psHashInfo);
void *pasHashNext(pasHashInfo *psHashInfo);
void *pasHashNextS(pasHashInfo *psHashInfo);
int pasHashLock(char *pHead);
int pasHashUnlock(char *pHead);
int pasHashRemoveLock();
pasHashText *pasHashNextGreatIndex(pasHashHead *psHead,long lIndex0,long lIndex,long *lRet);
int pasHashRepair(char *pHash);
/* pasutl_html.c */
char *pasHtmlWordFromTitle(char *pIn,char *pTitle,char *pPre,char *pEnd,char *pOut,int iMax);
char *pasHtmlGetWordFromTd(char *pIn,char *pOut,int iMax);
/* pasutl_http.c */
int pasSendHttpPost(char *pHost,char *pUrl,char *pHtml,int iMax,int iSum,...);
/* pasutl_iplocation.c */
int pasUtlCvtIpDbToBin(char *pFile,char *pOut);
pasStrMem *pasUtlInitStrMem(unsigned long lMax);
unsigned long pasUtlStrMemAdd(pasStrMem *psHead,char *pName);
int pasUtlIpDbSave2Bin(char *pFile,pasUtlIpDb *psIp,int iSumIp,pasStrMem *psHead);
int pasLoadIpdb();
int pasUtlLoadIpDb(char *pFile);
int pasUtlGetIpLocation(unsigned long lIp,char *caArea,char *caName);
int pasUtlFreeIpLocation();
/* pasutl_load2db.c */
int pasUtlLoad2Db(utShmHead *psShmHead);
void pasUtlLoad2DbSetTerminated(char *pFieldTerminited,char *pLineTerminited,char *pEscStr);
void pasUtlLoad2DbSetMaxKeepSize(uint4 lMax);
int pasUtlDoLoad2DbPath( char *pFold,
                     char *pKey,
                     char *pKeep,
                     char *pErrPath);
int4 pasUtlLoadDataFromFile(char *pInPath,char *pFile);
int4 pasUtlLoadDataFromFile0(char *pInPath,char *pFile,char *pKeepPath,char *pErrorPath);
int4 pasUtlGetTableFromLoadFileName(char *pFile,char *pTable);
int4 pasUtlGetKeepFileName(char *pFile,char *pPreName,char *pPost);
int pasUtlDoLoadDataFile2Db(char *caTempFile,char *caTable);
int pasUtlFileAddAndCompose(char *pSPath,char *pSfile,char *pDPath,int4 lMaxSize);
int pasUtlDbDoSqlFromFile(char *pFile,char *pComm,char *pDate,char *pEng);
int pasUtlOut2DbLoadFile(FILE *fp,int iSumField,...);
int pasUtlLoad2DbIsBackTable(char *pTabList,char *pTable);
/* pasutl_mac2vendor.c */
int ncUtlLoadAllMac(char *pFile);
int pasCvtMac3(char *pMac,char *mac);
unsigned char *ncUtlCvtMac2Vendor(char *pMac,char *pVendor);
/* pasutl_useragent.c */
int pasUtlLoadUaConfig(char *pFile);
int pasUtlTermAttr(char *pUa,char *pTerm,char *pOs,char *pBro,char *pTtype);
int pasUtlUaFree();
int pasUtlLoadUaDev(char *pFile);
void pasUtlUaSetDebug(int iFlags);
int pasUtlSortDevType(char *pDev1,char *pDev2);
/* pasutl_pub.c */
int pasConfigValueIs(utShmHead *psShmHead,char *pVar,char *pValue,char *pDefault);
/* pasutl01.c */
char *pasUtlCvtInputText(char *caBuf,char *pName,char *pValue,int iSize,int iMaxLen);
char *pasUtlCvtInputSelect(char *pName,char *pValue,char *pList);
char *pasUtlCvtInputSelectOn(char *pName,char *pValue,char *pAction,char *pList);
char *pasUtlCvtInputSelectn(char *pName,int iNum,char *pList);
char *pasUtlCvtInputSelectAction(char *pName,char *pAction,int iNum,char *pList);
char *pasUtlCvtInputSelect_d(char *pName,   
                           char *pValue,  
                           char *pList,
                           int  iSize,
                           int  iMaxLen
                           );
int pasKillProcess0(char *pUser,char *pName);
int pasKillProcess(char *pUser,char *pName);
int pasKillProcess99(char *pUser,char *pName);
int pasKillProcess5(char *pUser,char *pName);
int pasSignalProcess(char *pUser,char *pName,int iSignal);
int pasSignalProcess0(char *pUser,char *pName,int iSignal);
int pasProcessIsExist(char *pUser,char *pName);
int pasPidIsExist(int *pid,int *status,int iSum);
int pasSysNetPing(char *pHost,char *pDev,int iTimes);
int pasSysPingTest(char *pHost,char *pDev,int iTimes,double dStep,double *dTime);
/* pasutl02.c */
int pasUtlModiConfVar(char *pConfigFile,int iSum,...);
int pasUtlGetConfVar(char *pConfigFile,int iSum,...);
pasConfList *pasUtlReadConfig(char *pFile);
int pasUtlFreeConfig(pasConfList *ps);
int pasUtlPrintConfig(pasConfList *ps);
char *pasUtlLookConfig(pasConfList *ps,char *pVar,char *pDefault);
int pasUtlModiConfig(pasConfList *ps0,char *pVar,char *pValue0);
int pasUtlDelConfig(pasConfList *ps0,char *pVar);
int pasUtlSaveConfig(char *pConfigFile,pasConfList *ps);
int pasUtlSystem(char *pCommand,...);
pasConfList *pasUtlReadNcClient(char *pFile);
int pasUtlSaveNcClient(char *pConfigFile,pasConfList *ps);
/* pasutl03.c */
char *pasGetHwAddr(char *pDev);
/* pasutl04.c */
int pasDefaultCode();
int pasCharSetCode(char *pName);
int pasCharsetSetDefault(int iCode);
int pasCharsetCvt(int sCode,int dCode,char *pIn,char *pOut,int iMax);
char *pasCharsetCvtP(int sCode,int dCode,char *pIn);
int pasCharsetClose();
int pasUtf8ToGBK(char *pIn,char *pOut,int iMax);
int pasBig5ToGBK(char *pIn,char *pOut,int iMax);
int pasJPToGBK(char *pIn,char *pOut,int iMax);
int pasGBKToBig5(char *pIn,char *pOut,int iMax);
int pasUtf8ToBig5(char *pIn,char *pOut,int iMax);
int pasGBKToUtf8(char *pIn,char *pOut,int iMax);
int pasIconvClose();
char *pasCharSetName(int iCode);
char *pasCvtGBK(int iCode,char *pStr,char *pOut,int iMax);
int pasCharsetCvtl(int sCode,int dCode,char *pIn,int iLen,char *pOut,int iMax);
int pasCharIsUtf8(unsigned char *pBuffer, long size);
int pasUcodeToGBK(char *org,char *gn,int iLen);
/* pasutl05.c */
int pasMilCcode();
int pasCharsetCode(char *p);
/* pasutl06.c */
char *pasGetSelectLabel(char *pBuf,char *pname,char *pDefault);
char *pasGetSelectLabel_l(char *pBuf,long lId,char *pDefault);
/* pasutl07.c */
int pasStrGetSomeVarValueBy(char *caRecord,char *pTerm,int iSum,...);

/* pasutl_memtest.c */
void *pas_malloc(size_t size,int line,char *f);
void *pas_calloc(size_t numb,size_t size,int line,char *f);
void pas_free(void *p,int line,char *f);
void *pas_realloc(void *p,size_t size,int line,char *f);
void pas_pintmem();
void pas_freemem();
char *pas_utStrDup(char *pIn,int line,char *f);
char *pas_utStrnDup(char *pIn,int iLen,int line,char *f);
FILE *pas_fopen(const char *path, const char *mode,int line,char *f);
int pas_fclose(FILE *fp,int line,char *f);
int pas_open(const char *pathname, int flags,int mode,int line,char *f);
int pas_accept(int iSock,char *s,int *iLen,int line,char *f);
int pas_close(int iFd,int line,char *f);
void pas_utMsgFree(char *psMsgHead,int line,char *f);
char *pas_utMsgInit(unsigned long  lCode,unsigned long lFunCode,
                        unsigned long lMsgId,unsigned long lMaxBytes,int line,char *f);
int pas_socket(int domain, int type, int protocol,int line,char *f);
/* pasutl_proc.c */
int pasUtlInitProcess(utShmHead *psShmHead,int iLnk,int iMaxFun);
int pasUtlSetProcessName(utShmHead *psShmHead, char *pName, 
                        int (*fFunName)(utShmHead *),
                        unsigned long  lStepTime,
                        unsigned long  lOnTime);
int pasUtlSetProcessNameD(utShmHead *psShmHead,
                        char *pName,
                        char *pDef,
                        int (*fFunName)(utShmHead *),
                        unsigned long  lStepTime,
                        unsigned long  lOnTime);
char *pasUtlGetMyProcessName(utShmHead *psShmHead);
int pasUtlStopOneProcess(utShmHead *psShmHead,char *pName);
int pasUtlStartOneProcess(utShmHead *psShmHead,char *pName);
int pasUtlSetCheckLicense(int iFlags);
int pasUtlStartProcess(utShmHead *psShmHead);
int pasProcessCheck(utShmHead *psShmHead);
int pasUtlRestartProcess(utShmHead *psShmHead);
int pasUtlInitOntimeFun(utShmHead *psShmHead,int iLnk,int iMaxFun);
int pasUtlSetOntimeFun(utShmHead *psShmHead, char *pName, int (*fFunName)(utShmHead *),
              unsigned long lStepTime,
              unsigned long lOnTime);
int pasUtlStartOnTimeFun(utShmHead *psShmHead);
unsigned long  pasUtlGetThatDays(unsigned long lTime);
unsigned long  pasUtlGetThatDayTime(unsigned long lTime);
int pasWebShowProcess(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead,char *pPlate,char *pErrPlate);
int pasWebShowOnTime(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead,char *pPlate,char *pErrPlate);
/* pasutl_processcheck.c */
int pasUtlChkProCpu(char *pConfig);
/* pasutl_py.c */
int pasPyLoad(char *pIn);
char *pasCvtGb2PyOneWord(char *pIn,char *pOut);
char *pasCvtGb2Py(char *pIn,char *pOut);
int pasPyFree();
int pasUtlCvtPyDat(char *pIn,char *OutFile);
/* pasutl_sendhttp.c */
int pasSendHttpGet(char *pHost,char *pUrl,char *pOut,char *pTitle);
/* pasutl_str.c */
char *pasStrReplaceWith(char *pBuf,char *pSrc,char *pDst,int *iCount);
/* pasutl_syscheck.c */
int pasSysCheckDataBase(char *pConfig,char *pMessage);
int pasSysCheckTcpServer(utShmHead *psShmHead,char *pMessage);
int pasSysCheckHttp(unsigned short nPort,char *pKey,char *pMessage);
int pasSysCheckDBServer(char *pUser,char *pPass,char *pDbname,char *pMessage);
char *pasUtlCvtSqlSelect(char *pSmt);


/* pasutl_file.c */
pasFileList *pasUtlGetFileListFromPath(char *pFold,char *pKey,int *iSumFile,int iFlags);
/* pasutl_radius.c */
int pasRadiusSetDebug(int iFlags);
int pasRadiusSetFlags(int iRetry,int iTimeOut);
int pasRadiusAuth(char *pIp,char *pKey,char *pUser,char *pPass,char *pAuthType,char *pFramedIp,char *pCalling,char *pCalled,
                     int *lSessTimeOut,int *lIdleTime);

/* pas_auth.c */
int pasAuthByPop3(unsigned long lIp,unsigned long lPort,char *pUser,char *pPass);

/* pas_log.c */
int pasSetLogPath(char *pPath);
int pasSetMaxLogSize(unsigned long lMax);
int pasLogOutType(char *pType);
int pasSysLog(int iType,char * sFormat, ...);
int pasSysDebugLevel(int deblevel);
int pasSetLogServer(char *pLogserver,unsigned short nPort);
int pasSysLogInit(int loglevel,char *pLogserver,unsigned short nPort);
void pasSysLogA(char *pFile,char *pFormat,...);
void pasLog(int iLevel,char *pFormat,...);
void pasDebugLogL(int iLine,char *pFile,int iLevel,char *pFormat,...);
void pasLogs(int iFile,int iLevel,char *pFormat,...);
void pasLogsTxt(int iFile,int iLevel,char *pMsg,char *pFormat,...);
void pasLogsBin(int iFile,int iLevel,unsigned char *pMsg,int iLen,char *pFormat,...);
void pasLogsHex(int iFile,int iLevel,unsigned char *pMsg,int iLen,char *pFormat,...);
int pasLogsIsOut(int iFile,int iLevel,char *pFile);
FILE *pasLogvFp(int iFile,int iLevel,char *pFormat,va_list args);
void pasDebugLog(int iLevel,int row,char *sFile,char *pFormat,...);
void pasSetLogLevel(int iLevel);
void pasSetLogSecurity(int iSec);
void pasSetLogFile(char *pFile);
void pasPutErrorMsg(int iCode,char *pFormat,...);
int pasGetLastErrorCode();
char *pasGetLastErrorMsg();
char *pasGetFreeFileName(char *pFile);
int pasIsInDebuglist(unsigned long  *lLevel,char *pFile);
int pasIsInDebuglist2(int iFile,int iLevel,char *pFile);
int pasSetDebugLogList(int iFile,int iLevel,char *pFile);
int pasSetDebugLogInfo(char *pStr);
/* pasmd5.c */
int utFilGenMd5(char *pConfig,char *pMd5);
int utFilCompare(char *pConfig,char *pMd5,char *pChange);
int utFilGetChange(char *pDir,char *pChange);
int utFilStrDo(char *pIn,int iDo,char *pSub);
int utFilGenList(char *pConfig,char *pChange);
/* pasnet01.c */
int pasNetGetMacFromIp(unsigned long lSip,char *mac,char *name,char *pDev);
int pasNetGetMacFromArp(char *pDev,unsigned long lSip,char *mac);
int pasNetGetMacFromWins(unsigned long lSip,char *mac,char *name,int iTimeout);
int pasNetSendWinsRequest(int iSock,unsigned long lSip);
int pasNetRecvWinsResponse(int iSock,unsigned long *lSip,char *mac,char *name);
/* pasbase641.c */
int pasBase64_setbz(char  c62,char c63,char cterm);
int pasBase64_decode(const char *p,char *result);
int pasBase64_encode(const char *decoded_str,char *result);
int pasBase64_encode_bin(const char *data, int len,char *result);
/* pasutl_mobile.c */
int pasCheckMobileNumber(char *pStr,char *pSpname,char *pType);
int pasUtlFreeMobileSp();
int pasUtlLoadMobileSp(char *pFile);
int pasUtlLoadMobileSpFromTxtFile(char *pFile,uint4 lMaxType, uint4 lMaxSeg, char *pOutFile);
int pasUtlMobileOutSp(char *pOutFile);
int pasUtlFreeMobileArea();
int pasUtlLoadMobileArea(char *pFile);
int pasCheckMobileArea(char *pStr,char *pSpname,char *pType,char *pArea);
int pasUtlLoadMobileAreaFromTxtFile(char *pFile,char *pOutFile,uint4 lMaxType,uint4 lMaxNumber);
int pasUtlMobileOutNum(char *pOutFile);
int utStrIsMobileNumber(char *pStr);
int utStrIsForMobileNumber(char *pStr);
int pasUtlMobileAreaInit();
int pasUtlLoadForgin(char *pFile);

/* utocon01.c */
int utContinue();
/* utodes01.c */
int utDesInit(char *key);
void utDesSInit();
int utDesEncrypt(char *in,int iLen,char *out,char *key);
int utDesDecrypt(char *in,int in_len,char *out,char *key);
int utDesCvtAsc(unsigned char *pBin,int iLen,unsigned char *pAsc);
int utDesCvtBin(unsigned char *pAsc, int iLen,unsigned char *pBin);
int utDesGetData(unsigned char *in,unsigned char *out,int iLen);
int utDesEncryptA(unsigned char *in,unsigned char *out,unsigned char *key);
int utDesDecryptA(unsigned char *in,unsigned char *out,unsigned char *key);
int utDesEncrypt_CbcPkcs7(unsigned char *from, int nLength,  unsigned char *to, unsigned char key[],unsigned char iv[]);
int utDesDecrypt_CbcPkcs7(unsigned char * from, int nLength,  unsigned char * to, unsigned char key[], unsigned char iv[]);
/* utodes02.c */
int utDesEncryptFile(char *pInFile,char *pOutFile,char *pKey);
int utDesDencryptFile(char *pInFile,char *pOutFile,char *pKey);
char *utDesDencryptFile2Buf(char *pInFile,char *pKey);
/* utodes03.c */
void utDesCBCInitKey(char *InputKey);
void utDesCBCInitIv(char *init);
int utDesCBCEncrypt(char *MingWen,int iLen,char *out,char *init,char *InputKey);
/* utoerr01.c */
void utErrSetCode(int code);
void utErrSetStatus(int iProject,int iFun,int iType,int iError);
/* utoerr02.c */
void utSetErrLevel(int iFlags,char *pFile);
void utSetErrCode(int code,char *pFormat,...);
char *utGetErrMsg(void);
/* utoexp01.c */
int utExpSetVariableFun(int (*f)(char *pHead,char *pVarname,double *dResult));
long utExpCompute(char *pHead,char *express,double *result);
long utExpLogicalCompute(char *pHead,char *express);

/* utolog01.c */
int utLogSetDebugFlags(int iFlags);
int utLogDebugIs();
char *utLogDebugFile();
int utLogSetDebugFile(char *pDebugFile);
int utLogSetFile(short nFileName, char *pFileName);
void utLogPrint(short nFileName,char *pFormat, ...);
void utLogError(char *pFormat,...);
void utLogError1(char *pFormat,...);
void utLogOut(char *pFormat,...);
void utSetLogFile(char *pFileName);
void utSetErrorFile(char *pFileName);
void utSetDebug(int iFlags);
int utSysLogInit();
int utSysLog(char * sFormat, ...);
void utLogMsg(char *pFile,char *pFormat,...);
/* utomd501.c */
void utMd5(unsigned char  *input,unsigned int inlen,
        unsigned char  *output);
char *utMd5Encrypt(unsigned char  *pInput, int iInLen,unsigned char  *pOutput);
char *utMd5Encrypto(unsigned char  *pInput, int iInLen,unsigned char  *pOutput);
void utMd5_Code(unsigned char  *input,unsigned int inlen,unsigned char *pKey,
        unsigned char  *output);
char *utMd5Ascii22(unsigned char  *pInput,int iInLen,char *pKey,unsigned char  *pOutput);
char *utMd5Ascii16(unsigned char  *pInput,int iInLen,char *pKey,unsigned char  *pOutput);
int utMd5File(char *caFileName,char *caMd5);
void utMd5Compute(char *caMd5,int iSum,...);
void utMd5ComputeArgs(char *caMd5,int iSum,va_list args);
int utCvtFileMd5Asc(char *caFileName,unsigned char *caMd5);
void utComputeMd5Asc(char *caMd50,int iSum,...);
/* utomd502.c */
unsigned long utMd5Code(char *pInput,int iLen,char *pKey);
int utMsgSetCheckSum(utMsgHead *psMsgHead,char *pKey);
int utMsgCheckSum(utMsgHead *psMsgHead,char *pKey);
int utMsgDesEncrypt(utMsgHead **psMsgHead0,char *pKey);
int utMsgDesDecrypt(utMsgHead **psMsgHead0,char *pKey);
int utFsMd5(char *caFileName,char *caMd5);
int utMsgTeaEncrypt(utMsgHead *psMsgHead,char *pKey);
int utMsgTeaDecrypt(utMsgHead *psMsgHead,char *pKey);
int utMsgTeaDecrypt1(utMsgHead *psMsgHead,char *pKey);
unsigned char *utStrCvtMd5(unsigned char *pInput,char *pMd5);

/* pasbase64.c */
int pasBase64_decode5(const char *p,char *result);
int pasBase64_encode5(const char *decoded_str,char *result);
int pasBase64_encode_bin5(const char *data, int len,char *result);
/* pascom01.c */
utMsgHead *pasTcpReadMsgT(int iSock,int iTimeOut,char *pKey);
utMsgHead *pasTcpReadMsg(int iSock,char *pKey);

/* pascrc32.c */
uint32_t pasCrc32(const void *buf, size_t size);
/* pasftp.c */
int pasFtpConnect(char *server,int port,char *pUser,char *pPass);
int pasFtpSend(const char *pInFile,const char *pFile,int cType);

/* pastea.c */
typedef  int4  word32;

void pasTea_tean(word32 *k, word32 *v, long N);
void pasTeaEncBlock(word32 *k, word32 *v);
void pasTeaDecBlock(word32 *k, word32 *v);
int pasTeaEnCode(char *pKey,char *pBuf,int iLen);
int pasTeaDeCode(char *pKey,char *pBuf,int iLen);
/* pastime.c */
int pasTcpSetOntimeDo(int fFunName(char *), long lTimeOut,char *pData);
int pasUdpSetOntimeDo(int fFunName(char *), long lTimeOut,char *pData);
int pasSetOntimeDo(int fFunName(char *), long lTimeOut,char *pData);
int pasSetOntimeDo0(int fFunName(char *), long lTimeOut,char *pData,int iRunFlags);
int pasOnTimeDo(utShmHead *psShmHead);
int utFunTcpOnTimeDo(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int utFunUdpOnTimeDo(utShmHead *psShmHead, utComSockAddr *psSockAddr,utMsgHead *psMsgHead);
/* utcvt01.c */
/* utcvt02.c */
char *pasBig5Togb2312(unsigned char *pStr);
char *pasBig5Togbk(unsigned char *pStr);
char *pasBig5Togbk_n(unsigned char *pStr,int iLen);
/* utcvt03.c */
char *pasGbkTobig5(unsigned char *pStr);
char *pasGbkTobig5_n(unsigned char *pStr,int iLen);
int pasGbkToBig5Head(unsigned char *inStr,int iLen);
int utCvtGbToBig5(unsigned char *inStr,int iLen);
int utCvtGbToBig5Head(unsigned char *inStr,int iLen);
/* utfile01.c */
unsigned long utFilFileSize(FILE *fp);
long utFileSize(char *pFile);
int utFileIsExist(char *pFile);
int utFilFilCopy(char *pSfile,char *pDfile);
int utFilFilMove(char *pSfile,char *pDfile);
int utFilFilAdd(char *pSfile,char *pDfile);
int utFileRename(char *pSfile,char *pDfile);
int utFilDeleteDir(char *pDir,
                 unsigned long lStartTime,
                 unsigned long lStopTime);
int utFilDeleteDirByName(char *pDir,char *pStart,char *pStop);
int utFilDeleteDir_1(char *pDir,
                 unsigned long lStartTime,
                 unsigned long lStopTime);
int utFilRmDir(char *pDir);
int utFilDeleteDir_2(char *pDir,
                 char *pKeepList,
                 unsigned long lStartTime,
                 unsigned long lStopTime);
int utFsGetDiskSpaceD(char *pDev,long *lTotal,long *lUsed,long *lFree);
int utFsGetDiskSpaceF(char *pMount,long *lTotal,long *lUsed,long *lFree);
int utFilFilCopyF(char *pSfile,char *pDfile);
int pasFileWriteTxt(FILE *fp,unsigned char *pBuf,int iLen);
int pasFileWriteHex(FILE *fp,unsigned char *pBuf,int iLen);
FILE *pasFopen(char *pFile,char *Opt);
char *pasFgets(char *buf,int iMax,FILE *fp);
int pasFCurRow();
int pasStrIsSpaces(char *buf);
int pasFileSetFileLock(FILE *fp);
int pasFileTestFileLock(char *caFile,int iFLags);
int pasFileSetFileUnLock(FILE *fp);
int pasFileSetFdLock(int iFd);
int pasFileSetFdUnLock(int iFd);
int pasUtlMkdir(char *caPath);
char *pasUtlGetBackFileName999999(char *caFile,char *caNewName);
char *pasUtlGetBackFreeFileName(char *caFile,char *caNewName);
/* utmil01.c */
int utMilSendTo(char *pFrom,char *pTo,char *pCC,char *pSub,char *pContent,int iFlags);
/* utocnf01.c */
int utCnfReadConfig(char *pHand,char *caConfigFile,
               int utCnfSaveConf(char *p1,char *pVar,char *pVal));
int utCnfGetValue_l(char *pFile,char *pVarName);
int utCnfGetValue_s(char *pFile,char *pVarName,char *pVarValue);
int utCnfLookVar(char *pHand,char *pVarName,char *pVarValue);
char *utCnfGetConfig(utShmHead *psShmHead,short nLnkNo,
                   char *pVarName);
long utCnfGetConfig_l(utShmHead *psShmHead,short nLnkNo,
                   char *pVarName);
int utCnfCompareKey(char *pSource,char *pKey);
int utCnfSetDebug(utShmHead *psShmHead);


#endif
