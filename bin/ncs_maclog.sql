-- 终端特征信息
drop table if exists ncmactermatt_if_201507;
create table ncmactermatt_if_201507 (
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
    assid       char(255)     default ' ',                 -- 接入热点SSID
    sid         bigint        default 0                    -- 记录序列号
);




-- 热点信息
drop table if exists ncmachotinfo_if_201507;
create table ncmachotinfo_if_201507 (
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

-- 终端特征移动采集设备
drop table if exists ncmacmobileap_if_201507;
create table if not exists ncmacmobileap_if_201507 (
    apname      char(21)      primary key,              -- AP设备编号
    servicecode char(14)      NOT NULL,                 -- 场所端编码
    servicename char(255)     default ' ',              -- 场所名称
    longitude   char(11)      NOT NULL,                 -- 经度
    latitude    char(11)      NOT NULL,                 -- 维度
    stime       bigint        NOT NULL,                 -- 时间
    sid         bigint        NOT NULL                  -- 序列号
    );
    


