
-- 网络督察管理中心数据库表结构
-- 创建数据库:
-- create database ncs;
-- grant all privileges on ncs.*  to ncs@localhost identified by 'ncs';
-- alter table ncsfactorycod add moditime bigint default 0;

-- 20150523 增加单位表几个状态
-- alter table ncsuser add shflag int default 0; alter table ncsuser add workflag int default 0;alter table ncsuser add yyflag int default 0;
-- 20150520 增加场所各种数据统计表
-- 20150519


-- alter table ncsuser add jytype int default 1;
-- alter table ncsuser add ceoidtype char(4) default ' ';
-- alter table ncsuser add ceocardid char(32) default ' ';
-- alter table ncsuser add ceotel char(64) default ' ';
-- alter table ncsuser add sdate char(20) default '08:00';
-- alter table ncsuser add edate char(20) default '18:00';
-- alter table ncsuser add port_start char(20) default '';
-- alter table ncsuser add port_end char(20)   default '';
-- alter table ncsfactorycod add slink char(32) default ' ';
-- alter table ncsfactorycod add saddress char(255) default '';
-- alter table ncsfactorycod add stel char(64) default '';
-- alter table ncsfactorycod add smail char(32) default '';
-- alter table ncsuser modify gtype char(2) default '';
-- ---------------------------------------------修改记录------------------------------------------------------
-- 2015/03/10     alter table ncsuser modify longitude char(12) default ''; alter table ncsuser modify latitude char(12) default '';
-- 2014/08/06     ncs_twlog  增加字段filename      alter table ncs_twlog add filename char(80) default '';
-- 2014/03/18     ncssmslog 增加是否成功标志 alter table ncssmslog add flag int default 0;
-- 2013/12/02       nchoteluser 增加mobile ,slgbm,expiretime alter table nchoteluser add mobile char(20);alter table nchoteluser add expiretime int default 0;alter table nchoteluser modify slgbm char(16);
-- 2013/11/08        ncssmslog增加接入厂商   alter table ncssmslog add sms char(32) default '';
-- 2013/09/24     增加广告统计ncadlog
-- 2013/08/20     增加策略下发表 ncskeyword_procy
-- 2013/08/18     增加场所类别   ncsuser_lb
-- 2013/08/14     增加ncsuser的辅助表ncsuser_att,目的是增加一些辅助字段，保持ncsuser相对稳定
-- 2013/05/31     增加系统工具条按钮 dstoolsy_v4
-- 2013/05/22  -- 增加表 ncsuserinfo
--                alter table ncsenduser modify username char(32) default ' ';
-- 2013/05/08  -- 增加ncslastinfo_v4   主要原因是lasttime字段采用bigint 适应日志表的SID
-- 2013/05/02   --增加物流信息监控表
-- 2013/04/28  ncsnettype 表增加gid字段 alter table ncsnettype add gid int default 0;
-- 2013/04/26  dsuser增加管理厂商 alter table dsuser add fac char(255) default 'all';
-- 2013/04/25  ncsuer增加厂商代码和营业时间ID  alter table ncsuser add fcode char(10) default '';alter table ncsuser add dateid int default 0;
-- 2013/03/26  增加ncskeywordlog ncssenurllog,ncssennetidlog
-- 2013/03/20  增加重点监控用户库
-- 2012/10/30  增加接入厂家代码维护表 ncsfactorycod
-- 2012/06/27  增加IDC策略ncrule，ncruledef，ncrule_area
-- 2012/06/20  增加网络督察管理下的多个单位信息表ncsusersubcorp
-- 2012/05/23 增加存放接收迈外迪注册用户表ncmwduser;
-- 2012/03/30 ncsuser表中增加经度、维度两个字段 alter table ncsuser add longitude  bigint(20)  default 0; alter table ncsuser add latitude  bigint(20)  default 0;
-- 2012/03/26 reporttask 增加flags字段 alter table reporttask add flags int unsigned default 0;
-- 2012/03/26 ncsreportlog 增加flags字段 alter table ncsreportlog add flags int unsigned default 0;
-- 2012/03/19 增加常用手机表ncsmovdictel
-- 2012/02/09 增加单位特殊组 ncsexcpgroup  在这个单位的单位不统计在线率
-- 2011/11/04 ncsmonform 中增加关键字库keyid alter table ncsmonbbs add keyid int default 0;
-- 2011/11/04 ncsmonbbs 中增加关键字库keyid alter table ncsmonbbs add keyid int default 0;
-- 2011/11/04 ncsmonim 中增加关键字库keyid alter table ncsmonim add keyid int default 0;
-- 2011/11/04 ncsmonmail 中增加关键字库keyid alter table ncsmonmail add keyid int default 0;
-- 2011/11/03 ncsmondefault 中mid=7 表示确实虚拟身份发送规则 增加虚拟身份发送规则ncsvidmon mid=8 表示BBS缺省规则
-- 2011/11/01 增加ncuseridlist 字段 ntype alter table ncuseridlist add ntype int default 0;
-- 2011/10/18 增加ncsreportlog 报表生成日志
-- 2011/09/26 ncsccic增加字段 alter table ncsccic add bm char(32) default '';alter table ncsccic add ch char(32) default '';alter table ncsccic add bh char(32) default '';
-- 2011/09/19 增加表 ncssearchlog
-- 2011/09/16 增加表 ncsuploadlog
-- 2011/09/16 alter table ncsnetseslog add usid int unsigned default 0;
-- 2011/09/13 增加MAC地址库
-- 2011/09/11 即时通信增加贼段  ncsimlog  ---> contid 和 ftid
-- 2011/09/07 下发命令
-- 2011/09/04 ncshttpclass,ncsipserviceday 表增加字段 worktime
-- 2011/08/16 增加统计表ncshttpclass
-- 2011/08/13 增加日志文件中的usid
--            alter table ncsmailmon add usid unsigned int default 0;
-- 2011/05/03 增加QQ密码表
-- 2011/03/07 ncsgroup 增加行政区代码 alter table ncsgroup add groupcode char(10) default '';
-- 2010/12/28 增加表 ncsipservice
-- 2010/08/14 alter table ncsgroup add upip char(128) default '';alter table ncsgroup add upuser char(32) default'';alter table ncsgroup add uppasswd char(32) default '';
-- 2010/07/5 create index ncsimlog_ind3 on ncsimlog (mfrom);create index ncsimlog_ind4 on ncsimlog (sip);
-- 2010/07/3    create  index ncsnetlog_ind5 on ncsnetlog (did,uid);create  index ncsnetlog_ind6 on ncsnetlog (ip);create  index ncsnetlog_ind7 on ncsnetlog (did,vtime);
-- 2010/07/3    create index  ncsuserlog_ind7 on ncsuserlog (ip);
-- 2010/07/3    create index  ncsuserlog_ind8 on ncsuserlog (mac);
-- 2010/05/24   修改表结构ncsuser alter table ncsuser add monflag int default 0;
-- 2009/09/22   增加表ncsoffwarnlog 单位离线告警日志
-- 2009/02/18   增加表
-- 2008/12/16   增加表 ncuserbindmb，ncmbserno
-- 2008/06/18   修改结构 ncsuserlog
-- alter table ncsuserlog add sid int  unsigned auto_increment primary key;
-- 2008/05/16   增加子管理中心
-- 
-- 2008/04/10   增加敏感服务布控  增加两张表 ncscaseservice,ncssenservice
-- 2008/01/17   表dsuser增加字段key2 char(24)
-- alter table dsuser add key2 char(24) default '';
-- 2007/11/06  表 ncscasemesg 增加字段 dip,lsid
-- alter table ncscasemesg add dip int unsigned default 0;
-- alter table ncscasemesg add lsid int unsigned default 0;
-- 2007/06/13  最后整理  By Liyunming
-- 2007/05/21
--  修改表 ncsnetlog
-- 2007/04/09 
--  修改结构 ncsenduser
--  alter table ncsenduser add country    char(3)  default ' ',add idfytype char(3) default ' ',add    idfyunit    char(30) default ' ',add    sex         char(1);
--  2006-10-1 大修改  标识 /* 10061001 */
--  2006-05-08
--     增加 ncrnetlog
--     删掉 ncrnetuser
--     ncsnetlog  增加 netid
--         alter table  ncsnetlog add netid int default 0;  
--     ncsenduser 增加  netuse
--         alter table  ncsenduser add usebase int default 0;
--     去掉 ncsnettype 用 ncsbbsindex 代替
--     ncsnetlog
--         alter table ncsnetlog add descr char(64) default ' ';
-- 2005-12-12   ncshttplog
-- 2005-12-10   ncsuserlog
-- 2005-10-17                  Version 0.05   By Liyunming
-- 2005-09-25                  Version 0.04   By Liyunming
--    ncshttplog 中 tid -> urlid
-- 2005-09-12                  Version 0.03   By Liyunming
-- 2005-09-11                  Version 0.02   By Liyunming
-- 2005-09-01                  Version 0.01   By Liyunming
--


drop table if exists dsappsrv;
create table dsappsrv (
    id int(11)  NOT NULL  DEFAULT '0',
    name char(32) ,
    dispname char(32) ,
    addtime int(10) unsigned ,
    moditime int(10) unsigned ,
    righttime int(10) unsigned ,
    lkey char(24) ,
    clttype smallint(6) ,
    timeout int(11) ,
    lasttime int(10) unsigned ,
    logcount int(11) ,
    useflags smallint(6) ,
    status smallint(6) ,
    PRIMARY KEY(id),
    UNIQUE dsappsrv_01 (name)) engine=MyISAM;

drop table if exists dscltright;
create table dscltright (
    pasid int(11) ,
    name char(32) ,
    dispname char(32) ) engine=MyISAM;

drop table if exists dsdictlist;

create table dsdictlist (
    sid int(11)  NOT NULL  DEFAULT '0',
    dname char(32) ,
    dtype smallint(6) ,
    fname char(32) ,
    ftype smallint(6) ,
    fbytes int(11) ,
    fright char(8) ,
    PRIMARY KEY(sid),
    UNIQUE dsdictlist_ind1 (dname,fname)) engine=MyISAM;

drop table if exists dslogs;
create table dslogs (
    dotime int(10) unsigned ,
    name char(32) ,
    ip char(16) ,
    command char(132) ,
    INDEX dslogs_index (dotime)) engine=MyISAM;

drop table if exists dspasrole;
create table dspasrole (
    roleid int(11)  NOT NULL  DEFAULT '0',
    pasid int(11)  NOT NULL  DEFAULT '0',
    PRIMARY KEY(roleid,pasid)) engine=MyISAM;

drop table if exists dsrole;
create table dsrole (
    id int(11)  NOT NULL  DEFAULT '0',
    name char(32)  NOT NULL  DEFAULT '',
    dispname char(32) ,
    descr char(128) ,
    addtime int(10) unsigned ,
    moditime int(10) unsigned ,
    PRIMARY KEY(id),
    UNIQUE dsrole_01 (name)) engine=MyISAM;

drop table if exists dsroleattr;
create table dsroleattr (
    roleid int(11) ,
    pasid int(11) ,
    flags smallint(6) ,
    lright char(32) ) engine=MyISAM;

drop table if exists dssequen;
create table dssequen (
    id int(10) unsigned ) engine=MyISAM;

drop table if exists dsuser;
create table dsuser (
    id int(11)  NOT NULL  DEFAULT '0',
    name varchar(32) ,
    dispname varchar(32) ,
    groupid int(11) ,
    groupname varchar(32) ,
    addtime int(10) unsigned ,
    moditime int(10) unsigned ,
    lkey varchar(24) ,
    lasttime int(10) unsigned ,
    logcount int(11) ,
    useflags smallint(6) ,
    status smallint(6) ,
    usrlevel smallint(6) ,
    address varchar(128)  DEFAULT '',
    telphone varchar(64)  DEFAULT '',
    handphone varchar(32)  DEFAULT '',
    opt varchar(255)  DEFAULT 'all',
    fac varchar(255)  DEFAULT 'all',
    email varchar(64) ,
    key2 varchar(24) ,
    PRIMARY KEY(id),
    UNIQUE dsuser_01 (name)) engine=MyISAM;

drop table if exists dsuserrole;
create table dsuserrole (
    id int(11) ,
    roleid int(11) ,
    addtime int(10) unsigned ,
    addby char(32) ) engine=MyISAM;


-- 案件数据库
drop table if exists ncccasectl;
create table ncccasectl (
    cfrom    int   unsigned,         -- 来源
    ctype    int   unsigned,         -- 类别
        sid      int   unsigned,         -- Sid
        cid      int   unsigned,         -- 案件序列号
        context  char(255),              -- 内容, 加密保存
        lasttime int   unsigned          -- 最后修改时间
) engine=MyISAM;

create unique index ncccasectl_ind1 on ncccasectl(cfrom,ctype,sid);
create index ncccasectl_ind2 on ncccasectl(cfrom,cid);


-- 网络服务定义
drop table if exists ncipindex;
create table ncipindex (
    id         int   auto_increment primary key,     -- 服务ID, ID小于1000的数字为内部使用
    name       char(64),             -- 服务名称
    flags      int  default 0
) AUTO_INCREMENT=20000000 engine=MyISAM;

-- 网络服务定义(内容)
drop table if exists nciplist;
create table nciplist (
    sid        int   primary key,      -- 序列号
    id         int,                    -- 服务ID
    prot       int,                    -- 协议   6-TCP  17-UDP
    lip        int unsigned,           -- 起始IP地址
    uip        int unsigned,           -- 终止IP地址
    lport      int unsigned,           -- 起始端口
    uport      int unsigned,           -- 终止端口
    name       char(64),               -- 说明
    flags      int default 0           
) engine=MyISAM;

create index nciplist_index_1 on nciplist(id);
create index nciplist_index_2 on nciplist(prot);

-- 网络服务策略定义
drop table if exists ncippol;
create table ncippol (
    pid      int,
    tid      int,
    vflags   int,             -- 9--None 1--Y  0--X 
    warnid   int unsigned,    -- 告警ID
    flags    int default 0
) engine=MyISAM;
create index ncippol_index_1 on ncippol(pid);

drop table  if exists nclimdate;
create table nclimdate (
    sid        int auto_increment  primary key,  -- 纪录的序列号  
    id         int,                              -- 日期ID  
    ltype      int,                              -- 类型 1--Day 2--Week 3--Month 4--Year      
    llower     int unsigned,                     -- 时间下限 1-YYYYMMDD 2--0/6 3--1/31 4--MMDD 
    lupper     int unsigned
) engine=MyISAM;

create index nclimdate_index1 on nclimdate(id);


--  时间描述  

drop table if exists nclimdatedes;
create table nclimdatedes (
    id        int  primary key,         -- 日期
    name      char(32)                  -- 日期名称
    ) engine=MyISAM;
 

-- 自动提醒功能
drop table  if exists nclimwarnnotice;
create table nclimwarnnotice (
    id         int  primary key,                -- 告警ID
    name       char(32),                        -- 管理员
    email      char(64),
    mobphone   char(32),
    typeid     int,
    mesgnote   int,                             -- 消息提醒 0--不    1--是
    mailnote   int,                             -- 邮件提醒 0--不　　1--是
    mobile     int                              -- 手机提醒 0--不　　1--是
) engine=MyISAM;


-- 告警类别
drop table  if exists nclimwarntype;
create table nclimwarntype (
    id         int auto_increment  primary key,  -- 记录ID
    name       char(32),                         -- 名称
    localwanr  int,                              -- 本地告警 0-不告警  1--告警
    admcenter  int                               -- 管理中心告警
)AUTO_INCREMENT=20000000 engine=MyISAM;


-- 邮件地址内容
drop table  if exists ncmailaddr;
create table ncmailaddr (
    sid        int      auto_increment  primary key,
    id         int,                     -- 邮件地址类别
    mail       char(64),                -- Mail地址
    name       char(64),                -- 说明
    modtime    int unsigned,            -- 修改时间
    flags      int default 1            -- 来源
) engine=MyISAM;

create unique index ncmailaddr_index_1 on ncmailaddr(mail,flags);
create index cmailadd_index_2 on ncmailaddr(id);


-- 邮件地址列表
drop table  if exists ncmailclass;
create table ncmailclass (
    id         int       auto_increment primary key,
    name       char(32),                 -- 类别名称                      
    modtime    int unsigned,             -- 修改时间                      
    status     int  default 1,           -- 使用状态   0--暂停  1--启用
    flags      int  default 1            -- 1--自定义  9--系统  8--中心
)AUTO_INCREMENT=20000000 engine=MyISAM;

create unique index ncmailclass_index_1 on ncmailclass(flags,name);
insert into ncmailclass(id,name,modtime,status,flags) values (1,'其它',0,1,0);

