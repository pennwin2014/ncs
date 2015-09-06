/**
 定义策略tab
 */
 
Ext.define('ncViewer.mac_infoquery_place',{
    extend: 'Ext.panel.Panel',
	alias: 'widget.mac_infoquery_place',
//height: 900,
    initComponent : function(){
		/* 根据面板中心区域的高度决定tab显示区域的高度 */
		//grid_height=Ext.getCmp('layout_center').getHeight()-56;
		var theight=grid_height+30;
        Ext.apply(this,{
        //layout : 'border',
        //id:'id_mac_infoquery_place',
        margins: '0 0 0 0',
		frames: false,
		
        //forceFit: true,
        //activeTab: 0,        
        // defaults :{
 
        //    bodyPadding: 0
 
		html: '<iframe id="lan_itm_infoquery_place_s" name="lan_itm_infoquery_place_s" src="/mac/infoquery/place/proauth_place.htm" scrolling="no" width=100% height=' + theight + 'FrameBorder=0 ></iframe>'         		             
		     				
        });
        this.callParent(arguments);
    }
});