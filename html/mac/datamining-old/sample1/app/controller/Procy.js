Ext.define('proauthRzAuthlog.controller.Procy', {
    extend: 'Ext.app.Controller',  
    stores: ['List'],
    models: ['List'],  
    views: ['list.List'],

  
    init: function() {
        this.control({
          'authloglist':{
  //         select: this.showTab,
            render: this.showRender
          },
          //关键字查询
          'authloglist button[action=keycx]':{
                click: this.dwglkeycx
          },
          //添加
          'authloglist button[action=advancecx]':{
                click: this.cxyhadd
          },
          			//显示上传图片界面
		     'addwin button[action=upfile]':{
				  click: this.showupload
			    },
			              //添加保存
          'authloglist button[action=add]':{
                click: this.add
          },
          'authloglist button[action=edit]':{
            click: this.edit
          },
          'authloglist button[action=del]':{
            click: this.cxyhdel
          },
          'authloglist button[action=hidShw]':{
            click: this.cxyhhidshw
          }
        });
    },
    
    showRender: function(){
        var store = Ext.ComponentQuery.query('authloglist')[0].getStore();
        store.on('beforeload', function (store, options) {
//调整视图高度
        var qgrid=Ext.ComponentQuery.query('authloglist')[0];
    	  parent.grid_height=parent.Ext.getCmp('layout_center').getHeight()-56;
        qgrid.setHeight(parseInt(parent.grid_height));
        	
            var keyword = Ext.getCmp('keyword_mobileaccount').value;
           
           
         
       
            var new_params={keyword:keyword};
        
             Ext.apply(store.proxy.extraParams,new_params);

        });//store.on('beforeload', function (store, options) {

        proauthRzAuthlog.controller.Procy.loadProcyListStore();
    },// showRender: function(){
   showupload: function(thisBtn){
   var sid=thisBtn.up('form').getForm().findField('sid').getValue();
  
   if(sid==''){
		alert("请先保存记录,再上传");
		return 0;	
		}
  
	   	var win = Ext.create('proauthRzAuthlog.view.common.UploadWin',{
	     title: "上传图片",
	    field1: sid,
	    field2:''
	    });
	    win.showAt(340,100); 
  
  },
    //关键字查询
    dwglkeycx: function(thisBtn){
    	 
        proauthRzAuthlog.controller.Procy.searchtype = ""; 
        proauthRzAuthlog.controller.Procy.SetPage(1);  
        proauthRzAuthlog.controller.Procy.loadProcyListStore();
    },
  
   edit:function(thisBtn){
         var win = Ext.create('proauthRzAuthlog.view.common.AddWin',{
        title: "新建",
        field1: ""

      });
      win.showAt(300,20);      
      },
  add:function(thisBtn){
         var win = Ext.create('proauthRzAuthlog.view.common.AddWin',{
        title: "新建",
        field1: ""

      });
      win.showAt(300,20);      
      },
    /**************************************
    * 全局函数 
    ***************************************/
    inheritableStatics:{
        loadProcyListStore:function(){
            var store = Ext.ComponentQuery.query('authloglist')[0].getStore();
            var keyword = Ext.getCmp('keyword_mobileaccount').value;
         
            store.load();
        },
        SetPage:function(curpage){
            var store = Ext.ComponentQuery.query('authloglist')[0].getStore();
            store.currentPage = curpage;
        },
        setTitle: function(title){
             document.getElementById("titledx").innerHTML = 
                 '&nbsp'+ title +'&nbsp;&nbsp;';   
        }
    }
   
});

