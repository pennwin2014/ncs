/**
 定义策略tab
 */
 
Ext.define('ncViewer.proauthSsxx',{
  extend: 'Ext.tab.Panel',
  initComponent : function(){
  	grid_height=Ext.getCmp('layout_center').getHeight()-56;
    Ext.apply(this,{
      layout : 'border',
      id:'lan_ssxx',
      margins: '0 0 0 0',
      forceFit: true,
      activeTab: 0,        
      defaults :{
        bodyPadding: 0
      },
      items: [{
        title: '在线用户',
        id:'onlineuser',           
        html: '<iframe id="lan_ssxx_onlineuser" name="lan_ssxx_onlineuser" src="/ssxx/proauth_onlineuser.htm" scrolling="no" width=100% height=900 FrameBorder=0 ></iframe>' 
      }
     
      ]
   ,
  		listeners:{
  			'beforetabchange':function(tp, p){
  				
  				var t_id=p.getId();  

			          if(t_id=='onlinemac'){
			           	bStart_onlinemac=1;
			           	bStart_onlineap=0;
			           	bStart_onlinepeople=0;
			          }
			          else if(t_id=='onlineap'){
			           	bStart_onlinemac=0;
			           	bStart_onlineap=1;
			           	bStart_onlinepeople=0;
			          }
			          else if(t_id=='onlinpeople'){
			           	bStart_onlinemac=0;
			           	bStart_onlineap=0;
			           	bStart_onlinepeople=1;
		
			          }
			          else {
			          	bStart_onlinemac=0;
			           	bStart_onlineap=0;
			           	bStart_onlinepeople=0;
			          }
  			}
  		}
  	});
    this.callParent(arguments);
  }
});