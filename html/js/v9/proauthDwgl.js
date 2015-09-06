/**
 定义策略tab
 */
     
Ext.define('ncViewer.proauthDwgl',{
  extend: 'Ext.tab.Panel',
  initComponent : function(){
// alert(Ext.getCmp('layout_center').getHeight());
	
    Ext.apply(this,{
      layout : 'border',
      id:'lan_dwgl',
      margins: '0 0 0 0',
      forceFit: true,
      activeTab: 0,        
      defaults :{
        bodyPadding: 0
      },
      items: [{
        title: '单位管理',
        id:'dwgl_s',           
        html: '<iframe id="lan_dwgl_dwgl_s" name="lan_dwgl_dwgl_s" src="/v4/ncs_user_list.htm" scrolling="no" width=100% height='+900+' FrameBorder=0 ></iframe>' 
       },
       {
        title: '场所AP管理',
        id:'ap_s',           
        html: '<iframe id="lan_dwgl_ap_s" name="lan_dwgl_ap_s" src="/dwgl/apset/proauth_apset.htm" scrolling="no" width=100% height='+900+' FrameBorder=0 ></iframe>' 
       },
  /*
       {
        title: '安全厂商代码维护',
        id:'fac_s',           
        html: '<iframe id="lan_dwgl_fac_s" name="lan_dwgl_fac_s" src="/v4/ncs_joinuser_list.htm" scrolling="no" width=100% height='+900+' FrameBorder=0 ></iframe>' 
       },
  */
       {
        title: '单位组管理',
        id:'dwzgl_s',           
        html: '<iframe id="lan_dwgl_dwzgl_s" name="lan_dwgl_dwzgl_s" src="/v4/ncs_group_list.htm" scrolling="no" width=100% height='+900+' FrameBorder=0 ></iframe>' 
       },
       {
        title: '单位类别维护',
        id:'cslb_s',           
        html: '<iframe id="lan_dwgl_cslb_s" name="lan_dwgl_cslb_s" src="/v4/ncs_cslb_list.htm" scrolling="no" width=100% height='+900+' FrameBorder=0 ></iframe>' 
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