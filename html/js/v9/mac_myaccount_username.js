
 Ext.define('ncViewer.mac_myaccount_username', {
    extend: 'Ext.panel.Panel',
    alias: 'widget.mac_myaccount_username',
    height:900,
     autoscroll:false,  
    initComponent: function(){
        Ext.apply(this,{   
        	 margins: '0 0 0 0',      
        	 frame: false,
           html: '<iframe id="lan_itm_myaccount_username_s" name="lan_itm_myaccount_username_s" src="/mac/systemadmin/username/proauth_usernameset.htm" scrolling="no" width=100% height=900 FrameBorder=0 ></iframe>' 
    
        });

        this.callParent(arguments);
    }
});