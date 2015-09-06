//船名-vessel-return 1.ID;2.中文名；3.英文名
//客户-client-return 1.ID;2.中文名；3.英文名;4.中文名称+中文地址+电话+传真;5.英文名称+英文地址+电话+传真
//分供方-supplier-return 1.ID;2.中文名；3.英文名;4.中文名称+中文地址+电话+传真;5.英文名称+英文地址+电话+传真
//单位-unit-return 1.ID;2.中文名；3.英文名
//信用等级-clevel-return 1.ID;2.中文名；3.英文名
//行业-industry-return 1.ID;2.中文名；3.英文名
//客户来源-csource-return 1.ID;2.中文名；3.英文名
//付款方式-pmode-return 1.ID;2.中文名；3.英文名
//超时种类-otreason-return 1.ID;2.中文名；3.英文名
//卡车车类-otreason-return 1.ID;2.中文名；3.英文名
//卡车车型-otreason-return 1.ID;2.中文名；3.英文名
//成本项目-cost_item-return 1.ID;2.中文名；3.英文名
//服务项目-server_item-return 1.ID;2.中文名；3.英文名
//操作项目-ope_item-return 1.ID;2.中文名；3.英文名
//单证项目-doc_item-return 1.ID;2.中文名；3.英文名
//开户行-open_bank-return 1.ID;2.中文名；3.英文名
//业务编号-ope_no-return 1.ID;2.业务编号；3.委托时间；

function selDia(selName)
{
	var wb;
	var objEva=new Array();
	for(var i=1;i<=arguments.length/2;i++)
	{
		switch(arguments[2*i])
		{
			case 'id':
				if(typeof(eval('document.all.'+arguments[2*i-1]))=="object")
					objEva[0]=eval('document.all.'+arguments[2*i-1]);
				break;
			case 'cn':
				if(typeof(eval('document.all.'+arguments[2*i-1]))=="object")
					objEva[1]=eval('document.all.'+arguments[2*i-1]);
				break;
			case 'en':
				if(typeof(eval('document.all.'+arguments[2*i-1]))=="object")
					objEva[2]=eval('document.all.'+arguments[2*i-1]);
				break;
			case 'cd':
				if(typeof(eval('document.all.'+arguments[2*i-1]))=="object")
					objEva[3]=eval('document.all.'+arguments[2*i-1]);
				break;
			case 'ed':
				if(typeof(eval('document.all.'+arguments[2*i-1]))=="object")
					objEva[4]=eval('document.all.'+arguments[2*i-1]);
				break;
		}
	}
	if(selName=="vessel")
	{
		wb="/Wss/Wss_Sel_Dia/dia_vessel.htm";
	}
	else if(selName=="client")
	{
		wb="/Wss/Wss_Sel_Dia/dia_cli.htm";
		objEva[10]="0";
	}
	else if(selName=="supplier")
	{
		wb="/Wss/Wss_Sel_Dia/dia_cli.htm";
		objEva[10]="1";
	}
	else if(selName=="unit")
	{
		wb="/Wss/Wss_Sel_Dia/dia_unit.htm";
	}
	else if(selName=="clevel")
	{
		wb="/Wss/Wss_Sel_Dia/dia_cle.htm";
	}
	else if(selName=="industry")
	{
		wb="/Wss/Wss_Sel_Dia/dia_ind.htm";
	}
	else if(selName=="csource")
	{
		wb="/Wss/Wss_Sel_Dia/dia_cso.htm";
	}
	else if(selName=="pmode")
	{
		wb="/Wss/Wss_Sel_Dia/dia_pmo.htm";
	}
	else if(selName=="otreason")
	{
		wb="/Wss/Wss_Sel_Dia/dia_otr.htm";
	}
	else if(selName=="ttype")
	{
		wb="/Wss/Wss_Sel_Dia/dia_tty.htm";
	}
	else if(selName=="tmode")
	{
		wb="/Wss/Wss_Sel_Dia/dia_trm.htm";
	}
	else if(selName=="cost_item")
	{
		wb="/Wss/Wss_Sel_Dia/dia_cit.htm";
	}
	else if(selName=="server_item")
	{
		wb="/Wss/Wss_Sel_Dia/dia_sit.htm";
	}
	else if(selName=="ope_item")
	{
		wb="/Wss/Wss_Sel_Dia/dia_oit.htm";
	}
	else if(selName=="doc_item")
	{
		wb="/Wss/Wss_Sel_Dia/dia_dit.htm";
	}
	else if(selName=="open_bank")
	{
		wb="/Wss/Wss_Sel_Dia/dia_oab.htm";
	}
	else if(selName=="ope_no")
	{
		wb="/Wss/Wss_Sel_Dia/dia_opn.htm";
	}

	else
		return;
	showModalDialog(wb,objEva,
		 "status:no;center:yes;help:no;minimize:no;maximize:no;border:thick;statusbar:no;dialogWidth:400px;scroll:none;dialogHeight:330px;overflow:none");
}