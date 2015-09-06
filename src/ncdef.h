/* #include  "ncshmdebug.h"  */
#ifndef __NETCHARGDEF__
#define __NETCHARGDEF__
#define   NC_MAX_PROCESS        16   /* 最大进程类型  */
#define   NC_MAX_CONTROL        32   /* 最大控制变量  */
#define   NC_MAX_ONTIMEDO       16   /* 最大定时函数  */
#define   NC_DEFAULT_IP_CHECK   20
#define   NC_DEFAULT_IP_TIMEOUT 30
#define   NC_MAX_USERNUM      257
#define   NC_MAX_IPPKG        9999
#define   NC_MAX_HTTPNUM      999
#define   NC_MAX_LIMITED      100
#define   NC_MAX_MONITOR      100
#define   NC_MAX_INTERNAL     100
#define   NC_MAX_LASTIP       1000
#define   NC_CONTENT_GET      10
#define   NC_CONTENT_POST1    11
#define   NC_CONTENT_POST2    12
#define   NC_CONTENT_POST3    13
#define   NC_CONTENT_SMTP1    21
#define   NC_CONTENT_SMTP2    22
#define   NC_CONTENT_POP3     23
#define   NC_MAX_NOMONWEB     50
#define   NC_PF_UNUSED        0
#define   NC_PF_INUSED        1
#define   NC_LNK_THIRDAUTH  25  /* 第三方认证的用户  */
#define   NC_LNK_ALLUSER    26  /* 全部用户信息      */
#define   NC_LNK_MAILCTL    27  /* 邮件控制          */
#define   NC_LNK_CONTROL    28  /* 访问控制          */
#define   NC_LNK_IPFLOW     29  /* 总流量            */
#define   NC_LNK_MAIL       30  /* 邮件数据包        */
#define   NC_LNK_IPPKG      31  /* IP数据包          */
#define   NC_LNK_WEB        32  /* http数据包        */
#define   NC_LNK_IPLIMIT    33  /* 限制使用的IP地址  */
#define   NC_LNK_NETWORK    34  /* 要监控的IP地址  Add 2002/9/6 By Liyunming Replace
                                         NC_LNK_MONITOR NC_LNK_INTERNAL */

#define NC_LNK_PKGLOG       35  /* 重新装载控制  */
#define NC_LNK_USERINFO     36  /* 用户信息          */
#define NC_LNK_SERVICE      37  /* 众所周知的端口    */
#define NC_LNK_MMONFILTER   38  /* 邮件监控分类规则      */
#define NC_LNK_WEBLIMIT     39  /* Web访问限制       */
#define NC_LNK_IPLIST       40  /* IP地址列表        */
#define NC_LNK_IPPOL        41  /* 服务访问策略      */
#define NC_LNK_WEBLIST      42  /* Web地址列表        */
#define NC_LNK_IPPOLINDEX   43  /* 服务访问策略的索引  */
#define NC_LNK_GROUPINFO    44  /* 部门信息           */
#define NC_LNK_SUMMARY      45  /* 总体信息           */
#define NC_LNK_ONTIME       46  /* 定时程序           */
#define NC_LNK_PROCESS      47  /* 进程管理           */
#define NC_LNK_WEBPOLINDEX  48  /* 网站访问策略       */
#define NC_LNK_WEBPOL       49  /* 网站访问策略       */
#define NC_LNK_NTAUTH       50  /* NT验证             */
#define NC_LNK_FMONFILTER   51  /* 表单监控规则       */
#define NC_LNK_MCTLFILTER   52  /* 拦截邮件分类规则   */
#define NC_LNK_MAILPROXY    53  /* 收发邮件的目标IP地址  */
#define FIRE_LNK_DEFINE     54  /* 参数列表            */
#define FIRE_LNK_TABLES     55  /* 规则               */
#define FIRE_LNK_PLATE      56     /* 模板      */
#define FIRE_LNK_SUMMARY    57     /* 总体信息  */
#define NC_LNK_ICUSER       58     /* IC卡用户  */
#define NC_LNK_HTTPPROXY    59     /* http proxy    */
#define NC_LNK_POP3PROXY    60     /* pop3 proxy    */
#define NC_LNK_NOMONWEB     61     /* 免监控的网址  */
#define NC_LNK_MSNPKGBUF    62     /* MSN数据交换缓冲区,静态数组     */
#define NC_LNK_MSNUSRBUF    63     /* MSN在线用户缓冲区,Hash表       */
#define NC_LNK_CONTROLBUF   64     /* 访问控制缓冲区                 */
#define NC_LNK_BANDWIDTH    65     /* 带宽管理                       */
#define NC_LNK_PKGBUF       66     /* 数据包缓冲区                   */
#define NC_LNK_CONTBUF      67     /* 进一步处理的内容缓冲区         */
#define NC_LNK_IPUSER       68     /* 用户和IP缓存                   */
#define NC_LNK_DNSCASH      69     /* DNS Cash                       */
#define NC_LNK_HTTPCACHE    70     /* http cache                              */
#define NC_LNK_HTTPCACHEC   71     /* http cache content                      */
#define NC_LNK_ADMIPLIST    72     /* 管理员IP地址, 用来记录登录失败的IP地址  */
#define NC_LNK_IPEXCEPT     73     /* 不进行管理的例外IP地址表                */
#define NC_LNK_IPCONTACT    74     /* TCP连线信息                             */
#define NC_LNK_LANG         75    /* 语言对照表   */
#define NC_LNK_PKGSEQ       76    /* 序列号对照表, Hash表  */
#define NC_LNK_NCSRVINFO    77    /* 控制中心的有关信息    */
#define NC_LNK_NCPOLINFO    78    /* 访问控制策略索引      */
#define NC_LNK_MACLIST      79    /* 免监控的MAC地址       */
#define NC_LNK_BILLTYPE     80    /* 计费策略              */
#define NC_LNK_BILLRATE     81    /* 优惠政策              */
#define NC_LNK_BILLTIME     82    /* 时间费率              */
#define NC_LNK_BILLFLOW     83    /* 流量费率              */
#define NC_LNK_PROXYIP      84    /* Proxy的地址列表       */
#define NC_LNK_LOCK         85    /* 锁信息                */
#define NC_LNK_WEBNAME      86    /* 网址库对应的名字      */
#define NC_LNK_COMPUTE      87    /* 内网计算机列表        */
#define NC_LNK_DEVINFO      88    /* 设备信息缓存          */
#define NC_LNK_IPMACLIST    89    /* IP地址、MAC地址对照表          */
#define NC_LNK_MACIPLIST    90    /* MAC地址、IP地址、计算机名对照表*/
#define NC_LNK_DBSYNCINFO   91    /* 数据同步在线信息               */
#define NC_LNK_ONLINECOMP   92    /* 在线计算机情况                 */
#define NC_LNK_NCMAILLIST   93    /* 邮件地址黑白名单               */
#define NC_LNK_GROUPIPLST   94    /* 缺省IP和部门对照表             */


/* 异步控制命令                   */


