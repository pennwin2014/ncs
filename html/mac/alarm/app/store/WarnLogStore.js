Ext.define('proauthMobileAccount.store.WarnLogStore', {
  extend: 'Ext.data.Store',
  id:'WarnLogStore',
  pageSize: 40,
  field1:'',
  field2:'',
  field3:'',
  field4:'',
  model: 'proauthMobileAccount.model.warnLogModel',
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

