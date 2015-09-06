///////////////////////////////////////////////////////////////////////
//     This FavMenusystem was designed by Erik Arvidsson for WebFX   //
//                                                                   //
//     For more info and examples see: http://webfx.eae.net          //
//     or send mail to erik@eae.net                                  //
//                                                                   //
//     Feel free to use this code as lomg as this disclaimer is      //
//     intact.                                                       //
///////////////////////////////////////////////////////////////////////

var selectedItem = null;

var targetWin;

document.onclick = handleClick;
document.onmouseover = handleOver;
document.onmouseout = handleOut;
document.onmousedown = handleDown;
document.onmouseup = handleUp;

document.write(writeSubPadding(10));  //write the stylesheet for the sub. Getting the indention right

function handleClick() {
	el = getReal(window.event.srcElement, "tagName", "DIV");
	
	if ((el.className == "topFolder") || (el.className == "subFolder")) {
//		if (el.sub == null) el.sub = eval(el.id + "Sub");
//		alert(el.sub);
		el.sub = eval(el.id + "Sub");
		if (el.sub.style.display == null) el.sub.style.display = "none";
		if (el.sub.style.display != "block") { //hidden
			//any other sub open?
			if (el.parentElement.openedSub != null) {
				var opener = eval(el.parentElement.openedSub + ".opener");
				hide(el.parentElement.openedSub);
				if (opener.className == "topFolder")
					outTopItem(opener);
			}
			el.sub.style.display = "block";
			el.sub.parentElement.openedSub = el.sub.id;
			el.sub.opener = el;
		}
		else {
			if (el.sub.openedSub != null) hide(el.sub.openedSub);
			else hide(el.sub.id);
		}
	}
	
	if ((el.className == "subItem") || (el.className == "subFolder")) {
		if (selectedItem != null)
			restoreSubItem(selectedItem);
		highlightSubItem(el);
	}
	
	if ((el.className == "topItem") || (el.className == "topFolder")) {
		if (selectedItem != null)
			restoreSubItem(selectedItem);
	}

	if ((el.className == "topItem") || (el.className == "subItem")) {
		if ((el.href != null) && (el.href != "")) {
			if ((el.target == null) || (el.target == "")) {
				if (window.opener == null) {
//					alert(document.all.tags("BASE").item(0));
					if (document.all.tags("BASE").item(0) != null)
//						eval(document.all.tags("BASE").item(0).target + ".location = el.href");
						window.open(el.href, document.all.tags("BASE").item(0).target);
					else 
						window.location = el.href;					// HERE IS THE LOADING!!!
				}
				else {
					window.opener.location =  el.href;
				}
			}
			else {
				window.open(el.href, el.target);
//				eval(el.target + ".location = el.href");
			}
		}
	}
	
	var tmp  = getReal(el, "className", "favMenu");
	if (tmp.className == "favMenu") fixScroll(tmp);

}

function handleOver() {
	var fromEl = getReal(window.event.fromElement, "tagName", "DIV");
	var toEl = getReal(window.event.toElement, "tagName", "DIV");
	if (fromEl == toEl) return;
	
	el = toEl;
	
	if ((el.className == "topFolder") || (el.className == "topItem")) overTopItem(el);
	if ((el.className == "subFolder") || (el.className == "subItem")) overSubItem(el);
	
	if ((el.className == "topItem") || (el.className == "subItem")) {
		if (el.href != null) {
			if (el.oldtitle == null) el.oldtitle = el.title;
			if (el.oldtitle != "")
				el.title = el.oldtitle + "\n" + el.href;
			else
				el.title = el.oldtitle + el.href;
		}
	}
	
	if (el.className == "scrollButton") overscrollButton(el);
}

function handleOut() {
	var fromEl = getReal(window.event.fromElement, "tagName", "DIV");
	var toEl = getReal(window.event.toElement, "tagName", "DIV");
	if (fromEl == toEl) return;
	
	el = fromEl;

	if ((el.className == "topFolder") || (el.className == "topItem")) outTopItem(el);
	if ((el.className == "subFolder") || (el.className == "subItem")) outSubItem(el);
	if (el.className == "scrollButton") outscrollButton(el);
}

