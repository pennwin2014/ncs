-- 最后修改时间  2015/06/21     By Liyunming
-- MAC地址采集标准
-- 
-- 组信息


-- 终端特征采集设备基础信息
drop table if exists ncaplist;
create table if not exists ncaplist (
    apname          char(21)      primary key,                 -- AP设备编号(安全厂商组织编码+MAC地址) 9 + 12
    servicecode     char(14)      NOT NULL,                    -- 场所端编码
    dispname        char(128)     NOT NULL,                    -- 采集设备名称（新增)
    servicename     char(255)     default ' ',                 -- 场所端名称
    did             int  unsigned default 0,                   -- 场所端ID
    version         char(16)      default ' ',                 -- 版本号
    model           varchar(255)     default ' ',              -- 设备型号, 移动AP,该值表示车厢信息
    devid           char(64)      default ' ',                 -- 设备ID, 移动， 移动AP,该值表示车牌号码
    ip              char(16)      default ' ',                 -- IP地址
    mac             char(64)      default ' ',                 -- AP MAC地址,若多个用逗号分隔,一个AP可能会对应多个MAC地址
    ssid            char(64)      default ' ',                 -- SSID,若多个用逗号分隔    
    address         char(255)     NOT NULL,                    -- 安装位置
    plate           char(128)     default ' ',                 -- 移动AP时，该值为站点信息
    line            varchar(255)     default ' ',              -- 移动AP时，该值为线路信息
    mapid           varchar(255)     default ' ',              -- 当固定AP时，该值内容为楼层   移动AP时，该值为地铁车厢编号
    longitude       char(11)      default ' ',                 -- 经度
    latitude        char(11)      default ' ',                 -- 维度
    starttime       int unsigned  default 0,                   -- 开始时间
    lasttime        int unsigned  default 0,                   -- 最后使用时间
    bpflasttime     int unsigned  default 0,                   -- Bpf最后时间
    cursrvbpfrule   varchar(255)   default 0,                  -- Bpf最后同步规则
    aptype          int(1)           NOT NULL,                 -- AP类别 1--固定采集设备  2--移动车载采集设备  3--单兵采集设备 9--其它 
    fcode           char(9)          NOT NULL,                 -- 安全厂商代码
    radius          int(4)          NOT NULL,                  -- 数据采集半径（采集设备用)
    uptimestep      int(8)          NOT NULL,                  -- 数据上传时间间隔
    flags           int(1) default 0,                           -- 0--未审核  1--正常
    moditime        bigint(20)
) ENGINE=MyISAM;



