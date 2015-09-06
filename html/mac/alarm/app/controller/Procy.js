Ext.define('proauthMobileAccount.controller.Procy', {
  extend: 'Ext.app.Controller',  
  stores: ['List','MacStore','WarnStore','MacLimStore','WarnLogStore'],
 	models: ['List','warnLogModel'],  
  views: [ 'list.List','common.AddWin','common.macKeywordBk','common.macWarnNotfy','common.macLimBk','common.macWarnlogList'],
  init: function() {

		this.control({
		
		  'mobileaccountlist':{
		    //select: this.showTab,
		    render: this.showRender
		  },
		  
		  //关键字查询
		  'mobileaccountlist button[action=keycx]':{
				click: this.dwglkeycx
		  },
		  'macwarnloglist button[action=keycx_warnlog]':{
				click: this.dwglkeycx_warnlog
		  }, //删除布控
		  'mobileaccountlist button[action=del]':{
				click: this.dwgldel
		  }
		   ,//策略明细删除
		  'mackeylist button[action=del]':{
				click: this.mackeydel
		  },//告警人删除
		  'macwarnnotfylist button[action=del]':{
				click: this.macwarnnotfydel
		  },//告警阀值删除
		  'maclimlist button[action=del]':{
				click: this.maclimdel
		  },//告警日志设为已处理
		  'macwarnloglist button[action=setstatus]':{
				click: this.warnlogsetstatus
		  }
			
		});
  }
 
  ,
  
  showRender: function(){
  
  var store = Ext.ComponentQuery.query('mobileaccountlist')[0].getStore();
  
  store.on('beforeload', function (store, options) {
	  var keyword=Ext.getCmp('keyword_mobileaccount').value;
	  var groupid=parent.ncsgroupid;
	  var did=parent.servicecode;
	    if(did){
       groupid = "";
     }
    var fcode=parent.fcode;
	
    var servicecode=parent.servicecode;
	  var new_params={groupid:groupid,did:did,keyword:keyword,fcode:fcode,servicecode:servicecode};
     Ext.apply(store.proxy.extraParams,new_params);
       });	
   proauthMobileAccount.controller.Procy.loadProcyListStore();
 //   proauthMobileAccount.controller.Procy.setTitle(parent.dxtitle);
  },
  
  //关键字查询
  dwglkeycx: function(thisBtn){

    proauthMobileAccount.controller.Procy.loadProcyListStore();
  },
  dwglkeycx_warnlog: function(thisBtn){
	var store = Ext.ComponentQuery.query('macwarnloglist')[0].getStore();
      store.field2=Ext.getCmp('keyword_warnlog').value;
	  store.field3='';
	  store.load({params:{keyword:store.field2}});
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
    mackeydel: function(thisBtn){
  	var grid = Ext.ComponentQuery.query('mackeylist')[0];
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
  macwarnnotfydel: function(thisBtn){
  	var grid = Ext.ComponentQuery.query('macwarnnotfylist')[0];
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
   maclimdel: function(thisBtn){
  	var grid = Ext.ComponentQuery.query('maclimlist')[0];
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
  
    warnlogsetstatus: function(thisBtn){
  	var grid = Ext.ComponentQuery.query('macwarnloglist')[0];
    var rows = grid.getSelectionModel().getSelection();

     if(rows){
     	           var selsid;
     	            selsid='';
     	            Ext.MessageBox.confirm('确认', '确定将选择的记录设成已处理？', function(btn){
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
     
                   store.load({params:{del:"setstus",selsid:selsid}});
                  
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

    setTitle: function(title){
       document.getElementById("titledx").innerHTML='&nbsp'+title+'&nbsp;&nbsp;';   

    },
    loadWarnlogListStore:function(){

      var store = Ext.ComponentQuery.query('macwarnloglist')[0].getStore();
	  var keyword=Ext.getCmp('keyword_warnlog').value;
      //document.getElementById("form1").keyword_warnlog.value=Ext.getCmp('keyword_warnlog').value;
      //document.getElementById("form1").clstate.value=Ext.getCmp('clstate').value;
 
      
     // store.currentPage=1;   
      store.load();  
    }
  }
 
 
 
});
