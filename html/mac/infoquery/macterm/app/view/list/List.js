
var g_dispname;					//用户左侧点击的groupid或username
var g_typename = "";		//全局：默认从工具条场所树中选择区域名称或场所名称;
												//否则，用户是点击左侧区域名称(为0)、或场所名称(为1)
												
var g_Groupid = "";			//全局：区域名称对应 groupid 
var g_Username = "";		//全局：场所名称对应 username
																								

var g_count = 0;				//计数：全局搜索手机号

var clickRecord = "";   //获取超链接点击时数据


/* 左侧场所树：点击，设置场所名称，直接可在后台查询 ncsuser 表中的 username */
function setSearchValue(typename, dispname){

	g_typename = typename;
	g_dispname = dispname;
		
  /* 将传入区域名称或场所名称设置到场所树菜单 */	
	Ext.getCmp("id_mac_placetree").setValue(dispname);				
	
	/* 添加 callback 获取json中去重数量 distinctCount */
	var store = Ext.ComponentQuery.query('authloglist')[0].getStore();	
	store.load({callback : function(r, option, success) {   
		
		var responseText =  eval("("+option.response.responseText+")");        		                
		var g_distinctCount = "";
		g_distinctCount = responseText.distinctCount;
    				
		var str = '去重后采集的mac数量 <span style="color:red;font-size:1.5em">' + g_distinctCount + '</span>';							 						
		Ext.getCmp("id_click_leftplace").setText(str);     
    //alert("左侧点击场所回调" + g_distinctCount);
               
    //将全局标志恢复默认值              
		g_typename = "";  
    g_Groupid  = "";
		g_Username = "";   
		               
	}}); 
}
  
/* 右上角搜索框：输入，新增手机号搜索 钱智明 2015/9/14 10:32 */
function setFieldVname(phonenumber){
	Ext.getCmp("keyword_mobileaccount").setValue(phonenumber);
	g_count++;
		
	alert("in setFieldVname,"+g_count);
		
	var store = Ext.ComponentQuery.query('authloglist')[0].getStore();
	store.load({callback : function(r, option, success) {  
		 
		var responseText =  eval("("+option.response.responseText+")");        		                
		var g_distinctCount = "";
		g_distinctCount = responseText.distinctCount;
	  				
		var str = '去重后采集的mac数量 <span style="color:red;font-size:1.5em">' + g_distinctCount + '</span>';						 						
		Ext.getCmp("id_click_leftplace").setText(str);     
	  //alert("mac日志页面下点击一键搜索" + g_distinctCount);
	   
	  //将全局标志恢复默认值               
		g_typename = "";  
	  g_Groupid  = "";
		g_Username = "";
				              
	}});
} 



/* 超链接：终端mac */
function ghostMac(value, p, record){
	
	//var str = Ext.String.format('<a href="#" style="color:blue" onclick="checkMacInfo(\'{1}\')">{0}</a>', value, value);
	
	var str = Ext.String.format('<a href="#" style="color:blue" onclick="(\'{1}\')">{0}</a>', value, value);

	//拷贝功能的超链接	
	//var str = Ext.String.format('<a href="#" id="ghostMac_copy_a" style="color:blue" data-clipboard-text="{2}" onclick="checkMacInfo(\'{1}\')">{0}</a>',  value, value, value);
	//alert(str);
	
	return str;
}

// (暂未使用 2015/9/18 16:00)超链接功能：终端mac, 设置mac到输入框; 弹窗显示mac相关信息 
function checkMacInfo(mac){
			
	Ext.getCmp("keyword_mobileaccount").setValue(mac);

  //获取选择时间 
  var time_Flag = getTimeFlag();  
  
  parent.showMacWindow(mac, time_Flag);
  
  /* IE下复制内容到剪贴板 */	
	window.clipboardData.setData('Text', mac);
	copyToClipboard(mac);	
  
	//复制功能
	/*var client2= new ZeroClipboard(document.getElementById("ghostMac_copy_a"), { movie: "/mac/infoquery/macterm/app/zeroclipboard/ZeroClipboard.swf" });
    client2.on("ready", function(readyEvent) {
        client2.on("aftercopy", function(event) {
            alert("复制成功，内容为: " + event.data["text/plain"]);
        });
   });	
   */						
}



