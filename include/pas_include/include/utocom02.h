#ifndef __UTOCOMDEF2__
#define __UTOCOMDEF2__
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdarg.h>



/*************************************************************/

typedef struct {
    int      iSock;   /* Sock Id                                   */
    unsigned char    cKind;     /* 0--外部  1--内部                */
    unsigned char    cReserve;  /* 保留                            */
    unsigned char    cFlags;    /* 0--正常  1--正在等待应答        */
    unsigned char    cType;     /* 0 正常  1 大数据包  2 多数据包     */
    long     lTime;             /* 最后操作时间        */
    long     lClientIp;         /* Client IP地址       */
    int      iClientPort;       /* Client 端口号       */
} utTcpSockInfo;


typedef struct {
    int              iPort;        /* 端口号  */
    int              iSum;         /* 总数    */
    int              iThisPort;    /* Tcp内部端口号       */
    int              iThisIp;      /* 本机IP地址          */
    int              iPid;         /* 进程号              */
    long             lOffset;      /* 偏移量  */
} utTcpProSockTable;


/* 线程信息  */
typedef struct {
    int         iPid;       /* 进程标号  */
    int         iTaskId;    /* 任务标识  */
    int         iThreadId;  /* 线程标识  */
    int         iStartTime; /* 起始时间  */
} urTcpThreadInfo;


typedef struct {
    utTcpSockInfo     *psSockInfo;     /* Sock信息      */
    utTcpThreadInfo   *psThreadInfo;   /* 线程信息      */
    utShmHead         *psShmHead;      /* 共享内存句柄  */
} utTcpTransInfo;   

#endif

