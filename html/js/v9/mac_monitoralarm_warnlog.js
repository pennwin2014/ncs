
 Ext.define('ncViewer.mac_monitoralarm_warnlog', {
     extend: 'Ext.panel.Panel',
    alias : 'widget.mac_monitoralarm_warnlog',
   height:900,
     autoscroll:false,
    initComponent: function(){
    	
    	var theight=grid_height+30;
    	
        Ext.apply(this,{
                 margins: '0 0 0 0',
                 frame: false,
           //html: '<iframe id="mac_monitoralarm_warnlog_s" name="mac_monitoralarm_warnlog_s" src="/mac/alarm/mac_warnlog.html" scrolling="no" width=100% height=900 FrameBorder=0 ></iframe>'
					html: '<iframe id="mac_monitoralarm_warnlog_s" name="mac_monitoralarm_warnlog_s" src="/mac/alarm/mac_warnlog.html" scrolling="no" width=100% height='+ theight +' FrameBorder=0 ></iframe>'
						
        });

        this.callParent(arguments);
    }
 });
 
   