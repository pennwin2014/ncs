//------------------------------------------------------------
// Copyright (c) 2003-2004 LeadinSoft. All rights reserved.
// Version 1.0.1
// Ahthor dolphin
//------------------------------------------------------------
var bstSingle  = 0; //Chart边框为单线
var bstDouble  = 1; //Chart边框为双线
var fstSolid  = 0; //Chart背景实心填充
var fstTexture  = 1; //Chart背景材质填充
var fstRegular  = "regular"  //字体：正常
var fstItalic  = "italic"; //字体：斜体
var fstBold   = "bold"; //字体：粗体
var atLeft   = "left"; //Chart标题左对齐
var atCenter  = "center"; //Chart标题居中
var atRight   = "right"; //Chart标题右对齐

//定义 VML Chart 基类
function Graph(){
 this.Text    = new Text();
 this.Border    = new Border();
 this.Width    = 500;
 this.Height    = 320;
 this.Fill    = new Fill();
 this.Legend    = new Legend();
 this.SeriesCollection = [];
 this.Container   = null;
 this.Shadow    = false;
 this.VMLObject   = null;
};

//获取Graph类的一个引用
var _p = Graph.prototype;
//通过基类初始化Chart
_p.initialise = function(){
 if(this.Container == null) return;
 var o;
 //画外框
 var group = document.createElement("v:group");
 group.style.width = this.Width+"pt";
 group.style.height = this.Height+"pt";
 group.coordsize  = this.Width*10 +"," + this.Height*10;
 group.id   = "group1";

 //添加一个背景层
 var vRect = document.createElement("v:rect");
 vRect.style.width = (this.Width*10-100) +"px";
 vRect.style.height = this.Height*10+ "px";
 vRect.coordsize  = "21600,21600";

 group.appendChild(vRect);
 o = vRect;
 //设置边框大小 
 vRect.strokeweight = this.Border.Width;
 //设置边框颜色
 vRect.strokecolor = this.Border.Color;
 
 //设置背景
 if(this.Fill.Style == fstSolid){
  vRect.fillcolor = this.Fill.Color;
 }
 else{
  if(this.Fill.background != null)
   vRect.style.backgroundImage = this.Fill.background;
  else
   vRect.fillcolor = this.Fill.Color;
 }
 //边框是否为双线
 if(this.Border.Style == bstDouble){
  var tmp = document.createElement("v:rect");
  tmp.style.width  = (this.Width*10-300) +"px";
  tmp.style.height = (this.Height*10-200)+ "px";
  tmp.style.top  = "100px";
  tmp.style.left  = "100px";
  tmp.strokecolor  = this.Border.Color;
  if(this.Fill.Style == fstSolid){
   tmp.fillcolor = this.Fill.Color;
  }
  else{
   if(this.Fill.background != null)
    tmp.style.backgroundImage = this.Fill.background;
   else
    tmp.fillcolor = this.Fill.Color;
  }
  var filltmp = document.createElement("v:fill");
  filltmp.type = "Frame";
  tmp.appendChild(filltmp);
  group.appendChild(tmp);
  o = tmp;
 }

 //画标题
 var vCaption = document.createElement("v:textbox");
 vCaption.style.fontSize  = this.Text.Font.Size +"px"; 
 vCaption.style.color  = this.Text.Font.Color;
 vCaption.style.height  = this.Text.Height +"px";
 vCaption.style.fontWeight = this.Text.Font.Style;
 vCaption.innerHTML   = this.Text.Text;
 vCaption.style.textAlign = this.Text.Alignment;

 o.appendChild(vCaption);

 //画阴影
 if(this.Shadow){
  var vShadow = document.createElement("v:shadow");
  vShadow.on  = "t";
  vShadow.type = "single";
  vShadow.color = "graytext";
  vShadow.offset = "4px,4px";
  vRect.appendChild(vShadow);
 }
 
 this.VMLObject = group;
 this.Container.appendChild(group);
};

