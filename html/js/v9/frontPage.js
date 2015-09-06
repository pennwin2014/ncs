/**
 定义策略tab
 */
 
Ext.define('ncViewer.frontPage',{
    extend: 'Ext.panel.Panel',
    alias: 'widget.frontPage',
    height:900,
	width:parent.grid_width,
    autoscroll:false, 
    initComponent : function(){

        Ext.apply(this,{
        layout : 'border',
        id:'mac_frontpage',
        margins: '0 0 0 0',
		frame:false,
 //       frames:true,
   //     forceFit: true,
    //    activeTab: 0,        
     //    defaults :{
 
       //     bodyPadding: 0
 
//        },
  //      title: '首页',           
        html: '<iframe  id="mac_frm_frontpage" name="mac_frm_frontpage" src="/mac/report_1.html" scrolling="no" width=100% height=900  FrameBorder=0 ></iframe>' 
        
                  
        });
        this.callParent(arguments);
    }
});

