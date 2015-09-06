Ext.define('proauthXtgl.view.list.List' ,{
  extend: 'Ext.grid.Panel',
  alias : 'widget.xtgllist',
  store : 'List', 
 height:parent.grid_height+5,
 autoScroll: true,
  title:'',
  columnLines: true,
  initComponent: function() {

    var sm = Ext.create('Ext.selection.CheckboxModel',{
        listeners: {
            selectionchange: function(sm, selections) {  
            	var grid=Ext.ComponentQuery.query('xtgllist')[0];
                  
              grid.down('#removeButton').setDisabled(selections.length == 0);
             
    
                
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
               id:'keyword',
               width:160,
               style:'color:#7aa7d5',             
               emptyText:'输登录名\姓名查询',
               enableKeyEvents:true,
               listeners:{
                 'focus':function(){
        
                 },
         
                 'keydown' : function(i,e){
                var aae=e.getKey() ; 
                if(aae==13){
                   proauthXtgl.controller.Procy.loadProcyListStore();
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
     	 	text:'增加',
     	 	iconCls:'add',
     	 	action:'add'
     	},'-',
     	 {
     	 	text:'删除',
     	 	itemId: 'removeButton',
     	 	iconCls:'remove',
     	 	disabled: true,
     	 	action:'del'
     	},'-',
     	 {
     	 	text:'权限管理',
     	 	iconCls:'add',
     	 	action:'manager'
     	},'-',
		
     	 {xtype:'label', html:'<span id="titledx"></span>'}
     	]
    }
    );
    
    
   
  	
    this.columns = [
            {
          header: '编辑',
            xtype: 'actioncolumn',
            dataIndex: 'userid',
             icon: '/images/v4/edit_task.png',
            tooltip: '点击该图标进入修改单位信息界面。',
            width: 40,
            align: 'center',
            sortable: false,
            handler: function(grid, rowIndex, colIndex, actionItem, event, record, row) {
            
               proauthXtgl.controller.Procy.showedit(record.get('userid'),record);
          }

        },
      {header: '登录名',     dataIndex: 'username', width:100, sortable: true},
      {header: '姓名',       dataIndex: 'dispname',width:110, sortable: false},
      {header: '行政区划',       dataIndex: 'groupname',width:110, sortable: false},
      {header: '管理区域', dataIndex: 'opt',flex:1,sortable: false        
      },
     {header: '管理接入厂商', dataIndex: 'fac',flex:1,sortable: false},
      {header: '权限角色', dataIndex: 'role', flex:1,sortable: true},
      {header: '联系方式', dataIndex: 'email', flex:1,sortable: true},
      {header: '首页显示', dataIndex: 'levelname', flex:1,sortable: true}

      
    ];

    this.callParent(arguments);
  }
    
});