//画具体图形
_p.draw = function(){
 alert("基类不能够实例化具体数据");
};

//增加序列
_p.addSeries = function(o){
 var iCount = this.SeriesCollection.length;
 if(o.Title == null)
  o.Title    = "Series"+ iCount;
 this.SeriesCollection[iCount] = o;
};

//求数据对象的最大Value
_p.maxs = function(){
 var max = 0;
 for(var i=0; i<this.SeriesCollection.length; i++){
  if(max<this.SeriesCollection[i].max()) max = this.SeriesCollection[i].max();
 }
 return max;
}

//重载Object的toString方法
_p.toString = function(){
 return "oGraph";
};

//定义 VML Chart 边框类
function Border(){
 this.Color = "Black";
 this.Style = bstSingle;
 this.Width = 1;
};

//定义 VML Chart 背景类
function Fill(){
 this.Color  = "White";
 this.background = null;
 this.Style  = fstSolid;
};

//定义 VML Chart 标题类
function Text(){
 this.Alignment = atCenter;
 this.Height  = 24;
 this.Font  = new Font();
 this.Text  = "VML Chart Version 1.0";
};

//定义 VML Chart 字体类
function Font(){
 this.Color = "Black";
 this.Family = "Arial";
 this.Size = 12;
 this.Style = fstRegular;
};

//定义 VML Chart 图例类
function Legend(){
 this.Font = new Font();
};

//定义 VML Chart 序列类
function Series(){
 this.Color = Series.getColor();
 this.Title  = null;
 this.all = [];
};
//随机获取一种颜色
Series.getColor = function(){
 return "rgb("+Math.round(Math.random()*255)+","+Math.round(Math.random()*255)+","+Math.round(Math.random()*255)+")";
};
var _p = Series.prototype;
//增加具体数据
_p.addData = function(sName,sValue,sHref,sTooltipText){
 var oData = new Object();
 oData.Name = sName;
 oData.Value = sValue;
 oData.Href = sHref;
 if(sTooltipText == null || sTooltipText == "undefined")
  oData.TooltipText="本项数值为："+ sValue;
 else
  oData.TooltipText = sTooltipText;
 var iCount=this.all.length;
 this.all[iCount] = oData;
};

//求数据对象的最大Value
_p.max = function(){
 var max = 0;
 for(var i=0; i<this.all.length; i++){
  if(this.all[i].Value > max) max = this.all[i].Value;
 }
 return max;
}

//重载Object的toString方法
_p.toString = function(){
 return "oSeries";
};

//定义 VML Chart 时间序列类
function TimeSeries(){
 Series.call(this);
};
var _p = TimeSeries.prototype = new Series;
//增加具体数据
_p.addData = function(sTime,sValue,sType,sHref,sTooltipText){
 var oData = new Object();
 var dt = new Date(eval(sTime*1000));
 if(sType == "Minute"){
  oData.Name = dt.getHours() +":"+ dt.getMinutes();
 }
 else if(sType == "Hour"){
  oData.Name = dt.getHours();
 }
 else if(sType == "Day"){
  oData.Name = eval(dt.getMonth()+1) +"月"+ dt.getDate() +"日";
 }
 else if(sType == "Month"){
  oData.Name = dt.getYear() +"年"+ eval(dt.getMonth()+1)+ "月";
 }
 else{
  oData.Name = dt.getYear() +"年"
 }
 oData.Value = sValue;
 oData.Href = sHref;
 oData.TooltipText = "本项数值为："+ sValue + ", 时间："+ dt.getYear() +"年"+ eval(dt.getMonth()+1)+ "月"+ dt.getDate() +"日 "+ dt.getHours() +":"+ dt.getMinutes() +":"+ dt.getSeconds();
 var iCount=this.all.length;
 this.all[iCount] = oData;
};

