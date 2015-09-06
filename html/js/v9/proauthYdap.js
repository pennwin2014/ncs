
 Ext.define('ncViewer.proauthYdap', {
    extend: 'Ext.panel.Panel',
    alias: 'widget.proauthYdap',
    height:900,
     autoscroll:false,  
    initComponent: function(){
        Ext.apply(this,{   
        	 margins: '0 0 0 0',      
        	 frame: false,
           html: '<iframe id="infoquery_move_m" name="infoquery_move_m" src="/mobileap/proauth_mobileap.htm" scrolling="no" width=100% height=900 FrameBorder=0 ></iframe>' 
    
        });

        this.callParent(arguments);
    }
});