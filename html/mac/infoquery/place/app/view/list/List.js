function ghostsearch(value, p, record){
	var str = Ext.String.format('<a href="#" style="color:blue" onclick="jmpPage(\'{1}\')">{0}</a>', value, value);
	//alert(str);
	return str;
}
var g_groupidquery = ""; 
var g_usernamequery = "";
var g_dispname;

/* 设置场所名称，直接可在后台查询 ncsuser 表中的 username */
function setSearchValue(typename, dispname){
	
	g_dispname = dispname;
	
    /* 将传入 mac 设置到输入框 */
	Ext.getCmp("keyword_mobileaccount").setValue( '@' + dispname + '@');	
	
	
	if(typename == '0'){
		
		try{
			
			g_groupidquery = parent.getGlobalGroupId();	
			
			//alert(g_groupidquery);		
		}
		catch(e){
			alert(e.message);
		}
		
	}
	else{
		
		try{
						
			g_usernamequery = parent.getGlobalUserName();
			
			//alert(g_usernamequery);
		}
		catch(e){
			alert(e.message);
		}
		
	}	
				
	var store = Ext.ComponentQuery.query('authloglist')[0].getStore();	
	
	/* 添加 callback 获取json中去重数量，并覆写原来的 tbar 中的 displayMsg  */
	store.load({callback : function(r, option, success) {   
            	var responseText =  eval("("+option.response.responseText+")");        		                
               var g_distinctCount = "";
               g_distinctCount = responseText.distinctCount;
               
               var playInfo = Ext.getCmp('id_tbar_paging');
               playInfo.displayMsg = '显示{0} - {1}条记录 共{2}条记录,去重后共<span style="color:red;font-size:1.5em">' + g_distinctCount + '</span>条记录';
              
               playInfo.doRefresh();
               
            
   }
  }); 
	
}


function ghostsearchPlace(value, p, record){
	var str = Ext.String.format('<a href="#" style="color:blue" onclick="jmpPagePlace(\'{1}\')">{0}</a>', value, value);
	//alert(str);
	return str;
}

function jmpPagePlace(place){
	
	Ext.getCmp("keyword_mobileaccount").setValue(place);	
	
	//alert(place + ' 已经拷贝到剪切板')
	window.clipboardData.setData('Text', place);	
	copyToClipboard(place);
}


function jmpPage(mac){
	
	Ext.getCmp("keyword_mobileaccount").setValue(mac);
	//alert(mac + ' 已经拷贝到剪切板');	
	
	/* IE下复制内容到剪贴板 */	
	window.clipboardData.setData('Text', mac);
	copyToClipboard(mac);		
			
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
            emptyText: '场所名称/场所编码/场所经营法人',
						//value:'test_zone3',			
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
    autoScroll: true,
	
    title:'',
	
    columnLines: true,
    initComponent: function() {
		
		/* 添加复选框 */
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
            displayMsg: '显示{0} - {1}条记录 共{2}条记录',
            id:'id_tbar_paging',
            //displayMsg: '显示{0} - {1}条记录 共{2}条记录,去重后共{3}条记录',
            emptyMsg: "没有记录可显示",
            items:[ 
				'-',
				'-',	
                {
                    text:'电子地图',
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
                {xtype:'label', id:'id_label', html:'<span id="titledx"></span>'}
            ]
        });

        this.columns = [
			{header: '场所名称', dataIndex: 'servicename', flex: 1,
				renderer: ghostsearchPlace },
			{header: '采集设备名称', dataIndex: 'dispname', flex: 1, hidden: true},	
			{header: '安全厂商名称', dataIndex: 'factoryname', flex: 1, hidden: true},
      {header: '采集时间', dataIndex: 'stime',  flex: 1},			
      {header: '采集终端MAC', dataIndex: 'mac', flex: 1,sortable: false,
				renderer: ghostsearch
			},
			{header: '被采集终端场强', dataIndex: 'rssi', flex: 1},							
      {header: '接入热点SSID', dataIndex: 'ssid', flex: 1,sortable: true},
			{header: '接入热点MAC', dataIndex: 'apmac', flex: 1,
				renderer: ghostsearch
			},
			{header: '接入热点频道', dataIndex: 'channel', width:110},
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
			{header: '场所编号', dataIndex: 'servicecode', width: 120}, 
			{header: '采集设备编号', dataIndex: 'apname', width: 160}, 
			{header: '场所详细地址', dataIndex: 'address', flex: 1, hidden: true},
			//{header: '设备地址', dataIndex: 'address', flex: 1}, 
			{header: '安全厂商组织机构代码', dataIndex: 'factorycode', flex: 1, hidden: true}
			                       
        ];
     
        this.callParent(arguments);
    }// initComponent: function() {
    	
});