//重载Object的toString方法
_p.toString = function(){
 return "oTimeSeries";
};

//定义 VML Chart 坐标轴类
function Axis(){
 this.Color = "Black";
 this.Ln  = 0;
 this.NumberFormat = 0;
 this.Prefix = null;
 this.suffix = null;
 this.Spacing= 30;
 this.Width = 0;
 this.showPoint = 20;
};

//VerticalChart类，继承Graph
function VerticalChart(){
 Graph.call(this);
 //边距
 this.Margin  = new Array(300,100,320,450);
 this.AxisX  = new Axis();
 this.AxisY  = new Axis();
};
var _p = VerticalChart.prototype = new Graph;
//画坐标系
_p.drawCoord = function(oContainer){
 this.AxisY.Ln = eval(this.Height*10 - this.Margin[3]) - this.Margin[1] - 400;
 var vLine = document.createElement("v:line");
 vLine.id = "idCoordY";
 vLine.from = this.Margin[0] +","+ this.Margin[1];
 vLine.to = this.Margin[0] +","+ eval(this.Height*10 - this.Margin[3]);
 vLine.style.zIndex = 8;
 vLine.style.position = "absolute";
 vLine.strokecolor = this.AxisY.Color;
 vLine.strokeweight = 1;

 var vStroke = document.createElement("v:stroke");
 vStroke.StartArrow = "classic";

 vLine.appendChild(vStroke);
 oContainer.appendChild(vLine);

 this.AxisX.Ln = eval(this.Width*10 - this.Margin[0]) - this.Margin[2] - 300;
 var vLine = document.createElement("v:line");
 vLine.id = "idCoordX";
 vLine.from = this.Margin[0] +","+ eval(this.Height*10 - this.Margin[3]);
 vLine.to = eval(this.Width*10 - this.Margin[2]) +","+ eval(this.Height*10 - this.Margin[3]);
 vLine.style.zIndex = 8;
 vLine.style.position = "absolute";
 vLine.strokecolor = this.AxisX.Color
 vLine.strokeweight = 1;

 var vStroke = document.createElement("v:stroke");
 vStroke.EndArrow = "classic";

 vLine.appendChild(vStroke);
 oContainer.appendChild(vLine); 
};

//画X轴刻度
_p.drawLineX = function(oContainer){
 var totalPoint = this.SeriesCollection[0].all.length;
 var iCol  = totalPoint + 1;
 var fColWidth = Math.floor(this.AxisX.Ln/iCol);
 this.AxisX.Width= fColWidth;
 var showPoint = this.AxisX.showPoint,Step = 1;
 var tshowPoint=this.AxisX.showPoint,tStep=1;
 if(totalPoint > showPoint) {
  if(totalPoint < showPoint*2)
   showPoint = Math.round(5*showPoint/5);
  Step = Math.round(totalPoint/showPoint);
  tshowPoint=Math.round(3*showPoint/5);
  tStep=Math.round(totalPoint/tshowPoint);
 }
 else 
 {
 	showPoint = totalPoint;
 	tshowPoint=totalPoint;
}

 this.AxisX.showPoint = showPoint;
 
 var newLine, newStroke, newShape, newText;
 var px,ln;
 var y = eval(this.Height*10 - this.Margin[3]);

 for(var i=1; i<=showPoint; i++){
  ln = i*Step;
  if(ln>totalPoint) break;
  newLine   = document.createElement("v:line");
  px    = this.Margin[0] + (i-1)*fColWidth * Step;
  newLine.from = px +","+ y;
  newLine.to  = px +","+ eval(y + this.AxisX.Spacing);
  newLine.style.zIndex = 8;
  newLine.style.position = "absolute";


  newStroke = document.createElement("<v:stroke color='"+ this.AxisX.Color +"'>");
  newLine.appendChild(newStroke);

  oContainer.appendChild(newLine);

 // newShape= document.createElement("<v:shape style='position:absolute;left:"+ eval(px-80) +";top:"+ eval(y+this.AxisX.Spacing) +";WIDTH:420px;HEIGHT:150px;z-index:8' coordsize='21600,21600' fillcolor='white'></v:shape>");

//  newText = document.createElement("<v:textbox inset='0pt,0pt,0pt,0pt' style='font-size:12px;v-text-anchor:top-right-baseline;color:"+ this.AxisY.Color +"'></v:textbox>");
  
  
//  newText.innerHTML = this.SeriesCollection[0].all[ln-1].Name;
//  newShape.appendChild(newText);

 //oContainer.appendChild(newShape);
 }
 for(var i=1; i<=tshowPoint; i++)
 {
     ln = i*tStep;
     if(ln>totalPoint) break;
      px    = this.Margin[0] + (i-1)*fColWidth * tStep;
    newShape= document.createElement("<v:shape style='position:absolute;left:"+ eval(px+100) +";top:"+ eval(y+this.AxisX.Spacing) +";WIDTH:420px;HEIGHT:150px;z-index:8' coordsize='21600,21600' fillcolor='white'></v:shape>");

    newText = document.createElement("<v:textbox inset='0pt,0pt,0pt,0pt' style='font-size:12px;v-text-anchor:top-right-baseline;color:"+ this.AxisY.Color +"'></v:textbox>");
  
  
   newText.innerHTML = this.SeriesCollection[0].all[ln-1].Name;
   newShape.appendChild(newText);

   oContainer.appendChild(newShape);
}
};

