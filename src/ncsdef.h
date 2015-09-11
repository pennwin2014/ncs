#ifndef __NCSDEF___
#define __NCSDEF___
#include "ncscase.h"
#define  NCS_CLT_MINVERSION                        "V6.00.00"
#define  NCS_VERSION       "V7.50.126"              // 当前版本
#define  NCS_RELEASETIME   "2014/08/11"             // 发行时间
#define  NCS_MIN_DBID      20000000L
#define  NCS_MAX_DBID      29999999L
#define  NCS_MAX_SYNINFO     20                    /* 同步表的数量           */
#define  NCS_MAX_SYNPKG      400000L               /* 最大的同步数据包       */
#define  NCS_LOG_ERROR      0                      /* 出错信息                  */
#define  NCS_LOG_WARN       1                      /* 告警信息                  */
#define  NCS_LOG_EVENT      2                      /* 主要事件跟踪              */
#define  NCS_LOG_EVENT1     3                      /* 事件跟踪                  */
#define  NCS_LOG_EVENT2     4                      /* 事件跟踪                  */
#define  NCS_LOG_EVENT3     5                      /* 事件跟踪                  */
#define  NCS_LOG_DEBUG      9                      /* 调试信息                  */

#define  NCS_LNK_ONTIME      46                      /* 定时程序  */
#define  NCS_LNK_PROCESS     47                      /* 进程管理  */
#define  NCS_LNK_CLIENT      50                      /* 监控单位信息           */
#define  NCS_LNK_ONLINE      51                      /* 在线信息  */
#define  NCS_LNK_SYSINFO     52                      /* 存放系统当前各种信息   */
#define  NCS_LNK_ADMIPLIST   53                      /* 管理员IP列表           */
#define  NCS_LNK_SYNINDEX    54                      /* 各种策略索引           */
#define  NCS_LNK_SYNCONT     55                      /* 各种策略内容           */
#define  NCS_LNK_GROUP       56                      /* 单位类别               */
#define  NCS_LNK_ONLINEINDEX 57                      /* 在线信息索引           */
#define  NCS_LNK_ONLINEBUF   58                      /* 在线内容               */
#define  NCS_LNK_CASEAREA    59                      /* 布控区域               */
#define  NCS_LNK_CASEINDEX   60                      /* 案件信息               */
#define  NCS_LNK_CASECTL     61                      /* 案件布控信息           */
#define  NCS_LNK_UPDATEINFO  62                      /* 在线升级               */
#define  NCS_LNK_SUBADM      63                      /* 分管理中心             */
#define  NCS_LNK_CASEMOB     64                      /* 案件告警邮件或手机号       */
#define  NCS_LNK_MAILWARN    65                      /* 案件邮件告警记录       */

#define  NCS_LNK_ENDUSER     66                      /* 在线最终用户            */
#define  NCS_LNK_DOWNCOM     67                      /* 下发命令                */
#define  NCS_LNK_MYSTATUS    68                      /* 客户端信息                */
#define  NCS_LNK_INDCLIENT    69                      /* 目录同步客户端信息             */
#define  NCS_LNK_SMSMODEN    70                      /* 短信猫短信队列             */
#define  NCS_LNK_SMSNO    71                         /* 新网互联手机号             */
#define  NCS_LNK_SMSONLINE    72                  /* 短信场所发送在线信息             */
#define  NCS_LNK_CLINTCODETOID    73                /* 场所端代码到ID   */

#define  NCS_LNK_MACPROCYOBJ    74                /* MAC布控策略布控场所 */
#define  NCS_LNK_BABPROCYOBJ    75                /* 伴随行为分析 */
#define  NCS_LNK_FRONTPAGE      76                /*首页数据*/


#define  NCS_MAX_CLIENT      100
#define  NCS_RUNLOG          "../log/ncssystem.log"

