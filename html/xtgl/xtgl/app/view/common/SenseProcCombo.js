Ext.define('LanProcy.view.common.SenseProcCombo' ,{
  extend: 'Ext.form.field.ComboBox',
  alias : 'widget.sensecombo',

	fieldLabel: '进程类型',
	labelWidth: 80,
  store: 'SenseCombo',
  queryMode: 'remote',
  displayField: 'text',
  valueField: 'id',
	triggerAction: 'all',

  initComponent: function() {
    this.callParent(arguments);
  }
});