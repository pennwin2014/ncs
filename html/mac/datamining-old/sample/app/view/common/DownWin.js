Ext.define('proauthRzAuthlog.view.common.DownWin' ,{
  extend: 'Ext.window.Window',
  alias : 'widget.downwin',

	title: null,
	modal: true,
	field1: null,
	field2: null,
	value1:null,

  initComponent: function() {
  
var grid = new Ext.grid.GridPanel({
      
       height:500,
      columns: [
        { text: '报表名称', dataIndex: 'cid' },
        {
            text: '生成时间', dataIndex: 'cid',width:150
          
        },
         {
             header: '下载',
            xtype: 'actioncolumn',
            dataIndex: 'cid',
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

