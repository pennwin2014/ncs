
function currentRead(){
	var store = Ext.ComponentQuery.query('authloglist')[0].getStore();  
	
	var groupid="";
  var username="";
	var bRet = false;	
	
	//根据 g_dispname 是否为空来判断，用户是否点击左侧场所树		
	if(g_dispname != null){
		
		//groupid与username都为空时，默认查询所有场所
		try{			
			groupid = parent.getGlobalGroupId();	
			if(groupid != '')
				bRet = true;
			//alert(g_groupidquery);		
		}
		catch(e){
			alert(e.message);
		}
				
		try{						
			username = parent.getGlobalUserName();
			if(username != '')
				bRet = true;			
		}
		catch(e){
			alert(e.message);
		}
		
		if(bRet == false){
			groupid = "all";
		}	
	}
	/* 输入框关键字 */	
	var keyword = Ext.getCmp('keyword_mobileaccount').value;
					 	
  store.load({params:{keyword:keyword, groupid:groupid, username:username}});
}

Ext.define('proauthRzAuthlog.controller.Procy', {
    extend: 'Ext.app.Controller',  
    stores: ['List'],
    models: ['List'],  
    views: ['list.List'],
    servicecode:'',
    servicename:'',
    mac:'',
    apname:'',
    apmac:'',
    ssid:'',
    sdate:'',
    stime:'',
    edate:'',
    etime:'',
    searchtype:'',
  
    init: function() {
        this.control({
          'authloglist':{
 				 	//select: this.showTab,
            render: this.showRender
          },
          //关键字查询
          'myselfPanel button[action=keycx]':{
                click: this.dwglkeycx
          },
		  
		  		/* 增加按钮：2015/7/26 10:29  钱智明 */
		  		//人流分析
		  		'authloglist button[action=electricmap]':{
                click: this.placeelectricmap
          },
		  
		  		//查看图表
		  		'authloglist button[action=checkgraph]':{
                click: this.placecheckgraph
          },
          
          //导出
		  		'authloglist button[action=export]':{
        				click: this.infoexport
        	}
		  
        });
    },
    
    showRender: function(){
    	
    	var store = Ext.ComponentQuery.query('authloglist')[0].getStore();
    	 
    	/* 输入框关键字 */	
			//var keyword = Ext.getCmp('keyword_mobileaccount').value;
					 	
  		//store.load({params:{keyword:keyword}});
  		currentRead();
   	    	    	   	    	
			setInterval("currentRead();", 10000);
			     
    },

    //关键字查询
    dwglkeycx: function(thisBtn){
    	   			
			/* 通过有无 @ 字条判断输入框内容类型：输入关键字 或是左侧场所菜单选择 */		
			/*var tempvalue = Ext.getCmp("keyword_mobileaccount").value;	
			var tempvalue2 = tempvalue.substr(0, 1);
			var tempvalue3 = tempvalue.substr(tempvalue.length - 1, 1);
				
			if(tempvalue2 == '@' && tempvalue3 != '@'){
				Ext.getCmp("keyword_mobileaccount").setValue(tempvalue);	
				alert(Ext.getCmp("keyword_mobileaccount").value);		
			}	*/
						
        proauthRzAuthlog.controller.Procy.searchtype = ""; 
        proauthRzAuthlog.controller.Procy.SetPage(1);  
        currentRead();
    },
  		
	//人流分析
	placeelectricmap: function(thisBtn){
		var tabPanelelectricmap = new Ext.Panel({    
			frame: true,
    	layout: 'form',
    	activeTab: 0,
      items: [{
      	//title: '电子地图',
      	//html: '<iframe src="/mac/datamining/track/track.htm" frameborder="no" style="width:100%;height:1000px;"></iframe>'
      	html: '<iframe src="/mac/infoquery/currentinfo/crowd/crowd.htm" frameborder="no" style="width:100%;height:640px;"></iframe>'
      }]
    });
		
		var win4 = Ext.widget('window', {
    	title: '人流分析',
				//closeAction: 'hide',
      	x:50,
      	y:80,
      	width: 750,
				height: 550,
      	resizable: true,
      	draggable:true,
        modal:true,
        items: tabPanelelectricmap
    });
       
    win4.show();		
	},
	
	//查看图表
	placecheckgraph: function(thisBtn){
		var tabPanelchecktable = new Ext.Panel({    
			frame: true,
    	layout: 'form',
    	activeTab: 0,
    	items: [{             
    		html: '<iframe src="/mac/infoquery/currentinfo/checkgraph/allplacecollect.html" frameborder="no" style="width:100%;height:1000px;"></iframe>'
    	}]
  	});		
		var win5 = Ext.widget('window', {
  		title: '查看图表',
			//closeAction: 'hide',
    	x:50,
    	y:80,
    	width: 850,
			height: 500,
    	resizable: true,
    	draggable:true,
    	modal:true,
    	items: tabPanelchecktable
  	});       
  	
  	win5.show();		
	},
	
	infoexport: function(thisBtn){
		
		var form1 = document.getElementById('form1');	
		form1.currentexport.value = 'export';		
		form1.submit();
		
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

