#ifndef  ___PASDB_DEF__
#define  ___PASDB_DEF__
#ifdef PAS_MYSQL
#include "pasmysql.h"
#else
#include "pasoracle.h"
#include "pasoci.h"
#endif
#include "utomsg01.h"
#include "utoplt01.h"
/* pasdb01.c */
int pasDbCurRecordInto(pasDbCursor *psCur,int iStart,int iSum,...);
int pasDbFetchIntoMem(pasDbCursor *psCur,char *pBuf,int iFlags,int iSum,...);
int pasDbGetCurIntoMem(pasDbCursor *psCur,char *pBuf,int iFlags,int iStart,int iSum,va_list ap);
int pasDbInsertFromMem(char *pTable,                                         char *pBuf,                                           unsigned long lBytes,                                 int  iFlags,
                       int iSumField,...);
int pasUtlGetDataFromMem(char *pBuf,int iFlags,int iSumField,...);
int pasUtlGetDataFromMemN(char *pBuf,int iFlags,int iMax,int iSumField,...);
int pasUtlPutDataToMem(char *pBuf,int iFlags,int iSumField,...);
/* pasdb02.c */
unsigned long pasDbGetSid(utShmHead *psShmHead,char *pTable);
int pasSetSid(char *pTable,char *pSid,unsigned long lMinId);
unsigned long pasGetSid(char *pTable,char *pSid);
unsigned long pasGetMaxSid(char *pTable,char *pSid);
int pasSetSid8(char *pTable,char *pSid,unsigned long long lMinId);
unsigned long long pasGetSid8(char *pTable,char *pSid);
/* pasdb03.c */
int pasSetLastInfoTable(char *pTable);
char *pasGetLastInfo(char *pInfoVar,char *pDefault);
int pasSetLastInfo(char *pInfoVar,char *pValue);
unsigned long pasGetLastInfo_l(char *pInfoVar,unsigned long lDefault);
int pasSetLastInfo_l(char *pInfoVar,unsigned long lValue);
int pasDbFieldIsExist(char *pTable,char *pField);
/* pasmysql.c */
int pasDbIsConnect();
int pasDbSetLog(int iFlags);
int pasDbLogFlags();
int pasDbSetLogFile(char *pLogFile);
pasDbConn *pasDbConnect(char *pUser,char *pPassword,char *pConnectStr);
int pasDbClose(pasDbConn *psConn);
pasDbCursor *pasDbOpenSqlF(char *pSmt,...);
pasDbCursor *pasDbOpenSqlB(char *pSmt,int iSumBind,...);
pasDbCursor *pasDbOpenSql(char *pSmt,int iSumBind,...);
pasDbCursor *pasDbOpenSql0(char *pSmt,int iSumBind,va_list ap);
int pasDbCloseCursor(pasDbCursor *psCursor);
int pasDbFetch(pasDbCursor *psCursor);
int pasDbSumFields(pasDbCursor *psCursor);
int pasDbFieldName(pasDbCursor *psCursor,char *caName,int iPos);
int pasDbFieldType(pasDbCursor *psCursor,int iPos);
int pasDbValues(pasDbCursor *psCursor,int iPos,int iType,...);
char *pasDbTextValues(pasDbCursor *psCursor,int iPos,int iType,int *iLen);
int pasDbCommit(pasDbCursor *psCursor);
int pasDbRollback(pasDbCursor *psCursor);
int pasDbExecSqlB(char *pSmt,int iSumBind,...);
int pasDbExecSqlF(char *pSmt,...);
int pasDbSaveExecSqlF(char *pSmt,...);
int pasDbExecSqlBuf();
int pasDbExecSqlFn(char *pSmt,int iSize,...);
int pasDbRecordSetB(char *pSmt,int iStart,int iSum,int *iRetSum,char *caResult,int iSumBind,...);
int pasDbOneRecord(char *pSmt,int iSumBind,...);
int pasDbRecordSetF(char *pSmt,int iStart,int iSum,int *iRetSum,char *caResult,...);
int pasDbFetchInto(pasDbCursor *psCur,...);
int pasDbRecordSet(char *pSmt,int iStart,int iSum,int *iRetSum,char *caResult,int iSumBind,...);
int pasDbExecSql(char *pSmt,int iSumBind);
int pasDbExecMsg(char* sql,utMsgHead** psMsgHead,utPltDbHead* psDbHead);
pasDbConn *pasDbCurConn();
int pasDbSetConn(pasDbConn *psConn);
char *pasDbReplaceQuote(char *pIn);
char *pasDbReplaceQuote1(char *pIn);
char *pasDbReplaceQuote_l(char *pIn,int l);
int _pasDbGetWord2(char *p,char *caTemp1,char *caTemp2);
int pasDbLogsSetMaxBuf(int iMax);
int pasDbWriteLogs(char *pSmt);
int pasDbSetAutoWriteLog(int iFLags);
int pasDbSetDbLogType(int iType);
int pasDbSetMaxDbLogSize(unsigned long lMax);
int pasDbDbLogType();
int pasDbWriteCommit();
int pasDbWriteRollback();
pasDbConn *pasDbInitTemp(char *pUser,char *pPassword,char *pConnectStr);
int pasDbExecSqlTempF(pasDbConn *psConn,char *pSmt,...);
int pasDbExecSqlTemp(pasDbConn *psConn,char *pSmt,int iSumBind);
int pasDbCloseTemp(pasDbConn *psConn);
long pasDbSumRecord(char *pTables);
long pasDbSumRecordF(char *pTables,char *pSmt,...);
long pasDbSumValue(char *pTables,char *pField,char *pWhere);
long pasDbMaxValue(char *pTables,char *pField,char *pWhere);
/* pasmysql1.c */
int pasConnect(utShmHead *psShmHead);
pasDbConn *pasConnectTemp(utShmHead *psShmHead);
/* paspub.c */
int pubExecSQL_00(pasDbConn *pasConn,
              char *sql,utMsgHead** psMsgHead,utPltDbHead* psDbHead);
int pasDbBindValueByMsg(char *pSmt,char *caSmt,utMsgHead *psMsgHead);
/* utodb01.c  */
char *pasDbSplitResult(char *pIn,int iSum,...);
#endif
