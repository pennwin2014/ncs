strLocation = "Down";
function getNowDate()
{
   var nn=new Date();
   year1=nn.getYear();
   mon1=nn.getMonth()+1;
   date1=nn.getDate();
   var monstr1;
   var datestr1
   if(mon1<10) 
    monstr1="0"+mon1;
   else
    monstr1=""+mon1;
     
   if(date1<10) 
     datestr1="0"+date1;
   else
     datestr1=""+date1;
   return year1+"/"+monstr1+"/"+datestr1;
}
function getlastmonthDate()
{
   var nn=new Date();
   year1=nn.getYear();
   mon1=nn.getMonth();
   date1=nn.getDate();
   var monstr1;
   var datestr1
   if(mon1<10) 
    monstr1="0"+mon1;
   else
    monstr1=""+mon1;
     
   if(date1<10) 
     datestr1="0"+date1;
   else
     datestr1=""+date1;
   return year1+"/"+monstr1+"/"+datestr1;
}
function getlastweekDate()
{
   var nn=new Date();
   year1=nn.getYear();
   mon1=nn.getMonth()+1;
   date1=nn.getDate();
   
   var mm=new Date(year1,mon1-1,date1);
   var tmp1=new Date(2000,1,1);
   var tmp2=new Date(2000,1,8);
   var ne=tmp2-tmp1;
   var mm2=new Date();
   mm2.setTime(mm.getTime()-ne);
   
   
   year2=mm2.getYear();
   mon2=mm2.getMonth()+1;
   date2=mm2.getDate();
    
    
     if(mon2<10) 
    monstr2="0"+mon2;
   else
    monstr2=""+mon2;
     
   if(date2<10) 
     datestr2="0"+date2;
   else
     datestr2=""+date2;
 
   
    return year2+"/"+monstr2+"/"+datestr2;
}


var gdCtrl = new Object();
var goSelectTag = new Array();
var gcGray   = "#808080";
var gcToggle = "#FB8664";
var gcBG = "#e5e6ec";
var previousObject = null;

var gdCurDate = new Date();
var giYear = gdCurDate.getFullYear();
var giMonth = gdCurDate.getMonth()+1;
var giDay = gdCurDate.getDate();

function fSetDate(iYear, iMonth, iDay){
  VicPopCal.style.visibility = "hidden";
  if ((iYear == 0) && (iMonth == 0) && (iDay == 0)){
  	gdCtrl.value = "";
  }else{
  	iMonth = iMonth + 100 + "";
  	iMonth = iMonth.substring(1);
  	iDay   = iDay + 100 + "";
  	iDay   = iDay.substring(1);
  	if(gdCtrl.tagName == "INPUT"){
  	  	gdCtrl.value = iYear+"/"+iMonth+"/"+iDay;
  	}else{
  	  	gdCtrl.innerText = iYear+"/"+iMonth+"/"+iDay;
  	}
  }
  
  for (i in goSelectTag)
  	goSelectTag[i].style.visibility = "visible";
  goSelectTag.length = 0;
  
  window.returnValue=gdCtrl.value;
  //window.close();

}

function HiddenDiv()
{
	var i;
  VicPopCal.style.visibility = "hidden";
  for (i in goSelectTag)
  	goSelectTag[i].style.visibility = "visible";
  goSelectTag.length = 0;

}
function fSetSelected(aCell){
  var iOffset = 0;
  var iYear = parseInt(tbSelYear.value);
  var iMonth = parseInt(tbSelMonth.value);
  
  aCell.bgColor = gcBG;
  with (aCell.children["cellText"]){
  	var iDay = parseInt(innerText);
  	if (color==gcGray)
		iOffset = (Victor<10)?-1:1;
	iMonth += iOffset;
	if (iMonth<1) {
		iYear--;
		iMonth = 12;
	}else if (iMonth>12){
		iYear++;
		iMonth = 1;
	}
  }
  fSetDate(iYear, iMonth, iDay);
}

function Point(iX, iY){
	this.x = iX;
	this.y = iY;
}

function fBuildCal(iYear, iMonth) {
  var aMonth=new Array();
  for(i=1;i<7;i++)
  	aMonth[i]=new Array(i);
  
  var dCalDate=new Date(iYear, iMonth-1, 1);
  var iDayOfFirst=dCalDate.getDay();
  var iDaysInMonth=new Date(iYear, iMonth, 0).getDate();
  var iOffsetLast=new Date(iYear, iMonth-1, 0).getDate()-iDayOfFirst+1;
  var iDate = 1;
  var iNext = 1;

  for (d = 0; d < 7; d++)
	aMonth[1][d] = (d<iDayOfFirst)?-(iOffsetLast+d):iDate++;
  for (w = 2; w < 7; w++)
  	for (d = 0; d < 7; d++)
		aMonth[w][d] = (iDate<=iDaysInMonth)?iDate++:-(iNext++);
  return aMonth;
}

