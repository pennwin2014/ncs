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
        labelWidth: 60,
        width:140,
        fieldLabel:'任务状态',
        valueField:'value',
        displayField: 'name',
        id:'statway',
        name : 'statway',      
        value:'3',
        allowBlank: true,
        forceSelection: false,
                                    store: Ext.create('Ext.data.Store', {
                                    fields : ['name', 'value'],
                                    data   : [
                                        {name : '全部',   value: '3'},
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
			/*Ext.create('Ext.tip.ToolTip', {
          target: 'keyword',
          html: '跟随鼠标的提示',
          trackMouse: true            //  跟随鼠标移动
      });*/

        this.tbar = Ext.create('Ext.PagingToolbar',{
            store:'List',
            displayInfo: true,
            displayMsg: '显示{0} - {1}条记录 共{2}条记录',
            emptyMsg: "没有记录可显示",
            items:[              
               Combo
                ,
               '  ',
               {
                    iconCls:'question',
                    action:'help',
                    hidden:true
                    },
                {
                    xtype:'textfield',
                    id:'keyword',
                    width:180,
                    name:'keyword',
                    style:'color:#7aa7d5',             
                    emptyText:'请输入任务名称关键词',
                    value:'',
                    enableKeyEvents:true,
                    listeners:{
                        'focus':function(){
                        },           
                        'keydown' : function(i,e){
                             var aae=e.getKey() ; 
                             if(aae==13){
                                proauthRzAuthlog.controller.Procy.searchtype = ""; 
        proauthRzAuthlog.controller.Procy.SetPage(1);  
        proauthRzAuthlog.controller.Procy.loadProcyListStore();
                             }
                         }
                    }
                },'  ',
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
                   
                },
                '-',
                 {
                    text:'删除',
                    iconCls:'remove',
                    action:'del'
                },
                '-',
                 {
                    text:'生效',
                    iconCls:'true',
                    action:'active',
                     itemId:'activebutton',
                      disabled:true
                   
                },
                '-',
                 {
                    text:'失效',
                    iconCls:'false',
                    action:'disable',
                    itemId:'disacbutton',
                     disabled:true
                },'-',
                
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
          header: '编辑',
            xtype: 'actioncolumn',
            dataIndex: 'cid',
             icon: '/v4/images/edit_task.png',
            tooltip: '点击该图标进入修改界面。',
            width: 40,
            align: 'center',
            sortable: false,
           handler: function(grid, rowIndex, colIndex, actionItem, event, record, row) {
                   
                  var win = Ext.create('proauthRzAuthlog.view.common.UpdateWin',{
                            title: "编辑抽样分析",
                            field1: record

                          });
                 

                          win.showAt(340,20);        
                
               
            }

        },
            {header: 'cid', dataIndex: 'cid',align:'center', flex:1,sortable: true, hidden:true},
            {header: '任务名称', dataIndex: 'taskname',align:'center', flex:1,sortable: true},
            {header: '任务状态', dataIndex: 'result',  flex:1, align:'center',    renderer:function(value,metaData,record){
                    if(value=='0'){return '<font color="green">生效</font>'}
                    else if(value=='1'){return '<font color="red">失效</font>'}
                        else {return '<font color="orange">完成</font>'}
                   
                }            },
            {header: '任务描述',dataIndex: 'taskdesc',align:'center', flex:1,sortable: false},
            {header: '产生记录数', dataIndex: 'maccount',align:'center', flex:1,sortable: false},
            {header: '最新报表产生时间', dataIndex: 'reporttime',align:'center', flex:1,sortable: false},          
             {
             header: '报表列表',
            xtype: 'actioncolumn',
            dataIndex: 'cid',
             icon: '/newver/resources/themes/images/share/tezhengfenxi.png',
            tooltip: '点击下载最新报表',
            flex:1,
            align: 'center',
            sortable: false,
           handler: function(grid, rowIndex, colIndex, actionItem, event, record, row) {
                
                  var win = Ext.create('proauthRzAuthlog.view.common.DownWin',{
                            title: "下载列表",
                            field1:record

                          });
                          win.showAt(340,20);        
                
               
            }

        }
        ];
     
        this.callParent(arguments);
    }
    
});
