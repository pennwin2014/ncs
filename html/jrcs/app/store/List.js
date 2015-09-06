Ext.define('proauthMobileAccount.store.List', {
  extend: 'Ext.data.Store',
  pageSize: 28,
  model: 'proauthMobileAccount.model.List',
 remoteSort: true,
  proxy: {
    type: 'ajax',
    url : '/pronline/Msg?FunName@proauthWhloglist',
    reader: {
      type: 'json',
      root: 'eimdata',
      totalProperty: 'totalCount'
    },
     simpleSortMode: true
  },
   sorters: {
            property: 'sid',
            direction: 'DESC'
        }
});
