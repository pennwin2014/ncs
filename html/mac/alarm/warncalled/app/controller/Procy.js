

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
	
	Ext.define('State_deal', {
        extend: 'Ext.data.Model',
        fields: ['flags','stime','casename']
    });
   
  var store_deal= Ext.create('Ext.data.Store', {
        model: 'State_deal',
        remoteSort: true,
         proxy: {
            type: 'ajax',
            url: '/pronline/Msg?FunName@ncsSys_stafinfo_deal',
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
			//导出
			'apsetlist button[action=export]':{
				click: this.dwglexport
			},
			 'apsetlist button[action=sh]':{
				click: this.dwglsh
			}
			,
			//编辑单位
		  'apsetlist button[action=edit]':{
				click: this.dwgledit
			}
			,
			//高级查询
		  'apsetlist button[action=advancecx]':{
				click: this.dwglavcx
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
  dwglexport: function(thisBtn){
	   var form1 = document.getElementById('form1');
		document.getElementById('form1').export2.value='exp';
		document.getElementById('form1').limit.value='40';
		document.getElementById('form1').page.value=Ext.getCmp("pagtool").store.currentPage;
		 document.getElementById('form1').keyword.value=Ext.getCmp('keyword_dwgl').value;  
        document.getElementById("form1").submit();
        document.getElementById("form1").export2.value="";
  },
  
  dwgldel: function(thisBtn){
	  var store = Ext.ComponentQuery.query('apsetlist')[0].getStore();
  	var grid = Ext.ComponentQuery.query('apsetlist')[0];
    var rows = grid.getSelectionModel().getSelection();
  
     if(rows){
     	           var selsid;
     	            selsid='';
					
     	            Ext.MessageBox.confirm('确认', '确定删除该记录？', function(btn){
     	            	if(btn=='yes'){
     	    
                  for(var i=0;i<rows.length;i++){
                  	if(i==0){
                  		  selsid="'" + rows[i].get('sid') + "'";
                  	}
                  	else{
                       selsid=selsid+","+"'"+rows[i].get('sid')+"'";
					  
                    }
                   }
				  
                   store.load({params:{del:"del",sid:selsid}});
                 }
     	        })
		}
    
    
  //  proauthApset.controller.Procy.loadProcyListStore();
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
		
		var win =thisBtn.up('window');
		
		var store = Ext.ComponentQuery.query('apsetlist')[0].getStore();
   	var tform=win.query('form')[0];   	

	 var sid= tform.getForm().getValues().sid;
	 var name= tform.getForm().getValues().name;
    var department=tform.getForm().getValues().department;
    var phone=tform.getForm().getValues().phone;
    var email=tform.getForm().getValues().email;
	
	if(name==''||name=='输入姓名'){
    	alert('请填写姓名');
    	return;
    }
    if(department==''||department=='输入部门'){
    	alert('请输入部门');
    	return;
    }
     if(phone==''||phone=='输入电话'){
    	alert('请填写联系人电话');
    	return;
    }
    if(email==null||email=='输入邮箱'){
    	alert('请填写邮箱');
    	return;
    }
	
	
	store.load({params:{add:'add',name:name,department:department,phone:phone,email:email,sid:sid}});
	win.close();
	
    	
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
      title: "接 警 人 员 管 理",
      field1: record,
      field2:adsid
    });
    win.showAt(50,20);

    },
	show_deal:function(record,adsid){
	var name=record.get("name");
	var sid=record.get("sid");
    var win = Ext.create('proauthApset.view.common.DealWin',{
      title: "告 警 处 理",
      field1: record,
      field2:sid
    });
    win.showAt(100,20);
    },
    setTitle: function(title){
       document.getElementById("titledx").innerHTML='&nbsp'+title+'&nbsp;&nbsp;';   

    }
  }
});
