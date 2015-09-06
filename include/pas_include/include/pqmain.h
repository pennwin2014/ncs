#ifndef __PQSMAINDEF____
#define __PQSMAINDEF____
#ifdef	_WIN32
#include "utotype.h"
#else
#include "utoall.h"
#endif

#define PQ_MSG_CODE         990118
#define PQ_MAXTCPFUN        50
#define PQ_MAXUDPFUN        50

#define PQ_CODE_REQUEST      990118101
#define PQ_CODE_RESPONSE     990118102
#define PQ_CODE_ANNOUNCE     990118103
#define PQ_CODE_CONFIRM      990118104



#define PQ_STA_OFFLINE      0     /* 离线            */
#define PQ_STA_ONLINE       1     /* 在线            */
#define PQ_STA_AWAY         2     /* 走开            */
#define PQ_STA_QUIET        3     /* 别打扰          */

#define PQ_SHA_UDPPORT      11    /* UDP端口号       */
#define PQ_SHA_TCPPORT      12    /* TCP端口号       */
#define PQ_SHA_TCPTIME      13    /* TCP超时时间     */



#define PQ_LNK_PICQRESERVE  11    /* 保留PICQ号码    */
#define PQ_LNK_ACTIVEPICQ   12    /* Picq当前信息表  */
#define PQ_LNK_PICQNUMBER   13    /* 要分配的PICQ号码表  */
#define PQ_LNK_HOSTLIST     14    /* 服务器列表      */
#define PQ_LNK_NETMASK      15    /* 网络标识        */
#define PQ_LNK_ADVERTISING  16    /* 广告信息        */


/* 个人信息公开程度  */
#define PQ_SEC_BASE         0     /* 基本信息公开    */
#define PQ_SEC_BUSINESS     1     /* 商务信息公开    */
#define PQ_SEC_PRIVATE      2     /* 个人信息公开    */
#define PQ_SEC_PUBLIC       3     /* 全部公开        */
#define PQ_SEC_CONTROL      9     /* 控制信息(内部使用) */

/* 对指定人的动作  */
#define PQ_OPR_NORMAL       0     /* 正常            */
#define PQ_OPR_HIDEN        1     /* 隐身            */
#define PQ_OPR_SHIELD       2     /* 屏蔽            */
#define PQ_OPR_DELETE       4     /* 删除            */
#define PQ_OPR_UNAUTH       5     /* 没有确认        */

/* 联机服务器类型  */
#define PQ_ONLINE_NEWS      1     /* 新闻服务器      */
#define PQ_ONLINE_MEET      2     /* 会议服务器      */

/* 功能编码        */
#define PQ_FUN_STATUS       100   /* 状态信息        */
#define PQ_FUN_REQNUMBER    101   /* 申请号码        */
#define PQ_FUN_CANCELNUM    102   /* 取消号码申请    */
#define PQ_FUN_REGISTER     103   /* 用户注册        */
#define PQ_FUN_LOGIN        104   /* 用户登录        */
#define PQ_FUN_RENUMBER     105   /* 重新申请号码    */
#define PQ_FUN_VERIFYNET    106   /* 服务器确认      */
#define PQ_FUN_FINDUSER     107   /* 搜寻网友        */
#define PQ_FUN_ADDCONTACTLIST   108  /* 加入通信列表  */