#define  NCS_ERR_SUCCESS     0                       /* 正确              */
#define  NCS_ERR_RELOGIN     10001                   /* 用户需要重新登录  */
#define  NCS_ERR_USRNOEXIST  10002                   /* 用户不存在        */
#define  NCS_ERR_PASSERROR   10003                   /* 口令出错          */
#define  NCS_ERR_FILENOEXIST 10004                   /* 文件不存在        */
#define  NCS_ERR_MEMERROR    10005                   /* 内存出错          */
#define  NCS_CLTLOG_LOGIN        1
#define  NCS_CLTLOG_LOGINERR     2
#define  NCS_CLTLOG_LOGOUT       3                   

#define  NCS_CLTSTA_ERROR       -1    /* 严重错误   */
#define  NCS_CLTSTA_ONLINE      1     /* 在线  */
#define  NCS_CLTSTA_OFFLINE     2     /* 离线  */
#define  NCS_CLTSTA_ABNORMITY   3     /* 异常  */

#define  NCS_FTYPE_REALMON    0
#define  NCS_FTYPE_LOG        1
#define  NCS_FTYPE_CTL        2

#define  NCS_FIL_CASECTL      1         /* 案件文件   Case */    

#define  NCS_UFLAGS_COMM            0
#define  NCS_UFLAGS_ALL             3
#define  NCS_UFLAGS_GROUP           2
#define  NCS_UFLAGS_USER            1

/* 规则和参数  */
#define  NCS_SYN_MESSAGE            0                     /* 消息              */
#define  NCS_SYN_MONIP              1                     /* 端口和IP地址监控  */
#define  NCS_SYN_MONWEB             2
#define  NCS_SYN_MONMAIL            3
#define  NCS_SYN_MONIM              4
#define  NCS_SYN_MONFORM            5   
#define  NCS_SYN_MONFTP             6
#define  NCS_SYN_MONDEFAULT         7

#define  NCS_SYN_LIMIP              8
#define  NCS_SYN_LIMWEB             9
#define  NCS_SYN_LIMIM              10
#define  NCS_SYN_LIMFORM            11
#define  NCS_SYN_LIMMAIL            12
#define  NCS_SYN_LIMVID             13
#define  NCS_SYN_MONBBS             14                  /* BBS监控   */
#define  NCS_SYN_NETSENSE           15                  /* 表单信息  */

/* 各类数据记录长度  */
#define  NCS_SYN_MONIPLEN           32                    
#define  NCS_SYN_MONWEBLEN          80
#define  NCS_SYN_MONMAILLEN         340
#define  NCS_SYN_MONIMLEN           148
#define  NCS_SYN_MONFORMLEN         588
#define  NCS_SYN_MONFTPLEN           20
#define  NCS_SYN_MONBBSLEN          204
#define  NCS_SYN_MONDEFAULTLEN       16

#define  NCS_SYN_LIMIPLEN           56
#define  NCS_SYN_LIMWEBLEN          100
#define  NCS_SYN_LIMIMLEN           360
#define  NCS_SYN_LIMFORMLEN         864
#define  NCS_SYN_LIMMAILLEN         808
#define  NCS_SYN_LIMVIDLEN          104


/* 通用信息  */
#define  NCS_SYN_IPPOLTYPE          0
#define  NCS_SYN_IPPOL              1
#define  NCS_SYN_WEBPOLCLASS        2
#define  NCS_SYN_WEBPOLLIST         3
#define  NCS_SYN_IPLIST             4
#define  NCS_SYN_IPINDEX            5
#define  NCS_SYN_WEBCLASS           6
#define  NCS_SYN_WEBLIST            7
#define  NCS_SYN_MAILCLASS          8
#define  NCS_SYN_MAILADDR           9
#define  NCS_SYN_USERIDCLASS        10
#define  NCS_SYN_USERIDLIST         11
#define  NCS_SYN_LIMDATEDES         12
#define  NCS_SYN_LIMDATE            13
#define  NCS_SYN_LIMWARNTYPE        14
#define  NCS_SYN_LIMWARNNOTICE      15
#define  NCS_SYN_NCSKEYCONT         16
#define  NCS_SYN_NCSKEYINDEX        17
#define  NCS_SYN_POSTACCOUNT        18     // 表单账号信息

#define  NCS_SYN_IPPOLTYPELEN       76
#define  NCS_SYN_IPPOLLEN           16
#define  NCS_SYN_WEBPOLCLASSLEN     76
#define  NCS_SYN_WEBPOLLISTLEN      16
#define  NCS_SYN_IPLISTLEN          88
#define  NCS_SYN_IPINDEXLEN         68

