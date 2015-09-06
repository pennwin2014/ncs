var g_groupidquery = ""; 
var g_usernamequery = "";
var g_dispname;

function ghostsearch(value, p, record){
	var str = Ext.String.format('<a href="#" style="color:blue" onclick="jmpPage(\'{1}\')">{0}</a>', value, value);
	//alert(str);
	return str;
}

/* 设置场所名称，直接可在后台查询 ncsuser 表中的 username */
function setSearchValue(typename, dispname){
	g_dispname = dispname;
	
	var store = Ext.ComponentQuery.query('authloglist')[0].getStore();		
	var groupid="";
	var username="";	
	var bRet = false;
	
    /* 将传入 mac 设置到输入框 */
	Ext.getCmp("keyword_mobileaccount").setValue( '@' + dispname + '@');	
	if(typename == '0'){		
		try{
			
			g_groupidquery = parent.getGlobalGroupId();	
			groupid = g_groupidquery;
			//alert(typename + ',' + groupid);		
			if(groupid != '')
				bRet = true;
		}
		catch(e){
			alert(e.message);
		}
		
	}
	else{
		
		try{
						
			g_usernamequery = parent.getGlobalUserName();			
			username = g_usernamequery;
			if(username != '')
						bRet = true;
			//alert(typename + ',,' + username);		
			
		}
		catch(e){
			alert(e.message);
		}
		
	}
	
	if(bRet == false){
					groupid = "all";
	}
			
	var keyword = Ext.getCmp('keyword_mobileaccount').value;				
	store.load({params:{keyword:keyword, groupid:groupid, username:username}});
	
}
 

function jmpPage(mac){
	
	Ext.getCmp("keyword_mobileaccount").setValue(mac);	
	
	/* IE下复制内容到剪贴板 */	
	window.clipboardData.setData('Text', mac);	 	
	copyToClipboard(mac);
	
}


Ext.define('proauthRzAuthlog.view.list.List1', {
	extend:'Ext.panel.Panel',
    alias : 'widget.myselfPanel',
    tbar: [
			{
				fieldLabel:'请输入关键词',
				labelWidth: 80,
                xtype:'textfield',
                id:'keyword_mobileaccount',
                width:250,
                name:'keyword5',                  
                emptyText:'请输入MAC信息/场所名称',				
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
            },{
                text:'查询',
								action:'keycx',
								iconCls:'accept'			
            }
    ]
});

Ext.define('proauthRzAuthlog.view.list.List' ,{
    extend: 'Ext.grid.Panel',
    alias : 'widget.authloglist',
    store : 'List', 
    height:parent.grid_height+25,
    width:parent.grid_width,
    autoScroll: true,
		viewConfig:{
    	loadMask : false
    },
    title:'',
	
    columnLines: true,
    initComponent: function() {
		
		/* 添加复选框 */
		/* var sm = Ext.create('Ext.selection.CheckboxModel',{
            listeners: {
                selectionchange: function(sm, selections) {  
                    var grid=Ext.ComponentQuery.query('authloglist')[0];
                }
            }
        });
        this.selModel=sm; */
		
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
                    text:'人流分析',
                    anchor: 'right',
                    action:'electricmap',
                    iconCls:'maps'
                },
				'-',
				{
                    text:'查看图表',
                    anchor: 'right',
                    action:'checkgraph',
                    iconCls:'chakantubiao'
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

        this.columns = [
			{header: '场所名称', dataIndex: 'servicename', width:100,
				renderer: ghostsearch},	
			{header: '采集设备名称', dataIndex: 'dispname', width:100},
			{header: '采集时间', dataIndex: 'stime', width:140},
			{header: '最早终端场强', dataIndex: 'lssi', flex: 1},
			{header: '最近终端场强', dataIndex: 'ussi', flex: 1},
			{header: '终端MAC地址', dataIndex: 'mac', width:130, sortable: false,
				renderer: ghostsearch
			},
			{header: '采集设备经度', dataIndex: 'longitude', flex: 1, hidden: true},
      {header: '采集设备纬度', dataIndex: 'latitude', flex: 1, hidden: true},
			{header: '接入热点SSID', dataIndex: 'ssid', flex: 1,sortable: true},
			{header: '接入热点MAC', dataIndex: 'apmac', width:130,
				renderer: ghostsearch
			},
			{header: '接入热点加密类型', dataIndex: 'security', flex: 1,
            	 renderer:function(value,metaData,record){
                    if(value=='01'){return 'WEP'}
                    else if(value=='02'){return 'WPA'}
                    else if(value=='03'){return 'WPA2'}
                    else if(value=='04'){return 'PSK'}
                    else if(value=='99'){return '其它'}
                    else {return ''}
                }
            	
      },			
			{header: '场所编号', dataIndex: 'servicecode', flex: 1},            
			{header: '采集设备编号', dataIndex: 'apname', flex: 1}, 			
      {header: '身份内容', dataIndex: 'vname', flex: 1},				         
			{header: '终端品牌', dataIndex: 'termtype', flex: 1}			                       
        ];
     
        this.callParent(arguments);
    }
    
});
