Ext.define('proauthXtgl.store.system_role', {
  extend: 'Ext.data.Store',
  pageSize: 40,
 fields: ['username', 'fun', 'dx','userid'],idProperty: 'userid',autoLoad: false,id:'store_system_role',remoteSort: true,
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
