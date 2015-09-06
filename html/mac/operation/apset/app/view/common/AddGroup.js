
    		       			
Ext.define('proauthApset.view.common.AddGroup' ,{
  extend: 'Ext.window.Window',
  alias : 'widget.addgroup',
	title: '单位AP信息编辑',
	modal: true,
	field1: null,
	field2: null,
	value1:null,
  initComponent: function() {
  	  var required='<span style="color:red;font-weight:bold" data-qtip="Required">*</span>';
       
       var facCombo = Ext.create('Ext.form.field.ComboBox', {
        fieldLabel: '安全厂商名称',
        labelWidth: 100,
        width: 320,
        valueField:'groupid',
        value:'',
        displayField: 'groupname',
        name : 'fcode',      
        allowBlank: false,
        afterLabelTextTpl:required,
//        forceSelection: true,
        store: store_fac,
        triggerAction: 'all',
//      queryMode:'remote',
         queryMode: 'local',
         typeAhead: true,
              listeners: {
			          'change':function(){
			           
			            }
			        }
    });
     
     
   
    
  	this.items = [
  	{    
	title:'系统信息',
  	     xtype: 'fieldset',
		 frame:true,width:700, 
		 bodyPadding:5,
  	        items: [
  	        {xtype:'hidden',id:'groupid',name:'groupid',value:''},
            {
  	           xtype: 'fieldcontainer',layout: 'vbox',            
               items:[,{
						 xtype:'fieldcontainer',layout:'hbox',
						 items:[ 
						     ,{xtype:'textfield',fieldLabel: '采集设备名称',labelWidth:100,emptyText: '采集设备名称',width:320,name: 'gwid',afterLabelTextTpl:required}
						 ]
						 
					 },{
						 xtype:'fieldcontainer',layout:'hbox',
						 items:[ 
						    {
			            	 
			                fieldLabel: '&nbsp;采集设备类型',
			                labelWidth:100,
			                width:320,
                                xtype:          'combo',
                                mode:           'local',
                                value:          '1',
                                triggerAction:  'all',
                                forceSelection: true,
                                editable:       false,                                
                                name:           'aptype',              
                                displayField:   'name',
                                valueField:     'value',
                                queryMode: 'local',
                                store: Ext.create('Ext.data.Store', {
                                    fields : ['name', 'value'],
                                    data   : [
                                        {name : '固定采集类型',   value: '1'},
                                        {name : '移动车载设备',   value: '2'},
                                        {name : '单兵采集类型',   value: '3'},
                                        {name : '其它',   value: '9'}          
                                    ]
                                })
			                      
			            }  
						 ]
						 
					 },{
						 xtype:'fieldcontainer',layout:'hbox',
						 items:[
				,{xtype:'textfield',fieldLabel: '&nbsp;设备地址',labelWidth:100,emptyText: '',width:320,name: 'address'}
						 ]
					 },{ xtype: 'fieldcontainer',layout: 'hbox',
                items:[
				//安全厂商名称
                ,facCombo
			         ]
			         },
			         { xtype: 'fieldcontainer',layout: 'hbox',
                items:[
               {xtype:'textfield',fieldLabel: '安全设备经度',labelWidth:100,emptyText: '',width:320,name: 'longitude'}   
			         ]
			         },{
						  xtype:'fieldcontainer',layout:'hbox',
						  items:[
                          ,{xtype:'textfield',fieldLabel: '&nbsp;安全设备纬度',labelWidth:100,emptyText: '',width:320,name: 'latitude'}
						  ]
					 },{ xtype: 'fieldcontainer',layout: 'hbox',
                items:[
                    {xtype:'textfield',fieldLabel: '&nbsp;采集半径',labelWidth:100,emptyText: '',width:320,name: 'radius',afterLabelTextTpl:required}
			         ]
			         },
			         { xtype: 'fieldcontainer',layout: 'hbox',
                items:[
                {xtype:'textfield',fieldLabel: '上传数据间隔时间',labelWidth: 100,emptyText: '',width:320,name: 'uptimestep'}  
			         ]
			         }
			       ]
			     }
			     ],
			   	buttons:[
			   	 {xtype : 'displayfield',value:'&nbsp;&nbsp;'}
			   	,{text:'保存',action: 'addsave'}
			   	,{text:'关闭',handler: function() {
                        var tform=this.up('window').close();
		                    }	
					 }
	       ]		  
   
     },{
		 
  	     xtype: 'fieldset',
		 title:'移动采集设备参数',
		 frame:true,
		 width:700,
		 bodyPadding:5,
  	        items: [
  	        {xtype:'hidden',id:'groupid',name:'groupid',value:''},
            {
  	           xtype: 'fieldcontainer',layout: 'vbox',            
               items:[
			         { xtype: 'fieldcontainer',layout: 'hbox',
                items:[, 
               ,{xtype:'textfield',fieldLabel: '车牌号码',labelWidth: 70,emptyText: '',width:210,name: 'plate'}   
            	 ,{xtype:'textfield',fieldLabel: '&nbsp;交通线路信息',labelWidth: 80,emptyText: '',width:220,name: 'line'}
			         ]
			         },
			         { xtype: 'fieldcontainer',layout: 'hbox',
                items:[
              ,{xtype:'textfield',fieldLabel: '车辆信息',labelWidth: 70,emptyText: '',width:210,name: 'plate'}   
            	 ,{xtype:'textfield',fieldLabel: '&nbsp;车厢编号',labelWidth: 80,emptyText: '',width:220,name: 'line'}
			         ]
			         }
			       ]
			     }
			     ],
			   	buttons:[
			   	 {xtype : 'displayfield',value:'&nbsp;&nbsp;'}
			   	,{text:'保存',action: 'addsave'}
			   	,{text:'关闭',handler: function() {
                        var tform=this.up('window').close();
		                    }	
					 }
	       ]		  
   
     }
    ];
  
    this.callParent(arguments);

     var tform=this.query('form')[0];
    
     if(this.field1&&this.field1!=''){    	
        tform.getForm().loadRecord(this.field1);          
     }

  }
});  