drop table if exists ncmessage;
create   table ncmessage (
    sid     int unsigned auto_increment primary key,
    message_code       char(15) ,              -- 消息编号
    service_code  text,                        -- 服务场所编码
    send_time  int unsigned default 0,         -- 发送时间
    message_title  char(100),                  -- 消息标题
    message_content text,                      -- 消息内容
    flags     int  default   2,                -- 消息方向 0-管理中心发送/回复的消息 1-场所端发送/回复的消息 2-待发送
    qs_flag   int default    0                 -- 签收状态 0-未签收 1-已签收
) engine=MyISAM;

create index ncmessage_ind1 on ncmessage (send_time);


/* 消息分发日志  */
drop table if exists ncmessagelog;
create   table ncmessagelog (
    sid     int unsigned auto_increment primary key,
    sip     int unsigned,                      -- 客户端IP地址
    message_code       char(15) ,              -- 消息编号
    service_code       char(16),               -- 服务场所编码
    send_time          int unsigned default 0,     -- 发送时间
    message_title      char(100),                  -- 消息标题
    flags              int default 0               -- 0 管理中心发送  1 场所端发送
) engine=MyISAM;

create index ncmessagelog_ind1 on ncmessagelog (send_time);
create index ncmessagelog_ind2 on ncmessagelog (message_code);



drop table if exists ncsadminlog;
create table ncsadminlog (
    sid        int  auto_increment   primary key,
    sdate   char(20),               --  时间      
    name    char(16),               --  用户名   
    ipaddr  char(16),               --  IP地址  
    action  char(4) default '99',   --  动作代码 01 鉴别与登录 02 修改鉴别记录 03 修改系统配置 04 修改下发审计策略 05 增加修改删除用户 06 统计各类数据 07 数据备份与恢复 09 系统启动与关闭 99 其它
    result  char(10),               --  结果 
    descr   text                    --  操作内容  
) engine=MyISAM;

create index ncsadminlog_index1 on ncsadminlog (sdate);



drop table  if exists ncsbackuplog;
create table ncsbackuplog (
    sid        int      auto_increment  primary key,
    backtime      int unsigned,          -- 备份时间
    sdate         char(12),              -- 起始日期 yyyy-mm-dd 
    bakpath       char(128),             -- 路径               
    filename      char(128),             -- 文件名               
    fsize         bigint,                -- 文件大小        
    backtype      char(8),
    lcount        int default 0,         -- 邮件数量           
    flags         int default 0          -- 0--正常  1--已异地备份 
) engine=MyISAM;



create index ncsbackuplog_i on ncsbackuplog (sdate);

-- 网站社区
drop table if exists ncsbbsindex;
create table ncsbbsindex (
    id          int  unsigned   primary key,
    name        char(32),
    lasttime    int unsigned default 0,
    ntype       int  default 0,            -- 1-服务  0-网站 
    flags       int  default 1
) engine=MyISAM;


-- 详细描述
drop table if exists ncsbbsvar;
create table ncsbbsvar (
    sid          int  unsigned   auto_increment  primary key,
    tid          int  unsigned,                              
    host         char(64),
    url          char(64),
    varname      char(32)
) engine=MyISAM;


-- 布控记录
drop table if exists ncscasectllog;
create table ncscasectllog (
        sid      int   unsigned auto_increment primary key,
        cid      int   unsigned,        -- 案件序列号 
        sessid   int   unsigned,        --  下载标识   
        stime    int   unsigned,        -- 日期 
        cname    char(64),              -- 案件名称 
        did      int   unsigned,        -- 单位ID 
        flags    int  default 0         -- 0--未布控 1-成功   9--撤控
) engine=MyISAM;

create index  ncscasectllog_ind1 on ncscasectllog (cid);
create unique index ncscasectllog_ind2 on ncscasectllog (cid,did);
create index ncscasectllog_ind3 on ncscasectllog (sessid);


-- 聊天内容
drop table if exists ncscaseim;
create table ncscaseim (
        sid      int   unsigned auto_increment primary key,
        cid      int   unsigned,         -- 案件序列号
        nettype  int   unsigned,         -- 网络服务
        cont     char(128),              -- 表单内容
        contid   int   unsigned,         -- 敏感ID
        flags    int  default 0,         
        lasttime int  unsigned
) engine=MyISAM;
create index ncscaseim_ind1 on ncscaseim (cid);

-- 敏感服务
drop table if exists ncscaseservice;
create table ncscaseservice (
        sid      int   unsigned auto_increment primary key,
        cid      int   unsigned,         -- 案件序列号
        nettype  int   unsigned,         -- 网络服务
        flags    int  default 0,         
        lasttime int  unsigned
) engine=MyISAM;
create index ncscaseservice_ind1 on ncscaseim (cid);


-- 案件信息 
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
        

create unique index ncscase_ind1 on ncscaseinfo (casecode);
create index ncscase_ind2 on ncscaseinfo (groupid);

-- IP地址
drop table if exists ncscaseip;
create table ncscaseip (
    sid      int   unsigned auto_increment primary key,
    cid      int   unsigned,         -- 案件序列号
    slip     int   unsigned,         -- 源起始IP
    suip     int   unsigned,         -- 源终止IP
    dlip     int   unsigned,         -- 目标起始IP
    duip     int   unsigned,         -- 目标终止IP
    prot     int,                    -- 协议 0-全部  6-TCP  17-UDP
    dlport   int,                    -- 目标起始端口
    duport   int,                    -- 目标终止端口
    service  int,                    -- 网络服务
    flags    int  default 0,         -- 0-启用    1--删除   8-删除
    lasttime int  unsigned
) engine=MyISAM;
    
create index ncscaseip_ind1 on ncscaseip (cid);

-- 邮件内容
drop table if exists ncscasemail;
create table ncscasemail (
        sid      int   unsigned auto_increment primary key,
        cid      int   unsigned,         -- 案件序列号
        ctype    int default 0,          -- 0--全部 1-发送 2-收件 (mm)
        mfrom    char(64),               -- 发件人
        mto      char(64),               -- 收件人  
        subj     char(128),              -- 主题
        cont     char(128),              -- 内容
        contid   int  unsigned,          -- 内容ID
        flags    int  default 0,         
        lasttime int  unsigned
) engine=MyISAM;
    
create index ncscasemail_ind1 on ncscasemail (cid);

/* 布控告警  */

drop  table if exists ncscasemesg;
create   table ncscasemesg (
    sid    int   unsigned auto_increment primary key,
    stime  int unsigned,       -- 时间
    did    int unsigned,       -- 单位ID
    uid    int unsigned,       -- 用户ID
    udisp  char(32),           -- 用户名
    sip    int unsigned,       -- IP地址
    dip    int unsigned default 0,  -- 目标IP地址
    position char(64),         -- 上网位置
    cid    int unsigned,       -- 案件ID
    message  char(255),        -- 消息内容
    flags  int default 0,       --     0 待处理 1-有害信息 2-显示信息 9-丢弃
    opname char(32),            -- 操作人员
    optime int default 0,       -- 操作时间
    lsid   int(10) unsigned   
    ) engine=MyISAM;
    
create index ncscasemesg_ind1 on ncscasemesg(cid);
create index ncscasemesg_ind2 on ncscasemesg(stime);

-- 网络虚拟身份
drop table if exists ncscasenetid;
create table ncscasenetid (
    sid      int   unsigned auto_increment primary key,
    cid      int   unsigned,         -- 案件序列号
    nettype  int,                    -- 网络类别
    name     char(64),               -- 名称
    flags    int  default 0,         
    lasttime int  unsigned
) engine=MyISAM;
    
create index ncscasenetid_ind1 on ncscasenetid (cid);

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

create index ncscaseobj_ind1 on ncscaseobj (cid);

drop table if exists ncscasepost;
create table ncscasepost (
        sid      int   unsigned auto_increment primary key,
        cid      int   unsigned,         -- 案件序列号
        url      char(128),              -- 网址
        urlid    int   unsigned,         -- 网址类别
        cont     char(128),              -- 表单内容
        contid   int   unsigned,         -- 敏感ID
    flags    int  default 0,         
    lasttime int  unsigned
) engine=MyISAM;
    
create index ncscasepost_ind1 on ncscasepost (cid);

-- 使用者特征 2006/11/23 Update
drop table if exists ncscaseuser;
create table ncscaseuser (
    sid      int   unsigned auto_increment primary key,
    cid      int   unsigned,         -- 案件序列号
    ctype    int   default 1,        -- 信息类别  1--用户名 2-显示名  3-计算机名  4-MAC地址  5-证件
    cvalue   char(32),               -- 内容
    flags    int  default 0,         -- 1-停用  0-正在使用  8-删除    
    lasttime int  unsigned           -- 最后操作时间
) engine=MyISAM;

create index ncscaseuser_ind1 on ncscaseuser (cid);

-- 告警设置
drop table if exists ncscasewarn;
create table ncscasewarn (
        sid      int   unsigned auto_increment primary key,
        cid      int   unsigned,         -- 案件序列号
        uid      int   unsigned,         -- 操作者ID
        name      char(32),
        wtype    int,                    -- 告警方式 1-邮件 2-短信
        waddr    char(64),               -- 地址
        flags    int  default 0,         
        lasttime int  unsigned
) engine=MyISAM;
    
create index ncscasewarn_ind1 on ncscasewarn (cid);

-- Http
drop table if exists ncscaseweb;
create table ncscaseweb (
    sid      int   unsigned auto_increment primary key,
    cid      int   unsigned,         -- 案件序列号
    url      char(128),              -- 网址
    urlid    int   unsigned,         -- 网址类别
    flags    int  default 0,         
    lasttime int  unsigned
) engine=MyISAM;
    
create index ncscaseweb_ind1 on ncscaseweb (cid);

-- 客户端操作日志
drop table  if exists ncscltlog;
create table ncscltlog (
    sid        int  auto_increment  primary key,  -- 序列号
    sdate      char(20),                          -- 操作时间
    userid     int  unsigned,                     -- 客户端ID
    ip         int  unsigned,                     -- 客户端IP地址
    optid      int,                               -- 操作内容  1:登录  2:离线
    message    char(128)                          -- 描述    
) engine=MyISAM;

create index ncscltlog_ind1 on ncscltlog(sdate);
create index ncscltlog_ind2 on ncscltlog(userid,sdate);

-- 部门名称
drop table if exists ncsendgroup;
create table ncsendgroup (
    sid         int  unsigned  auto_increment  primary key,   --  用户序列号，在整个系统中唯一            
    did         int  unsigned,               --  单位ID
    gid         int  unsigned,               --  部门ID
    gname       char(16),                    --  部门名称
    dispname    char(32),                    --  显示名
    lasttime    int  unsigned                --  最后操作时间
) engine=MyISAM;

create unique index ncsenduser_i1 on ncsendgroup (did,gid);

-- 用户信息库
-- 用户信息库 (新增position,flag)   username --- 16==>32      增加 passwd
-- alter table ncsenduser modify username char(32) default ' ';
drop table if exists ncsenduser;
create table ncsenduser (
    sid         int  unsigned auto_increment  primary key,   --  用户序列号，在整个系统中唯一            
    did         int  unsigned,               --  单位ID
    uid         int  unsigned,               --  用户ID
    gid         int  unsigned,               --  部门ID
    username    char(32),                    --  用户名
    dispname    char(32),                    --  显示名
    namepy      char(64) default ' ',        --  名字的拼音
    ip          int  unsigned,               --  邮件地址
    mac         char(18) default ' ',        --  mac地址
    compname    char(32) default ' ',        --  计算机名
    email       char(64) default ' ',        --  邮件地址
    idcard      char(20) default ' ',        --  身份证号码
    position    char(32) default ' ',        --  房间号
    usebase     int  default 0,              --  用户管理方式
    syntime     int  unsigned default 0,     --  信息更新时间
    lasttime    int  unsigned default 0,     --  最后操作时间
    flag        int  unsigned default 0,     --  flag=1 重点监控对象
    endname     char(16) default ' ',        --  上网终端名称
    country     char(3)  default '',         --  国家代码
    idfytype    char(3)  default ' ',        --  证件类型
    idfyunit    char(30) default ' ',        --  发证单位
    sex         char(1)  default ''          --  性别
) engine=MyISAM;

create unique index ncsenduser_i1 on ncsenduser (did,uid);
create index ncsenduser_i2 on ncsenduser (flag);

-- - 用户实名资料  2013/05/22 新增
drop table if exists ncsuserinfo;
create table   ncsuserinfo (
    username    char(32) primary key,        --  用户名
    dispname    char(32) default 0,          --  显示名
    position    char(32) default ' ',        --  房间号
    passwd      char(32) default ' ',        --  密码
    ip          int  unsigned,               --  IP地址
    mac         char(18) default ' ',        --  mac地址
    idcard      char(20) default ' ',        --  身份证号码
    syntime     int  unsigned default 0,     --  信息更新时间
    idfytype        char(3)  default ' ',    --  证件类型
    idfyunit        char(64) default ' ',    -- 发证单位
    country         char(3)  default ' ',    -- 国籍代码
    did              int  unsigned,               --  单位ID
    sex         char(1)  default '',         --  性别
    lasttime    int  unsigned default 0      --  最后操作时间
)  engine=MyISAM;

create index ncsuserinfo_i1 on ncsuserinfo (syntime);

-- 序列号信息
drop table  if exists ncsequen;
create table ncsequen (
    id     int unsigned
) engine=MyISAM;
insert into ncsequen (id) values (10001);
-- 阻断访问日志
drop     table if exists ncserrlog;
create   table ncserrlog (
    sid       int unsigned auto_increment  primary key,
    did       int unsigned,        --  单位ID
    uid       int unsigned,        --  用户ID
    tid       int unsigned,        --  单位类别
    udisp     char(32),            --  用户名
    sdate     char(20),            -- 时间  
    edate     int unsigned default 0,        -- 结束时间       
    sip       int unsigned,        -- 源IP地址
    dip       int unsigned,        -- 目标IP地址
    dport     int unsigned,        -- 目标端口  
    prot      smallint,            -- 协议 6 TCP   17 UDP
    level     int,                 -- 告警级别 1--阻断  2--告警
    lcount    int,                 -- 计数       
    service   int,                 -- 告警类别  
    csid      int unsigned default 0,        -- 对应场所端的ID
    rule_id   int unsigned default 0,        -- 对应的规则ID
    action    int default 0,                 -- 1:阻断告警  2:允许告警  3:禁用    
    warnid    int unsigned default 0,        -- 告警类型
    mesg      char(96)             -- 内容      
)  engine=MyISAM;

create index ncserrlog_ind1 on ncserrlog (did,uid);
create index ncserrlog_ind2 on ncserrlog (sdate);
create index ncserrlog_ind3 on ncserrlog (did,csid);

-- 网页表单
drop table  if exists ncsformmon;
create table ncsformmon (
    sid        int unsigned auto_increment  primary key,
    did       int unsigned,        --  单位ID
    uid       int unsigned,        --  用户ID
    tid       int unsigned,        --  单位类别
    udisp     char(32),            --  用户名
    sip       int unsigned default 0,        --  IP地址
    sdate      char(20),
    host       char(64),           -- 主机      
    url        char(64),           --  url          
    upfile     char(64),           --  文件名          
    asize      bigint,             --  附件            
    msize      bigint,             --  邮件大小 大于   
    usid       int unsigned default 0,
    ccode      int  default 0,
    dip        int(10)
) engine=MyISAM;

create index ncsformmon_ind1 on ncsformmon (did,uid);
create index ncsformmon_ind2 on ncsformmon (sdate);
create index ncsformmon_ind3 on ncsformmon (usid);
create index ncsformmon_ind4 on ncsformmon (host);
create index ncsformmon_ind5 on ncsformmon (sip);
create index ncsformmon_ind6 on ncsformmon (dip);

-- 组信息--对应于区县
drop table if exists ncsgroup;
create table ncsgroup (
    groupid    int    primary key,          -- 组ID
    groupname  char(32),                    -- 组名
    groupcode  char(10) default '',         -- 行政区代码
    descr      char(128),                   -- 说明
    pid        int   default 0,             -- 上级组
    upip  char(128)   default '',           -- 上传日志服务器IP 供浩易用
    upuser char(32)   default '',           -- 上传日志登录用户名
    uppasswd char(32) default ''
) engine=MyISAM;

create UNIQUE index ncsgroup_index on ncsgroup (groupname,pid);


-- Web上网日志
drop table if exists ncshttplog;
create table ncshttplog (
    did         int  unsigned,               -- 单位ID
    uid         int  unsigned,               -- 用户Id     
    tid         int unsigned,        --  单位类别
    udisp       char(32),            --  用户名
    sdate       char(20),                    -- 日期, 格式为 yyyy/mm/dd hh:mm:ss
    conntime    int unsigned,
    sip         int unsigned,                -- 源IP  
    dip         int unsigned default 0,      -- 目标IP地址
    dport       int unsigned default 0,      -- 目标端口
    host        varchar(64),                 -- 主机名称 
    uri         varchar(128),                -- Uri    
    title       varchar(128) default ' ',    -- 标题           2005/01/07 新增
    urlid       int unsigned default 0,      -- 网址库类别     2005/01/11 新增
    sid         int unsigned default 0,      -- 对应网址说明
    usid        int unsigned default 0,
    bytes       bigint,                      -- 字节数 
    flags       smallint                     -- 记录处理情况 0--新 1--统计 
) engine=MyISAM;

