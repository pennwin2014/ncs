  Ext.define('State', {
        extend: 'Ext.data.Model',
        fields: [
            {type: 'string', name: 'groupid'},
            {type: 'string', name: 'groupname'}
        ]
    });
   
  var store_fac = Ext.create('Ext.data.Store', {
        model: 'State',
        remoteSort: true,
         proxy: {
            type: 'ajax',
            url: '/pronline/Msg?FunName@ncsWebFacComp',
            reader: {
                type:'json',
                root: 'corpclassdata'              
            },         
            simpleSortMode: true
        }
    });
    
Ext.define('proauthApset.controller.Procy', {
  extend: 'Ext.app.Controller',  
  stores: ['List'],
	models: ['List'],  
  views: ['list.List','common.AddWin'],
  init: function() {
 
		this.control({
		
		  'apsetlist':{
	//	    select: this.showTab,
		    render: this.showRender
		  },
		  
		  //关键字查询
		  'apsetlist button[action=keycx]':{
				click: this.dwglkeycx
			},
			//添加AP
		  'apsetlist button[action=add]':{
				click: this.dwgladd
			},
			//保存
		  'addwin button[action=addsave]':{
				click: this.addsave
			},
		  			//删除单位
		  'apsetlist button[action=del]':{
				click: this.dwgldel
			},
			 'apsetlist button[action=sh]':{
				click: this.dwglsh
			}
			,'apsetlist button[action=dwsz]':{
				click: this.dwgldwsz
			},
			//编辑单位
		  'apsetlist button[action=edit]':{
				click: this.dwgledit
			}
			,
			//高级查询
		  'apsetlist button[action=advancecx]':{
				click: this.dwglavcx
			}
			, 'apsetlist button[action=import]':{
				click: this.openDr
			}
			
		});
  },
  
  showRender: function(){
  var store = Ext.ComponentQuery.query('apsetlist')[0].getStore();
  store.on('beforeload', function (store, options) {
  	//调整视图高度
        var qgrid=Ext.ComponentQuery.query('apsetlist')[0];
    	  parent.grid_height=parent.Ext.getCmp('layout_center').getHeight()-56;
        qgrid.setHeight(parent.grid_height);
	  var keyword=Ext.getCmp('keyword_dwgl').value;
	  var groupid=parent.ncsgroupid;
	  var did=parent.corpdid;
	
	    if(did){
       groupid = "";
     }
//    var mark=parent.gmark;

     store_fac.load();

	  var new_params={groupid:groupid,did:did,keyword:keyword};
     Ext.apply(store.proxy.extraParams,new_params);
       });	
   
   proauthApset.controller.Procy.loadProcyListStore();
 //   proauthApset.controller.Procy.setTitle(parent.dxtitle);
  },
  //关键字查询
  dwglkeycx: function(thisBtn){
    proauthApset.controller.Procy.loadProcyListStore();
  },
  dwgldel: function(thisBtn){
  	var grid = Ext.ComponentQuery.query('apsetlist')[0];
    var rows = grid.getSelectionModel().getSelection();

     if(rows){
     	           var selsid;
     	            selsid='';
     	            Ext.MessageBox.confirm('确认', '确定删除该记录？', function(btn){
     	            	if(btn=='yes'){
     	             var store = grid.getStore();
                  for(var i=0;i<rows.length;i++){
                  	if(i==0){
                  		selsid="\'";
                  	  selsid=selsid+rows[i].get('gwid');
                  	  selsid=selsid+"\'";
                  	}
                  	else{
                       selsid=selsid+","+"\'"+rows[i].get('gwid')+"\'";
                    }
                   
                   }

                   store.load({params:{del:"del",selsid:selsid}});
                  
                 }
     	        })
     }
    
    
  //  proauthApset.controller.Procy.loadProcyListStore();
  },
  
  dwgldwsz:function(thisBtn){
		var tabPaneledit = new Ext.Panel({
			frame: true,
            layout: 'form',
            activeTab: 0,
			items:[{
				html: '<iframe src="/v4/ncs_group_list.htm" frameborder="no" style="width:100%;height:1000px;"></iframe>'
			}]			
		});
		
		var win7 = Ext.widget('window', {
            title: '身份查看',
            x:150,
            y:60,
            width: 900,
			height: 600,
			id: 'idinfocheck',
            resizable: true,
            draggable:true,
            modal:false,
            items: tabPaneledit
        });
       
        win7.show();
	},

    
  dwglsh: function(thisBtn){
  	var grid = Ext.ComponentQuery.query('apsetlist')[0];
    var rows = grid.getSelectionModel().getSelection();

     if(rows){
     	           var selsid;
     	            selsid='';
     	            Ext.MessageBox.confirm('确认', '确定审核该记录？', function(btn){
     	            	if(btn=='yes'){
     	             var store = grid.getStore();
                  for(var i=0;i<rows.length;i++){
                  	if(i==0){
                  		selsid="\'";
                  	  selsid=selsid+rows[i].get('gwid');
                  	  selsid=selsid+"\'";
                  	}
                  	else{
                       selsid=selsid+","+"\'"+rows[i].get('gwid')+"\'";
                    }
                   
                   }

                   store.load({params:{sh:"sh",selsid:selsid}});
                  
                 }
     	        })
     }
    
    
  //  proauthApset.controller.Procy.loadProcyListStore();
  },
  dwgladd:function(thisBtn){
     
  	 proauthApset.controller.Procy.show_add('','');
    
  },
  addsave:function(thisBtn){
   	var tform=thisBtn.up('window').query('form')[0];   	
    var toldApname= tform.getForm().getValues().oldApname;
    var tgwid= tform.getForm().getValues().gwid;
    var tgroupid=tform.getForm().getValues().groupid;
    var tmac=tform.getForm().getValues().mac;
    var tssid=tform.getForm().getValues().ssid;
	//alert(toldApname);
    if(tgwid==''||tgwid=='AP编号或AP标识'){
    	alert('请填写AP名称');
    	return;
    }
    if(tgroupid==null||tgroupid==''){
    	alert('请先选取单位');
    	return;
    }
     if(tmac=='可以多个，之间用“,”分割'||tmac==''){
    	alert('请填写AP mac');
    	return;
    }
    if(tssid==null||tssid==''){
    	alert('请填写SSID');
    	return;
    }
    tform.getForm().doAction('submit',{
                url:'/pronline/Msg',
                 method:'post',
                 waitMsg: '正在保存...',
                  params:{FunName:'proauth_apset_Save_v9'},
                  success:function(form,action){

    //      tform.getForm().findField('sid').setValue(action.result.sid);  
    //      var store = Ext.ComponentQuery.query('resourcelist')[0].getStore();
         
         proauthApset.controller.Procy.loadProcyListStore();  
         thisBtn.up('window').close();
                  }
                  ,
                 failure:function(form,action){
                 	Ext.Msg.alert(action.result.msg);

                   }
                     
                 })
    	
  },
openDr:function()
{
var windr;
     var htmlvar='<iframe src="/pronline/Msg?FunName@ncsWebReplace&title_name@热点信息导入&funname@ncS_APsbDr_Next&plate@nc/cust_dr_first.htm'+' " scrolling="no" name="drIframe" frameborder="no" style="width:100%;height:100%;"></iframe>';      
      var windr = new Ext.Window ({
                             id:"windr",
                             title:"设备批量导入",
                             x:0,
                             y:0,
                             width:500,
                             height:500,
                             bodyPadding: 0,
                             modal:true,
//                         headerPosition: 'bottom',
                             resizable:true,
                             closable : true,
                             draggable:true,
                             html:htmlvar
                                                
         }
        )       

       windr.on("beforeclose", function() {
        	reflash();
        }
        )
         windr.show();
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
      var store = Ext.ComponentQuery.query('apsetlist')[0].getStore();
  //    var keyword=Ext.getCmp('keyword_dwgl').value;
      store.load();
    },
    SetPage:function(curpage){
      var store = Ext.ComponentQuery.query('apsetlist')[0].getStore();
      store.currentPage=curpage;
       
    },
    show_add:function(record,adsid){

    	var win = Ext.create('proauthApset.view.common.AddWin',{
      title: "编 辑 AP 信 息",
      field1: record,
      field2:adsid
    });
    win.showAt(50,20);

    },
    setTitle: function(title){
       document.getElementById("titledx").innerHTML='&nbsp'+title+'&nbsp;&nbsp;';   

    }
  }
});