#define NC_IPCONTROL_BY_ICMP      0
#define NC_IPCONTROL_BY_IPTABLES  1
#define NC_SERVICE_OTHER  0    /* 未知服务           */
#define NC_SERVICE_TELNET 1    /* Telnet 服务        */
#define NC_SERVICE_FTP    2    /* Ftp    服务        */
#define NC_SERVICE_HTTP   3    /* Web服务            */
#define NC_SERVICE_HTTP1  300  /* Web服务            */
#define NC_SERVICE_SMTP   4    /* SMTP服务           */
#define NC_SERVICE_POP    5    /* POP服务            */
#define NC_SERVICE_QQ     6    /* QQ                */
#define NC_SERVICE_YHMSG  7    /* YHMSG               */
#define NC_SERVICE_IRC    8    /* IRC                */
#define NC_SERVICE_VIDEO  9    /* IRC                */
#define NC_SERVICE_MSN    10    /* 视频                */
#define NC_SERVICE_ICQ    11   /* ICQ                */
#define NC_SERVICE_P2P    12   /* P2P服务             */
#define NC_SERVICE_P2P_BASE  1200
#define NC_SERVICE_SSH    15   /* ssh                */


#define NC_HTTP_GET       1
#define NC_HTTP_POST      2
#define NC_QQ_LOGIN       1
#define NC_QQ_LOGOUT      2
#define NC_QQ_SENDMSG     3

#define NC_IM_LOGIN       1
#define NC_IM_LOGOUT      2
#define NC_IM_SENDMSG     3



#define NC_PROTOCOL_ICMP  1
#define NC_PROTOCOL_IGMP  2
#define NC_PROTOCOL_TCP   6
#define NC_PROTOCOL_UDP   17

/*   Define the Debug Level   */
#define NC_DEBUG_SHOWTCP      1
#define NC_DEBUG_SHOWCONTROL  2
#define NC_DEBUG_SHOWUDP      3
#define NC_DEBUG_SHOWICMP     4
#define NC_DEBUG_SHOWIGMP     5
#define NC_DEBUG_SHOWCHECKMAC 6
#define NC_DEBUG_SHOWSMTP     7
#define NC_DEBUG_SHOWPOP      8
#define NC_DEBUG_NTAUTH       9
#define NC_DEBUG_UPLOAD       10
#define NC_DEBUG_IPCONTROL    11
#define NC_DEBUG_MAILAUDIT    12     /* 邮件审计  */
#define NC_DEBUG_MAILNON      13     /* 邮件监控  */
#define NC_DEBUG_TIMECTL      14     /* 时间控制  */
#define NC_DEBUG_MSGINFO      15     /* 显示数据包   */
#define NC_DEBUG_CHECKNETIP   16     /* 检查网络情况 */
#define NC_DEBUG_THIRDAUTH    17     /* 第三方验证   */
#define NC_DEBUG_SAVEIPBUF    18     /* 定时保存缓冲区中的IP信息  */

/* 有关错误级别       */
#define NC_LOG_SUCESS          0      /* 一般日志         */
#define NC_LOG_ERROR           1      /* 严重错误         */
#define NC_LOG_WARN            2      /* 警告信息         */
#define NC_LOG_EVENT           4      /* 重要事件         */
#define NC_LOG_DEBUG           8      /* 调试信息         */

#define NC_DEBUG_UPDATE       119     /* 在线升级的调试信息    */
#define NC_DEBUG_MAILCTL      120     /* 邮件监控              */

/* 禁止原因代码    */
#define NC_DENY_NONE           0    /* 直接禁止    */
#define NC_DENY_NORMAL         1    /* 一般禁用    */
#define NC_DENY_WEBPOL         2    /* 网址库策略  */
#define NC_DENY_SERVICE        3    /* 服务内容    */
#define NC_DENY_SERVICEPOL     4    /* 服务策略    */
#define NC_EXCEPT_FORBIT        1    /* 一般禁止   */
#define NC_EXCEPT_IPPOL         2    /* IP地址     */
#define NC_EXCEPT_WEBPOL        3    /* 网址库策略 */
#define NC_EXCEPT_IPNET         4    /* IP禁止上网 */
#define NC_EXCEPT_IPSERVICE     5    /* IP服务限制  */
#define NC_EXCEPT_IPPORT        6    /* 禁止访问IP地址和端口  */
#define NC_EXCEPT_WEBURL        7    /* 禁止访问指定的URL */

/*针对cStatus而言, 0 正常,未知
                   1 正常允许访问
                  >5 禁止访问
 */
#define NC_USER_NOUSE           0        /* Nouse                  */
#define NC_USER_NORMAL          1        /* Normal                 */
#define NC_USER_NOMON           2        /* 用户免监控             */

#define NC_USER_FORBIDCTL       5        /* 访问控制被禁止         */
#define NC_USER_FORBIDLIMIT     6        /* 用户访问超出限制       */
#define NC_USER_FORBIDNOUSER    7        /* 用户不存在             */
#define NC_USER_FORBIDMACERR    8        /* MAC地址不一致          */
#define NC_USER_USRFORBID       9        /* 用户禁用               */
#define NC_USER_FORBIDAUTH      10       /* 用户验证               */
#define NC_USER_FORBIDNAMEERR   11       /* 计算机名不一致         */
#define NC_USER_FORBIDIPERR     12        /* MAC地址不一致          */

/* 阻断级别  */
#define NC_FORBID_USER       1        /* 用户禁止访问互联网  */
#define NC_FORBID_SERVICE    2        /* 用户禁止访问某个服务 */
#define NC_FORBID_IP         3        /* 禁止访问某个IP地址   */
#define NC_FORBID_IPPORT     4        /* 禁止访问某个IP和PORT */
#define NC_FORBID_IPCONTRACT 5        /* 阻断某个连接         */

#define NC_DEF_IPADDRESS    1        /* 根据IP地址命名 */
#define NC_DEF_COMPUTER     2        /* 根据计算机名字命名  */
#define NC_DEF_RAND         0        /* 随机名字            */

#define NC_MCTL_QMAIL       1        /* Qmail 转发          */
#define NC_MCTL_SMTPPROXY  2        /* Smtp Proxy          */
#define NC_MCTL_POP3PROXY  3        /* Pop3 Proxy          */

#define NC_MCTL_NONE       0
#define NC_MCTL_SEND       1
#define NC_MCTL_REJECT     2
#define NC_MCTL_FORWARD    3
#define NC_MCTL_WAIT       4

#define NC_NAT_NATIP         1        /* Ip地址映射   */
#define NC_NAT_NATPORT       2        /* 端口重定向   */
#define NC_NAT_NATSNAT       3        /* 源地址伪装   */
#define NC_NAT_NATNAP        4        /* 源地址伪装   */

#define NC_NAT_ROUTE         8        /* 路由配置     */

#define NC_DBCHK_EXTENDED    1
#define NC_DBCHK_CHANGED     2
#define NC_DBCHK_FAST        3

#define   NC_IPCONTROL_ID       918123123L
#define NC_TABLES_REMOTEFILE       6 

