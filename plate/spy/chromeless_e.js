var Chrome_Collection = new Array ();
var ChromelessWindow_Collection = new Array ();
var Chrome_Browser = new checkBrowser ();

var windowControls_None = 0;
var windowControls_Close = 1;
var windowControls_Maximize = 2;
var windowControls_Minimize = 4;

var windowAnchor_TopLeft = 0;
var windowAnchor_TopRight = 1;
var windowAnchor_TopMiddle  = 2;
var windowAnchor_BottomLeft = 4;
var windowAnchor_BottomRight = 5;
var windowAnchor_BottomMiddle = 6;
var windowAnchor_MiddleLeft = 8;
var windowAnchor_MiddleRight = 9;
var windowAnchor_Center = 10;

function Chrome ()
{
	this.instance = Chrome_Collection.length;
	Chrome_Collection[this.instance] = this;
	this.name = "Chrome_" + this.instance;
	
	this.width = 0;
	this.bgColor = "#FFFFFF";
	this.leftmargin = 0;
	this.rightmargin = 0;
	this.topmargin = 0;
	this.bottommargin = 0;
	
	this.icon = new Image ();
	this.content = "";
	this.windowControls = windowControls_Close;
	
	this.winCloseBut_Norm = new Image ();
	this.winCloseBut_Norm.src = "images/ChromeClose_Norm.gif";
	this.winCloseBut_Over = new Image ();
	this.winCloseBut_Over.src = "images/ChromeClose_Over.gif";
	this.winCloseBut_Down = new Image ();
	this.winCloseBut_Down.src = "images/ChromeClose_Down.gif";
	
	this.winMinBut_Norm = new Image ();
	this.winMinBut_Norm.src = "images/ChromeMin_Norm.gif";
	this.winMinBut_Over = new Image ();
	this.winMinBut_Over.src = "images/ChromeMin_Over.gif";
	this.winMinBut_Down = new Image ();
	this.winMinBut_Down.src = "images/ChromeMin_Down.gif";
	
	this.winMaxBut_Norm = new Image ();
	this.winMaxBut_Norm.src = "images/ChromeMax_Norm.gif";
	this.winMaxBut_Over = new Image ();
	this.winMaxBut_Over.src = "images/ChromeMax_Over.gif";
	this.winMaxBut_Down = new Image ();
	this.winMaxBut_Down.src = "images/ChromeMax_Down.gif";
	
	this.dragable = false;
	this.html = Chrome_Html;
}

