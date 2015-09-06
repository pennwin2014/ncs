

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
		servicecode:'',
		
		searchtype:'',        
    sname:'',              
   	servicegtype:'', 
    servicearea:'',
    ceoname:'',
    jytype:'',
    
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
						click:this.highquery  
		  		},
		  
		  		/* 增加按钮：2015/7/26 10:29  钱智明 */
		  		//电子地图
		  		'authloglist button[action=electricmap]':{
          	click: this.placeelectricmap
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
        
        var groupid="";
				var username="";
                
        
        store.on('beforeload', function (store, options) {
        	
				//调整视图高度
        var qgrid=Ext.ComponentQuery.query('authloglist')[0];
    	  parent.grid_height=parent.Ext.getCmp('layout_center').getHeight()-56;
        qgrid.setHeight(parseInt(parent.grid_height)+25);
        	
        var keyword = Ext.getCmp('keyword_mobileaccount').value;
				//取到 combox 中选择的时间
      	var time_flag = Ext.getCmp('time_flag').value;                        				
				
				
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
            var sdate = proauthRzAuthlog.controller.Procy.sdate;
            var stime = proauthRzAuthlog.controller.Procy.stime;
            var edate = proauthRzAuthlog.controller.Procy.edate;
            var etime = proauthRzAuthlog.controller.Procy.etime;
            var servicecode = proauthRzAuthlog.controller.Procy.servicecode;
            
            var searchtype = proauthRzAuthlog.controller.Procy.searchtype;                    
            var sname = proauthRzAuthlog.controller.Procy.sname;               
            var servicegtype = proauthRzAuthlog.controller.Procy.servicegtype;                                
						var servicearea = proauthRzAuthlog.controller.Procy.servicearea;
      			var ceoname = proauthRzAuthlog.controller.Procy.ceoname;
      			var jytype = proauthRzAuthlog.controller.Procy.jytype;
       
            var new_params={groupid:groupid,username:username,/* did:did,*/  keyword:keyword, time_flag:time_flag, mac:mac, servicename:servicename, sdate:sdate,stime:stime,edate:edate,etime:etime, servicecode:servicecode, searchtype:searchtype, sname:sname, servicegtype:servicegtype, servicearea:servicearea, ceoname:ceoname, jytype:jytype};
        
             Ext.apply(store.proxy.extraParams,new_params);

        });

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
											fieldLabel:'安全厂商名称',
                      xtype: 'textfield',
                      name:'sname'	                        
                    },
                    {
                    	fieldLabel:'&nbsp;&nbsp;场所编号',
                      xtype: 'textfield',
                      name:'servicecode'                    	                        
                    }]
					
                },{
                    layout: 'hbox',
                    xtype: 'container',
                    items:[
                    {
                    	fieldLabel:'场所服务类型',
                      xtype: 'textfield',
                      name:'servicegtype'
												
                    },{                        
                        fieldLabel:'&nbsp;&nbsp;场所所属区域',
                        xtype: 'textfield',
                        name:'servicearea'
										}]
					
                },{
                    layout: 'hbox',
                    xtype: 'container',
                    items:[
                    {                        
                        fieldLabel:'场所法人或负责人姓名',
                        xtype: 'textfield',
                        name:'ceoname'
                    },{                        
                        fieldLabel:'&nbsp;&nbsp;场所经营性质',
                        xtype: 'textfield',
                        name:'jytype'
										}]
                },{
                    layout: 'column',
                    xtype: 'container',                     
                    items:[
                    {
												xtype: 'container',
                        columnWidth: .20
                    },{
                        //fieldLabel:'&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;          ',
                        //labelWidth: 85,                        
                        xtype: 'button',
                        anchor: 'left',
                        //style : 'margin-left:80px',
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
							
							proauthRzAuthlog.controller.Procy.sname = 
							formquery.getForm().getValues().sname;
							
							
							
													proauthRzAuthlog.controller.Procy.servicegtype = 
													formquery.getForm().getValues().servicegtype;
											
													proauthRzAuthlog.controller.Procy.servicearea = 
													formquery.getForm().getValues().servicearea;
										
													proauthRzAuthlog.controller.Procy.ceoname = 
													formquery.getForm().getValues().ceoname;

													proauthRzAuthlog.controller.Procy.jytype = 
													formquery.getForm().getValues().jytype;
													
													store.load();
                        }
                           
                   },{
                        xtype: 'container',
                            columnWidth: .10
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
	
  		
	//电子地图
	placeelectricmap: function(thisBtn){
		var placeValue = Ext.getCmp('keyword_mobileaccount').value;
		if(placeValue == ''){
			alert('请输入场所名称或场所编号');
			return ;
		} 
		
		var tabPanelelectricmap = new Ext.Panel({
            //renderTo: "grid",
			frame: true,
            layout: 'form',
            activeTab: 0,
            items: [{
              //title: '电子地图',
              html: '<iframe src="/mac/infoquery/place/crowd/crowd.htm" frameborder="no" style="width:100%;height:500px;"></iframe>'
              //html: '<iframe src="/mac/datamining/crowd/crowd.htm" frameborder="no" style="width:100%;height:640px;"></iframe>'
              
            }]
        });
		
		var win4 = Ext.widget('window', {
            title: '电子地图',
            //closeAction: 'hide',
            x:50,
            y:50,
            width: 750,
						height: 550,
            resizable: false,
            draggable:true,
            modal:true,
            items: tabPanelelectricmap
        });
       
        win4.show();
		
	},
	
	//查看图表
	placecheckgraph: function(thisBtn){
		var placeValue = Ext.getCmp('keyword_mobileaccount').value;
		if(placeValue == ''){
			alert('请输入场所名称或场所编号');
			return ;
		} 
		
		var tabPanelchecktable = new Ext.TabPanel({
            //renderTo: "grid",
			frame: true,
            layout: 'form',
            activeTab: 0,
            items: [{   
			  			title: '场所频繁出现的终端TOP10',	
              html: '<iframe src="/mac/infoquery/place/checkgraph/ChosenPlaceTerminalmacTop.html" frameborder="no" style="width:100%;height:1000px;"></iframe>'
            },{
              title: '指定场所的人流密集时段',
              html: '<iframe src="/mac/infoquery/place/checkgraph/ChosenPlacePeopleCrowedTime.html" frameborder="no" style="width:100%;height:1000px;"></iframe>'
            },{
              title: '指定区域的人流趋势',
              html: '<iframe src="/mac/infoquery/place/checkgraph/ChosenAreaPeopleStreamTrend.html" frameborder="no" style="width:100%;height:1000px;"></iframe>'
            }]
        });
		
		var win5 = Ext.widget('window', {
            title: '查看图表',
            //closeAction: 'hide',
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
		form1.placeexport.value = 'export';
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