#define  NCS_SYN_WEBCLASSLEN        48
#define  NCS_SYN_WEBLISTLEN         140
#define  NCS_SYN_MAILCLASSLEN       48
#define  NCS_SYN_MAILADDRLEN        140
#define  NCS_SYN_USERIDCLASSLEN     48
#define  NCS_SYN_USERIDLISTLEN      140

#define  NCS_SYN_LIMDATEDESLEN      36
#define  NCS_SYN_LIMDATELEN         16
#define  NCS_SYN_LIMWARNTYPELEN     44
#define  NCS_SYN_LIMWARNNOTICELEN   48
#define  NCS_SYN_NCSKEYCONTLEN      76
#define  NCS_SYN_NCSKEYINDEXLEN     40
#define  NCS_SYN_POSTACCOUNTLEN     172   // 表单账号信息长度



/* 在线信息  */
#define  NCS_DATA_USEDMARK         88   // 数据标识
#define  NCS_MAX_ONLINE            20
#define  NCS_ONLINE_IP              0    // IP访问信息
#define  NCS_ONLINE_WEB             1    // Http访问
#define  NCS_ONLINE_ERRLOG          2    // 阻断信息
#define  NCS_ONLINE_WARN            4    // 告警信息
#define  NCS_ONLINE_MAIL            5    // 邮件信息
#define  NCS_ONLINE_FORM            6    // 表单信息
#define  NCS_ONLINE_IM              7    // 即时通信
#define  NCS_ONLINE_USERLOG         8    // 用户登录
#define  NCS_ONLINE_NETID           9    // 网络标识
#define  NCS_ONLINE_TELNET          10   // Telnet ftp
#define  NCS_ONLINE_USERLOGN        11   // 用户上网记录--替代10

/* 日志信息  */
#define  NCS_DBLOG_USER             0    // 用户信息
#define  NCS_DBLOG_GROUP            1    // 部门信息
#define  NCS_DBLOG_MAILMON          2    // 邮件 
#define  NCS_DBLOG_FORMMON          3    // 表单
#define  NCS_DBLOG_MAILCTL          4    // 拦截邮件
#define  NCS_DBLOG_IPLOG            5    // IP访问
#define  NCS_DBLOG_HTTPLOG          6    // Http访问
#define  NCS_DBLOG_IMLOG            7    // 即时通信
#define  NCS_DBLOG_ERRLOG           8    // 阻断日志
#define  NCS_DBLOG_WARNLOG          9    // 告警日志
#define  NCS_DBLOG_USERLOG          10   // 用户上网记录
#define  NCS_DBLOG_NETIDLOG         11   // 网络身份上网记录
#define  NCS_DBLOG_NETID            12   // 网络身份资料
#define  NCS_DBLOG_ACCOUNTLOG       13   // 表单账号信息
#define  NCS_DBLOG_BBSMON           14   // BBS信息
#define  NCS_DBLOG_IPSERVICE        15   // IPService
#define  NCS_DBLOG_QQNUMBER         16   // QQ号码
#define  NCS_DBLOG_NETSENSE         17   // 网络会话信息
#define  NCS_DBLOG_SEARCHLOG        18   // 搜索引擎使用情况

#define     NCSCTL_DEFKEY       "Pro@#$NetW12%^As123QW"


/* 进程信息  */
typedef struct ncsProcessInfo_s {
    int           iPid;
    unsigned long lStartTime;       /* 启动时间  */
    unsigned long lTimeOut;         /* 超时时间  */
    unsigned long lOntime;          /* 在每天指定时间重新启动  */
    unsigned char caControl[32];    /* 控制信息  */
    char          caName[32];       /* 功能名称  */
    int   (*fFunName)(utShmHead *); /* 函数名    */
    unsigned long lLastTime;        /* 进程最后操作时间                                 */
    unsigned long lStepTime;        /* 最大间隔时间，通常超过该时间表明进程有问题         */
    char     caRev[32];             /* 保留，不同的进程作用不同                         */
    int      iFlags;                /* 0--无  1--内部进程  2--外部                     */
} ncsProcessInfo;


