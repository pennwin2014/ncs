Ext.define('proauthApset.store.List', {
  extend: 'Ext.data.Store',
  pageSize: 40,
  model: 'proauthApset.model.List',
 remoteSort: true,
  proxy: {
    type: 'ajax',
 //   url : '/pronline/Msg?FunName@proauthWebAplist_normally',
	  url : '/pronline/Msg?FunName@ncsSysoperation',
    reader: {
      type: 'json',
      root: 'eimdata',
      totalProperty: 'totalCount'
    },
     simpleSortMode: true
  }
});