create index ncshttplog_ind1 on ncshttplog (did,uid);
create index  ncshttplog_ind2 on ncshttplog (sdate);
create index  ncshttplog_ind3 on ncshttplog (usid);
create index  ncshttplog_ind4 on ncshttplog (sip);
create index  ncshttplog_ind5 on ncshttplog (dip);
create index  ncshttplog_ind6 on ncshttplog (host);
create index  ncshttplog_ind7 on ncshttplog (urlid);

-- 即时通信内容
drop     table if exists ncsimlog;
create   table ncsimlog (
    sid       int unsigned auto_increment  primary key,
    did       int unsigned,        --  单位ID
    uid       int unsigned,        --  用户ID
    tid       int unsigned,        --  单位类别
    udisp     char(32),            --  用户名
    sip       int unsigned default 0, --  IP地址
    sdate     char(20),            --  日期, 格式　yyyy/mm/dd hh:mm:ss
    service   int,                 --  服务类型
    funcode   int,                 --  服务代码 
    num       int,                 --  序号
    mfrom     char(48),            --  发件人
    mto       char(48),            --  收件人
    mesg      varchar(255),         --  内容
    contid    int unsigned default 0, --   --- 文件ID
    ftid      int unsigned default 0,  --   --- 聊天ID
    usid      int unsigned default 0,
    dip       int(10) unsigned 
) engine=MyISAM;

create index ncsimlog_ind1 on ncsimlog (did,uid);
create index ncsimlog_ind2 on ncsimlog (sdate);
create index ncsimlog_ind3 on ncsimlog (mfrom);
create index ncsimlog_ind4 on ncsimlog (sip);


create index ncsimlog_ind5 on ncsimlog (usid);
create index ncsimlog_ind7 on ncsimlog (mto);


-- IP访问日志
drop table if exists ncsiplog;
create table ncsiplog (
    did         int  unsigned,               --  单位ID
    uid         int,                         --  用户ID
    tid         int unsigned,                --  单位类别
    udisp       char(32),                    --  用户名
    sdate       char(20),                    --  日期，格式为 yyyy/mm/dd hh:mm:ss
    conntime    int,                         --  连接时间  
    sip         int  unsigned,               --  源IP   
    dip         int  unsigned,               --  目标IP 
    trantype    smallint,                    --  协议 6:tcp  17:Udp
    port        int,                         --  端口号     
    service     smallint,                    --  服务id   
    ubytes      bigint,                      --  上行字节数 
    usid        int  unsigned default 0,
    dbytes      bigint,                      --  下行字节数 
    flags       smallint                     --  记录标志，内部处理用
) engine=MyISAM;
create index ncsiplog_ind1 on ncsiplog (did,uid,sdate);
create index ncsiplog_ind2 on ncsiplog (sdate);
create index ncsiplog_ind3 on ncsiplog (sip);
create index ncsiplog_ind4 on ncsiplog (dip);
create index ncsiplog_ind5 on ncsiplog (usid);
create index ncsiplog_ind6 on ncsiplog (service);



-- IP服务统计信息 add 2010/12/28 本表每天统计一次，供管理中心查询用

drop table if exists ncsipservice;
create table ncsipservice (
    did         int  unsigned,               --  单位ID
    uid         int,                         --  用户ID
    tid         int unsigned,                --  单位类别
    sdate       char(10),                    --  日期，格式为 yyyy/mm/dd
    conntime    int,                         --  连接时间  
    service     smallint unsigned,           --  服务id   
    worktime    smallint default 1,          --  时间 1--上班时间  2--下班时间
    ubytes      bigint,                      --  上行字节数 
    dbytes      bigint,                      --  下行字节数 
    flags       smallint                     --  记录标志，内部处理用
) engine=MyISAM;

create index ncsipser_ind1 on ncsipservice (did,uid,sdate);
create index ncsipser_ind2 on ncsipservice (sdate);


drop table if exists ncshttpclass;
create table ncshttpclass (
    did         int  unsigned,               --  单位ID
    uid         int,                         --  用户ID
    tid         int unsigned,                --  单位类别
    sdate       char(10),                    --  日期，格式为 yyyy/mm/dd
    cid         int unsigned,                --   网址类别ID  
    bytes       bigint,                      --   流量字节数 
    worktime    smallint default 1,          --  时间 1--上班时间  2--下班时间
    lcount      bigint,                      --  点击数 
    ctime       int                          --  连接时间
) engine=MyISAM;

create index ncshttpclass_ind1 on ncshttpclass (did,uid,sdate);
create index ncshttpclass_ind2 on ncshttpclass (sdate);


-- --- 搜索引擎记录
drop table if exists ncssearchlog;
create table ncssearchlog (
    did         int  unsigned,               --  单位ID
    uid         int,                         --  用户ID
    tid         int unsigned,                --  单位类别
    udisp       char(32),                    --  用户名
    sdate       char(20),                    --  日期，格式为 yyyy/mm/dd hh:mm:ss
    sip         int  unsigned,           -- 源地址
    mac         varchar(18),
    usid       int  unsigned default 0,
    sname      char(16),                -- 搜索引擎名称
    svalue     varchar(128)             -- 内容
)  max_rows=2000000000 avg_row_length= 1000 engine=MyISAM;

create index ncssearchlog_ind1 on ncssearchlog (did,uid,sdate);
create index ncssearchlog_ind2 on ncssearchlog (sdate);



-- 服务策略定义 (ID范围 20000001 -- 29999999) 
drop table  if exists ncsippoltype;
create table ncsippoltype (
    pid       int   auto_increment primary key,
    name      char(64),            -- 名称
    vister    int   default 9,     -- 缺省访问  9--未设置 1--允许  0--禁止 
    warnid    int  unsigned,       -- 告警ID, 0--不告警
    flags     int   default 0
) AUTO_INCREMENT=20000000 engine=MyISAM;

-- 敏感关键字内容
drop table if exists ncskeycont;
create table ncskeycont (
    sid int(10) unsigned  NOT NULL  auto_increment primary key,
    tid int(10) unsigned ,                -- 类别
    content char(64) ,                    -- 内容
    ktype int(11)  DEFAULT '0',           -- 匹配类别  0-文本  1-正则表达式  2-扩展正则表达式
    caseflags int(11)  DEFAULT '0') engine=MyISAM;      -- 大小写是否敏感  0-不敏感  1-敏感


-- 敏感关键字类别
drop table if exists ncskeyindex;
create table ncskeyindex (
    id int(10) unsigned  NOT NULL  DEFAULT '0',    -- 类别ID
    name char(32) ,                                -- 名称
    lasttime int(10) unsigned ,                    -- 最后操作时间
    flags int(11)  DEFAULT '1',                    -- 状态
    PRIMARY KEY(id)) engine=MyISAM;

drop table if exists ncslastinfo;
create table ncslastinfo (
    name char(16)  NOT NULL  DEFAULT '',
    lasttime int(10) unsigned ,
    PRIMARY KEY(name)) engine=MyISAM;

-- 表单发送

-- 访问控制 网络访问(访问外网、服务、服务策略、IP地址限制)
drop table  if exists  ncslimip;
create table ncslimip (
    sid         int  auto_increment primary key,
    gtype       int,                              -- 控制级别  1--单位 2--组  3--全部  4--策略
    id          int,                              -- 用户或部门ID
    num         int,                              -- 序号
    lowerip     int unsigned,                     -- 起始IP, 当 conflags = 4时, 为 服务策略ID
    upperip     int unsigned,                     -- 终止IP
    trantype    smallint,                         -- 协议    0--全部  6 TCP   17 UDP
    service     int,                              -- 服务ID 
    lowerport   int,                              -- 起始端口
    upperport   int,                              -- 终止端口
    timetype    int,                              -- 日期类别 0--每天
    starttime   int,                              -- 起始时间  YYMMDD
    stoptime    int,                              -- 结束时间  YYMMDD
    conflags    smallint,                         -- 1--外网 2--网络服务 3--Ip地址 4--服务策略
    flags       smallint default 0,                         -- 0--禁止 1--允许 3--告警 2--告警阻断 
    warntype    int default 0,                              -- 告警类别 0--不告警 >0 告警类别
    rule_id     char(20)  default ' ',                         -- 策略代码
    rule_name   char(100) default ' ',                        -- 策略名称
    set_time    int unsigned default 0,                     -- 设置日期
    status      int default 1,                    -- 1－启用 2－停用
    validation_time int unsigned default 0,       -- 有效期
    cfflag      smallint default 1                 -- 0,有审计结果 1-无审计结果 
    
) engine=MyISAM;

create index ncslimip_ind1 on ncslimip(gtype,id,num);


-- 网站相关
drop table if exists ncslimweb;
create table ncslimweb (
    sid         int auto_increment primary key,
    gtype       int,                             -- 控制级别  1--单位 2--组  3--全部  4--策略
    id          int,                             -- 用户或部门ID
    num         int,                             -- 序号
    url         varchar(64),                     -- Url
    urlid       int unsigned default 0,          -- URL策略 
    timetype    int,                             -- 时间类型 0--每天 
    starttime   int,                             -- 起始时间  YYMMDD       
    stoptime    int,                             -- 终止端口       
    conflags    smallint,                        -- 5--网址库 
    flags       smallint,                        -- 限制方式  0--禁止使用   1--允许使用
    warntype    int default 0,                             -- 告警类别 0--不告警 >0 告警类别
    rule_id     char(20) default ' ',                         -- 策略代码
    rule_name   char(100) default ' ',                        -- 策略名称
    set_time    int unsigned default 0,                     -- 设置日期
    status      int default 1,                    -- 1－启用 2－停用
    validation_time int unsigned default 0,                 -- 有效期
    cfflag      smallint default 1                 -- 0,有审计结果 1-无审计结果 
) ENGINE=MyISAM;

create index ncslimweb_ind1 on ncslimweb(gtype,id,num);

-- 即时通信
drop table if exists ncslimim;
create table ncslimim (
    sid         int auto_increment primary key,
    gtype       int,                             -- 控制级别  1--单位 2--组  3--全部  4--策略
    id          int,                             -- 用户或部门ID
    num         int,                             -- 序号
    timetype    int,                             -- 时间类型 0--每天 
    starttime   int,                             -- 起始时间  YYMMDD       
    stoptime    int,                             -- 终止端口       
    service     int,                             -- 服务类型  见ncipindex中的即时通信
    fun         int,                             -- 动作
    uid         char(64),                        -- 用户ID
    uidlist     int unsigned,                    -- 用户ID列表
    content     char(255),                       -- 内容
    flags       smallint,                        -- 限制方式  0--禁止使用   1--允许使用
    warntype    int default 0,                              -- 告警类别 0--不告警 >0 告警类别
    rule_id     char(20) default ' ',                         -- 策略代码
    rule_name   char(100) default ' ',                        -- 策略名称
    set_time    int unsigned default 0,                     -- 设置日期
    status      int default 1,                    -- 1－启用 2－停用
    validation_time int unsigned default 0,                  -- 有效期
    cfflag      smallint default 1                 -- 0,有审计结果 1-无审计结果 
) ENGINE=MyISAM;

create index ncslimim_ind1 on ncslimim(gtype,id,num);


-- 表单发送
drop table if exists ncslimform;
create table ncslimform (
    sid         int auto_increment primary key,
    gtype       int,                             -- 控制级别  1--单位 2--组  3--全部  4--策略
    id          int,                             -- 用户或部门ID
    num         int,                             -- 序号
    timetype    int,                             -- 时间类型 0--每天 
    starttime   int,                             -- 起始时间  YYMMDD       
    stoptime    int,                             -- 终止端口       
    url         char(64),                        -- URL
    urllist     int unsigned,                    -- URL库的ID
    varname     char(255),                       -- 变量名
    content     char(255),                       -- 内容
    attname     char(255),                       -- 附件名称
    flags       smallint,                        -- 限制方式  0--禁止使用   1--允许使用
    warntype    int default 0,                              -- 告警类别 0--不告警 >0 告警类别
    rule_id     char(20) default ' ',                         -- 策略代码
    rule_name   char(100) default ' ',                        -- 策略名称
    set_time    int unsigned default 0,                     -- 设置日期
    status      int default 1,                    -- 1－启用 2－停用
    validation_time int unsigned default 0,                  -- 有效期
    cfflag      smallint default 1                 -- 0,有审计结果 1-无审计结果 
) ENGINE=MyISAM;
create index ncslimform_ind1 on ncslimform(gtype,id,num);


-- 邮件发送
drop table if exists ncslimmail;
create table ncslimmail (
    sid         int auto_increment primary key,
    gtype       int,                             -- 控制级别  1--单位 2--组  3--全部  4--策略
    id          int,                             -- 用户或部门ID
    num         int,                             -- 序号
    timetype    int,                             -- 时间类型 0--每天 
    starttime   int,                             -- 起始时间  YYMMDD       
    stoptime    int,                             -- 终止端口       
    mfrom       char(64),                        -- 发件人
    mfromlist   int unsigned,                    -- 发件人列表
    mto         char(64),                        -- 收件人
    mtolist     int unsigned,                    -- 收件人列表
    subject     char(128),                       -- 标题
    content     char(255),                       -- 内容
    attname     char(255),                       -- 附件
    msize       int unsigned,                    -- 邮件大小
    flags       smallint,                        -- 限制方式  0--禁止使用   1--允许使用
    warntype    int default 0,                              -- 告警类别 0--不告警 >0 告警类别
    rule_id     char(20) default ' ',                         -- 策略代码
    rule_name   char(100) default ' ',                        -- 策略名称
    set_time    int unsigned default 0,                     -- 设置日期
    status      int default 1,                    -- 1－启用 2－停用
    validation_time int unsigned default 0,                  -- 有效期
    cfflag      smallint default 1                 -- 0,有审计结果 1-无审计结果 
) ENGINE=MyISAM;

create index ncslimmail_ind1 on ncslimmail(gtype,id,num);

-- 虚拟身份
drop table if exists ncslimvid;
create table ncslimvid (
    sid         int auto_increment primary key,
    gtype       int,                             -- 控制级别  1--单位 2--组  3--全部  4--策略
    id          int,                             -- 用户或部门ID
    num         int,                             -- 序号
    timetype    int,                             -- 时间类型 0--每天 
    starttime   int,                             -- 起始时间  YYMMDD       
    stoptime    int,                             -- 终止时间
    service     int,                             -- 服务类型 
    fun         int,                             -- 动作 
    uid         char(64),                        -- 用户ID
    uidlist     int unsigned,                    -- 用户ID列表
    flags       smallint,                        -- 限制方式  0--禁止使用   1--允许使用
    warntype    int default 0,                             -- 告警类别 0--不告警 >0 告警类别
    rule_id     char(20) default ' ',                        -- 策略代码
    rule_name   char(100) default ' ',                       -- 策略名称
    set_time    int unsigned default 0,                    -- 设置日期
    status      int default 1,                   -- 状态 1 启用  0 停用
    validation_time int unsigned default 0,                 -- 有效期
    cfflag      smallint default 1                 -- 0,有审计结果 1-无审计结果 
) ENGINE=MyISAM;

create index ncslimvid_ind1 on ncslimvid(gtype,id,num);


-- 邮件收发日志
drop table if exists ncsmailmon;
create table ncsmailmon (
    sid        int unsigned    auto_increment primary key, -- 记录序列号
    did        int  unsigned,               -- 单位ID
    uid        int  unsigned,               -- 用户Id     
    tid       int unsigned,        --  单位类别
    udisp     char(32),            --  用户名
    sip        int  unsigned default 0,     -- IP地址 (不用)
    sdate      char(20),                    -- 日期，格式　yyyy/mm/dd hh:mm:ss
    mailfile   char(64),                    -- 邮件文件名  保存相对路径    
    mailfrom   char(128),                   -- 发件人          
    mailto     char(128),                   -- 收件人          
    mailsub    char(128),                   -- 主题            
    mailasize  bigint,                      -- 附件大小
    mailsize   bigint,                      -- 邮件大小大于   
    usid       int  unsigned default 0,
    flags      int,                         -- 邮件状态 1--SMTP  2--POP3  3--WebMail
    host       char(64) default ' ',        -- 主机名 2004/03/04 增加
    ccode      int default 0,                -- 邮件编码
    dip        int  unsigned default 0
) ENGINE=MyISAM;

create index ncsmailmon_ind1 on ncsmailmon (did,uid);
create index ncsmailmon_ind2 on ncsmailmon (sdate);
create index ncsmailmon_ind3 on ncsmailmon (usid);
-- create index ncsmailmon_ind4 on ncsmailmon (sip);
create index ncsmailmon_ind5 on ncsmailmon (mailfrom);
create index ncsmailmon_ind6 on ncsmailmon (mailto);

