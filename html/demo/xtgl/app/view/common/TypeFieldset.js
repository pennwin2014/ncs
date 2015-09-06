Ext.define('LanProcy.view.common.TypeFieldset',{
  extend: 'Ext.form.FieldSet',
  alias : 'widget.typefieldset',
  
  gid: null,
  combo: null,
  columns: null,
  store: null,
  item1: null,
  item2: null,
  item3: null,
  
  initComponent: function() { 	
  	this.items = [{
  	  xtype: 'grid',
  	  id: this.gid,
  	  width: 400,
  	  height: 200,
  	  tbar: [this.combo,{
  	    text: '编辑',
  	    action: 'edit'  
  	  }],
  	  rbar:[{
  	    text: '增加',
  	    action: 'add'
  	  },{
  	    text: '修改',
  	    action: 'mod'
  	  },{
  	    text: '删除',
  	    action: 'del'
  	  },{
  	    text: '删除全部',
  	    action: 'delall'
  	  }],
  	  store: this.store,
  	  columns: this.columns
  	},this.item1,this.item2,this.item3];
     	
  	this.callParent(arguments);
  }
});