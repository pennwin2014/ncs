select ifnull(aa.online,0) as onlinenum, ifnull(bb.total,0) as totalnum,bb.name from  (
	select count(*) as online,name  from ncaplist a left join ncsfactorycod b on (fcode=b.code) where apname in ( '0034CB388648','0034CB483D4D','0034CB387EA6') group by name
) aa   
right join  (
	select count(*) as total,name  from ncaplist a left join ncsfactorycod b on (fcode=b.code)  group by name
) bb 
on aa.name=bb.name;