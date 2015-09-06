-- 改变原来的代码
update ncsnettype set ntype=13001 where ntype=4;
update ncsnettype set ntype=13002 where ntype=5;
update ncsnettype set ntype=1001 where ntype=6;
update ncsnettype set ntype=1006 where ntype=1004;
update ncsnettype set ntype=1002 where ntype=10;
update ncsnettype set ntype=1008 where ntype=11;
update ncsnettype set ntype=4002 where ntype=14;
update ncsnettype set ntype=1005 where ntype=1012;



-- 虚拟账号
delete from ncsnettype;
insert into ncsnettype (ntype,name,flags,descr,lasttime) values (2001,'联众游戏',0,'联众游戏',0);
insert into ncsnettype (ntype,name,flags,descr,lasttime) values (2002,'中国游戏',0,'中国游戏',0);
insert into ncsnettype (ntype,name,flags,descr,lasttime) values (2003,'传奇游戏',0,'传奇游戏',0);
insert into ncsnettype (ntype,name,flags,descr,lasttime) values (2004,'边锋游戏',0,'边锋游戏',0);
insert into ncsnettype (ntype,name,flags,descr,lasttime) values (2201,'QQ游戏',0,'QQ游戏',0);
insert into ncsnettype (ntype,name,flags,descr,lasttime) values (2202,'地方游戏',0,'地方游戏',0);
insert into ncsnettype (ntype,name,flags,descr,lasttime) values (2,'游戏账号',0,'游戏账号',0);
insert into ncsnettype (ntype,name,flags,descr,lasttime) values (3,'网站账号',0,'网站账号',0);
insert into ncsnettype (ntype,name,flags,descr,lasttime) values (13001,'SMTP邮件',0,'SMTP邮件',0);
insert into ncsnettype (ntype,name,flags,descr,lasttime) values (13002,'POP3邮件',0,'PoP3邮件',0);
insert into ncsnettype (ntype,name,flags,descr,lasttime) values (1001,'QQ',0,'QQ',0);
insert into ncsnettype (ntype,name,flags,descr,lasttime) values (1006,'雅虎通',0,'雅虎通',0);
insert into ncsnettype (ntype,name,flags,descr,lasttime) values (1002,'MSN',0,'MSN',0);
insert into ncsnettype (ntype,name,flags,descr,lasttime) values (1008,'ICQ',0,'ICQ',0);
insert into ncsnettype (ntype,name,flags,descr,lasttime) values (1005,'新浪UC',0,'新浪UC',0);
insert into ncsnettype (ntype,name,flags,descr,lasttime) values (1007,'网易泡泡',0,'网易泡泡',0);
insert into ncsnettype (ntype,name,flags,descr,lasttime) values (4002,'FTP',0,'FTP',0);


insert into ncsnettype (ntype,name,flags,descr,lasttime) values (1004,'SKYPE',0,'SKYPE',0);
insert into ncsnettype (ntype,name,flags,descr,lasttime) values (1009,'淘宝旺旺',0,'淘宝旺旺',0);
insert into ncsnettype (ntype,name,flags,descr,lasttime) values (1012,'飞信',0,'飞信',0);
insert into ncsnettype (ntype,name,flags,descr,lasttime) values (1013,'baiduHi',0,'baiduHi',0);
insert into ncsnettype (ntype,name,flags,descr,lasttime) values (1014,'GTALK',0,'GTALK',0);
insert into ncsnettype (ntype,name,flags,descr,lasttime) values (1034,'校内通',0,'校内通',0);

insert into ncsnettype (ntype,name,flags,descr,lasttime) values (10001,'盛大帐号',0,'盛大帐号',0);
insert into ncsnettype (ntype,name,flags,descr,lasttime) values (10002,'飞信手机号',0,'飞信手机号',0);

insert into ncsnettype (ntype,name,flags,descr,lasttime) values (20001,'百度帐号',0,'百度帐号',0);
insert into ncsnettype (ntype,name,flags,descr,lasttime) values (20002,'搜狐帐号',0,'搜狐帐号',0);
insert into ncsnettype (ntype,name,flags,descr,lasttime) values (20003,'新浪帐号',0,'新浪帐号',0);
insert into ncsnettype (ntype,name,flags,descr,lasttime) values (20004,'126WEBMAIL帐号',0,'126WEBMAIL帐号',0);

insert into ncsnettype (ntype,name,flags,descr,lasttime) values (20005,'163WEBMAIL帐号',0,'163WEBMAIL帐号',0);
insert into ncsnettype (ntype,name,flags,descr,lasttime) values (20006,'QQ邮箱',0,'QQ邮箱',0);
insert into ncsnettype (ntype,name,flags,descr,lasttime) values (20007,'天涯论坛',0,'天涯论坛',0);
insert into ncsnettype (ntype,name,flags,descr,lasttime) values (20009,'猫扑论坛',0,'猫扑论坛',0);






insert into ncsnettype (ntype,name,flags,descr,lasttime) values (50000,'MAC地址',0,'MAC地址',0);
