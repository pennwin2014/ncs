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
               emptyText:'设备名称/设备地址/',
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
     	 	text:'增加设备',
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
     	},{
                text:'导出',
             //   iconCls:'exp',
				iconCls:'exportimg',
                handler:function(){
                document.getElementById('form1').export2.value='exp';
                document.getElementById("form1").submit();
               document.getElementById("form1").export2.value="";
                }
            }
     	,{
     	 	text:'单位组',
     	 	itemId: 'dwz',
     	 	iconCls:'add',
     	 	action:'add_group'
     	},
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
            tooltip: '点击该图标进入修改AP信息界面。',
            width: 40,
            align: 'center',
            sortable: false,
            handler: function(grid, rowIndex, colIndex, actionItem, event, record, row) {
                proauthApset.controller.Procy.show_add(record,record.get('gwid'));
          }

        },{header: '设备状态', dataIndex: 'flags',   width:70,
      	 renderer:function(value,metaData,record){
          if(value=='1'){return '<font color=green>运行</font>'}
          else {return '<font color=red>维护</font>'};
          }
      	},{header: '审核状态', dataIndex: 'flags',   width:70,
      	 renderer:function(value,metaData,record){
          if(value=='1'){return '<font color=green>已审</font>'}
          else if(value=='0'){return '<font color=red>未审</font>'};
          }
      	}, 
		 {header: '采集设备编号',     dataIndex: 'apname',width:120, sortable: true},
        {header: '采集设备名称',     dataIndex: 'disname',width:120, sortable: true},
      {header: '采集设备类型',     dataIndex: 'aptype',width:80, sortable: true,
      	  renderer:function(value,metaData,record){
          if(value=='1'){return '固定采集设备'}
          else if(value=='2'){return '移动车载设备'}
          else if(value=='3'){return '单兵采集设备'}
          else {return '其它'}
          }
      	
      	},
		 {header: '关联场所',       dataIndex: 'servicename',width:120, sortable: false},
          {header: '设备地址', dataIndex: 'address', width:160,sortable: true},
		   {header: '采集设备经度', dataIndex: 'longitude',  width:80},
      {header: '采集设备纬度', dataIndex: 'latitude',   width:80},
	   {header: '安全厂商名称',       dataIndex: 'fname',width:120, sortable: false},
	   {
          header: '维修记录',
            xtype: 'actioncolumn',
            dataIndex: 'userid',
			 hidden:true,
             icon: '/images/v4/edit_task.png',
            tooltip: '点击该图标查看维修记录详情。',
            width:80,
            align: 'center',
            sortable: false,
            handler: function(grid, rowIndex, colIndex, actionItem, event, record, row) {
				alert('弹出反馈窗口');
                proauthApset.controller.Procy.show_add(record,record.get('gwid'));
          }

        },
	   {
          header: '最新维护反馈',
            xtype: 'actioncolumn',
            dataIndex: 'userid',
             icon: '/images/v4/edit_task.png',
            tooltip: '点击该图标进入修改AP信息界面。',
            width:80,
            align: 'center',
            sortable: false,
            handler: function(grid, rowIndex, colIndex, actionItem, event, record, row) {
				alert('弹出反馈窗口');
                proauthApset.controller.Procy.show_add(record,record.get('gwid'));
          }

        },
		 {header: '上传数据间隔时间',       dataIndex: 'uptimesteps',width:120, sortable: false, hidden:true},
        {header: '采集半径',       dataIndex: 'redius',width:120, hidden:true,  hidden:true,sortable: false}, 
		  {header: '车牌号码', dataIndex: 'plate', hidden:true,   width:70},	 
        {header: '线路信息', dataIndex: 'line', hidden:true,   width:70},
		{header: '地铁车箱信息', dataIndex: 'model',  hidden:true,  width:70},
		{header: '地铁车厢编号', dataIndex: 'number', hidden:true,   width:70}
    ];

    this.callParent(arguments);
  }
    
});