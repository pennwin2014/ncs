Ext.define('proauthApset.store.Lista', {
  extend: 'Ext.data.Store',
  pageSize: 40,
  model: 'proauthApset.model.Lista',
 remoteSort: true,
  proxy: {
    type: 'ajax',
   // url : '/pronline/Msg?FunName@proauthWebAplist',
   url : '/pronline/Msg?FunName@ncsSys_stafinfo_deal',
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