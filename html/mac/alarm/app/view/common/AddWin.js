            			       			
Ext.define('proauthMobileAccount.view.common.AddWin' ,{
  extend: 'Ext.window.Window',
  alias : 'widget.addwin',

	title: '布控策略编辑',
	modal: true,
	field1: null,
	field2: null,
	value1:null,
  initComponent: function() {

         var cid=this.field1.get('sid');
         var macstore=this.field2;
  //MAC关键字布控
         var macBk=	Ext.create('proauthMobileAccount.view.common.macKeywordBk',{field1: cid,field2:macstore					
					   });
					   
					//MAC阀值布控
         var macLimBk=	Ext.create('proauthMobileAccount.view.common.macLimBk',{field1: cid});   
					   
 //告警方式
         var warnNotfy=	Ext.create('proauthMobileAccount.view.common.macWarnNotfy',{field1: cid	 });				   
  	     var tabs = Ext.createWidget('tabpanel', {

        width: 858,
        height:510,
        activeTab: 0,
         defaults :{
            bodyPadding: 0
        },
        items: [
               macBk,macLimBk,warnNotfy
        ],
         listeners: {           
          'beforetabchange':function(tp, p){

           store_warnman.load(); 	

        }
        }
    });
    
  	this.items = [tabs];
  
    this.callParent(arguments);

   

  }
});  
