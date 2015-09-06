
 Ext.define('ncViewer.proauthSysuser', {
    extend: 'Ext.panel.Panel',
    alias: 'widget.proauthSysuser',
    height:900,
     autoscroll:false,  
    initComponent: function(){
        Ext.apply(this,{   
        	 margins: '0 0 0 0',      
        	 frame: false,
           html: '<iframe id="lan_sysuser_s" name="lan_sysuser_s" src="/xtgl/ncs_adminuser_list.htm" scrolling="no" width=100% height=900 FrameBorder=0 ></iframe>' 
    
        });

        this.callParent(arguments);
    }
});