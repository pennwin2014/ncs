Ext.define('proauthMobileAccount.controller.Procy', {
  extend: 'Ext.app.Controller',  
  stores: ['List','WarnLogsStore'],
  models: ['List','warnLogModel'],  
  views: [ 'list.List','list.warnProcyTree','common.WarnlogWin'],
  init: function() {

		this.control({
			
		  'mobileaccountlist':{
		    //select: this.showTab,
		    render: this.showRender
		  },	  
		  //告警日志设为已处理
		  'mobileaccountlist button[action=setstatus]':{
			click: this.warnlogsetstatus
		  },
		  //关键字查询
		  'mobileaccountlist button[action=keycx_warnlog]':{
				click: this.dwglkeycx_warnlog
		  }
		});
  },
  
  showRender: function(){
  var keyword=Ext.getCmp('keyword_warnlog').value;
  var store = Ext.ComponentQuery.query('mobileaccountlist')[0].getStore();

  store.on('beforeload', function (store, options) {
  	

	  var new_params={cid:store.field1,keyword:store.field2,status:store.field3};
      Ext.apply(store.proxy.extraParams,new_params);
       });	
     store.load();

  },
  //关键字查询
  dwglkeycx_warnlog: function(thisBtn){
      var store = Ext.ComponentQuery.query('mobileaccountlist')[0].getStore();
      store.field2=Ext.getCmp('keyword_warnlog').value;
      //store.field1='';
	  store.field3='';
	  proauthMobileAccount.controller.Procy.SetPage(1);
	  //var new_params={page:1,start:0,limit:40};
      //Ext.apply(store.proxy.extraParams,new_params);
      store.load();
	
	  //proauthMobileAccount.controller.Procy.loadProcyListStore();
  },
    warnlogsetstatus: function(thisBtn){
  	var grid = Ext.ComponentQuery.query('mobileaccountlist')[0];
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
    loadProcyListStore:function(cid){
      var store = Ext.ComponentQuery.query('mobileaccountlist')[0].getStore();
    
     var keyword=Ext.getCmp('keyword_warnlog').value;
     store.field1=cid;
	
      store.load();
    },
    SetPage:function(curpage){
      var store = Ext.ComponentQuery.query('mobileaccountlist')[0].getStore();
      store.currentPage=curpage;
    },

    setTitle: function(title){
       document.getElementById("titledx").innerHTML='&nbsp'+title+'&nbsp;&nbsp;';   

    },
   
  }
 
 
 
});
