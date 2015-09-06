
 Ext.define('ncViewer.mac_infoquery_move', {
    extend: 'Ext.panel.Panel',
    alias: 'widget.mac_infoquery_move',
    height:900,
     autoscroll:false,  
    initComponent: function(){
    	var theight=grid_height+30;
        Ext.apply(this,{   
        	 margins: '0 0 0 0',      
        	 frame: false,
           html: '<iframe id="lan_itm_infoquery_move_s" name="lan_itm_infoquery_move_s" src="/mac/infoquery/mobileap/proauth_mobileap.htm" scrolling="no" width=100% height=' + theight + 'FrameBorder=0 ></iframe>' 
    
        });

        this.callParent(arguments);
    }
});