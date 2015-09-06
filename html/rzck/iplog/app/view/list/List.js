Ext.define('proauthRzSmslog.view.list.List' ,{
    extend: 'Ext.grid.Panel',
    alias : 'widget.smsloglist',
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
                var grid=Ext.ComponentQuery.query('smsloglist')[0];
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
            {
                xtype:'combo',mode:'local',                          
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
             emptyText:'账号/MAC/场所代码',
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
         ]
    }
    );
    
      
    this.columns = [
      {header: '账号', dataIndex: 'authAccount',width:110,sortable: false},
      {header: '时间', dataIndex: 'stime', width:110, sortable: false},
      {header: '终端MAC', dataIndex: 'mac',   width:110},
      {header: '场所编码', dataIndex: 'servicecode', width:110,sortable: true},
      {header: '场所名称', dataIndex: 'servicename',  width:110},
      {header: '应用类别', dataIndex: 'nettype',  width:110},
      {header: '内网IP', dataIndex: 'sip',   width:110},
      {header: '内网端口', dataIndex: 'sport1',   width:110},
      {header: '外网IP', dataIndex: 'oip',   width:110},
      {header: '外网端口', dataIndex: 'sport',   width:110},
      {header: '目的IP', dataIndex: 'dip',   width:110},
      {header: '目的端口', dataIndex: 'dport',   width:110},
      {header: 'AP编号', dataIndex: 'apname',  width:110},
      {header: 'AP MAC', dataIndex: 'apmac',  width:110},
      {header: 'AP经度', dataIndex: 'longitude',  width:110},
      {header: 'AP 纬度', dataIndex: 'latitude', width:110},
      {header: '会话id', dataIndex: 'sessionid',  width:110 }
    ];


    this.callParent(arguments);
  }
    
});
