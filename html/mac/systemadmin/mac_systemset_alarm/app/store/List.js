Ext.define('proauthApset.store.List', {
  extend: 'Ext.data.Store',
  pageSize: 40,
  model: 'proauthApset.model.List',
 remoteSort: true,
  proxy: {
    type: 'ajax',
	
  //  url : '/pronline/Msg?FunName@proauthWebAplist',
   url : '/pronline/Msg?FunName@ncsSysystem_alarm',
    reader: {
      type: 'json',
      root: 'eimdata',
      totalProperty: 'totalCount'
    },
     simpleSortMode: true
  },
   sorters: {
            property: 'gwid',
            direction: 'DESC'
        }
});
/*
Ext.define('proauthApset.store.List2', {
	extend: 'Ext.data.Store',
	fields:[],
	proxy: {
		type: 'ajax',
		url : '/pronline/Msg?FunName@ncsSysystem_alarm',
		reader: {
			type: 'json',
			root: 'eimdata',
		},
		simpleSortMode: true
	}
});
*/