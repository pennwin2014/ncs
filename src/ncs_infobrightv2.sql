-- 2013/09/18  增加两张表  
-- 2013/05/02  修改
-- Copy From ncs_infobright.sql  2014/04/22
-- 使用Inforbright 数据库管理中心的数据结构
-- iplog    IP访问记录

drop table if exists ncsiplog;
create table ncsiplog (
    servicecode  	char(14)  ,                         -- 场所端编码
    username 		char(32),                           -- 用户登录名
    dispname   		char(32),                           -- 姓名
    mac     		char(18),                           -- mac地址
    position 		char(32)  default ' ' ,             -- 位置 可以是房间号，IP地址，MAC地址等
    idno    		char(18)  default ' ',              -- 证件号
    idtype  		char(3)   default ' ',              -- 证件类型
    did			    int,                                -- 场所ID
    tid             int,                                -- 管理中心的组ID
    uid             int,                                -- 用户ID
    stime       	int ,                     	        -- 开始时间
    sip         	char(15),    		                -- 源IP  
    dip         	char(15),    		                -- 目标IP地址
    dport       	int,                   		        -- 目标端口
    conntime    	int,                         	    -- 连接时间  
    trantype    	smallint,                    	    -- 协议 6:tcp  17:udp
    service     	smallint,              		        -- 服务类别
    usid            bigint default 0,                   -- 登录Sid
    ubytes      	int,                      	        -- 上行字节数 
    dbytes      	int,                      	        -- 下行字节数 
    sid             bigint default 0,                   -- 记录序号
    gid             int  default   0                     -- 部门id
)  ;


-- httplog  Web访问记录
drop table if exists ncshttplog;
create table ncshttplog (
    servicecode  	char(14)  ,                     -- 场所端编码
    username 		char(32)  ,                     -- 用户登录名
    dispname   		char(32)  ,                     -- 姓名
    mac     		char(18)  ,                     -- mac地址
    position 		char(32)  ,                     -- 位置 可以是房间号，IP地址，MAC地址等
    idno    		char(18)  ,                     -- 证件号
    idtype  		char(3),                        -- 证件类型
    did			    int,                            -- 场所ID
    tid             int,                            -- 管理中心的组ID
    uid             int,                            -- 用户ID
    stime       	int,                        	-- 开始时间
    sip         	char(15)  ,                		-- 源IP  
    dip         	char(15)  ,                		-- 目标IP地址
    dport       	int,                   		    -- 目标端口
    conntime    	int,				            -- 连接时间
    host        	char(64)  ,                     -- 主机名称 
    uri         	char(128) ,                     -- Uri    
    title       	char(128) ,                     -- 标题
    termtype        char(32)  ,                     -- 终端类别   (增加)
    os              char(32)  ,                     -- 操作系统   (增加)
    brower          char(32)  ,                     -- 浏览器版本 (增加)
    urlid       	int default 0,                  -- 网址库类别 2005/01/11 新增
    webid           int default 0,                  -- 对应网址ID,可用此关键字查看Url库中对应的说明
    usid            bigint default 0,               -- 登录Sid
    sid             bigint default 0,               -- 记录序号
    gid             int  default   0,               -- 部门id
    bytes       	int                          	-- 字节数 
)  max_rows=20000000000 avg_row_length= 200;


-- 表单
drop table if exists ncsformmon;
create table ncsformmon (
    servicecode  	    char(14)  ,     -- 场所端编码
    username 		    char(32)  ,     -- 用户登录名
    dispname   		    char(32)  ,     -- 姓名
    mac     		    char(18)  ,     -- mac地址
    position 		    char(32)  ,     -- 位置 可以是房间号，IP地址，MAC地址等
    idno    		    char(18)  ,     -- 证件号
    idtype              char(3),        -- 证件类型
    did                 int,            --
    tid             	int,            -- 管理中心的组ID
    uid             	int,            -- 用户ID
    stime               int,           	-- 开始时间
    sip         	    char(15)  , 	-- 源IP  
    dip         	    char(15)  , 	-- 目标IP地址
    host 		        char(64) ,      -- 域名
    url 		        char(64) ,      -- 访问URL
    fileid              bigint default 0,  -- 对应表单文件存放的ID
    asize               int,               -- 附件大小
    msize 		        int,               -- 表单大小
    usid                bigint default 0,
    sid                 bigint default 0,
    ccode 		        int,               -- 编码
    gid                 int  default   0,               -- 部门id
    filenum             int                                            -- 文件序号，0001，0002，0003
);

