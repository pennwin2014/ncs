Ext.define('proauthRzAuthlog.store.List', {
  extend: 'Ext.data.Store',
  pageSize: 40,
  model: 'proauthRzAuthlog.model.List',
  proxy: {
    type: 'ajax',
    url : '/pronline/Msg?FunName@ncsCurrent_FromRAM',    
    reader: {
      type: 'json',
      root: 'data',
      totalProperty: 'maccount'
    }
  }
});
