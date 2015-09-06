/**
 定义策略tab
 */
 
Ext.define('ncViewer.proauthMac',{
  extend: 'Ext.tab.Panel',
  initComponent : function(){
    Ext.apply(this,{
      layout : 'border',
      id:'lan_mac',
      margins: '0 0 0 0',
      forceFit: true,
      activeTab: 0,        
      defaults :{
        bodyPadding: 0
      },
      items: [{
        title: 'Mac地址库',
        id:'mac_s',           
        html: '<iframe id="lan_mac_mac_s" name="lan_mac_mac_s" src="/mack/proauth_mac.html" scrolling="no" width=100% height=900 FrameBorder=0 ></iframe>' 
      }
           
      ]
   ,
  		listeners:{
  			'beforetabchange':function(tp, p){
  				var t_id=p.getId();  				
  				if(t_id=='sjcomputer'){
  					 bStartim=0;
  					 bStartcomp=1;
  				}
  				else if(t_id=='immon'){
  					   bStartim=1;
  					   bStartcomp=0;
  				}
  				else if(t_id=='sspm'){
  					   bStartim=2;
  					   bStartcomp=0;
  				}else if(t_id=='ssjc'){
  					   bStartim=0;
  					   bStartcomp=1;
  //					 eval("lay_ssjk_sjcomputer").bStart=0;
  //					 eval("lay_ssjk_immon").bStart=1;
  				}else{
  					   bStartim=0;
  					   bStartcomp=0;
  	//			 eval("lay_ssjk_sjcomputer").bStart=0;
  //	  			 eval("lay_ssjk_immon").bStart=0;
  				}
  			}
  		}
  	});
    this.callParent(arguments);
  }
});