#define NC_SIGNAL_RESETPKG 0        /* 重新初始化数据包    */

#define nc_copt_default    '^'      /* 缺省的匹配方式      */


/* 综合策略 和表 nclimsumm对应  */
typedef struct ncLimSummer_s {
    unsigned char ipaccess;
    unsigned char httpaccess;
    unsigned char postsend;
    unsigned char httpbyip;
    unsigned char limitunit;         /* 限制单位  1--每日  2--每周  3--每月 4--全部  */
    unsigned char overctl;           /* 超出限制后的处理方式                         */
    unsigned char ipbind;
    unsigned char macbind;
    unsigned char namebind;
    unsigned char onlyone;
    unsigned char cRev[2];
    unsigned long long flowlimit;    
    unsigned long timelimit;    
    unsigned long overid;    
    unsigned long feeid;  
    unsigned long polid;  
}  ncLimSummer;

typedef struct ncLimPolInfo_s {        /* 访问控制策略信息  */
    unsigned long lId;                 /* 策略ID            */
    char           caName[16];          /* 策略名称          */
    ncLimSummer    sSumm;              /* 综合参数          */
    unsigned short nLimIp;             /* Ip控制起始序号    */
    unsigned short nLimWeb;            /* Web控制起始序号   */
} ncLimPolInfo;

   

/* 组的管理  从小到大排列  */
typedef struct ncLimGroup_s {    /* 组信息  */
    unsigned long  lId;
    char           groupname[16];
    ncLimSummer    sSumm;
    unsigned short nLimIp;
    unsigned short nLimWeb;
    ncLimPolInfo   *psMypol;
} ncLimGroup;


typedef struct ncUserCont_s {        /* 用户信息       */
    unsigned long  userid;           /* 0--用户不存在  */
    char           username[16];     /* 用户名         */
    char           dispname[16];     /* 显示名         */
    char           compname[16];     /* 计算机名       */
    char           password[24];     /* 口令           */
    unsigned long  groupid;          /* 组ID           */
    unsigned long  ip;               /* IP地址         */
    unsigned long  thistime;         /* 当前所用时间   */
    unsigned long  resettime;        /* 初始化时间     */
    long  long thisflow;             /* 当前流量       */
    unsigned long  lasttime;         /* 最后修改时间   */
    ncLimSummer    sSumm;            /* 总体控制信息   */
    ncLimGroup     *psGroup;
    ncLimPolInfo   *psMypol;
    unsigned short nLimWeb;          /* 网站限制访问, 序号从1开始   */
    unsigned short nLimIp;           /* IP限制访问     */
    double         fee;              /* 账户余额       */
    unsigned char  mac[6];
    unsigned char  useflags;         /* 用户状态  0 正常   1 禁用  9 免监控            */
    unsigned char  modflags;         /* 修改标识  0--没改  1--新增   2--修改   9--删除*/
    unsigned char  userbase;         /* 用户管理方式                                  */
    unsigned char  rev[3];           /* 保留                                          */
} ncUserCont;


#define NC_IPBASE_PRE    999
#define NC_MACBASE_PRE   111
#define NC_IDBASE_PRE    222

/* 通过Hash表来建立索引  */
typedef struct ncUserIpIndex_s {
    unsigned long pre;
    unsigned long ip;
    unsigned long lIndex;
} ncUserIpIndex;

typedef struct ncUserMacIndex_s {
    unsigned short pre;
    unsigned char  mac[6];
    unsigned long  lIndex;
} ncUserMacIndex;    

typedef struct ncUserIdIndex_s {
    unsigned long pre;
    unsigned long lId;
    unsigned long lIndex;
} ncUserIdIndex;    

typedef struct ncUserContHead_s {
    unsigned long lMaxUser;
    unsigned long lSumUser;
    unsigned long lHashPos;     /* Hash表的偏移量    */
    unsigned long lContPos;     /* 用户内容的偏移量  */
    unsigned long lLasttime;    /* 最后修改时间      */
    int           iModFlags;    /* 修改标识 0--未改过  1--已作修改  */
    ncUserCont    *psUser;
    char          *psHash;
} ncUserContHead;

#define NC_LOGIN_IP         2      /* 通过IP地址管理上网    */
#define NC_LOGIN_MAC        3      /* 通过MAC地址管理上网   */
#define NC_LOGIN_IC         4

#define NC_LOGIN_AUTH       3      /* 通过本地验证管理上网  */
typedef struct ncUserInfo_s {      /* 用户信息       */
    unsigned long  lIp;            /* 0 表示没有用户 */
    unsigned long  lId;            /* 用户Id  0--没用                          */
    unsigned long  lGid;           /* 组Id   groupid=0 未知用户  1=服务器      */
    unsigned char  cUseflags;      /* 使用状态0--正常 1--禁用  9--免监控            */
    unsigned char  cStatus;        /*                                               */
    unsigned char  mac[6];         /* MAC地址           */
    unsigned long  lStartTime;     /* 开始连接时间      */
    unsigned long  lLastTime;      /* 最后访问时间      */
    unsigned long  lLastCountTime; /* 最后一次计算时间  */
    unsigned long  lConnTime;      /* 本次连接时间,从连接时间起算   */
    unsigned long  lSaveTime;      /* 上次保存的连接时间  */
    long8          lSaveBytes[2];  /* 上次保存的流量 */
    long8          lBytes[2];      /* 0-总的上行流量 1--下行流量 从连接时间开始计算     */
    long8          lSumflow;       /* 从开始计时到当前流量总数                      */
    unsigned long  lSumtime;       /* 从开始计时到当前时间总数                       */
    long8          lSflow;         /* 起始流量             */
    unsigned long  lStime;         /* 起始时间             */
    unsigned long  lPkg[2];        /* 数据包数量 0:dat 1:syn  根据该值来判断TCP连接是否正常 */
    ncUserCont     *psUser;        /* 针对用户内容         */
    unsigned long  lCltLastTime;   /* 和客户端最后交互信息     */
    unsigned long  lCltId;         /* 客户端ID                 */
    unsigned char  cLogin;         /* 登录方式    2 基于IP地址  3 基于MAC地址 4 本机验证 5 第三方验证  6 客户端方式 */
    unsigned char  cMacCheck;      /* 0--NoCheck 1--Check  */
    unsigned char  cRev[2];
} ncUserInfo;