-- 综合设置
drop table if exists ncsmondefault;
create table ncsmondefault (
    sid         int unsigned  auto_increment primary key,
    ftype       int,                            -- 使用类别  0:实时监控  1:日志  
    uflags     smallint,                        -- 限制类别  0-全部 1--组名  2--用户名
    userid      int,                            -- 单位ID,   若为0表示全部
    mid         int,                            -- 信息ID    0-发送时间 1-IP 2-WEB  3-MAIL 4-IM 5-FORM 6-FTP 7-虚拟身份 8-BBS 9-发送指定时间
    enable      int unsigned default 0,         -- 启用      1--启用      0--不启用  9--未设置, 当mid=0时 1-间隔 2-每日 3-每周 4-
    defsend     int unsigned default 0          -- 缺省      1--发送  0--不发送  9--未设置
) ENGINE=MyISAM;

create index ncsmondefault_ind1 on ncsmondefault(ftype,userid,uflags,mid);

drop table if exists ncsmonform;
create table ncsmonform (
    sid         int unsigned  auto_increment primary key,
    ftype       int,                            -- 使用类别  0:实时监控  1:日志
    uflags     smallint,                        -- 限制类别 0-全部 1--组名  2--用户名
    userid      int,                            -- 单位ID, 若为0表示全部
    num         int,                            -- 序号
    url         char(64),                       -- URL
    urlid       int unsigned,                   -- 网址库中的类别
    varname     char(255),                      -- 变量名
    content     char(255),                      -- 内容
    keyid       int,                            -- 关键字ID
    flags       int                             -- 状态 0-不监控  1-监控
)ENGINE=MyISAM;

create index ncsmonform_ind1 on ncsmonform(ftype,userid,uflags,num);

-- BBS监控 Add 2009-12-07 By Liyunmimng
drop table if exists ncsmonbbs;
create table ncsmonbbs (
    sid         int unsigned  auto_increment primary key,
    ftype       int,                            -- 使用类别  0:实时监控  1:日志
    uflags     smallint,                        -- 限制类别 0-全部 1--组名  2--用户名
    userid      int,                            -- 单位ID, 若为0表示全部
    num         int,                            -- 序号
    url         char(64),                       -- URL
    urlid       int unsigned,                   -- 网址库中的类别
    bbsname     char(64),                       -- 论坛名称
    content     char(64),                       -- 内容
    keyid       int default 0,                  -- 关键字ID
    flags       int                             -- 状态 0-不监控  1-监控
) ENGINE=MyISAM;

create index ncsmonbbs_ind1 on ncsmonbbs(ftype,userid,uflags,num);

-- Telnet和FTP监控

drop table if exists ncsmonftp;
create table ncsmonftp (
    sid         int unsigned  auto_increment primary key,
    ftype       int,                            -- 使用类别  0:实时监控  1:日志
    uflags     smallint,                        -- 限制类别 0-全部 1--组名  2--用户名
    userid      int,                            -- 单位ID, 若为0表示全部
    num         int,                            -- 序号
    service     int,                            -- 服务名称
    sip         int unsigned,                   -- 起始IP地址
    eip         int unsigned,                   -- 终止IP地址
    flags       int                             -- 状态 0-不监控  1-监控
) ENGINE=MyISAM;
create index ncsmonftp_ind1 on ncsmonftp(ftype,userid,uflags,num);

-- 即时通信监控
drop table if exists ncsmonim;
create table ncsmonim (
    sid         int unsigned  auto_increment primary key,
    ftype       int,                            -- 使用类别  0:实时监控  1:日志
    uflags     smallint,                        -- 限制类别 0-全部 1--组名  2--用户名
    userid      int,                            -- 单位ID, 若为0表示全部
    num         int,                            -- 序号
    service     int,                            -- 类型
    uname       char(64),                       -- 使用者ID,可以为QQ号,MSN账号等
    uid         int unsigned,                   -- 用户ID列表
    fun         int,                            -- 操作
    keyid       int default 0,                  -- 关键字ID
    content     char(64),                       -- 正文
    flags       int                             -- 状态 0-不监控  1-监控
) ENGINE=MyISAM;

create index ncsmonim_ind1 on ncsmonim(ftype,userid,uflags,num);

-- 端口和IP地址监控
drop table if exists ncsmonip;
create table ncsmonip (
    sid         int unsigned  auto_increment primary key,
    uflags      smallint,                       -- 限制类别 0-全部 1--组名  2--用户名
    userid      int,                            -- 单位ID, 若为0表示全部
    ftype       int,                            -- 使用类别  0:实时监控  1:日志
    num         int,                            -- 序号
    sip         int unsigned,                   -- 起始IP地址
    eip         int unsigned,                   -- 终止IP地址
    sport       int unsigned,                   -- 起始端口
    dport       int unsigned,                   -- 终止端口
    service     int unsigned,                   -- 网络服务, ID为ncipindex中的id
    prot        int default 0,                  -- 网络协议 0-全部  6-TCP  17-UDP
    flags       int default 0                   -- 状态 0-不发送  1-发送
) ENGINE=MyISAM;

create index ncsmonip_ind1 on ncsmonip(ftype,userid,uflags,num);

-- 邮件监控
drop table if exists ncsmonmail;
create table ncsmonmail (
    sid         int unsigned  auto_increment primary key,
    ftype       int,                            -- 使用类别  0:实时监控  1:日志
    uflags     smallint,                        -- 限制类别 0-全部 1--组名  2--用户名
    userid      int,                            -- 单位ID, 若为0表示全部
    num         int,                            -- 序号
    sender      char(64),                       -- 发件人
    sendid      int unsigned,                   -- 发件人地址列表ID
    toname      char(64),                       -- 收件人
    toid        int unsigned,                   -- 收件人地址列表ID
    subject     char(64),                       -- 主题
    attname     char(64),                       -- 附件
    msize       int unsigned,                   -- 邮件大小
    content     char(64),                       -- 邮件正文
    keyid       int default 0,                  -- 关键字库ID
    flags       int                             -- 状态 0-不监控  1-监控
) ENGINE=MyISAM;

create index ncsmonmail_ind1 on ncsmonmail(ftype,userid,uflags,num);

-- Web监控
drop table if exists ncsmonweb;
create table ncsmonweb (
    sid         int unsigned  auto_increment primary key,
    ftype       int,                            -- 使用类别  0:实时监控  1:日志
    uflags     smallint,                        -- 限制类别 0-全部 1--组名  2--用户名
    userid      int,                            -- 单位ID, 若为0表示全部
    num         int,                            -- 序号
    url         char(64),                       -- 网址
    cond        int,                            -- 条件(0 Include  1 StartWith  2 End  3 等于)
    urlid       int unsigned default 0,         -- 网址库分类ID
    flags       int                             -- 状态 0-不监控  1-监控
) ENGINE=MyISAM;


create index ncsmonweb_ind1 on ncsmonweb(ftype,userid,uflags,num);

-- 网络标识
drop table if exists ncsnetid;
create table ncsnetid (
    sid         int  unsigned  auto_increment primary key,
    nid         int  unsigned  default 0,    --  对应用户ID,和用户信息关联
    ntype       int  unsigned,               --  网络类别(如:qq,MSN,游戏账号等)
    name        char(48),                    --  网络标识(如：qq号，msn号等)
    flags       int default 0,               --  来源 0--自动采集  1--人工输入  2--别处导入
    did         int  unsigned,               --  最后上网场所
    lasttime    int  unsigned,                --  最后操作时间
    lcount      int default 0                 --  使用次数
) ENGINE=MyISAM;

create unique index ncrnetid_ind1 on ncsnetid (name,ntype,did,nid);
create index  ncrnetid_ind2 on ncsnetid(did,nid);
create index  ncrnetid_ind3 on ncsnetid(did);
create index  ncrnetid_ind6 on ncsnetid(ntype,lasttime);
create index  ncrnetid_ind5 on ncsnetid(lasttime);
-- 根据网络虚拟身份日志生成网络虚拟身份库
-- insert into ncsnetuser(uname,udisp,idtype,idcard,sex,bdate,flags,did,lasttime) select  udisp,udisp,0,' ',0,' ',0,did,max(vtime) from ncsnetlog group by udisp,did;
-- insert into ncsnetid(nid,ntype,name,did,lasttime) select a.nid,b.ntype,b.name,b.did,max(b.vtime) from ncsnetuser a,ncsnetlog b where a.did = b.did and a.uname = b.udisp group by b.ntype,b.name,b.did;


-- 网络身份使用记录, 记录使用各种服务情况
drop table if exists ncsnetlog;
create table ncsnetlog (
    sid         int unsigned  auto_increment primary key,
    ntype       int  unsigned,               --  网络类别
    name        char(64),                    --  网络标识(如：qq号，msn号等)
    vtime       int unsigned,
    etime       int unsigned,                --  终止时间
    sdate       char(20),                    --  访问时间
    did         int  unsigned,               --  单位ID
    uid         int  unsigned,               --  用户ID
    tid         int unsigned,                --  单位类别
    udisp       char(32),                    --  用户名
    ip          int  unsigned,               --  IP地址
    netid       int  default 0,              --  网络ID类别
    descr        char(64) default ' ',       --  说明
    fun         int  unsigned,               --  1--登陆 2--退出
    mac         char(18),
    usid        int  unsigned default 0,     --  用户上网ID
    csid        int  unsigned default 0,     --  客户端表中的Sid
    position    char(32) default ' ',        --  上网位置
    flags       smallint default 0 ,          --  记录标志，内部处理用
    dip         int  unsigned default 0     --  目标IP
) max_rows=1000000000 avg_row_length=1500 ENGINE=MyISAM;

create  index ncsnetlog_ind1 on ncsnetlog (ntype,name);
create  index ncsnetlog_ind2 on ncsnetlog (vtime);
create  index ncsnetlog_ind3 on ncsnetlog (did,csid);
create  index ncsnetlog_ind4 on ncsnetlog (name);
create  index ncsnetlog_ind5 on ncsnetlog (did,uid);
create  index ncsnetlog_ind6 on ncsnetlog (ip);
create  index ncsnetlog_ind7 on ncsnetlog (did,vtime);
create  index ncsnetlog_ind8 on ncsnetlog (usid);

-- 网络会话日志
drop table if exists ncsnetseslog;
create table ncsnetseslog (
    did         int  unsigned,               --  单位ID
    uid         int,                         --  用户ID
    tid       int unsigned,        --  单位类别
    udisp     char(32),            --  用户名
    sdate       char(20),                    --  日期，格式为 yyyy/mm/dd hh:mm:ss
    sip         int  unsigned,               --  源IP
    dip         int  unsigned,               --  目标IP
    service     smallint,                    --  服务id 1-telnet 2-ftp
    funcode     int,                      --  功能代码
    usid        int  unsigned default 0,     --  用户上网ID
    mesg        varchar(255)              --  内容
) ENGINE=MyISAM;
create index ncsnetseslog_ind1 on ncsnetseslog (did,uid,sdate);
create index ncsnetseslog_ind2 on ncsnetseslog (sdate);



-- 网络身份类别
drop table if exists ncsnettype;
create table ncsnettype (
    ntype       int  unsigned primary key,   --  身份类别ID
    name        char(32),                    --  名称
    flags       int  default 0,
    descr       char(128),                   --  说明
    gid         int unsigned default 0,      --  20论坛 21微博博客 22交友 23招聘 24购物 25旅游住宿
    lasttime    int unsigned                 --  最后更新时间
) engine=MyISAM;


-- 用户网络虚拟身份

drop table if exists ncsnetuser;
create table ncsnetuser (
    nid         int  unsigned  auto_increment primary key,
    uname       char(32),                    --  用户名（如:上网id)
    udisp       char(32),                    --  姓名
    idtype      int  unsigned default 1,     --  证件类别
    idcard      char(20) default ' ',        --  身份证号码
    sex         int  default 0,              --  性别 1-男  2-女  0-未知
    bdate       char(12),                    --  出生日期
    flags       int  default 0,              --  来源 0--自动采集  1--人工输入  2--别处导入
    did         int  unsigned,               --  最后上网场所
    lasttime    int  unsigned                --  最后操作时间
) ENGINE=MyISAM;

create unique index ncrnetuser_ind2 on ncsnetuser (uname,did);
create index ncrnetuser_ind4 on ncsnetuser (udisp);
create index ncrnetuser_ind1 on ncsnetuser (idtype,idcard);


drop table if exists ncsonlineinfo;       -- 存放管理端在线率信息
create   table ncsonlineinfo (
    id        int unsigned,  -- 0 管理中心 >0 单位ID
    stime     int unsigned,  -- 记录时间
    lcount    int unsigned,   -- 当id为零时为在线单位数,id>0时为在线用户
    lzccount  int unsigned default 0  -- 注册用户数
) ENGINE=MyISAM;
create index ncsonlineinfo_1 on ncsonlineinfo (id);
create index ncsonlineinfo_2 on ncsonlineinfo (stime);

--  用户上网轨迹查询 
--  对异步查询，统一使用数据库进行数据交换
drop table if exists ncssearch;
create table ncssearch (
    sid         int  unsigned   primary key, --  查询ID
    lasttime    int  unsigned,               --  最后一次访问时间
    lcount      int  unsigned,               --  当前记录数
    stype       int,                         --  查询类别
    descr       varchar(255)                 --  条件描述
) ENGINE=MyISAM;

create index ncssearch_ind1 on ncssearch (lasttime);


drop  table if exists ncstat;
create   table ncstat (
    recip     int unsigned default 0,  -- ip记录数
    rechttp     int unsigned default 0,  -- http记录数
    recim    int unsigned default 0,  -- im记录数
    recmail   int unsigned default 0,  -- 邮件记录数
    recform    int unsigned default 0,  -- 表单
    recwarn    int unsigned default 0,  -- 告警
    recerr     int unsigned default 0,  -- 阻断
    recftp     int unsigned default 0,  -- ftp
    recusernet int unsigned default 0, -- 用户上网记录
    recnetid   int unsigned default 0, -- 虚拟身份记录
    stime      int unsigned default 0
) ENGINE=MyISAM;
create index ncstat_ind1 on ncstat (stime);

-- 用户统计信息
drop table if exists ncsuserstat;
create table ncsuserstat (
    did         int  unsigned,               --  单位ID
    uid         int  unsigned,               --  用户ID
    warncount   int  unsigned  default 0,    --  告警次数
    webcount    int  unsigned  default 0,    --  敏感web访问次数
    mailcount   int  unsigned  default 0,    --  敏感邮件次数
    formcount   int  unsigned  default 0,    --  敏感表单次数
    imcount     int  unsigned  default 0     --  敏感聊天内容记录
   
) ENGINE=MyISAM;
create unique index ncsuserstat_i1 on ncsuserstat (did,uid);
-- alter table ncsuser add column longitude  bigint(20)  default 0;           -- 经度
-- alter table ncsuser add column latitude bigint(20) default 0;              -- 纬度
-- alter table ncsuser add column fcode char(10) default ' ';              -- 纬度
-- alter table ncsuser add column dateid int  default 0;              -- 纬度

-- 监控单位信息
drop table if exists ncsuser;
create table ncsuser (
    userid      int   primary key,              -- 单位ID
    username    char(16),                       -- 登录名, 单位编码 服务场所代码
    password    char(32),                       -- 密码
    groupid     int    default 0,               -- 组ID
    dispname    char(64),                       -- 显示名
    address     char(128),                      -- 地址
    telphone    char(64),                       -- 安全人联系电话
    contact     char(32),                       -- 信息安全员
    handphone   char(32),                       -- 法人代表(负责人)电话
    email       char(128) default ' ',          -- 信息安全员E-Mail地址
    ceoname     char(30) default ' ',           -- 法人代表(负责人)
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
    lasttime    char(16),                       -- 最后操作时间
    longitude  bigint(20)  default 0,           -- 经度
    latitude   bigint(20) default 0,            -- 纬度
    monflag     int     default 0,              -- 监控级别 0-正常监控 1-重点监控 2-一般监控
    fcode    char(10)  default '',              -- 厂商代码
    dateid     int default 0,                   -- 营业时间ID
    jytype     int default 1,                   -- 场所经营性质 0--经营 1--非经营 3--其它
    ceoidtype char(4) default ' ',            -- 法人代表有效证件类别
    ceocardid   char(32) default ' ',           -- 法人代表有效证件
    ceotel         char(64)  default '',            --   联系方式
    sdate   char(20)  default '08:00',              --  营业开始时间
    edate   char(20)  default '18:00',              --  营业结束时间
    port_start       char(20),                      --  场所外网起始端口
    port_end         char(20),                       --  场所外网终止端口
    shflag          int default 0,                  -- 0 未审 1 已审
    workflag        int default 0,                  -- 0 正常 1 维护
    yyflag          int default 0                  -- 0 营业 1 停业
) ENGINE=MyISAM;

