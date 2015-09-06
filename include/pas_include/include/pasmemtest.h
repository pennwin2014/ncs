#ifndef __NETALLOCDEF__
#define __NETALLOCDEF__
#ifdef MTEST_1
#include <stdio.h>
#include <stdlib.h>
#define malloc(x)    pas_malloc(x,__LINE__,__FILE__)
#define calloc(x,y)  pas_calloc(x,y,__LINE__,__FILE__)
#define free(x)      pas_free(x,__LINE__,__FILE__)
#define realloc(x,y) pas_realloc(x,y,__LINE__,__FILE__)
#define utStrDup(x)  pas_utStrDup(x,__LINE__,__FILE__)
#define utStrnDup(x,y)  pas_utStrnDup(x,y,__LINE__,__FILE__)
#define fopen(x,y)   pas_fopen(x,y,__LINE__,__FILE__)
#define fclose(x)    pas_fclose(x,__LINE__,__FILE__)
#define open(x,y,z)  pas_open(x,y,z,__LINE__,__FILE__)
#define close(x)     pas_close(x,__LINE__,__FILE__)
#define accept(x,y, z) pas_accept(x,y,z,__LINE__,__FILE__)
#define utMsgInit(x,y,z,a) pas_utMsgInit(x,y,z,a,__LINE__,__FILE__)
#define utMsgFree(x) pas_utMsgFree(x,__LINE__,__FILE__)
#define socket(x,y,z) pas_spcket(x,y,z,__LINE__,__FILE__)
#endif
#endif
