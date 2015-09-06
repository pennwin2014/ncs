Ext.define('proauthMobileAccount.controller.Procy', {
  extend: 'Ext.app.Controller',  
   stores: ['List'],
 	models: ['List'],  
  views: [ 'list.List'],
  init: function() {

		this.control({
		
		  'mobileaccountlist':{
	//	    select: this.showTab,
		    render: this.showRender
		  },
		  
		  //关键字查询
		  'mobileaccountlist button[action=keycx]':{
				click: this.dwglkeycx
			},
			//添加消息
		  'mobileaccountlist button[action=addmesg]':{
				click: this.dwgladd
			},
			//移动部门
		  'mobileaccountlist button[action=saveBtnText]':{
				click: this.movertogroup
			},
		  			//删除单位
		  'mobileaccountlist button[action=del]':{
				click: this.dwgldel
			}
			,
			//编辑单位
		  'mobileaccountlist button[action=edit]':{
				click: this.dwgledit
			}
			,
			//高级查询
		  'mobileaccountlist button[action=advancecx]':{
				click: this.dwglavcx
			}
			
		});
  }
 
  ,
  
  showRender: function(){
  
  var store = Ext.ComponentQuery.query('mobileaccountlist')[0].getStore();
  
  store.on('beforeload', function (store, options) {
	  var keyword=Ext.getCmp('keyword_mobileaccount').value;
	  var flag=Ext.getCmp('whflag').value;
	  var groupid=parent.ncsgroupid;
	  var did=parent.corpdid;
	    if(did){
       groupid = "";
     }
	  var new_params={groupid:groupid,did:did,keyword:keyword,flag:flag};
     Ext.apply(store.proxy.extraParams,new_params);
       });	

   proauthMobileAccount.controller.Procy.loadProcyListStore();
 //   proauthMobileAccount.controller.Procy.setTitle(parent.dxtitle);
  },
  //关键字查询
  dwglkeycx: function(thisBtn){
    proauthMobileAccount.controller.Procy.loadProcyListStore();
  },
   dwgladd: function(thisBtn){
   	
   	   var facCombo = Ext.create('Ext.form.field.ComboBox', {
        fieldLabel: '接收对象',
        labelWidth: 65,
        width: 223,
        valueField:'groupid',
        displayField: 'groupname',
        name : 'mto',      
        allowBlank: false,
//      forceSelection: true,
        store: store_fac,
        triggerAction: 'all',
//      queryMode:'remote',
         queryMode: 'local',
         typeAhead: true
            
    });
   	
      var fpw=Ext.create('Ext.FormPanel', {         
			        frame: true,
			        fieldDefaults: {
			           labelWidth: 65,                        
			           xtype: 'textfield' 
			        },
              bodyPadding: 10,
                  items: [
                   {xtype: 'hidden',name: 'corpid',value:''},
                   {xtype: 'hidden',name: 'flag',value:''},
                   facCombo,                   
                 {
                        fieldLabel:'主题',
                        xtype: 'textfield',
                        name:'message_title'
                       
                 },{
                        fieldLabel:'内容',
                        xtype: 'textarea',
                        name:'message_content'
                       
                    }       
                    ,{
                        xtype: 'button',
                        anchor: 'right',
                         style : 'margin-left:80px',
                        text:'发&nbsp;&nbsp;送',
               handler: function(){
       
  //   if(fpw.getForm().getValues().mfrom==undefined){
   //  	  alert('请选择消息接收对象');
   //  	  return;
  //   }
     
               fpw.form.doAction('submit',{
               url:'/pronline/Msg',
                method:'POST',
               params:{FunName:'proauth_mesgsave_v8'},                
                success:function(form,action){ 
                	alert(action.result.msg);        					
         					 winw.close();
         					 reflash();
                 },
             		 failure:function(form,action){        		 
                     alert(action.result.msg);
                 
            		 }
                         
                         })
                  }
                       
                    },
							         {
							            text: '关&nbsp;&nbsp;闭',
							            style : 'margin-left:10px',
							            xtype: 'button',
							            handler: function(){
							                winw.close();
							            }
							         }
							         
							         ]
                  });
                  
        var winw = Ext.widget('window', {
                title: '我要发消息',
                closeAction: 'hide',
                x:400,
                y:32,
                width: 280,
                height: 280,
                minHeight: 280,
        //        layout: 'fit',
                resizable: true,
                modal: false,
                items: fpw
            });
             winw.show();       
                  
   
  },
  dwgldel: function(thisBtn){
  	var grid = Ext.ComponentQuery.query('mobileaccountlist')[0];
    var rows = grid.getSelectionModel().getSelection();

     if(rows){
     	           var selsid;
     	            selsid='';
     	            Ext.MessageBox.confirm('确认', '确定删除该记录？', function(btn){
     	            	if(btn=='yes'){
     	             var store = grid.getStore();
                  for(var i=0;i<rows.length;i++){
                  	if(i==0){
                  	 selsid=rows[i].get('sid');
                  	}
                  	else{
                       selsid=selsid+","+rows[i].get('sid');
                    }
                   
                   }
 
                   store.load({params:{del:"del",selsid:selsid}});
                  
                 }
     	        })
     }
    
    
  //  proauthMobileAccount.controller.Procy.loadProcyListStore();
  },
  
  
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
      var store = Ext.ComponentQuery.query('mobileaccountlist')[0].getStore();
    
     var keyword=Ext.getCmp('keyword_mobileaccount').value;
   
      store.load();
    },
    SetPage:function(curpage){
      var store = Ext.ComponentQuery.query('mobileaccountlist')[0].getStore();
      store.currentPage=curpage;
       
    },
    showedit:function(userid){
       var htmlvar='<iframe src="/pronline/Msg?FunName@ncsWebDispClientAddForm_v4&groupid@'+parent.ncsgroupid+'&userid@'+userid+' " scrolling="yes" name="moduleIframe" frameborder="no" style="width:100%;height:600px;"></iframe>';      
       var win = new Ext.Window ({
                             id:"addwindow",
                             title:"增加或修改单位信息",
                             x:0,
                             y:0,
                             width:670,
                             height:700,
                             bodyPadding: 10,
                             modal:true,
  //                         headerPosition: 'bottom',
                             resizable:true,
                             draggable:true,
                             html:htmlvar
                                                
         }
        )
        win.show();
       
    },
    setTitle: function(title){
       document.getElementById("titledx").innerHTML='&nbsp'+title+'&nbsp;&nbsp;';   

    }
  }
 
 
 
});
