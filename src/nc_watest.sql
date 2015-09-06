-- 场所基础信息

drop table if exists  ncgroup;
create table ncgroup (
    groupid     int  unsigned   primary key,                     -- 组ID
    groupname   char(32) NOT NULL,                        -- 场所编码
    flags       int    default 0  NOT NULL,
    limitunit   int    default  9,                      --  限制周期 1--每日 2--每周   3--每月  4--全部
    flowlimit   int unsigned  default 0,                --  流量限制  MB   
    timelimit   int unsigned  default 0,                --  时间限制  分钟 
    overctl     int    default  0,                      --  超出后的控制方式 0-禁用  服务策略 ID 
    thistime    int unsigned  default  0,               --  当前时间
    thisflow    bigint  unsigned default 0,             --  当前流量
    resettime   int unsigned default 0,                 --  流量和时间重置时间(系统内部使用)
    lasttime    int unsigned  default 10,               --  最后操作时间
    addtime     int unsigned  default 10,               --  记录更新时间
    pid int unsigned default 0,                         --  上一级部门
    status      int unsigned default  10,               --  2008年12月31日修改 监控状态  0 正常  1 禁用  4 监控网站   3 监控IP访问  9 免监控 10 未设置
    dispname    char(255) default ' ',                   --  场所名称
    level int unsigned default 0
);

create UNIQUE index ncgroup_index on ncgroup (groupname,level);
-- 场所基础信息附加表
drop table if exists ncgroupatt;
create table ncgroupatt (
    groupid        int unsigned  primary key,
    address        char(255) default '',            --  地址
    longitude      char(11) default '',             --  经度
    latitude       char(11) default '',             --  纬度
    gtype          char(2) default '',              --  服务类型  1:旅馆类 2：图书馆阅览室 3：电脑培训中心 4：娱乐场所类 5：交通枢纽 6：公共交通工具 7：餐饮服务场所 8：金融服务场所 A购物场所 B 公共服务场所 C 文化服务场所 D公共休闲场所 9其它
    jytype         int default 9,                   --  经营   0 经营 1：非经营 3 其它
    ceoname        char(64) default '',             --  法人姓名
    ceoidtype      char(3) default '111',           --  证件类型
    ceoidno        char(128)  default '',            --  证件号码
    ceotel         char(128)  default '',            --  联系方式
    sdate          char(20)  default '08:00',              --  营业开始时间
    edate          char(20)  default '18:00',              --  营业结束时间
    jointype       char(4) ,                         --  接入方式
    joincode       char(4) default '01',             --  网络服务商
    ip             char(64) default '',              --  外网IP
    scode          char(10) default '',              --  安全厂商组织机构代码
    sname         char(128) default '',              --  安全厂商名称
    slink         char(128) default '',              --  安全厂商联系人
    saddress      char(128) default '',             --  安全厂商地址
    stel          char(16) default '',              --  联系电话
    smail         char(128) default '',              --  邮件地址
    port_start       char(20),                       --  场所外网起始端口
    port_end         char(20)                        --  场所外网终止端口
);


-- AP信息描述, 在原有基础上有所修改 2015/04
drop table if exists ncaplist;
create table if not exists ncaplist (
    apname      char(21)      primary key,                 -- AP设备编号
    servicecode char(32)      default ' ',                 -- 场所端编码
    servicename char(255)     default ' ',                 -- 场所端名称
    did         int  unsigned default 0,                   -- 场所端ID
    version     char(16)      default ' ',                 -- 版本号
    model       char(255)     default ' ',                 -- 设备型号, 移动AP,该值表示车厢信息
    devid       char(64)      default ' ',                 -- 设备ID, 移动， 移动AP,该值表示车牌号码
    ip          char(16)      default ' ',                 -- IP地址
    mac         char(64)      default ' ',                 -- AP MAC地址,若多个用逗号分隔,一个AP可能会对应多个MAC地址
    ssid        char(64)      default ' ',                 -- SSID,若多个用逗号分隔    
    address     char(64)      default ' ',                 -- 安装位置
    plate       char(128)     default ' ',                 -- 移动AP时，该值为站点信息
    line        char(255)     default ' ',                 -- 移动AP时，该值为线路信息
    mapid       char(255)     default ' ',                 -- 当固定AP时，该值内容为楼层   移动AP时，该值为地铁车厢编号
    longitude   char(11)      default ' ',                 -- 经度
    latitude    char(11)      default ' ',                 -- 维度
    starttime   int unsigned  default 0,                   -- 开始时间
    lasttime    int unsigned  default 0,                   -- 最后使用时间
    bpflasttime int unsigned  default 0,                   -- Bpf最后时间
    cursrvbpfrule char(255)   default 0,                   -- Bpf最后同步规则
    aptype      int           default 0,                   -- AP类别 0--固定  1--移动
    flags       int default 0                              -- 0--未审核  1--正常
);

--  安全厂商信息
drop table if exists ncsfactorycod;
create table ncsfactorycod (
    sid          int unsigned   primary key, --  主键
    code         char(10),                   --  代码
    name         char(128),                  --  厂家名称
    slink        char(128) default '',       --  安全厂商联系人
    saddress     char(128) default '',       --  安全厂商地址
    stel         char(32) default '',        --  联系电话
    smail        char(128) default '',         --  邮件地址
    flags        int default 0               -- 0-未审核 1-已审核
)engine=MyISAM;
create unique index ncsfactory_ind1 on ncsfactorycod(code);


