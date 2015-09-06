
 Ext.define('ncViewer.proauthCsgl', {
    extend: 'Ext.panel.Panel',
    alias: 'widget.proauthCsgl',
    height:900,
     autoscroll:false,  
    initComponent: function(){

        Ext.apply(this,{   
        	 margins: '0 0 0 0',      
        	 frame: false,
           html: '<iframe id="lan_csgl_s" name="lan_csgl_s" src="/v4/ncs_user_list.htm" scrolling="no" width=100% height=900 FrameBorder=0 ></iframe>' 
    
        });

        this.callParent(arguments);
    }
});