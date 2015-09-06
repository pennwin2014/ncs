Ext.define('proauthMobileAccount.store.WarnLogsStore', {
  extend: 'Ext.data.Store',
  id:'WarnLogsStore',
  pageSize: 40,
  field1:'',
  field2:'',
  field3:'',
  field4:'',
  field5:'',
  field6:'',
  model: 'proauthMobileAccount.model.warnLogModel',
 remoteSort: true,
  proxy: {
    type: 'ajax',
    url : '/pronline/Msg?FunName@ncsSys_WarnLogAss',
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

