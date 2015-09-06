Ext.define('LanProcy.store.SoftCombo', {
  extend: 'Ext.data.Store',

  model: 'LanProcy.model.Combo',
  autoLoad: true,

  proxy: {
    type: 'ajax',
    url : '/pronline/Msg?FunName@lan_soft_class_search_base'
  }
});