/* 同步标识  */
typedef struct ncsSynMark_s { 
    unsigned long  id;       // 记录ID
    unsigned char  ntype;    // 表名
    unsigned char  gtype;    // 类别
    unsigned char  status;   // 0--开始  1--完成  
    unsigned char  rev;      // 保留
    unsigned long  pos;      // 在表中的偏移量
    unsigned long  check;    // 在表中的偏移量
    
} ncsSynMark;


/* 同步信息, 主机字节顺序 
 */
typedef struct ncsSynInfo_s {
    unsigned long  allchsum;       /* 总的校验码          */
    unsigned long  checksum[NCS_MAX_SYNINFO];   /* 每一个记录的校验码  */
} ncsSynInfo;

/* 策略索引, 存放在一个Hash表中  */
typedef struct ncsSynIndex_s {
    unsigned long  lId;            /* 0--Check  1--LastTime   */
    unsigned char  ntype;          /* 数据表                                           */
    unsigned char  gtype;          /* 组类别   0-通用 1-单位 2--组 3--全部 4--策略     */
    unsigned short nLen;           /* 记录大小   */
    unsigned long  lSum;           /* 记录数量   */
    unsigned long  lStart;         /* 起始偏移量 */
    unsigned long  lCheckSum;      /* 校验码     */
}  ncsSynIndex;   



/* 客户端信息资料  */
typedef struct ncsClient_s {
    unsigned long  userid;              // 用户ID
    int            num;                 // 在内存中的序号        
    char           username[16];        // 用户名
    char           password[32];        // 口令
    char           dispname[64];        // 显示名
    unsigned long  groupid;             // 组ID
    unsigned long  usertype;            // 单位类别
    int            gnum;                // 在组信息中的序号
    char           useflags;            // 用户状态  
    char           status;              // 当前状态  0--离线  1--在线  2--异常  
    char           cmod;                // 修改状态  0--正常  1--新增  2--修改   9--删除
    char           csync;               // 同步状态  0--无
    int            errcount;            // 错误计数，连续多次出错，系统将不予应答
    unsigned long  lLastTime;           // 最后操作时间
    ncsSynInfo     syninfo;
    ncsSynInfo     syndata;             // 数据同步记录
    unsigned long  lCaseCode;           // 布控下载代码 0--没有  >0 正在同步,开始同步时间
    unsigned char  caVer[12];           // 客户端版本
    unsigned char  caMsg[64];           // 备注信息
    double         fx;                  // 经度
    double         fy;                  // 纬度
} ncsClient;

/* 告警邮件地址或手机号*/
typedef struct ncsCaseMob_s {
    unsigned long  cid;              // 案件ID
    char           waddr[64];        //邮件地址或手机号
    int            wtype;           // 告警方式 1－邮件 2－短信
} ncsCaseMob;
/*告警邮件记录*/
typedef struct ncsMailWarnRecord_s {
    unsigned long  md5;              // 生成的MD5
    char           mesg[512];        //告警内容
    int            lasttime;           //上次发送时间
} ncsMailWarnRecord;



/* 单位类别   */
typedef struct ncsGroup_s {
    unsigned long  groupid;              // 组ID
    int            num;
    char           groupname[32];        // 组名
    ncsSynInfo     syninfo;
} ncsGroup;

/* 在线客户信息  */
typedef struct ncsOnline_s {     
    unsigned long  userid;
    int            num;            /* 在用户资料中的序号  */
    char           caKey[24];      /* 通信Key             */
    unsigned long  lSip;           /* IP地址              */
    unsigned short nPort;          /* 端口                */
    char           cSyn;           /* 同步状态            */
    char           cMsg;           /* 消息                */
    long long      lBytes[2];      /* 当前速率 字节/秒    */
    unsigned long  lTcp;
    unsigned long  lUser;
    unsigned long  lStartTime;
    unsigned long  lLastTime;           /* 最后操作时间         */
    unsigned long  lSynCheck;           /* 同步数据的校验码,在登录的时候计算  */
    unsigned long  lHeatCount;          // 心跳记录
    unsigned long  lHeatRecTime;        // 心跳记录时间
    unsigned long  long lSendBytes;     // 发送数据字节数
    unsigned long  long lRecvBytes;     // 接收数据字节数
    char           cflags;              // 0-探针  1-通过分管理中心转发   8-模拟
    char           cRev[3];
    ncsClient      *psClient;          // 类别
} ncsOnline;


