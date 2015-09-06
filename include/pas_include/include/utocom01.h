#ifndef __UTOCOMDEF__
#define __UTOCOMDEF__
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdarg.h>

/* Data in Share Memory Link */
#define UT_LNK_CONF        0      /* ÅäÖÃÐÅÏ¢   */
#define UT_LNK_RECV        1      /* UDP½ø³Ì    */
#define UT_LNK_PROCESS     2      /* Tcp½ø³Ì    */
#define UT_LNK_ROLEINFO    3      /* ½ÇÉ«ÐÅÏ¢   */
#define UT_LNK_CONFIRM     4      /* ÒªÈ·ÈÏµÄÐÅÏ¢       */
#define UT_LNK_SHARE       5      /* ¹²ÏíÊý¾Ý           */
#define UT_LNK_DSCLIENTS   6      /* Ó¦ÓÃ·þÎñÆ÷´æ·ÅµÄÁ´±í, Ó¦ÓÃ·þÎñÆ÷±£´æÔÚÒ»¸ö¾²Ì¬Êý×éÖÐ  */
#define UT_LNK_MYSTATUS    7      /* ·þÎñÆ÷±¾ÉíµÄ×´Ì¬    */
#define UT_LNK_TCPFUNLIST  8      /* Tcp¹¦ÄÜ´úÂë         */
#define UT_LNK_UDPFUNLIST  9      /* Udp¹¦ÄÜ´úÂë         */
#define UT_LNK_DSSRVUSERINFO    10     /* ÓÃ»§ÐÅÏ¢(Ä¿Â¼·þÎñÆ÷)    */
#define UT_LNK_USERATTR      11     /* ÓÃ»§ÐÅÏ¢(·þÎñÆ÷¶Ë)  */
#define UT_LNK_PROXYSRVINFO  11     /* pas proxy ·þÎñÆ÷¹²ÏíÄÚ´æ */
#define UT_LNK_PROXYCLTINFO  12     /* pas proxy ¿Í»§¶Ë¹²ÏíÄÚ´æ */
#define UT_LNK_NETCONFIG   12     /* ÍøÂçÅäÖÃ            */
#define UT_LNK_DICTIONARY  13     /* Êý¾Ý×Öµä            */
#define UT_LNK_MYSRVFRIEND 14     /* ¿Í»§¶ËµÄ¹ØÁª·þÎñÆ÷(¿Í»§¶Ë)  */
#define UT_LNK_MYFRIEND    15     /* ÎÒµÄÍøÓÑ            */
#define UT_LNK_SYNCCLIENT  16     /* Í¬²½·þÎñÆ÷ÖÐÍ¬²½µÄ¿Í»§¶ËÐÅÏ¢    */
#define UT_LNK_SYNCSERVER  17     /* Í¬²½¿Í»§»úÖÐÍ¬²½µÄ·þÎñÆ÷ÐÅÏ¢    */
#define UT_LNK_SESSIONVALUE 18    /* SessionµÄÄÚÈÝ£¬ºÍÓÃ»§ÐÅÏ¢Ïà¶ÔÓ¦  */
#define UT_LNK_SIDTABLE     19

/* Default Configurate Data */
#define UT_COM_TCPACCEPT       8
#define UT_COM_TCPCONNECT      5
#define UT_COM_TCPDOPROCESS    4

#define UT_COM_TIMEOUT       60
#define UT_COM_TCPPORT       8089
#define UT_COM_MAXPROCESS    100
#define UT_COM_UDPACCEPT     4
#define UT_COM_UDPDOPROCESS  4
#define UT_COM_MAXSHARE      10
#define UT_COM_MAXUDPFUNCODE 100
#define UT_COM_MAXTCPFUNCODE 100
#define UT_COM_BUSY         1
#define UT_COM_FREE         0
/* Process Type */
#define UT_COM_DOACCEPTTCP     1
#define UT_COM_DOACCEPTUDP     2
#define UT_COM_DOTCPREQUEST    3
#define UT_COM_DOUDPREQUEST    4
#define UT_COM_AUTOSTATUS      5
#define UT_COM_DOCHECK         6
#define UT_COM_DOTIMEOUT       7
#define UT_COM_PROCESSMGR      8     /* ½ø³Ì¹ÜÀí  */
#define UT_COM_FASTCGI         9     /* Fast Cgi  */
#define UT_COM_TCP          1
#define UT_COM_UDP          2

