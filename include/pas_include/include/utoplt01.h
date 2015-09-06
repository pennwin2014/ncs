/* 模板处理软件
   Writen by Li Yunming  in 2000/05/30
 */
#ifndef  UTPLTDEF0001
#define  UTPLTDEF0001    1
#include <stdarg.h>
#define UTPLT_NOEXISTVAR_SPACES         0
#define UTPLT_NOEXISTVAR_PRNVAR         1
#define UTPLT_NOEXISTVAR_EXIT           2
#define UTPLT_MAXVARLEN      32
#define UTPLT_ALLOCSTEP      4096      /* 每次分配空间的大小    */
#define UTPLT_MARKB          "[#"      /* 变量标识起始字符串    */
#define UTPLT_MARKE          "#]"      /* 变量标识的终止字符串  */
#define UTPLT_LOOPB          "SLPB"    /* 循环变量的起始标识    */
#define UTPLT_LOOPE          "SLPE"    /* 循环变量的终止标识    */
#define UT_WEB_USRNOEXIST              -1   /* 用户不存在       */
#define UT_WEB_INVUSRINFO              -2   /* 无效的用户信息   */


/* 链表数据  2002/07/11 由于效率原因改为二叉树, 本结构已不用  */
typedef struct utPltDbLink_S_bak {
        char  *caVarName;   /* 变量名     */
        char  *pValue;                       /* 值        */
        struct utPltDbLink_S *next;          /* 下一个值  */
} utPltDbLink_bak;


typedef struct utPltDbLink_S {
        char  *caVarName;   /* 变量名     */
        char  *pValue;                       /* 值        */
        struct utPltDbLink_S *Left;          /* 左子树    */
        struct utPltDbLink_S *Right;         /* 右子树    */
} utPltDbLink;



/* Html数据缓冲区  */

typedef struct utPltHtmBuf_S {
        unsigned int iLen;                  /* 当前字符串长度          */
        unsigned int iMaxBytes;             /* 目前分配空间的最大长度  */
        char     *pBuffer;                  /* 数据缓冲区              */
} utPltHtmBuf;


/* 模板数据链表头指针  */
typedef struct  utPltDbHead_S_bak {
        utPltDbLink  *psFirst;   /* 数据头指针  */
        utPltDbLink  *psEnd;     /* 数据尾指针  */
        utPltHtmBuf  *psHtmBuf;  /* Html缓冲区  */
        int           iCookLen;   /* Cookies变量的长度  */
        int           iCookMax;   /* Cookies分配的长度  */
        char         *pCookies;   /* Cookies 变量      */
        int           iVersion;   /* Version > 0 表示最新版本  */
} utPltDbHead_bak;


typedef struct  utPltDbHead_S {
        utPltDbLink  *psFirst;    /* 数据头指针  */
        utPltHtmBuf  *psHtmBuf;   /* Html缓冲区  */
        int           iCookLen;   /* Cookies变量的长度  */
        int           iCookMax;   /* Cookies分配的长度  */
        char         *pCookies;   /* Cookies 变量      */
        int           iVersion;   /* Version > 0 表示最新版本  */
} utPltDbHead;


/* utoplt01.c */
int utPltSetCvtHtml(int iFlags);
utPltDbHead *utPltInitDbHead();
utPltDbHead *utPltInitDb();
int utPltFreeDb(utPltDbHead *psDbHead);
int utPltFreeDbLink(utPltDbLink *psDbLink);
int utPltSetPlatePath(char *pPath);
char *utPltGetPlatePath();
int utPltSetVarNoExist(int iFlags);
char *utPltToHtmBuf(char *pIn,utPltDbHead *psDbHead);
int utPltDoLoop(char *pIn,utPltDbHead *psDbHead,char *pPre);
int utPltPutVar(utPltDbHead *psDbHead,char *pVarName,char *pValue);
char *utPltCvtHtmlChar(char *pValue);
int utPltShowDb(utPltDbHead *psDbHead);
int utPltShowDbLink(utPltDbLink *psDbLink);
char *utPltLookVar(utPltDbHead *psDbHead,char *pVarName);
int utPltGetStrToMark(char *pIn, utPltHtmBuf *psBuf,
                    char *pVarName, char *pMarkB,char *pMarkE);
utPltHtmBuf *utPltInitHtmBuf(int iMaxBytes);
int utPltFreeHtmBuf(utPltHtmBuf *psHtmBuf);
int utPltGetMemoryNum();
char *utPltFileToHtml(char *pFile,utPltDbHead *psDbHead);
int utPltFileToHtmlFile(char *pFile,char *pOutFile,utPltDbHead *psDbHead);
int utPltPutSomeVar(utPltDbHead *psDbHead,int iNum,...);
int utPltPutVarF(utPltDbHead *psDbHead,char *pVarName,char *pFormat,...);
int utPltPutLoopVar(utPltDbHead *psDbHead,char *pVarName,int i,char *pValue);
int utPltPutLoopVarF(utPltDbHead *psDbHead,char *pVarName,int iNum,char *pFormat,...);
int utPltPutLoopVar3(utPltDbHead *psDbHead,char *pVarName,int i,int j,int k,char *pValue);
int utPltPutLoopVarF3(utPltDbHead *psDbHead,char *pVarName,int iNum,int iNum1,int iNum2,char *pFormat,...);
int utPltPutLoopVar2(utPltDbHead *psDbHead,char *pVarName,int i,int j,char *pValue);
int utPltPutLoopVarF2(utPltDbHead *psDbHead,char *pVarName,int iNum,int iNum1,char *pFormat,...);
int utPltStrcpy(utPltDbHead *psDbHead,char *pStr);
char *utPltMsgToFile(char *caPltFile,int iSumVar,...);
int utPltOutToHtml(int iFd,utMsgHead *psMsgHead,utPltDbHead *psDbHead,char *pPlate);
int utPltDoLoopNew(char *pIn,utPltDbHead *psDbHead,char *pPre);
int utPltSetCookies(utPltDbHead *psDbHead,char *pName,char *pValue,
        char *pPath,char *pDomain,char *pExpire);
int utPltDelCookies(utPltDbHead *psDbHead,char *pName);
int pasSetCookies(char *pName,char *pValue,
        char *pPath,char *pDomain,char *pExpire);
int pasDelCookies(char *pName,char *pPath,char *pDomain);
int utPltFileDownload(int iSock,char *pContentType,char *pPath,char *pFileName,char *pSave);
int utPltDispBin(int iSock,char *pContentType,char *pContent,
                 int lBytes);
int utPltHtmlFileOut(int iFd,utMsgHead *psMsgHead,char *pFile);
/* utoplt02.c */
int utWebDispMsg(int iFd,utMsgHead *psMsgHead,char *pPltName,char *pTitle,char *pFormat,...);
int utWebSetUserInfo(utPltDbHead *psDbHead,char *UserName,char *caRight,char *ipAddress,char *path,char *domain);
int utWebCheckUser(utMsgHead *psMsgHead,int iRight);
int utWebGetLoginUser(utMsgHead *psMsgHead,char *caUserName,char *caIp);
int utWebRightOk(char *caRight,int iRight);
int utWebSetRight(char *caRight,int iRight);
int utWebDelRight(char *caRight,int iRight);
int utWebClearRight(char *caRight);
/* utoplt03.c */
int pasDispTcpMsg(int iFd,utMsgHead *psMsgHead,int iStatus,char *pPltName,char *pTitle,char *pFormat,...);
#endif

