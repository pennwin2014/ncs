#ifndef __UTOLOCDEF
#define __UTOLOCDEF      1
int  utLocCreate(long lKey);
int  utLocOpen(long lKey);
void utLocRemove(long id);
void utLocClose(long id);
int  utLocLock(long id);
int  utLocUnlock(long id);
int  utLocSemOp(long id,int value);

#endif

