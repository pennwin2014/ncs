
  

Ext.define('proauthApMacset.view.list.List' ,{
 extend: 'Ext.Panel',
  alias : 'widget.apmacsetlist',
	title: ' 场所离线短信告警设置',
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
                {fieldLabel: '&nbsp;启用场所离线短信告警功能',labelWidth: 170,width:240,afterLabelTextTpl:required,
                	             xtype:          'combo',
                                mode:           'local',
                              
                                triggerAction:  'all',
                                forceSelection: true,
                                editable:       false,                                
                                name:           'StartWarnOff',              
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
           	   xtype:'fieldset',
           title:'选择告警对象',
       //    layout: 'form',
        	 defaults: {
               labelStyle: 'padding-left:4px;'
        	 },
            collapsible: false,
            items: [{
              xtype: 'checkboxgroup',
                name:'rightgroup',
                columns: 1,
                items: [
                {boxLabel:'单位负责人(对应联网单位中的单位负责人电话)',inputValue:'1',checked:true,name:'fzrmobno'},
                {boxLabel:'单位安全员(对应联网单位中的信息安全员电话)',inputValue:'1',checked:true,name:'aqymobno'},
                {boxLabel:'网监负责人(对应联网单位中的网监负责人电话)',inputValue:'1',checked:true,name:'wjmobno'},
                {boxLabel:'必报手机',inputValue:'1',checked:true,name:'bbmobflag'},
                 {xtype:'textfield',fieldLabel: '&nbsp;多个手机号之间，以逗号分隔',labelWidth: 170,emptyText: '',width:450,name: 'bbmobno'}
              
                ]
            }]
            },
            { xtype: 'fieldcontainer',layout: 'hbox',
                items:[
                {xtype:'textfield',fieldLabel: '&nbsp;告警内容',labelWidth: 70,emptyText: '',width:350,name: 'warncont'}
                ,{
			            	  xtype     : 'displayfield',
			                value:'&nbsp;(告警内容不要超过50个字符)'
			            }
			         ]
			         },
           
			     
			     
			     
			     	
			     
			     ],
			   	buttons:[			  
			   	 {xtype : 'displayfield',value:'&nbsp;&nbsp;'}
			   	,{text:'保存',action: 'addsave'}
	   	,{text:'查看短信告警日志',handler: function() {
   		
                     window.open('/pronline/Msg?FunName@ncsShowOfflinelog');
                    }	
	
				 }
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