function handleDown() {
	el = getReal(window.event.srcElement, "tagName", "DIV");
		
	if (el.className == "scrollButton") {
		downscrollButton(el);
		var mark = Math.max(el.id.indexOf("Up"), el.id.indexOf("Down"));
		var type = el.id.substr(mark);
		var menuID = el.id.substring(0,mark);
		eval("scroll" + type + "(" + menuID + ")");
	}
}

function handleUp() {
	el = getReal(window.event.srcElement, "tagName", "DIV");
		
	if (el.className == "scrollButton") {
		upscrollButton(el);
		window.clearTimeout(scrolltimer);
	}
}

////////////////////// EVERYTHING IS HANDLED ////////////////////////////

function hide(elID) {
	var el = eval(elID);
	el.style.display = "none";
	el.parentElement.openedSub = null;
	if (el.openedSub != null) hide(el.openedSub);
}

function writeSubPadding(depth) {
	var str, str2, val;

	var str = "<style type='text/css'>\n";
	
	for (var i=0; i < depth; i++) {
		str2 = "";
		val  = 0;
		for (var j=0; j < i; j++) {
			str2 += ".sub "
			val += 22;
		}
		str += str2 + ".subFolder {padding-left: " + val + "px;}\n";
		str += str2 + ".subItem   {padding-left: " + val + "px;}\n";
	}
	
	str += "</style>\n";
	return str;
}

//If you wan't to change colors do so below

function overTopItem(el) {
	with (el.style) {
		background   = "buttonface";
		borderLeft   = "1px solid buttonhighlight";
		borderRight  = "1px solid buttonshadow";
		borderTop    = "1px solid buttonhighlight";
		borderBottom = "1px solid buttonshadow";
		paddingBottom = "2px";
	}
}

function outTopItem(el) {
	if ((el.sub != null) && (el.parentElement.openedSub == el.sub.id)) { //opened
		with(el.style) {
			borderTop = "1px solid buttonshadow";
			borderLeft  = "1px solid buttonshadow";
			borderRight    = "1px solid buttonhighlight";
			borderBottom = "0px";
			paddingBottom = "3px";
			background = "url(/images/tileback.gif) buttonface";
		}
	}
	else {
		with (el.style) {
			border = "1px solid buttonface";
			background = "buttonface";
			padding = "2px";
		}
	}
}

function overSubItem(el) {
	el.style.textDecoration = "underline";
}

function outSubItem(el) {
	el.style.textDecoration = "none";
}

function highlightSubItem(el) {
	el.style.background = "buttonshadow";
	el.style.color      = "white"; //"highlighttext";
	selectedItem = el;
}

function restoreSubItem(el) {
	el.style.background = "url(/images/tileback.gif) buttonface";
	el.style.color      = "menutext";
	selectedItem = null;
}

function overscrollButton(el) {
	overTopItem(el);
	el.style.padding = "0px";
}

function outscrollButton(el) {
	outTopItem(el);
	el.style.padding = "0px";
}

function downscrollButton(el) {
	with (el.style) {
		borderRight   = "1px solid buttonhighlight";
		borderLeft  = "1px solid buttonshadow";
		borderBottom    = "1px solid buttonhighlight";
		borderTop = "1px solid buttonshadow";
	}
}

function upscrollButton(el) {
	overTopItem(el);
	el.style.padding = "0px";
}

// ...till here

function getReal(el, type, value) {
	temp = el;
	while ((temp != null) && (temp.tagName != "BODY")) {
		if (eval("temp." + type) == value) {
			el = temp;
			return el;
		}
		temp = temp.parentElement;
	}
	return el;
}


//////////// SCROLLING /////////////////////////////////////////////////////

var scrolltimer;
var scrollAmount = 20;

function scrollDown(el) {
	if (el.offsetHeight > el.parentElement.offsetHeight) {
		var mt = parseInt(el.style.marginTop);
		mt -= scrollAmount;
		if (mt >= el.parentElement.offsetHeight - el.offsetHeight - 2) {
			el.style.marginTop = mt;
			scrolltimer = window.setTimeout("scrollDown(" + el.id + ")",100);
		}
		else {
			el.style.marginTop = el.parentElement.offsetHeight - el.offsetHeight - 2;
		}
	}
	fixScroll(el)
}