#define UT_TCP_CONFIRMMSG   113
#define UT_TCP_STOPMSG      111

/* ²ÎÊý±£´æ  */
#define UT_SHA_TCPPROCESS   0      /* TCPÖ÷½ø³Ì     */
#define UT_SHA_UDPPROCESS   1      /* UDPÖ÷½ø³Ì     */
#define UT_SHA_TIMEPROCESS  2      /* ¶¨Ê±´¦Àí½ø³Ì  */
#define UT_SHA_LASTTIME     3      /* ×îºóÒ»´ÎÊÕµ½ÏûÏ¢Ê±¼ä Ä¿Ç°²»ÓÃ  */
#define UT_SHA_UDPPORT      4
#define UT_SHA_TCPPORT      5
#define UT_SHA_DEBUG        6      /* Debug ÐÅÏ¢  0--²»  1--´¦ÔÚDebug×´Ì¬  */
#define UT_SHA_BASETIME     7      /* Ê±¼ä»ùÊý                             */
#define UT_SHA_CURUID       8      /* Éú³ÉµÄIDÖµ                           */

#define UT_TRAN_SHM         0
#define UT_TRAN_TCP         1
#define UT_TRAN_FILE        2
#define UT_FUN_TEST         1      /* ²âÊÔÃüÁî  */
#define UT_COM_MAXSOCKET    100
/* Udp Accept Buffer */
#define UT_COM_MAXBUFFER    4096
#define UT_COM_TCPBUFFER    4096
#define UT_COM_TCPTIMEOUT   30
#define UT_COM_TCPREADTIMEOUT 30
typedef struct {
    unsigned short   nPort;    /* ¶Ë¿ÚºÅ  */
    short            nSum;     /* ×ÜÊý    */
    long             lOffset;  /* Æ«ÒÆÁ¿  */
} utTcpSockTable;


typedef struct {
    long  lPid;
    long  lStartTime;
    long  lLastTime;
    long  lCount;
    long  nStatus;   /* 0 Free   1 Busy */
    long  nFun;
} utComProcess;

/* ÓÃÓÚÊý¾Ý´«µÝµÄ½Ó¿Ú  */
typedef struct {
    short nStatus;   /* 1 ---Êý¾Ý´æ·ÅÓÚ¹²ÏíÄÚ´æÖÐ
                        2 ---Êý¾Ý´æ·ÅÓÚÎÄ¼þÖÐ,ÎÄ¼þÃ÷´æ·ÅÔÚ¹²ÏíÄÚ´æÖÐ
                        3 ---Êý¾ÝÍ¨¹ýTcp´«µÝ, nPageNoÎª¶Ë¿ÚºÅ lOffsetÎªIpµØ·
                      */
    short nPageNo;
    long  lOffset;
} utComShm;

typedef struct {
    int      iResend;
    int      iUdpSock;
    unsigned long lTimeOut;
    int      iSumNum;    /* »ØÊÕµÄÊýÁ¿  */
    int      iMaxNum;    /* ×î´óÊýÁ¿    */
    long     *lMsgId;    /* MsgId       */
    utShmHead *psShmHead;
} utUdpCheck;

typedef struct {
    uint4   lSIp;      /* Server Ip   */
    uint2   nSPort;    /* Server Port */
    uint2   nCPort;    /* Client Port */
} utComIpAddr;



