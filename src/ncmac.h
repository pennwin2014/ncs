#ifndef __NCMACDEF__
#define __NCMACDEF__
#define NCMAC_VERSION         "V1.60.001"
#define NCMAC_RELEASE         "2015/06/03"
#include "ncapi.h"

// 输出内容
#define MACLOG_OUTFLAG_ALL          7
#define MACLOG_OUTFLAG_STARTSTOP    3
#define MACLOG_OUTFLAG_STOP         2
#define MACLOG_OUTFLAG_START        1
#define MACLOG_OUTFLAG_NONE         0


//     人流统计
//     当markid变化时，写日志   268
//     mac,starttime,endtime,mark
typedef struct ncLbsMacLocation_s {
    uchar   mac[6];                                   // Mac地址，为索引   
    uchar   apmac[6];                                 // AP Mac地址
    char    lrssi;                                    // 最早
    char    urssi;                                    // 最近
    char    noise;
    uchar   channel;
    uint4   starttime;                                // 起始时间
    uint4   gpstime;
    uint4   lasttime;                                 // 最后一次访问时间
    uchar   cFlags;                                   // 1--离线
    uchar   cDb;                                      // 0--未保存到日志  1--已保存到日志
    uchar   caBssid[6];                               // 关联的Bssid
    uchar   cAssocicated;                             // 关联  1--Yes  2--No
    uchar   encrypt;
    uchar   distance;
    uchar   mutype;                                   // 1-终端  2--热点
    uchar   vtype;                                    // 身份类别 0--真实身份  1--虚拟身份
    uchar   cMacDb;                                   // 是否同步过Mac库 0-无 1-是成功  2-正在同步 9-无手机号
    uchar   caRev[2];
    uchar   longitude[12];
    uchar   latitude[12];
    uint4   reporttime;                               // 上报数据时间
    uchar   ssid[32];                                 // 
    uchar   termtype[32];                             // 终端品牌
    uchar   name[60];                                 // 身份内容
    uint4   lSynMacDbTime;                            // 同步时间
    uchar   *pAp;                                     // 存放AP信息的结构
} ncLbsMacLocation;



typedef struct ncLbsMacRecord_s {
    ncLbsMacLocation *psMacLoc;
} ncLbsMacRecord;




//     虚拟身份
typedef struct ncMacLogVname_s {
    uchar   mac[6];                                   // Mac地址，为索引   
    uint2   num;                                      // 记录序号，自0开始
    uint2   sum;                                      // 记录总数
    uint2   vtype;                                    // 虚拟身份类别
    uchar   apmac[6];                                 // AP Mac地址
    uchar   vname[32];                                // 虚拟身份内容
    uchar   vdisp[32];                                // 显示名
    uint4   lSip;
    uint4   lDip;
    uint4   starttime;                                // 抓取时间
    uint4   lasttime;                                 // 最后时间
    uint4   reporttime;                               // 上报时间
    uchar   cDb;                                      // 是否保存在数据库中
    uchar   cRev;
    uint2   nDport;
} ncMacLogVname;



// 服务器状态
typedef struct ncPortalSrvStatus_s {
    uchar               caName[32];                // 名称
    unsigned long       lIp;                       // IP地址
    unsigned long       lLastTime;                 // 最后一次活动时间
    uint2               nPort;                     // 端口
    uint2               nStatus;                   // 状态
    uint2               nRev[2];
} ncPortalSrvStatus;


// 位置标签
typedef struct ncLbsLocApMark_s {
    char        caMark[32];
    uint4       lMacCount;        // Mac地址数量
} ncLbsLocMark;


#define NCSRV_LBS_HEAD         0x7C83
#define NCSRV_LBS_HEADXOR      0x7C85
#define NCSRV_LBS_H3C          0x0001
#define NCSRV_LBS_MUREPORT     0xD6
#define NCSRV_LBS_PDUSPEC      0xD8
#define NCSRV_LBS_MAXLEN       16000

typedef struct ncSrvLbsPdu_s {
    uint2   nHead;       // 0x7C83
    uint2   nReqId;      // Request ID
    uchar   cCode;
    uchar   cSubCode;
    uint2   nLen;
    uchar   data[4096];
} ncSrvLbsPdu;


    
// 48字节
typedef struct ncSrvMuReport_s {   // Code: 0xD6
    uchar   apmac[6];    // APMac
    uint2   vendorid;    // 厂商ID
    uint2   rev1;
    uchar   bssid[6];    // AP's SSID
    uchar   radtype;     
    uchar   channel;
    uchar   isassocicated;
    uchar   rev2;       // 0--扫描  1--离开  （寰创扩展)
    uint4   timestamp;
    uint2   rev3;
    uchar   mutype;      //  0x01--Unknow  0x02:Wifi Mobile Unit  0x03:Rogue Ap
    uchar   rev4;
    char    rssi;        // -128--127 dBm
    uchar   rev5;
    uchar   noisefloor;  // 
    uchar   rev6[3];
    uchar   datarate;
    uchar   mpduflags;
    uchar   mumac[6];
    uint2   fc;
    uint2   sc;
    uint2   rev7;
} ncSrvMuReport;


// 72字节
typedef struct ncSrvMuReportHC_s {   // Code: 0xD6  寰创新版
    uchar   apmac[6];    // APMac
    uint2   vendorid;    // 厂商ID
    uint2   rev1;
    char   longitude[10];   // 经度
    char   latitude[10];    // 纬度
    uchar   gpstime[4];      // GPS Time
    uchar   bssid[6];    // AP's SSID
    uchar   ssid[32];    // SSID
    uchar   radtype;     
    uchar   channel;
    uchar   isassocicated;  // 1--Yes  2--No
    uchar   encrypttype;       // 接入点加密类型 0--Open 1--WEP 2--WPA 3-WPA2
    uchar   distance;          // 被采集设备距采集设备的距离(米)
    uchar   mutype;      //  0x01--STA  2--AP  3 Adhox
    char    rssi;        // -128--127 dBm
    char   noisefloor;  // 
    uint4   timestamp;
    uchar   datarate;
    uchar   mpduflags;
    uchar   mumac[6];
    uint2   fc;
    uint2   sc;
} ncSrvMuReportHC;

typedef struct ncSrvPduMsg_s {
    uint2   nHead;       // 0x7C83
    uint2   nReqId;      // Request ID
    uchar   cCode;
    uchar   cSubCode;
    uint2   nLen;
    ncSrvMuReportHC sPayload;
} ncSrvPduMsg;


typedef struct ncSrvLbsH3C_s {
    uint2   nHead;       // Version
    uint2   nLen;
    uint4   nReqId;      // 
    uint2   nCode;
    uint2   nSubCode;
    uchar   data[1600];
} ncSrvLbsH3C;

typedef struct ncSrvMuReportH3C_s {   // Code: 0xD6
    uchar   mumac[6];
    uchar   apmac[6];    // APMac
    uchar   radid;     
    uchar   radtype;     
    uchar   bssid[6];    // AP's SSID
    uchar   txid;        // 天线ID
    uchar   cRev0;
    uchar   channel;
    uchar   isassocicated;
    uint4   timestamp;
    char    rssi;        // -128--127 dBm
    char    noisefloor;  // 
    uint2   fc;
    uint2   vendorid;    // 厂商ID
    uint2   rev1;
} ncSrvMuReportH3C;



#endif