/* 超链接：历史ssid列表 */
function ghostHistorySsid(value, p, record){
	var str = Ext.String.format('<a href="#" style="color:blue" onclick="checkHistorySsid(\'{1}\')">{0}</a>', '历史ssid列表', '历史ssid列表');
	//alert(str);
	return str;
}

// 超链接功能：历史ssid列表, 发送请求给后台 
function checkHistorySsid(){
	var rows = Ext.getCmp('id_maclogtable').getSelectionModel();
	var macvalue = [];
	for(var i = 0; i <rows.selected.items.length;i++){											
			macvalue.push([rows.selected.items[i].data.mac]);			
	}
	
	var tabPaneledit = new Ext.Panel({
			frame: true,
      layout: 'form',
      activeTab: 0,
			items:[{
				html: '<iframe src="/mac/infoquery/macterm/historyssid/historyssid.html" frameborder="no" style="width:100%;height:900px;"></iframe>'
			}]			
		});
		
	var win1 = Ext.widget('window', {
  	title: '当前mac：' + macvalue ,    
    x:parent.grid_width/5,
	  y:parent.grid_height/5,
	  width: parent.grid_width/2,
		height: parent.grid_height/2,
		id: 'id_historyssid',
		autoScroll: true, 
    resizable: false,
    draggable:true,
    modal:true,
    items: tabPaneledit
  });
       
	win1.show();			
}	



/* 超链接：上网认证账号 */
function ghostVname(value, p, record){
	var Vname = [];
		
	Vname = value.split(','); 
	var str="";
	for(var i in Vname){	
		if(i==0){
			str = Ext.String.format('<a href="#" style="color:blue" onclick="(\'{1}\')">{0}</a>', Vname[i], Vname[i]);	
		}	else{
			str = str +","+ Ext.String.format('<a href="#" style="color:blue" onclick="(\'{1}\')">{0}</a>', Vname[i], Vname[i]);	
		}		
					
	}	
	//var str = Ext.String.format('<a href="#" style="color:blue" onclick="checkVname(\'{1}\')">{0}</a>', value, value);
	
	//复制
	//var str = Ext.String.format('<a href="#" id="ghostMac_copy_a" style="color:blue" data-clipboard-text="{2}" onclick="checkVname(\'{1}\')">{0}</a>',  value, value, value);
	
	return str;
}

//(暂未使用 2015/9/19 17:21)超链接功能：查看上网认证账号关联信息, mac、虚拟身份
function checkVname(vname){
	Ext.getCmp("keyword_mobileaccount").setValue(vname);
		
	var rows = Ext.getCmp('id_maclogtable').getSelectionModel();
	var macValue = [];
	var vnameValue = [];
	for(var i = 0; i <rows.selected.items.length;i++){											
			macValue.push([rows.selected.items[i].data.mac]);	
			vnameValue.push([rows.selected.items[i].data.vname]);			
	}

		
	var tabPanel = new Ext.Panel({
			frame: true,
      layout: 'form',
      activeTab: 0,
			items:[{		
				html: '<iframe src="/pronline/Msg?FunName@ncsMacLog_ChosenMac_NetAuthID&mac@' + macValue + '" frameborder="no" style="width:100%;height:1000px;"></iframe>'										
			}]			
		});
		
	var win2 = Ext.widget('window', {
  	title: '当前上网认证账号为：' + vnameValue ,    
    x:20,
    y:80,
    width: 300,
		height: 500,
		id: 'id_vname',
    resizable: false,
    draggable:true,
    modal:true,
    
    items: tabPanel
  });
       
	win2.show();
			
}	

/* 超链接：场所名称 */
function ghostServicename(value, p, record){
	
	//var str = Ext.String.format('<a href="#" style="color:blue" onclick="checkMacInfo(\'{1}\')">{0}</a>', value, value);
	
	var str = Ext.String.format('<a href="#" style="color:blue" onclick="(\'{1}\')">{0}</a>', value, value);	
	return str;
}


