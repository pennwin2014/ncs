Ext.define('proauthApset.view.list.List',{
  extend: 'Ext.grid.Panel',
  alias : 'widget.apsetlist',
  store : 'List', 
  height:parent.grid_height,
  width:parent.grid_width,
  autoScroll:true,
  title:'',
  columnLines:true,
  initComponent:function() {
    var sm = Ext.create('Ext.selection.CheckboxModel',{
        listeners: {
            selectionchange: function(sm, selections) {  
            	var grid=Ext.ComponentQuery.query('apsetlist')[0];
                  
              grid.down('#removeButton').setDisabled(selections.length == 0);
              grid.down('#shButton').setDisabled(selections.length == 0);
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
               emptyText:'厂家名称/地址',
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
     	 	text:'审核',
     	 	itemId: 'shButton',
     	 	iconCls:'add',
     	 	disabled: true,
     	 	action:'sh'
     	}
     	,
     	 {xtype:'label', html:'<span id="titledx"></span>'}
     	]
    }
    );
    
    
   
  	
    this.columns = [
     {
          header: '编辑',
            xtype: 'actioncolumn',
            dataIndex:'userid',
             icon: '/images/v4/edit_task.png',
            tooltip: '点击该图标进入修改AP信息界面。',
            width:40,
            align: 'center',
            sortable: false,
            handler: function(grid, rowIndex, colIndex, actionItem, event, record, row) {
               proauthApset.controller.Procy.show_add(record,record.get('gwid'));
          }
        },
       {header: '厂家名称',     dataIndex: 'dispname',width:120, sortable: true,flex:1},
      {header: '厂家简称',       dataIndex: 'name',width:120, sortable: false,hiddern:true,flex:1},
       {header: '厂商组织机构代码',       dataIndex: 'code',width:120, sortable: false,flex:1},
       {header: '管理场所数',       dataIndex: 'famount',width:120, sortable: false,flex:1},
      {header: '管理设备数', dataIndex: 'APamount', width:170,sortable: false,flex:1},
     	 {header: '当前维护场所数', dataIndex: 'dqamount', width:160,sortable: true,flex:1},
		  {header: '厂商在线率', dataIndex: 'online', width:160,sortable: true,flex:1},
     	 {header: '厂商地址', dataIndex: 'saddress',  width:80,flex:1},
      {header: '联系人', dataIndex: 'slink',   width:80,flex:1},
      {header: '联系电话', dataIndex: 'stel',   width:70,flex:1},
	   {header: '联系人邮箱', dataIndex: 'smail',   width:70,flex:1}
	   /*,
      {header: '审核状态', dataIndex: 'flags',   width:70,
      	 renderer:function(value,metaData,record){
          if(value=='1'){return '<font color=green>已审</font>'}
          else if(value=='0'){return '<font color=red>未审</font>'};
          }
      	}
       
      */
    ];

    this.callParent(arguments);
  }
    
});