-- 场所基础信息(管理中心端)
drop table if exists ncsuser;
create table ncsuser (
    userid      int   primary key,              -- 单位ID md5(username)
    username    char(14)            NOT NULL,   -- 登录名, 单位编码 服务场所代码
    password    char(32) default ' ',           -- 密码
    groupid     int      default 0,               -- 组ID
    dispname    char(255)           NOT NULL,   -- 场所名称
    address     char(255)           NOT NULL,   -- 场所地址
    telphone    char(64) default ' ',                       -- 安全人联系电话
    contact     char(32) default ' ',                       -- 信息安全员
    handphone   char(32) default ' ',                       -- 法人代表(负责人)电话
    email       char(128) default ' ',          -- 信息安全员E-Mail地址
    ceoname     char(32) default ' ',           -- 法人代表(负责人)
    postcode     char(6) default ' ',           -- 邮编
    servicestate char(1) default '9',           -- 服务状态 1-装机开始服务 2-装机停止服务 3-未装机开始服务 4-未装机停止服务 9-其他
    joincode     char(6) default ' ',           -- 接入服务商代码
    endnum       char(6) default '0',           -- 终端数
    servernum  char(2)   default '0',           --  服务器数
    jointype    char(2)  default '99',          -- 接入方式代码 01-专网真实ip地址 02-专线 03-ADSL拨号 04-ISDN 05-普通拨号 06-Cable moden拨号 07-电力线 08-无线上网 99-其他
    empnum      char(6)  default '0',           -- 从业人数
    area        char(70) default ' ',           -- 所属地区
    areaman     char(30) default ' ',           -- 管理单位负责人
    areatel     char(30) default ' ',           -- 管理单位负责人电话
    demo        char(255) default ' ',          -- 备注
    ip          char(64) default ' ',           -- 出口IP地址
    addtime     char(16) default ' ',           -- 加入时间
    gtype       char(1)  NOT NULL,              -- 单位类别  M20061001
                                                --  1-酒店宾馆 2-图书馆阅览室 3-学校 4-娱乐场所 5-一般企业 
                                                --  9-其它
    useflags    int     default 0,              -- 状态 0 正常
    moditime    bigint  default 0,              -- 记录最后修改时间
    version     char(16) default 0,             -- 版本号
    lasttime    char(16) default ' ',                       -- 最后操作时间
    longitude   char(11)  default ' ',          -- 经度
    latitude    char(11)  default ' ',            -- 纬度
    monflag     int     default 0,              -- 监控级别 0-正常监控 1-重点监控 2-一般监控
    fcode       char(9)  NOT NULL,              -- 厂商代码
    dateid     int default 0,                   -- 营业时间ID
    jytype     int default 1,                   -- 场所经营性质 0--经营 1--非经营 3--其它
    ceoidtype   char(3) default ' ',              -- 法人代表有效证件类别
    ceocardid   char(128) default ' ',          -- 法人代表有效证件
    ceotel      char(128) default  '',            --   联系方式
    sdate   char(5)  default '08:00',              --  营业开始时间
    edate   char(5)  default '18:00',              --  营业结束时间
    port_start       char(20)  default ' ',                      --  场所外网起始端口
    port_end         char(20)  default ' ',                       --  场所外网终止端口
    shflag          int default 0,                  -- 0 未审 1 已审
    workflag        int default 0,                  -- 0 正常 1 维护
    yyflag          int default 0                  -- 0 营业 1 停业
) ENGINE=MyISAM;

create unique index ncsuser_ind1 on ncsuser(username);
create index ncsuser_ind2 on ncsuser(groupid);



--  安全厂商信息
   
drop table if exists ncsfactorycod;
create table ncsfactorycod (
    sid          int unsigned   primary key, --  主键
    code         char(9)   NOT NULL,                   --  代码
    name         char(70)  NOT NULL,                  --  厂家名称
    slink        char(128) NOT NULL,       --  安全厂商联系人
    saddress     char(255)  NOT NULL,       --  安全厂商地址
    stel         char(128)  NOT NULL,        --  联系电话
    smail        char(32)  NOT NULL,       --  邮件地址
    flags        int default 0,              -- 0-未审核 1-已审核
    moditime     bigint default 0            -- 数据最后更新时间 
) engine=MyISAM;

create unique index ncsfactory_ind1 on ncsfactorycod(code);



-- 终端特征信息
drop table if exists ncmactermatt_if_201506;
create table ncmactermatt_if_201506 (
    mac         char(17)      NOT NULL,                    -- Mac地址
    termtype    char(128)     default ' ',                 -- 终端品牌
    servicecode char(14)      NOT NULL,                    -- 场所端编号
    servicename char(128)     default ' ',                 -- 场所端名称
    address     char(128)     default ' ',                 -- 安装位置
    stime       bigint        NOT NULL,                    -- 采集时间
    rssi        char(8)       NOT NULL,                    -- 场强
    vtype       int(1)        default 0,                   -- 身份类型
    vname       char(64)      default ' ',                 -- 身份内容
    ssid        char(255)     default ' ',                 -- SSID,若多个用逗号分隔    
    apname      char(21)      NOT NULL,                    -- AP名称
    apmac       char(17)      NOT NULL,                    -- APMac地址
    channel     char(2)       default ' ',                 -- 接入热点频道
    security    char(2)       default ' ',                 -- 加密类型
    xpos        char(8)       default ' ',                 -- X坐标
    ypos        char(8)       default ' ',                 -- Y坐标
    longitude   char(11)      NOT NULL,                    -- 经度
    latitude    char(11)      NOT NULL,                    -- 维度
    flags       int(1)        default 0,                   -- 上下线标识 0--进入  1--离开  2--Update
    sid         bigint        default 0                    -- 记录序列号
);