function fDrawCal(iYear, iMonth, iCellHeight, sDateTextSize) {
  var WeekDay = new Array("日","一","二","三","四","五","六");
  var styleTD = " bgcolor='"+gcBG+"' bordercolor='"+gcBG+"' valign='middle' align='center' height='"+iCellHeight+"' style='font:bold arial "+sDateTextSize+";";            //Coded by Liming Weng(Victor Won) email:victorwon@sina.com

  with (document) {
	write("<tr>");
	for(i=0; i<7; i++){
		write("<td "+styleTD+"color:#990099' >"+ WeekDay[i] + "</td>");
	}
	write("</tr>");

  	for (w = 1; w < 7; w++) {
		write("<tr>");
		for (d = 0; d < 7; d++) {
			write("<td id=calCell "+styleTD+"cursor:hand;' onMouseOver='this.bgColor=gcToggle' onMouseOut='this.bgColor=gcBG' onclick='fSetSelected(this)'>");
			write("<font id=cellText Victor='Liming Weng'> </font>");			
			write("</td>")
		}
		write("</tr>");
	}
  }
}

function fUpdateCal(iYear, iMonth) {
  myMonth = fBuildCal(iYear, iMonth);
  var i = 0;
  for (w = 0; w < 6; w++)
	for (d = 0; d < 7; d++)
		with (cellText[(7*w)+d]) {
			Victor = i++;
			if (myMonth[w+1][d]<0) {
				color = gcGray;
				innerText = -myMonth[w+1][d];
			}else{
				color = ((d==0)||(d==6))?"red":"black";
				innerText = myMonth[w+1][d];
			}
		}
}

function fSetYearMon(iYear, iMon){
  tbSelMonth.options[iMon-1].selected = true;
  for (i = 0; i < tbSelYear.length; i++)
	if (tbSelYear.options[i].value == iYear)
		tbSelYear.options[i].selected = true;
  fUpdateCal(iYear, iMon);
}

function fPrevMonth(){
  var iMon = tbSelMonth.value;
  var iYear = tbSelYear.value;
  
  if (--iMon<1) {
	  iMon = 12;
	  iYear--;
  }
  
  fSetYearMon(iYear, iMon);
}

function fNextMonth(){
  var iMon = tbSelMonth.value;
  var iYear = tbSelYear.value;
  
  if (++iMon>12) {
	  iMon = 1;
	  iYear++;
  }
  
  fSetYearMon(iYear, iMon);
}

function fToggleTags(){
  with (document.all.tags("SELECT")){

 	for (i=0; i<length; i++)
 		if ((item(i).Victor!="Won")&&fTagInBound(item(i))){
 			item(i).style.visibility = "hidden";

 			goSelectTag[goSelectTag.length] = item(i);
 		}
  }
}

function fTagInBound(aTag){
  with (VicPopCal.style){
  	var l = parseInt(left);
  	var t = parseInt(top);
  	var r = l+parseInt(width);
  	var b = t+parseInt(height);
	var ptLT = fGetXY(aTag);
	return !((ptLT.x>r)||(ptLT.x+aTag.offsetWidth<l)||(ptLT.y>b)||(ptLT.y+aTag.offsetHeight<t));
  }
}

function fGetXY(aTag){
  var oTmp = aTag;
  var pt = new Point(0,0);
  do {
  	pt.x += oTmp.offsetLeft;
  	pt.y += oTmp.offsetTop;
  	oTmp = oTmp.offsetParent;
  } while(oTmp.tagName!="BODY");
  return pt;
}

