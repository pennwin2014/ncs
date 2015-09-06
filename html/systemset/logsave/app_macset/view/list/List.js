
  

Ext.define('proauthApMacset.view.list.List' ,{
 extend: 'Ext.Panel',
  alias : 'widget.apmacsetlist',
	title: '日志留存设置',
	modal: true,
	field1: null,
	field2: null,
	value1:null,
	frame:true,
	layout: { pack: 'center', type: 'hbox' } ,
	height:900,
	
  initComponent: function() {
  	  var required='<span style="color:red;font-weight:bold" data-qtip="Required">*</span>';
      
  	this.items = [
  	{
  	     xtype: 'form',frame:true,width:700, bodyPadding:5,margin: '50 0 0 0',
  	     id:'formmacset',
  	        items: [
  	                    {
  	           xtype: 'fieldcontainer',layout: 'vbox',            
               items:[			        
			         { xtype: 'fieldcontainer',layout: 'hbox',
                items:[
                {fieldLabel: '&nbsp;启用日志留存功能',labelWidth: 170,width:240,afterLabelTextTpl:required,
                	             xtype:          'combo',
                                mode:           'local',
                              
                                triggerAction:  'all',
                                forceSelection: true,
                                editable:       false,                                
                                name:           'StartAutoClean',              
                                displayField:   'name',
                                valueField:     'value',
                                queryMode: 'local',
                                store: Ext.create('Ext.data.Store', {
                                    fields : ['name', 'value'],
                                    data   : [ {name : '启用',   value: 'Yes'}, {name : '不启用',   value: 'No'}]
                                })
                	
                	}   

			         ]
			         }		         
			       ]
			     },
            {
  	           xtype: 'fieldcontainer',layout: 'vbox',            
               items:[			        
			         { xtype: 'fieldcontainer',layout: 'hbox',
                items:[
                {xtype:'numberfield',fieldLabel: '&nbsp;上下线日志数据库保存时间',labelWidth: 170,emptyText: '6',width:240,name: 'termlog'}
                 ,{
			            	  xtype     : 'displayfield',
			                value:'&nbsp;(单位:天) 数据上报后数据库日志留存时限'
			            }
			         ]
			         },
			         { xtype: 'fieldcontainer',layout: 'hbox',
                items:[
                {xtype:'numberfield',fieldLabel: '&nbsp;上下线日志保存文件时间',labelWidth: 170,emptyText: '30',width:240,name: 'termlogfile'}
                 ,{
			            	  xtype     : 'displayfield',
			                value:'&nbsp;(单位:天) '
			            }
			         ]
			         }
			         ,
			         
			          { xtype: 'fieldcontainer',layout: 'hbox',
                items:[
                {xtype:'numberfield',fieldLabel: '&nbsp;上网日志数据库保存时间',labelWidth: 170,emptyText: '6',width:240,name: 'termiplog'}
                 ,{
			            	  xtype     : 'displayfield',
			                value:'&nbsp;(单位:天) 数据上报后数据库日志留存时限'
			            }
			         ]
			         },
			         { xtype: 'fieldcontainer',layout: 'hbox',
                items:[
                {xtype:'numberfield',fieldLabel: '&nbsp;上网日志文件保存时间',labelWidth: 170,emptyText: '30',width:240,name: 'termiplogfile'}
                 ,{
			            	  xtype     : 'displayfield',
			                value:'&nbsp;(单位:天) '
			            }
			         ]
			         }
			         
			         ,
			         { xtype: 'fieldcontainer',layout: 'hbox',
                items:[
                {xtype:'numberfield',fieldLabel: '&nbsp;终端特征信息保存时间',labelWidth: 170,emptyText: '30',width:240,name: 'termmaclog'}
                 ,{
			            	  xtype     : 'displayfield',
			                value:'&nbsp;(单位:天) '
			            }
			         ]
			         }
			         ,
			         { xtype: 'fieldcontainer',layout: 'hbox',
                items:[
                {xtype:'numberfield',fieldLabel: '&nbsp;热点信息保存时间',labelWidth: 170,emptyText: '30',width:240,name: 'hotinfolog'}
                 ,{
			            	  xtype     : 'displayfield',
			                value:'&nbsp;(单位:天) '
			            }
			         ]
			         }
			          ,
			         { xtype: 'fieldcontainer',layout: 'hbox',
                items:[
                {xtype:'numberfield',fieldLabel: '&nbsp;移动AP轨迹保存时间',labelWidth: 170,emptyText: '30',width:240,name: 'mobileaplog'}
                 ,{
			            	  xtype     : 'displayfield',
			                value:'&nbsp;(单位:天) '
			            }
			         ]
			         }
			         
			         ,
			         { xtype: 'fieldcontainer',layout: 'hbox',
                items:[
                {xtype:'numberfield',fieldLabel: '&nbsp;操作日志保存时间',labelWidth: 170,emptyText: '30',width:240,name: 'operlog'}
                 ,{
			            	  xtype     : 'displayfield',
			                value:'&nbsp;(单位:天) '
			            }
			         ]
			         }
			         
			         
			         
			         
			         	         
			       ]
			     }
			     
			     
			     
			     	
			     
			     ],
			   	buttons:[			  
			   	 {xtype : 'displayfield',value:'&nbsp;&nbsp;'}
			   	,{text:'保存',action: 'addsave'}
	//	   	,{text:'取消',handler: function() {
  //                     var tform=this.up('form').getForm().reset();
	//                    }	
	
	//				 }
	       ]		  
   
     }
    ];
 
    this.callParent(arguments);
     var tform=this.query('form')[0];
   store_logsave.load({callback:function(r,options,success){
	  if(success){          
	     tform.getForm().loadRecord(r[0]); 
	  }
    }
    });
  }    
});