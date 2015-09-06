Ext.define('LanProcy.store.SenseCombo', {
  extend: 'Ext.data.Store',

  model: 'LanProcy.model.Combo',
  autoLoad: true,

  proxy: {
    type: 'ajax',
    url : '/pronline/Msg?FunName@lan_SenseCombo'
  }
});