
 Ext.define('ncViewer.proauthLogSavesz', {
    extend: 'Ext.panel.Panel',
    alias: 'widget.proauthLogSavesz',
    height:900,
     autoscroll:false,  
    initComponent: function(){
        Ext.apply(this,{   
        	 margins: '0 0 0 0',      
        	 frame: false,
           html: '<iframe id="lan_logsavesz_s" name="lan_logsavesz_s" src="/systemset/logsave/proauth_logsave.htm" scrolling="no" width=100% height=900 FrameBorder=0 ></iframe>' 
    
        });

        this.callParent(arguments);
    }
});