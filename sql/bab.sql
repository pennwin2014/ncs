----伴随行为策略表
drop table if exists ncbabtask;
create table ncbabtask (
    cid          int unsigned  auto_increment primary key,  --  主键
    groupid      int unsigned,     -- 行政区ID,关联用户权限
    starttime    int unsigned,     -- 起始时间
    endtime        int unsigned,     --  结束时间
    taskname     char(64),           -- 任务名称
    taskdesc     char(255),          -- 任务描述
	dids         char(255) default '', -- 伴随分析的场所ID，可以多个,逗号分割 空为全部
    mac          char(20),             -- 主MAC
    opname       char(32),           -- 操作员
    optime       int unsigned,        -- 操作员创建时间
    modtime      int unsigned,        -- 操作员最后更新时间
    reoporttime  int unsigned  default 0,     -- 报表记录更新时间
    updatetime   int unsigned  default 0,      -- 状态更新时间
    flag       int unsigned default 0     -- 任务状态 0-正常 1-失效 2-结束
) engine=InnoDB;

/*
----伴随行为策略场所关系表
drop table if exists ncbabtaskzone;
create table ncbabtaskzone (
	cid          int unsigned,     -- 策略id
    servicecode   char(32)      -- 场所编码
) engine=InnoDB;
create unique index uqidx_ncbabtaskzone_cid_servicecode on ncbabtaskzone(cid,servicecode); 
ALTER TABLE ncbabtaskzone ADD CONSTRAINT fk_ncbabtaskzone_cid FOREIGN KEY (cid) REFERENCES ncbabtask(cid) ON DELETE CASCADE ON UPDATE CASCADE;


----统计各个mac在不同时间段出现次数表前20位
drop table if exists ncbabappearrank;
create table ncbabappearrank (
	cid          int unsigned,     -- 策略id
	mac          char(20),             -- 伴随MAC    
    appearcount   int unsigned default 0  -- 伴随出现次数 
) engine=InnoDB;
create unique index uqidx_ncbabappearrank_cid_mac on ncbabappearrank(cid,mac); 
ALTER TABLE ncbabappearrank ADD CONSTRAINT fk_ncbabappearrank_cid FOREIGN KEY (cid) REFERENCES ncbabtask(cid) ON DELETE CASCADE ON UPDATE CASCADE;


----统计每个场所编码对应的mac的统计信息，即不同mac出现的个数 
drop table if exists ncbabplacecount;
create table ncbabplacecount (
	cid          int unsigned,     -- 策略id
	servicecode   char(32),     -- 场所编码
    maccount   int unsigned default 0  -- 不同mac个数 
) engine=InnoDB;
create unique index uqidx_ncbabplacecount_cid_mac on ncbabplacecount(cid,servicecode); 
ALTER TABLE ncbabplacecount ADD CONSTRAINT fk_ncbabplacecount_cid FOREIGN KEY (cid) REFERENCES ncbabtask(cid) ON DELETE CASCADE ON UPDATE CASCADE;

*/

----统计每个场所在各个时间段。出现有哪几个mac 
drop table if exists ncbabplacetimemac;
create table ncbabplacetimemac (
	cid          int unsigned,     -- 策略id
	servicecode   char(32),     -- 场所编码
	---timestr       char(20),     -- 时间2015080112
    mac   char(20),  -- 伴随mac
	starttime    int unsigned,     -- 起始时间
    endtime        int unsigned,     --  结束时间
) engine=InnoDB;
create unique index uqidx_ncbabplacetimemac_cid_servicecode_timestr_mac on ncbabplacetimemac(cid,servicecode,timestr,mac); 
ALTER TABLE ncbabplacetimemac ADD CONSTRAINT fk_ncbabplacetimemac_cid FOREIGN KEY (cid) REFERENCES ncbabtask(cid) ON DELETE CASCADE ON UPDATE CASCADE;
