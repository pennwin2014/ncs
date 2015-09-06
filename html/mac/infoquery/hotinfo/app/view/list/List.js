function ghostsearch(value, p, record){
	var str = Ext.String.format('<a href="#" style="color:blue" onclick="jmpPage(\'{1}\')">{0}</a>', value, value);
	//alert(str);
	return str;
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
            emptyText:'请输入热点名称',
						value: '',
						//value: '31001234567892',
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
            action:'keycx',
            iconCls:'accept'		
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
		id:'id_hotspottable',
		/*viewConfig:{
    	loadMask : false
    },*/
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
			{header: '场所名称', dataIndex: 'servicename', width:90},			
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
			{header: '采集时间', dataIndex: 'stime', flex: 1},
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
    }// initComponent: function() {
    
});
