/* 最后更新时间
   2012/09/17
 */
#include "utotype.h"
#ifndef  ___PAS_UTL__DEF___
#define  ___PAS_UTL__DEF___   
#define PAS_LOG_ERROR           0      /* 严重错误         */
#define PAS_LOG_WARN            1      /* 警告信息         */
#define PAS_LOG_EVENT           2      /* 重要事件         */
#define PAS_LOG_EVENT1          3      /* 重要事件         */
#define PAS_LOG_EVENT2          4      /* 重要事件         */
#define PAS_LOG_EVENT3          5      /* 重要事件         */
#define PAS_LOG_DEBUG           6      /* 调试信息         */
#define PAS_LOG_DEBUG1          7      /* 调试信息         */
#define PAS_LOG_DEBUG2          8      /* 调试信息         */
#define PAS_LOG_DEBUG3          9      /* 调试信息         */


#define PAS_LOGS_WAITFORDEBUG   3888
#define PAS_BYTESORDER_NET      1      /* 网络字节顺序     */
#define PAS_BYTESORDER_HOST     0      /* 主机字节顺序     */

#define PAS_PROG_NC         1001       /* 网络督察          */
#define PAS_PROG_NCS        1002       /* 管理中心          */
#define PAS_PROG_NCSRV      1003       /* Portal            */
#define PAS_PROG_NCMAC      1004       /* Mac嗅探           */
#define PAS_PROG_NNMACSRV   1005       /* Mac嗅探服务器     */


/* 字符集  */
#define PAS_CCODE_GB       0   /* gbk bg2312 */
#define PAS_CCODE_BIG5     1   /* big5       */
#define PAS_CCODE_UTF8     2   /* utf-8      */
#define PAS_CCODE_JP       3   /* iso-2022-jp */
#define PAS_CCODE_UTF16    4   /* utf-16     */
#define PAS_CCODE_KR       5   /* EUC-KR 韩文  */
#define PAS_CCODE_UCODE    10  /* UCOde  */
#define PAS_CCODE_AUTO    0xff
typedef struct pasMemInfo_s {
    char *pBuf;
    int  iMaxSize;
    int  iLen;
    int  iStep;
} pasMemInfo;


typedef struct pasHtmlTdList_s {
    char  caCont[132];
} pasHtmlTdList;


typedef struct pasStrMem_s {
    unsigned long lMax;
    unsigned long lPos;
    unsigned long lSum;
    unsigned long lTime;
} pasStrMem;

typedef struct pasUtlIpDb_s {
    unsigned long lSip;
    unsigned long lEip;
    unsigned long lArea;
    unsigned long lName;
} pasUtlIpDb;

typedef  struct pasUtlIpDbHead_s {
        unsigned long lId;                   // File ID
        unsigned long lSumIp;                // Ip 总数
        unsigned long lTime;
        unsigned long lSumCz;
        unsigned long lSumPos;
        unsigned long lIpLen;
        unsigned long lStrLen;
        unsigned long lCrc;
        pasUtlIpDb    *psIp;
        pasStrMem     *psHead;
} pasUtlIpDbHead;

typedef  struct pasFileList_s {
    unsigned char caFile[128];           // 文件名
    unsigned long lSize;                 // 文件大小
    unsigned long lTime;              // 文件修改时间
    int           iMode;                 // 文件操作权限
} pasFileList;


typedef struct pasProcessList_s {
    uint4    iPid; 
    unsigned char    caName[64];
} pasProcessList;

#ifndef  pasmax
#define  pasmax(a,b) ((a) > (b) ? (a) : (b))
#define  pasmin(a,b) ((a) < (b) ? (a) : (b))
#endif    
#define pasLog1(iLevel,p)              pasDebugLogL(__LINE__,__FILE__,iLevel,p);
#define pasLog2(iLevel,p,p1)           pasDebugLogL(__LINE__,__FILE__,iLevel,p,p1);
#define pasLog3(iLevel,p,p1,p2)        pasDebugLogL(__LINE__,__FILE__,iLevel,p,p1,p2);
#define pasLog4(iLevel,p,p1,p2,p3)     pasDebugLogL(__LINE__,__FILE__,iLevel,p,p1,p2,p3);
#define pasLog5(iLevel,p,p1,p2,p3,p4)  pasDebugLogL(__LINE__,__FILE__,iLevel,p,p1,p2,p3,p4);


#endif
