/**
 定义策略tab
 */
     
Ext.define('ncViewer.proauthVidstat',{
  extend: 'Ext.tab.Panel',
  initComponent : function(){
// alert(Ext.getCmp('layout_center').getHeight());
	
    Ext.apply(this,{
      layout : 'border',
      id:'lan_vidstat',
      margins: '0 0 0 0',
      forceFit: true,
      activeTab: 0,        
      defaults :{
        bodyPadding: 0
      },
      items: [{
        title: '虚拟身份统计',
        id:'xnsf_s',           
        html: '<iframe id="lan_vidstat_xnsf_s" name="lan_vidstat_xnsf_s" src="/pronline/Msg?FunName@ncsVid_stat_v4" scrolling="no" width=100% height='+900+' FrameBorder=0 ></iframe>' 
       }           
      ]
   ,
  		listeners:{
  			'beforetabchange':function(tp, p){
  				 
  				
  			}
  		}
  	});
    this.callParent(arguments);
  }
});