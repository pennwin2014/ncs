Ext.define('proauthRzAuthlog.view.common.AddGroup' ,{
  extend: 'Ext.window.Window',
  alias : 'widget.addgroup',

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
      var startday=yy+'/'+mm+'/'+dd;
  	 
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
								width:255,
								afterLabelTextTpl: required,
								allowBlank:false, //是否允许为空
						    blankText:'任务名称不能为空！',	
						    msgTarget:'qtip' //显示一个浮动的提示信息	
						},{
								xtype:'textfield',
								fieldLabel:'任务描述',
								id:'taskdesc',
								name:'taskdesc',
								width:255,
								value:''		
						},{
								xtype:'textfield',
								fieldLabel:'主mac信息',
								id:'mac',
								name:'mac',
								width:255,
		//						value:'[#name#]',
								afterLabelTextTpl: required,
								allowBlank:false, //是否允许为空
						    blankText:'主mac信息不能为空！',	
						    msgTarget:'qtip' //显示一个浮动的提示信息 		
						},{
	
								
								    fieldLabel:'伴随分析场所',
	                                labelWidth:100,    
									width:          250,
									xtype:          'treecombox',
									mode:           'local',
									value:          '0',
									triggerAction:  'all',
									forceSelection: true,
									iconCls:'add',
									editable:       false,
								
									iconCls:'remove',
									emptyText:'选择单位查询',
									name:           'groupname',
									id:           'groupname',
									displayField:   'text',
									valueField:     'id',
									queryMode: 'local',
									value : '',
									store:store_classtree
							
				},
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
                          fieldLabel:'起始时间',
                          xtype: 'datefield',
                          format: "Y/m/d",
                          width:195,
                          value:startday,
                          name:'sdate',
                           id:'sdate'
                      },                    
                      {xtype: 'textfield',fieldLabel: '',  id:'stime',name: 'stime', value:'00:00:00', width: 60},
                    ]},
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
                          fieldLabel:'结束时间',
                          xtype: 'datefield',
                          format: "Y/m/d",
                          width:195,
                          value:today,
                          name:'edate',
                           id:'edate'
                      },
                                          
                      {xtype: 'textfield',fieldLabel: '', name: 'etime', id:'etime', value:'23:59:59', width: 60},
                    ]},
		                  {
		              width:255,
								xtype:'textfield',
								fieldLabel:'重复出现次数',
								id:'servcount',
								name:'servcount',
								value:''
								
						},{
								xtype:'textfield',
								fieldLabel:'伴随Mac个数',
								id:'maccount',
								name:'maccount',
								width:255,
								value:''
								
						}	]
						}			

				]
    }];
    
    this.buttons=[{
      text: '保存',
      action: 'adds'
    },{
      text: '取消',
      scope: this,
      handler: this.close
    }];
    
    this.callParent(arguments);
    
  }
});
