var g_dispname;					//用户左侧点击的groupid或username
var g_typename = "";		//全局：默认从工具条场所树中选择区域名称或场所名称;
												//否则，用户是点击左侧区域名称(为0)、或场所名称(为1)
												
var g_Groupid = "";			//全局：区域名称对应 groupid 
var g_Username = "";		//全局：场所名称对应 username
																								
var clickRecord = "";   //获取超链接点击时选中行数据


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
		Ext.getCmp("id_distinctmac").setText(str);     
    //alert("左侧点击场所回调" + g_distinctCount);
               
    //将全局标志恢复默认值              
		g_typename = "";  
    g_Groupid  = "";
		g_Username = "";   
		               
	}}); 
}


/* 超链接：热点ssid、热点MAC共用超链接 */
function ghostsearch(value, p, record){
	var str = Ext.String.format('<a href="#" style="color:blue" onclick="jmpPage(\'{1}\')">{0}</a>', value, value);
	//var str = Ext.String.format('<a href="#" style="color:blue" onclick="(\'{1}\')">{0}</a>', value, value);
	
	//alert(str);
	return str;
}

//(暂未使用 2015/9/21 09:09)超链接功能：设置到输入框 
function jmpPage(mac){
	
	Ext.getCmp("keyword_mobileaccount").setValue(mac);
	
	/* IE下复制内容到剪贴板 */	
	window.clipboardData.setData('Text', mac);
	copyToClipboard(mac);		

}


/* 超链接：场所名称 */
function ghostServicename(value, p, record){
	
	//var str = Ext.String.format('<a href="#" style="color:blue" onclick="checkMacInfo(\'{1}\')">{0}</a>', value, value);
	
	var str = Ext.String.format('<a href="#" style="color:blue" onclick="(\'{1}\')">{0}</a>', value, value);	
	return str;
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


//全局函数：点击场所名称，查看电子地图
function funElectricMap(){
	var servicecodevalue; 						       
	servicecodevalue = clickRecord.data.servicecode;
	
			
	var paraTrack = '<iframe src="/mac/datamining/map/place.htm?servicecode=' + servicecodevalue + '" frameborder="no" style="width:100%;height:500px;"></iframe>';
	//alert(paraTrack);
		
	var tabPanel = new Ext.TabPanel({           
		frame: true,
    layout: 'form',
    activeTab: 0,
    items: [{
      html: paraTrack
    }]
  });
	
	var winPeopleTrack = Ext.widget('window', {
    title: '电子地图',			
    x:parent.grid_width/7,
	  y:parent.grid_height/7,
	  width: parent.grid_width/2,
		height: parent.grid_height - 120,
    resizable: false,
    draggable:true,
    modal:true,
    items: tabPanel
  });
     
  winPeopleTrack.show();

}	
	



//功能封装：获取时间下拉框 Text 对应的 value
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
			url: '/pronline/Msg?FunName@ncsMacLog_ChosenMac_ChoseQueryPlace'
		},
		root: {
			text: '全部场所',
			id: 'all',			
			expanded: true
		}
}); 

Ext.define('proauthRzAuthlog.view.list.List1', {
	extend:'Ext.panel.Panel',
  alias : 'widget.myselfPanel',
  tbar: [	
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
	{
		id:'keyword_mobileaccount',
		title:'',
		xtype:'label',
        html:'<input id="id_keyword_mobileaccount" type="text" value="请输入热点名称"/>'
	},	
	/*{			
    xtype:'textfield',
    id:'keyword_mobileaccount',
    width:160,
    name:'export',
    //style:'color:#7aa7d5',             
    emptyText:'请输入热点名称',
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
  },*/
  '-',
  {
    text:'查询',
    action:'keycx',
    iconCls:'accept'		
        
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
		id:'id_distinctmac',
		text:'',		
		focusCls:'',
		handleMouseEvents:false						
	}]
  
});


