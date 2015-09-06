/*
This file is part of Ext JS 4
Copyright (c) 2011 Sencha Inc
Contact:  http://www.sencha.com/contact
GNU General Public License Usage
This file may be used under the terms of the GNU General Public License version 3.0 as published by the Free Software Foundation and appearing in the file LICENSE included in the packaging of this file.  Please review the following information to ensure the GNU General Public License version 3.0 requirements will be met: http://www.gnu.org/copyleft/gpl.html.

If you are unsure which license is appropriate for your use, please contact the sales department at http://www.sencha.com/contact.

*/
Ext.require(['*']);

Ext.onReady(function(){
    Ext.QuickTips.init();
   
    var menu = Ext.create('Ext.menu.Menu', {
        id: 'mainMenu',
        style: {
            overflow: 'visible'     
        }
        ,
        items: [
          {
                text: '联网单位管理',        
                handler: onItemClick,
       
           },
           {
           	    text: '单位分组管理',
         
                 handler: onItemClick,
           }
        ]
    });

    var menu1 = Ext.create('Ext.menu.Menu', {
        id: 'mainMenu',
        style: {
            overflow: 'visible'     
        },
        items: [
          {
                text: '联网单位管理',        
                handler: onItemClick,
       
           },
           {
           	    text: '单位分组管理',
         
                 handler: onItemClick,
           }
        ]
    });

    var tb = Ext.create('Ext.toolbar.Toolbar');
    tb.suspendLayout = true;
    tb.render('toolbar');

    tb.add(
        {
            text: '返回首页',
            iconCls: 'book',
            handler:onItemClick
        },'-',
        {
            text:'单位管理',
            iconCls: 'add16',
            handler:onItemClick
        }, '-',
        {
            text: '快速查询',
            iconCls: 'exp',
            handler:onItemClick
        },'-',{
            text: '归档日志',
            iconCls: 'viewlist',
            handler:onItemClick
        
      },'-',{
            text: '在线单位',
            iconCls: 'useradd',
            handler:onItemClick
        
      },'-',{
            text: '在线率统计',
            iconCls: 'usc',
            handler:onItemClick
        
      },'-',{
            text: '告警信息',
            iconCls: 'info',
            handler:onItemClick
        
      },'-', 
         {text:'布控管理',
          iconCls: 'iadd',
          handler:onItemClick
        }, '-',      
         {text:'审计管理',
          iconCls: 'fadd',
          handler:onItemClick
        },  '-',     
         {text:'系统管理',
            menu: menu  // assign menu by instance
        },'-'
        ,'->',{
                text: '系统刷新',
                iconCls: 'refresh',
                handler: Ext.Function.pass(handleAction, 'Right')
            },'-',{
                text: '退出',
                iconCls: 'close',
                handler: Ext.Function.pass(handleAction, 'Right')
            }
        );

    menu.add(' ');

  

    // They can also be referenced by id in or components
    tb.add('-', {
        icon: 'list-items.gif', // icons can also be specified inline
        cls: 'x-btn-icon',
        tooltip: '<b>Quick Tips</b><br/>Icon only button with tooltip',
        handler: function(){
            Ext.example.msg('Button Click','You clicked the "icon only" button.');
        }
    });

   
 

   
    tb.suspendLayout = false;
    tb.doLayout();

    // functions to display feedback
    function onButtonClick(btn){
 //       Ext.example.msg('Button Click','You clicked the "{0}" button.', btn.text);
    }

    function onItemClick(item){

    if(item.text=='单位管理'){   		
    	document.getElementById("viewframe").contentWindow.document.getElementById("moduleIframe").src='/v4/ncs_user_list.htm';  		
    }
    else if(item.text=='联网单位管理2'){

    }
    else if(item.text=='单位组管理'){
    	 document.getElementById("viewframe").contentWindow.document.getElementById("moduleIframe").src='/v4/ncs_group_list.htm';
    }
    else if(item.text=='归档日志'){
    	 document.getElementById("viewframe").contentWindow.document.getElementById("moduleIframe").src='/v4/ncs_gdrz_lab.htm';
    }
    else if(item.text=='返回首页'){
    	 document.getElementById("viewframe").contentWindow.document.getElementById("moduleIframe").src='/v4/ncs_first_right.htm';
    }
    else if(item.text=='在线单位'){
    	 document.getElementById("viewframe").contentWindow.document.getElementById("moduleIframe").src='/main_Online_Mon.htm';
    }
    else if(item.text=='布控管理'){
    	 document.getElementById("viewframe").contentWindow.document.getElementById("moduleIframe").src='/v4/ncs_case_lab.htm';
    }
    else if(item.text=='在线率统计'){
    	 document.getElementById("viewframe").contentWindow.document.getElementById("moduleIframe").src='/v4/ncs_onlinestat_list.htm';
    }
 //       Ext.example.msg('Menu Click', 'You clicked the "{0}" menu item.', item.text);
    }





   var handleAction = function(action){
//   	alert(action);
        Ext.example.msg('<b>Action</b>', 'You clicked "' + action + '"');
    };

  

   

});

