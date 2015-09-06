function checkBrowser()
{
	this.agt=navigator.userAgent.toLowerCase();
	this.ver=navigator.appVersion
	this.dom=document.getElementById?1:0
	this.ie5=(this.ver.indexOf("MSIE 5")>-1 && this.dom)?1:0;
	this.ie4=(document.all && !this.dom)?1:0;
	this.ns5=(this.dom && parseInt(this.ver) >= 5) ?1:0;
	this.ns4=(document.layers && !this.dom)?1:0;
	this.bw=(this.ie5 || this.ie4 || this.ns4 || this.ns5)
	this.mac  = (this.agt.indexOf("mac")!=-1);
	this.ie4n5 = (this.ie4 || this.ie5)?1:0;
	this.ie4n5mac = (this.ie4n5 && this.mac)?1:0;
	return this
}

function Browser_ATagClickDisable (browser)
{
	if (browser.ie4n5)
	{
		window.event.returnValue = false;
	}
	
	return (false);
}

