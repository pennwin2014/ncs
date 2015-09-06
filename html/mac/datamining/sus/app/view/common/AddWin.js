Ext.define('proauthRzAuthlog.view.common.AddWin' ,{
  extend: 'Ext.window.Window',
  alias : 'widget.addwin',

	title: null,
	modal: true,
	field1: null,
	field2: null,
	value1:null,

  initComponent: function() {
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
  	var stylesCombo = Ext.create('Ext.form.field.ComboBox', {
  	   	fieldLabel:'加密方式',
        labelWidth: 75,
        width:185,
        valueField:'value',
        displayField: 'name',
        id:'styles',
        name : 'styles',      
        value:'0',
        allowBlank: true,
        forceSelection: false,
          store: Ext.create('Ext.data.Store', {
           fields : ['name', 'value'],
            data   : [
             {name : 'WEP',   value: '0'},
             {name : 'WPA',   value: '1'},
             {name : 'WPA2',   value: '2'},
             {name : 'PSK',   value: '3'},
             {name : '其他',   value: '4'}
           
                       ]
                                }) ,
        queryMode: 'local',
        typeAhead: true,
                listeners: {
                      'change':function(){
                      
                        }
                    }
        });
  	 
      var statusCombo = Ext.create('Ext.form.field.ComboBox', {
  	   	fieldLabel:'状态',
        labelWidth: 75,
        width:185,
        valueField:'value',
        displayField: 'name',
        id:'status',
        name : 'freq',      
        value:'0',
        allowBlank: true,
        forceSelection: false,
          store: Ext.create('Ext.data.Store', {
           fields : ['name', 'value'],
            data   : [
             {name : '非法',   value: '0'},
             {name : '合规',   value: '1'},
             {name : '可疑',   value: '2'}
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
                        xtype: 'fieldcontainer',         
                        layout: 'hbox',
                        defaults: {
    //                      flex: 1,
                            labelWidth: 75,
                            hideLabel: false
                        },
                       items: [
                       {              
                          fieldLabel:'起始时间',
                          xtype: 'datefield',
                          format: "Y/m/d",
                          width:175,
                          value:startday,
                          name:'sdate'
                      },                    
                      {xtype: 'textfield',fieldLabel: '', name: 'stime', value:'00:00:00', width: 60},
                    ]},
                     {       
                        xtype: 'fieldcontainer',         
                        layout: 'hbox',
                        defaults: {
    //                      flex: 1,
                            labelWidth: 75,
                            hideLabel: false
                        },
                       items: [
                       {              
                          fieldLabel:'结束时间',
                          xtype: 'datefield',
                          format: "Y/m/d",
                          width:175,
                          value:today,
                          name:'edate'
                      },
                                          
                      {xtype: 'textfield',fieldLabel: '', name: 'etime', value:'23:59:59', width: 60},
                    ]},
                
                 	stylesCombo,
                 	statusCombo
					
									

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
    
    
    
  }
});
