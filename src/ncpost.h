/* 2009/02/20 增加 账号、密码采集   */
/* 表单分析                         */
#ifndef __NCPOSTDEF___
#define __NCPOSTDEF___
#define NC_WBM_ID        651129L
#define NC_FILE_BUF       8192
#define NC_MAX_POSTBUF    64000L
#define NC_WBM_From      0
#define NC_WBM_To        1
#define NC_WBM_Cc        2
#define NC_WBM_Bcc       3
#define NC_WBM_Sub       4
#define NC_WBM_Conetnt   5
#define NC_WBM_Attach    6
#define NC_WBM_TITLE     7
#define NC_WBM_CHARSET   8    // 字符集
#define NC_WBM_UID
#define NC_WBM_MAXITEM   16
#define NC_MAX_FILEREAD  2048    // 每次读文件的最大数量  

/* 表单变量名    */
typedef struct ncPostCont_s {  /* 表单内容    */
    char            *pVar;              /* 变量名        */
    char            *pValue;            /* 变量值        */
    char            *pContentType;      /* 文件类型      */
    unsigned long   lFileId;            /* 文件序号,以 proeim#保存  0--表示是一般字段  */
    struct   ncPostCont_s *next;
} ncPostCont;

#define NC_POST_FORM     1
#define NC_POST_UPLOAD   2
#define NC_POST_XML      3
#define NC_POST_EXCHANGE 4
typedef struct ncPostHead_s {   /* Http头信息(POST请求)   */
    char    *pHost;     /* 主机名       */
    char    *pUrl;      /* Url名称      */
    char    *pBound;    /* 分界符       */
    int     iLen;       /* 内容的长度   */
    int     iType;      /* 类型   1--FORM表单    2--Upload表单    3--XML表单*/
    unsigned long lAsize;  /* 附件大小     */
    unsigned long lTime;
    ncPostCont *psCont;
} ncPostHead;

/* WebMail的邮件头                    */
typedef struct ncWbmMailHead_s {
    unsigned long lUserid;                    /* 用户ID   */
    char         *pHost;                      /* 主机名   */
    ncPostCont   *psCont[NC_WBM_MAXITEM];
    struct ncWbmMailHead_s *next;
} ncWbmMailHead;


/* 这是Hash表保存的内容
       1--主机的Webmail格式模板
             name    为 0
             iType --- 0 附件和内容连在一起   1 附件和内容分开
 */
 
typedef struct ncWebMailPlate_s {
    char  caHost[16];     /* 主机名，若为0,表示缺省  */
    char  caName[16];     /* 名字                    */
    unsigned short nLen;  /* 长度                    */
    unsigned short nType; /* 类型  1-From 2-To  3-Cc  4-Bcc 5-Subject 6--Text 7--Attach  9--CharSet*/
} ncWebMailPlate;

typedef struct ncWMPList_s {
    char     caName[16];     /* 名字                    */
    unsigned short nLen;     /* 变量长度                */
    unsigned short nType;    /* 类型                    */
} ncWMPList;


typedef struct ncWMPIndex_s {
    char            caHost[16];   /* 主机名  */
    int             iStart;
    unsigned short  nLen;         /* 主机长度 */
    unsigned short  nSum;
} ncWMPindex;

typedef struct ncWMP_s {
    int             iId;
    int             iSumIndex;
    int             iSumPlate;
    ncWMPindex      *psIndex;
    ncWMPList       *psList;
} ncWMP;


/* 这是 Post表单账号获取规则
   表单以Hash表的方式存放
       根据URL查找 相关记录,进行判断
       支持域名模糊 如: sina.com = *.sina.com
*/
#define NC_POST_NAMEPASS  1
#define NC_POST_NAMEONLY  2

typedef struct ncPostAccount_s {
    char url[64];  // 网址
    int  sid;      // 序列号
    int  from;     // 从何处来
    char account[32];   // 账号对应的变量 
    char passwd[32];    // 密码对应的变量
    int  code;          // 编码
} ncPostAccount;




typedef struct ncPostFVarList_s {
    char     var[20];     // 变量名
    short    service;     // 服务代码---0不保存
    short    vartype;     // 变量类别, 每种服务类别代码不一样
    struct ncPostFVarList *next;
} ncPostFVarList;


/* 表单整理


*/

typedef struct ncPostFilter_s {
    char url[32];                    // 网址
    ncPostFVarList  *psFlist;        // 表单列表
} ncPostFilter;


typedef struct ncPostVarList_s1 {
    char   var[20];                   // 变量名
    char   disp[16];                  // 显示名
    short  vartype;
    short  cFlags;                    // 1--必须    0--可有可无
} ncPostVarList1;
    
/* 表单描述     */
typedef struct ncPostDesc_s {
    char  name[16];                 // 服务名称
    ncPostVarList1  psList;          // 包含变量列表
    int   iMaxVar;                  // 变量总数
    int   (*fFunName)(utShmHead *);     /* 执行函数名称  */
} ncPostDesc;    


typedef struct ncPostDataList_s1 {
    short service;
    short vartype;
    char  var[20];
    char  *pValue;
} ncPortDataList1;


/* 文件清单   */
typedef struct ncPostDirList_s {
    unsigned long lTime;        // 最后操作时间
    char     caFile[40];        // 文件名
} ncPostDirList;

#endif
