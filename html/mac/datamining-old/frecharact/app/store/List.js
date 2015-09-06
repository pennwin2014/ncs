Ext.define('proauthRzAuthlog.store.List', {
  extend: 'Ext.data.Store',
  pageSize: 40,
  model: 'proauthRzAuthlog.model.List',
 remoteSort: true,
  proxy: {
    type: 'ajax',
    url : '/pronline/Msg?FunName@ncsDispRoleFunlist_v4',
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
/*Ext.define('proauthRzAuthlog.store.List', {
  extend: 'Ext.data.Store',
  model: 'proauthRzAuthlog.model.List',
  data:[{flags:'hhas',taskid:110, taskname:'shenmegui', newreport:'hhas'},
  {flags:'hhas',taskid:111, taskname:'shenmegui', newreport:'hhas'},
  {flags:'hhas',taskid:112, taskname:'shenmegui', newreport:'hhas'},
  {flags:'hhas',taskid:113, taskname:'shenmegui', newreport:'hhas'},
  {flags:'hhas',taskid:114, taskname:'shenmegui', newreport:'hhas'},
  {flags:'hhas',taskid:115, taskname:'shenmegui', newreport:'hhas'},
  {flags:'hhas',taskid:116, taskname:'shenmegui', newreport:'hhas'},
  {flags:'hhas',taskid:117, taskname:'shenmegui', newreport:'hhas'}
  ]
});*/
