Ext.define('proauthMac.view.list.List' ,{
  extend: 'Ext.grid.Panel',
  alias : 'widget.maclist',
  store : 'List', 
  height:parent.grid_height,
  autoScroll: true,
  title:'',
  columnLines: true,
  initComponent: function() {
    var sm = Ext.create('Ext.selection.CheckboxModel',{
        listeners: {
            selectionchange: function(sm, selections) {  
            	var grid=Ext.ComponentQuery.query('maclist')[0];
                  
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
               emptyText:'帐号/MAC/场所代码',
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
      {header: 'Mac地址', dataIndex: 'mac', width:120, sortable: true},
      {header: '手机号',dataIndex: 'mobile',width:120, sortable: false},
      {header: '姓名', dataIndex: 'dispname', width:120,sortable: false},
      {header: '最近位置', dataIndex: 'position', width:120,sortable: true},
      {header: '最近使用时间', dataIndex: 'lasttime',   width:120},
      {header: '最近上网场所', dataIndex: 'servicecode',   width:120},
      {header: '证件类型', dataIndex: 'idtype',   width:70,
      	 renderer:function(value,metaData,record){
          if(value=='111'){return '身份证'}
          else if(value=='112'){return '临时身份证'}
          else if(value=='114'){return '军官证'}
          else if(value=='123'){return '警官证'}
          else if(value=='116'){return '暂住证'}
          else if(value=='133'){return '学生证'}
          else if(value=='335'){return '机动车驾驶证'}
          else if(value=='411'){return '外交护照'}
          else if(value=='412'){return '公务护照'}
          else if(value=='414'){return '普通护照'}
          else if(value=='511'){return '台胞证'}
          else {return ''}
        }},
      {header: '证件号码', dataIndex: 'idno',   width:160},
      {header: '加入时间', dataIndex: 'addtime',   width:120},
      {header: '数据来源', dataIndex: 'fcode',   width:60},
      {flex:1}
    ];

    this.callParent(arguments);
  }
    
});