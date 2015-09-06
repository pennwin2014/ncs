#ifndef __NCAPIDEF__
#define __NCAPIDEF__
#define NC_LNK_APSRVONLINE     161   /* AP的在线信息              		 */
#define NC_LNK_MACLOCATION     169   /* Mac采集信息                    */  
#define NC_LNK_APMACONLINE     170   /* AP MAC地址对照表               */
#define NC_LNK_DEPTINFO        171   /* 场所信息                       */
#define NC_LNK_SECFACTORY      172   /* 安全厂商信息                   */
#define  NC_MACLOG_VER_AEROSCOUT       1                 // 标准
#define  NC_MACLOG_VER_GBCOM           2                 // 寰创COM
#define  NC_MACLOG_VER_H3C             3                 // H3C


// 在线AP信息 2014/12/3 增加了一些字段
typedef struct ncApSrvOnline_s {
    uchar         apname[18];                  // Ap名称,12个大写的MAC地址表示
    uchar         status;                      // 0--空闲  1--正常  1--正在使用
    uchar         dbmark;                      // 1--在数据库中已存在  0--在数据库中不存在
    uint4         lSip;                        // 通信地址
    uint4         starttime;                   // 起始时间
    uint4         lasttime;                    // 最后时间   
    uint4         bpflasttime;
    uint4         lGid;                        // AP所属部门ID
    uint8         lBytes;                      // 报文流量
    uint4         lPkts;                       // 数据包数量
    uint4         lNum;                        // 在线人数
    uint4         lIp;                         // IP地址
    uchar         devid[32];                   // 设备ID
    uchar         model[16];                   // 型号
    uchar         ssid[32];
    uchar         servicecode[16];
    uchar         servicename[64];             // 场所端名称
    uchar         address[64];                 // 安装位置
    uchar         dispname[64];
    uchar         plate[32];
    uchar         line[32];
    uchar         mapid[32];
    uchar         longitude[12];               // 经度
    uchar         latitude[12];                // 纬度
    uint4         reporttime;                  // 上报时间
    uint4         uptimestep;                  // 上传时间间隔
    uint4         radius;                      // 采集半径
    uchar         cursrvbpfrule[256];          // srv bpf rule
    uchar         curbpfrule[246];             // ap current bpf rule
    uchar         scode[10];                   // 安全厂商编码
    uchar         version[8];
    uchar         apmac[64];                   // AP的MAC地址
    uchar         *pGroup;                     // 部门ID
    uchar         *pDept;                      // 单位ID
    uint4         lMacVer;                     // 嗅探格式版本   
    uint4         lSynTime;                    // 最后一次和服务器端同步时间
    uint4         lMacTime;                    // Mac嗅探最后采集时间
    uchar         cmode;                       // 修改标识
    uchar         mtype;                       // 类别 1--固定AP  2--移动车载   3--单兵采集设备 9--其它
    uchar         synflags;
    uchar         cWorkFlags;                  // 1--审计   2--Mac嗅探  3--审计+MAC嗅探
    uint2         nMarkNum;
    uint2         lMacCount;                   // MAC总数
    uint4         lGpsTime;                    // GPS采集时间
    uint4         lModiTime;                   // 最后一次修改时间
}  ncApSrvOnline;


typedef struct ncApMacOnline_s {
    uchar           apmac[6];
    uchar           apname[18];
    ncApSrvOnline   *psAp;
}   ncApMacOnline;


typedef struct ncBcpSec_s {    // 安全厂商
        uchar           scode[16];
        uchar           sname[64];
        uchar           slink[32];
        uchar           saddress[128];
        uchar           smail[32];
        uchar           stel[32];
        int             flags;
        uint4           moditime;
        struct ncBcpSec_s *next;
} ncBcpSec;




/*   
     场所信息
     数据放在数组中根据servicename排序
     
 */

typedef struct ncDeptinfo_s {    /* 组信息  */
    char           name[16];            // 场所编码
    char           disp[64];
    char           address[128];
    uint4          lDid;
    uint4          lGid;              /* 组ID                            */
    uchar          cmod;            /* 0--正常  1--修改                  */
    uchar          cdb;             /* 0--数据库中不存在  1--已存在      */
    uchar          gtype;           // 场所类别 1-D
    uchar          jytype;
    uchar          ceoname[16];
    uchar          ceoidtype[4];
    uchar          ceoidno[20];
    uchar          ceotel[16];
    uchar          sdate[6];
    uchar          edate[6];
    uint4          lSip;          // 通信地址
    uint2          nSport;        // 起始端口
    uint2          nEport;        // 终止端口
    uchar          caLongitude[12];
    uchar          caLatitude[12];
    uchar          caAccount[32];    // ADSL账号或者IP地址
    uint8          lUp;
    uint8          lDown;
    uint4          lUser;
    uint4          lAps;                 // AP数量
    uint4          lOip;                 // 外网地址
    uint4          lOnlineAp;            // 在线AP数量
    uint4          lasttime;             /* 最后时间                                       */
    uint4          moditime;             /* 最后修改时间                                   */
    uchar          caScode[10];
    uchar          jointype;
    uchar          joincode;
    uchar          shflags;              // 0--未审  1--已审
    uchar          status;               // 0--正常  1--停业
    uchar          caRev[2];             // 保留
    ncBcpSec       *psSec;
} ncDeptinfo;



#endif
