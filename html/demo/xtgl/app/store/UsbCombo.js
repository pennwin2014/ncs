Ext.define('LanProcy.store.UsbCombo', {
  extend: 'Ext.data.Store',

  model: 'LanProcy.model.Combo',
  autoLoad: true,

  proxy: {
    type: 'ajax',
    url : '/pronline/Msg?FunName@lan_USB_class_search'
  }
});