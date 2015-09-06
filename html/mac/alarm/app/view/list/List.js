

Ext.define('proauthMobileAccount.view.list.List' ,{
  extend: 'Ext.grid.Panel',
  alias : 'widget.mobileaccountlist',
  store : 'List', 
  height: 900,
  title:'',
  columnLines: true,
  initComponent: function() {
    var sm = Ext.create('Ext.selection.CheckboxModel',{
        listeners: {
            selectionchange: function(sm, selections) {  
            	var grid=Ext.ComponentQuery.query('mobileaccountlist')[0];
                  
              grid.down('#removeButton').setDisabled(selections.length == 0);
			  grid.down('#ckEmployee1').setDisabled(selections.length == 0);
			  grid.down('#ckEmployee').setDisabled(selections.length == 0);
    //          grid.down('#moveGroupButton').setDisabled(selections.length == 0);
    
                
            }
        }
    
    });
    this.selModel=sm;

      var rowEditing = Ext.create('Ext.ux.grid.plugin.RowEditing', {
    	      pluginId:'rowEditing', 
            saveBtnText: "保存", 
            cancelBtnText: "取消", 
            autoRecoverOnCancel:true,
            autoCancel: true,
            filed1:'casename',
            filed2:'expiretime',
            filed3:'cobj',
            filed4:'cmemo',
            filed5:'cobjid',
            filed6:'sid'
          
 //       clicksToMoveEditor: 1,
 //       autoCancel: false
    });
    
   
    this.plugins=rowEditing;
    this.tbar = Ext.create('Ext.PagingToolbar',{
    	store:'List',
		id:"pagtool",
      displayInfo: true,
      displayMsg: '显示{0} - {1}条记录 共{2}条记录',
     emptyMsg: "没有记录可显示",
     items:[
              {
               xtype:'textfield',
               id:'keyword_mobileaccount',
               width:140,
               name:'keyword5',
               style:'color:#7aa7d5',             
               emptyText:'请输入规则名称查询',
               enableKeyEvents:true,
               listeners:{
                 'focus':function(){
        
                 },
         
                 'keydown' : function(i,e){
                var aae=e.getKey() ; 
                if(aae==13){
                     
                }
               }
               }
             },
             {
                text:'查询',
                iconCls:'accept',
                action:'keycx'
   
            },
         '-',
     	 {
     	 	text:'添加布控',
     	 	iconCls:'add',
     	 	 handler : function() {
                rowEditing.cancelEdit();    
                 var expirtime=Ext.Date.add(new Date(), Ext.Date.DAY, 90);       
                 var r = Ext.create('proauthMobileAccount.model.List', {  
                 	  flags:'1',             	  
                    expiretime: expirtime                  
                                   
                });
               var store = Ext.ComponentQuery.query('mobileaccountlist')[0].getStore();
    
               store.insert(0, r);
               rowEditing.startAdd1(0,0);
            
       //     rowEditing.startEdit(0, 0);
              }
     	 	
     	 	
//     	 	action:'add'
     	},'-',
     	 {
     	 	text:'删除',
     	 	itemId: 'removeButton',
     	 	iconCls:'remove',
     	 	disabled: true,
     	 	action:'del'
     	},'-',
     	
     	{
            itemId: 'ckEmployee',
            text: '置为有效',
            iconCls: 'fix',
                handler:function(){
							   if(confirm('您真的要执行该操作吗？')){                  
                 var selsid;  
                 var rows=sm.getSelection();
                   selsid='';
                  for(var i=0;i<rows.length;i++){
                  	if(i==0){
                  selsid="'" + rows[i].get('sid') + "'";
                  	}
                  	else{
                     selsid=selsid + "," + "'" + rows[i].get('sid') + "'";
                    }
                   
                   }
                   var grid = Ext.ComponentQuery.query('mobileaccountlist')[0];
                   var store = grid.getStore();
                      store.load({params: {setflag: "1",selsid:selsid}});              
              }
                
            },
            disabled: true
        } ,'-',
        {
            itemId: 'ckEmployee1',
            text: '撤销布控',
            iconCls: 'forbid',
                handler:function(){
             
							   if(confirm('您真的要撤销选择的策略吗？')){                  
                 var selsid;  
                 var rows=sm.getSelection();
                   selsid='';
                  for(var i=0;i<rows.length;i++){
                  	if(i==0){
                  	 selsid="'" + rows[i].get('sid') + "'";
                  	}
                  	else{
                      selsid=selsid + "," + "'" + rows[i].get('sid') + "'";
                    }
                   
                   }
                    var grid = Ext.ComponentQuery.query('mobileaccountlist')[0];
                    var store = grid.getStore();
                      store.load({params: {setflag: "9",selsid:selsid}});
                  
                
              }
                 
            },
            disabled: true
        },
        '-',
        {
     	 	text:'应用',
     	 	iconCls:'accept',
     	 	 handler : function() {
                  var grid = Ext.ComponentQuery.query('mobileaccountlist')[0];
                  var store = grid.getStore();
                  store.load({params: {app: "reload"}});
              }
     	 	
     	 	
//     	 	action:'add'
     	}
     	
     	,
     	'-',
     	 {xtype:'label', html:'<span id="titledx"></span>'}
     	]
    }
    );
    
    
   
  	
    this.columns = [
		{
        	  header: 'sid',
            dataIndex: 'sid',
            sortable: false,
            align:'center',
            hidden:true,
            width: 80
			
        },
      {header: '规则状态', dataIndex: 'flags', width:60, sortable: true,
      	renderer:function(value,metaData,record){
			   if(value=='0'){return '<font  color=red>未生效</font'}
                  else  if(value=='1'){return '<font color=green>已生效</font'}
					 else   {return '<font  color=red>已失效</font'}
					<!--  -->
                }
      	
      	
      	},
      {header: '规则名称',dataIndex: 'casename',width:140, sortable: false,editor: {emptyText:'请输入规则名称',allowBlank:true}},
      {header: '规则失效日期', dataIndex: 'expiretime', format: "Y/m/d",width:110,sortable: true,editor: { xtype:'datefield',emptyText:'请输入规则失效时间',allowBlank:true}},
      {header: '规则生效范围', dataIndex: 'cobj',   flex:1,editor: {xtype: 'treecomboxcheck',displayField:'text',valueField:'id',value : '',store:store_groupcheck,emptyText:'请选择规则生效范围',id:'opt',optid:'optid',opname:'opt',allowBlank:true}},
      {header: '规则描述', dataIndex: 'cmemo',flex:1,editor: { emptyText:'请输入规则描述',allowBlank:true}},      
      {header: '规则修改时间', dataIndex: 'lasttime', width:120,sortable: false},
      {header: '范围ID', hidden:true,dataIndex: 'cobjid', width:0,editor: {id:'optid',allowBlank:true}},
      {header: '告警数', dataIndex: 'wcount', width:50},
      {header: '最近告警时间', dataIndex: 'warnlasttime', width:120},
     
        {
          header: '规则策略',
    //        xtype: 'actioncolumn',
            dataIndex: 'pcount',
             icon: '/images/v4/edit_task.png',
            tooltip: '点击该图标进入修改单位信息界面。',
            width: 100,
            align: 'center',
            sortable: false,
            listeners:{
            	click:function(grid, rowIndex, colIndex, actionItem, event, record, row) {
           
        		var macstore=Ext.data.StoreMgr.lookup('MacStore');     
        			new_params={cid:record.get('sid')};
            macstore.on('beforeload', function (store, options) {
		        Ext.apply(store.proxy.extraParams, new_params);
		       }); 
		       
		       var warnstore=Ext.data.StoreMgr.lookup('WarnStore');     
        			new_params={cid:record.get('sid')};
             warnstore.on('beforeload', function (store, options) {
		        Ext.apply(store.proxy.extraParams, new_params);
		       }); 
		       
		      var warnlimstore=Ext.data.StoreMgr.lookup('MacLimStore');     
        			new_params={cid:record.get('sid')};
             warnlimstore.on('beforeload', function (store, options) {
		        Ext.apply(store.proxy.extraParams, new_params);
		       }); 
		       
            macstore.load();
            var pname=record.get('casename');
            var ptitle=pname+'_布控策略编辑';
            		  var win = Ext.create('proauthMobileAccount.view.common.AddWin',{
					        title: ptitle,
					        field1: record
					      });
					      win.showAt(50,20); 	  	
              
          }
            }
           ,
          renderer:function(value,metaData,record){
          	  var temp_disp=record.data.pcount;
				      if(temp_disp=='0'){
				      	       return Ext.String.format( '<img src="/v4/images/edit_task.png">&nbsp;<font color=red>(点击添加)</font>&nbsp;');
				      }
				    else{
				       return Ext.String.format( '<img src="/v4/images/edit_task.png">&nbsp;({0}条策略)&nbsp;',temp_disp);
				      }
      

          }

        },
          {
          header: '告警信息',
            xtype: 'actioncolumn',
            dataIndex: 'sid',
            icon: '/newver/resources/themes/images/share/List.png',
            tooltip: '点击该图标查看告警信息。',
            width: 60,
            align: 'center',
            sortable: false,
            handler: function(grid, rowIndex, colIndex, actionItem, event, record, row) {
            	var pname=record.get('casename');
              var ptitle=pname+'_布控告警信息';
            
              var warnlogstore=Ext.data.StoreMgr.lookup('WarnLogStore');  
              var keyword=document.getElementById("form1").keyword_warnlog.value;
              var state=document.getElementById("form1").clstate.value;
               
        			new_params={cid:record.get('sid'),keyword:keyword,state:state};
              warnlogstore.on('beforeload', function (store, options) {
		        Ext.apply(store.proxy.extraParams, new_params);
		       });
              warnlogstore.load();
			
              var win = Ext.create('proauthMobileAccount.view.common.WarnlogWin',{
					        title: ptitle,
					        field1: record,
							layout:'fit',
							viewConfig:{
								forceFit:true,
								scrollOffset:0
							}
					
					      });
						
					      win.showAt(50,20); 	
      
          }
        },
          {
          	width:20
          }

      
      
    ];

    this.callParent(arguments);
  }
    
});