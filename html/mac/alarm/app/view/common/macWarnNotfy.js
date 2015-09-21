
var store_warnman = Ext.create('Ext.data.Store',{pageSize: 40,fields: ['nname', 'phone','email'],idProperty: 'userid',autoLoad: true,remoteSort: true,
        		proxy: {type: 'ajax',url: '/pronline/Msg?FunName@ncsMacWarnmanComp',reader: {type:'json',root: 'eimdata',totalProperty: 'totalCount'},simpleSortMode: true
        		},sorters:[{property: '',direction: 'DESC'}]});
Ext.define('proauthMobileAccount.view.common.macWarnNotfy' ,{
  extend: 'Ext.grid.Panel',
  alias : 'widget.macwarnnotfylist',
//  store : this.field2, 
  store:'WarnStore',
  height: 900,
  title:'�澯��ʽ',
  field1: this.field1,
  columnLines: true,
  initComponent: function() {
    var warnstore=Ext.data.StoreMgr.lookup('WarnStore'); 
    warnstore.load();

    var sm = Ext.create('Ext.selection.CheckboxModel',{
        listeners: {
            selectionchange: function(sm, selections) {  
          	var grid=Ext.ComponentQuery.query('macwarnnotfylist')[0];
                  
           grid.down('#removeButton3').setDisabled(selections.length == 0);
    //          grid.down('#moveGroupButton').setDisabled(selections.length == 0);
    
                
            }
        }
    
    });
    this.selModel=sm;

      var rowEditing1 = Ext.create('Ext.ux.grid.plugin.RowEditing', {
    	      pluginId:'rowEditing2', 
            saveBtnText: "����", 
            cancelBtnText: "ȡ��", 
            autoRecoverOnCancel:true,
            autoCancel: true,
            filed1:'sid',
            filed2:'name',
            filed3:'waddr',
            filed4:'wtype'
 //       clicksToMoveEditor: 1,
 //       autoCancel: false
    });
    
   
    this.plugins=rowEditing1;
    this.tbar = Ext.create('Ext.PagingToolbar',{
    	store: 'WarnStore',
      displayInfo: true,
      displayMsg: '��ʾ{0} - {1}����¼ ��{2}����¼',
     emptyMsg: "û�м�¼����ʾ",
     items:[
             
         '-',
     	 {
     	 	text:'����',
     	 	iconCls:'add',
     	 	 handler : function() {
                rowEditing1.cancelEdit();    
                  
                 var r = Ext.create('proauthMobileAccount.model.macModel', {  
                 	  wtype:'2',             	  
                    name: ''     
                });
               var store = Ext.ComponentQuery.query('macwarnnotfylist')[0].getStore();

              store.insert(0, r);
               rowEditing1.startAdd1(0,0);
            
       //     rowEditing.startEdit(0, 0);
              }
     	 	
     	 	
//     	 	action:'add'
     	},'-',
     	 {
     	 	text:'ɾ��',
     	 	itemId: 'removeButton3',
     	 	iconCls:'remove',
     	 	disabled: true,
     	 	action:'del'
     	},'-'
     	]
    }
    );
    
    
   
  	
    this.columns = [
      {header: '�澯��ʽ', dataIndex: 'wtype', width:100, sortable: true,
      	renderer:function(value,metaData,record){
      		if(value=='2'){return '����'}
                else  if(value=='1'){return '�ʼ�'}
					   
      	},
      	editor: {
          xtype: 'combo',
          id:'warntype',
          store: Ext.create('Ext.data.Store', {
                                    fields : ['classname', 'classid'],
                                    data   : [
                                   {"classid":"1","classname":"�ʼ�"},
                                   {"classid":"2","classname":"����"}                                                                     
                                    ]
            }),
           displayField: 'classname',
           valueField:'classid',
            queryMode: 'local',
				   value:'2',
           allowBlank: true,
              listeners:{
           	"change":function(){
           		var record=Ext.getCmp('nname').getStore().findRecord('nname', Ext.getCmp('nname').value).getData(); 
               var ttype=this.value;
               	var me=Ext.getCmp('phonemail');
               if(ttype=='2'){
               
               	me.setValue(record.phone);
              }
              else{
              	me.setValue(record.email);
              }
           		
           	}
          }
    
            }
      	
      	
      	},
      {header: '����',dataIndex: 'name',width:160, sortable: false,
      	  editor: {
      	  id:'nname',
      	  emptyText:'�����ѡ��澯��',
          xtype: 'combo',
          store: store_warnman,
           displayField: 'nname',
           valueField:'nname',
            queryMode: 'local',
				   value:'',
           allowBlank: true,
           listeners:{
           	"change":function(){
           		var record=this.getStore().findRecord('nname', this.getValue()).getData(); 
               var ttype=Ext.getCmp('warntype').value;
  
               	var me=Ext.getCmp('phonemail');
               if(ttype=='2'){
               
               	me.setValue(record.phone);
              }
              else{
              	me.setValue(record.email);
              }
           		
           	}
          }
    
            }
      	
      	
      	},
      {header: '�ֻ���/�����ַ',dataIndex: 'waddr',width:160, sortable: false,editor: {id:'phonemail',emptyText:'ѡ��״̬',allowBlank:true}}
     
      
      
    ];

    this.callParent(arguments);
  }
    
});