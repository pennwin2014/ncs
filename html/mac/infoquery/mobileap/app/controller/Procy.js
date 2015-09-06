

Ext.define('proauthRzAuthlog.controller.Procy', {
    extend: 'Ext.app.Controller',  
    stores: ['List'],
    models: ['List'],  
    views: ['list.List'],
    mac:'',
    apname:'',
    apmac:'',
          
    sdate:'',
    stime:'',
    edate:'',
    etime:'',    
    
    searchtype:'',          
    model:'',
    devid:'',
  	plate:'',
  	line:'',
  	aptype:'',
  
    init: function() {
        this.control({
          'authloglist':{
  				//select: this.showTab,
            render: this.showRender
          },
		  
          //关键字查询
          'myselfPanel button[action=keycx]':{
          	click: this.dwglkeycx
          },
          //高级查询
          'myselfPanel button[action=advancecx]':{
          	click: this.highquery
          },
		  
		  		/* 增加功能：钱智明2015/7/22/16:30 */
		  		//轨迹地图
		  		'authloglist button[action=termmacmap]':{
          	click: this.mobilemap
          },
		  
		  		//查看图表
		  		'authloglist button[action=checkgraph]':{
          	click: this.placecheckgraph
          },
          
          //导出
		  		'authloglist button[action=export]':{
        		click: this.infoexport
        	}
		  
        });
    },
    
    showRender: function(){
        var store = Ext.ComponentQuery.query('authloglist')[0].getStore();
        store.on('beforeload', function (store, options) {
		//调整视图高度
        var qgrid=Ext.ComponentQuery.query('authloglist')[0];
    	  parent.grid_height=parent.Ext.getCmp('layout_center').getHeight()-56;
        qgrid.setHeight(parseInt(parent.grid_height)+25);
        	
        var keyword = Ext.getCmp('keyword_mobileaccount').value;
				//取到 combox 中选择的时间
      	var time_flag = Ext.getCmp('time_flag').value;
        var groupid = parent.ncsgroupid;
				//var did = parent.corpdid;
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
            var servicename = proauthRzAuthlog.controller.Procy.servicename;
            var mac = proauthRzAuthlog.controller.Procy.mac;
            var ssid = proauthRzAuthlog.controller.Procy.ssid;
            
            var sdate = proauthRzAuthlog.controller.Procy.sdate;
            var stime = proauthRzAuthlog.controller.Procy.stime;
            var edate = proauthRzAuthlog.controller.Procy.edate;
            var etime = proauthRzAuthlog.controller.Procy.etime;            
            
            var searchtype = proauthRzAuthlog.controller.Procy.searchtype;            
            var model = proauthRzAuthlog.controller.Procy.model;
            var devid = proauthRzAuthlog.controller.Procy.devid;         
						var plate = proauthRzAuthlog.controller.Procy.plate;
      			var line = proauthRzAuthlog.controller.Procy.line;
      			var aptype = proauthRzAuthlog.controller.Procy.aptype;
       
            var new_params={groupid:groupid,did:did,keyword:keyword, time_flag:time_flag, servicecode:servicecode,mac:mac,ssid:ssid,servicename:servicename,sdate:sdate,stime:stime,edate:edate,etime:etime,searchtype:searchtype, model:model, devid:devid, plate:plate, line:line, aptype:aptype};
        
             Ext.apply(store.proxy.extraParams,new_params);

        });//store.on('beforeload', function (store, options) {

        proauthRzAuthlog.controller.Procy.loadProcyListStore();
    },// showRender: function(){

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
                    },{
                        fieldLabel:'&nbsp;&nbsp;起始时间',
                        xtype: 'textfield',
                        value:'00:00:00',
                        name:'stime',
                        regex :/^[0-2][0-9]:[0-6][0-9]:[0-6][0-9]$/,
                        regexText: "格式：xx:xx:xx",
                        allowBlank  : false,
                        id:'stime'
                    }]
					
                },{					
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
                    },{
						fieldLabel:'&nbsp;&nbsp;结束时间',
                        xtype: 'textfield',
                        value:'23:59:59',
                        name:'etime',
                        regex :/^[0-2][0-9]:[0-6][0-9]:[0-6][0-9]$/,
                        regexText: "格式：xx:xx:xx",
                        allowBlank  : false,
                        id:'etime'
                    }]
					
                },{					
                    layout: 'hbox',
                    xtype: 'container',                   
                    items:[
					{
                        fieldLabel:'车厢编号',
                        xtype: 'textfield',
                        name:'model'
                    },{
                        fieldLabel:'&nbsp;&nbsp;车牌号码',
                        xtype: 'textfield',
                        name:'devid'
                    }]
					
                },{
                    layout: 'hbox',
                    xtype: 'container',
                    items:[
                    {
												fieldLabel:'站点名称',
                        xtype: 'textfield',
                        name:'plate'
                    },{                        
                        fieldLabel:'&nbsp;&nbsp;线路名称',
                        xtype: 'textfield',
                        name:'line'
										}]
					
                },{
                    layout: 'hbox',
                    xtype: 'container',
                    items:[
                    {
												fieldLabel:'设备类型',
                        xtype: 'textfield',
                        name:'aptype'
                    }
                    ]
                },{
                    layout: 'column',
                    xtype: 'container',                     
                    items:[
                    {
												xtype: 'container',
                        columnWidth: .20
                    },{                                              
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
							
							
							proauthRzAuthlog.controller.Procy.model = 
							formquery.getForm().getValues().model;
							
							proauthRzAuthlog.controller.Procy.devid = 
							formquery.getForm().getValues().devid;
							
														
													proauthRzAuthlog.controller.Procy.plate = 
													formquery.getForm().getValues().plate;
											
													proauthRzAuthlog.controller.Procy.line = 
													formquery.getForm().getValues().line;
										
													proauthRzAuthlog.controller.Procy.aptype = 
													formquery.getForm().getValues().aptype;

							store.load();
                        }
                           
                   }]
				   
                }]
        });    
    
        var win3 = Ext.widget('window', {
            title: '高级查询',
//            closeAction: 'hide',
            x:200,
            y:32,
            width: 580,
            resizable: true,
            draggable:true,
            modal:false,
            items: formquery
        });
       
        win3.show();
    },
	
	
  		
	/* 增加功能：钱智明2015/7/22/15:30 */
	//采集设备移动轨迹地图
	mobilemap: function(thisBtn){
		var apValue = Ext.getCmp('keyword_mobileaccount').value;	
		if(apValue == ''){
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
              //title: '移动轨迹地图',
              //html: '<iframe src="/mac/datamining/track/track.htm" frameborder="no" style="width:100%;height:1000px;"></iframe>'
              html: '<iframe src="/mac/infoquery/mobileap/peopletrack/track.htm" frameborder="no" style="width:100%;height:500px;"></iframe>'
              
            }]
        });
		
		var win4 = Ext.widget('window', {
            title: '采集设备移动轨迹',
//            closeAction: 'hide',
            x:50,
            y:80,
            width: 700,
						height: 480,
            resizable: false,
            draggable:true,
            modal:true,
            items: tabPanelmap
        });
       
        win4.show();
		
	},
	
	//查看图表
	placecheckgraph: function(thisBtn){
		var macValue = Ext.getCmp('keyword_mobileaccount').value;
		if(macValue == ''){
			alert('请先输入移动采集设备名称或编号');
			return ;
		} 
		
		var tabPanelchecktable = new Ext.TabPanel({
            //renderTo: "grid",
			frame: true,
            layout: 'form',
            activeTab: 0,
            items: [{   
			  			title: '场所频繁出现的终端TOP10',	
              html: '<iframe src="/mac/infoquery/mobileap/checkgraph/ChosenApTerminalmacTop.html" frameborder="no" style="width:100%;height:1000px;"></iframe>'
            },{
              title: '指定移动设备的人流密集时段',
              html: '<iframe src="/mac/infoquery/mobileap/checkgraph/ChosenApPeopleCrowedTime.html" frameborder="no" style="width:100%;height:1000px;"></iframe>'
            },{
              title: '指定区域的人流趋势',
              html: '<iframe src="/mac/infoquery/mobileap/checkgraph/ChosenApPeopleStreamTrend.html" frameborder="no" style="width:100%;height:1000px;"></iframe>'
            }]
        });
		
		var win5 = Ext.widget('window', {
            title: '查看图表',
            x:50,
            y:80,
            width: 950,
						height: 560,
            resizable: false,
            draggable:true,
            modal:true,
            items: tabPanelchecktable
        });
       
        win5.show();
		
	},
	
	infoexport: function(thisBtn){
		
		var form1 = document.getElementById('form1');	
		form1.mobileexport.value = 'export';
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
         
            store.load({callback : function(r, option, success) {   
            	
            	 /* 添加 callback 获取json中去重数量，并覆写原来的 tbar 中的 displayMsg  */
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