#define PQ_FUN_SYNINFO          109  /* TCP 信息同步  */
#define PQ_FUN_SYNUSERINFO      110  /* 用户更新资料  */
#define PQ_FUN_STOPMSG          111  /* TCP 结束数据包 */
#define PQ_FUN_SYNCONTACTINFO   112  /* 通信列表中的用户资料  */
#define PQ_FUN_CONFIRMMSG       113  /* 确认数据包            */
#define PQ_FUN_MODUSRSTATUS     114  /* 用户状态修改          */
#define PQ_FUN_MODCONTACTLIST   115  /* 修改通信列表中的操作方式 */
#define PQ_FUN_AUTH             116  /* 用户验证    */
#define PQ_FUN_GETUSRINFO       117  /* 取用户信息  */
#define PQ_FUN_LOGOUT           118  /* 用户离线    */
#define PQ_FUN_MODUSRINFOR      119  /* 用户资料更新 */
#define PQ_FUN_SENDMAIL         120  /* Send Mail    */
#define	PQ_FUN_FINDUSERINFO		121
#define PQ_FUN_FINDUSERNETREC	122
#define PQ_FUN_SENDMESSAGE		123  /* 发送消息  */
#define PQ_FUN_CHATBEGIN		124
#define PQ_FUN_CHATMESSAGE		125
#define PQ_FUN_CHATEND			126
#define PQ_FUN_SENDFILEBEGIN	127
#define PQ_FUN_SENDFILEDATA		128
#define PQ_FUN_SENDFILEEND		129
#define	PQ_FUN_FINDMEETSVR		130    /* 查找会议服务器  */
#define PQ_FUN_PLACARD          131    /* 广告下载        */
#define PQ_FUN_REQONLINE        132    /* 请求联机服务    */


/*
#define PQ_FUN_WEATHERCITYLIST	139
#define PQ_FUN_WEATHERCUSTOM	140
#define PQ_FUN_WEATHER			141
*/
#define PQ_FUN_NEWSSERVER		142
/*
#define PQ_FUN_NEWSLIST			143
#define PQ_FUN_NEWSCUSTOM		144
#define PQ_FUN_NEWSCONTENT		145
*/
#define PQ_FUN_TCPTEST          160
#define PQ_FUN_TCPTEST1         161
#define PQ_FUN_GETUSERSTATUS    162   /* 取指定用户的当前状态 */

/*  错误代码   */

#define PQ_WRN_LOGONLINE	1	  /* 已有用户在线上      */
#define PQ_ERR_RQNALLOCMEM  10101 /* 内存申请出错        */
#define PQ_ERR_RQNSQLCUR    10102 /* 数据库游标出错      */

#define PQ_ERR_RGUNOPQNUM   10301 /* 数据包中没有PICQ号码*/
#define PQ_ERR_RGUNOPASSWD  10302 /* 数据包中无password  */
#define PQ_ERR_RGUINVPQNUM  10303 /* picq不在选择范围中  */
#define PQ_ERR_RGUINVMSGID  10304 /* Message Id 不合法   */
#define PQ_ERR_RGUSQLINSERT 10305 /* 数据插入出错        */

/* 用户登录  */
#define PQ_ERR_LOGNOEXIST	10401 /* PICQ号不存在        */
#define PQ_ERR_LOGPWRONG	10402 /* 密码不正确          */
#define PQ_ERR_LOGDBSERR    10403 /* 数据库操作出错      */
#define PQ_ERR_LOGPKGERROR	10404 /* 数据包不完整        */
#define PQ_ERR_LOGSVRFAIL	10405 /* 服务器内部发生错误  */

#define PQ_ERR_VFNNOIP      10601 /* 请求格式中无IP地址  */

#define PQ_ERR_FUSRDBERR    10701 /* 数据库出错          */
#define PQ_ERR_FUSRNOFOUND  10702 /* 用户不存在          */
#define PQ_ERR_FUSRMEMERR   10703 /* 内存分配出错        */
#define PQ_ERR_FUSRFMTERR   10704 /* 请求数据包不正确    */

#define PQ_ERR_ADCOFMTERR   10801 /* 数据包不正确        */
#define PQ_ERR_ADCODBSERR   10802 /* 数据库操作不正确    */
#define PQ_ERR_ADCOMEMERR   10803 /* 初始化消息时内存不对 */
#define PQ_ERR_ADCOUSRINFO  10804 /* 源PICQ的用户信息出错 */
#define PQ_ERR_ADCOMODI     10805 /* 通信列表数据库修改出错 */

#define PQ_ERR_SYNINFO01    10901 /* 请求数据包不完整     */
#define PQ_ERR_SYNINFO02    10902 /* 消息初始化不正确     */
#define PQ_ERR_SYNINFO03    10903 /* 用户信息不正确       */

#define PQ_ERR_MODSTA001    11401 /* 请求数据包不完整     */
#define PQ_ERR_MODSTA002    11402 /* 状态修改出错         */


