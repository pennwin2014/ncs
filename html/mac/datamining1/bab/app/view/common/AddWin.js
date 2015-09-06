Ext.define('proauthRzAuthlog.view.common.AddWin' ,{
  extend: 'Ext.window.Window',
  alias : 'widget.addwin',

	title: null,
	modal: true,
	field1: null,
	field2: null,
	value1:null,

  initComponent: function() {
  	 var required='<span style="color:red;font-weight:bold" data-qtip="Required">*</span>';

	  var typeCombo = Ext.create('Ext.form.field.ComboBox', {
	  	 fieldLabel:'商品小类名称',
//	  	 labelWidth:100,
//	  	 width:300,
	  	 valueField:'sid',
	  	 value:'',
	  	 displayField:'name',
	  	 id:'tyid',
	  	 name:'gtype',
	  	 afterLabelTextTpl: required,
	  	 allowBlank:false,
	     blankText:'商品小类不能为空！',	
	     msgTarget:'qtip', //显示一个浮动的提示信息 
	  	 store:store_type,
	  	 triggerAction:'all',
	  	 queryMode:'local',
	  	 typeAhead:true,
	  	 listeners:{
	  	 		'change':function(){
	  	 		}	
	  	 },
	  	 listConfig:{
	  	 		loadingText:'Searching...',
	  	 		emptyText:'No matching found.'	
	  	 }
	  });
  	 
  	 

	  var shopCombo = Ext.create('Ext.form.field.ComboBox', {
	  	 fieldLabel:'商店名称',

	  	 valueField:'id',
	  	 value:'',
	  	 displayField:'name',
	  	 id:'shid',
	  	 name:'shopid',
	  	 hidden:false,
	  	 afterLabelTextTpl: required,
	  	 allowBlank:false,
	     blankText:'商店名称不能为空！',	
	     msgTarget:'qtip', //显示一个浮动的提示信息 
	  	 store:store_shop,
	  	 triggerAction:'all',
	  	 queryMode:'local',
	  	 typeAhead:true,
	  	 listeners:{
	  	 		'change':function(){
	  	 		}	
	  	 },
	  	 listConfig:{
	  	 		loadingText:'Searching...',
	  	 		emptyText:'No matching found.'	
	  	 }
	  });
  	 
  	 	 var corpsCombo = Ext.create('Ext.form.field.ComboBox', {
	  	 fieldLabel:'商城名称',

	  	 valueField:'id',
	  	 value:'',
	  	 displayField:'name',
	  	 id:'cid',
	  	 name:'cid',
	  	 hidden:false,
	  	 afterLabelTextTpl: required,
	  	 allowBlank:false,
	     blankText:'商城名称不能为空！',	
	     msgTarget:'qtip', //显示一个浮动的提示信息 
	  	 store:store_comcorps,
	  	 triggerAction:'all',
	  	 queryMode:'local',
	  	 typeAhead:true,
	  	 listeners:{
	  	 		'change':function(){
	  	 		}	
	  	 },
	  	 listConfig:{
	  	 		loadingText:'Searching...',
	  	 		emptyText:'No matching found.'	
	  	 }
	  });
  	 
  	 
  	 
    this.items = [{
      xtype: 'form',frame:true,
      layout:'hbox',
      items:[
				{
						xtype:'fieldset',
						width:350,
						title:'基本信息',
						items:[{
								xtype:'textfield',
								fieldLabel:'增/改',
								id:'key',
								name:'key',
								hidden:true,
								value:''		
						},{
								xtype:'textfield',
								fieldLabel:'任务名称',
								id:'taskname',
								name:'taskname',
								hidden:true,
								value:''		
						},{
								xtype:'textfield',
								fieldLabel:'任务描述',
								id:'task',
								name:'task',
		//						value:'[#name#]',
								afterLabelTextTpl: required,
								allowBlank:false, //是否允许为空
						    blankText:'商品名称不能为空！',	
						    msgTarget:'qtip' //显示一个浮动的提示信息 		
						},{
								xtype:'textfield',
								fieldLabel:'显示名称',
								id:'dispname',
								name:'dispname'
					//			value:'[#dispname#]'		
						},{
								xtype:'textfield',
								fieldLabel:'商品型号',
								id:'type',
								name:'type'
					//			value:'[#type#]'		
						},
		
		 				typeCombo,
		 				corpsCombo,
	  				shopCombo,
		{
								xtype:'textfield',
								fieldLabel:'生产厂家',
								id:'factory',
								name:'factory',
								value:'',
								hidden:false
						},{
								xtype:'numberfield',
								fieldLabel:'原价',
								id:'fee',
								name:'fee',
								value:'',
								hidden:false,
								hideTrigger:false, //隐藏微调按钮
								decimalPrecision:2, //精确到小数点后两位
		        		allowDecimals:true, //不允许输入小数
		        		nanText:'请输入有效的小数' //无效数字提示	
						},{
								xtype:'numberfield',
								fieldLabel:'折扣',
								id:'discount',
								name:'discount',
								value:'',
								hidden:false,
								hideTrigger:true, //隐藏微调按钮
								decimalPrecision:2, //精确到小数点后两位
		        		allowDecimals:true, //不允许输入小数	
								emptyText:'请输入正确小数 0-1之间',
								msgTarget:'qtip', //显示一个浮动的提示信息
								maxText:'最大值不能大于1！',
								minText:'最小值不能小于0！', 	
								maxValue:1, //最大值 
        				minValue:0	//最小值
						},{
								xtype:'numberfield',
								fieldLabel:'库存',
								id:'reserve',
								name:'reserve',
								value:'',
								hidden:false,
								hideTrigger:false, //隐藏微调按钮
		        		allowDecimals:false, //不允许输入小数
		        		nanText:'请输入有效范围内的整数' //无效数字提示	
						},{
								xtype:'textfield',
								fieldLabel:'活动开始期限',
								id:'active',
								name:'active',
								value:''
						
						},
						{
			            	 
			                fieldLabel: '&nbsp;显示方式',
			                
                                xtype:          'combo',
                                mode:           'local',
                                value:          '1',
                                triggerAction:  'all',
                                forceSelection: true,
                                editable:       false,                                
                                name:           'showflag',              
                                displayField:   'name',
                                valueField:     'value',
                                queryMode: 'local',
                                store: Ext.create('Ext.data.Store', {
                                    fields : ['name', 'value'],
                                    data   : [
                                        {name : '一行显示一个商品',   value: '0'},
                                        {name : '一行显示三个商品',   value: '1'}
                                      
                                       
                                    ]
                                })			                
			            },
			         {
								xtype:'textfield',
								fieldLabel:'显示顺序',
								name:'num',
								value:'',
								hidden:false
						 }
						,{
								xtype:'textarea',
								fieldLabel:'简述',
								id:'descr',
								rows:2,
								name:'descr',
								hidden:false,
								value:''
						}]
				}
				
				]
    }];
    
    this.buttons=[{
      text: '保存',
      action: 'add'
    },{
      text: '取消',
      scope: this,
      handler: this.close
    }];
    
    this.callParent(arguments);
      var tform=this.query('form')[0];
   
     if(this.field1&&this.field1!=''){
       tform.getForm().loadRecord(this.field1);  
      Ext.getCmp('img_cxyh').setSrc(this.field1.get('imgpath'));
     }
    
    
  }
});