typedef struct ncsBackOnline_s {
    ncsOnline  sOnline;
    struct ncsBackOnline_s *next;
} ncsBackOnline;

typedef struct ncsDemoClient_s {
    ncsClient  *psClient;
    unsigned long lSip;                    // Ip地址
    unsigned long lSumUser;                // 用户总数
} ncsDemoClient;



typedef struct ncsMonIp_s {
            unsigned long ftype;
            unsigned long sip;
            unsigned long eip;
            unsigned long sport;
            unsigned long dport;
            unsigned long service;
            unsigned long prot;
            unsigned long flags;
} ncsMonIp;

typedef struct ncsMonWeb_s {
            unsigned long ftype;
            unsigned char url[64];
            unsigned long cond;
            unsigned long urlid;
            unsigned long flags;
} ncsMonWeb;

typedef struct ncsMonMail_s {
            unsigned long ftype;
            unsigned char sender[64];
            unsigned long sendid;
            unsigned char toname[64];
            unsigned long toid;
            unsigned char subject[64];
            unsigned char attname[64];
            unsigned long msize;
            unsigned char content[64];
            unsigned long flags;
} ncsMonMail;


typedef struct ncsMonIm_s {
            unsigned long ftype;
            unsigned long service;
            unsigned char uname[64];
            unsigned long uid;
            unsigned long fun;
            unsigned char content[64];
            unsigned long flags;
} ncsMonIm;

typedef struct ncsMonForm_s {
            unsigned long ftype;
            unsigned char url[64];
            unsigned long urlid;
            unsigned char varname[256];
            unsigned char content[256];
            unsigned long flags;
} ncsMonForm;

typedef struct ncsMonBbs_s {
   unsigned long ftype;
   unsigned long urlid;
   unsigned char url[64];
   unsigned char bbsname[64];
   unsigned char content[64];
   unsigned long flags;
} ncsMonBbs;


typedef struct ncsMonFtp_s {
            unsigned long ftype;
            unsigned long service;
            unsigned long sip;
            unsigned long eip;
            unsigned long flags;
} ncsMonFtp;

typedef struct ncsMonDefault_s {
            unsigned long ftype;
            unsigned long timestep;
            unsigned long ipenable;
            unsigned long ipdefault;
            unsigned long webenable;
            unsigned long webdefault;
            unsigned long mailenable;
            unsigned long maildefault;
            unsigned long imenable;
            unsigned long imdefault;
            unsigned long ftpenable;
            unsigned long ftpdefault;
            unsigned long formenable;
            unsigned long formdefault;
} ncsMonDefault;

/* 通用信息  */
typedef struct ncsIppoltype_s {
            unsigned long pid;
            char          name[64];
            unsigned long vister;
            unsigned long warnid;
} ncsIppoltype;

        
typedef struct ncsIppol_s {
    unsigned long  pid;
    unsigned long  tid;
    unsigned long  vflags;
    unsigned long  warnid;
}  ncsIppol;

typedef struct ncsWebpolclass_s {
    unsigned long  pid;
    unsigned char  name[64];
    unsigned long  vister;
    unsigned long  warnid;
}  ncsWebpolclass;

typedef struct ncsWebpollist_s {
    unsigned long  pid;
    unsigned long  tid;
    unsigned long  vflags;
    unsigned long  warnid;
}  ncsWebpollist;

typedef struct ncsIplist_s {
    unsigned long  id;
    unsigned long  prot;
    unsigned long  lip;
    unsigned long  uip;
    unsigned long  lport;
    unsigned long  uport;
    unsigned char  name[64];
}  ncsIplist;


typedef struct ncsIpIndex_s {
    unsigned long  id;
    unsigned char  name[64];
}  ncsIpIndex;


