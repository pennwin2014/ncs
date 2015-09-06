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
       this.items= [
                    {       
                        xtype: 'fieldcontainer',         
                        layout: 'hbox',
                        defaults: {
    //                      flex: 1,
                            labelWidth: 65,
                            hideLabel: false
                        },
                       items: [
                       {              
                          fieldLabel:'起始时间',
                          xtype: 'datefield',
                          format: "Y/m/d",
                          width:160,
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
                            labelWidth: 65,
                            hideLabel: false
                        },
                       items: [
                       {              
                          fieldLabel:'结束时间',
                          xtype: 'datefield',
                          format: "Y/m/d",
                          width:160,
                          value:today,
                          name:'edate'
                      },
                                          
                      {xtype: 'textfield',fieldLabel: '', name: 'etime', value:'23:59:59', width: 60},
                    ]}
      
                       ];
                       this.buttons=[ {
                        text:'查询',
                        handler: function()
                        {
                          fp2.getForm().reset();
                        }
                      } ,{
                        text:'清除',
                        handler: function()
                        {
                          fp2.getForm().reset();
                        }
                      }];
                        this.callParent(arguments);
    
  }
});
