/**
 定义策略tab
 */
     
Ext.define('ncViewer.proauthYxfx',{
  extend: 'Ext.tab.Panel',
  initComponent : function(){
// alert(Ext.getCmp('layout_center').getHeight());
	
    Ext.apply(this,{
      layout : 'border',
      id:'lan_yxfx',
      margins: '0 0 0 0',
      forceFit: true,
      activeTab: 0,        
      defaults :{
        bodyPadding: 0
      },
      items: [{
        title: '当前在线率',
        id:'sszxl_s',           
        html: '<iframe id="lan_yxfx_sszxl_s" name="lan_yxfx_sszxl_s" src="/v4/ncs_onlinestat_list.htm" scrolling="no" width=100% height='+900+' FrameBorder=0 ></iframe>' 
       },
       {
        title: '单位类别在线率',
        id:'lbzxl_s',           
        html: '<iframe id="lan_yxfx_lbzxl_s" name="lan_yxfx_lbzxl_s" src="/v4/ncs_lbonlinestat_list.htm" scrolling="no" width=100% height='+900+' FrameBorder=0 ></iframe>' 
       },
       {
        title: '在线单位',
        id:'zxdw_s',           
        html: '<iframe id="lan_yxfx_zxdw_s" name="lan_yxfx_zxdw_s" src="/v4/ncs_onlinecorp_list.htm" scrolling="no" width=100% height='+900+' FrameBorder=0 ></iframe>' 
       },
       {
        title: '离线单位',
        id:'lxdw_s',           
        html: '<iframe id="lan_yxfx_lxdw_s" name="lan_yxfx_lxdw_s" src="/v4/ncs_user_list_link.htm" scrolling="no" width=100% height='+900+' FrameBorder=0 ></iframe>' 
       },
       {
        title: '维护单位',
        id:'whdw_s',           
        html: '<iframe id="lan_yxfx_whdw_s" name="lan_yxfx_whdw_s" src="/v4/ncs_user_list_link.htm" scrolling="no" width=100% height='+900+' FrameBorder=0 ></iframe>' 
       },
       {
        title: '停业单位',
        id:'tydw_s',           
        html: '<iframe id="lan_yxfx_tydw_s" name="lan_yxfx_tydw_s" src="/v4/ncs_user_list_link.htm" scrolling="no" width=100% height='+900+' FrameBorder=0 ></iframe>' 
       }            
      ]
   ,
  		listeners:{
  			'beforetabchange':function(tp, p){
  				               if(p.title=='离线单位'){
                    	 	  document.getElementById("form1").show_flag.value='2';
                    	  }
                        if(p.title=='维护单位'){
                    	 	  document.getElementById("form1").show_flag.value='5';
                    	  }
                    	  if(p.title=='停业单位'){
                    	 	  document.getElementById("form1").show_flag.value='4';
                    	  }	
                    	  	
  				
  			}
  		}
  	});
    this.callParent(arguments);
  }
});