create unique index ncsuser_ind1 on ncsuser(username);
create index ncsuser_ind2 on ncsuser(groupid);

-- 单位IP地址轨迹
drop table if exists ncsuserip;
create table ncsuserip (
    id         int  unsigned  auto_increment primary key,
    userid      int   ,              -- 单位ID
    lip         int   unsigned,                 -- 原Ip地址
    uip         int   unsigned,                 -- 当前IP地址
    stime       int
) ENGINE=MyISAM;
create index ncsuserip_ind1 on ncsuserip(userid);

-- 最终用户上网记录
drop table if exists ncsuserlog;
create table ncsuserlog (
    sid         int  unsigned auto_increment primary key,
    sdate       char(20),                    --  访问时间
    edate       char(20),                    --  结束时间
    did         int  unsigned,               --  单位ID
    uid         int  unsigned,               --  用户ID
    tid         int  unsigned,               --  单位类别
    udisp       char(32),                    --  用户名
    ip          int  unsigned,               --  IP地址
    mac         char(18),                    --  MAC地址
    position    char(32),                    --  位置
    conntime    int  unsigned,               --  持续时间
    sumflow      bigint,                     --  字节数 
    usid        int  unsigned,               --  上网记录ID
    flags       smallint,                     --  记录标志，内部处理用,
    eip         int(10) unsigned default 0  
) ENGINE=MyISAM;

create index  ncsuserlog_ind1 on ncsuserlog (did,uid);
create index  ncsuserlog_ind2 on ncsuserlog (sdate);
create index  ncsuserlog_ind3 on ncsuserlog (did,usid);
create index  ncsuserlog_ind4 on ncsuserlog (did);
create index  ncsuserlog_ind6 on ncsuserlog (did,sdate);
create index  ncsuserlog_ind7 on ncsuserlog (ip);
-- 用户统计信息
drop table if exists ncsuserstat;
create table ncsuserstat (
    did         int  unsigned,               --  单位ID
    uid         int  unsigned,               --  用户ID
    warncount   int  unsigned  default 0,    --  告警次数
    webcount    int  unsigned  default 0,    --  敏感web访问次数
    mailcount   int  unsigned  default 0,    --  敏感邮件次数
    formcount   int  unsigned  default 0,    --  敏感表单次数
    imcount     int  unsigned  default 0     --  敏感聊天内容记录
   
) ENGINE=MyISAM;
create unique index ncsuserstat_i1 on ncsuserstat (did,uid);

-- 告警记录   
drop     table if exists ncswarnlog;
create   table ncswarnlog (
    sid       int unsigned auto_increment  primary key,
    did       int unsigned,        --  单位ID
    uid       int unsigned,        --  用户ID
    tid       int unsigned,        --  单位类别
    udisp     char(32),            --  用户名
    sdate     char(20),            --  时间         
    warntype  int,                 --  告警级别      
    msgtype   int,                 --  信息类别     
    mid       int unsigned,        --  消息对应的ID  
    mesg      char(128)            --  内容      
) ENGINE=MyISAM;

create index ncswarnlog_ind1 on ncswarnlog (did,uid);
create index ncswarnlog_ind2 on ncswarnlog (sdate);


-- XML文件生成log
drop table if exists ncsxmlfile;
create table ncsxmlfile (
    sid          int  unsigned   auto_increment  primary key,
    filename     char(128),                              
    stime        int unsigned
) ENGINE=MyISAM;

-- 重点监控人员规则
drop table if exists ncszdjkrule;
create table ncszdjkrule (
    stime       char(16) default '23:00:00',               --  每天统计时间
    flags       int  unsigned default 0,   --  是否启用 1-启用 0-不启用
    warncount   int  unsigned  default 0,    --  告警次数
    webcount    int  unsigned  default 0,    --  敏感web访问次数
    mailcount   int  unsigned  default 0,    --  敏感邮件次数
    formcount   int  unsigned  default 0,    --  敏感表单次数
    imcount     int  unsigned  default 0,    --  敏感聊天内容记录
    netidclass  int  unsigned  default 0,    --  虚拟身份类别
    lasttime    int  unsigned default 0,     --  上次统计时间
    starttime   int  unsigned default 0      --  统计起始时间
) ENGINE=MyISAM;


-- 用户ID列表
drop table  if exists ncuseridclass;
create table ncuseridclass (
    id         int    auto_increment    primary key,
    name       char(32),                 -- 类别名称                      
    modtime    int unsigned,             -- 修改时间                      
    status     int  default 1,           -- 使用状态   0--暂停  1--启用
    flags      int  default 1            -- 1--自定义  9--系统  8--中心
)AUTO_INCREMENT=20000000 ENGINE=MyISAM;

create unique index ncuidclass_index_1 on ncuseridclass(flags,name);
insert into ncuseridclass(id,name,modtime,status,flags) values (1,'其它',0,1,0);


-- 重点关注虚拟身份ID列表
drop table  if exists ncuseridlist;
create table ncuseridlist (
    sid        int      auto_increment  primary key,
    id         int,                     -- 用户ID类别
    uname      char(64),                -- 虚拟身份号
    ntype      int default 0,           -- 虚拟身份类别
    descr      char(64),                -- 说明
    modtime    int unsigned,            -- 修改时间
    flags      int default 1            -- 来源
) ENGINE=MyISAM;

create unique index ncuseridlist_ind_1 on ncuseridlist(uname,flags);
create index ncuseridlist_ind_2 on ncuseridlist(id);

-- 网址库类别
drop table  if exists ncwebclass;
create table ncwebclass (
    id         int       auto_increment primary key,  -- 小于 1000为内部使用
    name       char(32),                 -- 类别名称                      
    lcount     int,                      -- 总数(这是个统计数据，仅供参考)                          
    modtime    int unsigned,             -- 修改时间                      
    status     int  default 1,           -- 使用状态   0--暂停  1--启用
    flags      int  default 1            -- 1--自定义  9--系统  8--中心
)AUTO_INCREMENT=20000000 ENGINE=MyISAM;

create unique index ncwebclass_index_1 on ncwebclass(flags,name);
insert into ncwebclass(id,name,lcount,modtime) values (1,'其它',1,0);

-- 网址库内容
drop table  if exists ncweblist;
create table ncweblist (
    sid        int      auto_increment  primary key,
    id         int,                     -- 网址库类别
    url        char(64),                -- URL,前面若有 http
    name       char(64),                -- 说明
    lcount     int unsigned default 0,  -- 网址库访问次数
    modtime    int unsigned,            -- 修改时间
    flags      int default 1
) ENGINE=MyISAM;

create unique index ncweblist_index_1 on ncweblist(url,flags);
create index ncweblist_index_2 on ncweblist(id);
insert into ncweblist(sid,id,url,name,modtime) values (1,1,'Unknow','',0);


-- 网址库策略
drop table  if exists ncwebpolclass;
create table ncwebpolclass (
    pid       int   auto_increment primary key,
    name      char(64),
    vister    int   default 9,    -- 缺省 9--未设置 1--允许  0--禁止  */
    warnid    int    unsigned,    -- 告警ID
    flags     int   default 1
)AUTO_INCREMENT=20000000 ENGINE=MyISAM;

create unique index ncwebpolc_index_1 on ncwebpolclass(flags,name);


drop table  if exists ncwebpollist;
create table ncwebpollist (
    pid      int,
    tid      int unsigned,
    vflags   int,               -- 9--None 1--Y  0--X */
    warnid   int   unsigned,    -- 告警ID
    flags    int default 1
) ENGINE=MyISAM;
create index ncwebpol_ind_1 on ncwebpollist(pid);



drop table if exists punishinfo;          -- 处罚结果信息维护
create table punishinfo (
    punishinfo_id int  unsigned auto_increment primary key,
    service_code  char(14),                --  上网场所代码
    punish_date   int unsigned default 0,  --   处罚日期
    punish_result char(9)      default ' ' --  处罚代码
) ENGINE=MyISAM;



drop table if exists upstatusproperty;      --  存放系统上报信息属性
create   table upstatusproperty (
    callbackurl        char(128) default ' ',  -- 回调url
    verifyid           char(10)  default ' ',  -- 回调授权码
    intervalfig        char(5)   default '60', -- 时间间隔
    commitstatus       char(1)   default 'N',  -- 上报营业场所运行状态 Y/N  Y-启动 N-不启动
    alarmlog           char(1)   default 'N',  -- 报警日志     Y/N
    tudlog             char(1)   default 'N',  -- 终端上下线日志  Y/N
    onlinelog          char(1)   default 'N',  -- 上网日志   Y/N
    realidentity       char(1)   default 'N',  -- 真实身份 Y/N
    punishresult       char(1)   default 'N',  -- 处罚结果 Y/N
    daystat            char(1)   default 'N',  -- 日统计   Y/N
    sendsms            char(1)   default 'N',   -- 代发手机短信通知 Y/N
    sendtrac           char(1)   default 'N',   -- 发送上网轨迹数据 Y/N
    virtualidentity    char(1)   default 'N',   -- 虚拟身份
    virtualtrack       char(1)   default 'N'    -- 虚拟身份轨迹
) ENGINE=MyISAM;

delete from ncsbbsindex where id < 100;
insert into ncsbbsindex(id,name,lasttime,ntype,flags) values (13001,'SMTP邮件',0,1,9);
insert into ncsbbsindex(id,name,lasttime,ntype,flags) values (13002,'POP3邮件',0,1,9);
insert into ncsbbsindex(id,name,lasttime,ntype,flags) values (1001,'QQ',0,1,9);
insert into ncsbbsindex(id,name,lasttime,ntype,flags) values (1006,'雅虎通',0,1,9);
insert into ncsbbsindex(id,name,lasttime,ntype,flags) values (1002,'MSN',0,1,9);
insert into ncsbbsindex(id,name,lasttime,ntype,flags) values (1008,'ICQ',0,1,9);
insert into ncsbbsindex(id,name,lasttime,ntype,flags) values (4002,'FTP',0,1,9);
insert into ncsbbsindex(id,name,lasttime,ntype,flags) values (2001,'联众游戏',0,1,9);
insert into ncsbbsindex(id,name,lasttime,ntype,flags) values (2002,'中国游戏',0,1,9);
insert into ncsbbsindex(id,name,lasttime,ntype,flags) values (2201,'QQ游戏',0,1,9);

insert into ncsbbsindex(id,name,lasttime,ntype,flags) values (10001,'盛大帐号',0,1,9);
insert into ncsbbsindex(id,name,lasttime,ntype,flags) values (10002,'飞信手机号',1,1,9);

insert into ncsbbsindex(id,name,lasttime,ntype,flags) values (1005,'新浪UC',0,1,9);



/* 虚拟账号
delete from ncsnettype;
insert into ncsnettype (ntype,name,flags,descr,lasttime) values (2001,'联众游戏',0,'联众游戏',0);
insert into ncsnettype (ntype,name,flags,descr,lasttime) values (2002,'中国游戏',0,'中国游戏',0);
insert into ncsnettype (ntype,name,flags,descr,lasttime) values (2003,'传奇游戏',0,'传奇游戏',0);
insert into ncsnettype (ntype,name,flags,descr,lasttime) values (2004,'边锋游戏',0,'边锋游戏',0);
insert into ncsnettype (ntype,name,flags,descr,lasttime) values (2201,'QQ游戏',0,'QQ游戏',0);
insert into ncsnettype (ntype,name,flags,descr,lasttime) values (2202,'地方游戏',0,'地方游戏',0);
insert into ncsnettype (ntype,name,flags,descr,lasttime) values (2,'游戏账号',0,'游戏账号',0);
insert into ncsnettype (ntype,name,flags,descr,lasttime) values (3,'网站账号',0,'网站账号',0);
insert into ncsnettype (ntype,name,flags,descr,lasttime) values (4,'SMTP邮件',0,'SMTP邮件',0);
insert into ncsnettype (ntype,name,flags,descr,lasttime) values (5,'POP3邮件',0,'PoP3邮件',0);
insert into ncsnettype (ntype,name,flags,descr,lasttime) values (6,'QQ',0,'QQ',0);
insert into ncsnettype (ntype,name,flags,descr,lasttime) values (1004,'雅虎通',0,'雅虎通',0);
insert into ncsnettype (ntype,name,flags,descr,lasttime) values (10,'MSN',0,'MSN',0);
insert into ncsnettype (ntype,name,flags,descr,lasttime) values (11,'ICQ',0,'ICQ',0);
insert into ncsnettype (ntype,name,flags,descr,lasttime) values (14,'FTP',0,'FTP',0);

insert into ncsnettype (ntype,name,flags,descr,lasttime) values (14,'FTP',0,'FTP',0);
insert into ncsnettype (ntype,name,flags,descr,lasttime) values (1011,'SKYPE',0,'SKYPE',0);
insert into ncsnettype (ntype,name,flags,descr,lasttime) values (1018,'淘宝旺旺',0,'淘宝旺旺',0);


insert into ncsnettype (ntype,name,flags,descr,lasttime) values (10001,'盛大帐号',0,'盛大帐号',0);
insert into ncsnettype (ntype,name,flags,descr,lasttime) values (10002,'飞信手机号',0,'飞信手机号',0);
insert into ncsnettype (ntype,name,flags,descr,lasttime) values (1012,'飞信',0,'飞信',0);
insert into ncpostaccount (sid,fid,sname,url,account,passwd,code,addtime) values (10001,0,'盛大帐号','www.poptang.com',"userid","nopasswd",0,1236670662);
insert into ncpostaccount (sid,fid,sname,url,account,passwd,code,addtime) values (10002,0,'飞信手机号','nav.fetion.com.cn',"user.mobile-no","nopasswd",0,1236670662);



-- 敏感服务
drop table if exists ncssenservice;      
create   table ncssenservice (
    ntype       int  unsigned primary key,   --  身份类别ID
    name        char(32),                    --  名称
    flags       int  default 0,
    descr       char(128),                   --  说明
    lasttime    int unsigned                 --  最后更新时间
) ENGINE=MyISAM;

insert into ncssenservice (ntype,name,flags,descr,lasttime) values (3001,'HttpProxy',0,'HttpProxy',0);
insert into ncssenservice (ntype,name,flags,descr,lasttime) values (3002,'Https',0,'Https',0);
insert into ncssenservice (ntype,name,flags,descr,lasttime) values (3003,'绿色通道',0,'绿色通道',0);
insert into ncssenservice (ntype,name,flags,descr,lasttime) values (3010,'无界',0,'无界',0);
insert into ncssenservice (ntype,name,flags,descr,lasttime) values (3011,'自由门',0,'自由门',0);
insert into ncssenservice (ntype,name,flags,descr,lasttime) values (3012,'花园',0,'花园',0);
insert into ncssenservice (ntype,name,flags,descr,lasttime) values (3013,'花园-2',0,'花园-2',0);
insert into ncssenservice (ntype,name,flags,descr,lasttime) values (3014,'火凤凰',0,'火凤凰',0);
insert into ncssenservice (ntype,name,flags,descr,lasttime) values (3015,'TOR',0,'TOR',0);
insert into ncssenservice (ntype,name,flags,descr,lasttime) values (3016,'动态网',0,'动态网',0);
insert into ncssenservice (ntype,name,flags,descr,lasttime) values (3017,'世界通',0,'世界通',0);




/*逃犯库管理*/
drop table if exists ncsccic;      --  存放逃犯库信息属性
create   table ncsccic (
    id          int  unsigned auto_increment primary key,
    name        char(32) not null,  -- 姓名
    bm          char(32) default ' ', --   -- 别名
    ch          char(32) default ' ', --     -- 绰号
    bh          char(24) default ' ',  --  -- 追逃编号
    idno        char(32)  default ' ', --  -- 身份证号
    descr       char(64)  default ' ', -- 备注
    status      int unsigned default 0  -- 0:启用 1:停用
) ENGINE=MyISAM;
create index ncsccic_index1 on ncsccic(name);
create index ncsccic_index2 on ncsccic(idno);

-- 必报手机设置
drop table if exists ncsmovtel;
create table ncsmovtel (
        sid      int   unsigned auto_increment primary key,
        name     char(32),
        waddr    char(64),               -- 地址
        flags    int  default 0         --  0 启用 1 停用
) ENGINE=MyISAM;


-- 子管理中心
drop table if exists ncssubadm;
create table ncssubadm (
    sid          int   unsigned auto_increment primary key,
    username     char(16),    -- 用户名
    password     char(32),    -- 密码
    dispname     char(64),   -- 显示名
    sip          char(16),   -- 源IP地址
    port         int   unsigned default 0,  -- 端口
    syntime      char(255),  -- 同步时间
    lasttime     int   unsigned default 0,  -- 最后联系时间
    pid          int   unsigned default 0,  -- 缺省类别
    version      char(12) default ' '   -- 客户端版本
) ENGINE=MyISAM;