/* 备份的在线信息  */
typedef struct ncUserBack_s {      /* 用户信息       */
    unsigned long  lIp;            /* 0 表示没有用户 */
    unsigned long  lId;            /* 用户Id  0--没用                          */
    unsigned long  lGid;           /* 组Id   groupid=0 未知用户  1=服务器      */
    unsigned char  cUseflags;      /* 使用状态0--正常 1--禁用  9--免监控            */
    unsigned char  cStatus;        /*                                               */
    unsigned char  mac[6];         /* MAC地址           */
    unsigned long  lStartTime;     /* 开始连接时间      */
    unsigned long  lLastTime;      /* 最后访问时间      */
    unsigned long  lLastCountTime; /* 最后一次计算时间  */
    unsigned long  lConnTime;      /* 本次连接时间,从连接时间起算   */
    unsigned long  lSaveTime;      /* 上次保存的连接时间  */
    long8          lSaveBytes[2];  /* 没有保存的流量 */
    long8          lBytes[2];      /* 0-总的上行流量 1--下行流量 从连接时间开始计算     */
    long8          lSumflow;       /* 从开始计时到当前流量总数                      */
    unsigned long  lSumtime;       /* 从开始计时到当前时间总数                       */
    long8          lSflow;         /* 起始流量     */
    unsigned long  lStime;         /* 起始时间     */
    unsigned long  lPkg[2];        /* 数据包数量 0:dat 1:syn  根据该值来判断TCP连接是否正常 */
    unsigned char  cLogin;         /* 登录方式    2 基于IP地址  3 基于MAC地址 4 本机验证 5 第三方验证 */
    unsigned char  cRev[3];
    struct ncUserBack_s   *next;
} ncUserBack;

/* 系统锁信息         */
#define NC_LOCK_SYSREFRESH       0       /* 系统刷新     */
#define NC_LOCK_UPDATEURL        1       /* 在线升级URL  */
#define NC_LOCK_AUTOCLEAN        2       /* 自动整理     */

/* 锁记录            */
typedef struct ncLockInfo_s {
    int  iLock;                  /*  1--锁定    0--锁定  */
    unsigned long lTimeout;      /*  超时时间            */
    unsigned long lLasttime;     /*  上锁时间            */
    int  iPid;                   /*  进程号              */
    char     caDesc[16];         /*  描述                */
    char     caMsg[128];         /*  当前信息            */
} ncLockInfo;    

/* 
    # --- 开头
    ^ --- 包含
    = --- 等于
    ! --- 不
    $ --- 结束    

 */
typedef struct ncUtlStrStr_hs {
    unsigned char        cMark;         /* 带结构     */
    unsigned char        cSum;          /* 总个数     */
    unsigned char        nLen;          /* 长度       */
    unsigned char        cOpt;          /* 0--正常  1--不      */
} ncUtlStrStr_h;

typedef struct ncUtlStrStr_cs {
    unsigned char        cOpt;          /*  0--正常  1--不  */
    unsigned char        cFlags;        /*  操作符  =#$^    */
    unsigned short       nLen;          /* 长度             */
    char     caWord[256];                /* 字符串内容       */
} ncUtlStrStr_c;


typedef struct ncThreadInfo_s {
    int      iSumThread;                 /* 线程总数  */
    unsigned long lStartTime;            /* 进程起始时间  */
    unsigned long lLastTime;             /* 最后操作时间  */
    int      iPid;                       /* 进程ID        */
} ncThreadInfo;

typedef struct ncProxyProcessInfo_s {
    int iMaxThread;            /* 每个进程最大线程数  */
    int iMaxProcess;           /* 最大进程数          */
    int iMinProcess;           /* 最小进程数          */
    int iSumProcess;           /* 总计进程数          */
    ncThreadInfo *psThread;
} ncProxyProcessInfo;


typedef struct ncIcUserinfo_s {
    unsigned long  lSip;      /* 源IP地址   */
    unsigned long  lUserid;   /* 用户ID     */
    unsigned long  lStartTime;
    unsigned long  lLastTime;
} ncIcUser;


typedef struct ncMailIpList_s {
    unsigned long  lSip;      /* 源IP地址   */
    unsigned short nSport;    /* 源端口     */
    unsigned short nDport;    /* 目标端口   */
    unsigned long  lDip;      /* 目标IP地址 */
    unsigned long  lLastTime;
    int            iFlags;    /* 0--发送  1--接收  */
} ncMailIpList;


/* Proxy的Ip地址和端口    */
typedef struct ncProxyIpList_s {
    unsigned long  lIp;       /* IP地址      */
    unsigned short nPort;     /* 端口        */
    unsigned short nRecv;     /* 保留        */
} ncProxyIpList;


typedef struct pasSearchHead_s {
    char caFname[32];        /* 字段名               */
    char caDispname[32];     /* 显示名               */
    int  iSize;
    int  iMaxLen;
    int  iType;              /* 输入类型  1-文本  2-选择框  */
    char *pValue;
} pasSearchHead;

/* 内存中的用户信息  */
typedef struct ncUserBuf_s {       /* 用户信息                */
    long     lUserid;              /* 用户Id  0--没用         */
    char     caUsername[16];       /* 姓名                    */
    char     caDispname[16];       /* 显示名                  */
    long     lGroupid;             /* 部门ID                  */
    char     caGroupname[16];      /* 部门名称                */
} ncUserBuf;

/* 用户信息的缓存
         本结构主要用于用户信息的Case, 在系统中存放在一个Hash表中
 */
typedef struct ncUserBufIp_s {
	unsigned long lSip;           /* IP地址  */
	int      iFlags;              /* 状态 0--未用       1--已在线  2--用户超时  3--用户不存在
	                                      4--MAC不一致  7--禁用    9--免监控
	                               */
	unsigned long lLastTime;      /* 最后访问时间  */
} ncUserBufIp;


/* 第三方认证的用户信息  */
typedef struct ncThirdAuth_s {
    char          caUsername[16];    /* 用户名                */
    int           iIndex;            /* 在用户信息中的索引号  */
    unsigned long lIp;               /* IP地址 0 --表示没有内容 */
    unsigned long lLastTime;         /* 最后更新时间          */
} ncThirdAuth;

typedef struct ncNtAuth_s {
    unsigned long lIp;               /* IP地址             */
    char          caName[16];
    unsigned long lLastTime;         /* 最后操作时间       */
    char          caMd5[32];         /* 向客户端发放的Key,16位  */
} ncNtAuth;

typedef struct ncIcAuth_s {
    unsigned long lIp;               /* IP地址             */
    char          caName[16];
    unsigned long lLastTime;         /* 最后操作时间       */
} ncIcAuth;



/* 用户同步信息   */
typedef struct ncNtUser_s {        /* NT用户信息     */
    char          caName[32];      /* 姓名           */
    unsigned long lIp;             /* IP地址         */
    int           iStatus;        /* 1--登陆  0--退出  */
} ncNtUser;


/* 表的结构信息  */
typedef struct ncDbField_s {
    int  iFlags;
    char caField[32];
    char caType[32];
    char caNull[32];
  	char caKey[32];
  	char caDefault[64];
  	char caExtra[32];
} ncDbField;

typedef struct ncDbIndex_s {
    int  iFlags;
    char caIndex[32];
    char caUnique[4];    /* 0-- Unique  1-- No */
    char caSeq[4];       /* 序号               */
    char caField[32];
} ncDbIndex;