typedef struct ncsWebClass_s {
    unsigned long  modtime;
    unsigned long  flags;
    unsigned char  name[32];
    unsigned long  id;
    unsigned long  status;
}  ncsWebClass;

typedef struct ncsWebList_s {
    unsigned long  modtime;
    unsigned long  flags;
    unsigned char  url[64];
    unsigned char  name[64];
    unsigned long  id;
}  ncsWebList;


typedef struct ncsMailClass_s {
    unsigned long  modtime;
    unsigned long  flags;
    unsigned char  name[32];
    unsigned long  id;
    unsigned long  status;
}  ncsMailClass;

typedef struct ncsMailAddr_s {
    unsigned long  modtime;
    unsigned long  flags;
    unsigned char  mail[64];
    unsigned char  name[64];
    unsigned long  id;
}  ncsMailAddr;


typedef struct ncsUserIdClass_s {
    unsigned long  modtime;
    unsigned long  flags;
    unsigned char  name[32];
    unsigned long  id;
    unsigned long  status;
}  ncsUserIdClass;

typedef struct ncsUserIdList_s {
    unsigned long  modtime;
    unsigned long  flags;
    unsigned char  uname[64];
    unsigned char  descr[64];
    unsigned long  id;
}  ncsUserIdList;


typedef struct ncsLimDatedes_s {
    unsigned long  id;
    unsigned char  name[32];
}  ncsLimDatedes;

typedef struct ncsLimDate_s {
    unsigned long  id;
    unsigned long  ltype;
    unsigned long  llower;
    unsigned long  lupper;
}  ncsLimDate;

typedef struct ncsLimWarnType_s {
    unsigned long  id;
    unsigned char  name[32];
    unsigned long  localwanr;
    unsigned long  admcenter;
}  ncsLimWarnType;

typedef struct ncsLimWarnNotice_s {
    unsigned long  id;
    unsigned char  name[32];
    unsigned long  mesgnote;
    unsigned long  mailnote;
    unsigned long  mobile;
}  ncsLimWarnNotice;




typedef struct ncsLimWeb_s {
    unsigned char  gtype;
    unsigned char  cRev;
    unsigned char  conflags;
    unsigned char  flags;
    unsigned long  lId;            /* ID         */
    unsigned long  urlid;
    char           url[64];
    unsigned long  timetype;
    unsigned long  starttime;
    unsigned long  stoptime;
    unsigned long  warntype;
}  ncsLimWeb;



/* 实时信息  */
typedef struct ncsRealInfo_s {
    unsigned char   cMark;         /* 信息标识, 若有数据该字段置为88   99--结束 */
    unsigned char   cType;         /* 信息类别        */
    unsigned short  nLen;          /* 记录长度        */
} ncsRealInfo;


/* 在线网络访问      */
typedef struct ncsOnlineIndex_s {
    unsigned long   starttime; // 起始时间
    unsigned long   lasttime;  // 最后同步时间
    unsigned long   lastnum;   // 最后记录号
    unsigned long   maxrec;    // 最大记录数
    unsigned long   lstart;    // 在内存中的起始位置
    unsigned short  reclen;    // 每条记录的字节数
    unsigned short  lock;      // 锁
    unsigned long   count;     // 0--刚开始  >0--已循环使用
} ncsOnlineIndex;

/* 在线网络访问      */
typedef struct ncsOnBufHead_s {
    unsigned long   rtime;     // 接收时间
    unsigned long   did;       // 单位ID
    unsigned long   tid;       // 类型ID
    unsigned short  nlen;      // 记录长度
    unsigned short  nrev;
} ncsOnBufHead;


typedef struct ncsOnlineIp_s {
    unsigned long   did;       // 单位ID
    unsigned long   uid;       // 用户ID
    unsigned long   tid;       // 单位类别
    char            udisp[32]; // 用户名
    unsigned long   stime;     // 时间
    unsigned long   ctime;     // 连接时间
    unsigned long   sip;       // 源IP地址
    unsigned long   dip;       // 目标IP地址
    unsigned short  prot;      // 协议  6:Tcp  17:UDP
    unsigned short  port;      // 端口
    unsigned long   service;   // 服务协议
    unsigned long   ubytes;    // 上行字节数
    unsigned long   dbytes;    // 下行字节数
} ncsOnlineIp;
/*上网用户部门信息*/
typedef struct ncsEndGroup_s {
            unsigned long did;
            unsigned long gid;
            char name[32];
} ncsEndGroup;
/*虚拟身份类别*/
 typedef struct ncsNettype_s {
            long id;
            char caName[32];
 } ncsNettype;


