/**
 定义策略tab
 */
 
 
Ext.define('ncViewer.mac_infoquery_current',{
    extend: 'Ext.panel.Panel',
	alias: 'widget.mac_infoquery_current',
    initComponent : function(){
		/* 根据面板中心区域的高度决定tab显示区域的高度 */
		//grid_height=Ext.getCmp('layout_center').getHeight()-56;
		var theight=grid_height+30;
        Ext.apply(this,{
        layout : 'border',
        id:'id_mac_infoquery_current',
        margins: '0 0 0 0',
		frame: false,
            
        html: '<iframe  id="lan_itm_infoquery_id_s" name="lan_itm_infoquery_id_s" src="/mac/infoquery/currentinfo/proauth_currentinfo.htm" scrolling="no" width=100% height='+ theight + 'FrameBorder=0 ></iframe>' 
        
        
          
        });
        this.callParent(arguments);
    }
})