#define NC_NETWORK_NOMAL          0    /* 正常访问    */
#define NC_NETWORK_NAMEBASE       1    /* 基于名字    */
#define NC_NETWORK_INTERNET       99   /* Internet网  */
#define NC_NETWORK_IPBASE         2    /* 基于IP地址  */
#define NC_NETWORK_MACBASE        3    /* 基于MAC地址 */
#define NC_NETWORK_AUTH           4    /* 本机验证    */
#define NC_NETWORK_NTAUTH         5    /* 第三方验证  */
#define NC_NETWORK_NOMON          6    /* 免监控      */
#define NC_NETWORK_FORBID         7    /* 禁用        */
#define NC_NETWORK_CLIENT         8    /* 基于客户端ID*/
#define NC_NETWORK_OUTSIDE        9    /* 外部        */

typedef struct ncIpNetwork_s {      /* 内部IP地址    */
    unsigned long   lIpLower;       /* 起始IP地址    */
    unsigned long   lIpUpper;       /* 终止IP地址    */
    unsigned long   lAuthIp;        /* 验证服务器IP地址  */
    unsigned short  nAuthPort;      /* 验证服务器端口号  */
    unsigned short  iFlags;         /* 见宏定义      */
} ncIpNetwork;

/* Ip的头部信息  */
typedef struct ncIpHead_s {
	unsigned long  lSip;       /* 源IP地址     */
	unsigned long  lDip;       /* 目标IP地址   */
	unsigned short nDport;     /* 目标端口     */
	unsigned short nSport;     /* 源端口       */
	char     caSmac[6];        /* 源MAC地址    */
	char     caDmac[6];        /* 目标MAC地址  */
	short    cTran;            /* 传送方向 0--正常 1--反向    */
	short    cProt;            /* 协议  6 UDP      17 TCP     */
	unsigned long lSeq;
} ncIpHead;


typedef struct ncMsnPkg_s {
	unsigned long  lSip;       /* 源IP地址     */
	unsigned long  lDip;       /* 目标IP地址   */
	unsigned short nDport;     /* 目标端口     */
	unsigned short nSport;     /* 源端口       */
	unsigned long  lLen;       /* 数据包长度   */
} ncMsnPkg;



/* 限制访问  */
typedef struct ncIpLimited_s {       /* 限制访问的Ip地址  */
    unsigned long   lUid;              /* 用户Id  0 表示全部  */
    unsigned long   lIpLower;          /* 下限  */
    unsigned long   lIpUpper;          /* 上限  */
    unsigned char   cTranType;         /* 传送类型  0--全部   1--UDP  2--TCP          */
    unsigned char   cTimeType;         /* 时间类型                                    */
    unsigned short  nLowerport;        /* 端口号下限                                  */
    unsigned short  nUpperport;        /* 端口号上限                                  */
    unsigned char   cService;          /* 服务类型                                    */
    unsigned char   cFlags;            /* 限制方式 1--外部网  2--服务  3--IP地址和端口 */
    unsigned long   lStart;            /* 起始时间 HHMMSS                             */
    unsigned long   lEnd;              /* 终止时间 HHMMSS                             */
    unsigned char   cVister;           /* 0 禁止   1 允许                             */
    unsigned char   cReserve[3];       /* 保留  */
} ncIpLimited;


typedef struct ncWebLimited_s {        /* 限制访问的网站      */
    unsigned long   lUid;              /* 用户Id  0 表示全部  */
    char            caUrl[64];         /* 网站地址            */
    unsigned long   lStart;            /* 起始时间 HHMMSS     */
    unsigned long   lEnd;              /* 终止时间 HHMMSS     */
    unsigned char   cTimeType;         /* 时间类型            */
    unsigned char   cVister;           /* 0  禁止  1 允许     */
    unsigned char   cReserve[2];       /* 保留                */
} ncWebLimited;

/* The Value of cType */
#define NC_PROT_TCP      6
#define NC_PROT_UDP     17
/* 此处的源地址为客户端地址  */
typedef struct ncIp_s {
    unsigned long  lSip;             /* 内部IP地址                   */
    unsigned long  lDip;             /* 外部IP地址                   */
    unsigned short nDport;           /* 目标端口号                   */
    unsigned short nSport;           /* 源端口, 缺省情况下为 0, 只有当代理的时候才为非 0  */
    unsigned char  cType;            /* 协议    6--TCP 17 UDP        */
    unsigned char  cMark;            /* 标识,在不同的地方有不同的用处 */    
    unsigned char  cRev[2];          /* 保留  */
    unsigned long  lStartTime;       /* 起始时间                     */
    unsigned long  lLastTime;        /* 最后修改时间                 */
    unsigned long  lStop;            /* 访问控制检查时间 hhmmss， 若该值为0表示使用超时时间  */
    long8    lBytes[2];              /* 0-上行字节数  1-下行字节数   */
    unsigned char  cStatus;          /* 0--新   1--正常  2--禁用     */
    unsigned char  cUseFlags;        /* 使用标识 0--没内容 1--有内容 2--已处理 3--已保存   */
    unsigned char  caMac[6];         /* 源Mac地址                    */
    ncUserInfo     *psUser;          /* 用户信息，若为空表示用户信息不定    */
    unsigned long  lService;         /* 服务                                */
    unsigned long  lUrl;             /* 访问http                            */
} ncIp;



/* TCP连线信息  
      此处的源地址为内网地址
*/
/* Value of cConnect   */
#define NC_TCPSTAT_SYN     			1
#define NC_TCPSTAT_ESTABLISHED    	2
#define NC_TCPSTAT_CLOSED             9
/* Value of cDict      */
#define NC_DICT_SEND           0
#define NC_DICT_RECEIVE        1
typedef struct ncTcp_s {
    unsigned long  lSip;              /* 内部IP地址                   */
    unsigned long  lDip;              /* 外部IP地址                   */
    unsigned short nDport;            /* 目标端口号                   */
    unsigned short nSport;            /* 源端口号                     */
    unsigned long  lStartTime;        /* 起始时间                     */
    unsigned long  lLastTime;         /* 最后修改时间                 */
    unsigned char  cConnect;          /* 连接状态  1 Syn  2 建立  9 Close   */
    unsigned char  cDiction;          /* 方向 0 正向   1 反向               */
    unsigned char  cStatus;           /*      0 未知   1 正常      2 禁用   */
    unsigned char  cProt;             /* 协议   6-TCP  17 UDP               */
    unsigned char  cFlags;            /* 用于具体协议中的标识               */
    unsigned char  cProxy;            /* 0 -- 正常   1 -- 基于Proxy的协议   */
    unsigned char  cRev[2];
    ncIp           *psIp;             /* 对应的IP信息,NULL 表示该连接不可用 */
    unsigned long  lStop;             /* 规则作用的终止时间                 */
    unsigned long  lSeq;              /* 上行序列号                          */
    unsigned long  lAck_seq;          /* 下行序列号                          */
    unsigned long  lSid;              /* Session ID                          */
    unsigned long  lUrl;              /* Url序号                             */
} ncTcp;



/* 例外的IP连接, Key=12    
         cStatus --- 禁用原因
         ID      --- 规则ID
 */