//获取时间下拉框 Text 对应的 value
function getTimeFlag(){
	
	var timeFlagText = Ext.getCmp('time_flag').getText(); 
	var time_Flag = "1";
  switch(timeFlagText){
	  	case "今天":
	  		time_Flag = "1";
	  		break;
	  	case "最近三天":
	  		time_Flag = "2";
	  		break;
	  	case "最近一周":
	  		time_Flag = "3";
	  		break;
	  	case "最近一月":
	  		time_Flag = "4";
	  		break;
	  	case "最近三月":
	  		time_Flag = "5";
	  		break;
	  	case "最近半年":
	  		time_Flag = "6";
	  		break;			
	  	deault:
	  	  time_Flag = "1";			    
	};
  //alert(timeFlagText + " , " + time_Flag);
  
  return time_Flag;        
}

//工具条中：场所树需要 store 钱智明 2015/9/15 10:58
var store_areatree=Ext.create('Ext.data.TreeStore',{
		id:'store_areatree', 
		fields: ['text','id','code'],
		autoLoad : false,		
		proxy:{
			type: 'ajax',
			//url: '/pronline/Msg?FunName@ncsTreeUser_v9&plate@v4/ncs_user_tree.htm'
			
			url: '/pronline/Msg?FunName@ncsMacLog_ChosenMac_ChoseQueryPlace'
		},
		root: {
			text: '全部场所',
			id: 'all',			
			expanded: true
		}
}); 


//全局函数：特征分析		
function funFeatureAnalysis(typemark){
	
	var macValue; 
	if(typemark == "1")					       
		macValue = clickRecord.data.mac;
	else
	{
		macValue = Ext.getCmp('keyword_mobileaccount').value;
		if(macValue == ''){
			alert('请先输入MAC信息');
			return ;
		}	
	}
	
	var time_Flag = getTimeFlag();    		
	//alert(time_Flag + " In funFeatureAnalysis," + macValue);  			
	parent.macGlobalCtx.setGlobalInfoTime(time_Flag);
	parent.macGlobalCtx.setGlobalInfoMac(macValue);		
	
	var tabPanel = new Ext.TabPanel({           
		frame: true,
    layout: 'form',
    activeTab: 0,
    items: [{
      title: '频繁出现的时间',
      html: '<iframe src="/mac/infoquery/macterm/featureanalysis/ChosenMacTimeOftenCount.html" frameborder="no" style="width:100%;height:900px;"></iframe>'
    },{
      title: '频繁出现的场所',
      html: '<iframe src="/mac/infoquery/macterm/featureanalysis/ChosenMacPlaceOftenCount.html" frameborder="no" style="width:100%;height:900px;"></iframe>'
    },{
      title: '频繁出现的场所类型',
      html: '<iframe src="/mac/infoquery/macterm/featureanalysis/ChosenMacPlaceTypeOftenCount.html" frameborder="no" style="width:100%;height:900px;"></iframe>'
    }]
  });
	
	var winFeatureAnalysis = Ext.widget('window', {
    title: '特征分析',			
    x:parent.grid_width/5,
	  y:parent.grid_height/5,
	  width: parent.grid_width/1.4,
		height: parent.grid_height/1.5,
    resizable: false,
    draggable:true,
    modal:true,
    items: tabPanel
  });
     
  winFeatureAnalysis.show();  	
}

//全局函数：身份信息
function funInfoCheck(typemark){
	
	var macValue; 
	if(typemark == "1")					       
		macValue = clickRecord.data.mac;
	else
	{
		macValue = Ext.getCmp('keyword_mobileaccount').value;
		if(macValue == ''){
			alert('请先输入MAC信息');
			return ;
		}	
	}	
      		
	parent.macGlobalCtx.setGlobalInfoMac(macValue);
	var tabPanel = new Ext.Panel({
		frame: true,
		layout: 'form',
		activeTab: 0,
		items:[{
			html: '<iframe src="/mac/infoquery/macterm/idcheck/idcheck.html" frameborder="no" style="width:100%;height:900px;"></iframe>'
		}]			
	});
		
	var winInfoCheck = Ext.widget('window', {
	  title: '身份查看MAC： ' + macValue,				  
	  x:parent.grid_width/5,
	  y:parent.grid_height/5,
	  width: parent.grid_width/2,
		height: parent.grid_height/2,
		id: 'id_infocheck',
	  resizable: false,
	  draggable:true,
	  modal:true,
	  items: tabPanel
  });
 
  winInfoCheck.show(); 
}	

