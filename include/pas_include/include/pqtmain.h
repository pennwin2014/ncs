#ifndef   PQTMAIN_DEF
#define   PQTMAIN_DEF

#define   PQT_G_PICQNUM       0
#define   PQT_G_REPLYPORT     1
#define   PQT_G_SERVERIP      2
#define   PQT_G_TCPPORT       3
#define   PQT_G_UDPPORT       4
#define   PQT_G_NETTYPE       5
#define   PQT_G_REPEAT        6
#define   PQT_G_MSGID         7
#define   PQT_G_REGISTER      8

/* pqtmon02.c  */
int pqtUtlSetGlobal(int iNum,long lValues);
long pqtUtlGetGlobal(int iNum);
int pqtUtlSetDefaultGlobal(pqcMyStatus *psStatus);
int pqtUtlSetReqPicq(char *p);
int pqtUtlGetReqPicq(int iNumber);


#endif
