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
       
         var Combo = Ext.create('Ext.form.field.ComboBox', {
        fieldLabel:'任务状态',
        labelWidth: 60,
        width:140,
        valueField:'value',
        displayField: 'name',
        id:'statway',
        name : 'statway',      
        value:'0',
        allowBlank: true,
        forceSelection: false,
                                    store: Ext.create('Ext.data.Store', {
                                    fields : ['name', 'value'],
                                    data   : [
                                       
                                        {name : '生效',   value: '0'},
                                        {name : '失效',   value: '1'},
                                        {name : '完成',   value: '2'}
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
             
                Combo,
                 '  ' ,
                {
                    xtype:'textfield',
                    id:'keyword_mobileaccount',
                    width:180,
                    name:'keyword_bab',
                    style:'color:#7aa7d5',             
                    emptyText:'请输入任务名称关键词',
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
                    text:'新建',
                    iconCls:'add',
                    action:'add'
                }
                 ,
                '-',
                 {
                    text:'生效',
                    iconCls:'true',
                    action:'add',
                    itemId:'activebutton',
                     disabled:true
                 
                }
                 ,
                '-',
                 {
                    text:'失效',
                    iconCls:'false',
                    action:'add',
                    itemId:'disacbutton',
                    disabled:true
                   

                }
                 ,
                '-',
               
                 {
                    text:'删除',
                    iconCls:'remove',
                    action:'remove'
                },
                '-',
             {
			     	 	text:'导出',
			     	 	itemId: 'expbutton',
			     	 	iconCls:'exp',
			     	 	action:'exp'
			     	 },
                {xtype:'label', html:'<span id="titledx"></span>'}
            ]// items:[
        });//this.tbar = Ext.create('Ext.PagingToolbar',{
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
          header: '编辑',
            xtype: 'actioncolumn',
            dataIndex: 'sid',
             icon: '/v4/images/edit_task.png',
            tooltip: '点击该图标进入修改界面。',
            width: 40,
            align: 'center',
            sortable: false,
            handler: function(grid, rowIndex, colIndex, actionItem, event, record, row) {
                
                  var win = Ext.create('proauthRzAuthlog.view.common.AddWin',{
                            title: "新建",
                            field1: record
                    
                          });
                          win.showAt(300,20);        
                
               
            }

        },
             {header: '任务状态', dataIndex: 'vara',  align:'center', width:70,
            renderer:function(value,metaData,record){
                    if(value=='v0'){return '<img  src="/newver/resources/themes/images/map/qualified.png">'}
                    else if(value=='v1'){return '<img  src="/newver/resources/themes/images/map/unqualified.png">'}
                   
                }
                 
            },    
            {header: '任务名称', dataIndex: 'vara',align:'center',width:110,sortable: false},
            {header: '任务描述', dataIndex: 'checked1', width:110,align:'center',sortable: true},
             {header: '产生记录数', dataIndex: 'checked1',align:'center', width:110,sortable: true},
              {header: '最新报表产生时间', dataIndex: 'checked1', align:'center',width:110,sortable: true},

            { header: '最新报表',
            xtype: 'actioncolumn',
            dataIndex: 'sid',
             icon: '/newver/resources/themes/images/share/tezhengfenxi.png',
            tooltip: '点击下载最新报表',
            width: 90,
            align: 'center',
            sortable: false,
            handler: function(grid, rowIndex, colIndex, actionItem, event, record, row) {

               
            }}
        ];
     
        this.callParent(arguments);
    }
    
});