function Chrome_Html ()
{
	var html = "";
	
	html += "<html>";
	html += "<head>";
	html += "<script language=\"javascript\">";
	html += "function checkBrowser()";
	html += "{";
	html += "	this.agt=navigator.userAgent.toLowerCase();";
	html += "	this.ver=navigator.appVersion;";
	html += "	this.dom=document.getElementById?1:0;";
	html += "	this.ie5=(this.ver.indexOf(\"MSIE 5\")>-1 && this.dom)?1:0;";
	html += "	this.ie4=(document.all && !this.dom)?1:0;";
	html += "	this.ns5=(this.dom && parseInt(this.ver) >= 5) ?1:0;";
	html += "	this.ns4=(document.layers && !this.dom)?1:0;";
	html += "	this.bw=(this.ie5 || this.ie4 || this.ns4 || this.ns5);";
	html += "	this.mac  = (this.agt.indexOf(\"mac\")!=-1);";
	html += "	this.ie4n5 = (this.ie4 || this.ie5)?1:0;";
	html += "	this.ie4n5mac = (this.ie4n5 && this.mac)?1:0;";
	html += "	return this;";
	html += "}";
	html += "function Browser_ATagClickDisable (browser)";
	html += "{";
	html += "	if (browser.ie4n5)";
	html += "	{";
	html += "		window.event.returnValue = false;";
	html += "	}";
	html += "	return (false);";
	html += "}";
	html += "function Image_Switch (ImageObject, src)";
	html += "{";
	html += "	ImageObject.src = src;";
	html += "}";
	html += "var myBrowser = new checkBrowser ();"
	html += "function dc_mousedown ()";
	html += "{";
	html += "if ((window.event.srcElement.name != \"" + this.name + "_Min\") && (window.event.srcElement.name != \"" + this.name + "_Max\") && (window.event.srcElement.name != \"" + this.name + "_Close\"))";
	html += "{";
	html += "top.Drag_Engage (window.event);";
	html += "}";
	html += "}";
	html += "document.onselectstart=new Function (\"return false;\");";
	html += "document.ondragstart=new Function (\"return false;\");";
	html += "document.onmousemove=new Function (\"return false;\");";
	html += "document.oncontextmenu=new Function (\"return false;\");";
	html += "</script>";
	html += "<style type=\"text/css\">	";
	html += "#iconLayer {z-index:2;	position : absolute;left:0px;top:0px;}";
	html += "#buttonLayer {z-index:3; position : absolute;left:3px;top:0px;}";
	html += "</style>";	
	html += "</head>";
	html += "<body bgcolor=\"" + this.bgColor + "\" leftmargin=\"" + this.leftmargin + "\" rightmargin=\"" + this.rightmargin + "\" topmargin=\"" + this.topmargin + "\" bottommargin=\"" + this.bottommargin + "\"";
	if (this.dragable == true)
	{
		html += " onmousedown=\"dc_mousedown ()\" onmouseup=\"top.Drag_Release ();\" onmousemove=\"top.Drag_Drag (window.event);\"";
	}
	html += ">";
	html += this.content;
	
	if (this.icon.src != "")
	{
		html += "<div id=\"iconLayer\"><img border=\"0\" src=\"" + this.icon.src + "\"></div>";
	}

	if (this.windowControls != windowControls_None)
		{
			if (Chrome_Browser.ver.indexOf("MSIE 5.5")==-1) this.windowControls = (~windowControls_Minimize) & this.windowControls;
			
			html += "<div id=\"buttonLayer\" align=\"right\"><table border=\"0\" cellspacing=\"0\" cellpadding=\"0\" align=\"right\"><tr><td>";
			if (this.windowControls & windowControls_Minimize) html += "<a href=\"javascript://\" onMouseOver=\"Image_Switch (document." + this.name + "_Min, '" + this.winMinBut_Over.src + "');\" onMouseOut=\"Image_Switch (document." + this.name + "_Min, '" + this.winMinBut_Norm.src + "');\" onMouseDown=\"Image_Switch (document." + this.name + "_Min, '" + this.winMinBut_Down.src + "');\" ><img border=\"0\" name=\"" + this.name + "_Min\" src=\"" + this.winMinBut_Norm.src + "\" alt=\"minimize window\"></a>";
			if ((this.windowControls & windowControls_Minimize) && (this.windowControls & windowControls_Maximize)) html += "<img border=\"0\" width=\"1\" height=\"1\" src=\"images/pixel.gif\">";
			if (this.windowControls & windowControls_Maximize) html += "<a href=\"javascript://\" onMouseOver=\"Image_Switch (document." + this.name + "_Max, '" + this.winMaxBut_Over.src + "');\" onMouseOut=\"Image_Switch (document." + this.name + "_Max, '" + this.winMaxBut_Norm.src + "');\" onMouseDown=\"Image_Switch (document." + this.name + "_Max, '" + this.winMaxBut_Down.src + "');\" onClick=\"top.maximize ();Browser_ATagClickDisable (myBrowser);\"><img border=\"0\" name=\"" + this.name + "_Max\" src=\"" + this.winMaxBut_Norm.src + "\" alt=\"maximize window\"></a>";
			if (((this.windowControls & windowControls_Minimize) || (this.windowControls & windowControls_Maximize)) && (this.windowControls & windowControls_Close)) html += "<img border=\"0\" width=\"1\" height=\"1\" src=\"images/pixel.gif\">";
			if (this.windowControls & windowControls_Close) html += "<a href=\"javascript://\" onMouseOver=\"Image_Switch (document." + this.name + "_Close, '" + this.winCloseBut_Over.src + "');\" onMouseOut=\"Image_Switch (document." + this.name + "_Close, '" + this.winCloseBut_Norm.src + "');\" onMouseDown=\"Image_Switch (document." + this.name + "_Close, '" + this.winCloseBut_Down.src + "');\" onClick=\"top.close();Browser_ATagClickDisable (myBrowser);\"><img name=\"" + this.name + "_Close\" border=\"0\" src=\"" + this.winCloseBut_Norm.src + "\" style=\"cursor:default\" alt=\"close window\" ></a>";
			html += "</td></tr></table></div>";
		}
	
	html += "</body>";
	html += "</html>";
	
 	return (html);
}

function Border ()
{
	this.color = "#000000";
	this.width = 1;
}

function ChromelessWindow ()
{
	this.instance = ChromelessWindow_Collection.length;
	ChromelessWindow_Collection[this.instance] = this;
	
	this.window = void 0;
	
	this.x = 50;
	this.y = 200;
	this.width = 800;
	this.height = 600;
	this.title = "";
	this.url = "";
	
	this.ChromeTop = new Chrome ();
	this.ChromeBottom = new Chrome ();
	this.ChromeLeft = new Chrome ();
	this.ChromeRight = new Chrome ();
	
	this.BorderTop = new Border ();
	this.BorderBottom = new Border ();
	this.BorderLeft = new Border ();
	this.BorderRight = new Border ();
	
	this.open = ChromelessWindow_Open;
	this.close = ChromelessWindow_Close;
	this.resize = ChromelessWindow_Resize;
}

