/*   pasmsg.h
     PAS消息格式
     2011/5/1
 */
#ifndef  __PASMSGDEF__
#define  __PASMSGDEF__

#include "utotype.h"


#define PAS_MSGHEAD_ID   "Pas4"
#define PAS_MSG_VERSION   0x00010000

#define PAS_MSGTYPE_PAS      0x01     // PAS专用格式
#define PAS_MSGTYPE_BINARY   0x02     // 二进制格式     
#define PAS_MSGTYPE_TEXT     0x03     // 一般文本格式
#define PAS_MSGTYPE_POST     0x04     //  POST
#define PAS_MSGTYPE_XML      0x05     // XML
#define PAS_MSGTYPE_LIST     0x06     // LIST
#define PAS_MSGTYPE_FILE     0x07     // File 


/* Message Head */
typedef struct {
    uchar   MsgCode[4];     /*  消息头，固定Pas4                    */
    uint4   Version;        /*  版本号                              */
    uint4   Length;         /*  数据包长度                          */
    uint4   CheckSum;       /*  Check Sum                           */
    uint4   SendId;         /*  Send Id                             */
    uint4   MsgId;          /*  Message Id                          */
    uint4   TimeStamp;      /*  时间戳                              */
    uint4   FunCode;        /* 功能代码                             */
    uchar   Secutity;       /* 加密标志 0x00--不加密  0x03--DES加密  */
    uchar   MsgType;        /* 消息体数据格式                        */
    uchar   CharSet;        /* 字符集编码   0x00-GBK  0x01--BIG5 0x02--UTF8
                                            0x04-UTF-16                 */
    uchar   Status;         /* 状态                                  */
    uchar   AuthCode[16];   /* 授权码                                */
    uchar   cCurSta;        /* 消息当前状态 0-源数据 1-已处理        */
    uchar   cRev[3];        /* 保留                                  */
    uchar   Data[8];        /* 保留                                  */
}  pasMsgHead;


#endif

