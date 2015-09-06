
     
 
    
        			
        			       			
Ext.define('proauthApset.view.common.AddWin' ,{
  extend: 'Ext.window.Window',
  alias : 'widget.addwin',

	title: '接警人管理编辑',
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
     
     
    
	
	
	
	 Ext.define('State', {
        extend: 'Ext.data.Model',
        fields: [
            {type: 'string', name: 'groupid'},
            {type: 'string', name: 'groupname'}
        ]
    });
	
	var store_fac = Ext.create('Ext.data.Store', {
        model: 'State',
        remoteSort: true,
         proxy: {
            type: 'ajax',
            url: '/pronline/Msg?FunName@ncsWebFacComp',
            reader: {
                type:'json',
                root: 'corpclassdata'              
            },         
            simpleSortMode: true
        }
    });
	
    
  	this.items = [
  	{
  	     xtype: 'form',frame:true,width:400, bodyPadding:5,
  	        items: [
  	        {xtype:'hidden',id:'sid',name:'sid',value:''},
            {
  	           xtype: 'fieldcontainer',layout: 'vbox',            
               items:[
			   { xtype: 'fieldcontainer',layout: 'hbox',
                items:[
                {xtype:'textfield',fieldLabel: '姓名',labelWidth: 90,emptyText:'输入姓名',width:280,name: 'name',afterLabelTextTpl:required}   
			         ]
			         },
					 { xtype: 'fieldcontainer',layout: 'hbox',
                items:[
                ,{xtype:'textfield',fieldLabel: '所属部门',labelWidth: 90,emptyText:'输入部门',width:280,name: 'department'}   
			         ]
			         },{ xtype: 'fieldcontainer',layout: 'hbox',
                items:[
               
                ,{xtype:'textfield',fieldLabel: '&nbsp;联系人电话',labelWidth: 90,emptyText:'输入电话',width:280,name: 'phone'}
			         ]
			         },
					  { xtype: 'fieldcontainer',layout: 'hbox',
                items:[
                {xtype:'textfield',fieldLabel: '联系邮箱',labelWidth: 90,emptyText:'输入邮箱',width:280,name: 'email',afterLabelTextTpl:required}   
			         ]
			         }
			         
			       ]
			     }
			     ],
				buttonAlign:'center',
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
