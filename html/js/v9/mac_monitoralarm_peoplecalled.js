
 Ext.define('ncViewer.mac_monitoralarm_peoplecalled', {
    extend: 'Ext.panel.Panel',
    alias: 'widget.mac_monitoralarm_peoplecalled',
    height:900,
     autoscroll:false,  
    initComponent: function(){
        Ext.apply(this,{   
        	 margins: '0 0 0 0',      
        	 frame: false,
           html: '<iframe id="lan_itm_alarm_peoplecalled|_s" name="lan_itm_alarm_peoplecalled|_s" src="/mac/alarm/warncalled/mac_monitoralarm_peoplecalled.htm" scrolling="no" width=100% height=900 FrameBorder=0 ></iframe>' 
    
        });

        this.callParent(arguments);
    }
});