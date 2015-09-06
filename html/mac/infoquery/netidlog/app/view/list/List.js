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
                    width:180,
                    name:'keyword5',
                    style:'color:#7aa7d5',             
                    emptyText:'终端MAC/虚拟身份帐号',
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
            {header: '终端MAC', dataIndex: 'mac',width:110,sortable: false},
            {header: '虚拟类型', dataIndex: 'vtype',   width:110},
            {header: '虚拟帐号', dataIndex: 'vname',   width:110},
            {header: '场所名称', dataIndex: 'servicename',  width:110},
            {header: '捕获时间', dataIndex: 'stime',   width:110},
            {header: 'AP编号', dataIndex: 'apname',   width:110},
            {header: 'AP经度', dataIndex: 'longitude',  width:110},
            {header: 'AP 纬度', dataIndex: 'latitude', width:110},
            {header: '认证帐号', dataIndex: 'username',  width:110},
            {header: '上网IP', dataIndex: 'sip',  width:110},
            {header: '目标IP', dataIndex: 'dip',  width:110}
        ];
     
        this.callParent(arguments);
    }// initComponent: function() {
    
});