-- 邮件
drop table if exists ncsmailmon;
create table ncsmailmon (
    servicecode  	char(14)  ,     -- 场所端编码
    username 		char(32)  ,     -- 用户登录名
    dispname   		char(32)  ,     -- 姓名
    mac     		char(18)  ,     -- mac地址
    position 		char(32)  ,     -- 位置 可以是房间号，IP地址，MAC地址等
    idno    		char(18)  ,     -- 证件号
    idtype  		char(3),                        -- 证件类型
    did			        int,
    tid             	int,                        -- 管理中心的组ID
    uid             	int,                        -- 用户ID
    stime       	int,                        	-- 开始时间
    sip         	char(15)  ,                		-- 源IP  
    dip         	char(15)  ,                		-- 目标IP地址
    
    mailfrom            char(255) ,   -- 发件人
    mailto  		char(255) ,       -- 收件人
    mailsub 		char(255) ,       -- 主题
    host 		    char(255) ,       -- 域名(WebMail)
    fileid          bigint default,  -- 对应邮件文件存放的ID
    mailasize 		int,             -- 邮件附件大小
    mailsize 		int,             -- 邮件文件大小
    flags 		    smallint,        -- 邮件类别 1--SMTP  2--POP3  3--WebMail
    usid            bigint default 0,
    sid             bigint default 0,
    ccode 		    smallint,
    gid             int  default   0,               -- 部门id
    filenum         int                                            -- 文件序号
);

-- 即时通讯
drop table if exists ncsimlog;
create table ncsimlog (
    servicecode  	char(14)  ,     -- 场所端编码
    username 		char(32)  ,     -- 用户登录名
    dispname   		char(32)  ,     -- 姓名
    mac     		char(18)  ,     -- mac地址
    position 		char(32)  ,     -- 位置 可以是房间号，IP地址，MAC地址等
    idno    		char(18)  ,     -- 证件号
    idtype  		char(3),                        -- 证件类型
    did             int,
    tid             	int,                            -- 管理中心的组ID
    uid             	int,                            -- 用户ID
    stime       	int,                        	-- 开始时间
    sip         	char(15)  ,                		-- 源IP  
    dip         	char(15)  ,                		-- 目标IP地址
    service     smallint,                           -- 服务类别
    funcode     smallint,                           -- 功能
    num         smallint,                           -- 序号
    mfrom               char(48)  ,                 -- 发件人
    mto                 char(48)  ,                 -- 对方账号
    mesg        char(255) ,                         -- 消息内容
    usid        bigint default 0 ,
    gid             int  default   0,               -- 部门id
    sid         bigint default 0
);

-- BBS
drop table if exists ncsbbsmon;
create table ncsbbsmon (
    servicecode  	char(14)  ,     -- 场所端编码
    username 		char(32)  ,     -- 用户登录名
    dispname   		char(32)  ,     -- 姓名
    mac     		char(18)  ,     -- mac地址
    position 		char(32)  ,     -- 位置 可以是房间号，IP地址，MAC地址等
    idno    		char(18)  ,     -- 证件号
    idtype  		char(3),                        -- 证件类型
    did             int,
    tid             	int,                            -- 管理中心的组ID
    uid             	int,                            -- 用户ID
    stime       	int,                        	-- 开始时间
    sip         	char(15)  ,                		-- 源IP  
    dip         	char(15)  ,                		-- 目标IP地址
    msize           smallint,                       -- BBS大小
    ccode           smallint,
    bbsname         char(48)  ,                     -- BBS名称
    url                 char(128)  ,                -- 访问URL
    upfile              char(48)  ,                 -- 表单文件
    attfile             char(48)  ,                 -- 附件文件
    subject             char(255)  ,                -- 主题
    author              char(48)  ,                 -- 作者
    content 		text  ,                         -- 内容
    usid        bigint default 0,
    gid             int  default   0,               -- 部门id
    sid         bigint default 0 
);

