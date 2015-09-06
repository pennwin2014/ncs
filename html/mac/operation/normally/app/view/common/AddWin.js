
     
        			       			
Ext.define('proauthApset.view.common.AddWin' ,{
  extend: 'Ext.window.Window',
  alias : 'widget.addwin',

	title: '合规热点信息管理',
	modal: true,
	field1: null,
	field2: null,
	value1:null,
  initComponent: function() {
  	  var required='<span style="color:red;font-weight:bold" data-qtip="Required">*</span>';
       
       var facCombo = Ext.create('Ext.form.field.ComboBox', {
        fieldLabel: '接入厂商',
        labelWidth: 70,
        width: 220,
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
  	     xtype: 'form',frame:true,width:400, bodyPadding:5,
  	        items: [
			{xtype:'hidden',id:'oldssid',name:'oldssid',value:''},
            {
  	           xtype: 'fieldcontainer',layout: 'vbox',            
               items:[
			   { xtype: 'fieldcontainer',
			   layout: 'hbox',			   
                items:[
					{xtype:'combo',
					fieldLabel: '是否合规',
					labelWidth: 90,
					value:'1',
					width:300,
					name: 'authflag',
					valueField:'value',
					displayField: 'name',
					emptyText: '请选择',
					store: Ext.create('Ext.data.Store', {
                                    fields : ['name', 'value'],
                                    data   : [
                                        {name : '合规',   value:'1'},
                                        {name : '不合规',   value:'2'}                                             
                                    ]
                                })				
				 } 
			         ]
			         }
					 , { xtype: 'fieldcontainer',layout: 'hbox',
					items:[
                ,{
				xtype:'textfield',
				fieldLabel:'热点SSID',
				labelWidth:90,
				emptyText:'',
				width:300,
				name:'ssid',
				afterLabelTextTpl:required,
				allowBlank:false, //是否允许为空
				blankText:'SSID不能为空！',	
				msgTarget:'qtip' //显示一个浮动的提示信息	
				}
			         ]
			         }, { xtype: 'fieldcontainer',layout: 'hbox',
                items:[ 
                ,{xtype:'combobox',
				fieldLabel: '热点加密类型',
				labelWidth: 90,				
				width:300,
				value:'01',
				name: 'security',
				valueField:'value',
                displayField: 'name',
				emptyText: '请选择',
				store: Ext.create('Ext.data.Store', {
                                    fields : ['name', 'value'],
                                    data   : [
                                        {name : 'WEB',   value: '01'},
                                        {name : 'WPA',   value: '02'},
                                        {name : 'WPA2',  value: '03'},
										{name : 'PSK',   value: '04'},
                                        {name : '其他',  value: '99'}          
												]
                                })
				}   
               
			         ]
			         },
					 { xtype: 'fieldcontainer',layout: 'hbox',
                     items:[
					,{xtype:'combobox',
					fieldLabel: '&nbsp;场所服务类型',
					labelWidth: 90,
					width:300,
					value:'9',
					name: 'servicetype',
					valueField:'value',
					displayField: 'name',
					emptyText: '请选择',
					store: Ext.create('Ext.data.Store', {
                                    fields : ['name', 'value'],
                                    data   : [
                                        {name : '酒店宾馆', value: '1'},
                                        {name : '图书馆阅览室',value: '2'},
                                        {name : '学校',value: '3'},
										{name : '娱乐场所',value: '4'},
										{name : '一般企业',value: '5'},
                                        {name : '其它',value: '9'}          
                                    ]
                                })
				 
				 }
			         ]
			         },{ xtype: 'fieldcontainer',layout: 'hbox',
                items:[
                ,{
					xtype:'combobox',
					fieldLabel: '部署场所性质',
					labelWidth: 90,
					width:300,
					name: 'servicequlity',
					valueField:'value',
					value:'0',
					displayField: 'name',
					emptyText: '请选择',
					store: Ext.create('Ext.data.Store', {
                                    fields : ['name', 'value'],
                                    data   : [
                                        {name : '经营',   value: '0'},
                                        {name : '非经营',   value: '1'},           
                                        {name : '其它',   value: '3'}          
                                    ]
                                })	
				}     
			         ]
			         },{
				xtype: 'fieldcontainer',
				layout: 'hbox',
                items:[
                ,{xtype:'textfield',
				fieldLabel: '热点MAC',
				labelWidth: 90,
				emptyText: '',
				width:300,
				name: 'mac'
				}           
			         ]
			         },
				{ xtype: 'fieldcontainer',
				layout: 'hbox',
                 items:[
				 ,{xtype:'textfield',
				 fieldLabel: '&nbsp;联系人',
				 labelWidth: 90,
				 emptyText:'请输入联系人',
				 width:300,
				 name: 'ISPname',
				 afterLabelTextTpl:required
				 }  
			         ]
				},{ xtype: 'fieldcontainer',layout: 'hbox',
                    items:[
                ,{xtype:'textfield',
				fieldLabel: '联系电话',
				labelWidth: 90,
				emptyText: '请输入',
				width:300,
				name: 'ISPphone',
				allowBlank:false,
				afterLabelTextTpl:required,
				 msgTarget:'qtip'
				}      
			         ]
			         },{ xtype: 'fieldcontainer',layout: 'hbox',
                    items:[
                ,{xtype:'textfield',
				fieldLabel: '运营机构邮箱',
				labelWidth: 90,
				name: 'ISPemail',
				emptyText: '请输入邮箱',
				regex: /^[\w-]+(\.[\w-]+)*@[\w-]+(\.[\w-]+)+$/,
　　			regexText:'请输入正确的邮箱',
				afterLabelTextTpl:required,
				 msgTarget:'qtip',
				width:300
				}   
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
	 
	 if(this.field2&&this.field2!=''){ 
		tform.getForm().findField('oldssid').setValue(this.field2);
	}
    
     if(this.field1&&this.field1!=''){    	
        tform.getForm().loadRecord(this.field1);          
     }

  }
});  