#define PQ_ERR_MODCOML001   11501 /* 请求数据包不完整     */
#define PQ_ERR_MODCOML002   11502 /* 修改不成功           */

#define PQ_ERR_AUTH001      11601  /* 数据包不完整         */
#define PQ_ERR_AUTH002      11602  /* 用户不存在           */
#define PQ_ERR_AUTH003      11603  /* Password不正确       */
#define PQ_ERR_AUTH004      11604  /* 用户户不在线上       */

#define PQ_ERR_GETUSR001    11601  /* 数据包不完整         */
#define PQ_ERR_GETUSR002    11602  /* 消息初始化出错       */
#define PQ_ERR_GETUSR003    11603  /* 取用户信息出错       */
#define PQ_ERR_LOGOUT001    11801  /* 已有用户在线上       */

#define PQ_ERR_MODUSR001    11901  /* 数据包不对           */
#define PQ_ERR_MODUSR002    11902  /* Password不对         */
#define PQ_ERR_MODUSR003    11903   /* Data Base Error     */

#define PQ_ERR_MAIL001      12001   /* 数据包不对          */
#define PQ_ERR_MAIL002      12002   /* 管道打开出错        */

#define PQ_ERR_FINDMSRV01   13001   /* 数据包不对          */
#define PQ_ERR_FINDMSRV02   13002   /* 没有会议服务器      */

#define PQ_ERR_PLACARD01    13101   /* 没有广告内容        */
#define PQ_ERR_PLACARD02    13102   /* 广告文件不存在      */
#define PQ_ERR_PLACARD03    13103   /* 内存申请出错        */
#define PQ_ERR_PLACARD04    13104   /* GraphCode Error     */

#define PQ_ERR_REQONLINE01      13201  /* 已经预定过    */
#define PQ_ERR_REQONLINE02      13202  /* 预订出错      */

#define PQ_ERR_GETSTATUS01  16201   /* 数据包不对          */

/* 允许其它用户操作  */

typedef struct {
			long lMsgId;		    /* 申请的ID号          */ 
		    long laPqnumber[6];     /* 6个号码             */
            long lReqTime;	        /* 申请时间			   */
} pqMsgRevNum;

/* 服务器列表         */
typedef struct {
    char caIpAddress[16];
    char caUdpPort[16];
} pqSrvHostList;


/*  Active Picq Information */

typedef struct {
    long   lPicqNum;     /* Picq号码 */
    long   lMsgId;       /* 登录消息时消息的MessageId */
    long   lIpAddr;      /* Picq 的IP地址  */
    short  nUdpPort;     /* Udp端口号, 网络顺序  */
    short  nTcpPort;     /* Tcp端口号, 网络顺序  */
    short  nNetWork;     /*  0--直接 >0--Proxy   no used */
    short  nNetMask;     /*  0--163   1--169     */
    short  nVersion;     /* 版本号                   */
    short  nStatus;      /* 用户状态                 */
    long   lLoginTime;   /* 登录时间                 */
    long   lLastTime;    /* 最后一次收到消息时间     */
    char   caPublicKey[24]; /* 通信密钥               */ 
    short  nPower;
    short  nUserKind;       /* 0 Picq >0 OnlineServer */
    char   caName[16];   /* UserName */
    long   lOIpaddr;     /* 对方Ip地址  */
    short  nAccess;      /* 接入方式 0--接入=开户  1--接入  2--开户  */
    short  nReserve;
} pqStaActive;

typedef struct {
    long lSumNum;
    long lMaxNum;
    long lTime;
    long *lPicq;
} pqTimeList;
    
typedef struct {
    long           lNetType;     /* 网络标识  */
    unsigned long  lIpAddress;   /* 本机地址  */
    unsigned long  lStartIp;     /* 起始地址  */
    unsigned long  lNetMask;     /* 网络掩码  */
} pqNetMask;


typedef struct {
    long   graphcode;
    char   caFileName[32];
    short  nType;       /*  1-- bmp  2--gif  3---jepg  */
    long   lCount;
} pqAdvertising;    /* 广告信息  */

#define PQ_GIF_BMP     1
#define PQ_GIF_GIF     2
#define PQ_GIF_JEPG    3

