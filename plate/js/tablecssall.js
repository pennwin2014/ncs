function clickDbl(t){
if(t=="no"){}
else{
self.location.href=t
}
}
function testid(t){
tid=t
}
function noid(){
tid="no"
}
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

function MM_changeProp(objName,x,theProp,theValue)
{ //v3.0
  	var obj = MM_findObj(objName);
  	if (obj && (theProp.indexOf("style.")==-1 || obj.style)) eval("obj."+theProp+"='"+theValue+"'");
}

function goto(a)
{
//window.open(a,"_self","")
self.location.href= a;
}

function MM_reloadPage(init)
{  //reloads the window if Nav4 resized
  	if (init==true) with (navigator) {if ((appName=="Netscape")&&(parseInt(appVersion)==4)) {
    document.MM_pgW=innerWidth; document.MM_pgH=innerHeight; onresize=MM_reloadPage; }}
  else if (innerWidth!=document.MM_pgW || innerHeight!=document.MM_pgH) location.reload();
}
MM_reloadPage(true);

function mouseovertr2 (o)
{
	var i;
	df=o.style.backgroundColor
	o.style.backgroundColor= "#F2F2F2";
	o.style.borderColor= "#3399ff";
		
	for(i=0; i<o.cells.length; i++)
	{
		o.cells(i).style.color="#000000";
	}	
}

function mouseouttr2(o)
{
	o.style.backgroundColor=df
	for(i=0; i<o.cells.length; i++)
	{
		o.cells(i).style.color="#000000";
	}
}

function mouseovertd2(o)
{
	o.style.borderStyle="solid"
	o.style.borderColor="#000000"
	o.style.color="#ffffff"	
}

function mouseouttd2(o)
{
	o.style.color="#003366"
	o.style.borderColor="#cccccc"
}

function gotoPage(pagenum)
{
    form1.CurPg.value = pagenum;
}


function MM_findObj(n, d) { //v4.0
  var p,i,x;  if(!d) d=document; if((p=n.indexOf("?"))>0&&parent.frames.length) {
    d=parent.frames[n.substring(p+1)].document; n=n.substring(0,p);}
  if(!(x=d[n])&&d.all) x=d.all[n]; for (i=0;!x&&i<d.forms.length;i++) x=d.forms[i][n];
  for(i=0;!x&&d.layers&&i<d.layers.length;i++) x=MM_findObj(n,d.layers[i].document);
  if(!x && document.getElementById) x=document.getElementById(n); return x;
}

function MM_showHideLayers() { //v3.0
  var i,p,v,obj,args=MM_showHideLayers.arguments;
  for (i=0; i<(args.length-2); i+=3) if ((obj=MM_findObj(args[i]))!=null) { v=args[i+2];
    if (obj.style) { obj=obj.style; v=(v=='show')?'visible':(v='hide')?'hidden':v; }
    obj.visibility=v; }
}
function clickandel(t)
{
	var i;
var yyy=0;
	var tbl=document.all.DataTable;
	for(i=1;i<tbl.rows.length;i++)
	{
		if(typeof(tbl.rows[i].cells[0].childNodes[0])!="undefined")
		{
			if(tbl.rows[i].cells[0].childNodes[0].checked)
			{
				yyy=yyy+1
			}
		}
	}
if(yyy>=1){
form1.FunName.value=t;
form1.submit();
return;
}else{
	alert("您没有选择！");
}
}
function clickanedit(t)
{
var i;
var yyy=0;
	var tbl=document.all.DataTable;
	for(i=1;i<tbl.rows.length;i++)
	{
		if(typeof(tbl.rows[i].cells[0].childNodes[0])!="undefined")
		{
			if(tbl.rows[i].cells[0].childNodes[0].checked)
			{
				yyy=yyy+1
			}
		}
	}
if(yyy==1){
form1.FunName.value=t;
form1.submit();
return;
}else if(yyy>1){
alert("您不可以一次修改多条纪录!");
}
else{
	alert("您没有选择！");
}
}
