/**
 *下拉树类(Jelly)
 */
Ext.define('Redm.commons.TreeCombox', {
    extend: 'Ext.form.field.Picker',    
    //extend: 'Ext.form.field.Label', 
    xtype: 'treecombox',
   
   	triggerCls: Ext.baseCSSPrefix + 'form-arrow-trigger',
   
    //triggerCls:"renyuanguiji",
    config: {        		    				  		
        displayField: null,
        columns: null,
        rootVisible: true,
        selectOnTab: true,
        firstSelected: false,
        maxPickerWidth: 220,
        maxPickerHeight: 360,
        minPickerHeight: 100
    },
    	
    editable: false,      
        
    //style: 'background:red ',
    
    //设置输入框类型(默认为 Text)、设置按钮中初始值
    inputType:"button",        
    value:"全部场所",
    
    //cls:"aa",
    //overCls:"bb",  
    
    initComponent: function() {
        var me = this;
        me.callParent(arguments);
        this.addEvents('select');
        me.store.on('load', me.onLoad, me);
        
    },
    createPicker: function() {
    	
	    var me = this,
	    picker = Ext.create('Ext.tree.Panel', {
	        store: me.store,
	        floating: true,	        
	        hidden: true,	        	        
	        width: me.maxPickerWidth,	        
	        displayField: me.displayField,
	        columns: me.columns,
	        maxHeight: me.maxTreeHeight,
	        shadow: false,
	        rootVisible: me.rootVisible,
	        manageHeight: false,
	        	        	        
	        //增加属性：是否箭头图标
	        useArrows: true,	        	        
					emptyText : "全部场所",	        
	        listeners: {
	            itemclick: Ext.bind(me.onItemClick, me)
	        },
	        viewConfig: {
	            listeners: {
	                render: function(view) {
	                    view.getEl().on('keypress', me.onPickerKeypress, me);
	                }
	            }
	        }
	    }),
	    view = picker.getView();
	
	    view.on('render', me.setPickerViewStyles, me);
	    if (Ext.isIE9 && Ext.isStrict) {
	        view.on('highlightitem', me.repaintPickerView, me);
	        view.on('unhighlightitem', me.repaintPickerView, me);
	        view.on('afteritemexpand', me.repaintPickerView, me);
	        view.on('afteritemcollapse', me.repaintPickerView, me);
	    }
	    return picker;
	    
    },
    setPickerViewStyles: function(view) {
        view.getEl().setStyle({
            'min-height': this.minPickerHeight + 'px',
            'max-height': this.maxPickerHeight + 'px'
        });
    },
    repaintPickerView: function() {
        var style = this.picker.getView().getEl().dom.style;
        style.display = style.display;
    },
    alignPicker: function() {
        var me = this,
            picker;

        if (me.isExpanded) {
            picker = me.getPicker();
            if (me.matchFieldWidth) {
                picker.setWidth(this.picker.getWidth());
            }
            if (picker.isFloating()) {
                me.doAlign();
            }
        }
    },
    onItemClick: function(view, record, node, rowIndex, e) {
        this.selectItem(record);
    },
    onPickerKeypress: function(e, el) {
        var key = e.getKey();

        if(key === e.ENTER || (key === e.TAB && this.selectOnTab)) {
            this.selectItem(this.picker.getSelectionModel().getSelection()[0]);
        }
    },
    selectItem: function(record) {
        var me = this;
        
        //汪鹏代码，改动部分 钱智明 2015/9/15 17:13        
        //me.setValue(record.get(this.valueField || 'id'));        
        me.setValue(record.get(this.valueField || 'id') || record.raw.text);        
        
        //record.get(this.valueField || 'id') || record.raw.code
        
        me.picker.hide();
        me.inputEl.focus();
        me.fireEvent('select', me, record);
		// 
		/*document.getElementById("form1").groupcode.value=record.get("code");
		document.getElementById("form1").groupid.value=record.get("id");
		var temp=document.getElementById("form1").groupcode.value+document.getElementById("form1").jyxz.value+document.getElementById("form1").ffxz.value;
		Ext.getCmp('username1').setValue(temp);*/
			return record.raw.code;					
    },
    onExpand: function() {
        var me = this,
            picker = me.picker,
            store = picker.store,
            value = me.value;
        if(value) {
        	var node = store.getNodeById(value);
        	if(node)
            	picker.selectPath(node.getPath());
        } else {
        	var hasOwnProp = me.store.hasOwnProperty('getRootNode');
        	if(hasOwnProp)
            	picker.getSelectionModel().select(store.getRootNode());
        }

        Ext.defer(function() {
            picker.getView().focus();
        }, 1);
    },
    setValue: function(value) {
        var me = this,record;
        me.value = value;
        if (me.store.loading) {
            return me;
        }
        try{
        	var hasOwnProp = me.store.hasOwnProperty('getRootNode');
        	record = value ? me.store.getNodeById(value) : (hasOwnProp ? me.store.getRootNode() : null);
        	if(record != null)
        		me.setRawValue(record ? record.get(this.displayField) : '');
        	else
        		//汪鹏代码，改动部分 钱智明 2015/9/15 17:13 
        		me.setRawValue(value);	
        		
        }catch(e){
        	me.setRawValue('');
        }

        return me;
    },
    getValue: function() {
        return this.value;
    },
    onLoad: function(store,node,records) {
        var value = this.value;
        if (value) {
            this.setValue(value);
        }else{
        	if(this.firstSelected){
	        	if(records && records.length > 0){
	        		var record = records[0];
	        		this.setValue(record.get(this.valueField));
	        	}
        	}
        }
    },
    getSubmitData: function() {
        var me = this,
            data = null;
        if (!me.disabled && me.submitValue) {
            data = {};
            data[me.getName()] = '' + me.getValue();
        }
        return data;
    },
    onTriggerClick: function() {
        var me = this;
        //me.store.load();
        if (!me.readOnly && !me.disabled) {
            if (me.isExpanded) {
                me.collapse();
            } else {
                me.expand();
            }
            me.inputEl.focus();
        }
    }
});