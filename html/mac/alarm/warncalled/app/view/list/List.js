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
            //  grid.down('#shButton').setDisabled(selections.length == 0);
                
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
               emptyText:'姓名/所属部门/联系电话',
               enableKeyEvents:true,
               listeners:{
                 'focus':function(){
                 },
                 'keydown' : function(i,e){
                var aae=e.getKey() ; 
				var store = Ext.ComponentQuery.query('apsetlist')[0].getStore();
                if(aae==13){
				 var aa=Ext.getCmp('keyword_dwgl').value;
                    store.load({params:{keyword:aa}});  
                }
               }
               }
             },
             {
                text:'查询',
                iconCls:'accept',
               // action:'keycx'
			    handler: function(){
					//alert("查询");
					var store = Ext.ComponentQuery.query('apsetlist')[0].getStore();
                 var aa=Ext.getCmp('keyword_dwgl').value;              
                if(aa!='姓名/所属部门/联系电话'){
                	store.load({params:{keyword:aa}});					
                 }
                }
   
            },
         '-',
     	 {
     	 	text:'新增',
     	 	iconCls:'add',
     	 	action:'add'
     	},'-',
     	 {
     	 	text:'删除',
     	 	itemId: 'removeButton',
     	 	iconCls:'remove',
     	 	disabled: true,
     	 	action:'del'
     	},'-',{
     	 	text:'导出',
     	 	itemId: 'export',
     	 	iconCls:'exportimg',
     	 	action:'export'
     	},'-',
		/*,'-',
     	 {
     	 	text:'审核',
     	 	itemId: 'shButton',
     	 	iconCls:'add',
     	 	disabled: true,
     	 	action:'sh'
     	}*/
     	 {xtype:'label', html:'<span id="titledx"></span>'}
     	],
    }
    );
    
    
   
  	
    this.columns = [
     {
          header: '编辑',
            xtype: 'actioncolumn',
            dataIndex: 'sid',
		//	id:'sid',
             icon: '/images/v4/edit_task.png',
            tooltip: '点击进行编辑',
            width: 40,
            align: 'center',
            sortable: false,
            handler: function(grid, rowIndex, colIndex, actionItem, event, record, row) {
                proauthApset.controller.Procy.show_add(record,record.get('gwid'));
          }
        },
        {header: '姓名',dataIndex: 'name',width:120, sortable: true,flex:1},
		 {header: '所属部门',dataIndex: 'department',width:120, sortable: false,flex:1},
          {header: '联系人电话',dataIndex: 'phone', width:160,sortable: true,flex:1},
		{header: '联系邮箱',dataIndex: 'email',  width:80,flex:1},
         {header: '最新接警时间',dataIndex: 'calledtime',   width:80,flex:1},
	     {header: '待处理接警数',dataIndex: 'dealcount',   width:80,flex:1},
	    {header: '接警总数',dataIndex: 'calledcount',width:120, sortable: false,flex:1},
	     {
          header: '告警处理',
            xtype: 'actioncolumn',
            dataIndex: 'userid',
             icon: '/images/v4/edit_task.png',
            tooltip: '点击该图标进入修改AP信息界面。',
            width:80,
            align: 'center',
            sortable: false,
              handler: function(grid, rowIndex, colIndex, actionItem, event, record, row) {  
                  proauthApset.controller.Procy.show_deal(record,record.get('gwid'));          
            }	

        } 
    ];

    this.callParent(arguments);
  }
    
});