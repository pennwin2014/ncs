Ext.define('LanProcy.view.common.GroupComTree' ,{
  extend: 'Ext.window.Window',
  alias : 'widget.groupcomtree',
  
  title: "选择下发部门或者计算机",
  width: 200,
  height:300,
  modal: true,
 // autoShow: true,

  initComponent: function() {
    
   	this.items = [{
    	xtype: 'treepanel',
    	frame: true,
    	padding: 5,
      height:300,
    	rootVisible: false,
    	store:Ext.create('Ext.data.TreeStore', {
   	    fields:['id','text','pid'],
        proxy: {
          type: 'ajax',
          url: '/pronline/Msg?FunName@lanTree_depComp'
        },
        root: {
          text: '全部',
          id: '',
          expanded: true
        },
        folderSort: true
		  })
	  }],
	  
	  this.buttons = [
   		{text:"保存",action:'save'},
   		{text:"关闭",action:'close',scope: this,handler:this.close}
   	];

    this.callParent(arguments);
  }
});