//画Y轴刻度
_p.drawLineY = function(oContainer){
 var maxData = this.maxs();
 maxData  += (4 - maxData % 4)
 var showPoint = this.AxisY.showPoint;
 var dcs = 1;
 for(var i=showPoint; i>0; i--){
  if(maxData % i == 0){
   dcs = i;
   this.AxisY.showPoint = i;
   break;
  }
 }
 
 var newLine, newStroke, newShape, newText;
 var py;
 var x = this.Margin[0];
 var fRowHeight = Math.floor(this.AxisY.Ln/dcs);
 this.AxisY.Width = maxData;  //Y轴时存放最大值

 for(var i=0; i<=dcs; i++){
  py = eval(this.Height*10 - this.Margin[3]) - i*fRowHeight;
  if(i!=0){
   newLine   = document.createElement("v:line");
   newLine.from = eval(x-this.AxisY.Spacing) +","+ py;
   newLine.to  = x +","+ py;
   newLine.style.zIndex = 8;
   newLine.style.position = "absolute";

   newStroke = document.createElement("<v:stroke color='"+ this.AxisY.Color +"'>");
   newLine.appendChild(newStroke);

   oContainer.appendChild(newLine);
  }

  newShape= document.createElement("<v:shape style='position:absolute;left:"+ eval(x-280) +";top:"+ eval(py-50) +";WIDTH:280px;HEIGHT:150px;z-index:8' coordsize='21600,21600' fillcolor='white'></v:shape>");

  newText = document.createElement("<v:textbox inset='0pt,0pt,0pt,0pt' style='font-size:12px;v-text-anchor:top-right-baseline;color:"+ this.AxisY.Color +"'></v:textbox>");
  
  
  newText.innerHTML = i*(maxData/dcs);
  newShape.appendChild(newText);

  oContainer.appendChild(newShape);  
 }
};

