
 Ext.define('ncViewer.mac_systemset_admin', {
    extend: 'Ext.panel.Panel',
    alias: 'widget.mac_systemset_admin',
    height:900,
     autoscroll:false,  
    initComponent: function(){
        Ext.apply(this,{   
        	 margins: '0 0 0 0',      
        	 frame: false,
           html: '<iframe id="lan_itm_systemset_admin_s" name="lan_itm_systemset_admin_s" src="/xtgl/ncs_adminuser_list.htm" scrolling="no" width=100% height=900 FrameBorder=0 ></iframe>' 
    
        });

        this.callParent(arguments);
    }
});