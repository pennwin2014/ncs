Ext.define('proauthMac.store.List', {
  extend: 'Ext.data.Store',
  pageSize: 40,
  model: 'proauthMac.model.List',
 remoteSort: true,
  proxy: {
    type: 'ajax',
    url : '/pronline/Msg?FunName@proauthMaclist',
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
