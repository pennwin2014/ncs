


Ext.define('proauthRzAuthlog.controller.Procy', {
    extend: 'Ext.app.Controller',  
    stores: ['List'],
    models: ['List'],  
    views: ['list.List'],    
    mac:'',    
    apmac:'',
          
    sdate:'',
    stime:'',
    edate:'',
    etime:'',
    searchtype:'', 
         
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
       
       	//select: this.showTab, 	
      	'authloglist':{ 				
      			render: this.showRender
      	},		        	
		            		  
		  	/* 增加按钮：钱智明2015/7/20/11:00 */
		  	//特征分析
        'authloglist button[action=termac]':{
        		click: this.infoquerymac
        },
		  
		  	/* 增加按钮：钱智明2015/7/20/15:00 */
		  	//人员轨迹
		  	'authloglist button[action=termmacmap]':{
        		click: this.infoquerymap
        },
		  
		  	/* 增加按钮：钱智明2015/7/22/15:30 */
		  	//身份查看
		  	'authloglist button[action=idcheck]':{
        		click: this.infoidcheck
        },
		  
		  	/* 增加按钮：2015/7/26 10:55  钱智明 */
		  	//加入分组
		  	'authloglist button[action=joingroup]':{
        		click: this.infojoingroup
        },
        
        //导出
		  	'authloglist button[action=export]':{
        		click: this.infoexport
        }
		  		 		 
      });
    },
    
    showRender: function(){
			var store = Ext.ComponentQuery.query('authloglist')[0].getStore();
			
			var groupid="";
			var username="";
						
			store.on('beforeload', function (store, options) {				
				//调整视图高度
				var qgrid=Ext.ComponentQuery.query('authloglist')[0];
				parent.grid_height=parent.Ext.getCmp('layout_center').getHeight()-56;
				qgrid.setHeight(parseInt(parent.grid_height)+25-100);
     	 	var keyword = Ext.getCmp('keyword_mobileaccount').value;
      
				//取到 combox 中选择的时间
      	var time_flag = Ext.getCmp('time_flag').value;			
      	//var groupid = parent.ncsgroupid;
				
				var bRet = false;
				
				//根据 g_dispname 是否为空来判断，用户是否点击左侧场所树				
				if(g_dispname != null){									
					
					//groupid与username都为空时，默认查询所有场所
					if(g_groupidquery != ''){					
						groupid = g_groupidquery;			
						bRet = true;							
					}
					else{
						username = g_usernamequery;					
						if(username != '')
							bRet = true;		
					}	
					if(bRet == false){
						groupid = "all";
					}	
				}		
								
      	var did = parent.servicecode;
      	if(did){
      		groupid = "";
      	}
            
				var account = proauthRzAuthlog.controller.Procy.account;
      	var servicecode;
      	if(parent.servicecode!=''){
      		servicecode=parent.servicecode;
      	}
      	else{
      		servicecode = proauthRzAuthlog.controller.Procy.servicecode;
      	}
            	
      	var mac = proauthRzAuthlog.controller.Procy.mac;      	      	      	      	
      	var apmac = proauthRzAuthlog.controller.Procy.apmac;
      	
      	
      	var sdate = proauthRzAuthlog.controller.Procy.sdate;
      	var stime = proauthRzAuthlog.controller.Procy.stime;
      	var edate = proauthRzAuthlog.controller.Procy.edate;
      	var etime = proauthRzAuthlog.controller.Procy.etime;
      	
      	var searchtype = proauthRzAuthlog.controller.Procy.searchtype;         			       
      	var servicename = proauthRzAuthlog.controller.Procy.servicename;
      	
      	var sname = proauthRzAuthlog.controller.Procy.sname;
      	var ssid = proauthRzAuthlog.controller.Procy.ssid;  
      	var apname = proauthRzAuthlog.controller.Procy.apname;    	
      	var vname = proauthRzAuthlog.controller.Procy.vname;      	     	
      
      	var new_params={groupid:groupid,username:username, /*did:did,*/ keyword:keyword,time_flag:time_flag, mac:mac, apmac:apmac, sdate:sdate,stime:stime,edate:edate,etime:etime, servicecode:servicecode, servicename:servicename, searchtype:searchtype, sname:sname, ssid:ssid, apname:apname, vname:vname};
        
      	Ext.apply(store.proxy.extraParams,new_params);

   		});
			//alert("showRender");
    	proauthRzAuthlog.controller.Procy.loadProcyListStore();
    },

    //关键字查询
    dwglkeycx: function(thisBtn){    
    	
    	//g_groupidquery = ""; 
			//g_usernamequery = "";
		
			/* 通过有无 @ 字条判断输入框内容类型：输入关键字 或是左侧场所菜单选择 */		
			var tempvalue = Ext.getCmp("keyword_mobileaccount").value;	
			var tempvalue2 = tempvalue.substr(0, 1);
			var tempvalue3 = tempvalue.substr(tempvalue.length - 1, 1);
				
			if(tempvalue2 == '@' && tempvalue3 != '@'){
				Ext.getCmp("keyword_mobileaccount").setValue(tempvalue);	
				//alert(Ext.getCmp("keyword_mobileaccount").value);		
			}
    	
    	//alert("查询");	 
    	proauthRzAuthlog.controller.Procy.searchtype = ""; 
      //proauthRzAuthlog.controller.Procy.SetPage(1);  
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
          				}
          		]
          					
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
          				}
          		]					
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
          				}
          		]					
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
										}
								]					
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
                    }
                ]
            },
            {
                layout: 'hbox',
                xtype: 'container',
                height:20
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
                      text:'&nbsp;&nbsp;查&nbsp;&nbsp;询 &nbsp;&nbsp;',
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
															
												store.load();
                      }
                           
                   	},
                   	{
                   		xtype: 'container',
                      columnWidth: .10
                   	}
               	]
				   
            }
       	]
           
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
	infoquerymac: function(thisBtn){
		var macValue = Ext.getCmp('keyword_mobileaccount').value;
		if(macValue == ''){
			alert('请先输入MAC信息');
			return ;
		} 
		
		parent.setGlobalInfoTime(Ext.getCmp('time_flag').value);
		parent.setGlobalInfoMac(Ext.getCmp('keyword_mobileaccount').value);		
		var tabPanel = new Ext.TabPanel({
            //renderTo: "grid",
			frame: true,
            layout: 'form',
            activeTab: 0,
            items: [{
              title: '频繁出现的时间',
              html: '<iframe src="/mac/infoquery/macterm/featureanalysis/ChosenMacTimeOftenCount.html" frameborder="no" style="width:100%;height:1000px;"></iframe>'
            },{
              title: '频繁出现的场所',
              html: '<iframe src="/mac/infoquery/macterm/featureanalysis/ChosenMacPlaceOftenCount.html" frameborder="no" style="width:100%;height:1000px;"></iframe>'
            },{
              title: '频繁出现的场所类型',
              html: '<iframe src="/mac/infoquery/macterm/featureanalysis/ChosenMacPlaceTypeOftenCount.html" frameborder="no" style="width:100%;height:1000px;"></iframe>'
            }]
        });
		
		var win4 = Ext.widget('window', {
            title: '特征分析',
			//closeAction: 'hide',
            x:50,
            y:80,
            width: 950,
			height: 540,
            resizable: false,
            draggable:true,
            modal:true,
            items: tabPanel
        });
       
        win4.show();
		
	},
	
	
	/* 增加按钮：钱智明2015/7/20/15:00 */
	//人员轨迹
	infoquerymap: function(thisBtn){
		var macValue = Ext.getCmp('keyword_mobileaccount').value;	
		if(macValue == ''){
			alert('请先输入MAC信息');
			return ;
		} 
		
		parent.setGlobalInfoTime(Ext.getCmp('time_flag').value);
		parent.setGlobalInfoMac(Ext.getCmp('keyword_mobileaccount').value);
		
		var tabPanelmap = new Ext.Panel({
            //renderTo: "grid",
			frame: true,
            layout: 'form',
            activeTab: 0,
            items: [{
              //title: '人员轨迹',
              //html: '<iframe src="/mac/infoquery/macterm/peopletrack/peopletrack.htm" frameborder="no" style="width:100%;height:500px;"></iframe>'
              html: '<iframe src="/mac/infoquery/macterm/peopletrack/track.htm" frameborder="no" style="width:100%;height:500px;"></iframe>'
              
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
		
	//加入分组
	infojoingroup: function(thisBtn){
		
		//判断用户是否点击选择框
		if(!(Ext.getCmp('id_maclogtable').getSelectionModel().selected.length)){
			alert("请选中需要加入分组的mac");
			return;
		}	
		
		var tabPanelchecktable = new Ext.Panel({
            //renderTo: "grid",
						frame: true,
            layout: 'form',
            activeTab: 0,
            items: [{             
              html: '<iframe src="/mac/infoquery/macterm/joingroup/joingroup.html" frameborder="no" style="width:100%;height:1000px;"></iframe>'
            }]
        });
		
		var win6 = Ext.widget('window', {
            title: '加入分组',			
            x:50,
            y:80,
            width: 600,
						height: 400,
            resizable: false,
            draggable:true,
            modal:true,
            items: tabPanelchecktable
        });
       
        win6.show();
		
	},	
				
	
	/* 增加按钮: 钱智明2015/7/22/17:00 */
	//身份查看
	infoidcheck: function(thisBtn){
		
		var macValue = Ext.getCmp('keyword_mobileaccount').value;	
		if(macValue == ''){
			alert('请先输入MAC信息');
			return ;
		} 
		
		parent.setGlobalInfoMac(macValue);
		var tabPaneledit = new Ext.Panel({
			frame: true,
      layout: 'form',
      activeTab: 0,
			items:[{
				html: '<iframe src="/mac/infoquery/macterm/idcheck/idcheck.html" frameborder="no" style="width:100%;height:1000px;"></iframe>'
			}]			
		});
		
		var win7 = Ext.widget('window', {
            title: '身份查看MAC： ' + parent.getGlobalInfoMac(),
            //closeAction: 'hide',
            x:20,
            y:80,
            width: 940,
						height: 490,
						id: 'idinfocheck',
            resizable: false,
            draggable:true,
            modal:true,
            items: tabPaneledit
        });
       
        win7.show();
	},
	
	infoexport: function(thisBtn){
		
		var form1 = document.getElementById('form1');	
		form1.macLogExport.value = 'macexport';
		form1.time_flag.value =  Ext.getCmp('time_flag').value;	
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
            var keyword = Ext.getCmp('keyword_mobileaccount').value;
         
         	  /* 添加 callback 获取json中去重数量，并覆写原来的 tbar 中的 displayMsg  */
            store.load({callback : function(r, option, success) {   
            	 var responseText =  eval("("+option.response.responseText+")");        		                
               var g_distinctCount = "";
               g_distinctCount = responseText.distinctCount;
               //alert(g_distinctCount);
               var playInfo = Ext.getCmp('id_tbar_paging');
               playInfo.displayMsg = '显示{0} - {1}条记录 共{2}条记录,去重后共<span style="color:red;font-size:1.5em">' + g_distinctCount + '</span>条记录';
              
               playInfo.doRefresh();	
               //alert(Ext.getCmp('id_tbar_paging').displayMsg);
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

