Ext.define('proauthRzAuthlog.store.List', {
  extend: 'Ext.data.Store',
  pageSize: 40,
  model: 'proauthRzAuthlog.model.List',
 remoteSort: true,
  proxy: {
    type: 'ajax',
    url : '/pronline/Msg?FunName@dm_bab',
    reader: {
      type: 'json',
      root: 'eimdata',
      totalProperty: 'totalCount'
    },
     simpleSortMode: true
  },
   sorters: {
          //  property: 'sid',
           // direction: 'DESC'
        }
});
