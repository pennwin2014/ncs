Ext.define('proauthMobileAccount.view.list.List' ,{
  extend: 'Ext.grid.Panel',
  alias : 'widget.mobileaccountlist',
  store : 'List', 
  height: 650,
  title:'',
  columnLines: true,
  initComponent: function() {
    var sm = Ext.create('Ext.selection.CheckboxModel',{
        listeners: {
            selectionchange: function(sm, selections) {  
            	var grid=Ext.ComponentQuery.query('mobileaccountlist')[0];
                  
              grid.down('#removeButton').setDisabled(selections.length == 0);
    //          grid.down('#moveGroupButton').setDisabled(selections.length == 0);
    
                
            }
        }
    
    });
    this.selModel=sm;

        var whCombo = Ext.create('Ext.form.field.ComboBox', {
        fieldLabel: '操作类型',
        labelWidth: 60,
        width:170,
        valueField:'value',
        displayField: 'name',
        id:'whflag',
        name : 'flag',      
        value:'',
        allowBlank: true,
        forceSelection: false,
                                    store: Ext.create('Ext.data.Store', {
                                    fields : ['name', 'value'],
                                    data   : [
                                     {name:"选择操作类型",value:""},
                                        {name : '维护',   value: '1'},
                                        {name : '取消维护',   value: '0'}
                                                                      
                                    ]
                                })
                                ,
        queryMode: 'local',
        typeAhead: true,
                listeners: {
			          'change':function(){
			             proauthMobileAccount.controller.Procy.loadProcyListStore();
			            }
			        }
    });
    
  //  this.plugins=rowEditing;
    this.tbar = Ext.create('Ext.PagingToolbar',{
    	store:'List',
      displayInfo: true,
      displayMsg: '显示{0} - {1}条记录 共{2}条记录',
     emptyMsg: "没有记录可显示",
     items:[
              whCombo,
              {
               xtype:'textfield',
               id:'keyword_mobileaccount',
               width:160,
               name:'keyword5',
               style:'color:#7aa7d5',             
               emptyText:'维护内容/场所名称/场所代码',
               enableKeyEvents:true,
               listeners:{
                 'focus':function(){
        
                 },
         
                 'keydown' : function(i,e){
                var aae=e.getKey() ; 
                if(aae==13){

                }
               }
               }
             },
             {
                text:'查询',
                iconCls:'accept',
                action:'keycx'
   
            },
         '-'
     ,
     	 {xtype:'label', html:'<span id="titledx"></span>'}
     	]
    }
    );
    
    
   
  	
    this.columns = [
      {header: '厂商名称', dataIndex: 'fname', width:120, sortable: false},
        {header: '操作类型', dataIndex: 'flag',  width:80,
      	         renderer:function(value,metaData,record){
                    if(value=='1'){return '<font color=red>维护</font>'}
                  
                    else {return '<font color=green>取消维护</font>'}
                }
      	
      	},
      {header: '时间',dataIndex: 'stime',width:120, sortable: false},
      {header: '场所代码', dataIndex: 'servicecode', width:120,sortable: false},
      {header: '场所名称', dataIndex: 'servicename', width:120,sortable: false},
      {header: '维护描述', dataIndex: 'desc', flex:1},

      {header: '维护人', dataIndex: 'wman',  width:60}
     

      
    ];

    this.callParent(arguments);
  }
    
});