#define NCS_MAX_SYNTAB    16       // 最大同步的表
#define NCS_SYNTAB_ONLINEUSR  0    // 在线客户端同步时间
#define NCS_SYNTAB_NCSUSER    1    // 客户端资料
#define NCS_SYNTAB_GROUP      2    // 组信息
#define NCS_SYNTAB_ENDGROUP   3    // 场所端组信息
#define NCS_SYNTAB_ENDUSER    4    // 最终用户
#define NCS_SYNTAB_USERIP     5    // 联网单位IP轨迹
#define NCS_SYNTAB_CASEMSG    6    // 告警信息
#define NCS_SYNTAB_NETID      7    // 虚拟身份信息
#define NCS_SYNTAB_USERLOG    8    // 用户上网记录
#define NCS_SYNTAB_NETLOG     9    // 虚拟身份轨迹


#define NCS_TAB_CLIENT      1001
#define NCS_TAB_GROUP       1002
#define NCS_TAB_ENDGROUP    1003
#define NCS_TAB_ENDUSER     1004
#define NCS_TAB_USERIP      1005 
#define NCS_TAB_CASEMSG     1006
#define NCS_TAB_NETID       1007
#define NCS_TAB_USERLOG     1008 
#define NCS_TAB_NETLOG      1009 
#define NCS_TAB_SUBCROP     1010

/* 分管理中心信息，在内存中顺序存放 */
typedef struct ncsSubAdm_s {
    unsigned long  id;                  // 用户ID
    char           username[16];        // 用户名
    char           password[32];        // 口令
    char           key[24];             // 通信密钥
    char           dispname[64];        // 显示名
    char           version[12];         // 客户端版本号
    unsigned long  lasttime;            // 最后操作时间
    unsigned long  savetime;           // 状态保存时间
    unsigned long  lDeptTime;           // 部门信息更新最后时间
    unsigned long  lSip;                // 客户端IP地址
    unsigned long  lPid;                // 缺省类别
    unsigned long  lSynFlags[NCS_MAX_SYNTAB]; 
    unsigned short nPort;               // Tcp端口
    char           status;              // 当前状态  0--离线  1--在线  2--异常  
    char           rev[17];             // 保留
} ncsSubAdm;

/* 本机当前信息 */
typedef struct ncsMyStatus_s {
    unsigned long  id;                  // 用户ID
    unsigned long  pid;                 // 上级组
    char           username[16];        // 用户名
    char           password[32];        // 口令
    char           key[24];             // 通信密钥
    char           dispname[64];        // 显示名
    char           version[12];         // 客户端版本号
    unsigned long  lasttime;            // 最后操作时间
    unsigned long  srvnum;              // 在上级管理中心中的序号
    unsigned long  lDeptTime;           // 部门信息更新最后时间
    unsigned long  lSip;                // 服务器地址
    unsigned short nPort;               // 服务器Tcp端口
    unsigned short nTcpPort;            // 本机TCp端口
    unsigned long  lSynFlags[NCS_MAX_SYNTAB];
    unsigned long  lThisFlags[NCS_MAX_SYNTAB]; 
    unsigned long  lTimeStep;           // 心跳时间间隔
    unsigned long  lLogTimeStep;        // 日志时间间隔
    unsigned long  lMinCaseId;          // 本机最小CaseId
    unsigned long  lMaxCaseId;          // 本机最大CaseId
    char           status;              // 当前状态  -1 出错 0--离线  1--在线  2--异常  
    char           sendtype;            // 发送方式  0-TCP  1-Ftp
    char           rev[14];             // 保留
    unsigned long  lCaseTime;           // 布控案件最后时间
    
} ncsMyStatus;

