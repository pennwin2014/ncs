var g_groupidquery = ""; 
var g_usernamequery = "";
var g_dispname;


function ghostsearch(value, p, record){
	var str = Ext.String.format('<a href="#" style="color:blue" onclick="jmpPage(\'{1}\')">{0}</a>', value, value);
	//alert(str);
	return str;
}

/* 历史ssid列表 */
function ghostsearchssid(value, p, record){
	var str = Ext.String.format('<a href="#" style="color:blue" onclick="searchssid(\'{1}\')">{0}</a>', '历史ssid列表', '历史ssid列表');
	//alert(str);
	return str;
}


function searchssid(){
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
				html: '<iframe src="/mac/infoquery/macterm/historyssid/historyssid.html" frameborder="no" style="width:100%;height:1000px;"></iframe>'
			}]			
		});
		
	var win7 = Ext.widget('window', {
  	title: '当前mac：' + macvalue ,    
    x:20,
    y:80,
    width: 940,
		height: 490,
		id: 'id_historyssid',
    resizable: false,
    draggable:true,
    modal:true,
    items: tabPaneledit
  });
       
	win7.show();			
}	


/* 新增手机号搜索 */
function setFieldVname(vname){
	Ext.getCmp("keyword_mobileaccount").setValue(vname);
	var store = Ext.ComponentQuery.query('authloglist')[0].getStore();
	store.load({params:{vname:vname}});
} 
 
 
/* 设置场所名称，直接可在后台查询 ncsuser 表中的 username */
function setSearchValue(typename, dispname){
	
	g_dispname = dispname;
	
  /* 将传入 mac 设置到输入框 */
	Ext.getCmp("keyword_mobileaccount").setValue( '@' + dispname + '@');			
	if(typename == '0'){		
		try{
			
			g_groupidquery = parent.getGlobalGroupId();	
						
		}
		catch(e){
			alert(e.message);
		}		
	}
	else{		
		try{
									
			g_usernamequery = parent.getGlobalUserName();
						
		}
		catch(e){
			alert(e.message);
		}		
	}	
				
	var store = Ext.ComponentQuery.query('authloglist')[0].getStore();
	
	// 添加 callback 获取json中去重数量，并覆写原来的 tbar 中的 displayMsg  
	store.load({callback : function(r, option, success) {   
            	 var responseText =  eval("("+option.response.responseText+")");        		                
               var g_distinctCount = "";
               g_distinctCount = responseText.distinctCount;
              
               var playInfo = Ext.getCmp('id_tbar_paging');
               playInfo.displayMsg = '显示{0} - {1}条记录 共{2}条记录,去重后共<span style="color:red;font-size:1.5em">' + g_distinctCount + '</span>条记录';
              
               playInfo.doRefresh();	
               
   }}); 
}
  
 
function setFieldMac(mac){
	Ext.getCmp("keyword_mobileaccount").setValue(mac);
	
	var store = Ext.ComponentQuery.query('authloglist')[0].getStore();
	store.load({params:{mac:mac}});	
}


function copy_to_clipboard(text) {
    try {
        netscape.security.PrivilegeManager.enablePrivilege("UniversalXPConnect");
    } catch (e) {
        alert('您的Firefox安全设置限制了您进行剪贴板操作，请打开"about:config"将"signed.applets.codebase_principal_support"设置为"true"之后重试。');
        return false;
    }
    var clip = Components.classes['@mozilla.org/widget/clipboard;1'].createInstance(Components.interfaces.nsIClipboard);
    if (!clip)
        return;
    var trans = Components.classes['@mozilla.org/widget/transferable;1'].createInstance(Components.interfaces.nsITransferable);
    if (!trans)
        return;
    trans.addDataFlavor('text/unicode');
    var str = Components.classes["@mozilla.org/supports-string;1"].createInstance(Components.interfaces.nsISupportsString);
    str.data = text;
    trans.setTransferData("text/unicode", str, text.length * 2);
    var clipid = Components.interfaces.nsIClipboard;
    if (!clip)
        return false;
    clip.setData(trans, null, clipid.kGlobalClipboard);
}

