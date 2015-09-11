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


----统计每个场所在各个时间段。出现有哪几个mac 
drop table if exists ncbabplacetimemac;
create table ncbabplacetimemac (
	cid          int unsigned,     -- 策略id
	servicecode   char(32),     -- 场所编码
    mac   char(20),  -- 伴随mac
	starttime    int unsigned,     -- 起始时间
    endtime        int unsigned     --  结束时间
) engine=InnoDB;
create unique index uqidx_ncbabplacetimemac_cid_servicecode_timestr_mac on ncbabplacetimemac(cid,servicecode,starttime,endtime,mac); 
ALTER TABLE ncbabplacetimemac ADD CONSTRAINT fk_ncbabplacetimemac_cid FOREIGN KEY (cid) REFERENCES ncbabtask(cid) ON DELETE CASCADE ON UPDATE CASCADE;