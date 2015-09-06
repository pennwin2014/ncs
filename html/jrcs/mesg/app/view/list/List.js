Ext.define('proauthMobileAccount.view.list.List' ,{
  extend: 'Ext.grid.Panel',
  alias : 'widget.mobileaccountlist',
  store : 'List', 
  height: 650,
  title:'',
  columnLines: true,
  initComponent: function() {
    var sm = Ext.create('Ext.selection.CheckboxModel',{
        listeners: {
            selectionchange: function(sm, selections) {  
            	var grid=Ext.ComponentQuery.query('mobileaccountlist')[0];
                  
              grid.down('#removeButton').setDisabled(selections.length == 0);
    //          grid.down('#moveGroupButton').setDisabled(selections.length == 0);
    
                
            }
        }
    
    });
    this.selModel=sm;

        var whCombo = Ext.create('Ext.form.field.ComboBox', {
        fieldLabel: '收发方向',
        labelWidth: 60,
        width:170,
        valueField:'value',
        displayField: 'name',
        id:'whflag',
        name : 'flag',      
        value:'',
        allowBlank: true,
        forceSelection: false,
                                    store: Ext.create('Ext.data.Store', {
                                    fields : ['name', 'value'],
                                    data   : [
                                     {name:"选择收发方向",value:""},
                                        {name : '接收消息',   value: '1'},
                                        {name : '发送消息',   value: '2'}
                                                                      
                                    ]
                                })
                                ,
        queryMode: 'local',
        typeAhead: true,
                listeners: {
			          'change':function(){
			             proauthMobileAccount.controller.Procy.loadProcyListStore();
			            }
			        }
    });
    
  //  this.plugins=rowEditing;
    this.tbar = Ext.create('Ext.PagingToolbar',{
    	store:'List',
      displayInfo: true,
      displayMsg: '显示{0} - {1}条记录 共{2}条记录',
     emptyMsg: "没有记录可显示",
     items:[
              whCombo,
              {
               xtype:'textfield',
               id:'keyword_mobileaccount',
               width:160,
               name:'keyword5',
               style:'color:#7aa7d5',             
               emptyText:'内容/主题/接收发送对象',
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
            {
                text:'我要发消息',
                iconCls:'add',
                action:'addmesg'
   
            },
            '-',
     	 {
     	 	text:'删除',
     	 	itemId: 'removeButton',
     	 	iconCls:'remove',
     	 	disabled: true,
     	 	action:'del'
     	},
         '-'
     ,
     	 {xtype:'label', html:'<span id="titledx"></span>'}
     	]
    }
    );
    
    
   
  	
    this.columns = [
            {
          header: '回复消息',
            xtype: 'actioncolumn',
            dataIndex: 'userid',
             icon: '/images/v4/edit_task.png',
            tooltip: '点击该图标进入回复界面。',
            width: 55,
            align: 'center',
            sortable: false,
            handler: function(grid, rowIndex, colIndex, actionItem, event, record, row) {
              var mmto=record.get('mfrom');
              var mmsubject='回复:'+record.get('message_title');
  
              var fpw=Ext.create('Ext.FormPanel', {         
			        frame: true,
			        fieldDefaults: {
			           labelWidth: 65,                        
			           xtype: 'textfield' 
			        },
              bodyPadding: 10,
                  items: [
                   {xtype: 'hidden',name: 'mto',value:mmto},                                 
                 {
                        fieldLabel:'主题',
                        xtype: 'textfield',
                        value:mmsubject,
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
                        text:'回&nbsp;&nbsp;复',
               handler: function(){
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
          }

        },
      {header: '时间',dataIndex: 'stime',width:120, sortable: false},
      {header: '发送方', dataIndex: 'mfrom_name', width:120,sortable: false},
      {header: '接收方', dataIndex: 'mto_name', width:120,sortable: false},
      {header: '主题', dataIndex: 'message_title', flex:1},

      {header: '内容', dataIndex: 'message_content', flex:1}
     

      
    ];

    this.callParent(arguments);
  }
    
});