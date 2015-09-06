   var rolestore = Ext.create('Ext.data.Store', {
        pageSize: 25,
        model: 'EimData',
        remoteSort: true,
        proxy: {
            type: 'ajax',
            url: '/pronline/Msg?FunName@ncsRolelist_v9',
            reader: {
                type:'json',
                root: 'eimdata',
                totalProperty: 'totalCount'
            },         
            simpleSortMode: true
        },
        sorters: [{
            property: 'userid',
            direction: 'DESC'
        }]
    });

Ext.define('proauthXtgl.controller.Procy', {
  extend: 'Ext.app.Controller',  
  stores: ['List'],
	models: ['List'],  
  views: [ 'list.List'],
  init: function() {
  	store_system_role.load();
//  	store_fac.load();

  
this.control({
		
		  'xtgllist':{
		    render: this.showRender
		  },
		  
		  //关键字查询
		  'xtgllist button[action=keycx]':{
				click: this.xtglkeycx
			},
			//添加单位
		  'xtgllist button[action=add]':{
				click: this.xtgladd
			},
		  			//删除单位
		  'xtgllist button[action=del]':{
				click: this.xtgldel
			}
			,
			//编辑单位
		  'xtgllist button[action=edit]':{
				click: this.xtgledit
			}
			,
			//高级查询
		  'xtgllist button[action=advancecx]':{
				click: this.xtglavcx
			}

			
		});
  },
  
  showRender: function(){

  var store = Ext.ComponentQuery.query('xtgllist')[0].getStore();
  
  store.on('beforeload', function (store, options) {
	  var keyword=Ext.getCmp('keyword').value;
	  var groupid=parent.ncsgroupid;
	  var did=parent.corpdid;
	    if(did){
       groupid = "";
     }
	  var new_params={groupid:groupid,did:did,keyword:keyword};
     Ext.apply(store.proxy.extraParams,new_params);
      });	
   
   proauthXtgl.controller.Procy.loadProcyListStore();
 
  },
  //关键字查询
  xtglkeycx: function(thisBtn){
    proauthXtgl.controller.Procy.loadProcyListStore();
  },
  xtgldel: function(thisBtn){
  	var grid = Ext.ComponentQuery.query('xtgllist')[0];
    var rows = grid.getSelectionModel().getSelection();

     if(rows){
     	           var selsid;
     	            selsid='';
     	            Ext.MessageBox.confirm('确认', '确定删除该记录？', function(btn){
     	            	if(btn=='yes'){
     	       var store = grid.getStore();
     	
                  for(var i=0;i<rows.length;i++){
                  	if(i==0){
                  	 selsid=rows[i].get('userid');
                  	}
                  	else{
                       selsid=selsid+","+rows[i].get('userid');
                    }
                   
                   }

                   store.load({params:{del:"del",selsid:selsid}});
            
                  
                 }
     	        })
     }
    
    
  //  proauthDwgl.controller.Procy.loadProcyListStore();
  },

  xtgladd:function(){
    proauthXtgl.controller.Procy.showedit('','');

           
  }

  
  /**************************************
  * 策略标签 
  ***************************************/  
  showTips: function( thisTV, eOpts ){
    thisTV.tip = Ext.create('Ext.tip.ToolTip', {
      target: thisTV.el,
      trackMouse: true,
      dismissDelay : 60000,
      html: '<p>例如：</p><p>&nbsp;&nbsp;&nbsp;单个端口：80,21</p><p>&nbsp;&nbsp;&nbsp;端口段：2000~3000</p><p>&nbsp;&nbsp;&nbsp;组合：80,2000~3000,3005</p>'
    });
  },
  /**************************************
  * 全局函数 
  ***************************************/
  inheritableStatics:{
    loadProcyListStore:function(){
      var store = Ext.ComponentQuery.query('xtgllist')[0].getStore();
      var keyword=Ext.getCmp('keyword').value;
      store.load();
    },
    SetPage:function(curpage){
      var store = Ext.ComponentQuery.query('xtgllist')[0].getStore();
      store.currentPage=curpage;
       
    },
    showedit:function(userid,record){	
    	 //系统角色
    
      var temp_check='';
	     if(record!=''){
	         temp_check=record.data.role;	  
	     }
				var dataStr = "["; 
		    var i=0;
     
          var bCheck;
		      store_system_role.each(function(item){
		       bCheck="false";
		       if(temp_check.indexOf(item.get('username'))!=-1){
		           bCheck="true";
		       }
		      
           var caV;
           caV="v"+i;
	         if(i==0){
	       	
		       	    dataStr=dataStr+"{boxLabel: '"+item.get('username')+"' ,inputValue:'" +item.get('userid')+"',checked:"+bCheck+",name:'"+caV+"'}";
		       }
		       else{
		       	  dataStr=dataStr+",{boxLabel: '"+item.get('username')+"' ,inputValue:'" +item.get('userid')+"',checked:"+bCheck+",name:'"+caV+"'}";
		       }


		       i++;
		
		      });
          dataStr=dataStr+"]";
      	
 	    	var rightcheckGroup={
           xtype:'fieldset',
           title:'选择角色',
           layout: 'form',
        	 defaults: {
               labelStyle: 'padding-left:4px;'
        	 },
            collapsible: false,
            items: [{
              xtype: 'checkboxgroup',
                name:'rightgroup',
                columns: 4,
                items: eval(dataStr)
            }]
            };
            
    	  var fp2=Ext.create('Ext.FormPanel', {
        frame: true,
        fieldDefaults: {
            labelWidth: 110
        },
        width: 600,
        bodyPadding: 10,
        items: [
             {
                xtype: 'fieldset', title: '系统用户信息',
                collapsible: false,
                defaults: {labelWidth: 60,anchor: '100%',layout: {type: 'hbox',defaultMargins: {top: 0, right: 5, bottom: 0, left: 0}}},
                items: [              
                {
                  xtype: 'fieldcontainer',     
                  defaults: {labelWidth: 60, hideLabel: false},
                items: [
										{xtype: 'textfield',fieldLabel: '登录名', value: '',name:'username',width:250},
										{xtype: 'textfield',fieldLabel: '姓名', value: '',name:'dispname',width:250}
                ]
                },
                {
                xtype: 'fieldcontainer',     
                defaults: {labelWidth: 60, hideLabel: false},
                items: [
										{xtype: 'textfield',inputType: 'password',fieldLabel: '密码', value: '',name:'password1',width:250},
										{xtype: 'textfield',inputType: 'password',fieldLabel: '密码确认', value: '',name:'password2',width:250}
                ]
                },
                {
                xtype: 'fieldcontainer',     
                defaults: {labelWidth: 60, hideLabel: false},
                items: [
										{
                             
                                width:          250,
                                xtype:          'treecombox',
                   //             id: 'xwzsgroupid_first',
                                mode:           'local',
                                value:          '0',
                                triggerAction:  'all',
                                forceSelection: true,
       //                         iconCls:'add',
                                editable:       false,
                                fieldLabel:     '选择区域',
                                	iconCls:'remove',
                                name:           'groupname',
                                displayField:   'text',
                                valueField:     'id',
                                queryMode: 'local',
                                value : '',
                                store:store_classtree
                  
											}
				             ,
										{                              
                                width:         250,
                                xtype:          'treecomboxcheck',                               
                                triggerAction:  'all',                               
                                fieldLabel:     '管理范围',
                                optid:'optid',
                                opname:'opt',
                                id:'opt',
                                name:           'opt',
                                displayField:   'text',
                                valueField:     'id',
                             
                                value : '',
                                store:store_groupcheck
                   },
                   {xtype: 'hidden',name: 'facid', id:'facid',value: ''}
							
                ]
                },
                {
                xtype: 'fieldcontainer',     
                defaults: {labelWidth: 60, hideLabel: false},
                items: [
										{                              
                                width:         250,
                                xtype:          'treecomboxcheck_f',                               
                                triggerAction:  'all',                               
                                fieldLabel:     '接入厂商',
                                optid:'facid',
                                opname:'fac',
                                id:'fac',
                                name:           'fac',
                                displayField:   'text',
                                valueField:     'id',
                             
                                value : '',
                                store:store_faccheck
                   },
                  {                              
                                width:          250,
                                xtype:          'combo',
                                mode:           'local',
                                value:          '0',
                                triggerAction:  'all',
                                forceSelection: true,
                                editable:       false,
                                fieldLabel:     '首页显示',
                                name:           'level',
                                displayField:   'name',
                                valueField:     'value',
                                allowBlank: true,
                                queryMode: 'local',
                                store: Ext.create('Ext.data.Store', {
                                    fields : ['name', 'value'],
                                    data   : [                                       
                                        {name : '单位管理',  value: '1'}
                                        ,{name : '日志查看',  value: '6'}
                                        ,{name : '运行分析',  value: '12'}
                                        ,{name : '接入厂商管理',value:'13'}
                                        ,{name : '统计报表',  value: '14'}
                                        ,{name : '系统管理',  value: '5'}
                                        ,{name : '系统设置',  value: '15'}
                                    ]
                                })
                   }
                   ,
                                      
                   {xtype: 'hidden',name: 'optid', id:'optid',value: ''},
                   {xtype: 'hidden',name: 'userid', value: ''},
									 {xtype: 'hidden',name: 'sumright', value: ''}
							
                ]
                },
                {
                xtype: 'fieldcontainer',     
                defaults: {labelWidth: 60, hideLabel: false},
                items: [
										{xtype: 'textfield',fieldLabel: '联系方式', value: '',name:'email',width:250},
										{                              
                                width:          250,
                                xtype:          'combo',
                                mode:           'local',
                                value:          '0',
                                triggerAction:  'all',
                                forceSelection: true,
                                editable:       false,
                                fieldLabel:     '帐号状态',
                                name:           'useflags',
                                displayField:   'name',
                                valueField:     'value',
                                allowBlank: false,
                                queryMode: 'local',
                                store: Ext.create('Ext.data.Store', {
                                    fields : ['name', 'value'],
                                    data   : [                                       
                                        {name : '正常',  value: '0'}
                                        ,{name : '禁用',  value: '1'}
                                       

                                    ]
                                })
                            }
                ]
                }
                
                
  /*              
                ,
                {
                xtype: 'fieldcontainer',     
                defaults: {labelWidth: 60, hideLabel: false},
                items: [
													{                              
                                width:          250,
                                xtype:          'combo',
                                mode:           'local',
                
                                triggerAction:  'all',
                          //      forceSelection: true,
                                editable:       false,
                                fieldLabel:     '管理数据源',
                                name:           'fcode',
                                displayField:   'dispname',
                                valueField:     'code',
                                allowBlank: false,
                                queryMode: 'local',
                                store: store_fac
                            }
                            ,
										{                              
                                width:          250,
                                xtype:          'combo',
                                mode:           'local',
                                value:          '0',
                                triggerAction:  'all',
                                forceSelection: true,
                                editable:       false,
                                fieldLabel:     '首页显示',
                                name:           'level',
                                displayField:   'name',
                                valueField:     'value',
                                allowBlank: false,
                                queryMode: 'local',
                                store: Ext.create('Ext.data.Store', {
                                    fields : ['name', 'value'],
                                    data   : [                                       
                                        {name : '综合统计',  value: '0'}
                                        ,{name : '实时监控',  value: '1'}
                                       

                                    ]
                                })
                            }
                ]
                }
                                
*/         
                   
                ]
            }
           ,
        
        
        	rightcheckGroup
        ],
        buttons: [{
            text: '保存',
            handler: function(){
            	if(fp2.getForm().getValues().password1!=fp2.getForm().getValues().password2){
            		alert('输入的密码不一张');
            		return;
            	}
            	
            	var tpass=fp2.getForm().getValues().password1;
            	if(tpass.length<8){
            		alert('密码长度不能小于8位');
            		return;
            	}
         if(!isNaN(tpass)){
						alert('密码必须同时包含字符和数字');
						return ;
					}
		var reg= /^[A-Za-z]+$/;
		if(reg.test(tpass)){
			alert('密码必须同时包含字符和数字');
			return;	
		}
				
					var baa=tpass.match(/\D/);
					if(!baa){
								alert('密码必须同时包含字符和数字');
						return ;
					}
            	
            	
            	if(fp2.getForm().getValues().username==''||fp2.getForm().getValues().dispname==''){
            		alert('登录名或姓名不能为空');
            		return ;
            	}
            		
            var groupcheck=fp2.getForm().findField('rightgroup');
            var checkvalue=groupcheck.getValue();
            var sum=store_system_role.getCount();
            fp2.getForm().findField('sumright').setValue(sum);
            
             fp2.form.doAction('submit',{
               url:'/pronline/Msg',
                method:'POST',
               params:{FunName:'ncAdminUserSave_v8'},
                 
                success:function(form,action){ 
                	alert(action.result.msg);
         		//			Ext.getCmp('addwindow').close();
         		//		  store_adminuser.currentPage=1;
            //      store_adminuser.load();
         					 var store = Ext.ComponentQuery.query('xtgllist')[0].getStore();
         					 store.load();
         					  Ext.getCmp('addwindow').close();
                 },
             		 failure:function(form,action){        		 
                     alert(action.result.msg);
                 
            		 }
                }
             )  
  
 
           
            }
        },{
            text: '关闭',
            handler: function(){
            Ext.getCmp('addwindow').close();

            }
         }]
      });
        
      if(userid!=''){
      	fp2.loadRecord(record);
      	
      }
      
         var win2 = new Ext.Window ({
                             id:"addwindow",
                             title:"增加或修改系统用户",
                             x:200,
                             y:20,
                             width:630,
                             height:600,
                             bodyPadding: 10,
                             modal:true,
                             resizable:true,
           //                  draggable:false,
                             items: fp2
                                               
         })
        win2.show();    
      
    	
    
       
    },
    setTitle: function(title){
       document.getElementById("titledx").innerHTML='&nbsp'+title+'&nbsp;&nbsp;';   
 /*     
      var grid = Ext.ComponentQuery.query('xtgllist')[0];
      var oldtitle = grid.title;
   
      var str = "";
      var index = oldtitle.indexOf('(');
      if(index>0){
        str = oldtitle.substring(0,index);
      }else{
        str = oldtitle;
      }
      
      grid.setTitle(str+"("+title+")");
 */
    }
  }
});
