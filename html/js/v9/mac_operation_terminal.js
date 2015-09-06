
 Ext.define('ncViewer.mac_operation_terminal', {
    extend: 'Ext.panel.Panel',
    alias: 'widget.mac_operation_terminal',
    height:900,
     autoscroll:false,  
    initComponent: function(){
        Ext.apply(this,{   
        	 margins: '0 0 0 0',      
        	 frame: false,
         //  html: '<iframe id="lan_itm_operation_equipment_s" name="lan_itm_operation_equipment_s" src="/mac/operation/terminal/mac_operation_terminal.htm" scrolling="no" width=100% height=900 FrameBorder=0 ></iframe>' 
		   html: '<iframe id="lan_itm_operation_equipment_s" name="lan_itm_operation_equipment_s" src="/mac/v4/case/ncs_case_keyword.htm" scrolling="no" width=100% height=900 FrameBorder=0 ></iframe>' 
    
        });

        this.callParent(arguments);
    }
});