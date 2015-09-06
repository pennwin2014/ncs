
 Ext.define('ncViewer.mac_monitoralarm_controlmanage', {
    extend: 'Ext.panel.Panel',
    alias: 'widget.mac_monitoralarm_controlmanage',
    height:900,
     autoscroll:false,  
    initComponent: function(){
        Ext.apply(this,{   
        	 margins: '0 0 0 0',      
        	 frame: false,
      //       html: '<iframe id="monitoralarm" name="monitoralarm" src="/mac/alarm/mac_monitoralarm_controlmanage.html" scrolling="no" width=100% height=900 FrameBorder=0 ></iframe>' 
      html: '<iframe id="lan_itm_alarm_controlmanage_s" name="lan_itm_alarm_controlmanage_s" src="/mac/alarm/mac_bk.html" scrolling="no" width=100% height=900 FrameBorder=0 ></iframe>' 
        });

        this.callParent(arguments);
    }
});