typedef struct ncIpExcept_s {
    unsigned long  lSip;             /* 内部IP地址                         */
    unsigned long  lDip;             /* 外部IP地址                         */
    unsigned short nDport;           /* 目标端口号                         */
    unsigned char  cType;            /* 协议    6--TCP 17 UDP              */
    unsigned char  cStatus;          /* 类型                               */
    unsigned long  lUserid;          /* 用户ID, Userid为0表示非受控数据包  */
    unsigned long  lRid;             /* 规则ID                             */
    unsigned long  lStartTime;       /* 最后修改时间                       */
    unsigned long  lLastTime;        /* 最后修改时间                       */
    unsigned long  lCount;           /* 数据包的数量                       */
    char           caMsg[96];        /* 补充信息                           */
} ncIpExcept;


typedef struct ncHttpBuffer_s {
    unsigned long  lSip;             /* 源IP地址     */
    unsigned long  lStartTime;       /* 起始时间     */
    unsigned long  lLastTime;        /* 终止时间     */
    unsigned long  lBytes;           /* 流量         */
    unsigned long  lDip;             /* 目标地址     */
    unsigned short nDport;           /* 目标端口     */
    unsigned char  caMac[6];         /* 源Mac地址    */
    unsigned char  cUseFlags;        /* 标识，0--无内容 1--有内容 2--可以处理 3--已保存  */
    unsigned char  cReserve[3];      /* 保留  */
    unsigned long  userid;
    char     caHost[64];
    char     caUrl[128];
    char     caTitle[128];             /* 标题 2005/01/05 加入      */
} ncHttpBuffer;

typedef struct ncService_s {
    char            caName[32];      /* 服务名称  */
    unsigned long   lServices;       /* 服务类型  */
} ncService;



/* 邮件监控过滤规则  */
typedef struct ncMailFilter_s {
    int    tid;                  /* 类别ID */
    char   uname[128];
    char   caFrom[128];
    char   caTo[128];
    char   caSub[128];
    char   caCont[128];
    char   caAtt[128];
    int    lSize;
    int    lAsize;
    int    lTonum;
    char   cStype;            /* 0--全部  1--发送  2--接收  */
    char   cUflags;           /* 1--组    2--用户           */
    char   cTzflags;          /* 0--不通知  1--通知         */
    char   cSaveflags;        /* 0--不保存  1--保存         */
    char   caTzmail[64];
} ncMailFilter;


/* 拦截邮件分类规则 */
typedef struct ncMailCtlType_s {
    int    tid;                  /* 类别ID */
    char   uname[128];
    char   caFrom[128];
    char   caTo[128];
    char   caSub[128];
    char   caCont[128];
    char   caAtt[128];
    int    lSize;
    int    lAsize;
    int    lTonum;
    char   cStype;            /* 0--全部  1--发送  2--接收  */
    char   cUflags;           /* 1--组    2--用户           */
} ncMailCtlType;

typedef struct ncUploadFilter_s {
    int    tid;                  /* 类别ID */
    char   uname[128];
    char   caHost[128];
    char   caUrl[128];
    char   caCont[128];
    char   caAtt[128];
    int    lSize;
    int    lAsize;
    char   cUflags;           /* 1--组    2--用户           */
    char   cTzflags;          /* 0--不通知  1--通知         */
    char   cSaveflags;        /* 0--不保存  1--保存         */
    char   cReserve;          /* 保留                       */
    char   caTzmail[64];
} ncUploadFilter;



typedef struct ncPostFileList_s {
    char     pOriginFile[128];   /* 源文件    */
    char     pType[12];          /* 文件类型  */
    char     pOutFile[64];
    unsigned long lSize;
    struct ncPostFileList_s *psNext;
} ncPosFileList;



typedef struct ncPostDataContent_s {
    unsigned long  lBytes;         /* 总字节数  */
    unsigned long  lTime;          /* 时间      */
    char           caName[32];     /* 用户名    */
    char           caHost[128];    /* 主机名    */
    char           caUrl[128];     /* 访问URL   */
    char           *pValue;        /* 数据内容  */
    int            iSum;           /* 附件总数  */
    ncPosFileList  *psFileList;    /* 文件列表  */
} ncPostDataContent;


typedef struct ncPostFList_s {
    char     *pOriginFile;       /* 源文件     */
    char     *pType;             /* 文件类型   */
    char     *pOutFile;          /* 输出文件   */
    unsigned long lSize;
    struct ncPostFileList_s *psNext;
} ncPostFList;

typedef struct ncPostVarList_s {
    char          *pVar;              /* 变量名    */
    char          *pValue;            /* 内容      */
    struct ncPostVarList_s *psNext;
} ncPosVarList;

/* 表单内容(New)  */
typedef struct ncPostDataList_s {
    unsigned long  lBytes;         /* 总字节数  */
    unsigned long  lTime;          /* 时间      */
    unsigned long  lUserid;        /* 用户ID    */
    char           caHost[128];    /* 主机名    */
    char           caUrl[128];     /* 访问URL   */
    int            iSumVar;        /* 变量总数  */
    ncPosVarList   *psVarList;     /* 变量列表  */
    int            iSumFile;       /* 附件总数  */
    ncPosFileList  *psFileList;    /* 文件列表  */
} ncPostDataList;



#define NC_IPPKG_UPFLOW      0
#define NC_IPPKG_DOWNFLOW    1

typedef struct ncIpPkgInfo_s {
    long8    lIpFlow[2];           /*  0 Upload Bytea  1  DownBytes  */
    unsigned long lStartTime;      /*  起始时间                      */
    long8    lSumPkg;              /*  总数据包                      */
    unsigned long lpPkgPerSec;     /*  每秒数据包                    */
    unsigned long lStartCountTime; /*  记数的起始时间                */
    long8    lPkgCount;            /*  数据包记书数                  */
    unsigned long lCount;          /*  访问次数                      */
    unsigned long lStartControl;   /*  时间控制的起始时间            */
    char          caFilename[128];
} ncIpPkgInfo;

typedef struct ncSumControl_s {
    unsigned long  lStartTime;       /* 起动时间                     */
    unsigned long  lLastTime;        /* 最后操作时间                 */
    unsigned long  lTimeOut;         /* 最长超时时间                 */
    unsigned short nLock;            /* 锁  0--正常   1--正在检查    */
    unsigned short nR1;              /*                               */
    unsigned long  lR1[2];           /* 保留                          */
} ncSumControl;

/* 控制信息 从1开始  */
#define NC_CONTROL_NOUSE           0
#define NC_CONTROL_DELETE          1
#define NC_CONTROL_TABDROP         5
#define NC_CONTROL_MODPORT         6
#define NC_CONTROL_DISPERR         7

/* Iptables 的控制台命令           */


struct ncTestRemote_s {     /* 远程文件测试  */
    char  caIp[16];
    char  caHost[64];
    char  caUser[32];
    char  caPass[32];
    char  caMount[64];
    char  caShare[32];
    char  caSdate[12];
    char  caEdate[12];
    char  caOpt[12];       /* 内容       */
};

