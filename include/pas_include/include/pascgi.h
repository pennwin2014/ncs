/* Pas Cgi 
   Write by Li Yunming
 */

#ifndef UTDEFCGI_DEF
#define UTDEFCGI_DEF
#include <stdarg.h>
#define UT_CGI_MAXSERVER      8
#define UT_CGI_SERVER_NAME    0
#define UT_CGI_USER_AGENT     1
#define UT_CGI_SERVER_PORT    2
#define UT_CGI_HTTP_REFERER   3
#define UT_CGI_REMOTE_ADDR    4
#define UT_CGI_REQUEST_URI    5
#define UT_CGI_REMOTE_USER    6
#define UT_CGI_AUTH_TYPE      7
#define UT_CGI_REMOTE_PORT    8
#define UT_CGI_REMOTE_IDENT   9

typedef struct  {
  char  *name;             /* 名字  */
  char  *val;              /* 值    */
} utCgiEntry;


typedef struct  {
  char  *name;             /* 名字  */
  char  *val;              /* 值    */
  int   iLen;              /* 长度  */
  int   iType;             /* 类型  0--一般变量  1--文件字符   2---文件结构  */
} utCgiEntryU;

typedef struct {
    long   lIpAddress;    /* IP地址      */
    short  nPort;         /* 端口号      */
    short  nFlags;        /* 服务器状态 0 --正常  1--不正常  */
} utCgiServer;

typedef struct {
    short nSumServer;                               /* 服务器总数              */
    short nCurServer;                               /* 当前服务器              */
    short nSumProxy;
    short nCurProxy;
    utCgiServer    sCgiServer[UT_CGI_MAXSERVER];    /* 服务器信息              */
    utCgiServer    sProxy[UT_CGI_MAXSERVER];                          /* 代理服务器信息              */
    long  lTimeOut;                  /* 超时时间                */
    char           caUpLoadPath[64]; /* 上载文件存放的目录      */
    long  lMsgCode;                  /* 消息标识                */
    long  lMaxTcpPkg;                /* Tcp最大数据包           */
    long  lShmKey;                   /* 共享内存标识,当为0时,不用    */
    char  caSendEnv[16];              /* 环境变量传递           */ 
    int   iDebug;                    /* iDebug ==1 显示调试信息 0--不显示调试信息 */
    int   lMaxRequest;
} utCgiConfInfo;
     
typedef struct {
    utCgiConfInfo  *sCgiConf;         /* 配置文件信息  */
    int    iSumVar;                  /* 最大变量数    */
    int    iSumCookies;              /* 最多Cookies   */
    utCgiEntry *entries;           /* 变量信息      */
    utCgiEntry *cookies;           /* Cookies信息   */
} utFcgiHead;


char *utCgiMakeWord(char *line, char stop);
char utCgiX2C(char *what);
void utCgiUnEscapeUrl(char *url);
void utCgiPlusToSpace(char *str);
utCgiEntry *utCgiRetrieveArgs(int *iSumVar);
utCgiEntry *utCgiRetrieveCookie(char *cpGetCookie,int *iSum);
char *utCgiMsgErr(char *errorMsg);
int utCgiReadConfig(char *caConfigFile,utCgiConfInfo *psCgiConf);
int utCgiGetIpPort(char *caIp,long *lIpAddress,short *nPort);
int utCgiFreeHead(utFcgiHead *psCgiHead);
int utCgiConnect(utCgiConfInfo *psCgiConf);
int utCgiPrintf(char *pFormat,...);
int utCgiFreeEntries(utCgiEntry *psEntries,int iSum);

char *utCgiMakeWord(char *line, char stop);
char *utCgiFmakeWord(FILE *fp, char stop, int *length);
char utCgiX2c(char *what);
void utCgiUnEscapeUrl(char *url);
void utCgiPlusToSpace(char *str);
utCgiEntry *utCgiRetrieveArgs(int *iSumVar);
utCgiEntry *utCgiRetrieveCookie(char *cpGetCookie, int *iSum);
char *utCgiMsgErr(char *);
int utCgiFreeEntries(utCgiEntry *psEntries,int iSum);
int utCgiPrintf(char *pFormat,...);
int utCgiDo(utCgiConfInfo *psCgiConf);
int utCgiSendEnvTo(char *pEnvMark,utMsgHead **psMsgHead);
/*  utocgi02.c  */
utCgiEntryU *utCgiUpLoad(FILE *fp,char *pTempPath,int *iSumVar);
int utCgiFreeEntriesU(utCgiEntryU *psEntries,int iSum);
int utCgiFgets(char *pBuffer,int iMaxLen,int *iReadLen,FILE *fp);
int utCgiReadUntilF(FILE *fpi,FILE *fpo,char *pTerm,int iLen);
int utCgiReadUntilS(FILE *fpi,char *pBuffer,int *iReadLen,char *pTerm,int iLen);
char *utCgiReadUntilSP(FILE *fpi,int *iReadLen,char *pTerm,int iLen);
int utCgiFReadHead(FILE *fp,char *pBuffer);
int utCgiAnalyLine(char *pBuffer,char *caVarName,char *caFileName,char *caContentType);
int utCgiCvtSaveFile(char *caFileName,char *pTempPath,char *caSaveName);
int utCgiDoUpload(utCgiConfInfo *psCgiConf);
#endif
