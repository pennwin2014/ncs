Ext.define('proauthRzAuthlog.view.common.UpdateWin' ,{
  extend: 'Ext.window.Window',
  alias : 'widget.upwin',

	title: null,
	modal: true,
	field1: null,
	field2: null,
	value1:null,

  initComponent: function() {
  	 var required='<span style="color:red;font-weight:bold" data-qtip="Required">*</span>';  	 
  	   var myDate = new Date();
      var yy = ( myDate.getYear() < 1900 ) ? ( 1900 + myDate.getYear() ) : myDate.getYear();
      var mm=myDate.getMonth()+1;
      if(mm<10)
      {
        mm='0'+mm;
      }
      var dd=myDate.getDate();
      if(dd<10)
      {
        dd='0'+dd;
      }
      var today=yy+'/'+mm+'/'+dd;

  	   var actionCombo = Ext.create('Ext.form.field.ComboBox', {
  	   	fieldLabel:'分析行为模式',
        labelWidth: 100,
        width:250,
        allowBlank:false,
        afterLabelTextTpl: required,
        valueField:'value',
        displayField: 'name',
        id:'actions',
        name : 'actions',      
        value:'0',
        allowBlank: true,
        forceSelection: false,
          store: Ext.create('Ext.data.Store', {
           fields : ['name', 'value'],
            data   : [
             {name : '轨迹重合',   value: '0'}
                       ]
                                }) ,
        queryMode: 'local',
        typeAhead: true,
                listeners: {
                      'change':function(){
                      
                        }
                    }
        });
         var groupCombo = Ext.create('Ext.form.field.ComboBox', {
  	   	fieldLabel:'终端分组',
        labelWidth: 100,
        width:250,
        allowBlank:false,
        valueField:'value',
        displayField: 'name',
        id:'group',
        name : 'group',  
        afterLabelTextTpl: required,    
        value:'0',
        allowBlank: true,
        forceSelection: false,
          store: Ext.create('Ext.data.Store', {
           fields : ['name', 'value'],
            data   : [
             {name : '组名',   value: '0'},
             {name : '描述',   value: '1'}
           
                       ]
                                }) ,
        queryMode: 'local',
        typeAhead: true,
                listeners: {
                      'change':function(){
                      
                        }
                    }
        });
  	   var freCombo = Ext.create('Ext.form.field.ComboBox', {
  	   	fieldLabel:'分析行为模式',
        labelWidth: 100,
        width:250,
        valueField:'value',
        displayField: 'name',
        afterLabelTextTpl: required,
        id:'freq',
        allowBlank:false,
        name : 'freq',      
        value:'0',
        allowBlank: true,
        forceSelection: false,
          store: Ext.create('Ext.data.Store', {
           fields : ['name', 'value'],
            data   : [
             {name : '每日一次',   value: '0'},
             {name : '每周一次',   value: '1'},
             {name : '每月一次',   value: '2'},
             {name : '自定义',   value: '3'}
                       ]
                                }) ,
        queryMode: 'local',
        typeAhead: true,
                listeners: {
                      'change':function(){
                      
                        }
                    }
        });


    this.items = [{
      xtype: 'form',frame:true,
      
      items:[
                 {
                 	xtype:'fieldset',
                 	layout:'vbox',
                 	width:325,
                 	hight:500,
                 	
                 	items:[
                 	{
								xtype:'textfield',
								fieldLabel:'任务名称',
								id:'taskname',
								name:'taskname',
								value:''	,
								width:250,
								afterLabelTextTpl: required,
								allowBlank:false, //是否允许为空
						    blankText:'任务名称不能为空！',	
						    msgTarget:'qtip' //显示一个浮动的提示信息	
						},{
							afterLabelTextTpl: required,
								xtype:'textfield',
								fieldLabel:'任务描述',
								id:'taskdesc',
								name:'taskdesc',
								
								width:250,
								value:''		
						},
						{
							afterLabelTextTpl: required,
		             width:255,
								xtype:'textfield',
								fieldLabel:'cid',
								allowBlank:false,
								id:'cid',
								hidden:true,
								name:'cid',
								value:''
								
						},
				groupCombo,actionCombo,
						freCombo,
						{       
                        xtype: 'fieldcontainer',         
                        layout: 'hbox',
                        defaults: {
    //                      flex: 1,
                            labelWidth: 100,
                            hideLabel: false
                        },
                       items: [
                       {      
                       	afterLabelTextTpl: required,
                       	allowBlank:false,        
                          fieldLabel:'执行时间',
                          xtype: 'datefield',
                          format: "Y/m/d",
                          width:250,
                          value:today,
                          name:'sdate'
                      }                    
                      
                    ]}	]
						}			

				]
    }];
    
    this.buttons=[{
      text: '保存',
      action: 'edit'
    },{
      text: '取消',
      scope: this,
      handler: this.close
    }];
    
       this.callParent(arguments);
     var tform=this.query('form')[0];
       if(this.field1&&this.field1!=''){
       tform.getForm().loadRecord(this.field1);  
      //Ext.getCmp('img_cxyh').setSrc(this.field1.get('imgpath'));
     }
 
  }
});
