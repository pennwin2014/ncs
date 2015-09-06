Ext.define('proauthApset.store.List', {
  extend: 'Ext.data.Store',
  pageSize: 40,
  model: 'proauthApset.model.List',
 remoteSort: true,
  proxy: {
    type: 'ajax',
    //url : '/pronline/Msg?FunName@proauthWebAplist',
	url : '/pronline/Msg?FunName@ncsSys_terminal',
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
