
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
                    id:             'State_flag',                           
                    name:           'State_flag',
                    displayField:   'name',
                    valueField:     'value',
                    queryMode:     'local',
                    store: Ext.create('Ext.data.Store', 
                    {
                        fields : ['name', 'value'],
                        data   : 
                        [
                            {name : '合规',   value: '1'},
                            {name : '非法',  value: '2'},
                            {name : '可疑',  value: '3'}
                          
                        ]
                    }),
                    emptyText:'合规',allowBlank:true
                },
                {
                    xtype:'textfield',
                    id:'macInfo',
                    width:220,
                    name:'macInfo',
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
                    action:'normally_query'
                },
                '-',
             
             
                {
                    text:'添加',
                    anchor: 'termacttrait',
                    action:'normally_add',
                    iconCls:'add'
                },
				'-',
              /*  {
                    text:'删除',
                    anchor: 'termmap',
                    action:'normally_delete',
                    iconCls:'remove'
                },
				'-',
                {
                    text:'接警记录',
                    anchor: 'idedit',
                    action:'normally_log',
                    iconCls:'add'
                },
				'-',
                {
                    text:'接警处理',
                    anchor: 'idedit',
                    action:'normally_act',
                    iconCls:'option'
                },
				'-',*/
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
			{header: '接入热点SSID', dataIndex: 'ssid', width:140,sortable: true},
			{header: '是否合规', dataIndex: 'onlinestatus',   width:140},
			{header: '接入热点加密类型', dataIndex: 'security',   width:140,
            	 renderer:function(value,metaData,record){
                    if(value=='01'){return 'WEP'}
                    else if(value=='02'){return 'WPA'}
                    else if(value=='03'){return 'WPA2'}
                    else if(value=='04'){return 'PSK'}
                    else if(value=='99'){return '其它'}
                    else {return ''}
                }
            	
            },	
			{header: '允许的场所服务类型', dataIndex: 'vtype',  width:140},			
            {header: '允许部署场所的性质', dataIndex: 'vnature',width:140,sortable: false},


			{header: '运营商联系人', dataIndex: 'OperatorContact',   width:140},            
            {header: '运营商联系电话', dataIndex: 'OperatorContactNumber', width:140}, 
			{header: '运营商类型邮件', dataIndex: 'OperatorMail',  width:140},
           
			                       
        ];
     
        this.callParent(arguments);
    }// initComponent: function() {
    
});