/* 当为RemoteMount时 cPort的值为具体的命令  */
#define NC_REMOTE_MOUNT         1
#define NC_REMOTE_MOUNTTEST     2
#define NC_REMOTE_MAILBACK      3
#define NC_REMOTE_UNMOUNT       4
#define NC_REMOTE_MAILLOAD      5
#define NC_REMOTE_MAILEXPORT    6    /* 数据导出  */
typedef struct ncControl_s {
    unsigned long  lSip;             /* 起始IP    */
    unsigned long  lDip;
    unsigned short nDport;
    unsigned char  cProt;            /* 协议                         */
    unsigned char  cUse;             /* 见上面的宏                   */
    unsigned long  lLastTime;        /* 起始时间                     */
    unsigned long  lStop;            /* 终止时间                     */
    int            iIndex;
} ncControl;

typedef struct ncIpControlInfo_s {
    unsigned long  lSip;             /* 起始IP                      */
    unsigned long  lDip;
    unsigned short nDport;
    unsigned char  cProt;            /* 协议                        */
    unsigned char  cComm;            /* 命令 0--Add 1--Del 2--Flush */
    unsigned long  lStop;            /* 终止时间 hhmmss             */
    unsigned long  lId;              /* ControlId                   */
} ncIpControlInfo;

typedef struct ncDevList_s {
    char caDev[32];
    char caIp[32];
    char caNetmask[32];
    char caBoard[32];
    struct ncDevList_s *next;
} ncDevList;


#define  NC_PID_COLLECT        1     /* 数据采集     */
#define  NC_PID_SYSLOG         0     /* 系统日志     */
#define  NC_PID_TABLECTL       2     /* IpTable控制  */
#define  NC_PID_NTSYNC         3     /* 域同步       */
#define  NC_PID_ONTIME         4     /* 定时         */
#define  NC_PID_ICAUTH         5     /* 第三方验证   */
#define  NC_PID_CHECKSOFT      6     /* 检查Software */
#define  NC_PID_SMTPPROXY      7     /* Smtp Proxy   */
#define  NC_PID_LISTEN25       8     /* 侦听外网     */
#define  NC_PID_HTTPPROXY      9     /* Http Proxy   */
#define  NC_PID_POP3PROXY     10     /* Pop3 Proxy   */
#define  NC_PID_NTAUTH        11     /* NT域的验证   */
#define  NC_PID_GETPKG        12     /* 数据处理      */
#define  NC_PID_PKGDO         13     /* 数据采集      */
#define  NC_PID_ASYNC         14     /* 异步日志操作  */
#define  NC_PID_MAILMON       15     /* 邮件内容监控  */
#define  NC_PID_POSTMON       16     /* 表单内容监控  */
#define  NC_PID_CHECKMAC      17     /* Mac地址检查   */
#define  NC_PID_SYSMON        18     /* Mac地址检查   */
#define  NC_PID_DBSYNC        19     /* 数据同步进程  */
#define  NC_PID_FILESRV       20     /* 文件服务器   */
#define  NC_PID_DODBLOG       21     /* 处理数据库同步信息  */
#define  NC_PID_UPDATE        22     /* 自动在线升级       */
#define  NC_PID_MAILFILTER    23     /* 邮件监控       */
#define  NC_PID_AUTOBAK       24     /* 自动备份进程  */
#define  NC_PID_AUTOCLEAN     25     /* 自动整理进程  */
#define  NC_PID_VIRUSCHECK    26     /* 检查病毒扫描引擎  */
#define  NC_PID_NWCHECKCOMP5  27     /* 内网检查在线计算机机  */
#define  NC_PID_DBCHECK       28     /* 数据库检查和修复      */   
#define  NC_RELOAD_ALL            0  /* 全部        */
#define  NC_RELOAD_NETWORK        1  /* 网络设置    */
#define  NC_RELOAD_LIMIT          2  /* 访问限制    */
#define  NC_RELOAD_MAILMONFILTER  3  /* 邮件过滤规则 */
#define  NC_RELOAD_PQCONFIG       4  /* 网络配置参数 */
#define  NC_RELOAD_DEBUG          5  /* Debug参数    */
/* 进程信息  */
typedef struct ncProcessInfo_s {
    int           iPid;
    unsigned long lStartTime;       /* 启动时间  */
    unsigned long lTimeOut;         /* 超时时间  */
    unsigned long lOntime;          /* 在每天指定时间重新启动  */
    unsigned char caControl[32];    /* 控制信息  */
    char     caName[32];            /* 功能名称  */
    int   (*fFunName)(utShmHead *); /* 函数名   */
    unsigned long lLastTime;        /* 进程最后操作时间   */
    unsigned long lStepTime;        /* 最大间隔时间，通常超过该时间表明进程有问题 */
    char     caRev[32];             /* 保留，不同的进程作用不同                   */
    int      iFlags;                 /* 0--无  1--内部进程  2--外部  */
} ncProcessInfo;

/* 进程信息  */
#define NC_PROC_RESET       1     /* 进程重启  */
#define NC_PROC_STOP        9     /* 进程停止  */


typedef struct ncEimProcess_s {
    char     caName[16];           /* 别名                    */
    char     caPath[64];           /* 路径                    */
    char     caFun[32];            /* 功能名称                */
    int      iPrio;                /* 优先级                  */
    int      iPid;                 /* 0 表示没有启动          */
    unsigned long lStartTime;      /* 启动时间                */
    unsigned long lTimeOut;        /* 超时时间                */
    unsigned long lOnTime;         /* 在每天指定时间重新启动  */
    unsigned char caControl[32];   /* 控制信息                */
    int      iStatus;              /* 0--不启动   1--启动   -1 出错*/
    int      iFlags;               /* 1--单进程  2--多进程    */
} ncEimProcess;


#define  NC_ONTIME_IPCHECK        0   /* 定时检查IP缓存  */
#define  NC_ONTIME_PROCESSCHECK   1   /* 定时检查进程    */
#define  NC_ONTIME_COUNT          2   /* 数据统计        */
#define  NC_ONTIME_DELDETAILLOG   3   /* 清除详细信息    */
#define  NC_ONTIME_DELTEMPFILE    4   /* 删除临时文件    */
#define  NC_ONTIME_DATABACK       5   /* 数据备份        */
#define  NC_ONTIME_CHECKIPTAB     6   /* 检查IPtables    */
#define  NC_ONTIME_RELOADCNF      7   /* 检查配置重栽    */
#define  NC_ONTIME_TIMECONTROL    8   /* 检查时间控制    */
#define  NC_ONLINE_RESTARTNC      9   /* 重新启动NC      */


typedef struct ncOnTimeFun_s {
    unsigned long lLastTime;            /* 最后一次执行时间  */
    unsigned long lUsedTime;            /* 最后一次执行使用时间  */
    unsigned long lCount;               /* 执行次数      */
    unsigned long lStepTime;            /* 间隔时间      */
    unsigned long lOnTime;              /* 指定时间 hhmmss       */
    int      iStatus;                   /* 0--空闲  1--等待 2--正在执行  */
    char     caName[32];                /* 功能名称      */
    int   (*fFunName)(utShmHead *);     /* 执行函数名称  */
} ncOnTimeFun;


