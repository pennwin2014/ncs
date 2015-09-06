
 Ext.define('ncViewer.mac_infoquery_mac', {
    extend: 'Ext.panel.Panel',
    alias: 'widget.mac_infoquery_mac',
    //height:900,
    //autoscroll:false,  
	
    initComponent: function(){
    var theight=grid_height+30;
    //alert(theight);
		/* 根据面板中心区域的高度决定tab显示区域的高度 */
		//grid_height=Ext.getCmp('layout_center').getHeight()-56;
		
    	Ext.apply(this,{  
				layout : 'border',
				id:'id_mac_infoquery_mac',			
				margins: '0 0 0 0',      
        Frame: false,
				//html: '<iframe id="lan_itm_infoquery_mac_s" name="lan_itm_infoquery_mac_s" src="/mac/infoquery/macterm/proauth_macterm.htm" scrolling="no" width=100% height=100% FrameBorder=0 ></iframe>'   
        html: '<iframe id="lan_itm_infoquery_mac_s" name="lan_itm_infoquery_mac_s" src="/mac/infoquery/macterm/proauth_macterm.htm" scrolling="no" width=100% height='+ theight +' FrameBorder=0 ></iframe>'  
						 	
        });

        this.callParent(arguments);
    }
});