Ext.define('proauthMobileAccount.store.List', {
  extend: 'Ext.data.Store',
  pageSize: 40,
  field1:'',
  field2:'',
  field3:'',
  field4:'',
  model: 'proauthMobileAccount.model.List',
 remoteSort: true,
  proxy: {
    type: 'ajax',
    url : '/pronline/Msg?FunName@ncsSys_WarnLog',
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