//画图例
_p.drawSmallSeries=function(oContainer){
 var arrSeries = this.SeriesCollection;
 for(var i=0; i<arrSeries.length; i++){
  var newRect = document.createElement("v:rect");
  newRect.style.left = eval(this.Width*10 - this.Margin[2]*2) - 200;
  newRect.style.top  = this.Margin[1] + 200 + i*120;
  newRect.style.height = "100px";
  newRect.style.width  = "100px";
  newRect.fillcolor = arrSeries[i].Color;
  newRect.strokeweight="1";
  newRect.strokecolor="white";
  newRect.style.zIndex = 10;
  oContainer.appendChild(newRect);

  var newShape= document.createElement("<v:shape style='position:absolute;left:"+ eval(this.Width*10 - this.Margin[2]*2 - 70) +";top:"+ eval(this.Margin[1] + 200 + i*120) +";WIDTH:600px;HEIGHT:100px;z-index:8' coordsize='21600,21600' fillcolor='white'></v:shape>");

  var newText = document.createElement("<v:textbox inset='0pt,0pt,0pt,0pt' style='font-size:"+this.Legend.Font.Size+"px;v-text-anchor:top-right-baseline;color:"+ this.Legend.Font.Color +";cursor:default' title='"+ arrSeries[i].Title +"'></v:textbox>");

  newText.innerHTML = " "+ arrSeries[i].Title;

  newShape.appendChild(newText);
  oContainer.appendChild(newShape);
 }
};
//------------------------------------------------------------------------------
//竖向柱状图类，继承VerticalChart类
function VerticalBarChart(){
 VerticalChart.call(this);
};
var _p = VerticalBarChart.prototype = new VerticalChart;


//重花X轴刻度
_p.drawLineX = function(oContainer){
 var totalPoint = this.SeriesCollection[0].all.length;
 var iCol  = totalPoint + 1;
 var fColWidth = Math.floor(this.AxisX.Ln/iCol);
 this.AxisX.Width= fColWidth;
 var showPoint = this.AxisX.showPoint,Step = 1;
// if(totalPoint > showPoint) {
//  if(totalPoint < showPoint*2)
//   showPoint = Math.round(3*showPoint/5);
//  Step = Math.round(totalPoint/showPoint);
//}
// else 
 	showPoint = totalPoint;

 this.AxisX.showPoint = showPoint;
 
 var newLine, newStroke, newShape, newText;
 var px,ln;
 var y = eval(this.Height*10 - this.Margin[3]);

 for(var i=1; i<=showPoint; i++){
  ln = i*Step;
  if(ln>totalPoint) break;
  newLine   = document.createElement("v:line");
  px    = this.Margin[0] + i*fColWidth * Step;
  newLine.from = px +","+ y;
  newLine.to  = px +","+ eval(y + this.AxisX.Spacing);
  newLine.style.zIndex = 8;
  newLine.style.position = "absolute";

  newStroke = document.createElement("<v:stroke color='"+ this.AxisY.Color +"'>");
  newLine.appendChild(newStroke);

  oContainer.appendChild(newLine);

  newShape= document.createElement("<v:shape style='position:absolute;left:"+ eval((px-fColWidth/2)-50) +";top:"+ eval(y+this.AxisX.Spacing) +";WIDTH:210px;HEIGHT:400px;z-index:8' coordsize='21600,21600' fillcolor='white'></v:shape>");

  newText = document.createElement("<v:textbox inset='0pt,0pt,0pt,0pt' style='font-size:12px;v-text-anchor:top-right-baseline;color:"+ this.AxisY.Color +"'></v:textbox>");

  newText.innerHTML = this.SeriesCollection[0].all[ln-1].Name;
  newShape.appendChild(newText);

  oContainer.appendChild(newShape);
 }
};

//画VerticalBarChart
_p.draw = function(){
 var oContainer = this.VMLObject;
 this.AxisY.showPoint = 10;
 this.drawCoord(oContainer);
 this.drawLineX(oContainer);
 this.drawLineY(oContainer);
 this.drawSmallSeries(oContainer);
 this.drawBar(oContainer);
};

