
 Ext.define('proauthRzAuthlog.view.common.DealWin' ,{
	extend: 'Ext.window.Window',
	alias : 'widget.DealWin',

		title: null,
		modal: true,
		field1: null,
		field2: null,
		value1:null,

	initComponent: function() {
  
	var grid = new Ext.grid.GridPanel({
      
       height:500,
      columns: [
        { text: '布控任务', dataIndex: 'cid' },
        {
            text: '告警时间', dataIndex: 'cid',width:150
          
        },
         {
             header: '处理状态',
            xtype: 'actioncolumn',
            dataIndex: 'flags',
             icon: '/newver/resources/themes/images/share/xiazai.png',
            tooltip: '点击下载最新报表',
            width: 100,
            align: 'center',
            sortable: false,
           handler: function(grid, rowIndex, colIndex, actionItem, event, record, row) {
          
            }
        }
		]
   
    
	});
  	 this.items = [grid];
    
    this.callParent(arguments);
    
	}
	});
