/*
    通信消息
    
    创建 2011/05/01    By Liyunming
*/
#ifndef  __PASMESGDEF__
#define  __PASMESGDEF__
#include "utotype.h"

#define PAS_MSGHEAD_ID_H    0x34736150  // MsgID "Pas4" 主机字节序
#define PAS_MSGHEAD_ID_N    0x50617334  // MsgID "Pas4" 网络字节序

#define PAS_MSG_VERSION   0x00010000

#define PAS_MSGTYPE_PAS      0x01     // PAS专用格式
#define PAS_MSGTYPE_BINARY   0x02     // 二进制格式
#define PAS_MSGTYPE_TEXT     0x03     // 一般文本格式
#define PAS_MSGTYPE_POST     0x04     //  POST
#define PAS_MSGTYPE_XML      0x05     // XML
#define PAS_MSGTYPE_LIST     0x06     // LIST
#define PAS_MSGTYPE_FILE     0x07     // File

typedef struct pasMsgHead_s {
    uint4   lCode;       /*  消息标识,统一使用相当于"Pas4" 的值  */
    uint4   lVersion;    /*  版本号                              */
    uint4   lBytes;      /*  消息数据包的字节数,包含头           */
    uint4   lCheckSum;   /*  CRC32校验值(从第17个字节开始计算    */
    uint4   lSendId;     /*  发送端ID                            */
    uint4   lMsgId;      /*  lMsgId                              */
    uint4   lSendTime;   /*  发送时间戳                          */
    uint4   lFunCode;    /*  功能代码                            */
    uchar   caAuthCode[16];  /* 授权码                           */
    uchar   cCrypt;      /*  0-- No Crypt 1--DES  2--Tea         */
    uchar   cMsgType;    /* 消息体格式   0x01--专用格式  
                                        0x02--二进制  
                                        0x03--一般文本  
                                        0x04--POST 
                                        0x05--XML 
                                        0x06--数据列表 
                                        0x07--文件
                                                                 */
    uchar   cCharset;    /* 字符集编码                           */
    uchar   cStatus;     /* 状态                                 */
    uint4   lNum;        /* 服务器端在内存中的序号                */
    uint4   l1;
    uchar   cOrder;      /* 0--本地字节顺序   1--网络字节顺序     */
    uchar   c2;
    uchar   c3;
    uchar   c4;
} pasMsgHead;



typedef struct pasMsgFileSend_s {
    uint4   lStart;             /* 起始位置    */
    uint4   lBytes;             /* 字节数      */
    uint4   lSum;               /* 总共字节数  */
} pasMsgFileSend;


/* pasmsg01.c */
int pasMsgSetMyInfo(unsigned long lSendId,unsigned char *caAuth);
int pasMsgHeadInit(pasMsgHead *psMsgHead,unsigned long lFunCode,unsigned long lBytes,int iMsgType);
int pasMsgHead_Ntoh(pasMsgHead *psMsgHead);
int pasMsgHead_Hton(pasMsgHead *psMsgHead);
int pasMsgIsValidMsg(pasMsgHead *psMsgHead);
int pasMsgPostVar(char *pBuf,int iSum,...);


#endif
