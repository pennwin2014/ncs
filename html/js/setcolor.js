var setbkcolor = false;
var seturlcolor=false;
var markanchor=false;
var anchored=null;
var anchororder=null;
var maxlevel=80;
var TAG_LN="<BR>";
var blankset=/[_\u00ff-\uffff\W]/;
var charaset=/[a-zA-Z]/;
var digitset=/[0-9]/;
function _nextPosition(temp,tempI){
	while( temp.substr( tempI,TAG_LN.length ) == TAG_LN.toUpperCase() ){
		tempI=tempI+TAG_LN.length;
	}
	return tempI;
}
var COMMENT_HEAD="<!--";
var COMMENT_TAIL="-->";
function nextPosition(temp, tempI){
	while( tempI < temp.length && temp.charAt(tempI) == '<' ){
		var right;
		if( temp.substr( tempI,COMMENT_HEAD.length) == COMMENT_HEAD ){
			var rSub = temp.substr( tempI + COMMENT_HEAD.length );
			right = rSub.indexOf( COMMENT_TAIL );
			if( right == -1 ){
				right = rSub.indexOf('>');
				if(right == -1 ){
					tempI = temp.length;
					break;
				}
				tempI = right + 1 + tempI + COMMENT_HEAD.length;
			}else{
				tempI = right + COMMENT_TAIL.length + tempI + COMMENT_HEAD.length;
			}
		}else{
			var rSub = temp.substr(tempI+1);
			right = rSub.indexOf('>');
			if( right == -1 ){	break;	}
			var left = rSub.indexOf('<');
			if( left == -1 ){	// <>
				tempI = right+ 1 + tempI + 1;
				continue;
			}
			if( left > right ){	//<><
				tempI = right + 1 + tempI + 1;
				continue;
			}
			break;
		}// if ... else
	}// while
	return tempI;
}
function indexOfWord(temp, kw, p, n,last){
	var tempI, i , j ;
	for( i= nextPosition(temp,p) ; i< temp.length - kw.length + 1; i=nextPosition(temp, i+1 ) ){
		var success=true;
		tempI = i;
		for(j=0; j<kw.length; j++, tempI++){
			tempI = nextPosition(temp, tempI);
			if(  temp.charAt(tempI) != kw.charAt(j) ){
				break ;
			}
			if( j == kw.length -1 ){
				j=kw.length;break;
			}
		}
		if( j == kw.length){
			if(!seturlcolor ){
				var tempI1=nextPosition(temp,tempI+1);
if((i>0 &&((charaset.test(last) && charaset.test(temp.charAt(i)))||(digitset.test(last) && digitset.test(temp.charAt(i)))))||(tempI<temp.length-1&&((charaset.test(temp.charAt(tempI1)) && charaset.test(temp.charAt(tempI)))||(digitset.test(temp.charAt(tempI1)) && digitset.test(temp.charAt(tempI)))))){success=false;}
			}
		}else{success=false;}
		if( success )
			return i;
		last=temp.charAt(i);
	}
	return -1;
}
function render(str, colors, tp2){
	var color;
	var tp = anchororder[tp2-1];
	if (colors instanceof Array) color = colors[tp%colors.length];
	else color = colors;
	if (anchored != null && !anchored[tp]) {
		anchored[tp]=true;
		str = str.anchor("keyword"+(tp));
	}
	if (!setbkcolor)
		if(!seturlcolor) return str.fontcolor(color);
		else return str.bold();
	else
		return '<span style="background-color='+color+'"><b>'+str+'</b></span>'
}
function setColor(obj, keywords, colors){
	var text = eval(obj).innerHTML;

	var flag = new Array(text.length);
	var matched = new Array(keywords.length);
	var i,j;
	var text_up = text.toUpperCase();
	for(j=0; j<flag.length; j++)
		flag[j] = 0;
		

	for(i=0; i<keywords.length; i++){
		alert(keywords[i]);
		if( keywords[i].length <=0 ) continue;
		alert('ccc');
		var kw_up = keywords[i].toUpperCase();
		var p = 0, q = 0;
		matched[i] = false;
		var last=' ';
alert('aaa');
		while( (q = indexOfWord(text_up, kw_up, p, maxlevel,last)) != -1){
			var tq = q +1;
			flag[q] = i+1;
			for(j=1; j < keywords[i].length; j++){
				last=text_up.charAt(tq);
				tq = nextPosition( text, tq );
				flag[tq] = i+1;
				tq = tq+1;
			}
			q = tq;
			p = q ;
			matched[i] = true;
		}
	}
alert('fff');	
	
	var force1=false;
	if( (keywords.length==1 && keywords[0].length==1) || setbkcolor ) force1=true;
	var result="";
	var bg = 0;
	var tp = flag[0];
	var i;
	for(i=1; i<text.length; i++) {
		if(flag[i] != tp && ( tp==0 || setbkcolor || flag[i]==0)  ) {
			if (tp != 0 && ( force1 || i-bg>1 || text.charCodeAt(bg)< 0xff ) ) result += render(text.substr(bg, i - bg), colors, tp);
			else result += text.substr(bg, i - bg);
			tp = flag[i];
			bg = i;
		}
	}
	if (bg < text.length ){
		if( tp!=0 && ( force1 || i-bg>1 || text.charCodeAt(bg)< 0xff ) )
			result+=render(text.substr(bg,i-bg),colors,tp);
		else
			result += text.substr(bg, i - bg);
	}
	eval(obj).innerHTML = result;
	alert(result);
}
function _setAllColor(root, classname, keywords, color) {
	for (var i = 0; i < root.childNodes.length; ++i) {
		var obj = root.childNodes[i];
		if (obj.className&&obj.className==classname) {
			setColor(obj, keywords, color)
		} else if (obj.hasChildNodes()) {
			_setAllColor(obj, classname, keywords, color);
		}
	}
}
function setAllColor(classname, keywords, color){
	if ( anchororder == null ){
		anchororder = new Array(keywords.length);
		for (var i = 0; i < anchororder.length; ++i){
			anchororder[i] = i;
		}
	}
	for(var i=0; i<keywords.length; i++){
		var k = i;
		for(var j=k+1; j<keywords.length; j++)
			if(keywords[k].length < keywords[j].length)
				k = j;
		var temp = keywords[k];
		keywords[k] = keywords[i];
		keywords[i] = temp;
		var temp2 = anchororder[k];
		anchororder[k] = anchororder[i];
		anchororder[i] = temp2;
	}
	_setAllColor(document.documentElement, classname, keywords, color);
}
function setUrlColor( classname, keywords, color){
	seturlcolor = true;
	setAllColor( classname, keywords, color);
	seturlcolor = false;
}
function setAllBkColor(classname, keywords, color, makeanchor) {
	setbkcolor = true;
	if (anchored == null && makeanchor == true) {
		anchored = new Array(keywords.length);
		for (var i = 0; i < anchored.length; ++i) {
			anchored[i] = false;
		}
	}
	setAllColor(classname, keywords, color);
	setbkcolor = false;
}
