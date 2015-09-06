
 
Ext.define('proauthMobileAccount.view.common.WarnlogWin' ,{
	extend: 'Ext.window.Window',
	alias : 'widget.warnlogstore',
	store: 'WarnLogStore',
	width:600,
	//height:800,
  initComponent: function() {
	 
  	this.items = [
  	{
  	        items: [{
				xtype: 'form',
				frame:true,
				bodyPadding:5,
				defaults:{xtype:"textfield"},
				items:[
  	        {fieldLabel:'终端MAC地址',id:'mac',name:'mac',value:'',readOnly:true,fieldStyle:'background-color: #DFE9F6;border-color: #DFE9F6; background-image: none;'},
			{fieldLabel:'终端品牌',id:'termtype',name:'termtype',value:'',readOnly:true,fieldStyle:'background-color: #DFE9F6;border-color: #DFE9F6; background-image: none;'},
			{fieldLabel:'场所名称',id:'servicename',name:'servicename',value:'',readOnly:true,fieldStyle:'background-color: #DFE9F6;border-color: #DFE9F6; background-image: none;'},
			{fieldLabel:'安装位置',id:'address',name:'address',value:'',readOnly:true,fieldStyle:'background-color: #DFE9F6;border-color: #DFE9F6; background-image: none;'},
			{fieldLabel:'采集时间',id:'stime',name:'stime',value:'',readOnly:true,fieldStyle:'background-color: #DFE9F6;border-color: #DFE9F6; background-image: none;'},
			{fieldLabel:'终端场强',id:'rssi',name:'rssi',value:'',readOnly:true,fieldStyle:'background-color: #DFE9F6;border-color: #DFE9F6; background-image: none;'},
			{xtype:'combo',fieldLabel:'身份类型',id:'vtype',name:'vtype',valueField:'value',displayField:'name',value:'',readOnly:true,fieldStyle:'background-color: #DFE9F6;border-color: #DFE9F6; background-image: none;',
				store:Ext.create('Ext.data.Store',{
					fields:['name','value'],
					data:[
						{name:'真实身份',value:'0'},
						{name:'虚拟身份',value:'1'}
					]
				}),
				queryMode:'local'
			},
			{fieldLabel:'身份内容',id:'vname',name:'vname',value:'',readOnly:true,fieldStyle:'background-color: #DFE9F6;border-color: #DFE9F6; background-image: none;'},
			{fieldLabel:'接入热点MAC',id:'apmac',name:'apmac',value:'',readOnly:true,fieldStyle:'background-color: #DFE9F6;border-color: #DFE9F6; background-image: none;'},
			{fieldLabel:'接入热点SSID',id:'ssid',name:'ssid',value:'',readOnly:true,fieldStyle:'background-color: #DFE9F6;border-color: #DFE9F6; background-image: none;'},
			{fieldLabel:'接入热点频道',id:'channel',name:'channel',value:'',readOnly:true,fieldStyle:'background-color: #DFE9F6;border-color: #DFE9F6; background-image: none;'},
			{xtype:'combo',fieldLabel:'接入热点加密类型',id:'security',name:'security',valueField:'value',displayField:'name',value:'',readOnly:true,fieldStyle:'background-color: #DFE9F6;border-color: #DFE9F6; background-image: none;',
				store:Ext.create('Ext.data.Store',{
					fields:['name','value'],
					data:[
						{name:'WEP',value:'01'},
						{name:'WPA',value:'02'},
						{name:'WPA2',value:'03'},
						{name:'PSK',value:'04'},
						{name:'其它',value:'99'}
					]
				}),
				queryMode:'local'
			},	
			{fieldLabel:'采集设备编号',id:'apname',name:'apname',value:'',readOnly:true,fieldStyle:'background-color: #DFE9F6;border-color: #DFE9F6; background-image: none;'},
			{fieldLabel:'采集设备经度',id:'longitude',name:'longitude',value:'',readOnly:true,fieldStyle:'background-color: #DFE9F6;border-color: #DFE9F6; background-image: none;'},
			{fieldLabel:'采集设备纬度',id:'latitude',name:'latitude',value:'',readOnly:true,fieldStyle:'background-color: #DFE9F6;border-color: #DFE9F6; background-image: none;'},
			{fieldLabel:'采集设备X坐标',id:'xpos',name:'xpos',value:'',readOnly:true,fieldStyle:'background-color: #DFE9F6;border-color: #DFE9F6; background-image: none;'},
			{fieldLabel:'采集设备Y坐标',id:'ypos',name:'ypos',value:'',readOnly:true,fieldStyle:'background-color: #DFE9F6;border-color: #DFE9F6; background-image: none;'},
			{xtype:'combo',fieldLabel:'上下线标识',id:'flags',name:'flags',valueField:'value',displayField:'name',value:'',readOnly:true,fieldStyle:'background-color: #DFE9F6;border-color: #DFE9F6; background-image: none;',
				store:Ext.create('Ext.data.Store',{
					fields:['name','value'],
					data:[
						{name:'进入',value:'0'},
						{name:'离开',value:'1'},
						{name:'更新',value:'2'}
					]
				}),
				queryMode:'local'
			}
			]}
			]
	}];
    this.callParent(arguments);
		 
	var store = Ext.ComponentQuery.query('mobileaccountlist')[0].getStore();	
		
	var warnlogstore=Ext.data.StoreMgr.lookup('WarnLogsStore');
		new_params={sid:store.field5,stime:store.field6};
		warnlogstore.on('beforeload', function (store, options) {
		Ext.apply(store.proxy.extraParams, new_params);
		});
	var tform=this.query('form')[0];
        warnlogstore.load({
			callback:function(records,options,success){
				if(success){     
						tform.getForm().loadRecord(records[0]); 
				}
			} 		  
	})
  }   
});