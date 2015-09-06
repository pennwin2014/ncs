#ifndef	__UTOERROR_H__
#define	__UTOERROR_H__
/*
#ifdef __UTERROR_C__
unsigned long iutErrLastCode;
#endif
 */

#define ERR_MSG_MEMALLOCERR      01101
#define ERR_MSG_VARNOEXIST       01102
#define ERR_MSG_ERRDATATYPE      01103
#define	PQ_ERR_GENERAL			(-1)
#define PQC_ERR_MEMORY			30001
#define PQC_ERR_CREATSYNCOBJ	30002
#define PQC_ERR_SYNCOBJWRONG	30003
#define PQC_ERR_DEADLOCK		30004
#define PQC_ERR_CREATHREAD		30005
#define PQC_ERR_CREATHEAP		30006
#define PQC_ERR_HEAPALLOC		30007
#ifdef	__cplusplus
extern "C" { 
#endif	/* __cplusplus */
#ifdef _WIN32
#include	"picqcnet.h"
PICQCNET_API void utErrSetCode(int code);
PICQCNET_API int  utErrGetCode(void);
#else
void utErrSetCode(int code);
int  utErrGetCode(void);
#endif

#ifdef	__cplusplus
}
#endif	/* __cpluscplus */
#endif	/* !UTOERROR_H__ */