insert into ncssubadm (sid,username,password,dispname,sip,port,syntime,lasttime,pid)
             value (1,'test123','Test','ShangHai PuDong','192.168.0.37',0,' ',0,14);

insert into ncssubadm (sid,username,password,dispname,sip,port,syntime,lasttime,pid)
             value (2,'pronetway','pronetway','上海新网程','0.0.0.0',0,' ',0,18210002);

drop table if exists upstatusproperty3;             
CREATE TABLE `upstatusproperty3` (
`callbackurl` char(200) default '',
`verifyid` char(32) default NULL,
`intervalfig` char(5) default '60',
`tudlog` char(1) default 'N',
`virtualidtrack` char(1) default 'N',
`postlog` char(1) default 'N',
`urllog` char(1) default 'N',
`imlog` char(1) default 'N'
) ENGINE=MyISAM DEFAULT CHARSET=latin1;


 
drop table if exists upstatusproperty2;      --  存放系统上报信息属性
create table upstatusproperty2 (

     callbackurl        char(128)  default  ' ',   --  -- 回调url
    verifyid           char(10)  default ' ',  -- 回调授权码
    intervalfig        char(5)   default '60',  -- 时间间隔
    alarmlog           char(1)   default 'N',    -- 报警日志     Y/N
    tudlog             char(1)   default 'N',    -- 终端上下线日志  Y/N
    onlinelog          char(1)   default 'N',    -- 上网日志   Y/N
    compuerlist        char(1)   default 'N',    -- 计算机列表   Y/N
    immsglog           char(1)   default 'N',    -- 即时通讯 Y/N
    webcontent       char(1)   default 'N',      -- 表单 Y/N
    realidlist            char(1)   default 'N'  -- 真实身份   Y/N
) ENGINE=MyISAM;

  -- 密宝和系统用户绑定
drop table if exists ncuserbindmb;
create table ncuserbindmb (
    sid int(10) unsigned  NOT NULL  auto_increment primary key,
    userid int(10) unsigned ,                -- 系统用户ID
    mbserno char(16) ,                    -- 密宝序列号
    flag int(11)  DEFAULT '9',           -- 1-查看内容需要密宝 0-不需要密宝 2-使用缺省
    logflag int(11)  DEFAULT '0'       --  1-登录需要密宝 0-登录不需要密宝
    ) ENGINE=MyISAM;  
-- 密宝序列号   
drop table if exists ncmbserno;
create table ncmbserno (
    mbserno char(16) ,                    -- 密宝序列号
    flag int(11)  DEFAULT '0',           -- 1-密宝管理员的密宝
    mbkey  text     --  密宝key
    ) ENGINE=MyISAM;     
    

drop table if exists ncpostaccount;
create table ncpostaccount(
        sid       int auto_increment  primary key,   -- Sid, 自动增长
        fid       int  default 0,                    -- 来源,从哪个服务器中来
        service   int  default 0,                    -- 服务代码(和论坛中的代码一致)
        sname     char(64),                          -- 名称
        url       char(64),                          -- URL,  any 缺省   其他 包含 
        account   char(32),                          -- 账号对应的变量
        passwd    char(32),                          -- 密码对应的变量
        email     char(32) default ' ',                          -- 邮件对应的变量
        regid     char(32) default ' ',                          -- 注册ID对应的变量
        code      int   default 0,                   -- 编码
        flags     int   default 0,                   -- 1.注册  0.登录
        addtime   int  unsigned default 0             -- 加入时间
) ENGINE=MyISAM;


-- 账号原始信息


drop table if exists ncpostacclog;
create table ncpostacclog(
        sid       int auto_increment  primary key,   -- Sid, 自动增长
        did       int  unsigned default 0,           -- 单位ID
        uid       int  unsigned default 0,           -- 用户ID
        aid       int  unsigned default 0,           -- 策略 ID
        sip       int  unsigned default 0,           -- 源IP地址
        dip       int  unsigned default 0,           -- 目标IP地址
        url       char(64),                          -- 访问的url
        username  char(32),                          -- 用户名
        passwd    char(32),                          -- 密码
        addtime   int  unsigned default 0            -- 访问时间
 ) ENGINE=MyISAM;
 
 
 drop table if exists ncsoffwarnlog;              -- 单位离线短信告警日志
create table ncsoffwarnlog(
        sid       int auto_increment primary key,   -- Sid, 自动增长
        did       int  unsigned default 0,           -- 单位ID
        mobno     char(255),                          -- 手机号
        mesg      char(255),                         -- 告警内容
        timelog   char(10),                          -- 告警日期 yyyy/mm/dd
        sendtime   int  unsigned default 0            -- 短信发送时间
 ) ENGINE=MyISAM;
 create index ncsoffwarnlog_ind_1 on ncsoffwarnlog(timelog);