-- 热点信息
drop table if exists ncmachotinfo_if_201605;
create table ncmachotinfo_if_201605 (
    mac         char(17)      NOT NULL,                  -- Mac地址
    ssid        char(64)      default ' ',               -- 热点SSID
    channel     char(2)       NOT NULL,                  -- 接入热点频道
    servicecode char(14)      NOT NULL,                 -- 场所端编号
    servicename char(128)     default ' ',              -- 场所端名称
    address     char(128)     default ' ',              -- 安装位置
    stime       bigint        NOT NULL,                 -- 采集时间
    rssi        char(8)       NOT NULL,                 -- 场强
    apname      char(21)      NOT NULL,                    -- AP名称
    apmac       char(17)      NOT NULL,                    -- APMac地址
    security    char(2)       default ' ',                 -- 加密类型
    xpos        char(8)       default ' ',                 -- X坐标
    ypos        char(8)       default ' ',                 -- Y坐标
    longitude   char(11)      NOT NULL,                    -- 经度
    latitude    char(11)      NOT NULL,                    -- 维度
    flags       int(1)        default 0,                   -- 上下线标识 0--进入  1--离开  2--Update
    sid         bigint        default 0                    -- 记录序列号
);

-- 日志型，每月保存一个表
drop table if exists ncmacmobileap_if_201506;
create table if not exists ncmacmobileap_if_201506 (
    apname      char(21)      primary key,              -- AP设备编号
    servicecode char(14)      NOT NULL,                 -- 场所端编码
    servicename char(255)     default ' ',              -- 场所名称
    longitude   char(11)      NOT NULL,                 -- 经度
    latitude    char(11)      NOT NULL,                 -- 维度
    stime       bigint        NOT NULL,                 -- 时间
    sid         bigint        NOT NULL                  -- 序列号
    );
    
    
    
--  热点信息库

drop table if exists nchotspotinfo;
create table nchotspotinfo (
    hotspotid          int unsigned  auto_increment primary key,  --  主键
    ssid         char(64)   NOT NULL,               --  热点SSID
    security     char(2)    ,                       --  加密类型
    authflag     char(128) default 2,               --  合规判断 0--非合规  1--合规  2--其他
    isp          char(255)  default '' ,            --  热点运营商
    servicetype	  char(2)	  default ' ',            --   允许场所服务类型  对应ncsuser_lb中的代码
    servicequlity  int default 1,                   --  场所经营性质 0--经营 1--非经营 3--其它
	  ISPname	     char(30) default ' ',              -- 运营商联系人
	  ISPphone     char(64),                          -- 运营商联系电话
	  ISPemail	   char(32)  	,                       -- 运营商联系邮箱
	  Regtime      int unsigned,                      -- 备案时间
	  note        text                                -- 备注      
) engine=MyISAM;
create unique index nchotspotinfo_ind1 on nchotspotinfo(ssid); 

--  热点mac组

drop table if exists nchotmacgroup;
create table nchotmacgroup (
    ssid         char(64)   NOT NULL,               --  热点SSID
    mac          char(20)    ,                       --  热点MAC
    sitename    char(128) default 2               --  单位名称
) engine=MyISAM;
create index nchotmacgroup_ind1 on nchotmacgroup(ssid); 

--  热点mac组

drop table if exists nchotmacgroup;
create table nchotmacgroup (
    ssid         char(64)   NOT NULL,               --  热点SSID
    mac          char(20)    ,                       --  热点MAC
    sitename    char(128) default 2               --  单位名称
) engine=MyISAM;
create index nchotmacgroup_ind1 on nchotmacgroup(ssid); 

