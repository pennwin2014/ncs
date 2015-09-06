/**
 定义策略tab
 */
 
Ext.define('ncViewer.portalJrcs',{
    extend: 'Ext.tab.Panel',

    initComponent : function(){

        Ext.apply(this,{
        layout : 'border',
        id:'lan_jrcs',
        margins: '0 0 0 0',
 //       frames:true,
        forceFit: true,
        activeTab: 0,        
         defaults :{
 
            bodyPadding: 0
 
        },
        items: [{
            title: '厂商信息维护', 
            id:'fac_s',           
            html: '<iframe  id="lan_jrcs_fac_s" name="lan_jrcs_fac_s" src="/v4/ncs_joinuser_list.htm" scrolling="no" width=100% height=900  FrameBorder=0 ></iframe>' 
           },
           {
            title: '场所运行状态', 
            id:'csyxzt',           
            html: '<iframe  id="lan_jrcs_csyxzt" name="lan_jrcs_csyxzt" src="/v4/ncs_csyxzt_list.htm" scrolling="no" width=100% height=900  FrameBorder=0 ></iframe>' 
           }
           ,
           {
            title: '场所维护日志', 
            id:'whlog',           
            html: '<iframe  id="lan_jrcs_whlog" name="lan_jrcs_whlog" src="/jrcs/proauth_whlog.html" scrolling="no" width=100% height=900  FrameBorder=0 ></iframe>' 
           }
            ,
           {
            title: '消息管理', 
            id:'mesglog',           
            html: '<iframe  id="lan_jrcs_mesglog" name="lan_jrcs_mesglog" src="/jrcs/mesg/proauth_mesg.html" scrolling="no" width=100% height=900  FrameBorder=0 ></iframe>' 
           }
        ]
          
        });
        this.callParent(arguments);
    }
})