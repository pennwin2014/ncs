Ext.define('proauthRzAuthlog.view.list.List' ,{
    extend: 'Ext.grid.Panel',
    alias : 'widget.authloglist',
    store : 'List', 
    height:parent.grid_height,
    width:parent.grid_width,
    autoScroll: true,
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
   
   
        var actionCombo = Ext.create('Ext.form.field.ComboBox', {
        fieldLabel: '日志类型',
        labelWidth: 60,
        width:150,
        valueField:'value',
        displayField: 'name',
        name : 'shflag',      
        value:'2',
        allowBlank: true,
        forceSelection: false,
                                    store: Ext.create('Ext.data.Store', {
                                    fields : ['name', 'value'],
                                    data   : [
                                        {name : '离线日志',   value: '2'},
                                        {name : '上线日志',   value: '1'}
                                                                      
                                    ]
                                })
                                ,
        queryMode: 'local',
        typeAhead: true,
                listeners: {
			          'change':function(){
			          	proauthRzAuthlog.controller.Procy.oprFlag=this.value;
			          	
		//	              document.getElementById('form1').shflag.value=this.value;
    //                reflash()
			            }
			        }
    });
   
   
        this.tbar = Ext.create('Ext.PagingToolbar',{
            store:'List',
            displayInfo: true,
            displayMsg: '显示{0} - {1}条记录 共{2}条记录',
            emptyMsg: "没有记录可显示",
            items:[
                   actionCombo,
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
                    width:140,
                    name:'keyword5',
                    style:'color:#7aa7d5',             
                    emptyText:'帐号/MAC/场所代码',
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
                {xtype:'label', html:'<span id="titledx"></span>'}
            ]// items:[
        });//this.tbar = Ext.create('Ext.PagingToolbar',{

        this.columns = [
            {header: '认证类型',dataIndex: 'atype',width:100, sortable: false,
                renderer:function(value,metaData,record){
                    if(value=='1020004'){return '手机认证'}
                    else if(value=='1020100'){return 'APP验证'}
                    else if(value=='1020002'){return 'MAC验证'}
                    else if(value=='1021000'){return '证件号验证'}
                    else {return value}
                }
            },
            {header: '认证 帐号', dataIndex: 'authAccount',width:110,sortable: false},
            {header: '场所编码', dataIndex: 'servicecode', width:110,sortable: true},
            {header: '场所名称', dataIndex: 'servicename',  width:110},
            {header: '上线时间', dataIndex: 'stime',   width:110},
            {header: '下线时间', dataIndex: 'etime',  width:110},
            {header: '内网IP', dataIndex: 'sip',   width:110},
            {header: '外网IP', dataIndex: 'oip',   width:110},
            {header: '外网端口', dataIndex: 'sport',   width:110},
            {header: '终端MAC', dataIndex: 'mac',   width:110},
            {header: 'AP编号', dataIndex: 'apname',  width:110},
            {header: 'AP MAC', dataIndex: 'apmac',  width:110},
            {header: 'AP经度', dataIndex: 'longitude',  width:110},
            {header: 'AP 纬度', dataIndex: 'latitude', width:110},
            {header: '场强', dataIndex: 'rssi',  width:110},
            {header: '会话ID', dataIndex: 'sessionid',width:110},
            {header: '行为', dataIndex: 'flags',width:110}
        ];
     
        this.callParent(arguments);
    }// initComponent: function() {
    
});
