
  

Ext.define('proauthApMacset.view.list.List' ,{
 extend: 'Ext.Panel',
  alias : 'widget.apmacsetlist',
	title: '日志上传参数设置',
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
                {fieldLabel: '&nbsp;启动生成BCP文件进程',labelWidth: 170,width:240,afterLabelTextTpl:required,
                	             xtype:          'combo',
                                mode:           'local',                              
                                triggerAction:  'all',
                                forceSelection: true,
                                editable:       false,                                
                                name:           'StartDb2Bcp',              
                                displayField:   'name',
                                valueField:     'value',
                                queryMode: 'local',
                                store: Ext.create('Ext.data.Store', {
                                    fields : ['name', 'value'],
                                    data   : [ {name : '启动',   value: 'Yes'}, {name : '不启动',   value: 'No'}]
                                })
                	
                	},
                {xtype:'textfield',fieldLabel: '&nbsp;BCP文件存放目录',labelWidth: 130,emptyText: '/data/bcpout',width:340,name: 'Db2BcpOutPath'}

			         ]
			         }		         
			       ]
			     },
            {
  	           xtype: 'fieldcontainer',layout: 'vbox',            
               items:[			        
			         { xtype: 'fieldcontainer',layout: 'hbox',
                items:[
                {xtype:'numberfield',fieldLabel: '&nbsp;生成文件时间间隔(单位:秒)',labelWidth: 170,emptyText: '10',width:240,name: 'Db2BcpStep'}
                ,{xtype:'textfield',fieldLabel: '&nbsp;数据加密密码',labelWidth: 130,emptyText: 'Ac123ASfgq',width:340,name: 'DB2BcpZipPass'}
			         ]
			         },
			        { xtype: 'fieldcontainer',layout: 'hbox',
                items:[
                {xtype:'textfield',fieldLabel: '&nbsp;数据产生源标识',labelWidth: 170,emptyText: '310012',width:240,name: 'Db2BcpScode'}
                ,{xtype:'textfield',fieldLabel: '&nbsp;数据传送目标标识',labelWidth: 130,emptyText: '310000',width:340,name: 'Db2BcpDcode'}
			         ]
			         },
			         
			         
			         
			         { xtype: 'fieldcontainer',layout: 'hbox',
                items:[
            
                  {fieldLabel: '&nbsp;数据传送后是否保存文件',labelWidth: 170,width:240,
                	             xtype:          'combo',
                                mode:           'local',
                              
                                triggerAction:  'all',
                                forceSelection: true,
                                editable:       false,                                
                                name:           'FtpKeep',              
                                displayField:   'name',
                                valueField:     'value',
                                queryMode: 'local',
                                store: Ext.create('Ext.data.Store', {
                                    fields : ['name', 'value'],
                                    data   : [ {name : '保存',   value: 'Yes'}, {name : '不保存',   value: 'No'}]
                                })
                	
                	},
                	{xtype:'textfield',fieldLabel: '&nbsp;保存路径',labelWidth: 130,emptyText: '/data/ftpkeep',width:340,name: 'FtpKeepPath'}
			         ]
			         }
			         ,
			         
			          { xtype: 'fieldcontainer',layout: 'hbox',
                items:[
              {fieldLabel: '&nbsp;数据传送方式',labelWidth: 170,width:240,
                	             xtype:          'combo',
                                mode:           'local',
                              
                                triggerAction:  'all',
                                forceSelection: true,
                                editable:       false,                                
                                name:           'Db2BcpSendType',              
                                displayField:   'name',
                                valueField:     'value',
                                queryMode: 'local',
                                store: Ext.create('Ext.data.Store', {
                                    fields : ['name', 'value'],
                                    data   : [ {name : 'FTP',   value: 'FTP'}]
                                })
                	
                	},
                 {fieldLabel: '&nbsp;启动FTP传送进程',labelWidth: 130,width:340,
                	             xtype:          'combo',
                                mode:           'local',
                              
                                triggerAction:  'all',
                                forceSelection: true,
                                editable:       false,                                
                                name:           'StartSendFtp',              
                                displayField:   'name',
                                valueField:     'value',
                                queryMode: 'local',
                                store: Ext.create('Ext.data.Store', {
                                    fields : ['name', 'value'],
                                    data   : [ {name : '启动',   value: 'Yes'}, {name : '停止',   value: 'No'}]
                                })
                	
                	}
                
			         ]
			         },
			         { xtype: 'fieldcontainer',layout: 'hbox',
                items:[
                {xtype:'numberfield',fieldLabel: '&nbsp;FTP传送时间间隔(秒)',labelWidth: 170,emptyText: '10',width:240,name: 'FtpTimeStep'}
                ,{xtype:'textfield',fieldLabel: '&nbsp;FTP传送源路径',labelWidth: 130,emptyText: '/data/bcpload',width:340,name: 'FtpLocalPath'}
			         ]
			         },
			         
			         { xtype: 'fieldcontainer',layout: 'hbox',
                items:[
                {xtype:'textfield',fieldLabel: '&nbsp;文件名包含关键字',labelWidth: 170,emptyText: 'zip',width:240,name: 'FtpFileKey'}
                ,{xtype:'textfield',fieldLabel: '&nbsp;FTP传送远程路径',labelWidth: 130,emptyText: '/data/proeim/',width:340,name: 'FtpRemotePath'}
			         ]
			         },
			         
			         { xtype: 'fieldcontainer',layout: 'hbox',
                items:[
                {xtype:'textfield',fieldLabel: '&nbsp;FTP用户名',labelWidth: 170,emptyText: 'proeim',width:240,name: 'FtpUser'}
                ,{xtype:'textfield',fieldLabel: '&nbsp;FTP服务器地址',labelWidth: 130,emptyText: '',width:340,name: 'FtpServer'}
			         ]
			         },
			         { xtype: 'fieldcontainer',layout: 'hbox',
                items:[
                {xtype:'textfield',fieldLabel: '&nbsp;FTP密码',labelWidth: 170,emptyText: 'ProEIM123',width:300,name: 'FtpPass'}
              
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