function ChromelessWindow_Open ()
{
	var html = "";
	
	if ((this.window == void 0) || (this.window.closed))
	{
		if ((Chrome_Browser.ie4n5) && (!Chrome_Browser.mac))
		{
			html += "<html>";
			html += "<head>";
			html += "<title>" + this.title + "</title>";
			html += "</head>";
			html += "<script language=\"javascript\">";
			html += "var maximized = false;";
			html += "var minimized = false;";
			html += "var x = " + this.x + ";";
			html += "var y = " + this.y + ";";
			html += "var width = " + this.width + ";";
			html += "var height = " + this.height + ";";
			html += "var Drag_Enabled = false;";
			html += "var Drag_offsetX = 0;";
			html += "var Drag_offsetY = 0;";
			html += "function minimize ()";
			html += "{";
			html += "window.moveTo(0,-4000);"
			html += "if ((opener) && (!opener.closed)) opener.window.focus();"
			html += "window.blur();"
			html += "}";
			html += "function maximize ()";
			html += "{";
			html += "if (maximized == false)"
			html += "{";
			html += "window.resizeTo (screen.availWidth, screen.availHeight);";
			html += "window.moveTo (0,0);";
			html += "}";
			html += "else";
			html += "{";
			html += "window.resizeTo (width, height);";
			html += "window.moveTo (x, y);";
			html += "}";
			html += "maximized = !maximized;";
			html += "}";
			html += "function restore ()";
			html += "{";
			html += "if (maximized) window.moveTo (0,0);";
			html += "if (!maximized) window.moveTo (x,y);";
			html += "return(false);";
			html += "}";
			html += "function Drag_Engage (evt)";
			html += "{";
			html += "Drag_offsetX = evt.screenX;";
			html += "Drag_offsetY = evt.screenY;";
			html += "Drag_Enabled = true;";
			html += "}";
			html += "function Drag_Release ()";
			html += "{";
			html += "Drag_Enabled = false;"
			html += "}";
			html += "function Drag_Drag (evt)";
			html += "{";
			html += "var clientX = 0;";
			html += "var clientY = 0;";
			html += "if ((Drag_Enabled == true) && (maximized == false))";
			html += "{";
			html += "clientY = evt.screenY;";
			html += "clientX = evt.screenX;";
			html += "clientY -= Drag_offsetY;";
			html += "clientX -= Drag_offsetX;";
			html += "Drag_offsetY = evt.screenY;";
			html += "Drag_offsetX = evt.screenX;";
			html += "x+=clientX;";
			html += "y+=clientY;";
			html += "window.moveTo (x, y);";
			html += "}";
			html += "}";
			html += "function implementChrome ()";
			html += "{";
			html += "if (window.frames[\"BorderTop\"] && window.frames[\"ChromeTop\"] && window.frames[\"BorderBottom\"] && window.frames[\"ChromeBottom\"] && window.frames[\"BorderLeft\"] && window.frames[\"ChromeLeft\"] && window.frames[\"BorderRight\"] && window.frames[\"ChromeRight\"])";
			html += "{";
			html += "BorderTop.document.bgColor=\""+ this.BorderTop.color +"\";";
			html += "BorderBottom.document.bgColor=\""+ this.BorderBottom.color +"\";";
			html += "BorderLeft.document.bgColor=\""+ this.BorderLeft.color +"\";";
			html += "BorderRight.document.bgColor=\""+ this.BorderRight.color +"\";";
			html += "ChromeTop.document.open ();"
			html += "ChromeTop.document.write (\"" + CStyle_Encode (this.ChromeTop.html()) + "\");"
			html += "ChromeTop.document.close ();"
			html += "ChromeBottom.document.open ();"
			html += "ChromeBottom.document.write (\"" + CStyle_Encode (this.ChromeBottom.html()) + "\");"
			html += "ChromeBottom.document.close ();"
			html += "ChromeLeft.document.open ();"
			html += "ChromeLeft.document.write (\"" + CStyle_Encode (this.ChromeLeft.html()) + "\");"
			html += "ChromeLeft.document.close ();"
			html += "ChromeRight.document.open ();"
			html += "ChromeRight.document.write (\"" + CStyle_Encode (this.ChromeRight.html()) + "\");"
			html += "ChromeRight.document.close ();"
			html += "}";
			html += "else";
			html += "{";
			html += "setTimeout (\"implementChrome ();\", 10);";
			html += "}"
			html += "}";
			html += "implementChrome ();";
			html += "window.onfocus = restore;";
			html += "</script>";
			html += "<frameset border=\"0\" framespacing=\"0\" frameborder=\"0\" rows=\"" + this.BorderTop.width + ", 100%, " + this.BorderBottom.width + "\">";
			html += "<frame name=\"BorderTop\" src=\"about:blank\" scrolling=\"no\" noresize>";
			html += "<frameset border=\"0\" framespacing=\"0\" frameborder=\"0\" cols=\"" + this.BorderLeft.width + ", 100%, " + this.BorderRight.width + "\">";
			html += "<frame name=\"BorderLeft\" src=\"about:blank\" scrolling=\"no\" noresize>";
			html += "<frameset border=\"0\" framespacing=\"0\" frameborder=\"0\" rows=\"" + this.ChromeTop.width + ", 100%, " + this.ChromeBottom.width + "\">";
			html += "<frame name=\"ChromeTop\" src=\"about:blank\" scrolling=\"no\" noresize>";
			html += "<frameset border=\"0\" framespacing=\"0\" frameborder=\"0\" cols=\"" + this.ChromeLeft.width + ", 100%, " + this.ChromeRight.width + "\">";
			html += "<frame name=\"ChromeLeft\" src=\"about:blank\" scrolling=\"no\" noresize>";
			html += "<frame name=\"main\" src=\"" + this.url + "\">";
			html += "<frame name=\"ChromeRight\" src=\"about:blank\" scrolling=\"no\" noresize>";
			html += "</frameset>";
			html += "<frame name=\"ChromeBottom\" src=\"about:blank\" scrolling=\"no\" noresize>";
			html += "</frameset>";
			html += "<frame name=\"BorderRight\" src=\"about:blank\" scrolling=\"no\" noresize>";
			html += "</frameset>";
			html += "<frame name=\"BorderBottom\" src=\"about:blank\" scrolling=\"no\" noresize>";
			html += "</frameset>";
			html += "</html>";
	
			this.window = window.open ("", "", "fullscreen=1");
			this.window.resizeTo (this.width, this.height);
			this.window.moveTo (this.x, this.y);
			this.window.document.open ();
			this.window.document.write (html);
			this.window.document.close ();
		}
		else
		{
			this.window = window.open (this.url, this.winName, "toolbar=0,location=0,directories=0,status=0,scrollbars=1,resizable=0,width="+this.width+",height="+this.height);
			this.window.moveTo (this.x, this.y);
		}
	}
	else
	{
		this.window.focus ();
	}
	
}