/* 邮件过滤规则  */
typedef struct ncMailCtl_s {
    unsigned long lId;
    char caUname[128];
    char caFrom[128];
    char caTo[128];
    char caSub[128];
    char caCont[128];
    char caAttname[128];
    unsigned long lMsize;
    unsigned long lAsize;
    unsigned long lTonum;
    unsigned char cFlags;     /* 限制标识  2--个人 1--组  0-- 全部 */
    unsigned char cActflags;  /* 处理方式 0--备份 1--不备份  */
    unsigned char control;    /* 邮件控制 1--正常 2--拒绝  3--发到指定地址  4--等待确认 */
    unsigned char cTzflags;   /* 通知方式 0--不通知  1--通知发件人  2--收件人 3--指定邮箱  */
    unsigned char cStype;     /* 发送类别 0--全部                                          */
    unsigned char cReserv[3];
    char caSmail[64];         /* 转发邮箱        */
    char caTzmail[64];        /* 通知帐号        */
    char caTzcont[128];       /* 通知内容        */
} ncMailCtl;

/* Add 2002/11/30 */
typedef struct ncFieldList_s {
    char   caFname[32];
    char   cType;          /* 类型 n-数字 s-字符 */
    char   cFlags;
    short  nLen;
} ncFieldList;

/* 调试信息  */
extern int iDebugLevel;
#define NC_DEBUG_ERROR    1            /* 错误信息  */
#define NC_DEBUG_WARN     2
#define NC_DEBUG_LOG      4
#define NC_DEBUG_MSG      8
#define NC_DEBUG_SHMLOAD 16

#define NCS_SYNC_CONTFILTER   1
typedef struct ncNcsrvinfo_s {
    unsigned long  lId;
    unsigned long  lSip;
    unsigned short nPort;
    unsigned short nStatus;
    unsigned long  lLasttime;
    long           nTcpPort;
    char           caName[16];
    char           caSid[16];
} ncNcsrvinfo;



/* gtype    组类型    */
#define NC_GTYPE_LOCAL      0           /* 本地       */
#define NC_GTYPE_USER       1           /* 单个用户   */
#define NC_GTYPE_GROUP      2           /* 组         */
#define NC_GTYPE_COMPANY    3           /* 公司       */
#define NC_GTYPE_POLICY     4           /* 策略       */
#define NC_GTYPE_CENTER     8           /* 中心       */
#define NC_GTYPE_SYSTEM     9           /* 系统       */



/* DHCP数据结构  
   0                   1                   2                   3
   0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   |     op (1)    |   htype (1)   |   hlen (1)    |   hops (1)    |
   +---------------+---------------+---------------+---------------+
   |                            xid (4)                            |
   +-------------------------------+-------------------------------+
   |           secs (2)            |           flags (2)           |
   +-------------------------------+-------------------------------+
   |                          ciaddr  (4)                          |
   +---------------------------------------------------------------+
   |                          yiaddr  (4)                          |
   +---------------------------------------------------------------+
   |                          siaddr  (4)                          |
   +---------------------------------------------------------------+
   |                          giaddr  (4)                          |
   +---------------------------------------------------------------+
   |                                                               |
   |                          chaddr  (16)                         |
   |                                                               |
   |                                                               |
   +---------------------------------------------------------------+
   |                                                               |
   |                          sname   (64)                         |
   +---------------------------------------------------------------+
   |                                                               |
   |                          file    (128)                        |
   +---------------------------------------------------------------+
   |                                                               |
   |                          options (variable)                   |
   +---------------------------------------------------------------+
   */
#define MACIP_FROM_DHCP          1
#define MACIP_FROM_WINS          2
#define MACIP_FROM_FIXED         9

#define DHCP_MSG_DHCPDISCOVER    1
#define DHCP_MSG_DHCPOFFER       2
#define DHCP_MSG_DHCPREQUEST     3
#define DHCP_MSG_DHCPDECLINE     4
#define DHCP_MSG_DHCPACK         5
#define DHCP_MSG_DHCPNAK         6
#define DHCP_MSG_DHCPRELEASE     7
#define NC_MAIPFILE_ID           19650429L
typedef struct ncDhcp_s {
    unsigned char  op;       /* 1:from Client  2:from Server */
    unsigned char  htype;    /* 1:Ethernet, 7:Arcnet         */
    unsigned char  hlen;     /* MAC地址长度 6                */
    unsigned char  hops;     /* 转发次数                     */
    unsigned long  xid;      /* Transaction ID               */
    unsigned short secs;
    unsigned short flags;
    unsigned long  ciaddr;   /* 目前Client所使用的IP地址     */
    unsigned long  yiaddr;   /* 要分配的IP地址               */
    unsigned long  siaddr;
    unsigned long  giaddr;
    unsigned char  chaddr[16];
    unsigned char  sname[64];
    unsigned char  file[128];
} ncDhcp;



typedef struct ncComputeMacIp_s {
    unsigned char  mac[6];      /* Mac地址                      */
    char           flags;       /* 记录来源 1--固定不变 8--Wins 9--DHCP */
    char           status;
    unsigned long  ip;          /* Ip地址                       */
    unsigned char  cname[16];   /* 主机名,超过15位自动截掉      */
} ncComputeMacIp;


/* 该数据存放在内存中，用于查找相应机器的相关信息    */
typedef struct ncComputeIpMac_s {
    unsigned long  ip;          /* Ip地址                       */
    unsigned char  mac[6];
    char           flags;       /* 记录来源 1--固定不变 8--Wins 9--DHCP */
    char           status;
} ncComputeIpMac;


typedef struct ncUtlFileList_s {
    char name[64];                  /* 文件名     */
    unsigned long lSize;            /* 文件长度   */
} ncUtlFileList;


typedef struct ncUtlDBSynInfo_s {
    int           iFileServerId;    /* 文件服务器的进程号 */
    int           iProcessId;       /* 数据处理进程ID     */
    unsigned long lStartTime;       /* 起动时间           */
    unsigned long lMailMon;         /* 监控邮件的数量     */
    unsigned long lFormMon;
    unsigned long lMailCtl;
    unsigned long lDbLog;
    unsigned long long lBytes;      /* 同步的流量       */
    unsigned long lLastTime;        /* 最后一次同步时间 */
    unsigned long lLastDoTime;      /* 最后一次处理时间  */
    unsigned long lUndoFile;        /* 未处理的文件数量 */
    char     caDoFile[128];         /* 当前正在传送的文件  */
    char     caCurFile[128];        /* 正在处理            */
} ncUtlDBSynInfo;

typedef struct ncMailList_s {
    char mail[60];                  /* 邮件地址 */
    char cRev[2];
    char cdo;                       /* 处理方式 */
    char cflags;
} ncMailList;

typedef struct ncGroupIpList_s {
    unsigned long lSip;
    unsigned long lEip;
    unsigned long lGroupid;
} ncGroupIpList;




#ifdef MTEST_1
#include "ncalloc.h"
#endif
#endif