typedef struct  {
    int iSock;
} utComSock;

typedef struct  {
    long        lMaxSock;
    utComSock   *psComSock;
} utComSockHead;


typedef struct  {
    struct sockaddr_in  sSin;
} utComSockAddr;

/* ³¬Ê±´¦Àíº¯Êý  */

typedef struct  {
    int iSum;   /* ×Ü¹²ÊýÁ¿  */
    int iMax;   /* ×î´óÔÊÐíÊýÁ¿  */
    struct utComTime_S {
        long lTimeOut;    /* ³¬Ê±¼ä¸ô      */
        long lLastTime;   /* ÉÏ´Î·ÃÎÊÊ±¼ä  */
        char *pData;      /* ´«µÝ²ÎÊý      */
        int   (*fFunName)(char *pData);   /* ³¬Ê±´¦Àíº¯Êý  */
    } *psTime;
} utComTimeOutFun; 

  
 



typedef struct  {
    long  nFunCode;      /* ¹¦ÄÜ´úÂë                       */
    char  caFunName[32]; /* FunName                        */
    short nCheckFlags;   /* 0--Pass 1--Auth  2--CheckRight */
    short nUseFlags;     /* 0 -- Normal   1--Can't Use     */
    int   (*fFunName)(utShmHead *,int,utMsgHead *);
} utComTcpFunList;

    
typedef struct  {
    short nMaxFun;
    short nNumFun;
    utComTcpFunList *psFunList;
} utComTcpFun;

typedef struct  {
    long   nFunCode;
    int   (*fFunName)(utShmHead *,utComSockAddr *,utMsgHead *);
} utComUdpFunList;
    
typedef struct  {
    short nMaxFun;
    short nNumFun;
    utComUdpFunList *psFunList;
} utComUdpFun;


typedef struct pasConfList_s {
    int  iFlags;            /* 0--Î´ÐÞ¸Ä  1--ÒÑÐÞ¸Ä  */
    char *pVar;             /* ±äÁ¿Ãû                */
    char *pValue;           /* ±äÁ¿µÄÄÚÈÝ            */
    struct pasConfList_s *psNext;
} pasConfList;    
    
/* The Value of nActiob */
#define UTCOM_ACT_RETURNCLOSE      16  /* 10000   ·µ»ØÔ­Ïà¹ØÁ¬½Ó, ¹Ø±ÕËùÓÐÁ¬½Ó    */
#define UTCOM_ACT_RETURNKEEPA       6  /*  0110   ·µ»ØÔ­Ïà¹ØÁ¬½Ó, ±£ÁôÔ­,ÄÚ²¿Á¬½Ó */
#define UTCOM_ACT_RETURNKEEPI       4  /*  0100   ·µ»ØÔ­Ïà¹ØÁ¬½Ó, ±£ÁôÔ­Á¬½Ó    */
#define UTCOM_ACT_REDIRCLOSEA       8  /*  1000   ÖØ¶¨Ïò, ¹Ø±ÕËùÓÐÁ¬½Ó  */
#define UTCOM_ACT_REDIRKEEPFO       13 /*  1101   ÖØ¶¨Ïò, ±£ÁôÔ­,Ä¿±ê   */
#define UTCOM_ACT_REDIRKEEPIO       11 /*  1011   ÖØ¶¨Ïò, ±£ÁôÄÚ²¿,Ä¿±ê */
#define UTCOM_ACT_REDIRKEEPF        12 /*  1100   ÖØ¶¨Ïò, ±£ÁôÔ­        */
#define UTCOM_ACT_ENDPROCESS       999 /*  ÖÕÖ¹½ø³Ì           */






/* Tcp BufferÏûÏ¢ºóµÄÄÚÈÝ  */
typedef struct {
    short    nNum;            /* ¶ÓÁÐÖÐµÄÐòºÅ       */
    unsigned short    nPort;  /* ÄÚ²¿½ø³Ì¶Ë¿ÚºÅ     */
    long     lConnectId;      /* Á¬½Ó±êÖ¾           */
    long     lReserve;        /* ±£Áô                */
} utTcpSInfo;