-- 终端MAC组
drop table if exists nctermmacgroup; 
create table nctermmacgroup (
    groupid      int unsigned  auto_increment primary key,  --  主键
    groupname    char(32),                         -- 组名
    groupdesc    char(255)                         -- 描述
) engine=MyISAM;

-- 终端MAC组内容
drop table if exists nctermmacgroupinfo;
create table nctermmacgroupinfo (
    sid          int unsigned  auto_increment primary key,  --  主键
    mac          char(20) ,  --  
    groupid      int unsigned   ,  --  组ID
    modtime      int unsigned      -- 修改时间 
) engine=MyISAM;
create index nctermmacgroupinfo_ind1 on nctermmacgroupinfo(groupid); 

-- 终端伴随行为分析任务表
drop table if exists nctermtask;
create table nctermtask (
    cid          int unsigned  auto_increment primary key,  --  主键
    groupid      int unsigned,     -- 行政区ID,关联用户权限
    starttime    int unsigned,     -- 起始时间
    endtime        int unsigned,     --  结束时间
    taskname     char(64),           -- 任务名称
    taskdesc     char(255),          -- 任务描述
    dids         char(255) default '', -- 伴随分析的场所ID，可以多个,逗号分割 空为全部
    mac          char(20),             -- 主MAC
    maccount     int unsigned,      -- 相同伴随MAC个数大于
    servcount    int unsigned,      -- 相同场所出现次数
    macgroups    char(255) default '', -- 伴随mac组
    opname       char(32),           -- 操作员
    optime       int unsigned,        -- 创建时间
    modtime      int unsigned,        -- 修改时间
    reoporttime  int unsigned  default 0,     -- 报表记录更新时间
    updatetime   int unsigned  default 0,      -- 状态更新时间
    result       int unsigned default 0     -- 任务状态 0-正常 1-失效 2-结束
) engine=MyISAM;


-- 终端伴随行为分析结果表
drop table if exists nctermtasklog;
create table nctermtasklog (
    sid          int unsigned  auto_increment primary key,  --  主键
    cid          int unsigned  ,  --  任务ID 和nctermtask 表中的CID管理
    mac         char(17)      NOT NULL,                    -- Mac地址
    servicecode char(14)      NOT NULL,                    -- 场所端编号
    servicename char(128)     default ' ',                 -- 场所端名称
    address     char(128)     default ' ',                 -- 安装位置
    stime       bigint        NOT NULL,                    -- 采集时间
    vtype       int(1)        default 0,                   -- 身份类型
    vname       char(64)      default ' ',                 -- 身份内容
    ssid        char(255)     default ' ',                 -- SSID,若多个用逗号分隔    
    apname      char(21)      NOT NULL,                    -- AP名称
    apmac       char(17)      NOT NULL,                    -- APMac地址
    longitude   char(11)      NOT NULL,                    -- 经度
    latitude    char(11)      NOT NULL                    -- 维度
) engine=MyISAM;
create index nctermtasklog_ind1 on nctermtasklog(cid); 


-- 终端MAC布控策略表    为了和原管理中心兼容便于以后移植和增加功能沿用原来布控信息表
drop table if exists ncscaseinfo;
create table ncscaseinfo (
        sid      int   unsigned auto_increment primary key,
        casecode   char(20),             -- 案件编号
        casename   char(32),             -- 案件名称
        casetype   int,                  -- 案件种类
        caseclass  int,                  -- 审计级别
        groupid    int unsigned,         -- 所属区域
        casedept   char(64),             -- 立案单位
        casedate   int unsigned,         -- 立案时间
        addby      char(16),             -- 操作人员
        cmemo      text,                 -- 案件描述
        lasttime   int unsigned,         -- 记录修改时间
        expiretime int unsigned default 0,    -- 有效时间
        flags      int  default 0        -- 1--已布控  0--未布控   8--删除  9--撤控
) engine=MyISAM;


-- 终端布控MAC规则

