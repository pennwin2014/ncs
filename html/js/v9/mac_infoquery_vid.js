
 Ext.define('ncViewer.mac_infoquery_vid', {
    extend: 'Ext.panel.Panel',
    alias: 'widget.mac_infoquery_vid',
    //height:900,
    //autoscroll:false,  
	
    initComponent: function(){
		grid_height=Ext.getCmp('layout_center').getHeight()-56;
		
        Ext.apply(this,{  
			layout : 'border',
			id:'id_mac_infoquery_vid',			
			margins: '0 0 0 0',      
        	frame: false,
			
            html: '<iframe id="lan_itm_infoquery_vid_s" name="lan_itm_infoquery_vid_s" src="/mac/infoquery/netidlog/netidlog.htm" scrolling="no" width=100% height=900 FrameBorder=0 ></iframe>'  
						 	
        });

        this.callParent(arguments);
    }
});
