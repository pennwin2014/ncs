function sortTable(newsortid,srchwin){
if (newsortid == sortid || newsortid == - sortid) {sortid = -sortid}
else 
sortid = newsortid
setTimeout("outputResults(" + srchwin + ")",200);
return false;
}
function size_format (size){
if (size == "") {return "";}
else if (size == 0) { return "0K" ;}
else
	if (size < 1024) { return "1K"  ;}
    	else 
		if (size < 1048576) { return (parseInt((size/1024 + .5).toString()) + "K") ;}
    			else { return (parseInt(((10 * size/1048576 + .5) / 10).toString()) + " MB") ;}
}

function opensrch(){
wsrch = window.open("","srchwin","resizable,width=625,height=400");
var d = wsrch.document;
if (document.layers || document.all)
	tableheader = fT.desc.replace(/<[^<>]*>/g,"");
else
	tableheader = fT.desc
d.write('<HTML><HEAD><TITLE>Search for documents under: ' + tableheader + '</TITLE>');
d.write('<SCRIPT>function blank(){return "<HTML></HTML>";}</' + 'SCRIPT>');
d.write('<SCRIPT>function header(){return "' + srchheader() + '";}</' + 'SCRIPT>');
d.write('</HEAD>');
d.write('<FRAMESET rows="150,*">');
d.write('<FRAME SRC = "javascript:top.header();" name="srchheaderfrm" >');
d.write('<FRAME SRC="javascript:top.blank();" name="srchbasefrm">');
d.write('</FRAMESET>');
d.write('</HTML>');
d.close();
wsrch.focus();
}
function srchresults(node,path, srchreg, srchsize, srchsizecomp, srchdate){
var i = 0
found = true
if (srchreg) {
	found = srchreg.test(node.desc) 
	}
if (srchsizecomp == "at least")
	{if (node.getUserDef('Size') < srchsize) found = false}
else if (srchsizecomp == "at most")
	{if (node.getUserDef('Size') > srchsize) found = false}

if (node.getUserDef('Modified')*1000 < srchdate) found = false

if (found) {
if (!node.isFolder)
	if (node.openIcon != "")
		tmpIcon = imageArray[node.openIcon].src
	else
		tmpIcon = iNA["d"].src
else
	if (node.closedIcon != "")
		tmpIcon = imageArray[node.closedIcon].src
	else if (node.nodeLevel == 0)
		tmpIcon = iNA["tCF"].src
	else
		tmpIcon = iNA["cF"].src
srchArray[nsrch] = new Object
srchArray[nsrch].icon = tmpIcon
srchArray[nsrch].desc = node.desc
srchArray[nsrch].path = path
srchArray[nsrch].size = node.getUserDef('Size')
srchArray[nsrch].modified = node.getUserDef('Modified')
srchArray[nsrch].URL = node.getUserDef('URL')
nsrch++;
}
for (i=0 ; i < node.nC; i++){
	srchresults(node.c[i],path + node.desc + "/", srchreg, srchsize, srchsizecomp, srchdate)
}

}
function drawresults(f){
srchArray = new Array
nsrch = 0
f = f.form
if (!checkReal(f.elapsed) || !checkReal(f.size) )
	return false;
var srchstr = f.srchtext.value
if (srchstr == "") srchstr = "*"
srchstr = srchstr.replace(/\./,"\\.")
srchstr = srchstr.replace(/\*/g,".*")
var srchreg = new RegExp(srchstr,"i")

var srchsize = f.size.value * 1024
var srchsizecomp = f.sizecomp.value

var srchdate =  f.elapsed.value
var srchscale =  f.timescale.value
curdate = new Date()
if (srchdate != "")
	{
	srchdate = srchdate*60*60*24*1000 
	if (srchscale == "months")
		{
		curmonth = curdate.getMonth()
		if (curmonth == 8 || curmonth == 3 || curmonth == 5 ||curmonth == 10)
			srchdate = srchdate*30
		else if (curmonth == 1)
			srchdate = srchdate*28
		else
			srchdate = srchdate*31
		}
	srchdate = curdate.getTime() - srchdate
	}
else
	srchdate = 0

for (i=0 ; i < fT.nC; i++){
	srchresults(fT.c[i],"", srchreg, srchsize, srchsizecomp, srchdate)
	}
outputResults(true)
return true;
}

function checkReal(f){
if (isNaN(parseFloat(f.value)) && f.value != "")
{
wsrch.alert("You have entered " + f.value + "\nPlease enter a valid integer or real number")
f.value = ""
f.focus()
return false;
}
return true;
}

