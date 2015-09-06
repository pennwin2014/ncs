Ext.define('LanProcy.view.common.TimeCombo' ,{
  extend: 'Ext.form.field.ComboBox',
  alias : 'widget.timecombo',

	fieldLabel: '时间类型',
  store: 'TimeCombo',
  queryMode: 'local',
  displayField: 'text',
  valueField: 'id',
  value:"0",

  initComponent: function() {
    this.callParent(arguments);
  }
});