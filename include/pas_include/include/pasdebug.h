#ifndef __PASDEBUG__
#define __PASDEBUG__
#define pasDebug(iLevel,p)    pasDebug_0(__LINE__,__FILE__,iLevel,p);
#define pasDebug1(iLevel,p,p1)    pasDebug_0(__LINE__,__FILE__,iLevel,p,p1);
#define pasDebug2(iLevel,p,p1,p2)    pasDebug_0(__LINE__,__FILE__,iLevel,p,p1,p2);
#endif
