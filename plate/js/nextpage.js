function nextpage(wewe,tty){
document.write("<table align=left border=0 cellpadding=0 cellspacing=0 class=main width=111><tbody><tr><td width='10%'>")
if (ll==1){document.write("<img src='/images/buyer-06.gif' border=0 height=17 width=35 class='gray1'>")}
else{document.write("<img src='/images/buyer-06.gif' border=0 height=17 width=35 onClick='gotoPage(1);form1.submit()' class='hand2'>")}
document.write("</td><td width='10%'>")
if (ll==1){document.write("<img src='/images/buyer-03.gif' border=0 height=17 width=20 class='gray1'>")}
else{document.write("<img src='/images/buyer-03.gif' border=0 height=17 width=20 onClick='gotoPage("+wewe+"-1);form1.submit()' class='hand2'>")}
document.write("</td><td width='20%'><img src='/images/buyer-08.gif' border=0 height=17 width=32></td><td align=middle background='' width=92><input class=Main id=browsdopage maxlength=3  name=browsdopage size=3 style='FONT-SIZE: 12px; HEIGHT: 18px; TEXT-ALIGN: center; WIDTH: 30px' value="+wewe+" onKeyPress='if (event.keyCode < 48 || event.keyCode > 57) event.returnValue = false;' onBlur='checkme(this)'></td><td width='10%'><img src='/images/buyer-09.gif' border=0 height=17 width=16></td><td width='10%'><img align=middle border=0 height=17 src='/images/buyer-10.gif' width=17 onClick='gotoPage(form1.browsdopage.value);form1.submit()' class='hand2'><img align=middle height=17 src='/images/buyer-11.gif' width=3></td><td width='10%'>")
if (ll==yy){document.write("<img src='/images/buyer-04.gif' border=0 height=17 width=20 class='gray1'>")}
else if (yy==0){document.write("<img src='/images/buyer-04.gif' border=0 height=17 width=20 class='gray1'>")}
else{document.write("<img src='/images/buyer-04.gif' border=0 height=17 width=20 onClick='gotoPage("+wewe+"+1);form1.submit()' class='hand2'>")}
document.write("</td><td width='10%'>")
if (ll==yy){document.write("<img src='/images/buyer-05.gif' border=0 height=17 width=36 class='gray1'>")}
else if (yy==0){document.write("<img src='/images/buyer-05.gif' border=0 height=17 width=36 class='gray1'>")}
else{document.write("<img src='/images/buyer-05.gif' border=0 height=17 width=36 onClick='gotoPage("+tty+");form1.submit()' class='hand2'>")}
document.write("</td></tr></tbody></table>")
}