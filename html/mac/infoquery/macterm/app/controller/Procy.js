
Ext.define('proauthRzAuthlog.controller.Procy', {
	extend: 'Ext.app.Controller',  
	stores: ['List'],
	models: ['List'],  
	views: 	['list.List'],    	
	
	searchtype:'',       
	sdate:'',
	stime:'',
	edate:'',
	etime:'',	
	mac:'',    
	apmac:'',
	     
	servicecode:'',
	servicename:'',    
	sname:'',
	ssid:'',
	apname:'',	
	vname:'',      
		
  init: function() {
  	this.control({
  		
			//关键字查询
		  'myselfPanel button[action=keycx]':{
		  		click: this.dwglkeycx
		  },
  		
    	//高级查询	
	  	'myselfPanel button[action=advancecx]':{
					click:this.highquery  
	  	},
	  	
	  	/* 场所选择：钱智明 2015/9/13 */
	  	'myselfPanel button[action=choseplace]':{
					click:this.infoquerychoseplace  
	  	},
     
     	//select: this.showTab, 	
    	'authloglist':{ 				
    			render: this.showRender
    	},		        	
	            		  
	  	/* 增加按钮：钱智明2015/7/20/11:00 */
	  	//特征分析
      'authloglist button[action=termac]':{
      		click: this.featureAnalysis
      },
	  
	  	/* 增加按钮：钱智明2015/7/20/15:00 */
	  	//人员轨迹
	  	'authloglist button[action=termmacmap]':{
      		click: this.infoquerymap
      },
	  
	  	/* 增加按钮：钱智明2015/7/22/15:30 */
	  	//身份信息
	  	'authloglist button[action=idcheck]':{
      		click: this.infoCheck
      },
	  
	  	/* 增加按钮：2015/7/26 10:55  钱智明 */
	  	//加入布控
	  	'authloglist button[action=joingroup]':{
      		click: this.joinGroupControl
      },
      
      //导出
	  	'authloglist button[action=export]':{
      		click: this.infoexport
      }
	  		 		 
    });
  },
    
  showRender: function(){
  	//alert("in showrender");	    	
  	var store = Ext.ComponentQuery.query('authloglist')[0].getStore();	
  	
  	if(parent.macGlobalCtx.getPhoneNumber() != ""){
  		  
  			/* 右上角全局搜索框：输入手机号，第一次即可显示搜索结果 钱智明 2015/9/14 10:32 */
  									
				g_count++;
				var phonenumber = parent.macGlobalCtx.getPhoneNumber();  
    		Ext.getCmp("keyword_mobileaccount").setValue(phonenumber);
    		
				//alert("in ShowRender " + phonenumber);		 
  	}    	
  											
		store.on('beforeload', function (store, options) {					
		
			var groupid="";
			var username="";				
			
			//alert("in beforeload");	
					
			//调整视图高度
			var qgrid=Ext.ComponentQuery.query('authloglist')[0];
			parent.grid_height=parent.Ext.getCmp('layout_center').getHeight()-56;
			qgrid.setHeight(parseInt(parent.grid_height)+25-100);
			
			//左侧场所点击区域名称、或者场所名称、 或者工具条上选择
			if(g_typename == '0'){		
				try{
		
					groupid = parent.getGlobalGroupId();															
				}
				catch(e){
					alert(e.message);
				}											
			}
			else if(g_typename == '1'){
										
				try{									
					username = parent.getGlobalUserName();								
				}
				catch(e){
					alert(e.message);
				}						
			}
			else{
				
				//查询标志置为""，默认从工具条获取区域或场所名称											
				var allChosen = Ext.getCmp("id_mac_placetree").rawValue;
	    	if((allChosen == "全部场所") || (allChosen == "")){
	    		
	    		g_Groupid = "all";    	    		    		 		 
		    	//alert("所有场所 g_Groupid 为 " + g_Groupid);
	    	}	
	    	else{
	    		var treeChosen = Ext.getCmp("id_mac_placetree").picker.getSelectionModel();
		    	var bChosenType = treeChosen.selected.items[0].raw.cls;
		    	
		    	if(bChosenType == "folder"){    		
		    		
		    		g_Groupid = treeChosen.selected.items[0].raw.id;		    				    	  			
		    		//alert("选中区域名称 g_Groupid 为 " + g_Groupid);
		    	}	
		  		else{	  			
		  			
		  			g_Username = treeChosen.selected.items[0].raw.code;			  						
		  			//alert("选中场所名称g_Username 为  " + g_Username);
		  		}	
	    	}
		    	
	    	groupid		= g_Groupid;
				username	= g_Username; 				    	 	 							 
					
			}

			//获取控件中查询条件：combox 中选择的时间、输入框中关键字
			//var time_flag = Ext.getCmp('time_flag').value;			      	
						 
		  var time_flag = getTimeFlag(); 
		  //alert("In showRender " + time_flag);
			//关键字查询    
			var keyword = Ext.getCmp('keyword_mobileaccount').value;      				      					       	          
      	    	      	      	    	      	
    	//高级查询字段      	
    	var searchtype = proauthRzAuthlog.controller.Procy.searchtype;         			       
    	var sdate = proauthRzAuthlog.controller.Procy.sdate;
    	var stime = proauthRzAuthlog.controller.Procy.stime;
    	var edate = proauthRzAuthlog.controller.Procy.edate;
    	var etime = proauthRzAuthlog.controller.Procy.etime;      	
    	var mac = proauthRzAuthlog.controller.Procy.mac;      	      	      	      	
    	var apmac = proauthRzAuthlog.controller.Procy.apmac;    	
    	var servicename = proauthRzAuthlog.controller.Procy.servicename;     
    	var servicecode = proauthRzAuthlog.controller.Procy.servicecode; 	
    	var sname = proauthRzAuthlog.controller.Procy.sname;
    	var ssid = proauthRzAuthlog.controller.Procy.ssid;  
    	var apname = proauthRzAuthlog.controller.Procy.apname;    	
    	var vname = proauthRzAuthlog.controller.Procy.vname;      	     	
    
    	    	
    	var new_params="";
    	if(g_count>0){
    		
    		//alert(">0");
    		/* g_count > 0时，store 参数中需要传入 phonenumber，钱智明 2015/9/14 10:32 */
    		
    		//var phonenumber = parent.macGlobalCtx.getPhoneNumber();  
    		//Ext.getCmp("keyword_mobileaccount").setValue(phonenumber);
    		
    		new_params = {phonenumber:phonenumber, groupid:groupid, username:username, keyword:keyword,time_flag:time_flag, mac:mac, apmac:apmac, sdate:sdate,stime:stime,edate:edate,etime:etime, servicecode:servicecode, servicename:servicename, searchtype:searchtype, sname:sname, ssid:ssid, apname:apname, vname:vname};
    		
    	}
    	else{
    		//alert("=0");
    		
    		/* g_count == 0时，store 参数中依然有 phonenumber，
    			 此时需要将其置为 "" 即可不查询手机号 钱智明 2015/9/14 10:32 */
        store.proxy.extraParams["phonenumber"] = "";   
                 
    		new_params = {groupid:groupid, username:username, keyword:keyword,time_flag:time_flag, mac:mac, apmac:apmac, sdate:sdate,stime:stime,edate:edate,etime:etime, servicecode:servicecode, servicename:servicename, searchtype:searchtype, sname:sname, ssid:ssid, apname:apname, vname:vname};
    	}
    	        
    	Ext.apply(store.proxy.extraParams,new_params);
 		});
 		
 		//右上角全局搜索手机号加载过后，将标志置0
 		store.on('load', function (store, options){
			//alert("in load,"+g_count);
			
			//使用完全局搜索之后，要清空parent里面的PhoneNumber
			if(g_count>0)
				g_count--;						
				
			parent.macGlobalCtx.setPhoneNumber("");
			
			
		});
	
 		
  	proauthRzAuthlog.controller.Procy.loadProcyListStore();
  },

  //关键字查询
  dwglkeycx: function(thisBtn){        	    
  			    	
  	proauthRzAuthlog.controller.Procy.searchtype = ""; 
    proauthRzAuthlog.controller.Procy.SetPage(1);  
   	proauthRzAuthlog.controller.Procy.loadProcyListStore();
   	
  },
		
	//高级查询
  highquery: function(thisBtn){
		var myDate = new Date();
    var yy = ( myDate.getYear() < 1900 ) ? ( 1900 + myDate.getYear() ) : myDate.getYear();
    var mm = myDate.getMonth()+1;
    if(mm<10)
    {
    	mm = '0'+mm;
    }
    var dd = myDate.getDate();
    if(dd<10)
    {
    	dd = '0'+dd;
    }
    var today = yy+'/'+mm+'/'+dd;
    var startday = yy+'/'+mm+'/'+'01';

    var formquery = new Ext.FormPanel({         
    	frame: true,
      layout: 'form',
      fieldDefaults: {
      	labelWidth: 85,                        
        width: 250
      },
      collapsible: false,
      bodyPadding: 10,
      items: [
			{
  			layout: 'hbox',
    		xtype: 'container',  					
    		items:[
				{                        
					fieldLabel:'起始日期',
  				xtype: 'datefield',
  				format: "Y/m/d",
  				value:startday,
  				name:'sdate',
  				id:'sdate'
				},
				{
  				fieldLabel:'&nbsp;&nbsp;起始时间',
  				xtype: 'textfield',
  				value:'00:00:00',
  				name:'stime',
  				regex :/^[0-2][0-9]:[0-6][0-9]:[0-6][0-9]$/,
  				regexText: "格式：xx:xx:xx",
  				allowBlank  : false,
  				id:'stime'
				}]    					
  		},
  		{					
				layout: 'hbox',
    		xtype: 'container',                   
    		items:[
				{
					fieldLabel:'结束日期',
  				xtype: 'datefield',
  				id:'edate',
  				format: "Y/m/d",
  				value:today,
  				name:'edate'
				},
				{
					fieldLabel:'&nbsp;&nbsp;结束时间',
  				xtype: 'textfield',
  				value:'23:59:59',
  				name:'etime',
  				regex :/^[0-2][0-9]:[0-6][0-9]:[0-6][0-9]$/,
  				regexText: "格式：xx:xx:xx",
  				allowBlank  : false,
  				id:'etime'
				}]					
  		},
  		{					
  			layout: 'hbox',
    		xtype: 'container',                   
    		items:[
				{
					fieldLabel:'场所编号',
  				xtype: 'textfield',
  				name:'servicecode'
				},
				{
  				fieldLabel:'&nbsp;&nbsp;场所名称',
  				xtype: 'textfield',
  				name:'servicename'
				}]					
 			},
 			{
				layout: 'hbox',
  			xtype: 'container',
  			items:[
				{
					fieldLabel:'安全厂商名称',
					xtype: 'textfield',
					name:'sname'
				},
				{ 
					fieldLabel:'&nbsp;&nbsp;接入热点ssid',
          xtype: 'textfield',
          name:'ssid'                                 						
				}]					
  		},
  		{
  			layout: 'hbox',
  			xtype: 'container',
  			items:[
				{                        
        	fieldLabel:'采集设备编号',
					xtype: 'textfield',
					name:'apname'
        },
        {                        
        	fieldLabel:'&nbsp;&nbsp;上网认证账号',
					xtype: 'textfield',
					name:'vname'
        }]
      },
      {
        layout: 'hbox',
        xtype: 'container',
        items:[
        {                        
        	fieldLabel:'MAC信息',
					xtype: 'textfield',
					name:'mac'
        }]    
      },
      {
    	  layout: 'column',
        xtype: 'container',                     
        items:[
        {
					xtype: 'container',
          columnWidth: .20
        },
        {                    	                        
          xtype: 'button',
          anchor: 'left',                     
          text:'&nbsp;&nbsp;&nbsp;&nbsp;查&nbsp;&nbsp;询 &nbsp;&nbsp;',
          columnWidth: .25, 
          handler: function(){
						var grid = Ext.ComponentQuery.query('authloglist')[0];             
						var store = grid.getStore();
						
						proauthRzAuthlog.controller.Procy.searchtype ='adv';
						
						proauthRzAuthlog.controller.Procy.sdate = 
						formquery.getForm().getValues().sdate;
	
						proauthRzAuthlog.controller.Procy.stime = 
						formquery.getForm().getValues().stime;
	
						proauthRzAuthlog.controller.Procy.edate = 
						formquery.getForm().getValues().edate;
	
						proauthRzAuthlog.controller.Procy.etime = 
						formquery.getForm().getValues().etime;
											
											
											
						proauthRzAuthlog.controller.Procy.servicecode = 
						formquery.getForm().getValues().servicecode;
	
						proauthRzAuthlog.controller.Procy.servicename = 
						formquery.getForm().getValues().servicename;
																																																	
							proauthRzAuthlog.controller.Procy.sname = 
						formquery.getForm().getValues().sname;
																											
						proauthRzAuthlog.controller.Procy.ssid = 
						formquery.getForm().getValues().ssid; 																					
																																														
						proauthRzAuthlog.controller.Procy.apname = 
						formquery.getForm().getValues().apname;
											
						proauthRzAuthlog.controller.Procy.vname = 
						formquery.getForm().getValues().vname;	
						
						proauthRzAuthlog.controller.Procy.mac = 
						formquery.getForm().getValues().mac;		
									
						store.load({callback : function(r, option, success) { 
	  
          	 	var responseText =  eval("("+option.response.responseText+")");        		        
	             	var g_distinctCount = "";
             	g_distinctCount = responseText.distinctCount;

        			var str = '去重后采集的mac数量 <span style="color:red;font-size:1.5em">' + g_distinctCount + '</span>';	 							 						
    					Ext.getCmp("id_click_leftplace").setText(str);      
              //alert("查询中点击后回调 " + g_distinctCount);  
              
              //将标志参数恢复默认值              
							g_typename = "";  
			        g_Groupid  = "";
							g_Username = "";  
							          				
	 					}});
          }
               
       	},
       	{
       		xtype: 'container',
          columnWidth: .10
       	}]
	   
      }]
         
    });    
  
    var win3 = Ext.widget('window', {
    	title: '高级查询',
			//closeAction: 'hide',
      x:200,
      y:32,
      width: 580,
      resizable: false,
      draggable:true,
      modal:false,
      items: formquery
    });
     
    win3.show();
  },
	
	
	//特征分析
	featureAnalysis: function(thisBtn){	
			
		funFeatureAnalysis("0");						
	},
	
	
	/* 增加按钮：钱智明2015/7/20/15:00 */
	//人员轨迹
	infoquerymap: function(thisBtn){
		var macValue = Ext.getCmp('keyword_mobileaccount').value;	
		if(macValue == ''){
			alert('请先输入MAC信息');
			return ;
		} 
		
		var time_flag = getTimeFlag(); 
		
		//parent.macGlobalCtx.setGlobalInfoTime(Ext.getCmp('time_flag').value);
		
		parent.macGlobalCtx.setGlobalInfoTime(time_flag);
		parent.macGlobalCtx.setGlobalInfoMac(Ext.getCmp('keyword_mobileaccount').value);
		
		var tabPanelmap = new Ext.Panel({            
			frame: true,
      layout: 'form',
      activeTab: 0,
      items: [{
        //title: '人员轨迹',
        //html: '<iframe src="/mac/infoquery/macterm/peopletrack/peopletrack.htm" frameborder="no" style="width:100%;height:500px;"></iframe>'
        html: '<iframe src="/mac/datamining/track/track1.htm" frameborder="no" style="width:100%;height:500px;"></iframe>'
        
      }]
 		});
		
		var win5 = Ext.widget('window', {
	    title: '人员轨迹',			
	    x:50,
	    y:80,
	    width: 700,
			height: 480,
	    resizable: false,
	    draggable:true,
	    modal:true,
	    items: tabPanelmap
		});
	
		win5.show();
		
	},
		
		
	//加入布控
	joinGroupControl: function(thisBtn){
		
		funJoinGroupControl("0");
		
	},	
				
	
	/* 增加按钮: 钱智明 2015/7/22/17:00 */
	//身份信息
	infoCheck: function(thisBtn){
		
		funInfoCheck("0");
	},
	
	
	/* (未使用，工具条中已直接创建场所树)场所选择：钱智明 2015/9/15 */
	infoquerychoseplace: function(thisBtn){
		/*var btnchoseplace = new Ext.Panel({
			frame: true,
	    layout: 'form',
	    activeTab: 0,
			items:[{		
				//html: '<iframe src="/pronline/Msg?FunName@ncsMacLog_ChosenMac_ChoseQueryPlace" frameborder="no" style="width:100%;height:1000px;"></iframe>'
				
				//可创建面板：场所管理 '增加'
				//html: '<iframe src="/pronline/Msg?FunName@ncsWebDispClientAddForm_wif" scrolling="no" name="moduleIframe" frameborder="no" style="width:100%;height:640px"></iframe>'										
				
				//同样可创建面板：后台函数中，返回json文件改为 nc_form_clientedit.htm			
				html: '<iframe src="/pronline/Msg?FunName@ncsMacLog_ChosenMac_ChoseQueryPlace" scrolling="no" name="moduleIframe" frameborder="no" style="width:100%;height:640px"></iframe>'
				
			}]			
		});
		
		var win8 = Ext.widget('window', {
			title: '请选择需要查询的区域或场所名称',    
	  	x:20,
	  	y:80,
	  	width: 800,
			height: 500,
			id: 'id_vname',
	  	resizable: false,
	  	draggable:true,
	  	modal:true,
	  
	  	items: btnchoseplace
		});
	     
		win8.show(); */
},
	
	
	/* 导出功能 */
	infoexport: function(thisBtn){
		
		var time_flag = getTimeFlag(); 
		
		var form1 = document.getElementById('form1');	
		form1.macLogExport.value = 'macexport';
		form1.time_flag.value =  timeFlag;	
		form1.submit();
		
	},
	
	
  /**************************************
  * 策略标签 
  ***************************************/  
  showTips: function( thisTV, eOpts ){
		thisTV.tip = Ext.create('Ext.tip.ToolTip', {
		  target: thisTV.el,
		  trackMouse: true,
		  dismissDelay : 60000,
		  html: '<p>例如：</p><p>&nbsp;&nbsp;&nbsp;单个端口：80,21</p><p>&nbsp;&nbsp;&nbsp;端口段：2000~3000</p><p>&nbsp;&nbsp;&nbsp;组合：80,2000~3000,3005</p>'
		});
  },
  
  /**************************************
  * 全局函数 
  ***************************************/
  inheritableStatics:{
	  loadProcyListStore:function(){
	    var store = Ext.ComponentQuery.query('authloglist')[0].getStore();          
	 
	 	  /* 添加 callback 获取json中去重数量 distinctCount */
	    store.load({callback : function(r, option, success) { 
	    	  
	  	 	var responseText =  eval("("+option.response.responseText+")");        		             
	     	var g_distinctCount = "";
	     	g_distinctCount = responseText.distinctCount;
	
				var str = '去重后采集的mac数量 <span style="color:red;font-size:1.5em">' + g_distinctCount+ '</span>';	 							 						
				Ext.getCmp("id_click_leftplace").setText(str);      
	      //alert("查询中点击后回调 " + g_distinctCount);  
	      
	      //将标志参数恢复默认值              
				g_typename = "";  
	      g_Groupid  = "";
				g_Username = "";  
				          				
	    }}); 
	      
	  },
	  SetPage:function(curpage){
	    var store = Ext.ComponentQuery.query('authloglist')[0].getStore();
	    store.currentPage = curpage;
	  },
	  setTitle: function(title){
	    document.getElementById("titledx").innerHTML = 
	         '&nbsp'+ title +'&nbsp;&nbsp;';   
	  }
  }
    
    
    
});
