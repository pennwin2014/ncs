
 Ext.define('ncViewer.proauthDwz', {
    extend: 'Ext.panel.Panel',
    alias: 'widget.proauthDwz',
    height:900,
     autoscroll:false,  
    initComponent: function(){
        Ext.apply(this,{   
        	 margins: '0 0 0 0',      
        	 frame: false,
           html: '<iframe id="lan_dwz_s" name="lan_dwz_s" src="/v4/ncs_group_list.htm" scrolling="no" width=100% height=900 FrameBorder=0 ></iframe>' 
    
        });

        this.callParent(arguments);
    }
});