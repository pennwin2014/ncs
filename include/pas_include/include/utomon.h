#ifndef __UTOMONDEF__
#define __UTOMONDEF__
#include "utotype.h"
struct utMonCommand_S {
    char   caCom[16];   /* Command Name */
    short  nComType;    /* Command Type  <0 CommandCode  >0 DataType */
    struct utMonCommand_S  *psNextPar;
    struct utMonCommand_S  *psNextCom;
};

struct utMonParList_S {
    long  lComCode;
    short nNumPar;
    char  caParName[5][64];
};

typedef struct utMonParList_S utMonParList;

struct utMonDoFun_S {
    long lFunCode;
    int   (*fFunName)(utMonParList *, void *);
    struct utMonDoFun_S *psNextFun;
};


typedef struct utMonDoFun_S utMonDoFun;

typedef struct utMonCommand_S  utMonCommand;

typedef struct {
    utMonCommand *psCommand;
    utMonDoFun   *psDoFun;
} utMonComHead;

/* utomon01.c */
/* utomon01.c */
utMonComHead *utMonComInit();
int utMonSetCom(utMonComHead *psMonHead,
    int (*fFunName)(utMonParList *,void *), char *pCommand,...);
long utMonCommandCode(utMonComHead *psMonComHead,char *pCommand,
        struct utMonParList_S *psParList);
int utMonDoCommand(utMonComHead *psMonHead,utMonParList *psMonPar,void *pHead);
#endif

