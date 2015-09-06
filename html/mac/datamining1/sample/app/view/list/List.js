
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
            emptyMsg: "没有记录可显示",
            items:[
                   {xtype:'combo',mode:'local',                          
                    triggerAction:  'all',
                    forceSelection: true,
                    width:140,
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
                {
                    xtype:'textfield',
                    id:'keyword_mobileaccount',
                    width:220,
                    name:'keyword5',
                    style:'color:#7aa7d5',             
                    emptyText:'请输入MAC信息: XX-XX-XX-XX-XX-XX',
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
                    text:'特征分析',
                    anchor: 'termacttrait',
                    action:'termac',
                    iconCls:'option'
                },
				'-',
                {
                    text:'人员轨迹',
                    anchor: 'termmap',
                    action:'termmacmap',
                    iconCls:'option'
                },
				'-',
                {
                    text:'身份编辑',
                    anchor: 'idedit',
                    action:'edit',
                    iconCls:'option'
                },
				'-',
                {
                    text:'导出',
                    anchor: 'termexport',
                    action:'export',
                    iconCls:'exportimg'
                },
                {xtype:'label', html:'<span id="titledx"></span>'}
            ]// items:[
        });//this.tbar = Ext.create('Ext.PagingToolbar',{

        this.columns = [
			{header: '在线状态', dataIndex: 'onlinestatus',   width:110},
            {header: '终端MAC地址', dataIndex: 'mac',width:110,sortable: false},
			{header: '采集时间', dataIndex: 'stime',   width:110},
			{header: '终端场强', dataIndex: 'rssi',   width:110},
			{header: '身份类型', dataIndex: 'vtype',  width:110,
            	 renderer:function(value,metaData,record){
                    if(value=='0'){return '真实身份'}
                    else if(value=='1'){return '虚拟身份'}
                    else {return ''}
            }},
            {header: '身份内容', dataIndex: 'vname',  width:110},			
			{header: '场所名称', dataIndex: 'servicename',  width:110},			
			{header: '采集设备名称', dataIndex: 'apname',   width:110},
			{header: '终端品牌', dataIndex: 'termtype',  width:110},			              			
            {header: '终端历史SSID列表', dataIndex: 'apssid', width:110,sortable: true},
			{header: '接入热点SSID', dataIndex: 'ssid', width:110,sortable: true},
			{header: '接入热点MAC', dataIndex: 'apmac',  width:110},
			{header: '接入热点频道', dataIndex: 'channel',  width:110},
			{header: '接入热点加密类型', dataIndex: 'security',   width:110,
            	 renderer:function(value,metaData,record){
                    if(value=='01'){return 'WEP'}
                    else if(value=='02'){return 'WPA'}
                    else if(value=='03'){return 'WPA2'}
                    else if(value=='04'){return 'PSK'}
                    else if(value=='99'){return '其它'}
                    else {return ''}
                }
            	
            },			
			{header: '场所编号', dataIndex: 'servicecode',   width:110},            
            {header: '采集设备编号', dataIndex: 'apcollectcode', width:110}, 
			{header: '采集设备经度', dataIndex: 'longitude',  width:110},
            {header: '采集设备纬度', dataIndex: 'latitude', width:110},
			{header: '相对采集设备X坐标', dataIndex: 'xcoord',  width:110},
            {header: '相对采集设备Y坐标', dataIndex: 'ycoord', width:110}
			
            //{header: '安装地点', dataIndex: 'address',  width:110},
			                       
        ];
     
        this.callParent(arguments);
    }// initComponent: function() {
    
});