drop table if exists ncscasmac;
create table ncscasmac (
    sid      int   unsigned auto_increment primary key,
    cid      int   unsigned,          -- 案件序列号
    macgroupid  int,                  -- MAC地址组
    mac     char(20),                 -- mac地址
    flags    int  default 0,          -- 0--正常  1--停用
    lasttime int  unsigned
) engine=MyISAM;
-- 终端阀值布控策略
drop table if exists ncscasetermlim;
create table ncscasetermlim (
    sid      int   unsigned auto_increment primary key,
    cid      int   unsigned,                    -- 案件序列号
    level    int   unsigned default 1,          -- 监控级别   1-一般 2-中度 3-严重
    dateid   int,                               -- 统计时间频率  1-分钟 2-小时 3-每天
    countlimit     int unsigned default 0,      -- 阀值
    apname     varchar(21),                     -- AP编号
    flags    int  default 0,          -- 0--正常  1--停用
    lasttime int  unsigned
) engine=MyISAM;

-- 告警设置
drop table if exists ncscasewarn;
create table ncscasewarn (
        sid      int   unsigned auto_increment primary key,
        cid      int   unsigned,         -- 案件序列号
        uid      int   unsigned,         -- 操作者ID
        name      char(32),              -- 姓名
        wtype    int,                    -- 告警方式 1-邮件 2-短信
        waddr    char(64),               -- 地址
        flags    int  default 0,         
        lasttime int  unsigned
) engine=MyISAM;

-- 布控区域
drop table if exists ncscaseobj;
create table ncscaseobj (
        sid      int   unsigned auto_increment primary key,
        cid      int   unsigned,         -- 案件序列号
        ctype    int   default 0,        -- 类别 0-单位 1-单位类别  2-单位组
        did      int   unsigned,         -- 根据类别，是单位id,单位类别或组
        lasttime int   unsigned,         -- 记录更新时间
        flags    int   default 0         -- 0--正常  1--停用
) engine=MyISAM;

-- 布控告警日志  
drop  table if exists ncscasemacwarnlog;
create   table ncscasemacwarnlog (
    sid    int   unsigned auto_increment primary key,
    servicecode char(14),     -- 场所编码
    udisp  char(32),           -- 用户名
    mac    char(20), 
    stime  int unsigned,       -- 时间
    ruleid  int unsigned,      -- 规则ID
    msid    bigint default 0,      -- 日志ID 关联ncmactermatt_if 中的SID
    cid    int unsigned,       -- MAC布控策略表ID
    message  char(255),        -- 消息内容
    flags  int default 0,       --     0 待处理 1-已处理
    opname char(32),            -- 操作人员
    optime int default 0       -- 操作时间  
    ) engine=MyISAM;
create index ncscasemacwarnlog_ind1 on ncscasemacwarnlog(stime); 
create index ncscasemacwarnlog_ind2 on ncscasemacwarnlog(cid); 
create index ncscasemacwarnlog_ind3 on ncscasemacwarnlog(mac); 

--  统计数据表 每天从ncmactermatt_if_ 统计  
drop  table if exists ncmactermcount;
create   table ncmactermcount (
    sid    int   unsigned auto_increment primary key,
    servicecode char(14),     -- 场所编码
    apname     char(24),       -- apname
    lcount    int unsigned default 0,    -- mac数量
    sdate     char(10)               -- 日期 格式2014/01/01 
    ) engine=MyISAM;
create index ncmactermcount_ind1 on ncmactermcount(sdate); 

--  统计数据表 接警人员
drop  table if exists stafinfo;
create   table stafinfo (
    sid    int   unsigned auto_increment primary key,
    name  char(16),                -- 姓名
    department     char(64),       -- 部门
    phone    char(20),             -- 手机
    email     char(64)               -- 邮箱
    ) engine=MyISAM;
    
    
    -- 虚拟身份日志
