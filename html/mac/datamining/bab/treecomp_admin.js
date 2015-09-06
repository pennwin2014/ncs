 var store_system_role = Ext.create('Ext.data.Store',{pageSize: 40,fields: ['username', 'fun', 'dx','userid'],idProperty: 'userid',autoLoad: false,id:'store_system_role',remoteSort: true,
        		proxy: {type: 'ajax',url: '/pronline/Msg?FunName@ncsRolelist_v4',reader: {type:'json',root: 'eimdata',totalProperty: 'totalCount'},simpleSortMode: true
        		},sorters:[{property: '',direction: 'DESC'}]});
  //单位选择树
var store_groupcheck = Ext.create('Ext.data.TreeStore', {id:'store_groupcheck',autoLoad : false,
        proxy: {type: 'ajax',url: '/pronline/Msg?FunName@ncsTreeGroup_v4&plate@v8/nc_checkgroup.htm'},
        root: {text: '全部',id: 'all',checked:false,expanded: true},
        folderSort: true,
        sorters: [{property: 'text',direction: 'ASC'}]});  
//部门树 
 var store_classtree=Ext.create('Ext.data.TreeStore',{id:'store_classtree', fields: ['text','id'],autoLoad : false,
				proxy:{type: 'ajax',url: '/pronline/Msg?FunName@ncsTreeGroup_v4&plate@v8/nc_group_tree.htm'}  
				}); 
var store_faccheck = Ext.create('Ext.data.TreeStore', {id:'store_faccheck',autoLoad : false,
        proxy: {type: 'ajax',url: '/pronline/Msg?FunName@ncsTreeFac_v4&plate@v8/nc_checkgroup.htm'},
        root: {text: '全部',id: 'all',checked:false,expanded: true},
        folderSort: true,
        sorters: [{property: 'text',direction: 'ASC'}]});  