/* ×ª·¢µÄTcpµØÖ·  */
typedef struct {
    unsigned long  lAddress; /* IPµØÖ·, ÍøÂç×Ö½ÚË³Ðò  */
    unsigned short nPort;    /* ¶Ë¿ÚºÅ, ÍøÂç×Ö½ÚË³Ðò  */
    unsigned short nFlags;   /* ±£Áô */
} utTcpAddress;

typedef struct {
    int      iSock;     /* Sock Id                                  */
    unsigned char    cKind;     /* 0--Íâ²¿  1--ÄÚ²¿                */
    unsigned char    cReserve;  /* ±£Áô                            */
    unsigned char    cFlags;    /* 0--Õý³£  ÕýÔÚµÈ´ýÓ¦´ð           */
    unsigned char    cType;  /* 0 Õý³£  1 ´óÊý¾Ý°ü  2 ¶àÊý¾Ý°ü     */
    long     lTime;       /* ×îºó²Ù×÷Ê±¼ä   */
    long     lIpAddress;  /* IPµØÖ·         */
    char     *pData;      /* Êý¾ÝÖ¸Õë       */
    long     lConnectId;  /* Á¬½ÓId, ÔÚÁ¬½Ó½¨Á¢Ê±²úÉú  */
} utSockQuen;

#define UT_QUEN_OUTSOCK        0
#define UT_QUEN_INSOCK         1     
/* cReserve */
#define UT_COM_MULTPKG    2
#define UT_COM_BIGPKG     1

/* cFlags */
#define UT_COM_WAITCONFIRM  1
#include "pasutl.h"
#include "pasdef.h"


/* utocom01.c */
int utComSetIpAddr(utComSockAddr *psSockAddr,
                 const char *pHost,const char *pService,int iFlags);
int utComSetAddr0(utComSockAddr *psSockAddr,
                long lIpAddr,short nPort);
int utComSetPort0(utComSockAddr *psSockAddr,
                  short nPort);
unsigned long  utComHostAddress(const char *pHost);
long pasIpcvtLong(const char *pHost);
char *pasIpcvtStr(long lIp,char *pIp);
char *utComHostIp(long lIp);
char *utComHostIpF(unsigned long lIp);
short utComServicePort(const char *pServices,int iFlags);
int utComTcpConnect(const char *host,
                     const char *service);
int utComConnect(int iSock, utComSockAddr *psSock);
int utComTcpServer(const char *service, int iQlen);
int utComTcpServer0(const char *service, int iQlen);
int utComUdpServer(const char *service);
int utComTcpSock();
int utComUdpSock();
int utComClose(int iSock);
int utComTcpWrite(int iSock,char *pBuffer,int iLen);
int utComTcpAccept(int iSock, utComSockAddr *psSockAddr);
int utComTcpRead(int iSock,char *pBuffer,int iLen);
int utComUdpRecvFrom(int iSock,char *pBuffer,int iLen,utComSockAddr *psSin);
int utComUdpSendTo(int iSock,char *pBuffer,int iLen,utComSockAddr *psSin);
int utComGetCurAddr(utComSockAddr *psSin);
int utComTcpReadTT(int iSock,char *pBuffer,int iLen,int lTime);
int utComTcpReadTT2(int iSock,char *pBuffer,int iLen,int lTime);
int utComTcpReadT(int iSock,char *pBuffer,int iLen,int lTime);
int utComUdpRecvFromT(int iSock,char *pBuffer,int iLen,utComSockAddr *psSin,int lTime);
int utComGetSockIp(int iFd,unsigned long *lIp,unsigned short *nPort);
unsigned char *utComGetSockIpS(int iFd,char *pIp);
int utComUnixServer(const char *service, int iQlen);
int utComUnixConnect(char *service);
/* utocom03.c */
int utComInit(utShmHead *psShmHead,char *pConfFile);
int utComSetSValue(utShmHead *psShmHead,int iNumber,long lValue);
long utComGetSValue(utShmHead *psShmHead,int iNumber);
int utComSetVar_l(utShmHead *psShmHead,
                  char *pVarName,long lValue);
