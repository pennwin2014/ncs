
var bInfoquery = true;
var bAlarm = false;
var bDm = false;
var bOperation = false;
var bSystemset = false;
var globalSelectId = "";
var globalSelectGroupId = "itm_infoquery";

var mapInfoquery = {'id_infoquery_mac':'MAC库', 'id_infoquery_hotspot':'热点查询', 'id_infoquery_place':'场所采集设备', 'id_infoquery_move':'移动采集设备', 'id_infoquery_id':'实时信息'};
var mapAlarm = {'id_alarm_controlmanage':'布控管理', 'id_alarm_peoplecalled':'接警人员管理'};
var mapDm = {'id_dm_bab':'伴随行为分析', 'id_dm_sample':'抽样分析', 'id_dm_freCharact':'高频次分析', 'id_dm_track':'轨迹分析', 'id_dm_sus':'可疑热点分析', 'id_dm_crowd':'人流分析'};
var mapOperation = {'id_operation_equipment':'设备管理', 'id_operation_normally':'合规热点信息管理', 'id_operation_place':'场所管理', 'id_operation_safety':'安全厂商管理', 'id_operation_terminal':'终端身份信息维护'};
var mapSystemset = {'id_systemset_admin':'管理员账号', 'id_systemset_alarm':'系统运维告警', 'id_systemset_preferences':'系统参数配置', 'id_systemset_log':'系统运维日志'};
var listAllMap = [mapInfoquery, mapAlarm, mapDm, mapOperation, mapSystemset];



function setItemStatById(id, isNormal){
	var hasFound = false;
	for(i in listAllMap){
		for(key in listAllMap[i]){
			if(key == id){
				hasFound = true;
				if(isNormal){
					Ext.getCmp(key).setText('<font style="font-size : 12px !important;color:#828282;margin-left: 24px !important;">☆&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;'+listAllMap[i][key]+'</font>');
				}else{
					Ext.getCmp(key).setText('<font style="font-size : 12px !important;color:#0D3967;margin-left: 24px !important;">★&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;'+listAllMap[i][key]+'</font>');
				}
				break;
			}
		}
		if(hasFound == true)
			break;
	}
}

function toggleClickItem(item){
	//把旧的改为普通样式
	if(globalSelectId != ""){
		setItemStatById(globalSelectId, true);
	}
	globalSelectId = item.id;
	setItemStatById(globalSelectId, false);
}

function setGroupHeader(isExpand, groupId){
	var groupHeader = Ext.getCmp(groupId);
	if(isExpand){
		if(groupHeader.hasCls("cls_menu_first_normal")){
			groupHeader.removeCls("cls_menu_first_normal");
			groupHeader.addCls("cls_menu_first_focus");
		}
	}else{
		if(groupHeader.hasCls("cls_menu_first_focus")){
			groupHeader.removeCls("cls_menu_first_focus");
			groupHeader.addCls("cls_menu_first_normal");			
		}
	}
}

function toggleInfoquery(bVisible){
	bInfoquery = bVisible;
	setGroupHeader(bInfoquery, "id_infoquery");
	var itm;
	for (i in mapInfoquery){
		itm=Ext.getCmp(i);
		itm.setVisible(bVisible);
	}
}

function toggleAlarm(bVisible){
	bAlarm = bVisible;
	setGroupHeader(bAlarm, "id_alarm");
	var itm;
	for (i in mapAlarm){
		itm=Ext.getCmp(i);
		itm.setVisible(bVisible);
	}
}
	
function toggleDm(bVisible){
	bDm = bVisible;
	setGroupHeader(bDm, "id_dm");
	var itm;
	for (i in mapDm){
		itm=Ext.getCmp(i);
		itm.setVisible(bVisible);
	}
}

function toggleOperation(bVisible){
	bOperation = bVisible;
	setGroupHeader(bOperation, "id_operation");
	var itm; 
	for (i in mapOperation){
		itm=Ext.getCmp(i);
		itm.setVisible(bVisible);
	}
}

function toggleSystemset(bVisible){
	bSystemset = bVisible;
	setGroupHeader(bSystemset, "id_systemset");
	var itm;
	for (key in mapSystemset){
		itm=Ext.getCmp(key);
		itm.setVisible(bVisible);
	}
}
	



