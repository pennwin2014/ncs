Ext.define('proauthMobileAccount.store.MacLimStore', {
  extend: 'Ext.data.Store',
  id:'MacLimStore',
  pageSize: 40,
  model: 'proauthMobileAccount.model.macModel',
 remoteSort: true,
  proxy: {
    type: 'ajax',
    url : '/pronline/Msg?FunName@ncsSys_WarnMacLim',
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