-- 搜索引擎
drop table if exists ncssearchlog;
create table ncssearchlog (
    servicecode  	char(14)  ,     -- 场所端编码
    username 		char(32)  ,     -- 用户登录名
    dispname   		char(32)  ,     -- 姓名
    mac     		char(18)  ,     -- mac地址
    position 		char(32)  ,     -- 位置 可以是房间号，IP地址，MAC地址等
    idno    		char(18)  ,     -- 证件号
    idtype  		char(3),                        -- 证件类型
    did	    		int,
    tid             	int,                            -- 管理中心的组ID
    uid         	int,                            -- 用户ID
    stime       	int,                        	-- 开始时间
    sip         	char(15)  ,                		-- 源IP  
   
    sname             	char(48)  ,     -- 搜索引擎名称
    svalue              char(255) ,     -- 搜索关键字
    usid        bigint default 0,
    gid             int  default   0,               -- 部门id
    sid         bigint default 0
)engine=brighthouse;

-- 虚拟身份上下线
drop table if exists ncsnetlog;
create table ncsnetlog (
    servicecode  	char(14)  ,     -- 场所端编码
    username 		char(32)  ,     -- 用户登录名
    dispname   		char(32)  ,     -- 姓名
    mac     		char(18)  ,     -- mac地址
    position 		char(32)  ,     -- 位置 可以是房间号，IP地址，MAC地址等
    idno    		char(18)  ,     -- 证件号
    idtype  		char(3),                        -- 证件类型
    did			        int,
    tid             	int,                            -- 管理中心的组ID
    uid             	int,                            -- 用户ID
    stime       	int,                        	-- 开始时间
    sip         	char(15)  ,                		-- 源IP  
    dip         	char(15)  ,                		-- 目标IP地址
    netid		    int,                            -- 虚拟身份类别
    csid		    int,
    descr		 char(64) ,
    ntype             	smallint,     			       -- 虚拟身份类型
    name                char(64) ,     -- 虚拟身份名称(账号)
    funcode		    smallint,
    flag		    smallint,
    etime           int,                         	        -- 结束时间
    usid            bigint default 0,
    gid             int  default   0,               -- 部门id
    sid         bigint default 0
);


-- 网络会话日志
drop table if exists ncsnetseslog;
create table ncsnetseslog (
    servicecode  	char(14)  ,     -- 场所端编码
    username 		char(32)  ,     -- 用户登录名
    dispname   		char(32)  ,     -- 姓名
    mac     		char(18)  ,     -- mac地址
    position 		char(32)  ,     -- 位置 可以是房间号，IP地址，MAC地址等
    idno    		char(18)  ,     -- 证件号
    idtype  		char(3),                        -- 证件类型
    did			    int,
    tid             	int,                            -- 管理中心的组ID
    uid             	int,                            -- 用户ID
    stime       	int,                        	-- 开始时间
    sip         	char(15)  ,                		-- 源IP  
    dip         	char(15)  ,                		-- 目标IP地址
    service             smallint,     			--
    funcode		    smallint,
    mesg                char(255) ,     --
    usid              bigint  default 0,
    sid               bigint  default 0,
    gid             int  default   0,               -- 部门id
    contid       	int                        	-- 
) ;