int utComSetVar_s(char *pHead,char *pVarName,
                    char *pVarValue);
char *utComGetVar_s(utShmHead *psShmHead,
                   char *pVarName);
char *utComGetVar_sd(utShmHead *psShmHead,char *pVarName,char *pDefault);
long utComGetVar_ld(utShmHead *psShmHead,char *pVarName,long lValue);
long utComGetVar_l(utShmHead *psShmHead,char *pVarName);
int utComDelVar(utShmHead *psShmHead,char *pVarName);
int utComCompareCnfKey(char *pSource,char *pKey);
void utComReaper(int iSig);
int utComSetBaseTime(utShmHead *psShmHead,long lTime);
unsigned long utComRealTime(utShmHead *psShmHead);
char *pasGetConfigFileName();
int pasSetConfigFileName(char *pConffile);
int pasSetDebugInfoFromCnf(utShmHead *psShmHead);
int pasReLoadConfig(utShmHead *psShmHead);
/* utocom03_1.c */
int utComGetUdpSock();
int utComSetUdpSock(int iSock);
int utComUdpResponseTo(utShmHead *psShmHead,utComSockAddr *psSockAddr,
        utMsgHead *psMsgHead0,int iConfirm,short nVarNum,...);
int utComUdpSendMsgTo(utShmHead *psShmHead,utComSockAddr *psSockAddr,
         int iConfirm,utMsgHead *psMsgHead);
int utComUdpSend(utShmHead *psShmHead, utComSockAddr *psSockAddr,
                utMsgHead *psMsgHead);
int utComUdpSendMsgToIp(utShmHead *psShmHead,unsigned long lIp,unsigned short nUdpPort,long lMsgCode,int nFunCode,
    long l0,long l1,int iConfirm,int iSumVar,...);
int utUdpSendMsgToIp(utShmHead *psShmHead,unsigned long lIp,unsigned short nUdpPort,
    utMsgHead *psMsgHead);
/* utocom03_2.c */
char *utComEncryptVar(char *pIn);
char *utComDecryptVar(char *pIn);
/* utocom04.c */
int utComSetMaxTcpBuf(long lMaxTcpBuf);
int utComTcpRequest(int iSock,long lMsgCode,
            long nFunCode,long lMsgId,short nVarNum,...);
int utComTcpResponse(int iSock,utMsgHead *psMsgHead0,short nVarNum,...);
int utComTcpResponse1(int iSock,utMsgHead *psMsgHead0,int iFlags,short nVarNum,...);
int utComTcpResponse3(int iSock,utMsgHead *psMsgHead0,short nVarNum,...);
int utComTcpMsgRead(int iFd,utMsgHead *psMsgHead);
int utComTcpMsgReadT(int iFd,utMsgHead *psMsgHead,long lTime);
int utComTcpMsgHeadRead(int iFd,utMsgHead *psMsgHead);
int utComTcpMsgHeadReadT(int iFd,utMsgHead *psMsgHead,long lTime);
int utComTcpReadVar(int iSock,utMsgVar *psMsgVar);
int utComTcpReadValue(int iSock,utMsgVar *psMsgVar,void *pVar);
utMsgHead *utComTcpReadMsg(int iFd);
utMsgHead *utComTcpReadMsgT(int iFd,long lTime);
int utComTcpSendControlMsg(int iSock,long nFunCode);
int utComUdpMsgRecvFrom(int iSock,utComSockAddr *psSockAddr,
                        utMsgHead *psMsgHead);
