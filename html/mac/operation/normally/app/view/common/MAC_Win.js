
Ext.define('proauthApset.view.common.MAC_Win' ,{
  extend: 'Ext.window.Window',
  alias : 'widget.no_MAC_Win',
	title:'max',
	modal: true,
	//plain:true,
	border:false,
	field1: null,
	field2: null,
	value1:null,
	modal:true,
	width:300,
	height:400,
	frame:false, 
  initComponent: function() { 
	Ext.apply(this,{  
		html:'<iframe id="new_normally_mac" name="new_normally_mac" src="/mac/operation/normally/ncs_keyword_mac.htm" scrolling="no" width=100% height=100% FrameBorder=0 ></iframe>'       
        });  
        var ssid= this.field1;
        this.items = [{xtype:'textfield',
					hidden:true,
					id:'new_ssid_mac',
					name:'ssid',
					value:ssid
        	}]; 
    this.callParent(arguments);
   
    
  }
});

