Ext.define('proauthRzSmslog.view.list.List' ,{
  extend: 'Ext.grid.Panel',
  alias : 'widget.smsloglist',
  store : 'List', 
  height: 900,
  title:'',
  columnLines: true,
  initComponent: function() {
    var sm = Ext.create('Ext.selection.CheckboxModel',{
        listeners: {
            selectionchange: function(sm, selections) {  
            	var grid=Ext.ComponentQuery.query('authloglist')[0];
                  
     //         grid.down('#removeButton').setDisabled(selections.length == 0);
    //          grid.down('#moveGroupButton').setDisabled(selections.length == 0);
    
                
            }
        }
    
    });
    this.selModel=sm;

   
    this.tbar = Ext.create('Ext.PagingToolbar',{
    	store:'List',
      displayInfo: true,
      displayMsg: '显示{0} - {1}条记录 共{2}条记录',
     emptyMsg: "没有记录可显示",
     items:[
     {xtype:'combo',mode:'local',                          
                                	triggerAction:  'all',
                                	forceSelection: true,
                                	width:140,
                                	editable:       false,   
                                	id:             'time_flag',                           
                                	name:           'time_flag',
                                	displayField:   'name',
                                	valueField:     'value',
                                	queryMode: 	'local',
                               	 	store: Ext.create('Ext.data.Store', 
                               	 	{
                                    		fields : ['name', 'value'],
                                    		data   : 
                                    		[
                                        		{name : '今天',   value: '1'},
                                        		{name : '最近三天',  value: '2'},
                                        		{name : '最近一周',  value: '3'},
                                        		{name : '最近一月',   value: '4'},
                                        		{name : '最近三月',  value: '5'},
                                        		{name : '最近半年',  value: '6'}
                                        		
                                    		]
                                	}),
emptyText:'今天',allowBlank:true},
     
              {
               xtype:'textfield',
               id:'keyword_mobileaccount',
               width:140,
               name:'keyword5',
               style:'color:#7aa7d5',             
               emptyText:'手机号/MAC/场所代码',
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
         '-',
     	
     	 {xtype:'label', html:'<span id="titledx"></span>'}
     	]
    }
    );
    
    
   
  	
    this.columns = [
      {header: '手机号', dataIndex: 'mobile', flex:1, sortable: true},
      {header: '地区',dataIndex: 'acode',width:100, sortable: false},
      {header: '时间', dataIndex: 'stime', flex:1,sortable: false},
      {header: 'MAC地址', dataIndex: 'mac', flex:1,sortable: true},
      {header: '产品序列号', dataIndex: 'sn',   flex:1,hidden:true},
      {header: '场所代码', dataIndex: 'servicecode',   flex:1},
       {header: '场所名称', dataIndex: 'servicename',   flex:1},
      {header: '短信服务商', dataIndex: 'sms',   flex:1},
      {header: '短信内容', dataIndex: 'message',   flex:1},
      {header: '发送状态', dataIndex: 'status',   flex:1,
      	      	renderer:function(value,metaData,record){
          if(value=='0'){return '成功'}
          else if(value=='1'){return '失败'}
          else {return ''}
        }},
      {header: '数据来源', dataIndex: 'fcode',   flex:1}
    ];

    this.callParent(arguments);
  }
    
});