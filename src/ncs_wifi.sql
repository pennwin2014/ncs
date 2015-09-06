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

insert into dsroleattr(roleid,pasid,flags,lright)values(101,1004,0,'dwgl_s');
insert into dsroleattr(roleid,pasid,flags,lright)values(101,1004,0,'ap_s');
insert into dsroleattr(roleid,pasid,flags,lright)values(101,1004,0,'fac_s');
insert into dsroleattr(roleid,pasid,flags,lright)values(101,1004,0,'dwzgl_s');
insert into dsroleattr(roleid,pasid,flags,lright)values(101,1004,0,'cslb_s');

insert into dsroleattr(roleid,pasid,flags,lright)values(101,1004,0,'日志查看');
insert into dsroleattr(roleid,pasid,flags,lright)values(101,1004,0,'authlog_s');
insert into dsroleattr(roleid,pasid,flags,lright)values(101,1004,0,'smslog_s');

insert into dsroleattr(roleid,pasid,flags,lright)values(101,1004,0,'系统管理');
insert into dsroleattr(roleid,pasid,flags,lright)values(101,1004,0,'rightrole');
insert into dsroleattr(roleid,pasid,flags,lright)values(101,1004,0,'adminuser');
insert into dsroleattr(roleid,pasid,flags,lright)values(101,1004,0,'operlog');

insert into dsroleattr(roleid,pasid,flags,lright)values(101,1004,0,'退出系统');
insert into dsroleattr(roleid,pasid,flags,lright)values(101,1004,0,'运行分析');
insert into dsroleattr(roleid,pasid,flags,lright)values(101,1004,0,'sszxl_s');
insert into dsroleattr(roleid,pasid,flags,lright)values(101,1004,0,'lbzxl_s');
insert into dsroleattr(roleid,pasid,flags,lright)values(101,1004,0,'zxdw_s');
insert into dsroleattr(roleid,pasid,flags,lright)values(101,1004,0,'lxdw_s_s');
insert into dsroleattr(roleid,pasid,flags,lright)values(101,1004,0,'whdw_s');
insert into dsroleattr(roleid,pasid,flags,lright)values(101,1004,0,'tydw_s');

insert into dsroleattr(roleid,pasid,flags,lright)values(101,1004,0,'统计报表');
insert into dsroleattr(roleid,pasid,flags,lright)values(101,1004,0,'虚拟身份统计');
insert into dsroleattr(roleid,pasid,flags,lright)values(101,1004,0,'xnsf_s');
insert into dsroleattr(roleid,pasid,flags,lright)values(101,1004,0,'上网人数统计');
insert into dsroleattr(roleid,pasid,flags,lright)values(101,1004,0,'users_s');

insert into dsroleattr(roleid,pasid,flags,lright)values(101,1004,0,'系统设置');
insert into dsroleattr(roleid,pasid,flags,lright)values(101,1004,0,'statusset');
insert into dsroleattr(roleid,pasid,flags,lright)values(101,1004,0,'logsave');
insert into dsroleattr(roleid,pasid,flags,lright)values(101,1004,0,'logftppar');
insert into dsroleattr(roleid,pasid,flags,lright)values(101,1004,0,'warnoffpar');

insert into dsroleattr(roleid,pasid,flags,lright)values(101,1004,0,'接入厂商管理');
insert into dsroleattr(roleid,pasid,flags,lright) values(101,1004,0,'csyxzt');

insert into dsroleattr(roleid,pasid,flags,lright)values(101,1004,0,'终端特征信息');
insert into dsroleattr(roleid,pasid,flags,lright)values(101,1004,0,'热点信息');

insert into dsroleattr(roleid,pasid,flags,lright)values(101,1004,0,'移动AP轨迹');

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


insert into ncsuser_lb(code,name) values('1','旅馆类');
insert into ncsuser_lb(code,name) values('2','图书馆阅览室');
insert into ncsuser_lb(code,name) values('3','电脑培训中心');
insert into ncsuser_lb(code,name) values('4','娱乐场所类');
insert into ncsuser_lb(code,name) values('5','交通枢纽');
insert into ncsuser_lb(code,name) values('6','公共交通工具');

insert into ncsuser_lb(code,name) values('7','餐饮服务场所');
insert into ncsuser_lb(code,name) values('8','金融服务场所');
insert into ncsuser_lb(code,name) values('9','其它');
insert into ncsuser_lb(code,name) values('A','购物场所');
insert into ncsuser_lb(code,name) values('B','公共服务场所');
insert into ncsuser_lb(code,name) values('C','文化服务场所');
insert into ncsuser_lb(code,name) values('D','公共休闲场所');


drop table if exists ncswhlog;       -- 维护记录
create   table ncswhlog (
    sid        int auto_increment primary key,     -- 序列号 
    stime      int unsigned,  -- 记录时间
    servicecode char(20),    -- 场所编码
    servicename char(64),    -- 场所名称
    did        int unsigned,   -- 场所ID
    mesg       char(255),       -- 维护内容
    fcode      char(16),        -- 厂商代码
    wman       char(32),         -- 维护人 
    flag       int default 0    -- 0 恢复正常 1-维护  
) ENGINE=MyISAM;
create index ncswhlog_1 on ncswhlog (stime);
create index ncswhlog_2 on ncswhlog (fcode);

drop table if exists ncsmessage;
create   table ncsmessage (
    sid     int unsigned auto_increment primary key,
    send_time  int unsigned default 0,         -- 发送时间
    message_title  char(255),                  -- 消息标题
    message_content text,                      -- 消息内容
    mfrom char(16),                            -- 发送方
    mto char(16),                               -- 接收方
    flag int default 0                         -- 0 未读 1-已读
) engine=MyISAM;


create table if not exists ncipindex_ga (
 code      char(4)      primary key,
 name      char(30) 
)  engine=MYISAM max_rows=1000000000 avg_row_length=1500;

insert into ncipindex_ga(code,name) value ('01','HTTP协议');
insert into ncipindex_ga(code,name) value ('02','WAP协议');
insert into ncipindex_ga(code,name) value ('03','SMTP协议');
insert into ncipindex_ga(code,name) value ('04','POP3协议');
insert into ncipindex_ga(code,name) value ('05','IMAP协议');
insert into ncipindex_ga(code,name) value ('06','NNTP协议');
insert into ncipindex_ga(code,name) value ('07','FTP协议');
insert into ncipindex_ga(code,name) value ('08','SFTP协议');
insert into ncipindex_ga(code,name) value ('09','TELNET协议');
insert into ncipindex_ga(code,name) value ('10','HTTPS协议');
insert into ncipindex_ga(code,name) value ('11','RSTP协议');
insert into ncipindex_ga(code,name) value ('12','MMS协议');
insert into ncipindex_ga(code,name) value ('13','WEP协议');
insert into ncipindex_ga(code,name) value ('14','WPA协议');
insert into ncipindex_ga(code,name) value ('15','PPTP协议');
insert into ncipindex_ga(code,name) value ('16','L2TP协议');
insert into ncipindex_ga(code,name) value ('17','SOCKS协议');
insert into ncipindex_ga(code,name) value ('18','Compo');