// Main: popCtrl is the widget beyond which you want this calendar to appear;
//       dateCtrl is the widget into which you want to put the selected date.
// i.e.: <input type="text" name="dc" style="text-align:center" readonly><INPUT type="button" value="V" onclick="fPopCalendar(dc,dc);return false">
function fPopCalendar(popCtrl, dateCtrl,Location,strDate){
	
	strLocation = "Down";
	if(Location!=null&&Location!="")
		strLocation=Location;  


  if (popCtrl == previousObject){
	  	if (VicPopCal.style.visibility == "visible"){
  		HiddenDiv();
  		return true;
  	}
  	
  }

  previousObject = popCtrl;
  gdCtrl = dateCtrl;
  fInitialDate(strDate);
  fSetYearMon(giYear, giMonth); 
  var point = fGetXY(popCtrl);
  with (VicPopCal.style) {

	if(strLocation=="Up")
	{
		left = point.x;
		top  = point.y-210;
	}	
	else if(strLocation=="Down")
	{
		left = point.x;
		top  = point.y+popCtrl.offsetHeight;
	}
	else if(strLocation=="Left")
	{
		left = point.x-210;
		top  = point.y;
	}
	else if(strLocation=="Right")
	{
		left = point.x+90;
		top  = point.y;
	}
	else if(strLocation=="Self")
	{
		left = point.x-60;
		top  = point.y-100;
	}
	else if(strLocation=="LeftUp")
	{
		left = point.x-210;
		top  = point.y-155;
	}
	else if(strLocation=="LeftSelf")
	{
		left = point.x-210;
		top  = point.y-100;
	}
	width = VicPopCal.offsetWidth;
	width = 210; // Added by Danian Zhang/SUI

	height = VicPopCal.offsetHeight;
	

	fToggleTags(point); 	

	visibility = 'visible';
  }
}

// Added by Danian Zhang/SUI
function fInitialDate(strDate){
	if( strDate == null || strDate.length != 10 )
		return false;

	var sYear  = strDate.substring(0,4);
	var sMonth = strDate.substring(5,7);
	var sDay   = strDate.substring(8,10);

	if( sMonth.charAt(0) == '0' ) { sMonth = sMonth.substring(1,2); }
	if( sDay.charAt(0)   == '0' ) { sDay   = sDay.substring(1,2);   }

	var nYear  = parseInt(sYear );
	var nMonth = parseInt(sMonth);
	var nDay   = parseInt(sDay  );
	
	if ( isNaN(nYear ) )	return false;
	if ( isNaN(nMonth) )	return false;
	if ( isNaN(nDay  ) )	return false;

	var arrMon = new Array(12);
	arrMon[ 0] = 31;	arrMon[ 1] = nYear % 4 == 0 ? 29:28;
	arrMon[ 2] = 31;	arrMon[ 3] = 30;
	arrMon[ 4] = 31;	arrMon[ 5] = 30;
	arrMon[ 6] = 31;	arrMon[ 7] = 31;
	arrMon[ 8] = 30;	arrMon[ 9] = 31;
	arrMon[10] = 30;	arrMon[11] = 31;

	if ( nYear  < 1900 || nYear > 2100 )			return false;
	if ( nMonth < 1 || nMonth > 12 )				return false;
	if ( nDay < 1 || nDay > arrMon[nMonth - 1] )	return false;

	giYear  = nYear;
	giMonth = nMonth;
	giDay   = nDay;
	return true;
}

var gMonths = new Array("一月","二月","三月","四月","五月","六月","七月","八月","九月","十月","十一月","十二月");

with (document) {
write("<Div id='VicPopCal' style='OVERFLOW:hidden;POSITION:absolute;VISIBILITY:hidden;border:2px ridge;z-index:100;width=100'>");
write("<table border='0' bgcolor='#cccccc' width='100%'>");
write("<TR>");
write("<td valign='middle' align='center'><input type='button' name='PrevMonth' value='<' style='height:20;width:20;FONT:bold' onClick='fPrevMonth()'>");
write("&nbsp;<SELECT name='tbSelYear' onChange='fUpdateCal(tbSelYear.value, tbSelMonth.value)' Victor='Won'>");
for(i=1950;i<2015;i++)
	write("<OPTION value='"+i+"'>"+i+" 年</OPTION>");
write("</SELECT>");
write("&nbsp;<select name='tbSelMonth' onChange='fUpdateCal(tbSelYear.value, tbSelMonth.value)' Victor='Won'>");
for (i=0; i<12; i++)
	write("<option value='"+(i+1)+"'>"+gMonths[i]+"</option>");
write("</SELECT>");
write("&nbsp;<input type='button' name='PrevMonth' value='>' style='height:20;width:20;FONT:bold' onclick='fNextMonth()'>");
write("</td>");
write("</TR><TR>");
write("<td align='center'>");
write("<DIV style='background-color:teal'><table width='100%' border='0'>");
fDrawCal(giYear, giMonth, 20, '12');
write("</table></DIV>");
write("</td>");
write("</TR><TR><TD align='center'>");
write("<TABLE width='100%'><TR><TD align='center'>");
write("<B style='cursor:hand' onclick='fSetDate(0,0,0)' onMouseOver='this.style.color=gcToggle' onMouseOut='this.style.color=0'>清空</B>");
write("</td><td algin='center'>");
write("<B style='cursor:hand' onclick='fSetDate(giYear,giMonth,giDay)' onMouseOver='this.style.color=gcToggle' onMouseOut='this.style.color=0'>今天: "+giYear+"-"+giMonth+"-"+giDay+"</B>");
write("</td></tr></table>");
write("</TD></TR>");
write("</TABLE></Div>");
}