-- 用户上下线
drop table if exists ncsuserlog;
create table ncsuserlog (
    servicecode  	char(14)  ,     -- 场所端编码
    username 		char(32)  ,     -- 用户登录名
    dispname   		char(32)  ,     -- 姓名
    mac     		char(18)  ,     -- mac地址
    position 		char(32)  ,     -- 位置 可以是房间号，IP地址，MAC地址等
    idno    		char(18)  ,     -- 证件号
    idtype  		char(3),                        -- 证件类型
    did			    int,
    tid             int,                        -- 管理中心的组ID
    uid             int,                        -- 用户ID
    stime       	int,                        	-- 开始时间
    sip         	char(15),                		-- 源IP  
    conntime		int,
    etime		    int,                            -- 结束时间 
    sumflow		    bigint,
    flags		    smallint,                       -- 状态 1 上线  2 下线或超时
    usid            bigint default 0,
    sid             bigint default 0,
    gid             int
);

`
-- IPservice

drop table if exists ncsipservice;
create table ncsipservice (
    servicecode  	char(14)  ,     -- 场所端编码
    username 		char(32),     -- 用户登录名
    dispname   		char(32),     -- 姓名
    mac     		char(18),     -- mac地址
    did			    int,
    tid             int,                            -- 管理中心的组ID
    uid             int,                            -- 用户ID
    sdate       	char(10),                     	-- 日期，格式为 yyy/mm/dd
    conntime    int,                         --  连接时间  
    service     smallint unsigned,           --  服务id   
    worktime    smallint default 1,          --  时间 1--上班时间  2--下班时间
    ubytes      bigint,                      --  上行字节数 
    dbytes      bigint,                      --  下行字节数 
    flags       smallint,                     --  记录标志，内部处理用
    sid         bigint default 0,            -- 记录序号    
    gid         int default 0            -- 记录序号    
} ;


drop table if exists ncshttpclass;
create table ncshttpclass (
    servicecode  	char(14)  ,     -- 场所端编码
    username 		char(32),     -- 用户登录名
    dispname   		char(32),     -- 姓名
    mac     		char(18),     -- mac地址
    did			    int,
    tid             int,                            -- 管理中心的组ID
    uid             int,                            -- 用户ID
    sdate       	char(10),                     	-- 日期，格式为 yyy/mm/dd
    cid         int unsigned,                --   网址类别ID  
    bytes       bigint,                      --   流量字节数 
    worktime    smallint default 1,          --  时间 1--上班时间  2--下班时间
    lcount      bigint,                      --  点击数 
    ctime       int,                          --  连接时间
    sid         bigint default 0,            -- 记录序号    
    gid         int default 0            -- 记录序号    
};



-- 虚拟身份库
-- 这个表因为需要修改记录内容，考虑不用brighthours

drop table if exists ncsnetid;
create table ncsnetid_if (
    servicecode  	  char(14)  ,     -- 场所端编码
    username 		  char(32)  ,     -- 用户登录名
    dispname   		  char(32)  ,     -- 姓名
    mac     		  char(18)  ,     -- mac地址
    position 		  char(32)  ,     -- 位置 可以是房间号，IP地址，MAC地址等
    idno    		  char(18)  ,     -- 证件号
    idtype  		  char(3),                        -- 证件类型
    did			      int,
    tid               int,                            -- 管理中心的组ID
    uid               int,                            -- 用户ID
    sip         	  char(15)  ,                	  -- 源IP  
    ntype             int  ,                  --  网络类别(如:qq,MSN,游戏账号等)
    name              char(64) ,      --  网络帐号(如：qq号，msn号等)
    starttime         int ,
    lasttime       	  int ,                   -- 最后操作时间
    ndbsid            bigint default 0                -- 序号 did,uid,name,ntype 生成  (修改) (原来num)
) engine=MyISAM;

create index ncsnetid_if_ind1 on ncsnetid_if (ndbsid);
create index ncsnetid_if_ind2 on ncsnetid_if (ntype);
create index ncsnetid_if_ind3 on ncsnetid_if (tid);
create index ncsnetid_if_ind4 on ncsnetid_if (did);
create index ncsnetid_if_ind5 on ncsnetid_if (lasttime);
create index ncsnetid_if_ind6 on ncsnetid_if (name);
drop table if exists ncsfiles;

-- 文件内容
create table ncsfiles (
    fileid          bigint  primary key,
    fsize           int,
    stime           int,
    content         mediumtext
 )  engine=MyISAM;
 

