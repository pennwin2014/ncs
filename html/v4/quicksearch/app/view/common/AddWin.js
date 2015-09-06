Ext.define('quickResarch.view.common.AddWin' ,{
  extend: 'Ext.window.Window',
  alias : 'widget.addwin',

	title: null,
	modal: true,
	field1: null,
	field2: null,
	value1:null,

  initComponent: function() {
    this.items = [{
      xtype: 'form',
      items:[{
        xtype:"hidden",
        name:'value1',
        value:this.value1
      },{
        xtype: 'textfield',
        labelWidth: 60,
        margin: "5 5 5 5",
        fieldLabel: this.field1,
        name: 'field1',
        hidden: this.field1==null
      },{
        xtype: 'textfield',
        margin: "5 5 5 5",
        labelWidth: 60,
        fieldLabel: this.field2,
        name: 'field2',
        hidden: this.field2==null
      }]
    }];
    
    this.buttons=[{
      text: this.title,
      action: 'add'
    },{
      text: 'È¡Ïû',
      scope: this,
      handler: this.close
    }];
    
    this.callParent(arguments);
  }
});