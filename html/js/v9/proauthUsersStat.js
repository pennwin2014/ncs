/**
 定义策略tab
 */
     
Ext.define('ncViewer.proauthUsersStat',{
  extend: 'Ext.tab.Panel',
  initComponent : function(){
// alert(Ext.getCmp('layout_center').getHeight());
	
    Ext.apply(this,{
      layout : 'border',
      id:'lan_usersstat',
      margins: '0 0 0 0',
      forceFit: true,
      activeTab: 0,        
      defaults :{
        bodyPadding: 0
      },
      items: [{
        title: '上网人数统计',
        id:'users_s',           
        html: '<iframe id="lan_usersstat_users_s" name="lan_usersstat_users_s" src="/v4/stat/ncs_user_stat.html" scrolling="no" width=100% height='+900+' FrameBorder=0 ></iframe>' 
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