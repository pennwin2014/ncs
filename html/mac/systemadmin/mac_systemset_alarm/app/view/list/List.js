Ext.define('proauthApset.view.list.List' ,{
  extend: 'Ext.grid.Panel',
  alias : 'widget.apsetlist',
  store : 'List', 
  height:parent.grid_height,
  width:parent.grid_width,
  autoScroll: true,
  title:'',
  id:'id_store_list',
  columnLines: true,
  initComponent: function() {
    var sm = Ext.create('Ext.selection.CheckboxModel',{
        listeners: {
            selectionchange: function(sm, selections) {  
            	var grid=Ext.ComponentQuery.query('apsetlist')[0];
                  
             // grid.down('#removeButton').setDisabled(selections.length == 0);
             // grid.down('#shButton').setDisabled(selections.length == 0);
                
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
                  {xtype:'combo',
					mode:'local',                          
                    triggerAction:  'all',
                    forceSelection: true,
					fieldLabel:'上报时间',
					labelWidth: 60,
                    width:120,
                    editable:       false,   
                    id:'time_flag',                           
                    name:'time_flag',
                    displayField:'name',
                    valueField:'value',
                    queryMode:'local',
                    store: Ext.create('Ext.data.Store', 
                    {
                        fields : ['name', 'value'],
                        data   : 
                        [   {name:'全部',value:'100'},
                            {name : '今天',   value: '1'},
                            {name : '最近三天',  value: '2'},
                            {name : '最近一周',  value: '3'},
                            {name : '最近一月',   value: '4'},
                            {name : '最近三月',  value: '5'},
                            {name : '最近半年',  value: '6'}
                        ]
                    }),
                    emptyText:'全部',allowBlank:true,
					listeners: {
					'change':function(){
			        document.getElementById('form1').time_flag.value=this.value;
					reflash();
					}
					}
                },'-',
				{
					xtype:'combo',
					mode:'local',                          
                    triggerAction:  'all',
                    forceSelection: true,
					fieldLabel:'告警类型',
					labelWidth: 58,
                    width:140,
                    editable:false,   
                    id:'Alarm_reason',                           
                    name:'Alarm_reason',
                    displayField:'name',
                    valueField:'value',
                    queryMode:'local',
					typeAhead: true,
                    store: Ext.create('Ext.data.Store', 
                    {
                        fields : ['name', 'value'],
                        data   : 
                        [
						    {name : '全部',  value: '3'},  
                            {name : '设备离线',  value: '1'},
                            {name : '场所离线',  value: '2'}
                           
                        ]
                    }),
                    emptyText:'全部',
					allowBlank:true,
					listeners: {
					'change':function(){
						document.getElementById('form1').Alarm_reason.value=this.value;
						SetPage();
						reflash();
					}
					}
                },'-',
				{
					xtype:'combo',
					mode:'local',                          
                    triggerAction:'all',
                    forceSelection: true,
					fieldLabel:'告警状态',
					labelWidth: 58,
                    width:140,
                    editable:false,   
                    id:'isAlarm',                           
                    name:'isAlarm',
                    displayField:'name',
                    valueField:'value',
                    queryMode:'local',
					typeAhead: true,
                    store: Ext.create('Ext.data.Store', 
                    {
                        fields : ['name', 'value'],
                        data   : 
                        [
							{name : '全部',  value: '3'}, 
                            {name : '待确认',  value:'0'},
                            {name : '已确认',  value:'1'}
                            
                        ]
                    }),
                    emptyText:'全部',
					allowBlank:true,
					listeners: {
						'change':function(){
							document.getElementById('form1').isAlarm.value=this.value;
							reflash();
						}
					}
                }, {
               xtype:'textfield',
               id:'keyword_ywgj',
               width:160,
               name:'keyword_ywgj',
               style:'color:#7aa7d5',             
               emptyText:'告警类型/描述',
               enableKeyEvents:true,
               listeners:{
                 'focus':function(){
                 },
                 'keydown' : function(i,e){
                var aae=e.getKey() ; 
				var store = Ext.ComponentQuery.query('apsetlist')[0].getStore();
                if(aae==13){
					 var aa=Ext.getCmp('keyword_ywgj').value;
					document.getElementById('form1').keyword.value=aa;
                      reflash();
                }
               }
               }
             },
             {
                text:'查询',
                iconCls:'accept',
				handler:function(){
				var aa=Ext.getCmp('keyword_ywgj').value;
                if(aa!='告警类型/描述'){
					document.getElementById('form1').keyword.value=aa;
					 reflash();
				}				
					
				}
            },'-',{
                text:'批量确认',
                iconCls:'all_accept',
				handler:function(){
					var store = Ext.ComponentQuery.query('apsetlist')[0].getStore();
					 if(confirm('真的要执行批量确认操作吗？')){                  
                 var selsid;  
                 var rows=sm.getSelection();
                   selsid='';
                  for(var i=0;i<rows.length;i++){
                  	if(i==0){
                  	// selsid=rows[i].getId();
					 selsid="\'" + rows[i].get('sid') + "\'";
                  	}
                  	else{
                      selsid="\'" + rows[i].get('sid') + "\'"+ ","+selsid;
                    }
                   
                   }
                
                    var aa=Ext.getCmp('keyword_ywgj').value;
                     if(aa=='高级类型/描述'){
                     		aa='';
                     }
					 //alert(selsid);
                      store.load({params: {handle:"handle",sid:selsid,keyword:aa,status:1}});
					   reflash();
					
              }
					
				}
            },
 
     	 {xtype:'label', html:'<span id="titledx"></span>'}
     	]
    }
    );
    
    
   
  	
    this.columns = [
         {
			header: 'sid', 
			dataIndex: 'sid',
			width:170,
			sortable:true,
			hidden:true,
			//value:'暂时为空'
		},{
			header:"是否确认告警",
			dataIndex: 'status',
			icon:'/newver/resources/themes/images/share/queren.jpg',
			renderer: function (value, metaData, record){
				var sid=record.get('sid');
				if (value ==0){ 
					return '<input type="button"  value="确认" style="background:#00B1F1; color:#FFF; width:60px;" onclick="status_change('+sid+')"/>'
				}else{ 
					return  '<input disabled="disabled" type="button" value="确认" style="background:#CBDBEF; color:#FFF; width:60px;" />' 
				}					
            }			
		},
	/*修改时间：2015-9-15 10:16:31			
		{
			header: '告警类型', 
			dataIndex: 'alarmcode',
			width:170,sortable: true,
			value:'暂时为空'
		},
		*/
		{header: '告警类型',	//告警名称
		dataIndex: 'alarmcode',
		width:170, 
		sortable: true,
		renderer:function(value,metaData,record)
			{
				if(value=='10007'){return '<font color=black>场所离线告警</font>'}
				else if(value=='10008'){return '<font color=black>场所离线告警清除</font>'}
				else if(value=="10011"){return '<font color=black>设备离线告警</font>'}
				else if(value=="10012"){return '<font color=black>设备离线告警清除</font>'}
				else {return '<font color=black>其他</font>'}
			}
		},
		{
			header: '告警级别',   
			dataIndex: 'alarmlevel',
			width:170, 
			sortable: true,
			renderer:function(value,metaData,record)
			{
				if(value=='1'){return '<font color=black>紧急</font>'}
				else if(value=='2'){return '<font color=black>验证</font>'}
				else {return '<font color=black>一般</font>'}
			}
		},
		/*
		{
			header: '告警来源',  		
			dataIndex: 'devtype',
			width:170, 
			sortable: false,
			renderer:function(value,metaData,record)
			{
				if(value=='1'){return '<font color=black>AP</font>'}
				else if(value=='2'){return '<font color=black>场所</font>'}
				else if(value=='3'){return '<font color=black>管理中心</font>'}
				else if(value=='4'){return '<font color=black>上级平台</font>'}
				else {return '<font color=black>其他</font>'}
			}		
		},*/
		
	    {header: '告警来源', dataIndex: 'description',  width:170,renderer:descripwin1},		//告警描述
       {header: '告警上报时间', dataIndex: 'alarmtime',   width:170},
       {header: '告警更新时间',dataIndex: 'updatetime',width:170, sortable: false}
		
    ];

    this.callParent(arguments);
  }
});  

function descripwin1(value, p, record){
	//var sid=record.get('sid');
	var str = Ext.String.format('<a href="#" style="color:blue" onclick="detailWin(\'{1}\')">{0}</a>',value,value);
	
	return str;
}

function detailWin(value){
	//获取参数：
	//var rows = Ext.getCmp('id_store_list').getSelectionModel();
	//var sid = rows.selected.items[0].data.sid;

	
	var itemRush=[];
	var username = "";			//场所编码
	var userDispname = "" ;		//场所名
	var groupname = "";			//行政区域
	var address = "";			//场所地址
	var totalCount = 0;			//设备总数
	var data =[] ;
	var jyxz;					//经营性质
	var groupcode ="";			//1->经营性、 2->非经营性、 3->无线采集前端
	var status;					//在线、离线
	var statuscode;				//1->在线、 2->离线

	Ext.Ajax.request({
			url: '/pronline/Msg?FunName@ncsSysystem_alarmOne&dispname@'+value, 
			method: 'GET',
			success: function(resp,opts) {
				try{
					var respRecord =  resp.responseText;
					var record = eval("("+respRecord+")");	//字符串转成结构体	
									
					
					username = record.username;
					userDispname = record.userDispname;
					groupname = record.groupname;
					address = record.address;
					groupcode = username[6];
					
					totalCount = record.totalCount;
					data = record.data;
					//判断经营性质
					
					switch(groupcode){
						case "1":
							jyxz = "经营性";
							break;
						case "2":
							jyxz = "非经营性";
							break;
						case "3":
							jyxz = "无线采集前端";
							break;
						default:
							jyxz = "其他";
							break;
					}
					if(address==""){
						address='无';
					}
					
					itemRush.push(
						{html:'<a align="left"><b>行政区域：</b></a>' + '<a align="right">'+ groupname+'</a>'},
						{html:'<a align="left"><b>经营性质：</b></a>' + '<a align="right">' + jyxz+'</a>'},
						{html:'<a align="left"><b>场所名称：</b></a>' + '<a align="right">'+ userDispname+'</a>'},
						{html:'<a align="left"><b>场所编码：</b></a>' + '<a align="right">' + username+'</a>'},
						{html:'<a align="left"><b>场所地址：</b></a>'},{html:'<a align="left">'+address+'</a>'},
						{html:'<a align="left"><b>关联设备列表：</b></a>'},{html:'<a align="left">'+totalCount+'</a>'}
					);
		
					for( var i=0; i< totalCount; i++){
						statuscode = data[i].statuscode;
						if(statuscode =='1'){
							status ='<font color=green>在线</font>';
						}else if(statuscode =='2'){
							status ='<font color=red>离线</font>';
						}
						itemRush.push({html:'<a align="left">'+data[i]['aplistDispname']+'</a>'}, 		//设备名称
							{html:'<a align="left">'+status+'</a>'}
						);
					}
					createDetailWin(itemRush);
				}catch(e){
					//console.log(e.message);
				}	
			},   
			failure: function(resp,opts) {   
				var respText = eval("("+respText+")");
				alert('错误', respText.error);   
			}   				 
		});
	function createDetailWin(itemRush){	
		//创建弹窗
		var mytable = Ext.create('Ext.panel.Panel', {
			//title: '基础信息',
			layout: {
				type: 'table',
				columns: 2,		//列数
				tableAttrs:{
					width:'100%'
				}
			},
			defaults: {
				bodyStyle: 'padding:5px',			
			},
			items:itemRush
		});
		
		var mywin = Ext.create('Ext.window.Window',{
			title:'基础信息',
			height:mytable.height,
			width:350,
			x:30,
			y:30,
			resizable:false,
			items:mytable
		});
		mywin.show();
	}
}
	

   
