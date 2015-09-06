Ext.define('LanProcy.view.common.SoftCombo' ,{
  extend: 'Ext.form.field.ComboBox',
  alias : 'widget.softcombo',

	fieldLabel: '»Ìº˛¿‡–Õ',
  store: 'SoftCombo',
  queryMode: 'local',
  displayField: 'text',
  valueField: 'id',

  initComponent: function() {
    this.callParent(arguments);
  }
});