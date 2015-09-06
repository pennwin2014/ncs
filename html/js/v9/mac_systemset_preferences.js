
 Ext.define('ncViewer.mac_systemset_preferences', {
    extend: 'Ext.panel.Panel',
    alias: 'widget.proauthZtsz',
    height:900,
     autoscroll:false,  
    initComponent: function(){
        Ext.apply(this,{   
        	 margins: '0 0 0 0',      
        	 frame: false,
          html: '<iframe id="lan_itm_systemset_preferences_s" name="lan_itm_systemset_preferences_s" src="/xtgl/ncs_role_list.htm" scrolling="no" width=100% height=900 FrameBorder=0 ></iframe>' 
		
        });

        this.callParent(arguments);
    }
});