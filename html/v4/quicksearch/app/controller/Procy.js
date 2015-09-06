Ext.define('quickResarch.controller.Procy', {
  extend: 'Ext.app.Controller',  
  views: [ 'list.List','list.left','list.netidList','list.httplogList','list.searchlogList','list.bbslogList','list.maillogList'],
  init: function() {
 
		this.control({
		
		  'sxxlog':{
	//	    select: this.showTab,
		    render: this.showRender
		  }
		  
			
		});
  },
  
  showRender: function(){

  // proauthDwgl.controller.Procy.loadProcyListStore();
 //   proauthDwgl.controller.Procy.setTitle(parent.dxtitle);
  },
  /**************************************
  * 全局函数 
  ***************************************/
  inheritableStatics:{
    loadProcyListStore:function(form1){
    var time_flag=form1.getForm().getValues().time_flag;
    var sdate=form1.getForm().getValues().sdate;
    var edate=form1.getForm().getValues().edate;
    var stime=form1.getForm().getValues().stime;
    var etime=form1.getForm().getValues().etime;
    
    var username=form1.getForm().getValues().username;
    var dispname=form1.getForm().getValues().dispname;
    var idno=form1.getForm().getValues().idno;
    var mac=form1.getForm().getValues().mac;
    
    var netid=form1.getForm().getValues().netid;
    var nettype=form1.getForm().getValues().nettype;
    
    var keyword=form1.getForm().getValues().keyword;
    var httpurl=form1.getForm().getValues().url;
    var flag_sxx=0;
    var flag_xnsf=0;
    var flag_http=0;
    var flag_content=0;
    var where='';
    var did='';
    var  uid='';
    var i;
    if(username!=''||dispname!=''||idno!=''||mac!=''){
    	flag_sxx=1;
    	store_sxxlog.new_params={time_flag:time_flag,sdate:sdate,edate:edate,stime:etime,username:username,dispname:dispname,idno:idno,mac:mac};
      store_sxxlog.load(
       {
	    	  callback:function(records, options, success){
	   
             if(store_sxxlog.getCount()==0){ 
              Ext.getCmp('view_sxxlog').setVisible(false); 
             } 
             else{
             	 Ext.getCmp('view_sxxlog').setVisible(true); 
             }  
	    	  }
	    	}
      );
      
    }
 
    if(username!=''||dispname!=''||idno!=''||mac!=''||netid!=''){
    	  flag_xnsf=1;    	
    	  store_netidlog.new_params={time_flag:time_flag,sdate:sdate,edate:edate,stime:etime,username:username,dispname:dispname,idno:idno,mac:mac,netid:netid,nettype:nettype};
	      store_netidlog.load(
	       {
		    	  callback:function(records, options, success){
	//	          alert(store_sxxlog.getCount()); 
	             if(store_netidlog.getCount()==0){ 
	              Ext.getCmp('view_netidlog').setVisible(false); 	              
	             } 
	             else{
	             	 Ext.getCmp('view_netidlog').setVisible(true); 
	             	 
	             	 	   //根据返回的用户信息查询上下线日志    
      
	              if(flag_sxx==0){
                 	   var num=0;
	              	 	                                        //根据返回的结果查询
	              	 	 for(i=0;i<store_netidlog.getCount();i++){
	              	 	 	         did=records[i].get('did');
	              	 	 	         uid=records[i].get('uid');
	              	 	 	         temp=' did='+did+' and uid='+uid;
	              	 	 	         if(where.indexOf(temp)==-1){
	              	 	 	         	num++;
	              	 	 	         	if(num==1){
	              	 	 	         		where='('+temp+')';
	              	 	 	         	}
	              	 	 	         	else{
	              	 	 	         	 where=where+' or '+'('+temp+')';
	              	 	 	          }
	                            }
	              	 	 	 
	              	 	 }
	
	              	 	 if(where.length>10){
	              	 	 	    store_sxxlog.new_params={time_flag:time_flag,sdate:sdate,edate:edate,stime:etime,username:username,dispname:dispname,idno:idno,mac:mac,where:where};
										      store_sxxlog.load(
										       {
											    	  callback:function(records, options, success){
											   
										             if(store_sxxlog.getCount()==0){ 
										              Ext.getCmp('view_sxxlog').setVisible(false); 
										             } 
										             else{
										             	 Ext.getCmp('view_sxxlog').setVisible(true); 
										             }  
											    	  }
											    	}
										      );              	 	 	
	              	 	 }
	  //查询HTTPLOG            	 	 
	              	 	 if(flag_http==0){
	              	 	 	    store_httplog.new_params={time_flag:time_flag,sdate:sdate,edate:edate,stime:etime,username:username,dispname:dispname,idno:idno,mac:mac,host:httpurl,where:where};
										      store_httplog.load(
										       {
											    	  callback:function(records, options, success){
										             if(store_httplog.getCount()==0){ 
										              Ext.getCmp('view_httplog').setVisible(false); 									 										              
									             } 
										             else{
										             	 Ext.getCmp('view_httplog').setVisible(true); 
										             }  
											    	  }
											    	}
										      );
	              	 	 	
	              	 	 }
	              	 	
	         	         //查询searchlog            	 	 
	              	 	 if(flag_content==0){
	              	 	 	     store_searchlog.new_params={time_flag:time_flag,sdate:sdate,edate:edate,stime:etime,username:username,dispname:dispname,idno:idno,mac:mac,keyword:keyword,where:where};
										      store_searchlog.load(
										       {
											    	  callback:function(records, options, success){
										             if(store_searchlog.getCount()==0){ 
										              Ext.getCmp('view_searchlog').setVisible(false); 
										             } 
										             else{
										             	 Ext.getCmp('view_searchlog').setVisible(true); 
										             } 
											    	  }
											    	}
										      );
	              	 	 	
	              	 	 }    
	             
	             	     //查询bbslog            	 	 
	              	 	 if(flag_content==0){
	              	 	 	     store_bbslog.new_params={time_flag:time_flag,sdate:sdate,edate:edate,stime:etime,username:username,dispname:dispname,idno:idno,mac:mac,keyword:keyword,where:where};
										      store_searchlog.load(
										       {
											    	  callback:function(records, options, success){
										             if(store_bbslog.getCount()==0){ 
										              Ext.getCmp('view_bbslog').setVisible(false); 
										             } 
										             else{
										             	 Ext.getCmp('view_bbslog').setVisible(true); 
										             } 
											    	  }
											    	}
										      );
	              	 	 	
	              	 	 } 
	             
	             	      //查询邮件           	 	 
	              	 	 if(flag_content==0){
	              	 	 	    store_maillog.new_params={time_flag:time_flag,sdate:sdate,edate:edate,stime:etime,username:username,dispname:dispname,idno:idno,mac:mac,keyword:keyword,where:where};
										      store_maillog.load(
										       {
											    	  callback:function(records, options, success){
										             if(store_maillog.getCount()==0){ 
										              Ext.getCmp('view_maillog').setVisible(false); 
										             } 
										             else{
										             	 Ext.getCmp('view_maillog').setVisible(true); 
										             } 
											    	  }
											    	}
										      );
	              	 	 	
	              	 	 }
	             
	             
	             
	              	 	
	              	 	
	              
	              }
	              
	             	 
	             	 
	             	 
	             	 
	             	 
	             }  
		    	  }
		    	}
	      );
    	
    	
    	
    }
    if(username!=''||dispname!=''||idno!=''||mac!=''||httpurl!=''){
    	  flag_http=1;    	
    	  store_httplog.new_params={time_flag:time_flag,sdate:sdate,edate:edate,stime:etime,username:username,dispname:dispname,idno:idno,mac:mac,host:httpurl};
	      store_httplog.load(
	       {
		    	  callback:function(records, options, success){
	//	          alert(store_sxxlog.getCount()); 
	             if(store_httplog.getCount()==0){ 
	              Ext.getCmp('view_httplog').setVisible(false); 
	   //根据返回的用户信息查询上下线日志          
	              if(flag_sxx==0){
                 	   var num=0;
	              	 	                                        //根据返回的结果查询
	              	 	 for(i=0;i<store_httplog.getCount();i++){
	              	 	 	         did=records[i].get('did');
	              	 	 	         uid=records[i].get('uid');
	              	 	 	         temp=' did='+did+' and uid='+uid;
	              	 	 	         if(where.indexOf(temp)==-1){
	              	 	 	         	num++;
	              	 	 	         	if(num==1){
	              	 	 	         		where='('+temp+')';
	              	 	 	         	}
	              	 	 	         	else{
	              	 	 	         	 where=where+' or '+'('+temp+')';
	              	 	 	          }
	                            }
	              	 	 	 
	              	 	 }
	
	              	 	 if(where.length>10){
	              	 	 	    store_sxxlog.new_params={time_flag:time_flag,sdate:sdate,edate:edate,stime:etime,username:username,dispname:dispname,idno:idno,mac:mac,where:where};
										      store_sxxlog.load(
										       {
											    	  callback:function(records, options, success){
											   
										             if(store_sxxlog.getCount()==0){ 
										              Ext.getCmp('view_sxxlog').setVisible(false); 
										             } 
										             else{
										             	 Ext.getCmp('view_sxxlog').setVisible(true); 
										             }  
											    	  }
											    	}
										      );              	 	 	
	              	 	 }
	  //查询虚拟身份            	 	 
	              	 	 if(flag_xnsf==0){
	              	 	 	    store_netidlog.new_params={time_flag:time_flag,sdate:sdate,edate:edate,stime:etime,username:username,dispname:dispname,idno:idno,mac:mac,where:where};
										      store_netidlog.load(
										       {
											    	  callback:function(records, options, success){
										             if(store_netidlog.getCount()==0){ 
										              Ext.getCmp('view_netidlog').setVisible(false); 									 										              
									             } 
										             else{
										             	 Ext.getCmp('view_netidlog').setVisible(true); 
										             }  
											    	  }
											    	}
										      );
	              	 	 	
	              	 	 }
	              	 	
	         	         //查询searchlog            	 	 
	              	 	 if(flag_content==0){
	              	 	 	     store_searchlog.new_params={time_flag:time_flag,sdate:sdate,edate:edate,stime:etime,username:username,dispname:dispname,idno:idno,mac:mac,keyword:keyword,where:where};
										      store_searchlog.load(
										       {
											    	  callback:function(records, options, success){
										             if(store_searchlog.getCount()==0){ 
										              Ext.getCmp('view_searchlog').setVisible(false); 
										             } 
										             else{
										             	 Ext.getCmp('view_searchlog').setVisible(true); 
										             } 
											    	  }
											    	}
										      );
	              	 	 	
	              	 	 }    
	             
	             	     //查询bbslog            	 	 
	              	 	 if(flag_content==0){
	              	 	 	     store_bbslog.new_params={time_flag:time_flag,sdate:sdate,edate:edate,stime:etime,username:username,dispname:dispname,idno:idno,mac:mac,keyword:keyword,where:where};
										      store_searchlog.load(
										       {
											    	  callback:function(records, options, success){
										             if(store_bbslog.getCount()==0){ 
										              Ext.getCmp('view_bbslog').setVisible(false); 
										             } 
										             else{
										             	 Ext.getCmp('view_bbslog').setVisible(true); 
										             } 
											    	  }
											    	}
										      );
	              	 	 	
	              	 	 } 
	             
	             	      //查询邮件           	 	 
	              	 	 if(flag_content==0){
	              	 	 	    store_maillog.new_params={time_flag:time_flag,sdate:sdate,edate:edate,stime:etime,username:username,dispname:dispname,idno:idno,mac:mac,keyword:keyword,where:where};
										      store_maillog.load(
										       {
											    	  callback:function(records, options, success){
										             if(store_maillog.getCount()==0){ 
										              Ext.getCmp('view_maillog').setVisible(false); 
										             } 
										             else{
										             	 Ext.getCmp('view_maillog').setVisible(true); 
										             } 
											    	  }
											    	}
										      );
	              	 	 	
	              	 	 }
	             
	             
	             
	              	 	
	              	 	
	              
	              }
	              
	              
	              
	             } 
	             else{
	             	 Ext.getCmp('view_httplog').setVisible(true); 
	             }  
		    	  }
		    	}
	      );
    	
    	
    	
    } 
    
    if(username!=''||dispname!=''||idno!=''||mac!=''||keyword!=''){
    	  flag_content=1;    	
    	  store_searchlog.new_params={time_flag:time_flag,sdate:sdate,edate:edate,stime:etime,username:username,dispname:dispname,idno:idno,mac:mac,keyword:keyword};
	      store_searchlog.load(
	       {
		    	  callback:function(records, options, success){
	//	          alert(store_sxxlog.getCount()); 
	             if(store_searchlog.getCount()==0){ 
	              Ext.getCmp('view_searchlog').setVisible(false); 
	   //根据返回的用户信息查询上下线日志          
	              if(flag_sxx==0){
	              }
	              
	              
	              
	             } 
	             else{
	             	 Ext.getCmp('view_searchlog').setVisible(true); 
	             }  
		    	  }
		    	}
	      );
    	
    	
    	
    }  
      
     if(username!=''||dispname!=''||idno!=''||mac!=''||keyword!=''){
    	  flag_content=1;    	
    	  store_bbslog.new_params={time_flag:time_flag,sdate:sdate,edate:edate,stime:etime,username:username,dispname:dispname,idno:idno,mac:mac,keyword:keyword};
	      store_bbslog.load(
	       {
		    	  callback:function(records, options, success){
	       
	             if(store_bbslog.getCount()==0){ 
	              Ext.getCmp('view_bbslog').setVisible(false); 
	             
	             } 
	             else{
	             	 Ext.getCmp('view_bbslog').setVisible(true); 
	             }  
		    	  }
		    	}
	      );
    	
    	
    	
    }   
      
    if(username!=''||dispname!=''||idno!=''||mac!=''||keyword!=''){
    	  flag_content=1;    	
    	  store_maillog.new_params={time_flag:time_flag,sdate:sdate,edate:edate,stime:etime,username:username,dispname:dispname,idno:idno,mac:mac,keyword:keyword};
	      store_maillog.load(
	       {
		    	  callback:function(records, options, success){
	//	          alert(store_sxxlog.getCount()); 
	             if(store_maillog.getCount()==0){ 
	              Ext.getCmp('view_maillog').setVisible(false); 
	   //根据返回的用户信息查询上下线日志          
	              if(flag_sxx==0){
	              }
	              
	              
	              
	             } 
	             else{
	             	 Ext.getCmp('view_maillog').setVisible(true); 
	             }  
		    	  }
		    	}
	      );
    	
    	
    	
    }   
      
    },
    SetPage:function(curpage){
      var store = Ext.ComponentQuery.query('sxxlog')[0].getStore();
      store.currentPage=curpage;
       
    },
    setTitle: function(title){
       document.getElementById("titledx").innerHTML='&nbsp'+title+'&nbsp;&nbsp;';   
 /*     
      var grid = Ext.ComponentQuery.query('dwgllist')[0];
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