int utComUdpRequest(int iSock, utComSockAddr *psSockAddr, 
        long lMsgCode,long lFunCode,long lMsgId,short nVarNum,...);
int utComUdpResponse(int iSock,utComSockAddr *psSockAddr,
        utMsgHead *psMsgHead0,short nVarNum,...);
int utComTcpMsgRead000(int iFd,utMsgHead **psMsgHead0);
int utComTcpMsgReadT000(int iFd,utMsgHead **psMsgHead0,long lTime);
int utTcpConnect(const char *host,
                     const char *service);
int utTcpConnect0(unsigned long lIpAddress,unsigned short nPort);
int utTcpServer(const char *service, int iQlen);
int utUdpServer(const char *service);
int utTcpSock();
int utUdpSock();
int utTcpAccept(int iSock, utComSockAddr *psSockAddr);
int utUdpRecvFrom(int iSock,char *pBuffer,int iLen,utComSockAddr *psSin);
int utUdpSendTo(int iSock,char *pBuffer,int iLen,utComSockAddr *psSin);
int utTcpRead(int iSock,char *pBuffer,int iLen);
int utTcpWrite(int iSock,char *pBuffer,int iLen);
int utTcpReadMsg0(int iSock,utMsgHead *psMsgHead,
        unsigned long lMaxBytes);
int pasTcpMsgSend(int iSock,utMsgHead *psMsgHead,char *pKey);
int pasTcpMsgRead000(int iFd,utMsgHead **psMsgHead0,char *pKey);
int pasTcpMsgReadT000(int iFd,utMsgHead **psMsgHead0,long lTime,char *pKey);
int pasTcpMsgReadT000_1(int iFd,utMsgHead **psMsgHead0,long lTime,char *pKey);
int utComCompConfirmKey(char *pSource,char *pKey);
/* utocom05.c */
utShmHead *utComShmOpen(char *caConfFile);
utShmHead *utComShmReOpen();
utShmHead *utComShmCreate(char *caConfFile);
utShmHead *utComShmCreate777(char *caConfFile);
/* utocom07.c */
int utComInitUdpFun(int nMaxFun);
int utComAddUdpFun(int nFun);
int utComSetDefaultUdpFun(int utComUdpDoFun(utShmHead *,utComSockAddr *,utMsgHead *));
int utComSetUdpFun(int nFunCode, int utComUdpDoFun(utShmHead *,utComSockAddr *,utMsgHead *));
int utComFreeUdpFun();
int utComUdpDoRequest(utShmHead *psShmHead, utComSockAddr *psComSockAddr,
            utMsgHead *psMsgHead);
/* utocom09.c */
void utComDoTimeOut();
int utComCanStartTimeOut();
void utComDoTimeOut1(utShmHead *psShmHead);
int utComTimeControlExist();
int utComSetTimeOut(int fFunName(char *), long lTimeOut,char *pData);
int utComTimeControl(utShmHead *psShmHead);
int utComTimeControl1(utShmHead *psShmHead,
        int utComTimeInit(utShmHead *psShmHead),
        int utComTimeEnd(utShmHead *psShmHead));
int utComContinue(utShmHead *psShmHead);
int utComCheckPqOnline();
void utComCheckUdpTcp();
int utComRestartOnline(char *pConfFile);
int utComSetConfFile(char *pConfFile);
char *utComGetConfFile();
/* utocom10.c */
int utComTcpMsgSend(int iSock,utMsgHead *psMsgHead);
int utComUdpMsgSendTo(int iSock,utComSockAddr *psSockAddr,
                        utMsgHead *psMsgHead);
