Ext.define('proauthApset.view.list.List' ,{
  extend: 'Ext.grid.Panel',
  alias : 'widget.apsetlist',
  store : 'List', 
  height:parent.grid_height,
  width:parent.grid_width,
  autoScroll: true,
  title:'',
  columnLines: true,
  initComponent: function() {
    var sm = Ext.create('Ext.selection.CheckboxModel',{
        listeners: {
            selectionchange: function(sm, selections) {  
            	var grid=Ext.ComponentQuery.query('apsetlist')[0];                  
              grid.down('#removeButton').setDisabled(selections.length == 0);
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
                  {
               xtype:'textfield',
               id:'keyword_dwgl',
               width:160,
               name:'keyword5',
               style:'color:#7aa7d5',             
               emptyText:'热点SSID/联系人',
               enableKeyEvents:true,
               listeners:{
                 'focus':function(){
                
                 },
                 'keydown' : function(i,e){
                var aae=e.getKey() ; 
                if(aae==13){
                    var aa=Ext.getCmp('keyword_dwgl').value;
					var store = Ext.ComponentQuery.query('apsetlist')[0].getStore();
					store.load({params:{keyword:aa}});  
                }
               }
               }
             },
             {
                text:'查询',
                iconCls:'accept',
                action:'keycx'
            },'-',{
     	 	text:'导入',
     	 	itemId: 'importButton',
     	 	iconCls:'exp',
     	 	action:'import'
     	},
         '-',
     	 {
     	 	text:'增加',
     	 	iconCls:'add',
     	 	action:'add'
     	},'-',
     	 {
     	 	text:'删除',
     	 	itemId: 'removeButton',
     	 	iconCls:'remove',
     	 	disabled: true,
     	 	action:'del'
     	},'-',
		{
                text:'导出',
				iconCls:'exportimg',
                handler:function(){
                document.getElementById('form1').export2.value='exp';
				document.getElementById('form1').limit.value='40';
				//document.getElementById('form1').page.value=Ext.getCmp("pagtool").store.currentPage;
				document.getElementById('form1').keyword.value=Ext.getCmp('keyword_dwgl').value;  
                document.getElementById("form1").submit();
               document.getElementById("form1").export2.value="";
                }
            }
     	,
     	 {xtype:'label', html:'<span id="titledx"></span>'}
     	]
    }
    );
    
    
   
  	
    this.columns = [
	
     {
          header: '编辑',
            xtype: 'actioncolumn',
            dataIndex: 'hotspotid',
            icon: '/images/v4/edit_task.png',
            tooltip: '点击该图标修改热点信息。',
            width: 40,
            align: 'center',
            sortable: false,
            handler: function(grid, rowIndex, colIndex, actionItem, event, record, row) {
                proauthApset.controller.Procy.show_add(record,record.get('ssid'));
          }
    
        },{	
			header: '是否合规',
            flex:1, 			
			dataIndex: 'authflag',
			width:80, 
			sortable: true,
			renderer:function(value,metaData,record){
			if(value=='1'){return '合规'}
			else {return '不合规'}
          }     	
      	},
		{header: '热点SSID', dataIndex:'ssid',width:100,sortable: true},			
		{
			header: '热点加密类型',
			dataIndex: 'security',
			flex:1, 
			width:120, 
			sortable: false,
			renderer:function(value,metaData,record)
			{
				if(value=='01'){return '<font color=black>WEB</font>'}
				else if(value=='02'){return '<font color=black>WPA</font>'}
				else if(value=='03'){return '<font color=black>WPA2</font>'}
				else if(value=='04'){return '<font color=black>PSK</font>'}
				else   {return '<font color=black>其他</font>'}
			}
		},
		{
			header: '场所服务类型',  
			dataIndex: 'servicetype',
			flex:1, 
			width:120, 
			sortable: true,
			renderer:function(value,metaData,record)
			{
				if(value=='1'){return '<font color=green>酒店宾馆</font>'}
				else if(value=='2'){return '<font color=green>图书馆阅览室</font>'}
				else  if(value=='3') {return '<font color=green>学校</font>'}
				else  if(value=='4') {return '<font color=green>娱乐场所</font>'}
				else  if(value=='5') {return '<font color=green>一般企业</font>'}
				else   {return '<font color=green>其他</font>'}
			}
		},
		{
			header: '场所经营性质',
			dataIndex: 'servicequlity',
			flex:1,
			width:100,
			renderer:function(value,metaData,record)
			{
				if(value=='0'){return '<font color=green>经营</font>'}
				else if(value=='1'){return '<font color=red>非经营</font>'}
				else   {return '<font color=red>其他</font>'}
			}
      	},  
			{
			header: '热点MAC', 
			dataIndex: 'mac',
			flex:1,
			width:80,
			renderer:proauthApset.controller.Procy.mac_search
			},
			{header: '联系人', dataIndex: 'ISPname',  flex:1,  width:80},
			{header: '联系电话',dataIndex: 'ISPphone',flex:1, width:120, sortable: false},  
			{header: '运营机构邮箱',dataIndex: 'ISPemail',width:120, flex:1, sortable: false}  
    ];

    this.callParent(arguments);
  }
    
});