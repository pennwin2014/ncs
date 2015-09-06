/**
 定义策略tab
 */
 
Ext.define('ncViewer.proauthRz',{
  extend: 'Ext.tab.Panel',
  initComponent : function(){
    Ext.apply(this,{
      layout : 'border',
      id:'lan_rzck',
      margins: '0 0 0 0',
      forceFit: true,
      activeTab: 0,        
      defaults :{
        bodyPadding: 0
      },
      items: [{
        title: '上下线日志',
        id:'authlog_s',           
        html: '<iframe id="lan_rzck_authlog_s" name="lan_rzck_authlog_s" src="/rzck/proauth_rzck_authlog.html" scrolling="no" width=100% height=900 FrameBorder=0 ></iframe>' 
      },{
        title: '上网日志',
        id:'smslog_s', 
        html: '<iframe id="lan_rzck_smslog_s" name="lan_rzck_smslog_s" src="/rzck/iplog/proauth_rzck_iplog.html" scrolling="no" width=100% height=900 FrameBorder=0></iframe>' 
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