/* pastcp.c */
int pasTcpIsStart();
int pasSetPreDoTransactionFun(int (*psDoFun) (utShmHead *psShmHead));
int pasTcpServer(utShmHead *psShmHead,int utComTcpInit(utShmHead *psShmHead));
int pasTcpServiceRestart(utShmHead *psShmHead,int utComTcpInit(utShmHead *psShmHead));
int pasTcpServer0(utShmHead *psShmHead,int *iTcpSock,int utComTcpInit(utShmHead *psShmHead),int iThis,int iSum);
void utComSignal_fatal(int sig);
void utComSignal_hup(int sig);
int pasTcpMakeProcess(utShmHead *psShmHead,int *iFd,pasProcess *psProcess,
               int utComTcpInit(utShmHead *psShmHead),int iSumProcess);
int pasWriteFd(int fd, void *ptr, size_t nbytes, int sendfd);
int pasReadFd(int fd, void *ptr, size_t nbytes, int *recvfd);
int pasTcpDoRequest(int connfd,utShmHead *psShmHead,pasProcess *psProcess);
unsigned char *pasGetCurComKey();
int utComInitTcpFun(int  nMaxFun);
int utComAddTcpFun(int iFun);
int utComSetTcpFun(int iFunCode, int utComTcpDoFun(utShmHead *,int,utMsgHead *));
int utComSetTcpFunL(int iFunCode, int utComTcpDoFun(utShmHead *,int,utMsgHead *),int iMaxRunTime);
int utComSetDefaultTcpFun(int utComTcpDoFun(utShmHead *,int,utMsgHead *));
int utComFreeTcpFun();
int pasCheckProcess(utShmHead *psShmHead);
int pasReCountProcessAvail(pasProcess *psProcess,int iMaxProcess,int iMinProcess,int *iSumProcess);
utShmHead *pasServerInit(char *caConfFile);
utShmHead *pasServerInit777(char *caConfFile);
utShmHead *pasServerInit9(char *caConfFile);
int utTcpMultMsgSend(int iFd,utMsgHead *psMsgHead);
int utTcpSendMultMsg(int iFd,utMsgHead *psMsgHead);
int utTcpSendStopMsg(int iFd);
int utTcpSendConfirmMsg(int iFd);
int pasStopProcess(utShmHead *psShmHead);
int pasSetTcpFun(int iFunCode, int utComTcpDoFun(utShmHead *,int,utMsgHead *),char *pName,
               int iCheckSumFlags,int iSecretFlags, int iAuthFlags, int iMaxRunTime);
int pasSetTcpFunAttr(int iFunCode,char *pName,int iUseFlags,int iCHeckSumFLags,
            int iSecretFlags,int iAuthFlags,int iMaxRunTime);
int pasGetTcpFunNum(int iFunCode);
pasTcpFunList *pasGetTcpFunStr(int iFunCode);
int pasGetSumTcpFun();
int pasSetTcpFunCode(int iFunCode, int utComTcpDoFun(utShmHead *,int,utMsgHead *),int iAuthFlags);
int pasSetTcpFunName(char *pFunName, int utComTcpDoFun(utShmHead *,int,utMsgHead *),int iAuth);
int pasSetTcpFunNameS(char *pFunName, int utComTcpDoFun(utShmHead *,int,utMsgHead *),
                                      int utComTcpGetKey(utShmHead *,unsigned long,char *),int iAuth,int cSecret);
int pasSetTcpFunFlags(char *pFunName, int iNum,int iFlags);
int pasUtlCheckRight(int iFd,utShmHead *psShmHead,
                   utMsgHead *psMsgHead,
                   pasTcpFunList *psFun,
                   int  iNum);
