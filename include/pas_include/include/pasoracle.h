#ifndef  ___PAS_ORACLE8_DEF__
#define  ___PAS_ORACLE8_DEF__
#include "pasoci.h"
#define PAS_DB_LOG_ON               1
#define PAS_DB_LOG_OFF              0
#define PAS_DB_MEMORYERROR          -10001
#define PAS_DB_UNKONWTYPE           -10002
#define PAS_DB_OPENCURSORERR        -10003
#define PAS_DB_INVALIDPOS           -10004
#define PAS_DB_NOTFOUND             1403
#define PAS_DB_NULLVALUE            1405
#define PAS_DB_TRUNCATED            1
typedef DB_CONNECT pasDbConn;
typedef DB_CURSOR  pasDbCursor;

struct pasDbLogBuf_s {
    char *pSmt;
    struct pasDbLogBuf_s *psNext;
};

typedef struct pasDbLogHead_s {
    struct pasDbLogBuf_s *psFirst;
    struct pasDbLogBuf_s *psLast;
} pasDbLogHead;

    
typedef struct {
    char *pName;
    int  iType;
    char *pValue;
} pasDbBind;
char *pasDbReplaceQuote(char *pIn);    
pasDbConn *pasDbConnect(char *pUser,char *pPassword,char *pConnectStr);
int  pasDbClose(pasDbConn *psConn);
pasDbCursor *pasDbOpenSqlF(char *pSmt,...);
pasDbCursor *pasDbOpenSqlB(char *pSmt,int iSumBind,...);
pasDbCursor *pasDbOpenSql(char *pSmt,int iSumBind,...);
int pasDbCloseCursor(pasDbCursor *psCursor);
int pasDbFetch(pasDbCursor *psCursor);
int pasDbSumFields(pasDbCursor *psCursor);
int pasDbFieldName(pasDbCursor *psCursor,char *caName,int iPos);
// int pasDbValues(pasDbCursor *psCursor,int iPos,int iType,int iLen,...);
int pasDbCommit(pasDbCursor *psCursor);
int pasDbRollback(pasDbCursor *psCursor);
int pasDbExecSqlB(char *pSmt,int iSumBind,...);
int pasDbExecSqlF(char *pSmt,...);
// int pasDbExecSql(char *pSmt,int iSumBind,...);
int pasDbRecordSetB(char *pSmt,int iStart,int iSum,int *iRetSum,char *caResult,int iSumBind,...);
int pasDbRecordSetF(char *pSmt,int iStart,int iSum,int *iRetSum,char *caResult,...);
int pasDbOneRecord(char *pSmt,int iSumBind,...);
int pasDbFetchInto(pasDbCursor *psCur,...);
int pasDbRecordSet(char *pSmt,int iStart,int iSum,int *iRetSum,char *caResult,int iSumBind,...);
char *pasDbRecordValue(char *caRecord,int iRow,int iSumField,...);
char *pasDbErrorMsg(pasDbCursor *psCursor);
int  pasDbSqlCode(pasDbCursor *psCursor);
int pasConnect(utShmHead *psShmHead);
int pasDbSetBindValue(char *pSmt,char *caSmt,int iSumBind,va_list *ap);
pasDbCursor *pasDbOpenSql0(char *pSmt,int iSumBind,va_list ap);
#endif