Ext.define('proauthRzAuthlog.view.list.List' ,{
  extend: 'Ext.grid.Panel',
  alias : 'widget.authloglist',
  store : 'List', 
  height:700,
  width:parent.grid_width,
  autoScroll: true,
	id:'id_hotspottable',	
  title:'',

  columnLines: true,
	initComponent: function() {
				
		this.tbar = Ext.create('Ext.PagingToolbar',{
		  store:'List',
		  displayInfo: true,
		  id:'id_tbar_paging',
		  displayMsg: '显示{0} - {1}条记录 共{2}条记录',
		  emptyMsg: "没有记录可显示",
		  items:[
			'-',
			'-',	
			{
	      text:'标记合规',
	      anchor: 'right',
	      action: 'ruleyes',
				itemId: 'idruleyes',
	      iconCls:'hege',
				disabled:true
	    },
			'-',
			{
	      text:'标记非合规',
	      anchor: 'right',
	      action: 'ruleno',
				itemId: 'idruleno',
	      iconCls:'buhege',
				disabled:true
	    },	
			'-',
			{
	      text:'电子地图',
	      anchor: 'right',
	      action:'electricmap',
	      iconCls:'maps'
	    },
			'-',                
	    {
	      text:'导出',
	      anchor: 'termexport',
	      action:'export',
	      iconCls:'exportimg'                    
	    },
      	{xtype:'label', html:'<span id="titledx"></span>'}
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
      	funElectricMap();       	            	
      }
    });
    
    //场所名称菜单选项
    var contextServicenameMenu = Ext.create('Ext.menu.Menu', {
        items: [
            placeDetail,
            electricMap           
        ]
    });	
		    
    this.viewConfig = {
        stripeRows: true,
        listeners: {
            cellclick: function (table, td, cellIndex, record, tr, rowIndex, e, eOpts) {
                e.stopEvent();
                if(cellIndex == 3){
			          	contextServicenameMenu.showAt(e.getX(), e.getY());                	
			          	clickRecord = record;
			          }
                return false;
            }
        }
   	};
            
		/* 添加复选框 */
		var sm = Ext.create('Ext.selection.CheckboxModel',{
    	listeners: {
      	selectionchange: function(sm, selections) {  
      		var grid=Ext.ComponentQuery.query('authloglist')[0];          
          var rows = Ext.getCmp('id_hotspottable').getSelectionModel();
         /* if(rows != null){
          	grid.down('#idruleyes').setDisabled(false); 
						grid.down('#idruleno').setDisabled(false);
        	}
        	else{
        		grid.down('#idruleyes').setDisabled(true); 
						grid.down('#idruleno').setDisabled(true);
        	}	
          	*/
          }
      }
   	});
   	
    this.selModel=sm; 			

    this.columns = [
			{header: '是否合规', dataIndex: 'authflag', width:50, hidden: true, 
			  renderer:function(value,metaData,record){
				if(value == '0'){
					return '<font color="red">非合规</font>'
				}
				else if(value == '1'){
					return '<font color="green">合规</font>'
				}
	      else {
					return '<font color="orange">其他</font>'
				}                                            
      }},
      {header: '热点SSID', dataIndex: 'ssid', width:110, sortable: true,
				renderer: ghostsearch},
			{header: '场所名称', dataIndex: 'servicename', width:90,
				renderer: ghostServicename
			},			
      {header: '热点频道', dataIndex: 'channel', width:70},
			{header: '热点加密类型', dataIndex: 'security', width:80,
		  	renderer:function(value,metaData,record){
	        if(value=='01'){return 'WEP'}
	        else if(value=='02'){return 'WPA'}
	        else if(value=='03'){return 'WPA2'}
	        else if(value=='04'){return 'PSK'}
	        else if(value=='99'){return '其它'}
	        else {return ''}
		    }            	
		  },
			{header: '采集时间', dataIndex: 'stime', width:140},
			{header: '热点MAC地址', dataIndex: 'apmac', width:130, sortable: false,
				renderer: ghostsearch
			},
			{header: '热点厂商', dataIndex: 'hotspotfirm', flex: 1},
			{header: '热点场强', dataIndex: 'rssi', width:70}, 
			{header: '采集设备经度', dataIndex: 'longitude', width:100},
      {header: '采集设备纬度', dataIndex: 'latitude', width:100},			
			{header: '场所编号', dataIndex: 'servicecode', flex: 1},                     
			{header: '采集设备编号', dataIndex: 'apname', width:160},						
			{header: 'X坐标', dataIndex: 'xpos', flex: 1, hidden: true},
      {header: 'Y坐标', dataIndex: 'ypos', flex: 1, hidden: true}	            					            
			                       
   ];
      
   this.callParent(arguments);
	}
    
});
