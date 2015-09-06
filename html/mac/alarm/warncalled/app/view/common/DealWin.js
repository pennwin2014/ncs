
Ext.define('proauthApset.view.common.DealWin' ,{
  extend: 'Ext.window.Window',
  alias : 'widget.dealwin',

	title: null,
	modal: true,
	plain:true,
	border:false,
	field1: null,
	field2: null,
	value1:null,
	modal:false,
	width:800,
	height:400,
	frame:true, 

  initComponent: function() { 
  	 Ext.apply(this,{  
  	 	   html: '<iframe id="ncs_keyword_deal" name="ncs_keyword_deal" src="/mac/alarm/warncalled/ncs_keyword_deal.htm" scrolling="no" width=100% height=100% FrameBorder=0 ></iframe>'       
          // html: '<iframe id="new_list" name="new_list" src="/mac/alarm/warncalled/mac_alarm_deal.html" scrolling="no" width=100% height=100% FrameBorder=0 ></iframe>' 
    
        });  
        var tmp = this.field1.get('name');
        
        this.items = [{xtype:'textfield',
        	hidden:true,
        	id:'name',
        	name:'name',
        	value:tmp
        	}]; 
    this.callParent(arguments);
   
    
  }
});

