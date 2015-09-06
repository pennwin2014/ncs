
 Ext.define('ncViewer.proauthZtsz', {
    extend: 'Ext.panel.Panel',
    alias: 'widget.proauthZtsz',
    height:900,
     autoscroll:false,  
    initComponent: function(){
        Ext.apply(this,{   
        	 margins: '0 0 0 0',      
        	 frame: false,
           html: '<iframe id="lan_ztsz_s" name="lan_ztsz_s" src="/systemset/status/proauth_statusset.htm" scrolling="no" width=100% height=900 FrameBorder=0 ></iframe>' 
    
        });

        this.callParent(arguments);
    }
});