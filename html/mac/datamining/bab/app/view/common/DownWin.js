
Ext.define('proauthRzAuthlog.view.common.DownWin' ,{
  extend: 'Ext.window.Window',
  alias : 'widget.downwin',

	title: null,
	modal: false,
	plain:true,
	border:false,
	field1: null,
	field2: null,
	value1:null,
	modal:true,
	width:700,
	height:350,
	frame:true, 

  initComponent: function() { 
  	 Ext.apply(this,{  
  	 	       
           html: '<iframe id="new_list" name="new_list" src="/mac/datamining/bab/list.htm" scrolling="no" width=100% height=100% FrameBorder=0 ></iframe>' 
    
        });  
        var tmp = this.field1.get('cid');
        var tmpmac = this.field1.get('mac');
        this.items = [{xtype:'textfield',
        	hidden:true,
        	id:'cid',
        	name:'cid',
        	value:tmp
        	},
        	{xtype:'textfield',
        	hidden:true,
        	id:'mac',
        	name:'mac',
        	value:tmpmac
        	}]; 
    this.callParent(arguments);
   
    
  }
});

