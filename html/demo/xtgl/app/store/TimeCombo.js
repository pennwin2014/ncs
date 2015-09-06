Ext.define('LanProcy.store.TimeCombo', {
  extend: 'Ext.data.Store',

  model: 'LanProcy.model.Combo',
  autoLoad: true,

  proxy: {
    type: 'ajax',
    url : '/pronline/Msg?FunName@lan_time_group_search'/*,
    reader: {
      type: 'json',
      root: 'data'
    }*/
  },
  listeners: {
    'load' : function(thisStore,records,successful,operation,eOpts ){
      //alert("here ");
      thisStore.insert(0,{id:"0",text:'È«²¿'});
    }
  }
});