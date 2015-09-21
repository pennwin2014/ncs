Ext.define('proauthApset.view.list.List',{
  extend: 'Ext.grid.Panel',
  alias : 'widget.apsetlist',
  store : 'List', 
  height:parent.grid_height,
  width:parent.grid_width,
  autoScroll:true,
  title:'',
  columnLines:true,
  initComponent:function() {
    var sm = Ext.create('Ext.selection.CheckboxModel',{
        listeners: {
            selectionchange: function(sm, selections) {  
            	var grid=Ext.ComponentQuery.query('apsetlist')[0];
                  
              grid.down('#removeButton').setDisabled(selections.length == 0);
              grid.down('#shButton').setDisabled(selections.length == 0);
            }
        }
    
    });
    this.selModel=sm;
    this.tbar = Ext.create('Ext.PagingToolbar',{
    	store:'List',
      displayInfo: true,
      displayMsg: '��ʾ{0} - {1}����¼ ��{2}����¼',
     emptyMsg: "û�м�¼����ʾ",
     items:[
                  {
               xtype:'textfield',
               id:'keyword_dwgl',
               width:160,
               name:'keyword5',
               style:'color:#7aa7d5',             
               emptyText:'��������/��ַ',
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
                text:'��ѯ',
                iconCls:'accept',
                action:'keycx'
            },
         '-',
     	 {
     	 	text:'����',
     	 	iconCls:'add',
     	 	action:'add'
     	},'-',
     	 {
     	 	text:'ɾ��',
     	 	itemId: 'removeButton',
     	 	iconCls:'remove',
     	 	disabled: true,
     	 	action:'del'
     	},'-',
     	 {
     	 	text:'���',
     	 	itemId: 'shButton',
     	 	iconCls:'add',
     	 	disabled: true,
     	 	action:'sh'
     	}
     	,
     	 {xtype:'label', html:'<span id="titledx"></span>'}
     	]
    }
    );
    
    
   
  	
    this.columns = [
     {
          header: '�༭',
            xtype: 'actioncolumn',
            dataIndex:'userid',
             icon: '/images/v4/edit_task.png',
            tooltip: '�����ͼ������޸�AP��Ϣ���档',
            width:40,
            align: 'center',
            sortable: false,
            handler: function(grid, rowIndex, colIndex, actionItem, event, record, row) {
               proauthApset.controller.Procy.show_add(record,record.get('gwid'));
          }
        },
       {header: '��������',     dataIndex: 'dispname',width:120, sortable: true,flex:1},
      {header: '���Ҽ��',       dataIndex: 'name',width:120, sortable: false,hiddern:true,flex:1},
       {header: '������֯��������',       dataIndex: 'code',width:120, sortable: false,flex:1},
       {header: '����������',       dataIndex: 'famount',width:120, sortable: false,flex:1},
      {header: '�����豸��', dataIndex: 'APamount', width:170,sortable: false,flex:1},
     	 {header: '��ǰά��������', dataIndex: 'dqamount', width:160,sortable: true,flex:1},
		  {header: '����������', dataIndex: 'online', width:160,sortable: true,flex:1},
     	 {header: '���̵�ַ', dataIndex: 'saddress',  width:80,flex:1},
      {header: '��ϵ��', dataIndex: 'slink',   width:80,flex:1},
      {header: '��ϵ�绰', dataIndex: 'stel',   width:70,flex:1},
	   {header: '��ϵ������', dataIndex: 'smail',   width:70,flex:1}
	   /*,
      {header: '���״̬', dataIndex: 'flags',   width:70,
      	 renderer:function(value,metaData,record){
          if(value=='1'){return '<font color=green>����</font>'}
          else if(value=='0'){return '<font color=red>δ��</font>'};
          }
      	}
       
      */
    ];

    this.callParent(arguments);
  }
    
});