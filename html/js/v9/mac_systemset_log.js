
 Ext.define('ncViewer.mac_systemset_log', {
    extend: 'Ext.panel.Panel',
    alias: 'widget.mac_systemset_log',
    height:900,
     autoscroll:false,  
    initComponent: function(){
        Ext.apply(this,{   
        	 margins: '0 0 0 0',      
        	 frame: false,
           html: '<iframe id="lan_itm_systemset_log_s" name="lan_itm_systemset_log_s" src="/mac/systemadmin/mac_systemset_log.html" scrolling="no" width=100% height=900 FrameBorder=0 ></iframe>' 
    
        });

        this.callParent(arguments);
    }
});