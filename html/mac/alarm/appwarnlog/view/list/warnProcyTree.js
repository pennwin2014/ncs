/**
 定义单位树
 */
 
 Ext.define('proauthMobileAccount.view.list.warnProcyTree', {
    extend: 'Ext.tree.Panel',
    alias: 'widget.warnProcyTree',
    initComponent: function(){
  
		Ext.define('treedata', {
			extend: 'Ext.data.Model',
			fields: [
				{name: 'text',  type: 'string'},
				{name: 'id',    type: 'string'},
				{name: 'did', 	type: 'string'},
				{name: 'code',  type: 'string'}
	 
			]
		});

		store_corp = Ext.create('Ext.data.TreeStore', {
			model: 'treedata',
			proxy: {
				type: 'ajax',
				url: '/pronline/Msg?FunName@ncsTreeBkProcy_v4'
			},
			root: {
				text: '全部策略',
				id: '',
				expanded: true
			},
			folderSort: true,
			sorters: [{
				property: 'text',
				direction: 'ASC'
			}]
		});	
		/*
		store_corp.on('itemclick', function (node) {
			alert("hhhhh");
	 // 	alert(parent.wireflag);

	// 	  var new_params={wireflag:wireflag};
	//      Ext.apply(store.proxy.extraParams,new_params);
		   });	
		*/
		Ext.apply(this, {
			margins: '0 0 0 0',
			id:'groupTree',
			store: store_corp,
			autoScroll: true,
			border:false,
			forceFit: true,
			rootVisible: true,
	   //     width: 110,
			useArrows: false,
			dockedItems: [{
				xtype: 'toolbar',
				items: [
					{
						xtype:'textfield',
						id:'keyword_procytree',
						name:'keyword',
						style:'color:#7aa7d5',
						width:140,
						emptyText:'输入策略名称查询',
						enableKeyEvents:true,
						listeners:{
							'keydown' : function(i,e){
								var aae=e.getKey() ; 
								if(aae==13){
									var aa=Ext.getCmp('keyword_procytree').value;
									if(aa!=''){
										var vkeyword=Ext.getCmp('keyword_procytree').value;	
										alert(vkeyword);
										store_corp.load({params: {cname:vkeyword}});
									}
								}
							}
						}
					},{
						text:'查询',
						itemId: 'moveButton',
						iconCls:'accept',
		 //               disabled: true,
						id:"treeBtn",
						listeners: {
							'click': function(){
								var aa=Ext.getCmp('keyword_procytree').value;
								var vkeyword=Ext.getCmp('keyword_procytree').value;		         
								store_corp.load({params: {cname:vkeyword}});
							}
						}
					}
				]
			}],        
			listeners:{
				'itemclick':function(view, rcd, item, idx, event, eOpts)
				{
					var cid=rcd.get('id');
					if(cid == ""){
						cid = 'allWarn';
					}
					var dxtitle = rcd.get('text');	
					try{		
						proauthMobileAccount.controller.Procy.loadProcyListStore(cid);    									
					}catch(e){
						
					}
				}
			}
		});
		this.callParent(arguments);
    }
});