Ext.define('TreeCombox', {  
    extend: 'Ext.form.field.Picker',  
    xtype: 'treecombox',  
    triggerCls: Ext.baseCSSPrefix + 'form-arrow-trigger',  
    config: {  
        displayField: null,  
        valueField:null,
        columns: null,  
        rootVisible: false,  
        selectOnTab: true,  
        firstSelected: false,  
        maxPickerWidth: 119,  
        maxPickerHeight: 360,  
        minPickerHeight: 100  
    },  
    editable: false,  
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
                  valueField:me.valueField,
                columns: me.columns,  
                maxHeight: me.maxTreeHeight,  
                shadow: false, 
                rootVisible:false,  
                manageHeight: false,  
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

  
//    	alert(record.data.id);
        this.selectItem(record);  
    },  
    onPickerKeypress: function(e, el) {  
        var key = e.getKey();  
  
        if(key === e.ENTER || (key === e.TAB && this.selectOnTab)) {  
            this.selectItem(this.picker.getSelectionModel().getSelection()[0]);  
        }  
    },  
    selectItem: function(record) {      
  // me.context.record.data.username=record.get('code');

        var me = this; 
        me.setValue(record.get('text'));  
        me.picker.hide();  
        me.inputEl.focus();  
        me.fireEvent('select', me, record);

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
            //var hasOwnProp = me.store.hasOwnProperty('getRootNode');  
            //record = value ? me.store.getNodeById(value) : (hasOwnProp ? me.store.getRootNode() : null);  
            //me.setRawValue(record ? record.get(this.displayField) : '');  
            var i = value.indexOf(">");
            var j = value.indexOf("</");
            if(i != -1 && j != -1)
            {
            		me.setRawValue(value.substring(i+1,j));
            }
            else 
            {
            		me.setRawValue(value);
            }
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


Ext.override(Ext.form.CheckboxGroup,{    
    //在inputValue中找到定义的内容后，设置到items里的各个checkbox中    
    setValue : function(value){   
        this.items.each(function(f){
          if(value.indexOf(f.inputValue) != -1){  
                 f.setValue(true);   
             }else {   
                 f.setValue(false);   
             }      
          
        });   
    },   
    //以value1,value2的形式拼接group内的值   
    getValue : function(){   
        var re = "";   
        this.items.each(function(f){   
            if(f.getValue() == true){   
                re += f.inputValue + ",";   
            }   
        });   
        return re.substr(0,re.length - 1);   
    },   
    //在Field类中定义的getName方法不符合CheckBoxGroup中默认的定义，因此需要重写该方法使其可以被BasicForm找到   
    getName : function(){   
        return this.name;   
    }   
});





 Ext.define('TreeComboxCheck', {  
    extend: 'Ext.form.field.Picker',  
    xtype: 'treecomboxcheck',  
    triggerCls: Ext.baseCSSPrefix + 'form-arrow-trigger',  
    config: {  
        displayField: null,  
        valueField:null,
        columns: null,  
        rootVisible: false,  
        selectOnTab: true,  
        firstSelected: false,  
        maxPickerWidth: 300,  
        maxPickerHeight: 450,  
        minPickerHeight: 100  
    },  
    editable: false,  
    initComponent: function() { 
    	 treeobj=this; 
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
 //           id:'checkgrouppick', 
            opname:me.opname,
            optid:me.optid,
            hidden: true,  
            width: me.maxPickerWidth, 
            displayField: me.displayField,
            valueField:me.valueField,
            columns: me.columns, 
             maxHeight: me.maxTreeHeight,  
                shadow: false, 
                rootVisible:true,  
                manageHeight: false, 
            
        frame:true,
        containerScroll: true,
        useArrows: false,
        
          dockedItems: [
          {
            xtype: 'toolbar',
            items: [{
             text: '确认选择',
                iconCls:'accept',
                handler: function(){
                	 var me = this; 

                   var records=picker.getView().getChecked();                  

						       var temps=''; 
						       var temp_name='';
						       var iNum=0;            
                    Ext.Array.each(records, function(rec){

                    if(iNum==0){
                    temps=rec.get('id');
                 
                    temp_name=rec.get('text');
                    }
                    else{
                    temps=temps+','+rec.get('id');
                    temp_name=temp_name+','+rec.get('text');
                    }
                    iNum++;

                    });

                     
							//		   treeobj.setValue(temp_name);
									   var tid= treeobj.optid;
								
									 //  Ext.getCmp('optid').setValue(temps);
									   var tname= treeobj.opname;									   
									   Ext.getCmp('groupid').setValue(temps);
									   Ext.getCmp('groupname').setValue(temp_name);

                      picker.hide();  
					//					dialogArguments.form1.opt.value=temp_name;
										
					//					dialogArguments.form1.opt1.value=temps; 
						
									
                }
            }]
        }]

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

  
//    	alert(record.data.id);
        this.selectItem(record);  
    },  
    onPickerKeypress: function(e, el) {  
        var key = e.getKey();  
  
        if(key === e.ENTER || (key === e.TAB && this.selectOnTab)) {  
            this.selectItem(this.picker.getSelectionModel().getSelection()[0]);  
        }  
    },  
    selectItem: function(record) {      
  // me.context.record.data.username=record.get('code');

        var me = this; 
        me.setValue(record.get('text'));  
        me.picker.hide();  
        me.inputEl.focus();  
        me.fireEvent('select', me, record);

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
            //var hasOwnProp = me.store.hasOwnProperty('getRootNode');  
            //record = value ? me.store.getNodeById(value) : (hasOwnProp ? me.store.getRootNode() : null);  
            //me.setRawValue(record ? record.get(this.displayField) : '');  
            var i = value.indexOf(">");
            var j = value.indexOf("</");
            if(i != -1 && j != -1)
            {
            		me.setRawValue(value.substring(i+1,j));
            }
            else 
            {
            		me.setRawValue(value);
            }
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



 Ext.define('TreeComboxCheck_f', {  
    extend: 'Ext.form.field.Picker',  
    xtype: 'treecomboxcheck_f',  
    triggerCls: Ext.baseCSSPrefix + 'form-arrow-trigger',  
    config: {  
        displayField: null,  
        valueField:null,
        columns: null,  
        rootVisible: false,  
        selectOnTab: true,  
        firstSelected: false,  
        maxPickerWidth: 300,  
        maxPickerHeight: 450,  
        minPickerHeight: 100  
    },  
    editable: false,  
    initComponent: function() { 
    	 treeobj=this; 
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
 //           id:'checkgrouppick', 
            optid:me.optid,
            opname:me.opname,
            hidden: true,  
            width: me.maxPickerWidth, 
            displayField: me.displayField,
            valueField:me.valueField,
            columns: me.columns, 
             maxHeight: me.maxTreeHeight,  
                shadow: false, 
                rootVisible:true,  
                manageHeight: false, 
            
        frame:true,
        containerScroll: true,
        useArrows: false,
        
          dockedItems: [
          {
            xtype: 'toolbar',
            items: [{
             text: '确认选择',
                iconCls:'accept',
                handler: function(){
                	 var me = this; 

                   var records=picker.getView().getChecked();                  

						       var temps=''; 
						       var temp_name='';
						       var iNum=0;            
                    Ext.Array.each(records, function(rec){

                    if(iNum==0){
                    temps=rec.get('id');
                 
                    temp_name=rec.get('text');
                    }
                    else{
                    temps=temps+','+rec.get('id');
                    temp_name=temp_name+','+rec.get('text');
                    }
                    iNum++;

                    });
               //     alert(temps);


					//				   treeobj.setValue(temp_name);
									 	var tname= treeobj.opname;
							
									   Ext.getCmp(tname).setValue(temp_name);
									   var tid= treeobj.optid;
						
									   Ext.getCmp(tid).setValue(temps);

                      picker.hide();  
					//					dialogArguments.form1.opt.value=temp_name;
										
					//					dialogArguments.form1.opt1.value=temps; 
						
									
                }
            }]
        }]

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

  
//    	alert(record.data.id);
        this.selectItem(record);  
    },  
    onPickerKeypress: function(e, el) {  
        var key = e.getKey();  
  
        if(key === e.ENTER || (key === e.TAB && this.selectOnTab)) {  
            this.selectItem(this.picker.getSelectionModel().getSelection()[0]);  
        }  
    },  
    selectItem: function(record) {      
  // me.context.record.data.username=record.get('code');

        var me = this; 
        me.setValue(record.get('text'));  
        me.picker.hide();  
        me.inputEl.focus();  
        me.fireEvent('select', me, record);

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
            //var hasOwnProp = me.store.hasOwnProperty('getRootNode');  
            //record = value ? me.store.getNodeById(value) : (hasOwnProp ? me.store.getRootNode() : null);  
            //me.setRawValue(record ? record.get(this.displayField) : '');  
            var i = value.indexOf(">");
            var j = value.indexOf("</");
            if(i != -1 && j != -1)
            {
            		me.setRawValue(value.substring(i+1,j));
            }
            else 
            {
            		me.setRawValue(value);
            }
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