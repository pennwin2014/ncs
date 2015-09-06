            			       			
Ext.define('proauthMobileAccount.view.common.WarnlogWin' ,{
  extend: 'Ext.window.Window',
  alias : 'widget.warnlogwin',

	title: '告警信息',
	modal: true,
	field1: null,
	field2: null,
	value1:null,
  initComponent: function() {

         var cid=this.field1.get('sid');
       
  //布控告警信息
         var macWarnlog=	Ext.create('proauthMobileAccount.view.common.macWarnlogList',{field1: cid});
    
  	    this.items = [macWarnlog];  
        this.callParent(arguments);

   

  }
});  
