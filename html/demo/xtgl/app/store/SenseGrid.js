Ext.define('LanProcy.store.SenseGrid', {
  extend: 'Ext.data.Store',

  model: 'LanProcy.model.Params',
  autoLoad: true,

  proxy: {
    type: 'ajax',
    url : '/pronline/Msg?FunName@lan_searchParamValue',
    reader: {
      type: 'json',
      root: 'data'
    }
  }
});