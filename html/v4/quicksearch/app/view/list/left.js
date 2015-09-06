Ext.define('quickResarch.view.list.left' ,{
  extend: 'Ext.form.Panel',
  alias : 'widget.left',
  region:'west',
  frame:true,    
  height: 2340,
  title:'',  
  initComponent: function() {
  
   this.items = [
     {     	
     	  xtype: 'fieldcontainer',                          
         layout: 'vbox',
          items:[
               {
                  xtype: 'fieldset',
                  collapsible: false,
                  collapsed:false,
                  title:'查询时间',
                   margin: '0 0 0 0',
  	              defaultType: 'textfield',
  	              items: [
                  {
          	       xtype: 'fieldcontainer',
                   layout: 'vbox',
                    items:[
                    { 
                      fieldLabel: '时间段',
			                labelWidth: 40,
			                width:195, xtype:'combo',mode:'local',value:'1',triggerAction:  'all',forceSelection: true,editable:false,                                
                      id:'time_flag',name: 'time_flag', displayField:'name',valueField:'value', queryMode: 'local',
                      store: Ext.create('Ext.data.Store', {
                                    fields : ['name', 'value'],
                                    data   : [{name : '今天',   value: '1'},{name : '三天以内',value: '2'},{name : '最近一周',value: '3'},{name : '最近一月',value: '4'},{name : '最近三月',value: '5'},{name : '最近半年',value: '6'},{name : '自定义时间',value: '9'}
                                    ]
                      }),
                      listeners:{
			                 'change':function(){
			                 	if(Ext.getCmp('time_flag').value==9){
			                     Ext.getCmp('containersdate').setVisible(true);
			                     Ext.getCmp('containeredate').setVisible(true);
			                  }
			                  else{
			                  	 Ext.getCmp('containersdate').setVisible(false);
			                     Ext.getCmp('containeredate').setVisible(false);
			                  }
			                
                      }
                      }		                
			              },
			             { xtype: 'fieldcontainer',layout: 'hbox',hidden:true,id:'containersdate',
                   items:[{xtype: 'datefield',id:'sdate',format: "Y/m/d",fieldLabel: '起始',labelWidth: 40,emptyText:'',width:135,name: 'sdate',value:''},	{                        
                        fieldLabel:'',
                        width:60,
                        format: "H:i",
                        xtype: 'timefield',
                        value:Ext.Date.add(new Date(), Ext.Date.HOUR, -1),
                        id:'stime',
                        name:'stime'
                     }]
			             }
			              ,
			           { xtype: 'fieldcontainer',layout: 'hbox',hidden:true,id:'containeredate',
                  items:[{xtype: 'datefield',id:'edate',format: "Y/m/d",fieldLabel: '终止',labelWidth: 40,emptyText:'',width:135,name: 'edate',value:''},	{                        
                        fieldLabel:'',
                        width:60,
                        format: "H:i",
                        xtype: 'timefield',
                        value:Ext.Date.add(new Date(), Ext.Date.HOUR, -1),
                        id:'etime',
                        name:'etime'
                     }]
			             }
			              
			              
			              
			              
			              ]
			            }
			            ]
			          }
			          ,
			          {
                  xtype: 'fieldset',
                  collapsible: false,
                  collapsed:false,
                  title:'用户信息',
                   margin: '0 0 0 0',
  	              defaultType: 'textfield',
  	              items: [
  	               {xtype:'textfield',fieldLabel: '登录名',labelWidth: 52,emptyText: '',width:195,name: 'username'},
  	               {xtype:'textfield',fieldLabel: '姓名',labelWidth: 52,emptyText: '',width:195,name: 'dispname'},
  	               {xtype:'textfield',fieldLabel: '证件号',labelWidth: 52,emptyText: '',width:195,name: 'idno'},
  	               {xtype:'textfield',fieldLabel: 'MAC地址',labelWidth: 52,emptyText: '',width:195,name: 'mac'}
  	              ]
  	            }
			          
			         ,
			          {
                  xtype: 'fieldset',
                  collapsible: false,
                  collapsed:false,
                  title:'虚拟身份',
                   margin: '0 0 0 0',
  	              defaultType: 'textfield',
  	              items: [
  	               {xtype:'textfield',fieldLabel: '虚拟帐号',labelWidth: 52,emptyText: '',width:195,name: 'netid'},
  	                  { 
        	                        width:          195,
        	                        fieldLabel: '虚拟类别',labelWidth: 52,
                                	xtype:          'treecombox',
                                	mode:           'local',
                                	value:          '',
                                	name:           'nettype',
                                	displayField:   'text',
                                	valueField:     'id',
                                	queryMode: 	'local',
                               	 	store:   store_classtree
	                
			              }
  	              ]
  	            }
  	           ,
			          {
                  xtype: 'fieldset',
                  collapsible: false,
                  collapsed:false,
                  title:'网址',
                   margin: '0 0 0 0',
  	              defaultType: 'textfield',
  	              items: [
  	               {xtype:'textfield',fieldLabel: '网址关键字',labelWidth: 52,emptyText: '',width:195,name: 'url'}
  	             
  	              ]
  	            }
  	            
  	            
			          ,
			          {
                  xtype: 'fieldset',
                  collapsible: false,
                  collapsed:false,
                  title:'内容关键字',
                   margin: '0 0 0 0',
  	              defaultType: 'textfield',
  	              items: [
  	               {xtype:'textfield',fieldLabel: '关键字',labelWidth: 52,emptyText: '',width:195,name: 'keyword'}
  	             
  	              ]
  	            }
  	            ,
  	             { xtype: 'fieldcontainer',layout: 'hbox',
                items:[
			          {
                  xtype: 'button',text:'查&nbsp;&nbsp;询',
                  style : 'margin-top:10px;margin-left:50px',
                  handler:function(){
                  	var form1=this.up('form');
                  
             //     	alert(this.up('form'));
              	quickResarch.controller.Procy.loadProcyListStore(form1);
                  }
        
  	            },{
                  xtype: 'button',text:'清除条件',
                  style : 'margin-top:10px;margin-left:10px',
                  handler:function(){
                  	
                  }
        
  	            }]
			             }
			          
			          ]
			        }
     
   ]
    
  ;

    this.callParent(arguments);
  }
});            