drop table if exists nctermnetlog_if_201508;
create table if not exists nctermnetlog_if_201508 (
    sessionid       char(64)  NOT NULL,                      -- SessionID
    servicecode     char(14) NOT NULL,                       -- 场所端编码
    username        char(64) NOT NULL,                       -- 用户登录名
    mac             char(17) NOT NULL,                       -- 终端mac地址
    apname          char(21) NOT NULL,                       -- AP 编码
    longitude       char(11) NOT NULL,                       -- 移动终端经度
    latitude        char(11) NOT NULL,                       -- 移动终端维度
    stime           bigint NOT NULL,                         -- 日志记录时间
    etime           bigint NOT NULL,                         -- 日志结束时间
    nettype         int(2) NOT NULL,                         -- 网络应用类别
    vname           char(64) NOT NULL,                       -- 虚拟身份账号
    vdisp           char(64) NOT NULL,                       -- 虚拟身份昵称
    sip             int unsigned NOT NULL,                   -- 源IP  
    dip             int unsigned NOT NULL,                   -- 目标IP  
    oip             int unsigned NOT NULL,                   -- 源外网IP地址
    fcode           char(10) NOT NULL,                       -- 安全厂商代码
    flags           int(1)  NOT NULL,                        -- 0--进入  1--离开
    sid             bigint default 0                         -- 记录序列号      
)  max_rows=1000000000 avg_row_length=1500;


--- 虚拟身份库
drop table if exists nctermnetlog;
create table if not exists nctermnetlog (
    mac             char(17) NOT NULL,                       -- 终端mac地址
    nettype         int    NOT NULL,                         -- 网络应用类别 和ncsnettype 表中的nettype关联
    vname           char(32) NOT NULL,                       -- 虚拟身份账号
    vdisp           char(32) default ' ',                    -- 虚拟身份昵称
    username        char(32) default ' ',                    -- 关联账号
    starttime       bigint NOT NULL,                         -- 采集时间
    moditime        bigint NOT NULL,                         -- 最后更新时间
    UNIQUE INDEX   nctermnetlog_ind1 (mac,nettype,vname),
    INDEX          nctermnetlog_ind2 (nettype,vname)
)  max_rows=1000000000 avg_row_length=1500;

--- Mac地址库_场所
drop table if exists nctermmacs;
create table if not exists nctermmacs (
    mac             char(17) NOT NULL,                       -- 终端mac地址
    servicecode     char(14),                                -- 场所
    username        char(32) default ' ',                    -- 关联账号
    starttime       bigint NOT NULL,                         -- 采集时间
    moditime        bigint NOT NULL,                         -- 最后更新时间
    UNIQUE INDEX   nctermmacs_ind1 (mac,servicecode),
    INDEX          nctermmacs_ind2 (moditime)
)  max_rows=1000000000 avg_row_length=1500;

--- 系统运维告警日志
drop table if exists nctermsysalarm;
create table if not exists nctermsysalarm (
    sid    int   unsigned auto_increment primary key,
    alarmcode     char(14),                                -- 告警类别码
    alarmlevel    char(32) default ' ',                -- 告警级别 1-紧急 2-验证 3-一般
    devtype       char(4),                             -- 01 -AP 02-场所 03-管理中心 04-上级平台
    alarmtime          int default 0,                         -- 告警时间
    updatetime         int default 0,                  -- 告警上报时间
    description          char(128),                          -- 告警描述
    status           int default 0,                    -- 0未确认 1-已确认                 
    INDEX          nctermsysalarm_ind1 (alarmtime)
)  max_rows=1000000000 avg_row_length=1500;

--- Mac地址库_AP
drop table if exists nctermmacs_ap;
create table if not exists nctermmacs_ap (
    mac             char(17) NOT NULL,                       -- 终端mac地址
    apname          char(22),                                -- ap
    starttime       bigint NOT NULL,                         -- 采集时间
    moditime        bigint NOT NULL,                         -- 最后更新时间
    UNIQUE INDEX   nctermmacsap_ind1 (apname,mac),
    INDEX          nctermmacsap_ind2 (moditime)
)  max_rows=1000000000 avg_row_length=1500;

