function ghostsearch(value, p, record){
	var str = Ext.String.format('<a href="#" style="color:blue" onclick="jmpPage(\'{1}\')">{0}</a>', value, value);
	//alert(str);
	return str;
}

function jmpPage(mac){
	
	Ext.getCmp("keyword_mobileaccount").setValue(mac);
	
	//alert(mac + ' 已经拷贝到剪切板');	 
	window.clipboardData.setData('Text', mac);
	copyToClipboard(mac);
}

function ghostsearchApname(value, p, record){
	var str = Ext.String.format('<a href="#" style="color:blue" onclick="jmpPageAp(\'{1}\')">{0}</a>', value, value);
	//alert(str);
	return str;
}

function jmpPageAp(apname){
	
	Ext.getCmp("keyword_mobileaccount").setValue(apname);	
	
	//alert(apname + ' 已经拷贝到剪切板')
	window.clipboardData.setData('Text', apname);	
	copyToClipboard(apname);
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
				id:        		'time_flag',                           
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
            },{
				fieldLabel:'请输入关键词',
				labelWidth: 80,	
                xtype:'textfield',
                id:'keyword_mobileaccount',
                width:260,
                name:'keyword5',
                //style:'color:#7aa7d5',             
                //emptyText:'热点MAC/SSID/场所代码/AP MAC',
				emptyText:'移动采集设备名称/编号',
				//value: '3100123450034CBA0BA2B',
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
            },
			'-',
			{
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
            id:'id_tbar_paging',
            displayMsg: '显示{0} - {1}条记录 共{2}条记录',
            emptyMsg: "没有记录可显示",
            items:[ 
				'-',
				'-',	
				{
                    text:'轨迹地图',
                    anchor: 'termmap',
                    action:'termmacmap',
                    iconCls:'renyuanguiji'
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
			{header: '采集设备名称', dataIndex: 'dispname', width: 150, hidden: true},
      {header: '采集设备编号', dataIndex: 'apname', width: 160,
				renderer: ghostsearchApname },
			{header: '安全厂商名称', dataIndex: 'factoryname', flex: 1, hidden: true},
			{header: '采集设备经度', dataIndex: 'longitude', flex: 1,sortable: true},
      {header: '采集设备纬度', dataIndex: 'latitude', flex: 1},
			{header: '采集时间', dataIndex: 'stime',  width: 110},
			{header: '终端MAC', dataIndex: 'mac',  width: 130,
				renderer: ghostsearch
			},
			{header: '被采集终端场强', dataIndex: 'rssi', flex: 1},
			{header: '身份类型', dataIndex: 'vtype', hidden: true, flex: 1,
            	 renderer:function(value,metaData,record){
                    if(value=='0'){return '真实身份'}
                    else if(value=='1'){return '虚拟身份'}
                    else {return ''}
            		}
      },
      {header: '身份内容', dataIndex: 'vname', flex: 1, hidden: true},
			{header: '接入热点SSID', dataIndex: 'ssid', flex: 1},
			{header: '接入热点MAC', dataIndex: 'apmac',  width: 130,
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
            	
            },			
			{header: '安全厂商组织机构代码', dataIndex: 'factorycode', flex: 1, hidden: true}
			                       
        ];
     
        this.callParent(arguments);
    }// initComponent: function() {
    
});