function DateFormatUtil(dateValue)
{
	if(dateValue<10)
		dateFormatValue="0"+dateValue;
	else
		dateFormatValue=dateValue;
	return dateFormatValue;			
}


function SelectCalendarUtil(selectValue,BeginTime,EndTime)
{
	var now=new Date();
   	nowYear=now.getYear();
   	nowMonth=now.getMonth()+1;
   	nowDate=now.getDate();
   	
   	EndTime.value=nowYear+"/"+DateFormatUtil(nowMonth)+"/"+DateFormatUtil(nowDate);
   	
   	var mm=new Date(nowYear,nowMonth-1,nowDate);
   	var tmp1=new Date(2000,1,1);
   	
   	if(selectValue==1)
   		var tmp2=new Date(2000,1,1);
   	if(selectValue==2)
   		var tmp2=new Date(2000,1,2);
	if(selectValue==3)
   		var tmp2=new Date(2000,1,3);	   		
   	if(selectValue==4)
   		var tmp2=new Date(2000,1,8);
   	if(selectValue==5)
   		var tmp2=new Date(2000,1,16);	
	if(selectValue==6)
   		var tmp2=new Date(2000,2,1);	   		
	if(selectValue==7)
   		var tmp2=new Date(2000,2,14);	   		
	if(selectValue==8)
   		var tmp2=new Date(2000,3,1);	   		
	if(selectValue==9)
   		var tmp2=new Date(2000,4,1);
   	if(selectValue==10)
   		var tmp2=new Date(2000,6,1);
   	if(selectValue==11)
   		var tmp2=new Date(2001,1,1);	   		
   		
   	var ne=tmp2-tmp1;
   	
   	var begintime=new Date();
   	begintime.setTime(mm.getTime()-ne);
   	
   	beginYear=begintime.getYear();
   	beginMonth=begintime.getMonth()+1;
   	beginDate=begintime.getDate();
   	
	BeginTime.value=beginYear+"/"+DateFormatUtil(beginMonth)+"/"+DateFormatUtil(beginDate);
   	
   	if(selectValue==0)
   	{
   		BeginTime.value="";
   		EndTime.value="";	
   	}
   	
}

function SelectAll(values,selectAll)
{
	if ((selectAll.checked==false))
	{   
		for (i=0 ; i<  values.length; i++)
		{            	   
			values[i].checked=false;
		}
	} 
	if ((selectAll.checked==true))
	{   
		for (i=0 ; i<  values.length; i++)
		{            	   
			values[i].checked=true;
		}
	}                 
}

function SelectSeveral(values,selectCortrol,CortrolNum)
{
	if ((selectCortrol.checked==false))
	{   
		for (i=0;i<values.length;i++)
		{
			for(j=0;j<CortrolNum.length;j++)
			{
				if(values[i].value==CortrolNum[j])
					values[i].checked=false;
			}
		}
	} 
	if ((selectCortrol.checked==true))
	{   
		for (i=0;i<values.length;i++)
		{
			for(j=0;j<CortrolNum.length;j++)
			{
				if(values[i].value==CortrolNum[j])
					values[i].checked=true;
			}
		}
	}              
}

function AboutInputCheckBox(values)
{
	var st = "false";
	for(j=0;j<values.length;j++)
	{    	
		if(values[j].checked == true)
		{   	
			st = "true";
		}
	}  
	if(st=="false")
	{
		for (i=0 ; i<values.length; i++)
		{            	   
			values[i].checked=true;
		}    
	}
}    

function getNowWeek(nowDate)
{
	Day=86400000;
	startDate=new Date(year,0,1);
	startDay=startDate.getDay();
	var De=nowDate-startDate;
	s=De/Day-7;
	Weeks=(De/Day-7+startDay)/7+2;
	return parseInt(Weeks);
}    
   
   
   
  