/* 监控软件---有关Web的应用
     Modify 2002/9/9   by Liyunming*/
#define PAS_SRCFILE      9888
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <string.h>
#include <locale.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/time.h>
#include <time.h>
#include "utoall.h"
#include "utoplt01.h"
#include "pasdb.h"
#include "dsutl.h"
#include "dsclt.h"
#include "ncsdef.h"
#include"dlfcn.h"
#include "ncapi.h"
#include "ncmac.h"


int procapFrontpageSetFun(utShmHead * psShmHead);
int procapWeb001Fun_infoquery(utShmHead *psShmHead);
int dataMiningFun(utShmHead * psShmHead);
int procap_myaccount_setfun(utShmHead * psShmHead);
int procapAlarmSetFun(utShmHead * psShmHead);
int procapImportSetFun(utShmHead *psShmHead);
int procapAuditSetFun(utShmHead * psShmHead);
int procapOfflineSetFun(utShmHead * psShmHead);



int procapWeb001Fun(utShmHead * psShmHead)
{
    int iReturn;
    iReturn = procapAlarmSetFun(psShmHead);
    iReturn = procapFrontpageSetFun(psShmHead);
    iReturn = procapWeb001Fun_infoquery(psShmHead);
    iReturn = dataMiningFun(psShmHead);
    iReturn = procap_myaccount_setfun(psShmHead);
    iReturn = procapAlarmSetFun_L(psShmHead);
    iReturn = procapImportSetFun(psShmHead);
    iReturn = procapAuditSetFun(psShmHead);
	iReturn = procapOfflineSetFun(psShmHead);
    return 0;

}