Ext.Loader.setConfig({enabled: true,paths:{'ncViewer':'/js/v9'}});
Ext.define('ncViewer.App', {
    extend: 'Ext.container.Viewport',    
    uses:['ncViewer.frontPage','ncViewer.lanGroupTree','ncViewer.mac_infoquery_mac','ncViewer.mac_infoquery_move','ncViewer.mac_infoquery_place','ncViewer.mac_infoquery_current','ncViewer.mac_infoquery_hotspot','ncViewer.mac_monitoralarm_controlmanage','ncViewer.mac_monitoralarm_peoplecalled','ncViewer.mac_dm_bab','ncViewer.mac_dm_sample','ncViewer.mac_dm_freCharact','ncViewer.mac_dm_track','ncViewer.mac_dm_sus','ncViewer.mac_dm_crowd','ncViewer.mac_operation_place','ncViewer.mac_operation_equipment','ncViewer.mac_operation_safety','ncViewer.mac_operation_normally','ncViewer.mac_operation_terminal','ncViewer.mac_systemset_admin','ncViewer.mac_systemset_alarm','ncViewer.mac_systemset_preferences','ncViewer.mac_systemset_log'],
     initComponent: function(){
        Ext.define('State', {
            extend: 'Ext.data.Model',
            fields: ['groupid', 'groupname']
        });       
        Ext.apply(this, {
            layout: {
                type: 'border',
                padding: 2
            },
            items: [this.createNcTb(),this.createNcLeft(),this.createNcCenter(),this.createNcStatus()]
        });
        this.callParent(arguments);
    },
  
    /**
    创建工具条
     */
    createNcTb: function(){
			
        var right_fun_s=right_fun;

	 //创建统计报表菜单
	    var menu_tj = Ext.create('Ext.menu.Menu', {
			id: 'baseMenu',
			style: {
				overflow: 'visible'  
			},
			items: [
			  {
				  text: '虚拟身份统计',
				  itemid:'vidstat',
				  srcurl:'ncViewer.proauthVidstat',  
				  handler:this.onItemClick       
			   },
			   {
				  text: '上网人数统计',
				  itemid:'usersstat',
				  srcurl:'ncViewer.proauthUsersStat',  
				  handler:this.onItemClick
		   
			   }
			]
		});

		menu_tj.items.each(function(item){
			if(right_fun_s.indexOf(item.text)== -1){
				item.destroy();
			}
		});
		

		
    	var tb = Ext.create('Ext.toolbar.Toolbar',{ style: 'background-image:url(/images/large/toolbarbackground.png);',id: "maintab"});
    	    tb.add({
	    	    itemid:'logo',
				xtype:'label',
        		html:'&nbsp;<img src="/images/mac/banner_logo.png" style="margin:0 30px 0 30px;" width="125" height="40">'
         	},'-',{
				text: '首页',
				itemid:'itm_frontPage',
				scale:'large',
				iconAlign: 'top',
				srcurl:'ncViewer.frontPage',
				iconCls: 'dvd',
				handler:this.onItemClick        
			}
			/*,'-',{
				itemid:'rzck',
				hidden:false,
				text:'日志查看',
				scale:'large',
				iconCls: 'search',
				iconAlign: 'top',
	//            menu:menu_procy
				srcurl:'ncViewer.proauthRz',
				handler:this.onItemClick
			},'-',{
				itemid:'aqcs',
				hidden:false,
				text:'安全厂商管理',
				scale:'large',
				iconCls: 'address',
				iconAlign: 'top',
				srcurl:'ncViewer.proauthSfac',
				handler:this.onItemClick
			},'-',{
				itemid:'itm_infoquery_hotspot',
				hidden:false,
				text:'热点信息',
				scale:'large',
				iconCls: 'lamp',
				iconAlign: 'top',
				srcurl:'ncViewer.mac_infoquery_hotspot',
				handler:this.onItemClick
			},'-',{
				itemid:'itm_infoquery_mac',
				hidden:false,
				text:'终端特征信息',
				scale:'large',
				iconCls: 'sitemap',
				iconAlign: 'top',
				srcurl:'ncViewer.mac_infoquery_mac',
				handler:this.onItemClick
			},'-',{
				itemid:'itm_infoquery_move',
				hidden:false,
				text:'移动AP轨迹',
				scale:'large',
				iconCls: 'dvd',
				iconAlign: 'top',
				srcurl:'ncViewer.mac_infoquery_move',
				handler:this.onItemClick
			},'-',{
				itemid:'tjbb',
				hidden:false,
				text:'统计报表',
				scale:'large',
				iconCls: 'static',
				menu:menu_tj,
				iconAlign: 'top'
			}, '-',{
				itemid:'glyzh',
				hidden:false,
				text:'管理员帐号',
				scale:'large',
				iconCls: 'address',
				iconAlign: 'top',
				srcurl:'ncViewer.proauthSysuser',
				handler:this.onItemClick
			}, '-',{
				itemid:'sysset',
				hidden:false,
				text:'系统设置',
				scale:'large',
				iconCls: 'pencil',
				iconAlign: 'top',
				srcurl:'ncViewer.portalSysset',
				handler:this.onItemClick
			} */, '-','-','->','->',
			{
				xtype:'combo',
				id:'id_global_search',
				width:200,
				//cls:"search_box", 
				name:'name_global_search',
				frame:false,
				text:'MAC/手机号/场所名称',
				enableKeyEvents:true,
				listeners:{
					'focus':function(i){
							if(Ext.getCmp("id_global_search").getText() == "MAC/手机号/场所名称"){
								Ext.getCmp("id_global_search").setText("");
							}							
                    },
                    'keydown' : function(i,e){
                        var aae=e.getKey() ; 
                        if(aae==13){
                        }
                    }
				}
			},{
	    	    itemid:'eee',
				xtype:'label',
        		html:'&nbsp;<img src="/images/large/search.png" style="margin-left:-205px;" width="15" height="15">'
         	},{
				text: '系统刷新',
				itemid:'xtsx',
				scale:'large',
				iconAlign: 'top',
				iconCls: 'refresh',
				handler: this.onItemReflash
			},'-',{
				text: '重新登录',
				scale:'large',
				itemid:'cxdl',
				iconAlign: 'top',
				iconCls: 'unlock',
				handler: this.onItemClose
			},{
				text: '退出系统',
				scale:'large',
				itemid:'tcxt',
				iconAlign: 'top',
				iconCls: 'macs',
				handler: this.onItemClose2
			}        
        ); 
        
        //console.info(right_fun_s);
        var isep=0;


/*  权限暂时注释掉
         tb.items.each(function(item){
 //     console.info(right_fun_s);
 //     console.info(item.text);
      	if((right_fun_s.indexOf(item.text)!=-1)||(item.text=='系统刷新')||(item.text=='重新登录')||item.itemid=='logo'){
      	
      	    item.hidden=false;
      	    isep=1;
      	 
      	}
      	else{
      		if(item.text){

      			item.hidden=true;
      			isep=0;
      		
      		}
      		else{
      			
      			if(isep==0){
      			 item.hidden=true;
      			 isep=1;
      			}
      			else{
      				item.hidden=false;
      				isep=0;
      			}
      		}
      	
      	}

      	 
      	});  
 */
		this.toolPanel = Ext.create(Ext.panel.Panel, {
             id: 'header',
             margins: '0 2 0 2',
             border: true,
             region: 'north',
             items:[tb]
          
        });
        return this.toolPanel;
       
    },
    onItemClose:function(){
    	self.location='/login.htm';
    },
    onItemClose2:function(){
    	self.location='/pronline/Msg?FunName@proauthExit';
    },
    onItemReflash:function(){
          var fp=Ext.create('Ext.FormPanel');
               fp.form.doAction('submit',{
               url:'/pronline/Msg',
                method:'POST',
                waitMsg:'正在重新加载配置, 请稍候...', 
				        waitTitle:'提示', 
               params:{FunName:'ncsSysReflesh_v4'},
                 
                success:function(form,action){ 
           					Ext.Msg.alert('操作',"加载完成!");
                 },
             		 failure:function(form,action){        		 
                    Ext.Msg.alert('操作',"系统正忙,稍候重试!");
                   
            		 }
                }
             )
					  
	//				    loadFn('win-button', 'win-statusbar');
    
    },
    createNcLeft:function(){   	
    	var left_tabs=Ext.createWidget('tabpanel', {
			width: 290,
			border:false,
			margins: '0 0 0 0',
			id:'treetab',    
			activeTab: 0,
			defaults :{
				bodyPadding: 0
			},
			listeners: {              
				tabchange:function(tp,p){
					servicecode='';
					corpdid='';
					ncsgroupid='';
					dxtitle='';
					fcode='';
				}
			}
		});
   
		left_tabs.add({
						closable: false,
						border:false,
						items:[Ext.create(ncViewer.lanGroupTree)],
						title: '接入场所'
		});
	//     
		var sy_obj;
		var sy_title,sy_id;
		if(right_sy=='日志查看'){
			sy_title='日志查看';
		}
		else if(right_sy=='单位管理'){
			sy_title='单位管理';
		}
		else if(right_sy=='运行分析'){
			 sy_title='运行分析';
		}
		else if(right_sy=='接入厂商管理'){
			 sy_title='接入厂商管理';
		}
		else if(right_sy=='统计报表'){
			 sy_title='统计报表';
		}
		else if(right_sy=='系统管理'){
			 sy_title='系统管理';
		}
		else if(right_sy=='系统设置'){
			 sy_title='系统设置';
		}
		else{
			sy_title='单位管理';
		}
		var formTmp = Ext.create('Ext.form.Panel');
		  
		this.leftPanel=Ext.create(Ext.panel.Panel, {
				id: 'left',
				margins: '0 2 0 2',          
			   // layout: 'border',
				title:'当前位置：'+sy_title,
				region: 'west',
				collapsible: true,
				autoScroll: true,
				animCollapse: true,
				layout: "column",  //设置为手风琴布局
				layoutConfig: {
					animate: true
				},  
				defaults:{                     
					layout: 'anchor', 
					defaults: {   anchor: '100%'  } 
				},
				//bodyStyle: 'background:#F0F3F8;',
				split:true,
				width: 200,
				minWidth:200,
				maxWidth:500,
				minSize: 100,
				maxSize: 500,   
				items: [
						{  
							columnWidth: 10 / 10,        //设置列的宽度
							items:
							[
								{
									xtype : 'button',
									iconCls: "mi_infoquery", 
									bodyStyle: 'background:#F0F3F8;',
									cls:"cls_menu_first_focus", 
									style: 'border-radius:0;background:#D5DDF0; margin:1px 0 0px 0px;',
									text : '<font style="font-size : 14px !important;color:#0D3967;margin:10px 0 0 60px !important;">信息查询</font>',
									id : 'id_infoquery',
									itemid : 'itm_infoquery',
									listeners:{
										click:this.onToggleFirst
									}
								},{
									xtype : 'button',
									hidden  : false,
									//overCls : 'clsListItemOver',
									cls:"cls_menu_second_normal", 
									text : '<font style="font-size : 12px !important;color:#828282;margin-left: 24px !important;">☆&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;MAC库</font>',
									title : 'MAC库',
									itemid:'itm_infoquery_mac',
									id:'id_infoquery_mac',
									srcurl:'ncViewer.mac_infoquery_mac',
									listeners:{
										mouseover:this.changeOnOver,
										mouseout:this.changeOnOut,
										click:this.onItemClick
									}
								},{
									xtype : 'button',
									hidden  : false,
									//overCls : 'clsListItemOver',·111·
									cls:"cls_menu_second_normal", 
									text : '<font style="font-size : 12px !important;color:#828282;margin-left: 24px !important;">☆&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;热点查询</font>',
									title : '热点查询',
									itemid:'itm_infoquery_hotspot',
									id: 'id_infoquery_hotspot',
									srcurl:'ncViewer.mac_infoquery_hotspot',  
									listeners:{
										mouseover:this.changeOnOver,
										mouseout:this.changeOnOut,
										click:this.onItemClick
									}
								},{
									xtype : 'button',
									hidden  : false,
									cls:"cls_menu_second_normal", 
									//overCls : 'clsListItemOver',
									style: 'text-align: left !important;',
									text : '<font style="font-size : 12px !important;color:#828282;margin-left: 24px !important;">☆&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;场所采集设备</font>',
									title : '场所采集设备',
									id:'id_infoquery_place',
									itemid:'itm_infoquery_place',
									srcurl:'ncViewer.mac_infoquery_place',
									listeners:{
										mouseover:this.changeOnOver,
										mouseout:this.changeOnOut,
										click:this.onItemClick
									}
								},{
									xtype : 'button',
									hidden  : false,
									cls:"cls_menu_second_normal", 
									//overCls : 'clsListItemOver',
									text : '<font style="font-size : 12px !important;color:#828282;margin-left: 24px !important;">☆&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;移动采集设备</font>',
									title:'移动采集设备',
									itemid:'itm_infoquery_move',
									id:'id_infoquery_move',
									srcurl:'ncViewer.mac_infoquery_move',  
									listeners:{
										mouseover:this.changeOnOver,
										mouseout:this.changeOnOut,
										click:this.onItemClick
									}
								},{
									xtype : 'button',
									hidden  : false,
									cls:"cls_menu_second_normal", 
									//overCls : 'clsListItemOver',
									text : '<font style="font-size : 12px !important;color:#828282;margin-left: 24px !important;">☆&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;实时信息</font>',
									title : '实时信息',
									itemid:'itm_infoquery_id',
									id:'id_infoquery_id',
									srcurl:'ncViewer.mac_infoquery_current',
									listeners:{
									    mouseover:this.changeOnOver,
										mouseout:this.changeOnOut,
										click:this.onItemClick
									}
								},{
									xtype : 'button',
									iconCls: "mi_alarm", 
									cls:"cls_menu_first_normal", 
									bodyStyle: 'background:#F0F3F8;',
									style: 'border-radius:0;background:#D5DDF0; margin:1px 0 0px 0px;',
									text : '<font style="font-size : 14px !important;color:#0D3967;margin:10px 0 0 60px !important;">布控告警</font>',
									id : 'id_alarm',
									itemid : 'itm_alarm',
									listeners:{
										click:this.onToggleFirst
									}
								},{
									xtype : 'button',
									hidden  : true,
									cls:"cls_menu_second_normal", 
									//overCls : 'clsListItemOver',
									text : '<font style="font-size : 12px !important;color:#828282;margin-left: 24px !important;">☆&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;布控管理</font>',
									title : '布控管理',
									itemid:'itm_alarm_controlmanage',
									id:'id_alarm_controlmanage',
									srcurl:'ncViewer.mac_monitoralarm_controlmanage',
									listeners:{									
										mouseover:this.changeOnOver,
										mouseout:this.changeOnOut,
										click:this.onItemClick
									}
								},{
									xtype : 'button',
									hidden  : true,
									cls:"cls_menu_second_normal", 
									//overCls : 'clsListItemOver',
									text : '<font style="font-size : 12px !important;color:#828282;margin-left: 24px !important;">☆&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;接警人员管理</font>',
									title : '接警人员管理',
									itemid:'itm_alarm_peoplecalled',
									id:'id_alarm_peoplecalled',
									srcurl:'ncViewer.mac_monitoralarm_peoplecalled',  
									listeners:{
										mouseover:this.changeOnOver,
										mouseout:this.changeOnOut,
										click:this.onItemClick
									}
								},{
									xtype : 'button',
									iconCls: "mi_dm", 
									cls:"cls_menu_first_normal", 
									bodyStyle: 'background:#F0F3F8;',
									style: 'border-radius:0;background:#D5DDF0; margin:1px 0 0px 0px;',
									text : '<font style="font-size : 14px !important;color:#0D3967;margin:10px 0 0 60px !important;">数据挖掘</font>',
									itemid : 'itm_dm',
									id:'id_dm',
									listeners:{
										click:this.onToggleFirst
									}								
								},{
									xtype : 'button',
									hidden  : true,
									cls:"cls_menu_second_normal", 
									//overCls : 'clsListItemOver',
									text : '<font style="font-size : 12px !important;color:#828282;margin-left: 24px !important;">☆&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;伴随行为分析</font>',
									title : '伴随行为分析',
									itemid:'itm_dm_bab',
									id:'id_dm_bab',
									srcurl:'ncViewer.mac_dm_bab',
									listeners:{
										mouseover:this.changeOnOver,
										mouseout:this.changeOnOut,
										click:this.onItemClick
									}
								},{
									xtype : 'button',
									hidden  : true,
									cls:"cls_menu_second_normal", 
									//overCls : 'clsListItemOver',
									text : '<font style="font-size : 12px !important;color:#828282;margin-left: 24px !important;">☆&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;抽样分析</font>',
									title : '抽样分析',
									itemid:'itm_dm_sample',
									id:'id_dm_sample',
									srcurl:'ncViewer.mac_dm_sample', 
									listeners:{
										mouseover:this.changeOnOver,
										mouseout:this.changeOnOut,
										click:this.onItemClick
									}
								},{
									xtype : 'button',
									hidden  : true,
									cls:"cls_menu_second_normal", 
									//overCls : 'clsListItemOver',
									text : '<font style="font-size : 12px !important;color:#828282;margin-left: 24px !important;">☆&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;高频次分析</font>',
									title : '高频次分析',
									itemid:'itm_dm_freCharact',
									id:'id_dm_freCharact',
									srcurl:'ncViewer.mac_dm_freCharact',
									listeners:{
										mouseover:this.changeOnOver,
										mouseout:this.changeOnOut,
										click:this.onItemClick
									}
								},{
									xtype : 'button',
									hidden  : true,
									cls:"cls_menu_second_normal", 
									//overCls : 'clsListItemOver',
									text : '<font style="font-size : 12px !important;color:#828282;margin-left: 24px !important;">☆&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;轨迹分析</font>',
									title : '轨迹分析',
									itemid:'itm_dm_track',
									id:'id_dm_track',
									srcurl:'ncViewer.mac_dm_track',
									listeners:{
										mouseover:this.changeOnOver,
										mouseout:this.changeOnOut,
										click:this.onItemClick
									}
								},{
									xtype : 'button',
									hidden  : true,
									cls:"cls_menu_second_normal", 
									//overCls : 'clsListItemOver',
									text : '<font style="font-size : 12px !important;color:#828282;margin-left: 24px !important;">☆&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;可疑热点分析</font>',
									title : '可疑热点分析',
									itemid:'itm_dm_sus',
									id:'id_dm_sus',
									srcurl:'ncViewer.mac_dm_sus',
									listeners:{
										mouseover:this.changeOnOver,
										mouseout:this.changeOnOut,
										click:this.onItemClick
									}
								},{
									xtype : 'button',
									hidden  : true,
									cls:"cls_menu_second_normal", 
									//overCls : 'clsListItemOver',
									text : '<font style="font-size : 12px !important;color:#828282;margin-left: 24px !important;">☆&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;人流分析</font>',
									title : '人流分析',
									itemid:'itm_dm_crowd',
									id:'id_dm_crowd',
									srcurl:'ncViewer.mac_dm_crowd',
									listeners:{
										mouseover:this.changeOnOver,
										mouseout:this.changeOnOut,
										click:this.onItemClick
									}
								},{
									xtype : 'button',
									iconCls: "mi_operation", 
									cls:"cls_menu_first_normal", 
									bodyStyle: 'background:#F0F3F8;',
									style: 'border-radius:0;background:#D5DDF0; margin:1px 0 0px 0px;',
									text : '<font style="font-size : 14px !important;color:#0D3967;margin:10px 0 0 60px !important;">业务配置</font>',
									id : 'id_operation',
									itemid : 'itm_operation',
									listeners:{
										click:this.onToggleFirst
									}
								},{
									xtype : 'button',
									hidden  : true,
									cls:"cls_menu_second_normal", 
									//overCls : 'clsListItemOver',
									text : '<font style="font-size : 12px !important;color:#828282;margin-left: 24px !important;">☆&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;场所管理</font>',
									title : '场所管理',
									itemid:'itm_operation_place',
									id:'id_operation_place',
									srcurl:'ncViewer.mac_operation_place',  
									listeners:{
										mouseover:this.changeOnOver,
										mouseout:this.changeOnOut,
										click:this.onItemClick
									}
								},{
									xtype : 'button',
									hidden  : true,
									cls:"cls_menu_second_normal", 
									//overCls : 'clsListItemOver',
									text : '<font style="font-size : 12px !important;color:#828282;margin-left: 24px !important;">☆&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;设备管理</font>',
									title : '设备管理',
									itemid:'itm_operation_equipment',
									id:'id_operation_equipment',
									srcurl:'ncViewer.mac_operation_equipment',  
									listeners:{
										mouseover:this.changeOnOver,
										mouseout:this.changeOnOut,
										click:this.onItemClick
									}
								},{
									xtype : 'button',
									hidden  : true,
									cls:"cls_menu_second_normal", 
									//overCls : 'clsListItemOver',
									text : '<font style="font-size : 12px !important;color:#828282;margin-left: 24px !important;">☆&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;安全厂商管理</font>',
									title : '安全厂商管理',
									itemid:'itm_operation_safety',
									id:'id_operation_safety',
									srcurl:'ncViewer.mac_operation_safety',  
									listeners:{
										mouseover:this.changeOnOver,
										mouseout:this.changeOnOut,
										click:this.onItemClick
									}
								},{
									xtype : 'button',
									hidden  : true,
									cls:"cls_menu_second_normal", 
									//overCls : 'clsListItemOver',
									text : '<font style="font-size : 12px !important;color:#828282;margin-left: 24px !important;">☆&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;合规热点信息管理</font>',
									title : '合规热点信息管理',
									itemid:'itm_operation_normally',
									id:'id_operation_normally',
									srcurl:'ncViewer.mac_operation_normally',  
									listeners:{
										mouseover:this.changeOnOver,
										mouseout:this.changeOnOut,
										click:this.onItemClick
									}
								},{
									xtype : 'button',
									hidden  : true,
									cls:"cls_menu_second_normal", 
									//overCls : 'clsListItemOver',
									text : '<font style="font-size : 12px !important;color:#828282;margin-left: 24px !important;">☆&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;终端身份信息维护</font>',
									title : '终端身份信息维护',
									itemid:'itm_operation_terminal',
									id:'id_operation_terminal',
									srcurl:'ncViewer.mac_operation_terminal',  
									listeners:{
										mouseover:this.changeOnOver,
										mouseout:this.changeOnOut,
										click:this.onItemClick
									}
								},{
									xtype : 'button',
									iconCls: "mi_systemset", 
									cls:"cls_menu_first_normal", 
									bodyStyle: 'background:#F0F3F8;',
									style: 'border-radius:0;background:#D5DDF0; margin:1px 0 0px 0px;',
									text : '<font style="font-size : 14px !important;color:#0D3967;margin:10px 0 0 60px !important;">系统配置</font>',
									id : 'id_systemset',
									itemid : 'itm_systemset',
									listeners:{
										click:this.onToggleFirst
									}
								},{
									xtype : 'button',
									hidden  : true,
									cls:"cls_menu_second_normal", 
									//overCls : 'clsListItemOver',
									text : '<font style="font-size : 12px !important;color:#828282;margin-left: 24px !important;">☆&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;管理员账号</font>',
									title : '管理员账号',
									itemid:'itm_systemset_admin',
									id:'id_systemset_admin',
									srcurl:'ncViewer.mac_systemset_admin',  
									listeners:{
										mouseover:this.changeOnOver,
										mouseout:this.changeOnOut,
										click:this.onItemClick
									}
								},{
									xtype : 'button',
									hidden  : true,
									cls:"cls_menu_second_normal", 
									//overCls : 'clsListItemOver',
									text : '<font style="font-size : 12px !important;color:#828282;margin-left: 24px !important;">☆&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;系统运维告警</font>',
									title : '系统运维告警',
									itemid:'itm_systemset_alarm',
									id:'id_systemset_alarm',
									srcurl:'ncViewer.mac_systemset_alarm',  
									listeners:{
										mouseover:this.changeOnOver,
										mouseout:this.changeOnOut,
										click:this.onItemClick
									}
								},{
									xtype : 'button',
									hidden  : true,
									cls:"cls_menu_second_normal", 
									//overCls : 'clsListItemOver',
									text : '<font style="font-size : 12px !important;color:#828282;margin-left: 24px !important;">☆&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;系统参数配置</font>',
									title : '系统参数配置',
									itemid:'itm_systemset_preferences',
									id:'id_systemset_preferences',
									srcurl:'ncViewer.mac_systemset_preferences',  
									listeners:{
										mouseover:this.changeOnOver,
										mouseout:this.changeOnOut,
										click:this.onItemClick
									}
								},{
									xtype : 'button',
									hidden  : true,
									cls:"cls_menu_second_normal", 
									//overCls : 'clsListItemOver',
									text : '<font style="font-size : 12px !important;color:#828282;margin-left: 24px !important;">☆&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;系统运维日志</font>',
									title : '系统运维日志',
									itemid:'itm_systemset_log',
									id:'id_systemset_log',
									srcurl:'ncViewer.mac_systemset_log',  
									listeners:{
										mouseover:this.changeOnOver,
										mouseout:this.changeOnOut,
										click:this.onItemClick
									}
								}
							]
						}          
					],
				dockedItems: [{
					dock: 'top',
	 //           margins: '0 0 0 0',
					border:true,
					xtype: 'toolbar',
					items: [
								{     
									width:          200,
									xtype:          'treecombox',
									mode:           'local',
									value:          '0',
									triggerAction:  'all',
									forceSelection: true,
									iconCls:'add',
									editable:       false,
									fieldLabel:     '',
									iconCls:'remove',
									style:'color:red;background:green;',
									emptyText:'选择单位查询',
									name:           'groupname',
									displayField:   'text',
									valueField:     'id',
									queryMode: 'local',
									value : '',
									store:store_classtree
								}            
						]
				}]

	 //            items:[left_tabs]
			});
						
			return this.leftPanel;
    },
    createNcCenter:function(){
		//   var charPort=Ext.create('widget.ncCharPoral');
		servicecode='';
		corpdid='';
		ncsgroupid='';    
		compid='';
		dxtitle='';
		var sy_obj;
		var sy_title,sy_id;
		
		if(right_sy=='日志查看'){
			sy_title='日志查看';
			sy_id='rzck';
			sy_obj=ncViewer.mac_operation_equipment;
		}
		else if(right_sy=='单位管理'){
			sy_obj=ncViewer.mac_operation_equipment;
			sy_title='单位管理';
			sy_id='dwgl';
		}
		else if(right_sy=='运行分析'){
			sy_obj=ncViewer.mac_operation_equipment;
			sy_title='运行分析';
			sy_id='yxfx';
		}
		else if(right_sy=='接入厂商管理'){
			sy_obj=ncViewer.mac_operation_equipment;
			sy_title='接入厂商管理';
			sy_id='jrcs';
		}
		else if(right_sy=='统计报表'){
			sy_obj=ncViewer.mac_operation_equipment;
			sy_title='统计报表';
			sy_id='tjbb';
		}
		else if(right_sy=='系统管理'){
			sy_obj=ncViewer.mac_operation_equipment;
			sy_title='系统管理';
			sy_id='system';
		}
		else if(right_sy=='系统设置'){
			sy_obj=ncViewer.mac_operation_equipment;
			sy_title='系统设置';
			sy_id='sysset';
		}
		else{
			sy_obj=ncViewer.frontPage;
			sy_title='首页';
			sy_id='id_frontPage';
		}
      
    	this.centerPanel=Ext.createWidget('tabpanel', {
			resizeTabs: true,
			enableTabScroll: true,
			region:'center',
        	id:'layout_center',
        	margins: '2 2 1 0',
			defaults: {
				autoScroll: false,
				bodyPadding: 0
			},
			items: [
			{
				closable: true,
				autoScroll:false,
				id:sy_id,
				iconCls: 'add16',          
				title: sy_title,
				items:[Ext.create(sy_obj)]
			}],
			plugins: Ext.create('Ext.ux.TabCloseMenu', {
				extraItemsTail: [
					'-',
					{
						text: 'Closable',
						checked: true,
						hideOnClick: true,
						handler: function (item) {
							currentItem.tab.setClosable(item.checked);
						}
					},
					'-',
					{
						text: 'Enabled',
						checked: true,
						hideOnClick: true,
						handler: function(item) {
							currentItem.tab.setDisabled(!item.checked);
						}
					}
				],
            listeners: {
                aftermenu: function () {
                    currentItem = null;
                },
                beforemenu: function (menu, item) {
                    menu.child('[text="Closable"]').setChecked(item.closable);
                    menu.child('[text="Enabled"]').setChecked(!item.tab.isDisabled());
                    currentItem = item;
                }
            }
         }),
         listeners: {              
            tabchange:function(tp,p){
				/*
            	grid_height=Ext.getCmp('layout_center').getHeight()-36;
            	grid_width=Ext.getCmp('layout_center').getWidth()-56;
				var tabs_center=Ext.getCmp("layout_center");
        	    var active = tabs_center.getActiveTab();
				var frame1 = Ext.get(active.el).frame();
				var childComp = frame1.getCmp("keyword"); //取得控件
				childComp.setText("success");
				
				console.log(active.getId());
        	    
              //console.log(active.getId());
		        	var active_id='lan_'+active.id;
		        	if(active_id=='lan_hotinfo'||active_id=='itm_infoquery_mac'||active_id=='itm_infoquery_move'){
		        		return;
		        	}
	    
		        	var tabs_if=Ext.getCmp(active_id).getActiveTab();
		        	if(tabs_if){
			          if(tabs_if.id=='immon'){
			           	bStartcomp=0;
			           	bStartim=1;
			          }
			          else if(tabs_if.id=='sjcomputer'){
			          	bStartcomp=1;
			          	bStartim=0;
			          }
          		  else if(tabs_if.id=='sspm'){
          		 	  bStartcomp=0;
			          	bStartim=0;
          		  }
          		  var vframe= active_id+"_"+tabs_if.getId()
          		  //alert(vframe);
          		  try{
            		  eval(vframe).reflash();
            		}catch(e){}
          		}
			*/
          	}
         }
                
       });

//     console.log(right_fun); 
      
       var right_fun_s=right_fun;
       var item2 = Ext.getCmp("maintab").items.items;
//console.info(right_fun_s);  



   /*
      for(var i=0;i<item2.length;i++){
   //   alert(right_fun_s);
      	
        //console.log(item2[i]);
        try{
          if((right_fun_s.indexOf(item2[i].text)!=-1)){
        	  if(item2[i].itemid=="jcxxgl"){
        	    continue;
        	  }
        	  
    if(item2[i].itemid=='dwgl'){
          this.onItemClick(item2[i]);
            break;
          }
        	}
        }catch(e){}  
  	
      }
       
       */
       
       return this.centerPanel;
    },
    createNcStatus:function(){   	
    	this.ncstatus=Ext.create('Ext.ux.StatusBar', {
        id: 'statusbar',
        region: 'south',
        margins: '1 2 0 2',
        defaultText: 'Default status text',
        text: 'Ready',
        iconCls: 'x-status-valid',
        items: [
			{
				xtype: 'button',
				text: ''
			},{
				xtype: 'button',
				text: '上海新网程信息技术股份有限公司&nbsp;&nbsp;',
				handler: function (){
					var sb = Ext.getCmp('statusbar');
					sb.setStatus({
						text: 'Oops!',
						iconCls: 'x-status-error',
						clear: true // auto-clear after a set interval
					});
				}
			}]
      });
      return this.ncstatus;
    },
	onToggleFirst:function(item){
		if('itm_infoquery' == item.itemid){
			toggleInfoquery(!bInfoquery);
			toggleAlarm(false);
			toggleDm(false);
			toggleOperation(false);
			toggleSystemset(false);
		}else if('itm_alarm' == item.itemid){
			toggleInfoquery(false);
			toggleAlarm(!bAlarm);
			toggleDm(false);
			toggleOperation(false);
			toggleSystemset(false);
		}else if('itm_dm' == item.itemid){
			toggleInfoquery(false);
			toggleAlarm(false);
			toggleDm(!bDm);
			toggleOperation(false);
			toggleSystemset(false);
		}else if('itm_operation' == item.itemid){
			toggleInfoquery(false);
			toggleAlarm(false);
			toggleDm(false);
			toggleOperation(!bOperation);
			toggleSystemset(false);
		}else if('itm_systemset' == item.itemid){
			toggleInfoquery(false);
			toggleAlarm(false);
			toggleDm(false);
			toggleOperation(false);
			toggleSystemset(!bSystemset);
		}
		
	},
	changeOnOut:function(item){
		//Ext.fly(item.id).removeCls("x-btn-center_3");
		//Ext.fly(item.id).addCls("cls_menu_second_normal");
		if(item.id != globalSelectId){
			Ext.getCmp(item.id).setText('<font style="font-size : 12px !important;color:#828282;margin-left: 24px !important;">☆&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;'+item.title+'</font>');
		}				
	},
	changeOnOver:function(item){
		/*
		var myself = Ext.getCmp(item.id);
		myself.removeClass("cls_menu_second_normal");
		myself.addClass("x-btn-center_3");	
		*/
		//Ext.fly(item.id).removeCls("cls_menu_second_normal");
		//Ext.fly(item.id).addCls("x-btn-center_3");	
		if(item.id != globalSelectId){
			Ext.getCmp(item.id).setText('<font style="font-size : 12px !important;color:#328BE4;margin-left: 24px !important;">★&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;'+item.title+'</font>');
		}
	},
    onItemClick:function(item){
    	 //bbbb=document.all('ltitle');
		toggleClickItem(item);
    	var tab_title='';
    	var temp_text=item.text;    	 
    	if(temp_text.indexOf('font')== -1){
			tab_title=temp_text;
    	}
    	else{
    		tab_title=item.title;
    	}
		Ext.getCmp('left').setTitle('当前位置：'+tab_title);          
		//改变中间的视图      
		var i=0;
		var m=0;
		var tab_src=item.srcurl;
		var tab_id=item.itemid;
		var tabs_center=Ext.getCmp("layout_center");
		var tab_len=tabs_center.items.length;
		tabs_center.items.each(function(item) { 
	     	if(item.id==tab_id)
	     	{ 
				i++;
			}
			if(i==0){m++;}
		});   
		if(i>0){
			tabs_center.setActiveTab(m);
			return;
     	} 
		if(tab_len>5){
			tabs_center.remove(0); 
			//tabs_center.remove(1);  
		}  
		//alert(grid_height);
		//grid_height=Ext.getCmp('layout_center').getHeight()-36;        
		//grid_width=Ext.getCmp('layout_center').getWidth()-56;
		var grid=Ext.create(tab_src);
		/*暂时去掉权限检查
		var str = right_fun;
//console.log(right_fun);      
		grid.items.each(function(item){
	 //   console.info(str);
	 //   console.info(item.getId());
	 //   console.info(str.indexOf(item.getId()));
	//     console.log(item.getId());   	
			if(str.indexOf(item.getId())== -1){
				item.destroy();
			}
		});
     */
                  	
		tabs_center.add({
			closable: true,
			autoScroll:false,
			id:tab_id,
			iconCls: 'add16',          
			title: tab_title,
			items:[grid]
		}).show();
    },
    showTabs:function(parentTab,childTab){
		alert("show tabs");
    	/*
    	var i=0;
			var m=0;
			var tab_title=parentTab.text;
			var tab_src=parentTab.srcurl;
			var tab_id=parentTab.itemid;
			var tabs_center=Ext.getCmp("layout_center");
			var tab_len=tabs_center.items.length;
			tabs_center.items.each(function(item) { 
				if(item.id==tab_id){
					i++;
				}
				if(i==0){
					m++;
				}
			});
      if(i>0){
      	tabs_center.remove(m);
     	} 
      if(tab_len>5){
      	tabs_center.remove(1);  
      }

      var grid=Ext.create(tab_src,{activeTab:childTab});
      
      //权限处理
      var str = right_fun;
    
      grid.items.each(function(item){
//console.info(str,",",item.getId(),",",str.indexOf(item.getId()));
      	if(str.indexOf(item.getId())== -1){
      		item.destroy();
      	}
      });
      
      tabs_center.add({
        closable: true,
        autoScroll:false,
        id:tab_id,
        iconCls: 'add16',          
        title: tab_title,
        items:[grid]
      }).show();
      //Ext.getCmp('lay_'+tab_id).setActiveTab(childTab);
      */
  
    },
    refreshComTree: function(){
      Ext.getCmp('treeBtn').fireEvent('click');
    }

 
});