int pasCheckSoftware(utShmHead *psShmHead,char *Prod);
int pasRunShell(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
char *pasDoSystem(char *caCommand);
int pasSetTcpPort(char *caPort,int *iTcpSock);
int pasCloseTcpPort(int *iTcpSock);
int pasSetMyPid(char *pFile);
int pasGetMyPid(char *pFile);
/* pastcp01.c */
int pasUtlInitAllowedList(utShmHead *psShmHead);
int pasUtlAuthIsAllowed(unsigned long lIp);
/* pastcp02.c */
utMsgHead *pasTcpRequest(unsigned long lIpAddress,unsigned short nTcpPort,
                int iCheckSum,int iCrypt,char *pFunName,
                unsigned long lSid,unsigned long lRid,
                unsigned long lTimeOut,
                char *pKey,int iSumVar,...);
utMsgHead *pasTcpRequest0(unsigned long lIpAddress,unsigned short nTcpPort,
                int iCheckSum,int iCrypt,unsigned long lFunCode,
                unsigned long lSid,unsigned long lRid,
                unsigned long lTimeOut,
                char *pKey,int iSumVar,va_list ap);
utMsgHead *pasTcpRequestMsg(unsigned long lIpAddress,unsigned short nTcpPort,
                unsigned long lTimeOut,
                utMsgHead *psMsgHead);
int pasMsgMd5CheckSum(utShmHead *psShmHead,utMsgHead *psMsgHead);
int pasTcpResponse0(int iFd,int iCheckSum,int iCrypt,unsigned long lFunCode,
                unsigned long lSid,unsigned long lRid,
                char *pKey,int iSumVar,...);
int pasTcpResponse(int iSock,utMsgHead *psMsgHead0,char *pKey,short nVarNum,...);
/* pasudp.c */
int pasUdpFunInit00();
int pasUdpServer(utShmHead *psShmHead,int utComUdpInit(utShmHead *psShmHead));
int pasUdpMakeProcess(utShmHead *psShmHead,int iFd,pasProcess *psProcess,
               int utComUdpInit(utShmHead *psShmHead),int iSumProcess);
int utComInitUdpFun(int  nMaxFun);
int utComAddUdpFun(int iFun);
int pasSetUdpFunName(char *pFunName, int utComUdpDoFun(utShmHead *,utComSockAddr *,utMsgHead *));
int utComSetUdpFun(int iFunCode, int utComUdpDoFun(utShmHead *,utComSockAddr *,utMsgHead *));
int utComSetUdpFunL(int iFunCode, int utComUdpDoFun(utShmHead *,utComSockAddr *,utMsgHead *),int iMaxTime);
int utComSetDefaultUdpFun(int utComUdpDoFun(utShmHead *, utComSockAddr *,utMsgHead *));
int utComFreeUdpFun();
int utFunUdpTest(utShmHead *psShmHead, utComSockAddr *psSockAddr,utMsgHead *psMsgHead);
int utFunUdpSetDebug(utShmHead *psShmHead, utComSockAddr *psSockAddr,utMsgHead *psMsgHead);
int pasUdpAddSendBytes(unsigned long lBytes);
int pasUdpAddRecvBytes(unsigned long lBytes);
int utComUdpResponse1(utShmHead *psShmHead,utComSockAddr *psSockAddr,
        utMsgHead *psMsgHead0,int iConfirm,short nVarNum,...);
int utComUdpRequest1(utShmHead *psShmHead, utComSockAddr *psSockAddr,
        long lMsgCode,long nFunCode,long lMsgId,int iConfirm,short nVarNum,...);
int utComUdpSend0(utShmHead *psShmHead,
        unsigned long lIpAddr,unsigned short nPort,
        utMsgHead *psMsgHead);
int utComUdpResend(char *psShmHead0);
int utComUdpCheck(char *pHead,char *pValue);
int utComUdpSetTimeOut(int   (*utTimeOutFun)(utShmHead *,utComSockAddr *,utMsgHead *));
int utComCompConfirmKey1(char *pSource,char *pKey);
int pasUdpSendMsgToIp(utShmHead *psShmHead,unsigned long lIp,unsigned short nUdpPort,
            long lMsgCode,char *pFunName,long l0,long l1,int iConfirm,int iSumVar,...);
            

#define utTcpReadMsg(x,y,z)  utComTcpReadMsg(x,y,x)

#endif

