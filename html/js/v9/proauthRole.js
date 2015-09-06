
 Ext.define('ncViewer.proauthRole', {
    extend: 'Ext.panel.Panel',
    alias: 'widget.proauthRole',
    height:900,
     autoscroll:false,  
    initComponent: function(){
        Ext.apply(this,{   
        	 margins: '0 0 0 0',      
        	 frame: false,
           html: '<iframe id="lan_role_s" name="lan_role_s" src="/xtgl/ncs_role_list.htm" scrolling="no" width=100% height=900 FrameBorder=0 ></iframe>' 
    
        });

        this.callParent(arguments);
    }
});