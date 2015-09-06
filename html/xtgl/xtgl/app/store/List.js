Ext.define('proauthXtgl.store.List', {
  extend: 'Ext.data.Store',
  pageSize: 40,
  model: 'proauthXtgl.model.List',
 remoteSort: true,
  proxy: {
    type: 'ajax',
    url : '/pronline/Msg?FunName@ncsSysUserlist_v4',
    reader: {
      type: 'json',
      root: 'eimdata',
      totalProperty: 'totalCount'
    },
     simpleSortMode: true
  },
   sorters: {
 //           property: 'userid',
 //           direction: 'DESC'
        }
});
