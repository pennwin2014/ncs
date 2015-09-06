/* 最后修改时间 2008/04/21   */
#ifndef __NCSCASEDEF___
#define __NCSCASEDEF___
/* 布控信息, 存放在共享内存中   */
// 案件信息
#define  NCS_MAX_CASECONT            9   // 最大布控内容
#define  NCS_CASECONT_USER           0   // 用户特征
#define  NCS_CASECONT_NETID          1   // 虚拟身份
#define  NCS_CASECONT_IP             2   // IP地址
#define  NCS_CASECONT_WEB            3   // Web
#define  NCS_CASECONT_POST           4   // Post
#define  NCS_CASECONT_IM             5   // IM
#define  NCS_CASECONT_MAIL           6   // Mail
#define  NCS_CASECONT_WARN           7   // Warn
#define  NCS_CASECONT_SENS           8   // 敏感服务
#define  NCS_CASE_KEY              "SProNetWay!@#878"

typedef struct ncsCaseIndex_s {
    unsigned long   cfrom;               // 控制信息来源  1-管理中心 2-2级管理中心
    unsigned long   caseid;              // 案件Id
    unsigned long   lCrc32;              // 校验码, 同步完成后可根据Crc来检查同步信息是否正确
    unsigned long   lStart[NCS_MAX_CASECONT];     // 在内存中的起始地址
    unsigned long   lBytes;              // 总的字节数
    unsigned long   lSumRec;             // 记录总数
    unsigned long   lLastTime;           // 最后修改时间
    unsigned long   lExpire;             // 有效时间
    char            casecode[16];        // 案件编号
    char            casename[32];        // 案件名称
    unsigned char   cFlags;              // 状态 1-正常布控  8-删除  9-撤控
    unsigned char   cRev[3];
} ncsCaseIndex;




// 布控内容
typedef struct ncsCaseContHead_s {
    unsigned long   lMax;          // 最大长度
    unsigned long   lCur;          // 当前使用的最大长度
    char   *pBuf;         // 地址
} ncsCaseContHead;

// 布控范围, 内容按类别和ID的大小排列
typedef struct ncsCaseArea_s {
    unsigned long   caseid;           // 案件序列号
    unsigned char   cType;            // 类别 0-单位 1-单位类别  2-单位组
    unsigned char   cRev[3];
    unsigned long   did;              // ID
    ncsCaseIndex    *psCase;          // 对应案件的指针
} ncsCaseArea;



typedef struct ncsCaseContIndex_s {
        unsigned long   lNext;             // 下一条记录
        unsigned long   lLastTime;         // 最后修改时间

        unsigned short  nLen;              // 记录长度 (不包括头)
        unsigned char   cType;             // 类别
        unsigned char   cFlags;            // 1--正常  8--删除  9--撤控
        unsigned long   lSid;
} ncCaseContIndex;


typedef struct ncsCaseContH_s {
        unsigned short  nLen;              // 记录长度 (不包括头)
        unsigned char   cType;             // 类别
        unsigned char   cFlags;            // 1--正常  8--删除  9--撤控
        unsigned long   lSid;              // 在表中的序列号
} ncCaseContH;


// 使用者特征, 按修改时间从大到小排列
#define  NCS_CASEUSER_USERNAME     1
#define  NCS_CASEUSER_DISPNAME     2
#define  NCS_CASEUSER_COMPNAME     3
#define  NCS_CASEUSER_MACADDR      4
#define  NCS_CASEUSER_CARDID       5

// 基于用户特征
typedef struct ncsCaseUser_s {
    char      cType;          // 1--用户名  2-显示名  3-计算机名  4-MAC地址  5-证件
    char      cRev[3];
    char      caName[32];
} ncsCaseUser;

// 基于虚拟账号
typedef struct ncsCaseNetId_s {
    unsigned  long  lType;      // 服务类别
    unsigned  long  lUid;       // 对应的用户ID(用于关联)
    char      caName[64];       // 账号名称
} ncsCaseNetId;

// 基于IP地址
typedef struct ncsCaseIp_s {
    unsigned  long  lSlip;        // 源起始IP地址
    unsigned  long  lSuip;        // 源终止IP地址
    unsigned  long  lDlip;        // 目的起始IP地址
    unsigned  long  lDuip;        // 目的终止IP地址
    unsigned  short nDlport;      // 起始端口
    unsigned  short nDuport;      // 终止端口
    unsigned  long  lService;     // 服务
    unsigned  char  cProt;        // 协议
    unsigned  char  cRev[3];      // 保留
} ncsCaseIp;

// 基于访问网址
typedef struct ncsCaseWeb_s {
    unsigned  long  lUrlId;       // 网址库类别
    char  caUrl[128];             // 访问网址
} ncsCaseWeb;


// 基于访问表单
typedef struct ncsCasePost_s {
    unsigned  long  lUrlId;       // 网址类别
    unsigned  long  lContId;      // 敏感类别
    char  caCont[256];            // 表单内容,将网址和内容一起存放 012www.sina.com012aaaaaa
} ncsCasePost;


//  聊天内容
typedef struct ncsCaseIm_s {
    unsigned  long  lType;        // 服务类别
    unsigned  long  lContId;      // 敏感类别
    char  caCont[128];  // 表单内容
} ncsCaseIm;

// 邮件内容
typedef struct ncsCaseMail_s {
    unsigned  char  cType;        // 0-全部  1-发送  2-接收
    unsigned  char  cRev[3];
    unsigned  long  lContId;      // 敏感类别
    char      caCont[256];        // 内容 000From000To000Sub000Cont
} ncsCaseMail;

// 邮件内容
typedef struct ncsCaseMail1_s {
    unsigned  char  cType;        // 0-全部  1-发送  2-接收
    unsigned  char  cRev[3];
    unsigned  long  lContId;      // 敏感类别
    char      *pFrom;
    char      *pTo;
    char      *pSub;
    char      *pCont;
    char      caCont[256];        // 内容 000From000To000Sub000Cont
} ncsCaseMail1;


// 告警内容
typedef struct ncsCaseWarn_s {
    unsigned  long  lUid;         // 用户Id
    unsigned  char  cType;        // 告警方式 1-邮件  2-短信
    unsigned  char  cRev[3];
    char      caAddr[64];         // 地址
} ncsCaseWarn;

// 敏感服务
typedef struct ncsCaseSens_s {
    unsigned  long  lService;     // 服务类别
} ncsCaseSens;


#define NCS_CASEFILE_ID     19651129L
// 案件文件头部
typedef struct ncsCaseFile_s {
    unsigned long  lFid;         // 文件ID
    unsigned long  lSumCase;     // 案件总数
} ncsCaseFile;

#endif