//全局函数：加入布控
function funJoinGroupControl(typemark){
	
	var macValue; 
	if(typemark == "1")					       
		macValue = clickRecord.data.mac;
	else
	{
		//判断用户是否点击选择框
		if(!(Ext.getCmp('id_maclogtable').getSelectionModel().selected.length)){
			alert("请选中需要加入分组的mac");
			return;
		}			
	}				
	
	var tabPanel = new Ext.Panel({    
		frame: true,
    layout: 'form',
    activeTab: 0,
    items: [{             
      html: '<iframe src="/mac/infoquery/macterm/joingroup/joingroup.html" frameborder="no" style="width:100%;height:1000px;"></iframe>'
    }]
	});
	
	var winJoinControl = Ext.widget('window', {
    title: '加入分组',
    style: 'font-size: 25px',			
    x:parent.grid_width/5,
	  y:parent.grid_height/5,
	  width: parent.grid_width/2,
		height: parent.grid_height/2,
    resizable: false,
    draggable:true,
    modal:true,
    items: tabPanel
	});

	winJoinControl.show();
	
}	

//全局函数：点击场所名称，查看场所明细、电子地图
function funPlaceDetail(){
	//获取参数：
	// FunName : ncsSysystem_alarmOne&sid@'sid'
	// plate : /home/ncmysql/ncs/plate/normally/proauth_system_alarmOne.htm
	
	var value = clickRecord.data.servicename;
	var itemRush=[];
	var username = "";			//场所编码
	var userDispname = "" ;		//场所名
	var groupname = "";			//行政区域
	var address = "";			//场所地址
	var totalCount = 0;			//设备总数
	var data =[] ;
	var jyxz;					//经营性质
	var groupcode ="";			//1->经营性、 2->非经营性、 3->无线采集前端
	var status;					//在线、离线
	var statuscode;				//1->在线、 2->离线

	Ext.Ajax.request({
			url: '/pronline/Msg?FunName@ncsSysystem_alarmOne&dispname@'+value, 
			method: 'GET',
			success: function(resp,opts) {
				try{
					var respRecord =  resp.responseText;
					var record = eval("("+respRecord+")");	//字符串转成结构体	
									
					
					username = record.username;
					userDispname = record.userDispname;
					groupname = record.groupname;
					address = record.address;
					groupcode = username[6];
					
					totalCount = record.totalCount;
					data = record.data;
					//判断经营性质
					
					switch(groupcode){
						case "1":
							jyxz = "经营性";
							break;
						case "2":
							jyxz = "非经营性";
							break;
						case "3":
							jyxz = "无线采集前端";
							break;
						default:
							jyxz = "其他";
							break;
					}
					if(address==""){
						address='无';
					}
					
					itemRush.push(
						{html:'<a align="left"><b>行政区域：</b></a>' + '<a align="right">'+ groupname+'</a>'},
						{html:'<a align="left"><b>经营性质：</b></a>' + '<a align="right">' + jyxz+'</a>'},
						{html:'<a align="left"><b>场所名称：</b></a>' + '<a align="right">'+ userDispname+'</a>'},
						{html:'<a align="left"><b>场所编码：</b></a>' + '<a align="right">' + username+'</a>'},
						{html:'<a align="left"><b>场所地址：</b></a>'},{html:'<a align="left">'+address+'</a>'},
						{html:'<a align="left"><b>关联设备列表：</b></a>'},{html:'<a align="left">'+totalCount+'</a>'}
					);
		
					for( var i=0; i< totalCount; i++){
						statuscode = data[i].statuscode;
						if(statuscode =='1'){
							status ='<font color=green>在线</font>';
						}else if(statuscode =='2'){
							status ='<font color=red>离线</font>';
						}
						itemRush.push({html:'<a align="left">'+data[i]['aplistDispname']+'</a>'}, 
							{html:'<a align="left">'+status+'</a>'}
						);
					}
					createDetailWin(itemRush);
				}
				catch(e){
					//console.log(e.message);
				}	
			},   
			failure: function(resp,opts) {   
				var respText = eval("("+respText+")");
				alert('错误', respText.error);   
			}   				 
		});
		
	function createDetailWin(itemRush){	
		//创建弹窗
		var mytable = Ext.create('Ext.panel.Panel', {
			//title: '基础信息',
			layout: {
				type: 'table',
				columns: 2,		//列数
				tableAttrs:{
					width:'100%'
				}
			},
			defaults: {
				bodyStyle: 'padding:5px',			
			},
			items:itemRush
		});
		
		var mywin = Ext.create('Ext.window.Window',{
			title:'基础信息',
			x:parent.grid_width/5,
		  y:parent.grid_height/5,
		  width: parent.grid_width/2,
			height: parent.grid_height/4,
			autoScroll: true,  
			resizable:false,
			items:mytable
		});
		mywin.show();
	}
}	