/* 系统总体信息   */
typedef struct ncsSystemInfo_s {
    utShmHead      *psShmHead;
    ncsClient      *psClient;
    ncsGroup       *psGroup;
    ncsSubAdm      *psSub;
    ncsMyStatus     sMyInfo;       /* 本机信息                  */
    int             iSumClient;     /* 单位总数                  */
    int             iSumGroup;
    int             iSumSubAdm;     /* 分管理中心的总数          */
    unsigned long   lMyId;          /* 自己的ID                  */
    unsigned char   caHome[64];     /* 主目录                    */
    unsigned long   lLastTime;      /* 系统最后操作时间          */
    unsigned long   lStartTime;     /* 系统启动时间              */
    unsigned long   lCaseTime;      /* 案件布控信息最后更新时间  */
    unsigned long   lMsgTime;       /* 消息更新时间              */
    unsigned long   lComTime;       /* 命令时间                  */
    ncsSynInfo      syncomm;        /* 各种通用的描述信息        */
    ncsSynInfo      synall;         /* 全局信息                  */
    ncsCaseIndex    *psCase;        // 案件信息
    int             iSumCase;       // 案件总数
    ncsCaseContHead *psCaseCont;    // 布控内容
    int             iSumCaseArea;   // 布控区域
    ncsCaseArea     *psCaseArea;    // 布控区域
    char            password[32];   // 密钥        
    char            caMinVar[12];   // 要求客户端最低版本     
} ncsSystemInfo;

typedef struct ncsEndUser_s {
    

} ncsEndUser;


/* 需要下发的命令列表   
   数组，以时间从大到大排序
 */
typedef struct ncsDownloadCom_s {
    unsigned char  cGtype;            // 0--全局  1--组   2--类别  3--个人
    unsigned char  cRev[3];
    unsigned long  lId;
    unsigned long  lPos;              // 命令文本所在位置
    unsigned long  lBytes;            // 命令字节数
    unsigned long  lTime;             // 命令修改时间
    unsigned long  lExpireTime;       // 命令失效时间
} ncsDownloadCom;

typedef struct ncsDownloadComHead_s {
    unsigned long lSumBytes;             // 数组总的字节数
    unsigned long lLoadTime;             // 装载时间
    unsigned long lLastTime;             // 最后修改时间
    unsigned long lSumCom;               // 命令总数
    unsigned char *pBase;                // 基准地址
    ncsDownloadCom *psCom;
} ncsDownloadComHead;

typedef struct ncsSmsModenMesg_s {
    char caNo[20];
    char caMesg[128];
} ncsSmsModenMesg;    
typedef struct ncsSmsNo_s {
    char caNo[20];
    char caPass[20];
    char caMac[20];
    unsigned long flag;
    unsigned long lasttime;
} ncsSmsNo;

typedef struct ncsSmsOnline_s {         /*存放每个场所短信信息*/
    char caSn[16];                  /*场所端序列号*/
    long lFlag;                      /*1每天 2-每周 2-每月*/
    long lLimCount;                  /*限定短信数*/
    long lCount;                     /*已发短信数*/
    char caUptime[12];               /*限定数设置时间*/
} ncsSmsOnline;
typedef struct ncsuserCode2Id_s {           
    char   code[32];                  //场所代码
    unsigned long id;               //单位ID      
} ncsuserCode2Id; 

typedef struct ncsMacProcyObj_s {           
    char   code[32];                  // cid_type_did           type_id值为9_9 时代表
    unsigned long id;               //ID  部门，或单位ID 0 代表全部    
} ncsMacProcyObj; 
typedef struct ncsBabProcyObj_s {           
    char   code[32];                  // cid_type_did           type_id值为9_9 时代表
    unsigned long id;               //ID  部门，或单位ID 0 代表全部    
} ncsBabProcyObj; 
typedef struct ncsMacFrontpageObj_s {           
    uint8 userId;                      //用户id
    ulong lastAskTime;                 //最新请求时间
    ulong lastModTime;                 //最新更新时间
    ulong todayMacCount;               //今日新增mac数
    ulong totalMacCount;               // 总mac数
} ncsMacFrontpageObj; 


#endif