/* 通信列表的信息  */
typedef struct {
    long lPicqNum;   /* PICQ号码  */
    long lStatus;    /* 状态      */
    long lTime;      
} pqContactList;

/* 查找用户信息  */
typedef struct {
    long  lPicqNum;        /* 按网络字节顺序存放   */
	char  caMicqNum[12];    
	char  caUserName[16];
    char  caRealName[20];
    char  caEmail[64];
    long  lStatus;          /* 按网络字节顺序存放   */
} pqFUserInfo;

/* 用户状态信息  */
typedef  struct {
    long   lPicqNum;    /* PICQ号    */
    short  nStatus;     /* 用户状态  */
    short  nNetWork;    /* 网络类型  */
    short  nUdpPort;    /* Udp端口号 */
    short  nTcpPort;    /* Tcp端口号 */
    long   lIpAddress;  /* IP地址    */
    char   caPubKey[24]; /* 公开密钥  */
} pqUserStatus;

/* 联机服务器信息  */
typedef struct {
    char            caName[16];
    long            lPicqNum;
    unsigned long   ulAddr;
    unsigned short  unUdp;
    unsigned short  unTcp;
    unsigned short  unType;
    unsigned short  unNoUsed;
} pqOnlineInfo;

/*  pqm000.c */
int pqInit000(utShmHead *psShmHead);
int pqComTcpInit(utShmHead *psShmHead);
int pqComUdpEnd(utShmHead *psShmHead);
int pqComTcpEnd(utShmHead *psShmHead);
/*  pqm001.c  */
int pqFunVerifyNetT(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int pqFunRequestNumber(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int pqFunCancelNum(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead);
int pqFunReNumber(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int pqFunDefault(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead); 
int pqFunDownPlacard(utShmHead *psShmHead, int iFd, utMsgHead *psMsgHead);          
/* pqm002.c */
int pqFunSynInfo(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int pqFunModiStatus(utShmHead *psShmHead, int iFd,
                                    utMsgHead *psMsgHead);
/* pqm003.c  */
int pqFunRegister(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int pqFunFindUser(utShmHead *psShmHead, int iFd,
        utMsgHead *psMsgHead);
int pqFunModUserInfo(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);

/* pqm004.c   */
int pqFunAuth(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int pqFunGetUsrInfo(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int pqFunReqOnline(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
/* pqm005.c */
int pqFunSendMail(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
/* pqm006.c */
int pqFunTestConnect(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int pqFunTestBigPkg(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int pqFunFindMeetSvr(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int pqFunGetUserStatus(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
/* pqm100.c  */
int pqComUdpInit(utShmHead *psShmHead);

/* pqm101.c  */
int pqFunLogin(utShmHead *psShmHead, 
            utComSockAddr *psSockAddr,utMsgHead *psMsgHead);
int pqFunExchangeMsg(utShmHead *psShmHead, 
            utComSockAddr *psSockAddr,utMsgHead *psMsgHead);
/* pqm102.c */
int pqFunSynStatus(utShmHead *psShmHead, 
            utComSockAddr *psSockAddr,utMsgHead *psMsgHead);
/* pqm103.c */


/* pqm104.c */
int pqFunAddComList(utShmHead *psShmHead, 
            utComSockAddr *psSockAddr,utMsgHead *psMsgHead);
/* pqm105.c */
int pqFunStatus(utShmHead *psShmHead, 
            utComSockAddr *psSockAddr,utMsgHead *psMsgHead);
int pqFunVerifyNet(utShmHead *psShmHead, 
            utComSockAddr *psSockAddr,utMsgHead *psMsgHead);
int pqFunLogout(utShmHead *psShmHead, 
            utComSockAddr *psSockAddr,utMsgHead *psMsgHead);
int pqFunModiContactList(utShmHead *psShmHead, 
            utComSockAddr *psSockAddr,utMsgHead *psMsgHead);
/* pqm201.ec */
int pqDbsAddComList(long lSourcePicq,long lObjectPicq,
        long lStatus,long lOpFlags);
int pqDbsModiComList(long lSourcePicq,long lObjectPicq,
        long lStatus,long lOpFlags);
int pqDbsGetUserInfo(long lPicq,long lSecrecy,long lTime,
    utMsgHead **psMsgHead);
int pqDbsAddSendMsg(long lPicqNum,long lType,utMsgHead *psMsgHead);
int pqUdpSendPendMsg(utShmHead *psShmHead,pqStaActive  *psStaActive);
int pqDbsLogin(long lPicq,char *pPassWord,
    char *pUserName,long *lpPower,long *lpUserKind);
int pqSReadNumberFromDb(utShmHead *psShmHead);
int pqDbsUserAuth(long lPicq,char *pPassWord,long *lpPower,long *lpUserKind);
int pqDbsLogoff(long lPicq);
int pqDbsModiContactList(long lPicq1,long lPicq2,short lOpr);
int pqDbsGetAdver(utShmHead *psShmHead);
int pqDbsInitUserStatus();
int pqDbsLoginAddComLst(utShmHead *psShmHead, long lPicqNum0);
/* pqm202.ec */
int pqFunSynConactInfo(utShmHead *psShmHead, int iFd,
        utMsgHead **psMsgHead,long lPicq0,long lTime0,int iConfirm);
pqUserStatus *pqFunGetComStatus(utShmHead *psShmHead,
    long lPicqNum0,long *lReturn);
int pqUdpDoLogin(utShmHead *psShmHead,long lPicqNum);
int pqDbsAddLogs(pqStaActive *psStatus);
int pqDbsModiLogs(utShmHead *psShmHead,long lPicqNum);
/* pqm203.ec */
int pqFunRegister0(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);
int pqFunFindUser0(char *pSmt,long lStart,long lNumRec,pqFUserInfo *psUser);
int pqFunModUserInfo0(utShmHead *psShmHead, int iFd,utMsgHead *psMsgHead);   
   
/* pqm301.c */
pqStaActive *pqUtlGetPicqStatus(utShmHead *psShmHead,
            long lPicqNum);
int pqGetPicqUdpAddr(utShmHead *psShmHead,long lPicqNum, 
        utComSockAddr *psSockAddr);
int pqUdpDoLogOff(utShmHead *psShmhead,long lPicqNum);

/* pqm302.c */
void pqSetPicqNum(long lPicqNum, int nUdpPort);
int pqComUdpRequest1(utShmHead *psShmHead, utComSockAddr *psSockAddr, 
        long lPicqNum,short nFunCode,long lMsgId,int iConfirm,short nVarNum,...);
int pqComUdpResponse1(utShmHead *psShmHead,utComSockAddr *psSockAddr,
        utMsgHead *psMsgHead0,int iConfirm,short nVarNum,...);
int pqComUdpRequest(int iSock, utComSockAddr *psSockAddr, 
        long lPicqNum,short nFunCode,long lMsgId,short nVarNum,...);
int pqNetGetNetwork(utShmHead *psShmHead,unsigned long lAddress,
            unsigned long *lReturn);
int pqSGetPqNumber(utShmHead *psShmHead,char *pPicq,long *lpNumber,int iNumber);
int pqSAddPqNumber(utShmHead *psShmHead,long *lpNumber,int iNumber);
int pqUtlSendStatus(utShmHead *psShmHead, long lPicqNum);
int pqUtlStaToFriend(utShmHead *psShmHead, long lPicqNum,
                                    long lStatus); 
int pqUdpLetLogOut(utShmHead *psShmHead,
        pqStaActive *psStaActive); 
int pqComUdpMsgToPicq(utShmHead *psShmHead,
        long lPicqNum1, long lPicqNum2,
        long lCode, short nFunCode,
        short nConfirm,
        short nVarNum,...);
int pqComUdpMsgToPicq0(utShmHead *psShmHead,
        long lPicqNum,utMsgHead *psMsgHead,int iConfirm);        
pqOnlineInfo *pqUtlGetOnlineSrv(utShmHead *psShmHead,
        int iType,int *iNumber);
int pqUtlStaExchange(utShmHead *psShmHead,long lPicqNum0, 
                 long lPicqNum);       
        
/* pqm303.c */
int pqUtlTimeControl(utShmHead *psShmHead);   
#endif