function scrollUp(el) {
	var mt = parseInt(el.style.marginTop);
	mt += scrollAmount;
	if (mt >= 0) {
		el.style.marginTop = 0;
	}
	else {
		el.style.marginTop = mt;
		scrolltimer = window.setTimeout("scrollUp(" + el.id + ")",100);
	}
	fixScroll(el);
}

function fixScroll(el) {
	if (el.style.marginTop == "") el.style.margin = "0px";
	mt = parseInt(el.style.marginTop);
	var downButton = eval(el.id + "Down");
	var upButton   = eval(el.id + "Up");

//Positioning of scroll buttons. You never now when someone changes something!
	upButton.style.left = leftPos(el.parentElement.parentElement) + 2;	// for outer
	upButton.style.top = topPos(el.parentElement.parentElement) + 2;
	upButton.style.width = el.parentElement.offsetWidth - 2;
	downButton.style.left = leftPos(el.parentElement.parentElement) + 2;
	downButton.style.top = topPos(el.parentElement.parentElement) + el.parentElement.offsetHeight - 16;
	downButton.style.width = el.parentElement.offsetWidth - 2;

	upButton.style.display   = (mt < 0) ? "block" : "none";
	downButton.style.display = ((mt == el.parentElement.offsetHeight - el.offsetHeight - 2)
		 || (el.offsetHeight <= el.parentElement.offsetHeight)) ? "none" : "inline";
		 
	if (el.offsetHeight < el.parentElement.offsetHeight) {
		el.style.marginTop = 0;
		upButton.style.display = "none";
	}
}

////////////////////////////////////////////////////////
//The following "two" functions are needed to calculate the position
function topPos(el) {
	return doPosLoop(el, "Top");
}

function leftPos(el) {
	return doPosLoop(el, "Left");
}

function doPosLoop(el, val) {
	var temp = el;
	var x = eval("temp.offset" + val);
	while ((temp.tagName!="BODY") && (temp.offsetParent.style.position != "absolute")) {
//	while (temp.tagName!="BODY") {
		temp = temp.offsetParent;
		x += eval("temp.offset" + val);
	}
	return x;
}
////////////////////////////////////////////////////////

function float(el, title, width, height) { 
	var str = "";
	var elid = el.id;
	if (title == null) title = "favorite bar (http:/webfx.eae.net)";
	if (width == null) width = 180;
	if (height == null) height = 340;
	windowURL = 'about:<html><head><title>Loading fav bar</title></head><body><p style="color: buttonface;">If you see this it probably means that this window has has stopped responding.<br><br>Press CTRL-ALT-DEL and kill this window before it takes the entire Windows with it!</p></html>';
	windowName = "floater";
	windowFeatures = "width=" + width + ",height=" + height + ",toolbar=0,location=0,directories=0,status=0,menuBar=0,scrollBars=0,resizable=1";
	tmpWin = window.open(windowURL, windowName, windowFeatures);

	str += '<html>\n<head>\n<title>' + title + '</title>\n';
	str += '<link rel="stylesheet" type="text/css" href="fav2.css">\n';
	str += '<script type="text/javascript" src="fav2.js"></script>\n';
	str += '<style type="text/css">\n';
	str += '.outer, .inner	{border: 1px solid buttonface;}\n';
	str += '</style>\n';
	str += '</head>\n';

	str += '<body style="margin: 0; border: 0;" scroll="no">\n';
	str += '<div class="outer" style="width: 100%; height: 100%;">\n';
	str += '<div class="inner" style="width: 100%; height: 100%;">\n';	
	str += el.outerHTML;
	str += '\n</div>\n</div>\n';
	
	str += '<div id="' + el.id + 'Up" class="scrollButton">5</div>\n';
	str += '<div id="' + el.id + 'Down" class="scrollButton">6</div>\n';

	str += '<script type="text/javascript">\n';
	str += 'window.onload = new Function("fixScroll(' + el.id + ')");\n';
//	str += 'window.setInterval("fixScroll(' + el.id + ');", 100);\n';
	str += 'document.body.style.width = document.body.clientWidth;\n';	//Needed to update. This helped. How strange?
	str += '</script>\n';
	
	tmpWin.document.open();
	tmpWin.document.write(str);
	tmpWin.document.close();
	tmpWin.focus();

	return tmpWin;
} 