-- 无线上下线日志  2015/04修改
drop table if exists nctermlog_if_201505;
create table if not exists nctermlog_if_201505 (
    sessionid       char(64),                       -- SessionID  ServiceCode+Mac+Starttime
    authtype        char(7),                        -- 认证类别
    servicecode     char(14),                       -- 场所端编码
    servicetype     char(2),                        -- 场所类别
    username        char(64),                       -- 用户登录名
    dispname        char(64),                       -- 姓名
    mac             char(18),                       -- 终端mac地址
    apmac           char(18),                       -- AP MAC地址
    apname          char(21),                       -- AP 编码
    longitude       char(11),                       -- 移动终端经度
    latitude        char(11),                       -- 移动终端维度
    rssi            char(8),                        -- 场强
    xposition       char(8),                        -- 位置 X
    yposition       char(8),                        -- 位置 Y
    idno            char(128),                       -- 证件号
    idtype          char(3),                        -- 证件类型
    appsrc          char(128),                       -- APP厂商
    appname         char(128),                       -- APP名称
    appversion      char(16),                       -- APP版本号
    appid           char(128),                       -- APP ID
    stime           int unsigned,                   -- 开始时间
    etime           int unsigned,                   -- 结束时间 
    sip             int unsigned,                   -- 源IP  
    oip             int unsigned,                   -- 源外网IP地址
    sport           int,                            -- 源起始端口
    eport           int,                            -- 源结束端口
    imsi            char(64),                       -- IMSI
    imei            char(64),                       -- Imei
    termname        char(128),                       -- 终端品牌
    termos          char(64),                       -- 终端系统
    termtype        char(128),                       -- 终端类型
    conntime        int,                            -- 连接时间
    sumflow         bigint,                         -- 总流量
    flags           smallint,                       -- 上下线标识 1--上线  --2下线
    fcode           char(10),                       -- 安全厂商代码
    sid             bigint default 0                -- 记录序列号      
)   max_rows=1000000000 avg_row_length=1500;

create index nctermlog_if_201505_i1 on nctermlog_if_201505 (sessionid);
create index nctermlog_if_201505_i2 on nctermlog_if_201505 (servicecode);
create index nctermlog_if_201505_i3 on nctermlog_if_201505 (stime);



-- 上网日志
drop table if exists nctermiplog_if_201505;
create table if not exists nctermiplog_if_201505 (
    sessionid       char(64),                       -- SessionID
    servicecode     char(14),                      -- 场所端编码
    servicetype     char(2),                        -- 场所类别
    username        char(64),                      -- 用户登录名
    dispname        char(64),                    -- 姓名
    mac             char(18),                       -- 终端mac地址
    apmac           char(18),                       -- AP MAC地址
    apname          char(21),                       -- AP 编码
    longitude       char(11),                       -- 移动终端经度
    latitude        char(11),                       -- 移动终端维度
    stime           int unsigned,                   -- 日志记录时间
    nettype         int,                            -- 网络应用类别
    sip             int unsigned,                    -- 源IP  
    sport           int,                            -- 源起始端口
    oip             int unsigned,                   -- 源外网IP地址
    osport          int,
    oeport          int,                            -- 源结束端口
    dip             int unsigned,                    -- 源IP  
    dport           int,                            -- 源起始端口
    sumflow         bigint,                         -- 总流量
    fcode           char(10),                       -- 安全厂商代码
    sid             bigint default 0                -- 记录序列号      

)  max_rows=1000000000 avg_row_length=1500;

create index nctermiplog_if_201505_1 on nctermiplog_if_201505 (sessionid);
create index nctermiplog_if_201505_2 on nctermiplog_if_201505 (stime,servicecode);


-- 管理中心数据结构

-- 监控单位信息
drop table if exists ncsuser;
create table ncsuser (
    userid      int   primary key,              -- 单位ID
    username    char(16),                       -- 登录名, 单位编码 服务场所代码
    password    char(32) default ' ',                       -- 密码
    groupid     int    default 0,               -- 组ID
    dispname    char(255) default ' ',                      -- 场所名称
    address     char(255) default ' ',                      -- 场所地址
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
    gtype       int     default 9,              -- 单位类别  M20061001
                                                --  1-酒店宾馆 2-图书馆阅览室 3-学校 4-娱乐场所 5-一般企业 
                                                --  9-其它
    useflags    int     default 0,              -- 状态 0 正常
    moditime    int  unsigned default 0,
    version     char(16) default 0,             -- 版本号
    lasttime    char(16) default ' ',                       -- 最后操作时间
    longitude   char(11)  default ' ',          -- 经度
    latitude    char(11)  default 0,            -- 纬度
    monflag     int     default 0,              -- 监控级别 0-正常监控 1-重点监控 2-一般监控
    fcode    char(10)  default '',              -- 厂商代码
    dateid     int default 0,                   -- 营业时间ID
    jytype     int default 1,                   -- 场所经营性质 0--经营 1--非经营 3--其它
    ceoidtype char(4) default ' ',              -- 法人代表有效证件类别
    ceocardid   char(128) default ' ',          -- 法人代表有效证件
    ceotel         char(64)  default  '',            --   联系方式
    sdate   char(20)  default '08:00',              --  营业开始时间
    edate   char(20)  default '18:00',              --  营业结束时间
    port_start       char(20)  default ' ',                      --  场所外网起始端口
    port_end         char(20)  default ' ',                       --  场所外网终止端口
    shflag          int default 0,                  -- 0 未审 1 已审
    workflag        int default 0,                  -- 0 正常 1 维护
    yyflag          int default 0                  -- 0 营业 1 停业
) ENGINE=MyISAM;

create unique index ncsuser_ind1 on ncsuser(username);
create index ncsuser_ind2 on ncsuser(groupid);
