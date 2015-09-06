
 Ext.define('ncViewer.mac_myaccount_toolbar', {
    extend: 'Ext.panel.Panel',
    alias: 'widget.mac_myaccount_toolbar',
    height:900,
     autoscroll:false,  
    initComponent: function(){
        Ext.apply(this,{   
        	 margins: '0 0 0 0',      
        	 frame: false,
           html: '<iframe id="lan_itm_myaccount_toolbar_s" name="lan_itm_myaccount_toolbar_s" src="/mac/systemadmin/mytoolbar/proauth_toolbarset.htm" scrolling="no" width=100% height=900 FrameBorder=0 ></iframe>' 
    
        });

        this.callParent(arguments);
    }
});