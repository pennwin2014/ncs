
 //创建敏感规则菜单
   var menu_mggz = Ext.create('Ext.menu.Menu', {
        id: 'mggzMenu',
        style: {
            overflow: 'visible'     
        }
        ,
        items: [
          {
                text: '关键字库维护',
                iconCls: 'iadd',
                itemid:'keywordmenu', 
                tname:'keywordmenu', 
                srcurl:'/v4/case/ncs_case_keyword.htm',  
                handler:onItemClick    
       
           },
           {
           	    text: '重点关注用户库',
           	    iconCls: 'iadd',
                itemid:'zdyhmenu',
                tname:'zdyhmenu',
                srcurl:'/v4/sentive/ncs_sentive_user_list.htm',
                handler:onItemClick    
           },
           {
           	    text: '敏感网址库维护',
           	    iconCls: 'iadd',
                itemid:'mgurlmenu',
                tname:'mgurlmenu',
                srcurl:'/v4/sentive/ncs_sentive_url.htm',
                handler:onItemClick    
           },
           {
           	    text: '重点虚拟身份库',
           	    iconCls: 'iadd',
                itemid:'zdvidmenu',
                tname:'zdvidmenu',
                srcurl:'/v4/sentive/ncs_sentive_netid.htm',
                handler:onItemClick    
           },
           {
           	    text: 'CCIC库维护',
           	    iconCls: 'iadd',
                itemid:'ccicmenu',
                tname:'ccicmenu',
                srcurl:'/v4/sentive/ncs_sentive_ccic.htm',
                handler:onItemClick    
           },'-',
              {
           	    text: '关键字日志归档',
           	    iconCls: 'viewlist',
                itemid:'keywordgdmenu',
                tname:'keywordgdmenu',
                srcurl:'/v4/sentive/ncs_keywordlog_list.htm',
                handler:onItemClick    
           },
            {
           	    text: '敏感网址归档',
           	    iconCls: 'viewlist',
                itemid:'senwebgdmenu',
                tname:'senwebgdmenu',
                srcurl:'/v4/sentive/ncs_senurllog_list.htm',
                handler:onItemClick    
           },
            {
           	    text: '重点虚拟身份归档',
           	    iconCls: 'viewlist',
                itemid:'sennetidgdmenu',
                tname:'sennetidgdmenu',
                srcurl:'/v4/sentive/ncs_sennetidlog_list.htm',
                handler:onItemClick    
           },
            {
           	    text: '重点关注用户日志',
           	    iconCls: 'viewlist',
                itemid:'senuserlogmenu',
                tname:'senuserlogmenu',
                srcurl:'/v4/sentive/ncs_senuserlog_list.htm',
                handler:onItemClick    
           }
        ]

                
        
    });



 //创建系统菜单
   var menu_xtgl = Ext.create('Ext.menu.Menu', {
        id: 'xtglMenu',
        style: {
            overflow: 'visible'     
        }
        ,
        items: [
          {
                text: '权限角色',
                itemid:'rolemenu', 
                tname:'rolemenu', 
                srcurl:'/v4/ncs_role_list.htm',  
                handler:onItemClick    
       
           },
           {
           	    text: '系统用户',
                itemid:'xtyhmenu',
                tname:'xtyhmenu',
                srcurl:'/v4/ncs_adminuser_list.htm',
                handler:onItemClick    
           },
           {
           	    text: '移动密保管理',
                itemid:'ydmbmenu',
                tname:'ydmbmenu',
                srcurl:'/pronline/Msg?FunName@ncsWebReplace&plate@ds/ds_mbauth.htm&fname@ncWebDisplayMbManager',
                handler:onItemClick    
           },
           {
           	    text: '系统操作日志',
                itemid:'czrimenu',
                tname:'czrimenu',
                srcurl:'/v4/ncs_syslog_list.htm',
                handler:onItemClick    
           }
        ]

        
        
        
    });


 //创建身份库菜单
   var menu_sfkgl = Ext.create('Ext.menu.Menu', {
        id: 'sfkglMenu',
        style: {
            overflow: 'visible'     
        }
        ,
        items: [
          {
                text: '上网用户库',
                itemid:'swyhkmenu', 
                tname:'swyhkmenu', 
                srcurl:'/v4/ncs_enduser_list.htm',  
                handler:onItemClick    
       
           },
           {
           	    text: '虚拟身份库',
                itemid:'xnsfkmenu',
                tname:'xnsfkmenu',
                srcurl:'/v4/vid/ncs_vid_lab.htm',
                handler:onItemClick    
           }
        ]

        
        
        
    });





 //创建导航菜单
   var menu_dhtree = Ext.create('Ext.menu.Menu', {
        id: 'dhMenu_tree',
        style: {
            overflow: 'visible'     
        }
        ,
        items: [
          {
                text: '菜单树',
                id:'dhmenu', 
                tname:'dhtree', 
                srcurl:'/v4/main_dhleft.htm',
                checked:false,      
                checkHandler:onCheckItem    
       
           },
           {
           	    text: '单位组树',
                id:'dwzmenu',
                tname:'dwztree',
                srcurl:'/v4/ncs_tree_group.htm',
                checked:false, 
                checkHandler:onCheckItem    
           },
           {
           	    text: '单位树',
           	    checked:false,
           	    srcurl:'/v4/ncs_tree_user.htm',
                id:'dwmenu',
                tname:'dwtree',
                checkHandler:onCheckItem
           }
        ]

        
        
        
    });