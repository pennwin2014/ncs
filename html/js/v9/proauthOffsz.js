
 Ext.define('ncViewer.proauthOffsz', {
    extend: 'Ext.panel.Panel',
    alias: 'widget.proauthOffsz',
    height:900,
     autoscroll:false,  
    initComponent: function(){
        Ext.apply(this,{   
        	 margins: '0 0 0 0',      
        	 frame: false,
           html: '<iframe id="lan_offsz_s" name="lan_offsz_s" src="/systemset/warnoff/proauth_warnoff.htm" scrolling="no" width=100% height=900 FrameBorder=0 ></iframe>' 
    
        });

        this.callParent(arguments);
    }
});