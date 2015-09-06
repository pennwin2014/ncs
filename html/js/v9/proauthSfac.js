
 Ext.define('ncViewer.proauthSfac', {
    extend: 'Ext.panel.Panel',
    alias: 'widget.proauthSfac',
    height:900,
     autoscroll:false,  
    initComponent: function(){
        Ext.apply(this,{   
        	 margins: '0 0 0 0',      
        	 frame: false,
           html: '<iframe id="lan_sfac_s" name="lan_sfac_s" src="/v4/ncs_joinuser_list.htm" scrolling="no" width=100% height=900 FrameBorder=0 ></iframe>' 
    
        });

        this.callParent(arguments);
    }
});