function ChromelessWindow_Close ()
{
	if (this.window != void 0)
	{
		this.window.close ();
		this.window = void 0;
	}
}

function ChromelessWindow_Resize (AnchorPoint, width, height)
{
	if ((this.window != void 0) && (!this.window.closed))
	{
		DeltaX = width - this.width;
		DeltaY = height - this.height;
		Step = 0;
		
		if (Math.abs(DeltaX) > Math.abs(DeltaY))
		{
			Step = Math.floor (Math.abs(DeltaX) / 4);
		}
		else
		{
			Step = Math.floor (Math.abs(DeltaY) / 4);
		}
		DeltaX = DeltaX / Step;
		DeltaY = DeltaY / Step;
		
		incrementalResize (this, 1, Step, DeltaX, DeltaY, width, height, AnchorPoint);
	}		
}

function incrementalResize (winPtr, start, stop, DeltaX, DeltaY, width, height, AnchorPoint)
{
	for (i=start; i<stop; i++)
	{	
		winPtr.window.resizeTo (winPtr.width+Math.round(i*DeltaX), winPtr.height+Math.round(i*DeltaY));
		if (AnchorPoint & 1)
		{
			winPtr.window.x -= DeltaX;
		}
		else if (AnchorPoint & 2)
		{
			winPtr.window.x -= DeltaX/2;
		}
		
		if (AnchorPoint & 4)
		{
			winPtr.window.y -= DeltaY;
		}
		else if (AnchorPoint & 8)
		{
			winPtr.window.y -= DeltaY/2;
		}
	
	 	winPtr.window.moveTo (winPtr.window.x, winPtr.window.y);
	}
	
	winPtr.width = width;
	winPtr.height = height;
	winPtr.window.width = width;
	winPtr.window.height = height;
	winPtr.window.resizeTo (width, height);
	return (void 0);
}

function CStyle_Encode (Str)
{
	var ret = Str.toString ();
	var re=/\\/g;
	ret=ret.replace(re,"\\\\");
	re=/\//g;
	ret=ret.replace(re,"\\\/");
	re=/\"/g;
	ret=ret.replace(re,"\\\"");
	re=/\'/g;
	ret=ret.replace(re,"\\\'");
	re=/\n/g;
	ret=ret.replace(re,"\\n");
	
	return (ret);
}