//全局函数：点击上网认证账号，查看关联身份
function funNetAuthID(){
		
	var macValue = clickRecord.data.mac;
	var vnameValue = clickRecord.data.vname;
		
	var tabPanel = new Ext.Panel({
			frame: true,
      layout: 'form',
      activeTab: 0,
			items:[{		
				html: '<iframe src="/pronline/Msg?FunName@ncsMacLog_ChosenMac_NetAuthID&mac@' + macValue + '" frameborder="no" style="width:100%;height:1000px;"></iframe>'										
			}]			
		});
		
	var winRelateId = Ext.widget('window', {
  	title: '当前上网认证账号为：' + vnameValue ,    
    x:parent.grid_width/5,
	  y:parent.grid_height/5,
	  width: parent.grid_width/3,
		height: parent.grid_height/3,
		id: 'id_vname',
		autoScroll: true, 
    resizable: false,
    draggable:true,
    modal:true,
    
    items: tabPanel
  });
       
	winRelateId.show();
}

Ext.define('proauthRzAuthlog.view.list.List1', {
	extend:'Ext.panel.Panel',
    alias : 'widget.myselfPanel',    
    tbar: [    
		/*{
			//fieldLabel:'请选择时段',
			//labelWidth: 70,			
			xtype: 'combo',
			mode: 'local',                          
			//inputType: 'button',
      triggerAction:  'all',
      forceSelection: true,      
      width:100,
      editable:       false,   
      id:             'time_flag',                           
      name:           'time_flag',
      displayField:   'name',
      valueField:     'value',
      queryMode:     'local',
      store: Ext.create('Ext.data.Store', 
      {
          fields : ['name', 'value'],
          data   : 
          [
              {name : '今天',   value: '1'},
              {name : '最近三天',  value: '2'},
              {name : '最近一周',  value: '3'},
              {name : '最近一月',   value: '4'},
              {name : '最近三月',  value: '5'},
              {name : '最近半年',  value: '6'}
          ]
      }),
      emptyText:'今天',                        
      allowBlank:true
		}, */		
		
		{			
			xtype: 'cycle',			
			showText: true,
			id: 'time_flag', 			
			menu: 
			{
        items: 
        [
            { text: '今天'},
            { text: '最近三天'},
            { text: '最近一周'},
            { text: '最近一月'},
            { text: '最近三月'},
            { text: '最近半年'} 
        ]
    	}    	  	
		},
		'-',
		'-',
		'-',
		{
			xtype:'label',
			width: 80,
			id:'id_click_leftplace',
			text:'请输入关键词'						
		},
		{				
    	xtype:'textfield',
      id:'keyword_mobileaccount',
      width:200,
      name:'export',
      //style:'color:#7aa7d5',             
      emptyText:'请输入MAC信息：XX-XX-XX-XX-XX-XX',							
			value: '',
      enableKeyEvents:true,
      listeners:{
      	'focus':function(){
        },            
        'keydown' : function(i,e){
        	var aae=e.getKey() ; 
          	if(aae==13){
            }
          }
      }
     },
     '-',
     {
     	text:'查询',
      iconCls:'accept',
      action:'keycx'
     },
     '-',
     {
     	text:'高级',
      anchor: 'right',
     	action:'advancecx',
      iconCls:'option'
     },
     '-',
     {
     	/*text:'场所选择',
     	xtype:'treecombox',
      anchor: 'right',
     	action:'choseplace',
      iconCls:'option'
      */
      
      id:'id_mac_placetree',
			name: 'groupid',
			width: 100,						
			xtype: 'treecombox',														
			valueField: 'id',									
			displayField: 'text',						
			store:store_areatree
     },               
     {
     	xtype:'button',						
			id:'id_click_leftplace',
			text:''
     }     
   ]
});


