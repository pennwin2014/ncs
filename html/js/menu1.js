function goto_member()
{
	parent.parent.sbpm_top.module.module_id.value=11;
	data_str=window.showModalDialog("./user_data/html/search_member.php","","dialogHeight:210px;dialogWidth:400px;dialogTop:200;dialogLeft:200px;status:no;scroll:no");
	var undefined;
	if(data_str=='' || data_str==undefined){data_str="./user_data/html/member_explore.php";}
	else{data_str="./user_data/html/member_explore.php?search="+data_str;}
	window.open (data_str,'sbpm_right');
}
function goto_manuf()
{
	parent.parent.sbpm_top.module.module_id.value=14;
	data_str=window.showModalDialog("./user_data/html/search_manufacturer.php","","dialogHeight:260px;dialogWidth:360px;dialogTop:200;dialogLeft:200px;status:no;scroll:no");
	var undefined;
	if(data_str=='' || data_str==undefined){data_str="./user_data/manufacturer.php";}
	else{data_str="./user_data/manufacturer.php?search="+data_str;}
	window.open (data_str,'sbpm_right');
}
function goto_client()
{
	parent.parent.sbpm_top.module.module_id.value=15;
	data_str=window.showModalDialog("./user_data/html/search_client.php","","dialogHeight:260px;dialogWidth:360px;dialogTop:200;dialogLeft:200px;status:no;scroll:no");
	var undefined;
	if(data_str=='' || data_str==undefined){data_str="./user_data/client.php";}
	else{data_str="./user_data/client.php?search="+data_str;}
	window.open (data_str,'sbpm_right');
}
function goto_house()
{
	parent.parent.sbpm_top.module.module_id.value=16;
	data_str=window.showModalDialog("./user_data/html/search_warehouse.php","","dialogHeight:170px;dialogWidth:320px;dialogTop:200;dialogLeft:200px;status:no;scroll:no");
	var undefined;
	if(data_str=='' || data_str==undefined){data_str="./user_data/warehouse.php";}
	else{data_str="./user_data/warehouse.php?search="+data_str;}
	window.open (data_str,'sbpm_right');
}
function goto_product()
{
	parent.parent.sbpm_top.module.module_id.value=-1;
	window.open("./user_data/catalog.php","sbpm_right");
}
function goto_product_info()
{
	parent.parent.sbpm_top.module.module_id.value=-1;
	window.open("./user_data/product.php","sbpm_right");
}
function goto_log(tp)
{
	var win="/RUNMIT_PUBLIC/log/log_explore.php?tp="+tp;
	parent.parent.sbpm_top.module.module_id.value=18;
	window.open(win,"sbpm_right");
}
function imgover(img)
{	
	var img2="./image/ico/_"+img.name+".gif";
	img.src=img2;
}
function imgout(img)
{
	var img2="./image/ico/"+img.name+".gif";
	img.src=img2;
}
function hide()	{}
function myvoid()	{}
