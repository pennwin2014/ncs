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
        var timesCombo = Ext.create('Ext.form.field.ComboBox', {
        fieldLabel:'指定时间',
        labelWidth: 60,
        width:160,
        valueField:'value',
        displayField: 'name',
        id:'times',
        name : 'times',      
        value:'0',
        allowBlank: true,
        forceSelection: false,
                                    store: Ext.create('Ext.data.Store', {
                                    fields : ['name', 'value'],
                                    data   : [
                                        {name : '今天',   value: '0'},
                                        {name : '前三天',   value: '1'},
                                        {name : '前一周',   value: '2'},
                                        {name : '前一月',   value: '3'},
                                         {name : '前三月',   value: '3'},
                                          {name : '前半年',   value: '3'}
                                        
                                                                
                                    ]
                                })
                                ,
        queryMode: 'local',
        typeAhead: true,
                listeners: {
                      'change':function(){
                      
                        }
                    }
        });
        var stylesCombo = Ext.create('Ext.form.field.ComboBox', {
        labelWidth: 60,
        width:75,
        valueField:'value',
        displayField: 'name',
        id:'styles',
        name : 'styles',      
        value:'0',
        allowBlank: true,
        forceSelection: false,
                                    store: Ext.create('Ext.data.Store', {
                                    fields : ['name', 'value'],
                                    data   : [
                                        {name : 'WEP',   value: '0'},
                                        {name : 'WPA',   value: '1'},
                                        {name : 'WPA2',   value: '2'},
                                        {name : 'PSK',   value: '3'},
                                        {name : '其他',   value: '4'}
                                                                
                                    ]
                                })
                                ,
        queryMode: 'local',
        typeAhead: true,
                listeners: {
                      'change':function(){
                      
                        }
                    }
        });
         var statusCombo = Ext.create('Ext.form.field.ComboBox', {
        labelWidth: 60,
        width:75,
        valueField:'value',
        displayField: 'name',
        id:'places',
        name : 'places',      
        value:'0',
        allowBlank: true,
        forceSelection: false,
                                    store: Ext.create('Ext.data.Store', {
                                    fields : ['name', 'value'],
                                    data   : [
                                        {name : '非法',   value: '0'},
                                        {name : '合规',   value: '1'},
                                        {name : '可疑',   value: '2'}
                                                                
                                    ]
                                })
                                ,
        queryMode: 'local',
        typeAhead: true,
                listeners: {
                      'change':function(){
                      
                        }
                    }
        });

        this.tbar = Ext.create('Ext.PagingToolbar',{
            store:'List',
            displayInfo: true,
            displayMsg: '显示{0} - {1}条记录 共{2}条记录',
            emptyMsg: "没有记录可显示",
            items:[
                timesCombo,
                '  ',
                {
                    xtype:'textfield',
                    id:'keyword',
                    width:180,
                    name:'keyword',
                    style:'color:#7aa7d5',             
                    emptyText:'请输入热点SSID关键词',
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
                '-',
                {
                    text:'查询',
                    iconCls:'accept',
                    action:'keycx'
                },
                '-',
               /* {
                    text:'高级',
                    iconCls:'option',
                    action:'add'
                },
                '-',*/
                {
                    text:'加入合规库',
                    anchor: 'right',
                    action:'active',
                    iconCls:'add',
                    disabled:true	,
                    itemId:'activebutton'
                },
                 '-',
                {
                    text:'标记为非法',
                    anchor: 'right',
                    action:'disable',
                    iconCls:'remove',
                    disabled:true,
                    itemId:'disacbutton'
                },
                '-',
             {
			     	 	text:'导出',
			     	 	itemId: 'expbutton',
			     	 	iconCls:'exp',
			     	 	action:'exp'
			     	 },
                {xtype:'label', html:'<span id="titledx"></span>'}
            ]
        });
    var sm = Ext.create('Ext.selection.CheckboxModel',{
        listeners: {
            selectionchange: function(sm, selections) {  
            	var grid=Ext.ComponentQuery.query('authloglist')[0];                  
                grid.down('#activebutton').setDisabled(selections.length == 0); 
               grid.down('#disacbutton').setDisabled(selections.length == 0);    
            }
        }
    
    });
   
     this.selModel=sm;
        this.columns = [
           
            {
                header: '是否合规', dataIndex: 'inSecurity', align: 'center', flex: 1, sortable: false, renderer: function (value, metaData, record) {
                    if(value=='0'){return '<font color="green">不合规</font>'}
                    else if (value == '1') { return '<font color="red">合规</font>' }
                    else { return '<font color="orange">其他</font>' }
                   
                }},
            { header: '热点SSID', dataIndex: 'inSsid', align: 'center', flex: 1, sortable: true },
            { header: '场所名称', dataIndex: 'inServiceName', align: 'center', flex: 1, sortable: false },
            {header: '热点加密类型', dataIndex: 'inSecurity', align: 'center', flex: 1}, 
            { header: '出现场所数量', dataIndex: 'count', align: 'center', flex: 1, sortable: true },
          
            { header: '最早采集时间', dataIndex: 'firsttime', align: 'center', flex: 1},
            { header: '最近采集时间', dataIndex: 'lasttime', align: 'center', flex: 1}

        ];
     
        this.callParent(arguments);
    }
    
});