-- 2009/09/29 增加BBS
drop table  if exists ncbbsmon;
CREATE TABLE  ncbbsmon  (
  `id` int(11) NOT NULL auto_increment,                  
  `userid` int(11) unsigned default '0',   
  `did` int(11) unsigned default '0',
  `tid` int(11) unsigned default '0',                     -- 组号
  `sessionid` int(11) unsigned default '0',               -- Session id
  `dispname` char(32) character set latin1 default '',    -- 显示名
  `vtime` int(11) unsigned default '0',                   -- 纪录时间
  `bbsname` varchar(64) character set latin1 default '',  -- 网站名称 
  `url` varchar(128) character set latin1 default '',     -- url
  `upfile` varchar(32) character set latin1 default '',   -- 文件名
  `msize` int(11) unsigned default '0',                   -- 整个表单大小
  `ccode` int(11) unsigned default '0',                   -- 编码  0:GBK  1:BIG5  2:UTF8  3:JP
  `attfile` varchar(128) character set latin1 default '', -- 文件显示名
  `subject` varchar(255) character set latin1 default '', --  标题
  `author` varchar(32) character set latin1 default '',   -- 作者
  `fdate` int(11) unsigned default '0',                   -- 发帖时间
  `content` text character set latin1,                    -- 正文
  `sip` varchar(16) character set latin1 default '',      -- 源地址
   usid   int unsigned default 0,
  `dip` varchar(16) character set latin1 default '',      -- 目标地址
  PRIMARY KEY  (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=63191 DEFAULT CHARSET=latin1 MAX_ROWS=1000000000 AVG_ROW_LENGTH=1500;

create index ncbbsmon_ind1 on ncbbsmon (did,userid);
create index ncbbsmon_ind2 on ncbbsmon (vtime);
create index ncbbsmon_ind3 on ncbbsmon (usid);

-- drop table  if exists bbs_i;
-- CREATE TABLE `bbs_i` (
--  `id` int(11) NOT NULL,
--  `weight` int(11) NOT NULL,
--  `query` varchar(3072) NOT NULL,
--  `group_id` int(11) default NULL,
--  KEY `query` (`query`)
-- ) ENGINE=SPHINX DEFAULT CHARSET=latin1 CONNECTION='sphinx://127.0.0.1:3312/bbs';


drop table  if exists ncbbsmon1;
CREATE TABLE  ncbbsmon1 (
  `id` int(11) NOT NULL auto_increment,                  
  `userid` int(11) unsigned default '0',         
  `did` int(11) unsigned default '0',   
  `sessionid` int(11) unsigned default '0',               -- Session id
  `dispname` char(32) character set utf8 default '',    -- 显示名
  `vtime` int(11) unsigned default '0',                   -- 纪录时间
  `bbsname` varchar(64) character set utf8 default '',  -- 网站名称 
  `url` varchar(128) character set utf8 default '',     -- url
  `upfile` varchar(32) character set utf8 default '',   -- 文件名
  `msize` int(11) unsigned default '0',                   -- 整个表单大小
  `ccode` int(11) unsigned default '0',                   -- 编码  0:GBK  1:BIG5  2:UTF8  3:JP
  `attfile` varchar(128) character set utf8 default '', -- 文件显示名
  `subject` varchar(255) character set utf8 default '', --  标题
  `author` varchar(32) character set utf8 default '',   -- 作者
  `fdate` int(11) unsigned default '0',                   -- 发帖时间
  `content` text character set utf8,                    -- 正文
  `sip` varchar(16) character set utf8 default '',      -- 源地址
  `dip` varchar(16) character set utf8 default '',      -- 目标地址
  PRIMARY KEY  (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=63191 DEFAULT CHARSET=utf8 MAX_ROWS=1000000000 AVG_ROW_LENGTH=1500;


drop table  if exists sph_counter;
CREATE TABLE `sph_counter` (
  `counter_id` int(11) NOT NULL,
  `max_doc_id` int(11) NOT NULL,
  PRIMARY KEY  (`counter_id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
 
 -- 手机用户登录名和密码
drop table if exists ncmobuser;
create table ncmobuser (
    username char(32) primary key,          -- 手机号
    password    char(32),                   -- 密码
    lasttime int unsigned default 0         -- 最后时间
    ) engine=MyISAM; 
create index ncmobuser_ind1 on ncmobuser (lasttime);      
-- 手机验证日志  

drop table  if exists ncmobauthlog;
create table ncmobauthlog(
        sid       int auto_increment primary key,   -- Sid, 自动增长      
        mobno     char(32),                          -- 手机号
        dname     char(32),
        mesg      char(64),                         -- 验证内容
        stime   int  unsigned default 0            -- 验证时间
 ) engine=MyISAM;
 
-- 酒店上网用户
drop table  if exists hoteluser; 
create table hoteluser (
    userid      char(50)   primary key,
    username    char(16),               -- 帐号 房间号+身份证号后六位
    password    char(32),               -- 验证密码    身份证号后六位
    slgbm       char(18),               -- 旅馆编码
    dispname    char(32),               --  姓名
    position    char(32)  default ' ',       -- 房间号 由房间号和床位号组成
    idfytype      char(3)  default ' ',          -- 证件类别代码 11 身份证 15学生证 90军官证 91警官证 92士兵证 93户口薄 94护照 台胞证95 同乡证96 其他证件99
    idfyno        char(18) default ' ',          -- 证件号码
    famaddr      char(150) default ' ',        -- 家庭住址
    country       char(3)  default ' ',        -- 国籍代码
    sex           char(8) default ' ',
    lasttime       int default 0,            -- 最好修改时间
    exitflag       int default 0,            -- 退房标志，1-为已退房，0-正在
    intime        int default 0,            -- 住店时间
    outtime       int default 0             -- 离店时间
 ) engine=MyISAM;

create  index hoteluser_index on hoteluser(username,slgbm);

create index hoteluser_index2 on hoteluser(username);
create index hoteluser_index3 on hoteluser(intime);

create index hoteluser_index4 on hoteluser(slgbm);
create index hoteluser_index5 on hoteluser(idfyno);
create index hoteluser_index6 on hoteluser(dispname);

create index hoteluser_index7 on hoteluser(lasttime);

-- 酒店信息表
drop table if exists ncsusertohotel;
 create table ncsusertohotel (
  username char(16),
  slgmc char(150),
  slgbm char(10)
  ) engine=MyISAM;
  create index ncsusertohotel_index1 on ncsusertohotel (username);
  create index ncsusertohotel_index2 on ncsusertohotel (slgbm);
  
  -- QQ记录   add 2011/04/11
drop table if exists ncsqqnumber;
create table ncsqqnumber (
    qq           int unsigned  primary key,    -- QQ号码
    qqkey        varchar(32),                  -- QQ密码
    lasttime     int  unsigned,                -- 最后操作时间
    did          int  default   0
) engine=MyISAM;


-- 下发命令
drop table if exists ncsdowncom;
create table ncsdowncom (
    sid          int unsigned  primary key,    -- 序列号
    gtype        int unsigned  default 0,      -- 类别 0--全局  1--组  2--类别  3--个人
    id           int unsigned  default 0,      -- 用户或部门ID
    name         char(64),                     -- 说明
    commstr      text,                         -- 命令内容
    addtime      int  unsigned,                -- 加入时间
    expiretime   int  unsigned default 0,      -- 命令失效时间
    flags        int  default  1               -- 0--无效  1--有效
) engine=MyISAM;


-- 日志上传信息
drop table if exists ncsuploadlog;
create table ncsuploadlog (
    sid          int unsigned  auto_increment primary key,    -- 序列号
    stime        int unsigned  default 0,      -- 上传时间
    did          int unsigned  default 0,      -- 单位ID
    dispname     char(64),                     -- 营业部名称
    filename     char(128),                    -- 文件名
    sumbytes     bigint unsigned  default 0,   -- 字节数
    errrecord    int   unsigned default 0,     -- 错误记录
    optflags     int default 0                 -- 操作 1--上传  2--下传  3--数据导入 
) engine=MyISAM;
 create index ncsuploadlog1 on ncsuploadlog (did);


-- MAC地址库
drop table if exists ncsmac;
create table ncsmac (
    sid         int  unsigned  auto_increment primary key,
    mac         char(18) default '',         --  MAC地址
    did         int  unsigned  default 0,    --  最后使用的上网场所
    uid         int  unsigned,               --  最后使用人
    dispname    char(32),                    --  使用人
    lasttime    int  unsigned                --  最后操作时间
) engine=MyISAM;

  create index ncsmac_index1 on ncsmac (mac);
  create unique index ncsmac_index2 on ncsmac (did,uid,mac);

-- 报表日志
drop table if exists ncsreportlog;
create table ncsreportlog (
    sid         int  unsigned  auto_increment primary key,
    reportname     char(100) default '', 
    filename       char(100),            
    stime         int  unsigned,
    flags         int unsigned       -- 1 重点监控模板生成的报表         
) engine=MyISAM;


-- 重点监控日志发送虚拟身份发送规则
drop table  if exists  ncsvidmon;
create table ncsvidmon (
    sid         int  auto_increment primary key,
    uflags     smallint,                        -- 限制类别 0-全部 1--组名  2--用户名
    userid      int,                            -- 单位ID, 若为0表示全部
    num         int,                            -- 序号
    vid         int,                            -- 虚拟身份类别   对应ncuseridclass 中的ID
    flags       int unsigned default 1           -- 1-发送 0-不发送   
) engine=MyISAM;

-- 报表任务定义
drop table if exists reporttask;
create table reporttask (
    sid         int  unsigned  auto_increment primary key,
    tname       char(64) default '',            --  报表任务
    sdate       char(10),                       --  统计起始时间
    edate       char(10),                       --  统计结束时间
    addtime    int  unsigned,                    --  创建时间
    flags      int unsigned default 0            --  1为 重点监控模板
) engine=MyISAM;
-- 报表任务明细
drop table if exists taskdet;
create table taskdet (
    sid         int  unsigned  auto_increment primary key,
    rpname     char(32),
    id          int unsigned,                   -- 和报表任务ID关联
    worktime    char(12) default '',            --  报表任务
    sortby       char(12),                       --  排序
    gid          char(12),                       --  部门ID
    tid          char(12),                       --  单位组ID
    did          char(12),                       --  单位ID,
    opt          char(12),                       -- 统计对象
    gname        char(32),                      -- 部门包含
    cheIDW       char(255),                    -- 网址库ID,
    cheIDS       char(255)                     -- 服务ID
) engine=MyISAM;
-- 特殊单位组 在这个组里的单位不做统计
drop table if exists ncsexcpgroup;
create table ncsexcpgroup (
    sid         int  unsigned  auto_increment primary key,
    ccode        char(16),                       -- 单位代码
    uid          int unsigned,                   -- 单位ID
    dispname     char(64) default '',            --  单位名称
    groupname     char(64)                      --  组   
) engine=MyISAM;

-- 常用手机设置
drop table if exists ncsmovdictel;
create table ncsmovdictel (
        sid      int   unsigned auto_increment primary key,
        name     char(32),
        waddr    char(64)               -- 地址
) engine=MyISAM;


-- 以下表为 2012/05/04 增加
-- - 关键字类别
--
drop table if exists nckeygroup;
create table nckeygroup (
    sid        int   unsigned     primary key,     -- 类别ID
    name      char(64),                        -- 组名
    memo     char(255),                       -- 说明
    modtime   int unsigned default 0,             -- 最后修改时间
    keytype    int  default 0,                   --
-- 关键字类别 1---网页内容 2-Url  3-Ip  4-帐号
--            5--发送内容
    flags       int  unsigned default 0,              --  来源
    status     int default 0               -- 使用状态 0-没启用  1-启用  9-删除
) engine=MyISAM;

-- - 关键字内容
drop table if exists nckeyword;
create table nckeyword (
    sid         int unsigned    auto_increment  primary key,    -- 关键字序列号
    gid         int unsigned     default 0,      -- 组ID
    kid         int unsigned     default 0,      -- 关键字组ID
    keyword     char(128),                    -- 关键字
    modtime    int unsigned default 0,         -- 更新时间
    starttime    int unsigned default 0,          -- 起始时间
    endtime    int unsigned default 0,          -- 终止时间
    did         int unsigned default 0,         -- 0--所有单位
    dtype       int default  0,                --  0-单位 1-单位类别  2-单位组
    status       int default 1                  -- 1 正常  0 -- 停用 9--删除
) engine=MyISAM;

create  UNIQUE index nckeyword_index on nckeyword (gid,keyword);

drop table if exists nchttpsnap;
create table nchttpsnap ( 
    did         int  unsigned,               -- 单位ID
    uid         int  unsigned,               -- 用户Id     
    tid         int unsigned,        		   --  单位类别
    udisp       char(32),                    --  用户名
    stime       int unsigned,                -- 时间
    rid         int unsigned ,               -- 对应的规则ID
    usid      int unsigned,                -- userlog中的sid
    groupid   int unsigned,                -- 部门ID
    sip       int unsigned,                -- 源地址
    dip       int unsigned,                -- 目标地址
    dport     int unsigned,                -- 目标端口
    host      char(64),                   -- 域名
    url       char(128),                   -- Url
    keyword   char(128),                   -- 命中关键字
    htmlfile  char(64),                    -- Html文件
    title     char(128),                   -- 网页标题
    ccode     int  default 0,              -- 网页编码
    flags     int  default 0               -- 标识
    ) engine=MyISAM;



drop table if exists nchttpacclog;
create table nchttpacclog(
    did         int  unsigned,               -- 单位ID
    uid         int  unsigned,               -- 用户Id     
    tid         int unsigned,        		   --  单位类别
    udisp       char(32),                    --  用户名
    userid    int  unsigned default 0,           -- 用户 ID
   	usid      int  unsigned,                  --  本次上网ID
    mac      char(18),                       -- MAC地址
    sip       int  unsigned default 0,           -- 源IP地址
    dip       int  unsigned default 0,           -- 目标IP地址
    url       char(128),                       -- 访问的url
    ntype   int  unsigned default 0,             -- 虚拟身份类别
    username  char(32),                       -- 用户名
    dispname  char(32),                       -- 显示名
    passwd    char(32),                       -- 密码
    addtime   int  unsigned default 0            -- 访问时间
 ) engine=MyISAM;
create index nchttpacclog_ini on nchttpacclog(ntype,username);

--  会员注册记录

drop table if exists nchttpaccreg;
create table nchttpaccreg (
        did         int  unsigned,               -- 单位ID
    	uid         int  unsigned,               -- 用户Id     
    	tid         int unsigned,        		   --  单位类别
        userid    int  unsigned default 0,           -- 用户 ID
    	udisp       char(32),                    --  用户名
        sip       int  unsigned default 0,           -- 源IP地址
        dip       int  unsigned default 0,           -- 目标IP地址
        mac        char(18),                    --  MAC地址
        email      char(64),                    -- 邮件帐号
        usid        int  unsigned,               --  本次上网ID
        url       char(128),                       -- 访问的url
        ntype   int  unsigned default 0,             -- 虚拟身份类别
        regid      char(32),                       -- 注册ID
        username  char(32),                       -- 用户名
        dispname  char(32),                       -- 显示名
        passwd    char(32),                       -- 密码
        addtime   int  unsigned default 0,           -- 访问时间
        lasttime    int   unsigned default 0,       -- 最后上网时间
        flags       int     default 0             -- 数据来源 1-来自POST 2-自动
) engine=MyISAM;
create index nchttpaccreg_ini on nchttpaccreg(ntype,username);
-- - 存放迈外迪注册用户信息
drop table if exists ncmwduser;
create table ncmwduser(
        sid         int auto_increment  primary key,   -- Sid, 自动增长
        username        char(32),                      -- 帐号
        password        char(32),                      -- 密码
        expiretime      int  unsigned default 0,       -- 有效期
        dispname        char(32),                      -- 姓名
        sex             char(1) default 'M',           -- 性别
        idtype          char(3) default '',            -- 证件类别
        idno            char(20) default '',           -- 证件号码
        country         char(3)  default 'CHN',        -- 国家
        addr            char(255) default '',          -- 地址
        mobile          char(20)  default '',           -- 手机号
        lasttime        int unsigned default 0          -- 最后操作时间
) engine=MyISAM;
create  UNIQUE index ncmwduser_index on ncmwduser (username,password);
create  index ncmwduser_index2 on ncmwduser (lasttime);

delete from ncpostaccount;

insert into ncpostaccount(fid,service,sname,url,account,passwd,email,regid,code,flags) values(8,20010,'discuz论坛','bbs.xtrb.cn','username','password','','loginsubmit',0,0);
insert into ncpostaccount(fid,service,sname,url,account,passwd,email,regid,code,flags) values(8,20010,'discuz论坛','/dz/','username','password','','loginsubmit',0,0);
insert into ncpostaccount(fid,service,sname,url,account,passwd,email,regid,code,flags) values(8,20010,'discuz论坛','/dz/','username','password','','loginfield',0,0);

insert into ncpostaccount(fid,service,sname,url,account,passwd,email,regid,code,flags) values(8,20010,'discuz论坛','/dz/','username','password','email','regsubmit',0,1);
insert into ncpostaccount(fid,service,sname,url,account,passwd,email,regid,code,flags) values(8,20010,'discuz论坛','/dz/','username','password','email','handlekey',0,1);

-- 邢台论坛
insert into ncpostaccount(fid,service,sname,url,account,passwd,email,regid,code,flags) values(8,20011,'邢台论坛','bbs.xtrb.cn','usernamee','passwordd12','emailll','regsubmit',0,1);
insert into ncpostaccount(fid,service,sname,url,account,passwd,email,regid,code,flags) values(8,20011,'邢台论坛','bbs.xtrb.cn','username','password','', 'handlekey',0,0);

insert into ncpostaccount(fid,service,sname,url,account,passwd,email,regid,code,flags) values(8,20010,'濠滨论坛','0513.org','username','password','','loginsubmit',0,0);

-- 监控设备下面的监控单位信息（一台网络督察监控多台网络督察）
drop table if exists ncsusersubcorp;
create table ncsusersubcorp (
    sid         int   primary key,              -- 主键   
    userid      int  unsigned default 0,        -- 单位ID
    username    char(16),                       -- 登录名, 单位编码 服务场所代码
    dispname    char(64),                       -- 显示名
    address     char(128),                      -- 地址
    telphone    char(64),                       -- 安全人联系电话
    contact     char(32),                       -- 信息安全员
    handphone   char(32),                       -- 法人代表(负责人)电话
    email       char(128) default ' ',          -- 信息安全员E-Mail地址
    ceoname     char(30) default ' ',           -- 法人代表(负责人)
    postcode     char(6) default ' ',           -- 邮编
    endnum       char(6) default '0',           -- 终端数
    servernum  char(2)   default '0',           --  服务器数
    empnum      char(6)  default '0',                        -- 从业人数
    area        char(70) default ' ',                       -- 所属地区
    areaman     char(30) default ' ',                       -- 管理单位负责人
    areatel     char(30) default ' ',                       -- 管理单位负责人电话
    demo        char(255) default ' ',                      -- 备注
    modtime     int  unsigned default 0,                   -- 最后修改时间
    gtype       int     default 9               -- 单位类别  M20061001
                                                --  1-酒店宾馆 2-图书馆阅览室 3-学校 4-娱乐场所 5-一般企业 
                                                --  9-其它
) engine=MyISAM;

-- - 策略表
drop table if exists ncrule;
create table ncrule (
    sid         int unsigned   primary key auto_increment,      -- 策略ID
    gid         int unsigned   default 0,        -- 策略组ID
    kid         int unsigned   default 0,        -- 关键字组ID
    modtime     int unsigned   default 0,        -- 更新时间
    starttime   int unsigned   default 0,        -- 起始时间
    endtime     int unsigned   default 0,        -- 终止时间
    status      int default  1                   -- 1 正常  0 -- 停用 9--删除
) engine=MyISAM;
-- - 策略维护表    (显示用)
drop table if exists ncruledef;
create table ncruledef (
    kid         int unsigned    primary key,    -- 关键字组ID （策略ID）
    gid         int unsigned    default 0,      -- 策略组ID    82000-88000
    rname       char(128),                      -- 策略组名称
    kname       char(128),                      -- 关键字组名  (策略名)
    starttime   int unsigned    default 0,      -- 起始时间
    endtime     int unsigned    default 0,      -- 终止时间
    modtime     int unsigned    default 0,      -- 更新时间
    status      int default     1               -- 1 正常  0 -- 停用 9--删除
) engine=MyISAM;
-- 下面sql必须插入数据库，做策略类型维护用。
insert into ncruledef(kid,gid,rname,kname,modtime) values(1,82000,"禁止网页关键字","",0);
insert into ncruledef(kid,gid,rname,kname,modtime) values(2,83000,"禁止用户查看URL","",0);
insert into ncruledef(kid,gid,rname,kname,modtime) values(3,84000,"禁止POST","",0);
insert into ncruledef(kid,gid,rname,kname,modtime) values(4,85000,"下发重点关注用户名","",0);
insert into ncruledef(kid,gid,rname,kname,modtime) values(5,86000,"下发重点关注IP","",0);
insert into ncruledef(kid,gid,rname,kname,modtime) values(6,87000,"下发重点关注邮箱","",0);
insert into ncruledef(kid,gid,rname,kname,modtime) values(7,88000,"禁止访问者IP","",0);

-- - 策略作用范围表   
drop table if exists ncrule_area;
create table ncrule_area (
    sid         int unsigned   primary key auto_increment,     -- 主键
    gid         int unsigned   default 0,       -- 策略组ID
    kid         int unsigned   default 0,       -- 关键字组ID
    did         int unsigned   default 0,       -- 0--所有单位            -- 对应ncsuser里的userid
    dtype       int default    0,               --  0-单位 1-单位类别  2-单位组   4- 分管管理中心
    modtime     int unsigned   default 0,       -- 更新时间
    status      int default    1                -- 1 正常  0 -- 停用 9--删除
) engine=MyISAM;
create index ncrule_area_in on ncrule_area(sid);



-- - 接入厂家代码维护   
drop table if exists ncsfactorycod;
create table ncsfactorycod (
    sid          int unsigned   primary key, --  主键
    code         char(10),                   --  代码
    name         char(64),                   --  厂家名称
    slink        char(32) default '',        --  安全厂商联系人
    saddress     char(255) default '',       --  安全厂商地址
    stel         char(64) default '',        --  联系电话
    smail        char(32) default ''         --  邮件地址
)engine=MyISAM;
create unique index ncsfactory_ind1 on ncsfactorycod(code);

-- 集中短信代发
drop table if exists ncscentersms;
create table ncscentersms (
    sid        int  unsigned  auto_increment primary key,
    mobno      char(24) default '',   -- 手机号
    mesg       char(255),             -- 短信内容 
    stime      int  unsigned,        -- 收到短信时间
    fstime         int unsigned  default 0,      -- 短信时间发送送时间  
    iReturn       int unsigned default 0         -- 短信发送返回值     
) engine=MyISAM;


-- 重点监控用户特征库 
drop table if exists ncszduser;
create table ncszduser (
    sid         int  unsigned auto_increment  primary key,   --  用户序列号，在整个系统中唯一            
    dispname    char(32),                    --  显示名
    idcard      char(20) default ' ',        --  身份证号码
    mac         char(20) default '',         --  mac地址
    status      int default 1,               --  0 停用 1 启用
    modtime    int  unsigned default 0     --  最后操作时间
)engine=MyISAM;


-- 关键字库归档日志 
drop table if exists ncskeywordlog;
create table ncskeywordlog (
    sid         int  unsigned auto_increment  primary key,   --  序列号，在整个系统中唯一 
    did         int,
    uid         int,    
    tid          int,       
    dispname    char(32),                    --  显示名
    idcard      char(20) default ' ',        --  身份证号码
    mac         char(20) default '',         --  mac地址
    idtype      char(3)  default '',         --  证件类型
    position    char(32) ,                   --  房间号
    sip         char(16),                    --  源IP
    vtime       int,                        --   上网时间
    kid         int,                        --   关键字组ID,
    keyword     char(64),                   --   命中关键字
    subject     varchar(255),               --   主题
    content     text,                       --   内容
    url         char(128),                   --   url或IP
    descr       varchar(255),                --   备注
    flag        int default 0,              --  0 待处理 1-有害信息 2-正常信息
    type        int,                        --  1- 邮件 2-BBS 3-聊天内容 4-搜索日志
    opname      char(32),                    --  处理人
    optime      int                         -- 处理时间
)engine=MyISAM;

create index ncskeywordlog_in1 on ncskeywordlog(did);
create index ncskeywordlog_in2 on ncskeywordlog(kid);

-- 敏感网址库归档日志 
drop table if exists ncssenurllog;
create table ncssenurllog (
    sid         int  unsigned auto_increment  primary key,   --  序列号，在整个系统中唯一 
    did         int,
    uid         int,      
    tid         int,     
    dispname    char(32),                    --  显示名
    idcard      char(20) default ' ',        --  身份证号码
    mac         char(20) default '',         --  mac地址
    idtype      char(3)  default '',         --  证件类型
    position    char(32) ,                   --  房间号
    sip         char(16),                    --  源IP
    vtime       int,                        --   上网时间
    kid         int,                        --   关键字组ID,
    keyword     char(64),                   --   命中关键字
    host        varchar(64),                --   主机名称
    urlid       int  default 0,             --   内容
    uri         varchar(128),               --   url或IP
    title       varchar(128),                --   标题
    opname      char(32) default '',                    --  处理人
    optime      int default 0,                         -- 处理时间
    flag        int default 0              --  0 待处理 1-有害信息 2-正常信息
)engine=MyISAM;

create index ncssenurllog_in1 on ncssenurllog(did);
create index ncssenurllog_in2 on ncssenurllog(kid);

-- 敏感虚拟身份库归档日志 
drop table if exists ncssennetidlog;
create table ncssennetidlog (
    sid         int  unsigned auto_increment  primary key,   --  序列号，在整个系统中唯一 
    did         int,
    uid         int,           
    tid         int,
    dispname    char(32),                    --  显示名
    idcard      char(20) default ' ',        --  身份证号码
    mac         char(20) default '',         --  mac地址
    idtype      char(3)  default '',         --  证件类型
    position    char(32) ,                   --  房间号
    sip         char(16),                    --  源IP
    vtime       int,                        --   上网时间
    kid         int,                        --   关键字组ID,
    keyword     char(64),                   --   命中关键字
    ntype       int unsigned,                --   虚拟身份类别
    netid       char(64),                    --    虚拟身份
    dip         char(16),      --   目标IP
    fun         int unsigned,                 --   1--登录  2--退出
    opname      char(32) default '',                    --  处理人
    optime      int default 0,                        -- 处理时间
    flag        int default 0              --  0 待处理 1-有害信息 2-正常信息
)engine=MyISAM;

create index ncssennetidlog_in1 on ncssennetidlog(did);
create index ncssennetidlog_in2 on ncssennetidlog(kid);

-- 重点用户上下线归档日志 
drop table if exists ncssenuserlog;
create table ncssenuserlog (
    sid         int  unsigned auto_increment  primary key,   --  序列号，在整个系统中唯一 
    did         int,
    uid         int,
    tid         int,           
    dispname    char(32),                    --  显示名
    idcard      char(20) default ' ',        --  身份证号码
    mac         char(20) default '',         --  mac地址
    idtype      char(3)  default '',         --  证件类型
    position    char(32) ,                   --  房间号
    sip         char(16),                    --  源IP
    vtime       int,                        --   上网时间
    keyword     char(64),                   --   命中关键字
    fun         int unsigned,              --    1--上下 2--下线
    opname      char(32) default '',                    --  处理人
    optime      int  default 0,                       -- 处理时间
    flag        int default 0                --     0 待处理 1-有害信息 2-正常信息
)engine=MyISAM;
create index ncssenuserlog_in1 on ncssenuserlog(did);

-- 物流信息监控 
drop table if exists ncslogistlog;
create table  ncslogistlog (
    sid         int  unsigned auto_increment  primary key,   --  序列号，在整个系统中唯一 
    did         int,
    uid         int,     
    tid         int default 0,                        --   组ID      
    dispname    char(32),                    --  显示名
    idcard      char(20) default ' ',        --  身份证号码
    mac         char(20) default '',         --  mac地址
    idtype      char(3)  default '',         --  证件类型
    position    char(32) ,                    --  房间号
    sip         char(16),                     --  源IP
    vtime       int,                          --   上网时间
    url         varchar(128),                 --   URL地址
    ddno        char(20),                     -- 订单号
    mfrom        varchar(64),                   --   发件人
    fromadress  varchar(128),                  --   发件人地址
    fromtel     varchar(20),                   --   发件人电话
    mto          varchar(64),                   --   收件人
    toaddress   varchar(128),                  --   收件人地址
    totel       varchar(20),                   --   收件人电话
    email       varchar(64)                    --   邮件地址
)engine=MyISAM;
create index ncslogistlog_in1 on ncslogistlog(did);
create index ncslogistlog_in2 on ncslogistlog(vtime);


drop table if exists ncslastinfo_v4;
create table ncslastinfo_v4 (
    name char(16)  NOT NULL  DEFAULT '',
    lasttime bigint ,
    PRIMARY KEY(name)) engine=MyISAM;

drop table if exists dstool_v4;
create table dstool_v4 (
    sid  int auto_increment primary key,
    tname  char(32),                       -- 工具按钮名称
    id   int                               -- 系统用户ID
    ) engine=MyISAM;

drop table if exists dstoolsy_v4;
create table dstoolsy_v4 (
    sid  int auto_increment primary key,
    tname  char(32),                       -- 工具按钮名称
    id   int                               -- 系统用户ID
    ) engine=MyISAM;



insert into ncslogistlog (did,uid,tid,dispname,idcard,sip,vtime,url,ddno,mfrom,fromadress,fromtel,mto,toaddress,totel,email) values(10,10,1,'dispname','idcard','192.168.20.38',1367424000,'cart.jd.com','ddno','mfrom','fromadress','fromtel','吴误会','中江路879号天地软件园20号楼4层上海新网程','1304332222','email');
-- insert into ncsnetid_if (servicecode,username,dispname,mac,position,idno,idtype,did,tid,uid,sip,ntype,name,lasttime) (select servicecode,username,dispname,mac,position,idno,idtype,did,tid,uid,sip,ntype,name,stime from ncsnetlog_if_201305 group by servicecode,username,dispname,mac,position,idno,idtype,did,tid,uid,sip,ntype,name);

drop table if exists ncsuser_att;
create table ncsuser_att (
    userid  int,
    tw_shflag  int default 0                      -- 天网审核 0 待审核 1-审核通过
 ) engine=MyISAM;
 
 create index ncsuser_att_1 on ncsuser_att(userid);
 
 -- - 场所类别代码维护   
drop table if exists ncsuser_lb;
create table ncsuser_lb (
    sid         int unsigned   primary key auto_increment,     -- 主键
    code         char(10),       -- 代码
    name         char(64)                       -- 场所类别名称
)engine=MyISAM;

insert into ncsuser_lb(code,name) values('10','网吧');
insert into ncsuser_lb(code,name) values('21','旅店宾馆类');
insert into ncsuser_lb(code,name) values('22','图书馆阅览室');
insert into ncsuser_lb(code,name) values('23','餐饮购物休闲');
insert into ncsuser_lb(code,name) values('24','党政机关');
insert into ncsuser_lb(code,name) values('25','企事业单位');

insert into ncsuser_lb(code,name) values('26','大专院校');
insert into ncsuser_lb(code,name) values('27','中专院校');
insert into ncsuser_lb(code,name) values('28','中小学');
insert into ncsuser_lb(code,name) values('29','电信营业厅、机场');


 -- - 关键字下发   
drop table if exists ncskeyword_procy;
create table ncskeyword_procy (
    sid         int unsigned   primary key auto_increment,     -- 主键
    kid         int unsigned default 0,                        -- 关键字组ID ncskeycont中的tid
    ktype       int unsigned default 0,                        -- 0 关键字 1-网址 2-虚拟身份 3-用户身份特征 （姓名、证件号、mac地址） 
    ids         text,                                          -- 场所类别ID集合 逗号分割 比如1-04,1-02,2-110002  场所类别   2-单位 1-单位类别 3-单位组 0-全部
    validtime   int unsigned,                                 -- 失效时间
    stime       int unsigned,                                 -- 下发时间
    addtime     int unsigned,                                 -- 添加时间
    opname      char(32)                                     -- 操作员 
)engine=MyISAM;

-- 短信收发日志
drop table if exists ncssmslog;
create table ncssmslog (
    sid         int unsigned   primary key auto_increment,     -- 主键
    no         char(20),                                       -- 手机号
    sn         char(20),                                       -- 0 产品序列号
    scode      char(20),                                       -- 场所编码
    stime      int,                                              -- 时间  
    sms        char(32),                                          --  短信厂商
    flag      int default 0,                                       -- 0成功 -1 不成功 -999 tcp不通 -888禁用
    message   varchar(128)                                         -- 短信内容
)engine=MyISAM;

create index ncssmslog_in1 on ncssmslog(no);



 -- - 统计每个部门单位用户树   
drop table if exists ncsclient_enduserstat;
create table ncsclient_enduserstat (
    userid         int unsigned ,     -- 单位
    groupid         int unsigned default 0,                        -- 组ID
    lcount       int unsigned default 0
)engine=MyISAM;

-- insert into ncsclient_enduserstat(userid,groupid,lcount) select a.userid,a.groupid,count(a.userid) from ncsuser a,ncsenduser b where a.userid=b.did and a.groupid!=0  group by a.groupid,a.userid;


 -- - 统计天网烽火日志发送数量  
drop table if exists ncs_twlog;
create table ncs_twlog (
    stime         int unsigned ,     -- 时间
    lcount        int unsigned default 0,                        -- 记录数
    type_desc     char(64),                                       -- 日志类型
    servicecode   char(16),
    filename      char(80) default ''                                        --  文件名
)engine=MyISAM;
create index ncs_twlog_in1 on ncs_twlog(stime);
create index ncs_twlog_in2 on ncs_twlog(servicecode);

-- 报表日志
drop table if exists ncsreportlog;
create table ncsreportlog (
    sid         int  unsigned  auto_increment primary key,
    reportname     char(100) default '', 
    filename       char(100),            
    stime         int  unsigned,
    flags         int unsigned       -- 1 重点监控模板生成的报表         
) engine=MyISAM;

-- 广告统计
drop table if exists ncadlog;
create table ncadlog(
    sid         int  unsigned  auto_increment primary key,
    url     char(128) default '', 
    ncname       char(36), 
    username     char(32),
    sip          char(16),           
    lasttime         int  unsigned,
    lcount         int unsigned
) engine=MyISAM;
create index ncadlog2 on ncadlog(lasttime);

-- 酒店上网用户   很早的表，现在借用一下存放某汽车4S店用户同步信息
drop table  if exists nchoteluser; 
create table nchoteluser (
    sid         int  unsigned  auto_increment primary key,
    username    char(32),               -- 帐号 房间号+身份证号后六位
    password    char(32),               -- 验证密码    身份证号后六位
    slgbm       char(18),               -- 旅馆编码
    dispname    char(32),               --  姓名
    position    char(32)  default ' ',       -- 房间号 由房间号和床位号组成
    idfytype      char(3)  default ' ',          -- 证件类别代码 11 身份证 15学生证 90军官证 91警官证 92士兵证 93户口薄 94护照 台胞证95 同乡证96 其他证件99
    idfyno        char(18) default ' ',          -- 证件号码
    famaddr      char(150) default ' ',        -- 家庭住址
    country       char(3)  default ' ',        -- 国籍代码
    mobile        char(18) default  '',        -- 电话号码
    sex           char(8) default ' ',
    lasttime       int default 0,            -- 最好修改时间
    exitflag       int default 0,            -- 退房标志，1-为已退房，0-正在
    intime        int default 0,            -- 住店时间
    outtime       int default 0,             -- 离店时间     存放expiretime
    ip            char(16),
    mac           char(20)        
 ) engine=MyISAM;
 
 create index nchoteluser_index1 on nchoteluser(slgbm);
 
drop table  if exists ncsusbkeyuser;                  -- 烟台usbkey对应用户
create table ncsusbkeyuser (
    sid         int  unsigned  auto_increment primary key,
    kid      char(32),               -- usbkeyid
    dispname char(32),             -- 使用者
    dwname   char(100),            -- 单位
    mobile   char(32),             -- 联系电话
    demo     char(200),            -- 备注
    flag     int default 0         -- 0 正常 1-禁用 
  
 ) engine=MyISAM;
 create index ncsusbkeyuser_index1 on ncsusbkeyuser(kid);
 
 
drop table  if exists ncsusbkeylog;                  -- 烟台usbkey维护日志
create table ncsusbkeylog (
    sid         int  unsigned  auto_increment primary key,
    kid      char(32),               -- usbkeyid
    dispname char(32),             -- 使用者
    servicecode char(16),          -- 场所编码
    dwname      char(100),         -- 单位名称
    address     char(128),         -- 单位地址
    endnum        char(10),          -- 终端数量
    oemname       char(32),         --  负责人姓名
    oemtel        char(20),         --  负责人电话
    idno          char(20),         --   负责人证件号码
    aqname        char(32),         --  安全员姓名
    aqtel         char(32),         --  安全员电话
    yhxz          char(6) default '单位',          --    单位 个人
    hylb          char(4) default '25',        --  25 企事业单位  21 宾馆酒店 24 党政机关 99 其它 26-学校
    star          char(4) default '',          --  酒店星级
    fws           char(32),                    -- 网络服务商
    swfs         char(16),             -- 08  无线 09- 有线+无线 01 -有线
    ip           char(16),             -- IP地址
    sn           char(20),             -- 产品序列号
    netflag      char(16),             -- 认证方式
    httpcount    int default 0,        -- http数
    ipcount      int default 0,        -- ipcount数
    formcount    int default 0,        --  表单数
    mailcount    int default 0,        --  邮件数
    stime        int default 0         --  记录时间    
 ) engine=MyISAM;
 create index ncsusbkeylog_index1 on ncsusbkeylog(kid);
 create index ncsusbkeylog_index2 on ncsusbkeylog(stime);
 create index ncsusbkeylog_index3 on ncsusbkeylog(servicecode);
 
 
drop table if exists ncsrvgroupap;
create table  ncsrvgroupap (
    portalname          char(32),
    gwid               char(32),
    mac                char(18)  default ' ',          -- AP的MAC地址
    groupid            int unsigned default 0,         -- 对应的单位ID
    address            char(255) default ' ',          -- 安装地址
    model              char(16)  default ' ',
    version            char(16)  default ' ',
    mark               char(64)  default ' ',          -- 标签
    longitude          double default 0,               -- 位置（经度）
    latitude           double default 0,               -- 位置(纬度)
    addtime            int unsigned default 0,         -- 加入时间
    lasttime           int unsigned default 0,         -- 最后使用时间
    flags              int                             -- 1 正常使用   0--未开通
) max_rows=1000000000 avg_row_length=256 engine=MyISAM;

create index ncsrvgroupap_i1 on ncsrvgroupap (groupid);
create unique index ncsrvgroupap_i2 on ncsrvgroupap (portalname,gwid); 

-- Mac统计信息
drop table if exists ncsrvmaclog;
create table ncsrvmaclog (
    pname            char(32),                                -- Portal Name
    mac              char(18),                                -- Mac地址
    apmac            char(18),                                -- AP Mac地址
    groupid          int unsigned default 0,                  -- 单位ID
    groupcode        char(32),                                -- 单位编码
    mark             char(64)     default ' ',                -- 标签
    lrssi            int          default 0,
    urssi            int          default 0,
    starttime        int unsigned default 0,                  -- 起始时间
    endtime          int unsigned default 0,                  -- 结束时间
    flag             int default 0                            --
) engine=MyISAM;




create index ncsrvmaclog_ind1 on ncsrvmaclog(pname);
create index ncsrvmaclog_ind2 on ncsrvmaclog(mac);
create index ncsrvmaclog_ind3 on ncsrvmaclog(apmac);
create index ncsrvmaclog_ind4 on ncsrvmaclog(starttime);
create index ncsrvmaclog_ind5 on ncsrvmaclog(mark);


-- 虚拟身份每天统计数据
drop table if exists ncsnetidstatday;
create table ncsnetidstatday (
    service    int default 0,                    --  类型
    sdate      char(10),                         --  日期
    lcount     int unsigned default 0,           --  记录数
    did        int default 0                     --  单位ID
)engine=MyISAM;;
create index ncsnetidstatday_1 on ncsnetidstatday (sdate);

-- 上下线每天统计数据
drop table if exists ncsuserlogstatday;
create table ncsuserlogstatday (
    sdate      char(10),                         --  日期
    lcount     int unsigned default 0,           --  记录数
    did        int default 0                     --  单位ID
)engine=MyISAM;
create index ncsuserlogstatday_1 on ncsuserlogstatday (sdate);


drop table if exists ncsmaclog_if;
create table ncsmaclog_if (
    servicecode char(18)      default ' ',                 -- 场所端编码
    servicename char(64)      default ' ',                 -- 场所端名称
    address     char(64)      default ' ',                 -- 安装位置
    mac         char(18)      default ' ',                 -- Mac地址
    apname      char(32),                                  -- AP名称
    apmac       char(18),                                  -- APMac地址
    lrssi       int  default 0,                            -- 开始场强
    urssi       int  default 0,                            -- 离开场强
    ssid        char(64)      default ' ',                 -- SSID,若多个用逗号分隔    
    plate       char(32)      default ' ',                 -- 车辆编号
    line        char(32)      default ' ',                 -- 交通线路名称
    mapid       char(32)      default ' ',                 -- 地图标识(若有值存在，经度维度为想，x,y坐标
    longitude   char(16)      default ' ',                 -- 经度
    latitude    char(16)      default ' ',                 -- 维度
    sendtime         int unsigned default 0,
    starttime        int unsigned default 0,               -- 进入时间
    endtime          int unsigned default 0,               -- 离开时间
    flag             int default 0                         -- 0--进入  1--离开
) engine=MyISAM;


-- 控制命令
drop table  if exists ncssyscontrol; 
create table ncssyscontrol (
    sid         int auto_increment primary key,     -- 序列号 
    cfrom       char(16) default ' ',               -- 指令来源
    funname     char(16) default ' ',               -- 执行功能
    stime       int unsigned default 0,             -- 时间
    command     char(255) default ' ',              -- 命令
    timeout     int unsigned default 0,             -- 超时时间
    message     char(255) default ' ',              -- 信息
    checksum    char(32)   default ' '              -- md5(time+funname)
 ) engine=MyISAM;

-- 状态统计表
drop table  if exists ncssyscontrol; 
create table ncssyscontrol (
    sid         int auto_increment primary key,     -- 序列号 
    cfrom       char(16) default ' ',               -- 指令来源
    funname     char(16) default ' ',               -- 执行功能
    stime       int unsigned default 0,             -- 时间
    command     char(255) default ' ',              -- 命令
    timeout     int unsigned default 0,             -- 超时时间
    message     char(255) default ' ',              -- 信息
    checksum    char(32)   default ' '              -- md5(time+funname)
 ) engine=MyISAM;
-- 统计上网场所人数、手机用户数、手机号非法、证件号非法数、MAC地址非法数
drop table if exists ncsservicestat;       -- 场所统计
create   table ncsservicestat (
    sid        int auto_increment primary key,     -- 序列号 
    stime      int unsigned,  -- 记录时间
    servicecode char(20),    -- 场所编码
    ucount     int unsigned default 0,             --   上网记录数
    mobcount     int unsigned default 0,           --    手机号记录数
    idcount    int unsigned default 0,             --    身份证号记录数
    mcount_e   int unsigned default 0,             --    手机号非法数
    idcount_e  int unsigned default 0,             --    身份证号非法数
    maccount_e int unsigned default 0              --    mac地址非法数
) ENGINE=MyISAM;
create index ncsservicestat_1 on ncsservicestat (stime);
create index ncsservicestat_2 on ncsservicestat (servicecode);


-- AP信息描述, 在原有基础上有所修改 2015/04
drop table if exists ncaplist;
create table if not exists ncaplist (
    apname      char(21)      primary key,                 -- AP设备编号
    servicecode char(18)      default ' ',                 -- 场所端编码
    servicename char(64)      default ' ',                 -- 场所端名称
    did         int  unsigned default 0,                   -- 场所端ID
    version     char(16)      default ' ',                 -- 版本号
    model       char(16)      default ' ',                 -- 设备型号
    devid       char(32)      default ' ',                 -- 设备ID
    ip          char(16)      default ' ',                 -- IP地址
    mac         char(64)      default ' ',                 -- AP MAC地址,若多个用逗号分隔,一个AP可能会对应多个MAC地址
    ssid        char(64)      default ' ',                 -- SSID,若多个用逗号分隔    
    address     char(64)      default ' ',                 -- 安装位置
    plate       char(32)      default ' ',                 -- 车辆编号,车牌号码
    line        char(32)      default ' ',                 -- 交通线路名称，地铁线路
    mapid       char(32)      default ' ',                 -- 地图标识(若有值存在，经度维度为想，x,y坐标
    longitude   char(11)      default ' ',                 -- 经度
    latitude    char(11)      default ' ',                 -- 维度
    starttime   int unsigned  default 0,                   -- 开始时间
    lasttime    int unsigned  default 0,                   -- 最后使用时间
    bpflasttime int unsigned  default 0,                   -- Bpf最后时间
    cursrvbpfrule char(255)   default 0,                   -- Bpf最后同步规则
    flags       int default 1                              -- 0--不用  1--正常
);
