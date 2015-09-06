#ifndef _LUTEXP_DEF__
#define _LUTEXP_DEF__

long  utExpCompute(char *pHead,char *express,double *result);
long utExpLogicalCompute(char *pHead,char *express);
int  utExpSetVariableFun(int (*f)(char *pHead,char *pVarname,double *dResult));
#endif