//画VerticalBarChart的具体数据
_p.drawBar = function(oContainer){
 var arrSeries = this.SeriesCollection;
 var fColWidth,dcs;
 fColWidth = this.AxisX.Width;
 dcs    = this.AxisY.Ln/this.AxisY.Width;
 var iValueLn, iSeriesLn;
 iSeriesLn = arrSeries.length
 var barWidth = fColWidth/(iSeriesLn+1);
 var newShape = null;
 var l,t,barHeight;
 for(var i=0; i<iSeriesLn; i++){
  iValueLn = arrSeries[i].all.length;
  for(var k=0; k<iValueLn; k++){
   barHeight = dcs*eval(arrSeries[i].all[k].Value)
   l = eval(this.Margin[0]+ k*fColWidth + i*barWidth + barWidth/2);
   t = eval(this.Height*10 - this.Margin[3] - barHeight);
   newShape= document.createElement("<v:rect style='position:absolute;left:"+l+";top:"+t+";WIDTH:"+ barWidth + "px;HEIGHT:"+ barHeight +"px;z-index:9;border-width:0' fillcolor='" + arrSeries[i].Color + "' title = '"+ arrSeries[i].all[k].TooltipText +"'></v:rect>");

   //alert(this.AxisX.Width)
   oContainer.appendChild(newShape);
  }
 }
};
//-----------------------------------------------------------------------------
//------------------------------------------------------------------------------
//竖向线状图类，继承VerticalChart类
function VerticalLineChart(){
 VerticalChart.call(this);
 this.isDrawPoint = true;
};
var _p = VerticalLineChart.prototype = new VerticalChart;

//画VerticalLineChart
_p.draw = function(){
 if(this.Border.Style == 1){
  this.Margin = new Array(400,200,400,300);
 }

 var oContainer = this.VMLObject;
 this.AxisY.showPoint = 10;
 this.drawCoord(oContainer);
 this.drawLineX(oContainer);
 this.drawLineY(oContainer);
 this.drawSmallSeries(oContainer);
 this.drawLine(oContainer);
};

//画VerticalLineChart的具体数据
_p.drawLine = function(oContainer){
 var arrSeries = this.SeriesCollection;
 var fColWidth,dcs;
 fColWidth = this.AxisX.Width;
 dcs    = this.AxisY.Ln/this.AxisY.Width;
 var iValueLn, iSeriesLn;
 iSeriesLn = arrSeries.length
 var points = new Array(iSeriesLn);
 var l,t,barHeight;
 for(var i=0; i<iSeriesLn; i++){
  iValueLn = arrSeries[i].all.length;
  points[i] = new Array();
  for(var k=0; k<iValueLn; k++){
   barHeight = dcs*eval(arrSeries[i].all[k].Value)
   l = eval(this.Margin[0]+ k*fColWidth);
   t = eval(this.Height*10 - this.Margin[3] - barHeight);
   points[i][k] = l+","+t;
  }
 }
 
 //画PolyLine
 for(var i=0; i<points.length; i++){
  var newPolyLine = document.createElement("v:polyline");
  newPolyLine.filled = false;
  newPolyLine.style.zIndex = 8;
  newPolyLine.style.position = "absolute";
  newPolyLine.strokecolor = arrSeries[i].Color;
  newPolyLine.strokeweight = "1pt";
  for(var k=0; k<points[i].length; k++){
   if(k==0) newPolyLine.points = points[i][k];
   else newPolyLine.points += " "+ points[i][k];
   
   if(this.isDrawPoint){
    var newOval = document.createElement("v:oval");
    tmp = points[i][k].split(",");
    newOval.style.zIndex = 9;
    newOval.style.position = "absolute";
    newOval.style.left = tmp[0]-20;
    newOval.style.top = tmp[1]-20;
    newOval.style.width = 40;
    newOval.style.height = 40;
    newOval.strokecolor = arrSeries[i].Color;
    newOval.fillcolor = arrSeries[i].Color;
    newOval.title = arrSeries[i].all[k].TooltipText;
    oContainer.appendChild(newOval);
   }
  }
  oContainer.appendChild(newPolyLine);
 }
};

