 	//创建XMLHttpRequest对象       
function createXMLHttpRequest() {
	var C=null;
		if(window.XMLHttpRequest) { //Mozilla 浏览器
			C = new XMLHttpRequest();
		}
		else if (window.ActiveXObject) { // IE浏览器
			try {
				C = new ActiveXObject("Msxml2.XMLHTTP");
			} catch (e) {
				try {
					C = new ActiveXObject("Microsoft.XMLHTTP");
				} catch (e) {}
			}
		}
		return C;
	}
	//解析字符串  ins-输入字符串，   names-开始字符串 namee -结束字符串 返回值 （names 和namee 之间的字符串 可用于解析简单的XML
	function getxmlbyname(ins,names,namee){
	var start;
	var end;
	var ret='';

	start=ins.indexOf(names);
	end=ins.indexOf(namee);
	
	if(start!=-1&&end!=-1){
		ret=ins.substr(start+names.length,end-start-names.length);
	}
	return ret;	
}

