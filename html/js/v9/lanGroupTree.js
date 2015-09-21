/**
 定义单位树
 */
var globalGroupId = "";
var globalUserName = "";

function getGlobalGroupId(){
	return globalGroupId;
}

function getGlobalUserName(){
	return globalUserName;
}

function setGlobalGroupId(groupid){
	globalGroupId = groupid;
}

function setGlobalUserName(name){
	globalUserName = name;
}
 
 
 Ext.define('ncViewer.lanGroupTree', {
    extend: 'Ext.tree.Panel',
    alias: 'widget.lanGroupTree',
   
    change: function(val) {
        if (val > 0) {
            return '<span style="color:green;">' + val + '</span>';
        } else if (val < 0) {
            return '<span style="color:red;">' + val + '</span>';
        }
        return val;
    },
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
				url: '/pronline/Msg?FunName@ncsTreeUser_v4'
			},
			root: {
				text: '全部场所',
				id: '',
				expanded: true
			},
			folderSort: true,
			sorters: [{
				property: 'text',
				direction: 'ASC'
			}]
		});
		store_corp.on('beforeload', function (store, options) {
	 // 	alert(parent.wireflag);

	// 	  var new_params={wireflag:wireflag};
	//      Ext.apply(store.proxy.extraParams,new_params);
		   });	

		Ext.apply(this, {
			margins: '0 0 0 0',
			id:'groupTree',
			store: store_corp,
			autoScroll: true,
			border:false,
			scroll:true,
			forceFit: true,
			rootVisible: true,
	   //     width: 110,
			useArrows: false,
			dockedItems: [{
				xtype: 'toolbar',
				items: [
					{
						xtype:'textfield',
						id:'keyword_corptree',
						name:'keyword',
						style:'color:#7aa7d5',
						width:140,
						emptyText:'场所名称/代码',
						enableKeyEvents:true,
						listeners:{
							'focus':function(){
								if(this.value==''){                
									this.setValue('');
								}
							},
							'keydown' : function(i,e){
								var aae=e.getKey() ; 
								if(aae==13){
									var aa=Ext.getCmp('keyword_corptree').value;
									if(aa!=''){
										var vkeyword=Ext.getCmp('keyword_corptree').value;			         
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
								var aa=Ext.getCmp('keyword_corptree').value;
								var vkeyword=Ext.getCmp('keyword_corptree').value;		         
								store_corp.load({params: {cname:vkeyword}});
							}
						}
					}
				]
			}],        
			listeners:{
				itemclick:function(view, rcd, item, idx, event, eOpts){
					servicecode='';
					corpdid='';
					ncsgroupid='';
					corpdid=rcd.get('did');
					ncsgroupid=rcd.get('id');
					servicecode=rcd.get('code');
					/*
					if(corpdid!=''){
						dxtitle='单位：'+rcd.get('text');
					}
					else if(ncsgroupid!=''){
						dxtitle='单位组：'+rcd.get('text');
					}
					else{
						dxtitle='';
					}
					*/
					setGlobalGroupId(ncsgroupid);
					setGlobalUserName(servicecode);
					dxtitle = rcd.get('text');
					try{
						left_tabs_glob.setTitle('当前位置：'+dxtitle); 
						var tabs_center=Ext.getCmp("layout_center");
						var active = tabs_center.getActiveTab();
						var active_id='lan_'+active.id;
						var form_id="";
						if(Ext.getCmp(active_id)){
							var tabs_if=Ext.getCmp(active_id).getActiveTab();
							var form_id=active_id+'_'+tabs_if.id;
							//eval(form_id).SetPage();
							//eval(form_id).reflash();
						}else{
							var form_id=active_id+'_s';
							//eval(form_id).SetPage();
							//eval(form_id).reflash();								
						} 	
						if(servicecode=="")
							eval(form_id).setSearchValue(0, dxtitle);	
						else	
							eval(form_id).setSearchValue(1, dxtitle);						
					}catch(e){
						
					}
				}
			}
		});
		this.callParent(arguments);
    }
});