function jmpPage(mac){
	
	
	
	Ext.getCmp("keyword_mobileaccount").setValue(mac);
  var timeFlag = Ext.getCmp('time_flag').value;  
  parent.showMacWindow(mac, timeFlag);
  
	window.clipboardData.setData('text', mac);				
	copyToClipboard(mac);
	//copy_to_clipboard(mac);
	
	Ext.getCmp("keyword_mobileaccount").setValue(mac);	
				
	//parent.jumpToDestPage('itm_infoquery_mac', "ncViewer.mac_infoquery_mac","MAC日志查询");
	//mac;
}

Ext.define('proauthRzAuthlog.view.list.List1', {
	extend:'Ext.panel.Panel',
    alias : 'widget.myselfPanel',
    tbar: [
		{
			fieldLabel:'请选择时段',
			labelWidth: 70,
			xtype:'combo',mode:'local',                          
            triggerAction:  'all',
            forceSelection: true,
            width:170,
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
            emptyText:'今天',allowBlank:true
        },
		'-',
		{
			fieldLabel:'请输入关键词',
			labelWidth: 80,	
            xtype:'textfield',
            id:'keyword_mobileaccount',
            width:260,
            name:'export',
            //style:'color:#7aa7d5',             
            emptyText:'请输入MAC信息: XX-XX-XX-XX-XX-XX',
						//value: '64-27-37-23-5B-7C',			
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
        },'-',{
            text:'查询',
            iconCls:'accept',
            action:'keycx'
        },'-',{
            text:'高级',
            anchor: 'right',
            action:'advancecx',
            iconCls:'option'
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
		//renderTo: 'contenttbar',
    autoScroll: true,    
		//forceFit: true,
		scrollOffset: 0,
    title:'',
    columnLines: true,
    initComponent: function() {	
		var sm = Ext.create('Ext.selection.CheckboxModel',{
            listeners: {
                selectionchange: function(sm, selections) {  
                    var grid=Ext.ComponentQuery.query('authloglist')[0];
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
                    text:'身份查看',
                    anchor: 'idedit',
                    action:'idcheck',
                    iconCls:'shenfenchakan'
                },
				'-',
				{
                    text:'加入分组',
                    anchor: 'right',
                    action:'joingroup',
                    iconCls:'jiarufenzu',
                    //disabled:true
                    disabled:false
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
						
        this.columns = [
        			
      {header: '终端MAC地址', dataIndex: 'mac', width:130,
				renderer: ghostsearch
			},
			{header: '上网认证账号', dataIndex: 'vname',  flex: 1},
			{header: '采集时间', dataIndex: 'stime',   flex: 1},			
			{header: '终端场强', dataIndex: 'rssi',   flex: 1},					
			{header: '场所名称', dataIndex: 'servicename', flex: 1},			
			{header: '采集设备名称', dataIndex: 'dispname', flex: 1, hidden: true},								
			{header: '场所编号', dataIndex: 'servicecode',  flex: 1, hidden: true},            
      {header: '采集设备编号', dataIndex: 'apname', flex: 1}, 
			{header: '采集设备经度', dataIndex: 'longitude', flex: 1},
      {header: '采集设备纬度', dataIndex: 'latitude', flex: 1},			            
			{header: '终端品牌', dataIndex: 'termtype', flex: 1},			              			
      //{header: '终端历史SSID列表', dataIndex: 'apssid', flex: 1, sortable: true, hidden: true},
			{header: '相对采集设备X坐标', dataIndex: 'xpos', flex: 1, hidden: true},
      {header: '相对采集设备Y坐标', dataIndex: 'ypos', flex: 1, hidden: true},
			{header: '接入热点SSID', dataIndex: 'ssid', flex: 1},
			{header: '历史SSID列表', dataIndex: 'historyssid', flex: 1,
				/*renderer:function(value,metaData,record){
					if(value == '0'){
						return '历史ssid列表'
					}
        	else {
						return '历史ssid列表'
					}				
				},*/ 
				renderer: ghostsearchssid
			},	
			{header: '接入热点MAC', dataIndex: 'apmac', flex: 1, hidden: true,
				renderer: ghostsearch
			},
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