function srchheader(){
strbuf = "<HTML><BODY>";
strbuf += "<FORM Name='srchfrm'>";
strbuf += "<FONT SIZE=3>";
strbuf += "<TABLE BORDER=0, CELLSPACING = 0 WIDTH = 95%>";
strbuf += "<TR><TH ALIGN='left'>Name: </TH><TD COLSPAN=2><INPUT SIZE=30 NAME='srchtext' VALUE=''></TD>";
strbuf += "<TD><INPUT TYPE='button' VALUE ='Find Now' onClick='return top.opener.drawresults(this)'></TD></TR>";
strbuf += "<TR><TH ALIGN='left'>Modified during previous: </TH><TD><INPUT SIZE = 4 NAME='elapsed' Value=''></TD><TD><SELECT NAME='timescale'><OPTION VALUE='days' SELECTED>days<OPTION VALUE='months'>months</SELECT></TD>"; 
strbuf += "<TR><TH ALIGN='left'>Size: </TH><TD><SELECT NAME='sizecomp'><OPTION VALUE='' SELECTED><OPTION VALUE='at least'>at least<OPTION VALUE='at most'>at most</SELECT></TD>";
strbuf += "<TD><INPUT SIZE = 4 NAME='size' Value='0' >KByte</TD>";
strbuf += "<TD><INPUT TYPE='button' VALUE ='  Close   ' onClick='top.close()'></TD></TR>";
strbuf += "</TABLE>";
strbuf += "</FORM></BODY></HTML>";
return strbuf;
}
function drawTable(folderId){
dirArray = new Array
ndirArray = 0
clickedFolder = indexOfEntries[folderId]
tableheader = clickedFolder.desc
for (i=0 ; i < clickedFolder.nC; i++){
	if (!clickedFolder.c[i].isFolder)
		if (clickedFolder.c[i].openIcon != "")
			tmpIcon = imageArray[clickedFolder.c[i].openIcon].src
		else
			tmpIcon = iNA["d"].src
	else
		if (clickedFolder.c[i].closedIcon != "")
			tmpIcon = imageArray[clickedFolder.c[i].closedIcon].src
		else if (clickedFolder.c[i].nodeLevel == 0)
			tmpIcon = iNA["tCF"].src
		else
			tmpIcon = iNA["cF"].src

	dirArray[ndirArray] = new Object
	dirArray[ndirArray].icon = tmpIcon
	dirArray[ndirArray].desc = clickedFolder.c[i].desc
	dirArray[ndirArray].size = clickedFolder.c[i].getUserDef('Size')
	dirArray[ndirArray].modified = clickedFolder.c[i].getUserDef('Modified')
	dirArray[ndirArray].URL = clickedFolder.c[i].getUserDef('URL')
ndirArray++;
}
outputResults(false)
}
function outputResults(srchwin){
tableArray = new Array
if (srchwin)
	{
strbuf = "<HTML><HEAD></HEAD><BODY><TABLE BORDER=0 CELLSPACING = 0 CELLPADDING = 1 WIDTH = 100%>"
resultDoc = wsrch.frames["srchbasefrm"].window
	ref = "top.opener"
	tableArray = srchArray
	ntableArray = nsrch
	}
else
	{
strbuf = "<HTML><HEAD></HEAD><BODY><FORM><TABLE BORDER=0 CELLSPACING = 0 WIDTH = 100%>"
strbuf += "<TR><TD align = 'left'><I><B>Address: " + tableheader + "</B></I></TD>"
strbuf += "<TD align = 'right'><INPUT TYPE='button' VALUE ='Search' onClick='top.opensrch()'></TD></TR></TABLE></FORM>"
strbuf += "<HR><TABLE BORDER=0 CELLSPACING = 0 CELLPADDING = 1 WIDTH = 100%>"
resultDoc = top.frames[baseFrame].window
	ref = "parent"
	tableArray = dirArray
	ntableArray = ndirArray
}
if (document.layers || document.all) {
	if (sortid == 1) tableArray.sort(function(a,b){if (a.desc.toUpperCase() < b.desc.toUpperCase()) {return -1;} else if (a.desc.toUpperCase() > b.desc.toUpperCase()){return 1;} else {return 0;};})
	else if (sortid == -1) tableArray.sort(function(a,b){if (b.desc.toUpperCase() < a.desc.toUpperCase()) {return -1;} else if (b.desc.toUpperCase() > a.desc.toUpperCase()){return 1;} else {return 0;};})
	else if (sortid == 2) tableArray.sort(function(a,b){if (a.path.toUpperCase() < b.path.toUpperCase()) {return -1;} else if (a.path.toUpperCase() > b.path.toUpperCase()){return 1;} else {return 0;};})
	else if (sortid == -2) tableArray.sort(function(a,b){if (b.path.toUpperCase() < a.path.toUpperCase()) {return -1;} else if (b.path.toUpperCase() > a.path.toUpperCase()){return 1;} else {return 0;};})
	else if (sortid == 3) tableArray.sort(function(a,b){return a.size - b.size;})
	else if (sortid == -3) tableArray.sort(function(a,b){return b.size - a.size;})
	else if (sortid == 4) tableArray.sort(function(a,b){return b.modified -  a.modified;})
	else if (sortid == -4) tableArray.sort(function(a,b){return a.modified -  b.modified;})
	strbuf += "<TR><TH ALIGN=LEFT COLSPAN = 2><A HREF='javascript:;' onClick='return " + ref + ".sortTable(1," + srchwin + "); '>Name</A></TH><TD>&nbsp;</TD>"
	if (srchwin) strbuf += "<TH ALIGN=LEFT><A HREF='javascript:;' onClick='return " + ref + ".sortTable(2," + srchwin + ");'>Path</A></TH><TD>&nbsp;</TD>"
	strbuf += "<TH ALIGN=RIGHT><A HREF='javascript:;' onClick='return " + ref + ".sortTable(3," + srchwin + ");'>Size</A></TH><TD>&nbsp;</TD>"
	strbuf += "<TH ALIGN=LEFT COLSPAN = 6 WIDTH='1%'><A HREF='javascript:;' onClick='return " + ref + ".sortTable(4," + srchwin + ");'>Modified</A></TH></TR>"
}
else
{
	strbuf += "<TR><TH ALIGN=LEFT COLSPAN = 2>Name</TH><TD>&nbsp;</TD>"
	strbuf += "<TH ALIGN=RIGHT>Path</TH><TD>&nbsp;</TD>"
	strbuf += "<TH ALIGN=RIGHT>Size</TH><TD>&nbsp;</TD>"
	strbuf += "<TH ALIGN=LEFT COLSPAN = 6>Modified</TH></TR>"
}
tableFont = "<FONT size = 2 FACE='sans-serif'>" 
for (i=0 ; i < ntableArray; i++){
if (tableArray[i].URL != "")
	{
	linkOpen = "<A HREF='" + rootURL + tableArray[i].URL + "'>"
	linkClose = "</A>"
	}
else
	{
	linkOpen = ""
	linkClose = ""
	}

	strbuf += "<TR><TD ALIGN=LEFT >" + tableFont + linkOpen + "<IMG SRC = '" +  tableArray[i].icon + "'  border=0 align = 'absmiddle' >" + linkClose + "</TD>"
	strbuf += "<TD ALIGN=LEFT  >" + tableFont + linkOpen + tableArray[i].desc + linkClose + "</FONT></TD><TD></TD>"
	if (srchwin) strbuf += "<TD ALIGN=LEFT  >" + tableFont + tableArray[i].path + "</FONT></TD><TD></TD>"
      strbuf += "<TD ALIGN=RIGHT >" + tableFont + size_format(tableArray[i].size) + "</FONT></TD><TD></TD>"
modDate = new Date(parseInt(tableArray[i].modified)*1000)
daystr = Days[modDate.getDay()].substring(0,3) + ","
daynumstr = modDate.getDate()
monstr = Months[modDate.getMonth()].substring(0,3)
yearstr = modDate.getFullYear()
hourstr = modDate.getHours()
if (hourstr == 0) hourstr = "00"
hourstr += ":"
minstr = modDate.getMinutes()
if (minstr == 0) minstr = "00"
     strbuf += "<TD ALIGN=LEFT width = '1%' >" + tableFont + daystr + "</FONT></TD>"
     strbuf += "<TD ALIGN=RIGHT width =  '1%'>" + tableFont + daynumstr + "</FONT></TD>"
     strbuf += "<TD ALIGN=LEFT width = '1%' >" + tableFont + monstr + "</FONT></TD>"
     strbuf += "<TD ALIGN=LEFT width = '1%' >" + tableFont + yearstr + "</FONT></TD>"
     strbuf += "<TD ALIGN=RIGHT width = '1%' >" + tableFont + hourstr + "</FONT></TD>"
     strbuf += "<TD ALIGN=LEFT width =  '1%'>" + tableFont + minstr + "</FONT></TD>"
	strbuf += "</TR>"
}
strbuf +="</TABLE></BODY></HTML>"
resultDoc.document.write(strbuf)
resultDoc.document.close();
if (ntableArray == 0)
	{
	if (srchwin) wsrch.alert("No search results found.\n\n Try again.")
	else alert("No files or directories found.")
	}
}

var wsrch
dirArray = new Array
ndirArray = 0
srchArray = new Array
nsrch = 0
resultHeader = ""
tableheader = ""
sortid = 1

Days = new Array('Sunday', 'Monday', 'Tuesday', 'Wednesday', 'Thursday', 'Friday', 'Saturday');
Months = new Array('January', 'February', 'March', 'April', 'May', 'June', 'July','August', 'September', 'October', 'November', 'December');
clickIconMode = 1
noDocs = true
addImage('binary','binary.gif')
addImage('binaryo','binary.gif')
addImage('txt','text.gif')
addImage('txto','text.gif')
addImage('gif','graphic.gif')
addImage('gifo','graphic.gif')
addImage('jpg','graphic.gif')
addImage('jpgo','graphic.gif')
addImage('jpeg','graphic.gif')
addImage('jpego','graphic.gif')
addImage('pdf','pdf.gif')
addImage('pdfo','pdf.gif')
addImage('htm','ftd.gif')
addImage('htmo','ftdo.gif')
addImage('html','ftd.gif')
addImage('htmlo','ftdo.gif')
