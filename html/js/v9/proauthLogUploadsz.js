
 Ext.define('ncViewer.proauthLogSavesz', {
    extend: 'Ext.panel.Panel',
    alias: 'widget.proauthLogSavesz',
    height:900,
     autoscroll:false,  
    initComponent: function(){
        Ext.apply(this,{   
        	 margins: '0 0 0 0',      
        	 frame: false,
           html: '<iframe id="lan_loguploadsz_s" name="lan_loguploadsz_s" src="/systemset/logftp/proauth_logftp.htm" scrolling="no" width=100% height=900 FrameBorder=0 ></iframe>' 
    
        });

        this.callParent(arguments);
    }
});