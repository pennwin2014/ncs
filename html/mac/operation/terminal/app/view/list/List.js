Ext.define('proauthApset.view.list.List' ,{
  extend: 'Ext.grid.Panel',
  alias : 'widget.apsetlist',
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
            	var grid=Ext.ComponentQuery.query('apsetlist')[0];
              grid.down('#removeButton').setDisabled(selections.length == 0);
             // grid.down('#shButton').setDisabled(selections.length == 0);
                
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
               xtype:'textfield',
               id:'keyword_dwgl',
               width:160,
               name:'keyword5',
               style:'color:#7aa7d5',             
               emptyText:'请输入名称',
               enableKeyEvents:true,
               listeners:{
                 'focus':function(){
                 },
         
                 'keydown' : function(i,e){
                var aae=e.getKey(); 
                if(aae==13){
                   proauthApset.controller.Procy.loadProcyListStore();
			//	var  store=Ext.ComponentQuery.query('apsetlist')[0].getStore();
				var   keyword=Ext.getCmp('keyword_dwgl').value;
				store.load({params:{keyword:keyword}});       
                }
               }
               }
             },
             {
                text:'查询',
                iconCls:'accept',
                action:'keycx'
   
            },
         '-',{
     	 	text:'删除',
     	 	itemId: 'removeButton',
     	 	iconCls:'remove',
     	 	disabled: true,
     	 	action:'del'
     	},'-',
     	 {
     	 	text:'增加分组',
     	 	iconCls:'add',
     	 	action:'add'
     	},
     	 '-',{
     	 	text:'增加组成员',
     	 	iconCls:'add',
     	 	action:'addgroup'
     	},
     	 /*{
     	 	text:'审核',
     	 	itemId: 'shButton',
     	 	iconCls:'add',
     	 	disabled: true,
     	 	action:'sh'
     	}
     	,*/
     	 {xtype:'label', html:'<span id="titledx"></span>'}
     	]
    }
    );
    
    
   
  	
    this.columns = [
     {
          header: '编辑',
            xtype: 'actioncolumn',
            dataIndex: 'groupid',
             icon: '/images/v4/edit_task.png',
            tooltip: '点击该图标进入修改终端信息。',
            width: 40,
            align: 'center',
            sortable: false,
            handler: function(grid, rowIndex, colIndex, actionItem, event, record, row) {
                proauthApset.controller.Procy.show_add(record,record.get('gwid'));
          }
    
        },{header: '终端分组名称',     dataIndex: 'groupname',width:150, sortable: true,flex:1},
        {header: '终端分组成员信息',       dataIndex: 'mac',width:150, sortable: false,flex:1},
	    {header: '分组描述', dataIndex: 'groupdesc',  width:150,flex:1},
	   {
          header: '关联分析任务',
            xtype: 'actioncolumn',
            dataIndex: 'glfxtask',
             icon: '/images/v4/edit_task.png',
            tooltip: '点击该图标进入修改终端信息。',
            width: 80,
            align: 'center',
            sortable: false,
            handler: function(grid, rowIndex, colIndex, actionItem, event, record, row) {
                proauthApset.controller.Procy.show_add(record,record.get('gwid'));
          }
    
        }
    ];

    this.callParent(arguments);
  }
    
});