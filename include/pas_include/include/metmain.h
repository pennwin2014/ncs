#ifndef METINCLUDE_DEF
#define METINCLUDE_DEF

/* Mettcpdo.c */
int MetTcpInit(utShmHead *psShmHead);

#define MET_LNK_MEETROOM         20      /* 会议室      */
#define MET_LNK_MEETPEOPLE       21      /* 会议参加者  */

/* FunCOde of Online server */
#define PQ_FUN_REQMEETSVR		300      /* 请求会议服务器  */
#define PQ_FUN_MEETINVITE		301      /* 会议邀请        */
#define PQ_FUN_MEETJOIN			302      /* 请求加入会议    */
#define PQ_FUN_MEETFUNCTION		303      /* 转发会议功能    */
#define PQ_FUN_MEETDISPLACE		304      /* 会议转移        */
#define PQ_FUN_MEETEXIT			305      /* 退出会议        */
#define PQ_FUN_MEETEND			306      /* 会议结束        */
#define PQ_FUN_MEETINVITERES    307      /* 邀请应答        */

/* FunCOde of Meeting  */
#define PQ_MET_ANNOUNCEWORDS     1
#define PQ_MET_REQANNOUNCE       2
#define PQ_MET_FORBIDANNOUNCE    3
#define PQ_MET_ALLOWANNOUNCE     4
#define PQ_MET_REQDISPLACE       5
#define PQ_MET_REJDIDPLACE       6
#define PQ_MET_AGRDISPLACE       7
#define PQ_MET_DISPLACE          8
#define PQ_MET_JOININ            9     /* 加入会议  */
#define PQ_MET_NOTJOININ         10
#define PQ_MET_DISPELOUT         11
#define PQ_MET_EXIT              12
#define PQ_MET_TERMINATE         13

typedef struct {
    long  lMeetId;          /* 会议标识          */
    long  lPicq;             /* 会议主持人PICQ号  */
    char  caSubject[64];     /* 会议主题          */
    char  caAgenda[400];     /* 会议议题          */
    long  lTime;             /* 会议开始时间      */
    long  lLastTime;         /* 最后一次操作时间  */
    long  lSumMan;           /* 会议参加总人数    */
} metRoomInfo;

typedef struct {
    long lMeetId;          /* 会议标识  */
    long lPicqNum;         /* Picq号码  */
    char caName[16];
    long lLastTime;
    unsigned short nNetType;
    short          nType;    /* 0--一般参加者   1---主持人   2--等待确认 */
    short          nSock;
    unsigned short nInPort;
    long     lConnectId;     /* 连接标识   */
} metPeopleInfo;

/* The Value of nType  */
#define MET_TYPE_ATTENDE       0    /* 一般参加者  */
#define MET_TYPE_PRESIDING     1    /* 主持人       */
#define MET_TYPE_UNAUTH        2    /* 没有验证     */


#define MET_ERR_REQMET01           30101     /* 数据抱不完整  */
#define MET_ERR_REQMET02           30102     /* 内存分配出错  */
#define MET_ERR_REQMET03           30102     /* 插入链表错    */

#define MET_ERR_METINV01           30201     /* 数据包不完整  */
#define MET_ERR_METINV02           30202     /* 会议服务器超出负荷  */
#define MET_ERR_METINV03           30203     /* 插入链表出错  */
#define MET_ERR_METINV04           30204     /* 会议室不存在  */
#define MET_ERR_METINV05           30205     /* 没有被邀请    */
#define MET_ERR_METINV06           30206     /* 网络类型不对  */

#define MET_ERR_TRANS01            30301     /* 数据包不完整  */

#define MET_ERR_EXCH01             30401     /* 数据包不完整  */
#define MET_ERR_EXCH02             30402     /* 非主持人  */


#define MET_ERR_EXIT01             30501     /* 数据包不完整  */
#define MET_ERR_EXIT02             30502     /* 会议室不存在 */


/* 各种函数的错误信息  */
/* met000.c */
int metInit000(utShmHead *psShmHead);
int metTcpInit(utShmHead *psShmHead);
int metTcpEnd(utShmHead *psShmHead);
int MetUdpInit(utShmHead *psShmHead);

/* met002.c */
int metFunReqMeetSrv(utShmHead *psShmHead, int iFd,
            utMsgHead *psMsgHead);
int metFunMeetInvite(utShmHead *psShmHead, int iFd,
            utMsgHead *psMsgHead);            
int metFunTransFunMsg(utShmHead *psShmHead, int iFd,
            utMsgHead *psMsgHead);
int metFunExchangeMeet(utShmHead *psShmHead, int iFd,
            utMsgHead *psMsgHead);
int metFunExitMeet(utShmHead *psShmHead, int iFd,
               utMsgHead *psMsgHead);
int metFunMeetResInvite(utShmHead *psShmHead, int iFd,
            utMsgHead *psMsgHead);
/* met003.c */
metRoomInfo *metUtlGetRoomInfo(utShmHead *psShmHead,int lMeetId);
metPeopleInfo *metUtlGetPeopleInfo(utShmHead *psShmHead,long lMeetId,long lPicqNum);
int metUtlTcpMsgSend(int iPort,int iNum,long lConnectId,utMsgHead *psMsgHead,int iClose);
int metUtlTcpMsgSendTo(utShmHead *psShmHead, long lMeetId,long lPicqNum,
                        utMsgHead *psMsgHead,int iClose);
int metUtlTcpMsgSendToAll(utShmHead *psShmHead, long lMeetId,
                        utMsgHead *psMsgHead,int iClose);
int metUtlTcpAllStaToMe(utShmHead *psShmHead,long lMeetId,
        metPeopleInfo *psPeopleInfo);
int metUtlPeopleCompare(char *pSource,char *pKey);                                              
#endif
