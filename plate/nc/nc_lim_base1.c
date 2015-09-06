<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=gb2312">
<META HTTP-EQUIV="Expires" CONTENT="0">
<META HTTP-EQUIV="Cache-Control" CONTENT="no-cache, must-revalidate">
<META HTTP-EQUIV="Pragma" CONTENT="no-cache">
<title>Web地址列表</title>
<script>
    function goto(t){
        self.location.href=t;
    }
    
    function setdate(t){
       j=form1.dateid.options.length; 
       for(i=0;i<j;i++) { 
           gv = form1.dateid.options[i].value;
           if(gv == t) {
               form1.dateid.selectedIndex = i;
           }
       }
    }
      
	function setctl(t){
       j=form1.limctl.options.length; 
       for(i=0;i<j;i++) { 
           gv = form1.limctl.options[i].value;
           if(gv == t) {
               form1.limctl.selectedIndex = i;
           }
       }
    }

	function setlistid(t){
       j=form1.listid.options.length; 
	   for(i=0;i<j;i++) { 
           gv = form1.listid.options[i].value;
           if(gv == t) {
               form1.listid.selectedIndex = i;
           }
       }
    }
	  
    function mouseovertr2 (o){
        var i;
        df=o.style.backgroundColor
	    o.style.backgroundColor= "#003399";
	    o.style.borderColor= "#FFFF00";
	    o.style.cursor="hand";	
        for(i=0; i<o.cells.length; i++)
        {
	        o.cells(i).style.color="#ffffff";
        }	
    }

    function showf(sid,num,did,st,et,ctl,ser) {
        form1.sid.value=sid;
        form1.rnum.value=num;
        form1.stime.value=st;
		form1.etime.value=et;
		setdate(did);
		setctl(ctl);
		setlistid(ser);
    }
    
    function mouseouttr2(o){
	    o.style.backgroundColor=df
        for(i=0; i<o.cells.length; i++)
        {
	        o.cells(i).style.color="#000000";
        }
    }

    var dfd1="0"
    function checkme(p)
    {
	    var tt=p.value
	    if (tt>yy)
	    {
		    MM_changeProp('browsdopage','','value',yy,'INPUT/TEXT')
	    }
	    if (tt==0)
	    {
		    MM_changeProp('browsdopage','','value','1','INPUT/TEXT')
	    }
    }

    function allChecked(){
        if(dfd1=="1"){
            dfd1="0"
        }
        else {
             dfd1="1"
        }
	    for(var i=0;i<document.all.DataTable.rows.length;i++)
	    {
		    if(typeof(document.all.DataTable.rows[i].cells[0].childNodes[0])=="object"
				&&document.all.DataTable.rows[i].cells[0].childNodes[0].tagName=="INPUT"
				&&document.all.DataTable.rows[i].cells[0].childNodes[0].type=="checkbox") {
			    if(dfd1=="1"){
  			        document.all.DataTable.rows[i].cells[0].childNodes[0].checked="true";
                }
			    else{
                    document.all.DataTable.rows[i].cells[0].childNodes[0].checked="";
                }
            }
 	    }
    }
<!-- Hide
    function killErrors() {
        return true;
    }
    window.onerror = killErrors;
// -->

</script>

<style type="text/css">
<!--
.ttt {  border-style: outset; border-top-width: 2px; border-right-width: 2px; border-bottom-width: 2px; border-left-width: 2px}
.nnoo {  filter: Gray}
-->
</style>
<link rel="stylesheet" href="/css/css.css" type="text/css">
</head>
<body marginheight=0 leftmargin=0 marginwidth=0 topMargin="0" BGCOLOR="Menu">

<form name="form1" method="post" action="/pronline/Msg"> 
  <table width="100%" border="0" bordercolor="#D4D4D4" bgcolor="#BCBCBC">
    <tr> 
      <td> 
        <table width="100%" border="1" height="18">
          <tr> 
            <td height="26" width="79%"> 
              <input type="hidden" name="FunName" value="ncWebLim_Listbase">
              <input type="hidden" name="flags" value="[#flags#]">
              <input type="hidden" name="sid"   value="[#sid#]">
              <input type="hidden" name="sum"   value="[#sum#]">
              <input type="hidden" name="gtype"   value="[#gtype#]">
              <input type="hidden" name="id"   value="[#id#]">
            </td>
            <td height="26" width="21%"> 
              <input type="submit" name="del" value="删除" >
              <input type="button" name="del2" value="分类管理" >
            </td>
          </tr>
        </table>
      </td>
    </tr>
  </table>
<table border="0" width=100% height=100% cellspacing="0" cellpadding="0" align=center bgcolor='#ffffff'>
  <tr>
      <td width="100%"  valign=top align=center bgcolor="#BBBBBB" height="318"> 
        <table  id="DataTable" width="100%" border="0" cellspacing="0" cellpadding="0" bgcolor="#FFFFFF" >
          <tr  height="21" > 
            <td class="bord2" background="/images/ttt.gif" height="21" width="57"><img src="/images/lline.gif" width="1" height="18" align="absmiddle">&nbsp;&nbsp;<a href="#" onClick="allChecked()" >全选</a> 
            </td>
            <td class="bord2" height="21" background="/images/ttt.gif" width="74">序号</td>
            <td class="bord2" height="21" background="/images/ttt.gif" width="100">控制</td>
            <td class="bord2" height="21" background="/images/ttt.gif" width="135">日期</td>
            <td class="bord2" height="21" background="/images/ttt.gif" width="179"><img src="/images/lline.gif" width="1" height="18" align="absmiddle">&nbsp;时间</td>
            <td class="bord4" height="21" background="/images/ttt.gif" width="458"><img src="/images/lline.gif" width="1" height="18" align="absmiddle">限制内容</td>
          </tr>
          <!-- [#SLPB#] -->
          <tr onMouseOver="mouseovertr2(this)" onMouseOut="mouseouttr2(this)" onclick="showf('[#lsid#]','[#lrnum#]','[#ldid#]','[#lstime#]','[#letime#]','[#lctlid#]' ,'[#llistid#]')"> 
            <td class="bord1" width="57"> 
              <input type="checkbox" name="ch[#lff#][#lnum#]" value="[#lsid#]">
            </td>
            <td class="bord1" width="74">&nbsp;[#lrnum#]</td>
            <td class="bord1" width="100">&nbsp;[#llimctl#]</td>
            <td class="bord1" width="135">&nbsp;[#ldatename#]</td>
            <td class="bord1" width="179">&nbsp;[#lstime#]-[#letime#]</td>
            <td class="bord3" width="458">&nbsp;[#lctldes#]</td>
          </tr>
          <!-- [#SLPE#] -->
          <!-- [#SLPB#] -->
          <tr id=[#snum#]> 
            <td class="bord1" width="57">&nbsp; </td>
            <td class="bord1" width="74">&nbsp;</td>
            <td class="bord1" width="100">&nbsp;</td>
            <td class="bord1" width="135">&nbsp;</td>
            <td class="bord1" width="179">&nbsp;</td>
            <td class="bord3" width="458">&nbsp;</td>
          </tr>
          <!-- [#SLPE#] -->
        </table>
<!--//鼠标显示//-->
              <script language="JavaScript">
<!--
var MenuBarColor ='#cccccc';
DrawMouseRightButtonUpMenu();

</script>
</table>
  </form>
</body>
</html>