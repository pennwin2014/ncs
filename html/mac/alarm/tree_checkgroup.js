 var store_groupcheck = Ext.create('Ext.data.TreeStore', {id:'store_groupcheck',autoLoad : false,
        proxy: {type: 'ajax',url: '/pronline/Msg?FunName@ncsTreeGroup_v4&plate@v8/nc_checkgroup.htm'},
        root: {text: '全部',id: 'all',checked:false,expanded: true},
         folderSort: true,
        sorters: [{property: 'text',direction: 'ASC'}]});  
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
                    	var gflag='';
                    	gflag=rec.get('leaf');

                    if(iNum==0){
                    	if(gflag){
                        temps='2_'+rec.get('id');
                      }
                      else{
                      	temps='2_'+rec.get('id');
                       }
                 
                    temp_name=rec.get('text');
                    }
                    else{
                    	if(gflag){
                        temps=temps+',2_'+rec.get('id');               //单位
                      }
		                  else{
		                  	temps=temps+',2_'+rec.get('id');               //单位组
		                  }
                      temp_name=temp_name+','+rec.get('text');
                    }
                    iNum++;

                    });

 if(temps.indexOf('all')!='-1'){
 	temps='0_all';
 	temp_name='全部';
 }
        
							//		   treeobj.setValue(temp_name);
									   var tid= treeobj.optid;
					
				  				   Ext.getCmp('optid').setValue(temps);
				  			
									   var tname= treeobj.opname;
			//		alert(tname);	
									   Ext.getCmp('opt').setValue(temp_name);

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