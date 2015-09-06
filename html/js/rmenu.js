function PopupMouseRightButtonUpMenu()
{
var temp1,temp2,temp4

 if(MouseMenu.style.visibility=='visible') MouseMenu.style.visibility='hidden';
 if (event.srcElement.tagName=='A' || event.srcElement.tagName=='TEXTAREA' || event.srcElement.tagName=='INPUT' || document.selection.type!='None')
    return true;
 else
    {
     if (event.clientX+150 > document.body.clientWidth) MouseMenu.style.left=event.clientX+document.body.scrollLeft-150;
     else MouseMenu.style.left=event.clientX+document.body.scrollLeft;
     if (event.clientY+DivH > document.body.clientHeight) MouseMenu.style.top=event.clientY+document.body.scrollTop-DivH;
     else MouseMenu.style.top=event.clientY+document.body.scrollTop;
     MouseMenu.style.visibility='visible';
    }
return false;
}
function DrawMouseRightButtonUpMenu(){

DivH=2;
//oSelection = document.selection;

var HrStr='<tr><td align=\"center\" valign=\"middle\" height=\"2\"><TABLE border=\"0\" cellpadding=\"0\" cellspacing=\"0\" width=\"128\" height=\"2\"><tr><td height=\"1\" bgcolor=\"buttonshadow\"><\/td><\/tr><tr><td height=\"1\" bgcolor=\"buttonhighlight\"><\/td><\/tr><\/TABLE><\/td><\/tr>';
var MenuItemStr1='<tr><td align=\"center\" valign=\"middle\" height=\"20\"><TABLE border=\"0\" cellpadding=\"0\" cellspacing=\"0\" width=\"132\"><tr><td valign=\"middle\" height=\"16\" class=\"MouseOut\" onMouseOver=\"this.className=\'MouseOver\'\" onMouseOut=\"this.className=\'MouseOut\'\" onclick=\"'
var MenuItemStr2="<\/td><\/tr><\/TABLE><\/td><\/tr>";
var historyMenu=['window.print()\">打印','window.location.reload()\">刷新'];
var SysMenu=[];
		
             //'MouseMenu.style.visibility=\'hidden\';document.execCommand(\'SaveAs\',\'true\')\">另存为 ...',
			//'window.alert("不要!")\">  <div id=\"ttss\"><\/div>',
               //'MouseMenu.style.visibility=\'hidden\';window.print()\">打印',
             //'window.location.reload()\">刷新'

var MenuStr='';
for(i=0;i<historyMenu.length;i++)
   {
    MenuStr+=MenuItemStr1+historyMenu[i]+MenuItemStr2;
    DivH+=20;
   }
MenuStr+=HrStr;
for(i=0;i<arguments.length;i++)
   {
    MenuStr+=MenuItemStr1+arguments[i]+MenuItemStr2;
    DivH+=20;
   }

if(arguments.length>0)
  {
   MenuStr+=HrStr;
   DivH+=2;
  }

for(i=0;i<SysMenu.length;i++)
   {
    MenuStr+=MenuItemStr1+SysMenu[i]+MenuItemStr2;
    DivH+=20;
   }

var aboutMenu=['MouseMenu.style.visibility=\'hidden\';showModalDialog(\'aboutus.htm\', window, \'status:no;center:yes;help:no;minimize:no;maximize:no;border:thick;statusbar:no;dialogWidth:400px;scroll:auto;dialogHeight:280px\')">关于 ...']
MenuStr+=HrStr;
for(i=0;i<aboutMenu.length;i++)
   {
    MenuStr+=MenuItemStr1+aboutMenu[i]+MenuItemStr2;
    DivH+=20;
   }

var MenuTop = '<DIV id=\"MouseMenu\" class=\"div1\" style=\"position:absolute; left:0px; top:0px; width=150;height='+DivH+'; z-index:1; visibility:hidden;\">\n' +
             '<TABLE name=\"ts1\" border=\"0\" cellpadding=\"0\" cellspacing=\"0\" class=\"div2\">\n' +
             '<tr>\n' +
             '<td bgcolor=\"' + MenuBarColor+ '\" width=\"50\" valign=\"bottom\" align=\"center\"  bgcolor=\"buttonface\">\n' +
             '<\/td>\n'+ 
             '<td bgcolor=\"buttonface\">\n'+ 
             '<TABLE name=\"ts2\" border=\"0\" cellpadding=\"0\" cellspacing=\"0\">';
var MenuBottom = '<\/TABLE><\/td><\/tr><\/TABLE><\/DIV>';
document.write(MenuTop+MenuStr+MenuBottom);
//prompt('aa',MenuTop+MenuStr+MenuBottom)
document.body.oncontextmenu=new Function('return PopupMouseRightButtonUpMenu();');
document.body.onclick=new Function('if(event.srcElement.tagName !=\'INPUT\') MouseMenu.style.visibility=\'hidden\'');
document.body.onscroll=new Function('MouseMenu.style.visibility=\'hidden\';');
document.body.onselectstart=new Function('MouseMenu.style.visibility=\'hidden\';');
window.onresizestart=new Function('MouseMenu.style.visibility=\'hidden\';');
}
var NS4 = (document.layers);
var IE4 = (document.all);
var win = window;
var n = 0;
function findInPage(str) {
  var txt, i, found;
  if (str == '')
    return false;
  if (NS4) {
    if (!win.find(str))
      while(win.find(str, false, true))
        n++;
    else
      n++;
    if (n == 0)
      alert('文档搜索完毕。');
  }
  if (IE4) {
    txt = win.document.body.createTextRange();
    for (i = 0; i <= n && (found = txt.findText(str)) != false; i++) {
      txt.moveStart('character', 1);
      txt.moveEnd('textedit');
    }
    if (found) {
      txt.moveStart('character', -1);
      txt.findText(str);
      txt.select();
      txt.scrollIntoView();
      n++;
    }
    else {
      if (n > 0) {
        n = 0;
        findInPage(str);
      }
      else
        alert('文档搜索完毕。');
    }
  }
  return false;
}
