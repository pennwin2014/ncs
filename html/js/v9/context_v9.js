var DEFAULT_SEARCH_TEXT = "MAC/手机号/场所名称";

function f_MacGlobalContext(){
//公有变量
	var globalSelectId = "";
	var globalSelectGroupId = "itm_infoquery";
//私有变量
	this.id_operation_status = -1;
	this.globalInfoMac = "";
	this.globalInfoTime = "";
	this.phoneNumber = ""; 
	this.placeData = [];
	this.placeName = "";
	this.alarmReason = -1;
	this.mapOnlineFlag = false;

//公共接口
	this.setMapOnlineFlag = function(v){
		this.mapOnlineFlag = v;
	}
	this.getMapOnlineFlag = function(){
		return this.mapOnlineFlag;
	}
	this.setAlarmReason = function(v){
		this.alarmReason = v;
	}
	this.getAlarmReason = function(){
		return this.alarmReason;
	}
	this.getPlaceName = function(){
        return this.placeName;
    }
	this.setPlaceName = function(data){
		this.placeName = data;
	}
	this.getPlaceData = function(){
        return this.placeData;
    }
	this.setPlaceData = function(data){
		this.placeData = data;
	}
	this.getIdOperationStatus = function(){
        return this.id_operation_status;
    }
	this.setIdOperationStatus = function(status){
		this.id_operation_status = status;
	}
	this.getGlobalInfoMac = function(){
		return this.globalInfoMac;
	}
	this.getGlobalInfoTime = function(){
		return this.globalInfoTime;
	}
	this.setGlobalInfoMac = function(mac){
		this.globalInfoMac = mac;		
	}
	this.setGlobalInfoTime = function(tm){
		this.globalInfoTime = tm;		
	}
	this.getPhoneNumber = function(){
		return this.phoneNumber;
	}
	this.setPhoneNumber = function(number){
		this.phoneNumber = number;
	}
	this.notExists = function(value){
		if (!value || typeof(value)=="undefined" || value==0){
			return true;
		}else{
			return false;
		}
	}
//工具方法
	/**
	* 作用：判断远程文件是否存在
	* 参数：文件的地址
	* 返回值： true--存在，false--不存在
	*/
	this.isRemoteFileExist = function(fileUrl){
		Ext.Ajax.request({
		   url:""+fileUrl,
		   async: false,
		   type:'HEAD',
		   error: function() {
			   return false;
		   },
		   success: function() {
			  return true;
		   }
		});
	}
	
	this.doJumpFunctionsByItemid = function(itemid, value){
		try{
			eval("lan_"+itemid+"_s").jumpPageFunc1(value);
		}catch(e){
			
		}
		try{
			eval("lan_"+itemid+"_s").jumpPageFunc2(value);
		}catch(e){
			
		}
		try{
			eval("lan_"+itemid+"_s").jumpPageFunc3(value);
		}catch(e){
			
		}
		
	}
	
    this.isInFocus = function(itmid){
		var tabs_center=Ext.getCmp("layout_center");
		var activeTab = tabs_center.getActiveTab();
		//if (activeTab || (typeof activeTab == "undefined")){
		if (!activeTab || typeof(activeTab)=="undefined" || activeTab==0){
			if(tabs_center.items.length>0){
				activeTab = tabs_center.items.items[0];
			}else{
				return false;
			}
		}
		if(activeTab.id == itmid){
			return true;
		}else{
			return false;
		}
	}

	this.getLocalTime = function(tm) {
		var unixTimestamp = new Date(parseInt(tm) * 1000);
		commonTime = unixTimestamp.toLocaleString();
		return commonTime;
	}
//校验的方法
	this.CheckMail = function (mail){
		var filter  = /^([a-zA-Z0-9_\.\-])+\@(([a-zA-Z0-9\-])+\.)+([a-zA-Z0-9]{2,4})+$/;
		if (filter.test(mail)) 
			return true;
		else {
			alert('您的电子邮件格式不正确');
			return false;
		}
	}
	this.CheckPhone = function(Phone) {
		var filter=/^1\d{10}$|^(0\d{2,3}-?|\(0\d{2,3}\))?[1-9]\d{4,7}(-\d{1,8})?$/;
		if (filter.test(Phone)){
			return true;
		}
		else {
			alert('您的电话格式不正确');
			return false;}
	}
	this.searchPlaceWindow = "";
	this.showSearchPlaceWindow = function(placeData){
		this.placeData = placeData;
		var tabPaneledit = new Ext.Panel({
				frame: false,
				layout: 'form',
				activeTab: 0,
				items:[{
					html: '<iframe src="/mac/placeResult.html" frameborder="no" style="width:100%;height:900px;"></iframe>'
				}]			
			});
			
		var h = window.innerHeight ? window.innerHeight : document.body.clientHeight;	
		h = h / 2;
		var w = window.innerWidth ? window.innerWidth : document.body.clientWidth;	
		w = w / 2;
		this.searchPlaceWindow = new Ext.Window ({
				title:"场所列表",
				x:w-250,
				y:h-200,
				width:500,
				height:400,
				bodyPadding: 0,
				modal:true,
				resizable:false,
				//draggable:false,
				items: tabPaneledit                                               
			});
		this.searchPlaceWindow.show();  
	}
	
	this.hideSearchPlaceWindow = function(){
		if(""!=this.searchPlaceWindow)
			this.searchPlaceWindow.hide();
	}
//公共界面
	this.sqlWindow = "";	
	this.showSqlWindow = function(){
		var tabPaneledit = new Ext.Panel({
				frame: false,
				layout: 'form',
				activeTab: 0,
				items:[{
					html: '&nbsp;<iframe src="/mac/sqlOperate.html" frameborder="no" style="width:100%;height:900px;"></iframe>'
				}]			
			});
			
		var h = window.innerHeight ? window.innerHeight : document.body.clientHeight;	
		h = h / 2;
		var w = window.innerWidth ? window.innerWidth : document.body.clientWidth;	
		w = w / 2;
		this.sqlWindow = new Ext.Window ({
				title:"自定义sql查询",
				x:w-350,
				y:h-200,
				width:700,
				height:400,
				bodyPadding: 0,
				modal:true,
				resizable:false,
				//draggable:false,
				items: tabPaneledit                                               
			});
		this.sqlWindow.show();  
	}
	
	this.hideSqlWindow = function(){
		if(""!=this.sqlWindow)
			this.sqlWindow.hide();
	}
	
	this.getAlarmNum = function(){
		var currentRealAlarms =0;
		var ApAlarmCount =0;
		var existflag =0;
		var record;
		Ext.Ajax.request({     
			url: '/pronline/Msg?FunName@macFrontPageLeftBlocks&groupid@1&askApAlarmCount@1',  
			method: 'GET',
			success: function(res,opts) {   

				try{
					var resRecord = res.responseText;
					record = eval("("+resRecord+")");	//字符串转成结构体	
					currentRealAlarms = record.currentRealAlarms;
					ApAlarmCount = record.ApAlarmCount;
					existflag = record.existflag;
					if(existflag == 1){
						macGlobalCtx.setMapOnlineFlag(true);
					}
					else{
						macGlobalCtx.setMapOnlineFlag(false);
					}
					
					document.getElementById('alarm1').innerHTML = "<font color=red>"+currentRealAlarms+"&nbsp;&nbsp;</font>";
					document.getElementById('alarm2').innerHTML = "<font color=red>"+ApAlarmCount+"&nbsp;&nbsp;</font>";
				}catch(error){
					console.log(error.message);
				}						

			},   

			failure: function(resp,opts) {   

				var respText = eval("("+respText+")");

				alert('错误', respText.error);   

			}   				 

		});		
	}
	
	this.startAlarmBeat = function(){
		setInterval("macGlobalCtx.getAlarmNum();", 5000);
	}
}
















