Ext.define('proauthMac.controller.Procy', {
  extend: 'Ext.app.Controller',  
  stores: ['List'],
 	models: ['List'],  
  views: [ 'list.List'],
  init: function() {

		this.control({
		
		  'maclist':{
	//	    select: this.showTab,
		    render: this.showRender
		  },
		  
		  //关键字查询
		  'maclist button[action=keycx]':{
				click: this.dwglkeycx
			}
		});
  }
 
  ,
  
  showRender: function(){
  
  var store = Ext.ComponentQuery.query('maclist')[0].getStore();
  
  store.on('beforeload', function (store, options) {
	  var keyword=Ext.getCmp('keyword_mobileaccount').value;
	  var time_flag=Ext.getCmp('time_flag').value;
	  var groupid=parent.ncsgroupid;
	  var did=parent.corpdid;
	    if(did){
       groupid = "";
     }
	  var new_params={groupid:groupid,did:did,keyword:keyword,time_flag:time_flag};
     Ext.apply(store.proxy.extraParams,new_params);
       });	

   proauthMac.controller.Procy.loadProcyListStore();
 //   proauthMac.controller.Procy.setTitle(parent.dxtitle);
  },
  //关键字查询
  dwglkeycx: function(thisBtn){
    proauthMac.controller.Procy.loadProcyListStore();
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
      var store = Ext.ComponentQuery.query('maclist')[0].getStore();
    
     var keyword=Ext.getCmp('keyword_mobileaccount').value;
   
      store.load();
    },
    SetPage:function(curpage){
      var store = Ext.ComponentQuery.query('maclist')[0].getStore();
      store.currentPage=curpage;
       
    },
    setTitle: function(title){
       document.getElementById("titledx").innerHTML='&nbsp'+title+'&nbsp;&nbsp;';   

    }
  }
 
 
 
});
