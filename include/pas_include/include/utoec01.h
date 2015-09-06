#ifdef _ESQLDEF_
#define  UT_SQL_SUCESS   0  
#define  UT_SQL_NOFOUND  100

int utEsqlConnect(utShmHead *psShmHead);
int utEsqlClose(utShmHead *psShmHead);
int utEsqlError();
#endif 