Ext.define('proauthRzAuthlog.view.list.List' ,{
    extend: 'Ext.grid.Panel',
    alias : 'widget.authloglist',
    store : 'List', 
    height:parent.grid_height+25,
    width:parent.grid_width,	
    id:'id_maclogtable',
		
    autoScroll: true,    	
		scrollOffset: 0,
    title:'',
    columnLines: true,
    initComponent: function() {	
		var sm = Ext.create('Ext.selection.CheckboxModel',{
	    listeners: {
	        selectionchange: function(sm, selections) {  
	            var grid=Ext.ComponentQuery.query('authloglist')[0];
	            grid.down('#id_joingroupcontrol').setDisabled(selections.length == 0); 
	        }
	    }
    });
    this.selModel=sm;
		
		this.tbar = Ext.create('Ext.PagingToolbar',{
	    store:'List',
	    displayInfo: true,
	    id:'id_tbar_paging',
	    autoWidth:true,
	    displayMsg: '显示{0} - {1}条记录 共{2}条记录',
	    emptyMsg: "没有记录可显示",
        items:[
				'-',
				'-',
        {
            text:'特征分析',
            anchor: 'termacttrait',
            action:'termac',
            iconCls:'tezhengfenxi'
        },
				'-',
        {
            text:'人员轨迹',
            anchor: 'termmap',
            action:'termmacmap',
            iconCls:'renyuanguiji'
        },
				'-',
        {
            text:'身份信息',
            anchor: 'idedit',
            action:'idcheck',
            iconCls:'shenfenchakan'
        },
				'-',
				{
            text:'加入布控',
            anchor: 'right',
            action:'joingroup',
            iconCls:'jiarufenzu',
            id:'id_joingroupcontrol',            
            disabled:true
        },	
				'-',
        {
            text:'导出',
            anchor: 'termexport',
            action: 'export',
            iconCls: 'exportimg'          
	
        },
        	{xtype:'label', html:'<span id="titledx"></span>'}
        ]
    });
    
    
    /* 点击终端mac 功能工具条：特征分析、身份信息、加入布控、轨迹地图 */
    
    //特征分析
    var featureAnalysis = Ext.create('Ext.Action', {
	    iconCls   : 'tezhengfenxi',  
	    text: '特征分析',	    
	    handler: function() {
	      	           		    
  			funFeatureAnalysis("1");
	    }
  	});
    
    //身份信息
  	var infoCheck = Ext.create('Ext.Action', {
      iconCls: 'shenfenchakan',
      text: '身份信息',      
      handler: function() {
      	funInfoCheck("1");        	            	
      }
    });
    
    //加入布控
  	var joinGroupControl = Ext.create('Ext.Action', {
      iconCls: 'jiarufenzu',
      text: '加入布控',      
      handler: function() {
      	funJoinGroupControl("1");        	            	
      }
    });
    
    //终端mac菜单        
    var contextMacMenu = Ext.create('Ext.menu.Menu', {
        items: [
            featureAnalysis,
            infoCheck,
            joinGroupControl            
        ]
    }); 
    
    /* 点击场所名称工具条：场所明细、电子地图 */
    var placeDetail = Ext.create('Ext.Action', {
      iconCls: 'jiarufenzu',
      text: '场所明细',      
      handler: function() {
      	funPlaceDetail();        	            	
      }
    });
    
    var electricMap = Ext.create('Ext.Action', {
      iconCls: 'electricmap',
      text: '电子地图',      
      handler: function() {
      	alert("电子地图");        	            	
      }
    });
    
    //场所名称菜单
    var contextServicenameMenu = Ext.create('Ext.menu.Menu', {
        items: [
            placeDetail,
            electricMap           
        ]
    });
    
    /* 点击上网认证账号工具条：关联身份 */
    var relateId = Ext.create('Ext.Action', {
      iconCls: 'relateid',
      text: '关联身份',      
      handler: function() {
      	 funNetAuthID();    	            	
      }
    });
    
    //上网认证账号菜单
    var contextNetAuthID = Ext.create('Ext.menu.Menu', {
        items: [
           relateId          
        ]
    });
    
    this.viewConfig = {
        stripeRows: true,
        listeners: {
            cellclick: function (table, td, cellIndex, record, tr, rowIndex, e, eOpts) {
                e.stopEvent();
                if(cellIndex == 1){
                	//parent.xxx(e.getX(), e.getY(), record);
                	//parent.contextMacMenu.showAt(e.getX()+215, e.getY()+99);
                	
                	contextMacMenu.showAt(e.getX(), e.getY());                	                	
                	Ext.getCmp("keyword_mobileaccount").setValue(record.data.mac);
                	Ext.getCmp("id_joingroupcontrol").setDisabled(false);
                	clickRecord = record;
                }
                else if(cellIndex == 2){
                	contextNetAuthID.showAt(e.getX(), e.getY());                	                	
                	Ext.getCmp("keyword_mobileaccount").setValue(record.data.vname)
                	clickRecord = record;
                }
                else if(cellIndex == 5){
                	contextServicenameMenu.showAt(e.getX(), e.getY());                	
                	clickRecord = record;
                }                		
                return false;
            }
        }
   	};
						
    this.columns = [
      			
    {header: '终端MAC地址', dataIndex: 'mac', width:130,
			renderer: ghostMac
		},
		{header: '上网认证账号', dataIndex: 'vname',  flex: 1,
			renderer: ghostVname
		},
		{header: '采集时间', dataIndex: 'stime',	width:140},			
		{header: '终端场强', dataIndex: 'rssi',   flex: 1},					
		{header: '场所名称', dataIndex: 'servicename', flex: 1,
			renderer: ghostServicename
		},			
		{header: '安全厂商', dataIndex: 'factoryname', width:80, 
			renderer:function(value,metaData,record){
		        return "上海新网程";
			}
		},
		{header: '采集设备名称', dataIndex: 'dispname', flex: 1, hidden: true},								
		{header: '场所编号', dataIndex: 'servicecode',  flex: 1, hidden: true},            
    {header: '采集设备编号', dataIndex: 'apname', flex: 1}, 
		{header: '采集设备经度', dataIndex: 'longitude', flex: 1},
    {header: '采集设备纬度', dataIndex: 'latitude', flex: 1},			            
		{header: '终端品牌', dataIndex: 'termtype', flex: 1},			              			
		{header: '相对采集设备X坐标', dataIndex: 'xpos', flex: 1, hidden: true},
    {header: '相对采集设备Y坐标', dataIndex: 'ypos', flex: 1, hidden: true},
		{header: '接入热点SSID', dataIndex: 'ssid', flex: 1},
		{header: '历史SSID列表', dataIndex: 'historyssid', flex: 1,	 
			renderer: ghostHistorySsid
		},	
		{header: '接入热点MAC', dataIndex: 'apmac', flex: 1, hidden: true},
		{header: '接入热点频道', dataIndex: 'channel', flex: 1},
		{header: '接入热点加密类型', dataIndex: 'security', flex: 1,
			renderer:function(value,metaData,record){
		        if(value=='01'){return 'WEP'}
		        else if(value=='02'){return 'WPA'}
		        else if(value=='03'){return 'WPA2'}
		        else if(value=='04'){return 'PSK'}
		        else if(value=='99'){return '其它'}
		        else {return ''}
			}            	
     }			            
    ];	
		
		this.callParent(arguments);	
   }	
});



