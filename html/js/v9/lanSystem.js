/**
 定义策略tab
 */
 Ext.Loader.setConfig({enabled: true,paths:{'systemmanage':'/js/system'}});
Ext.define('ncViewer.lanSystem',{
    extend: 'Ext.tab.Panel',
    initComponent : function(){
        Ext.apply(this,{
        layout : 'border',
        id:'lan_system',
        margins: '0 0 0 0',
 //       frames:true,
        forceFit: true,
        activeTab: 0,        
         defaults :{
 
            bodyPadding: 0
 
        },
        items: [
//          Ext.create('systemmanage.nc_sys_user'),
       {
         title: '系统管理',
         id:'adminuser',            
         html: '<iframe  id="lan_system_adminuser" name="lan_system_adminuser" src="/xtgl/ncs_adminuser_list.htm" scrolling="no" width=100% height=950  FrameBorder=0 ></iframe>' 
       },
        {
            title: '权限角色', 
            id:'rightrole',           
            html: '<iframe  id="lan_system_rightrole" name="lan_system_rightrole" src="/xtgl/ncs_role_list.htm" scrolling="no" width=100% height=950  FrameBorder=0 ></iframe>' 
        },{
            title: '系统操作日志',
            id:'operlog',            
            html: '<iframe  id="lan_system_operlog" name="lan_system_operlog" src="/v4/ncs_syslog_list.htm" scrolling="no" width=100% height=950  FrameBorder=0 ></iframe>' 
        }
        ]
          
        });
        this.callParent(arguments);
    }
})