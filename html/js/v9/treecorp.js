
//部门树 
var store_classtree=Ext.create('Ext.data.TreeStore',{id:'store_classtree', fields: ['text','id','code'],autoLoad : false,
				proxy:{type: 'ajax',url: '/pronline/Msg?FunName@ncsTreeUser_v4&plate@v4/ncs_user_tree.htm'},
			  root: {text: '全部',id: 'all',expanded: true}}); 
var globalGroupId = "all";
var globalUserName = "";

function getGlobalGroupId(){
	return globalGroupId;
}

function getGlobalUserName(){
	return globalUserName;
}

function setGlobalGroupId(groupid){
	globalGroupId = groupid;
	globalUserName = "";
}

function setGlobalUserName(name){
	globalUserName = name;
	globalGroupId = "all";
}

function doSetFrameText(dxtitle){
	//改变左边测标题
	Ext.getCmp('left').setTitle('当前位置：'+dxtitle); 
	//填充中间的数据
	var tabs_center=Ext.getCmp("layout_center");
    var activeTab = tabs_center.getActiveTab();
	try{
		if(activeTab.id == "itm_dm_crowd"){
			eval("iframe_dm_track").setplace(dxtitle);
		}else if(activeTab.id == "itm_infoquery_id"){
			eval("mac_infoquery_current_id").setCurrentFieldText(dxtitle);
		}else if(activeTab.id == "itm_infoquery_place"){
			eval("mac_infoquery_p").setPlaceFieldText(dxtitle);
		}
	}catch(e){
		//console.log(e.message)
	}
}



Ext.define('TreeCombox', {  
    extend: 'Ext.form.field.Picker',  
    xtype: 'treecombox',  
    triggerCls: Ext.baseCSSPrefix + 'form-search-trigger',  
    config: {  
        displayField: null,  
        valueField:null,
        columns: null,  
        rootVisible: true,  
        selectOnTab: true,  
        firstSelected: false,  
        maxPickerWidth: 200,  
        maxPickerHeight: 460,  
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
					useArrows: false,
                    dockedItems: [{
						dock: 'bottom',        	
						xtype: 'toolbar',
						items: [{
							xtype:'textfield',
							id:'keyword_corptree',
							name:'keyword',
							style:'color:#7aa7d5',
							width:140,
							emptyText:'场所名称/代码',
							enableKeyEvents:true,
							listeners:{
								'focus':function(){
									if(this.value==''){                
										this.setValue('');
									}
								},
								'keydown' : function(i,e){
									var aae=e.getKey() ; 
									if(aae==13){
									var aa=Ext.getCmp('keyword_corptree').value;
										if(aa!=''){
											var vkeyword=Ext.getCmp('keyword_corptree').value;			         
											store_corp.load({params: {cname:vkeyword}});
										}
									}
								}
							}
						},
						{
							text:'筛选',
							itemId: 'moveButton',
							iconCls:'accept',
			//              disabled: true,
							id:"treeBtn",
							listeners: {
								'click': function(){
									var aa=Ext.getCmp('keyword_corptree').value;
									var vkeyword=Ext.getCmp('keyword_corptree').value;		         
									me.store.load({params: {cname:vkeyword}});
								}
							}
						}]
					}], 
					width: me.maxPickerWidth,  
					displayField: me.displayField,
					valueField:me.valueField,
					columns: me.columns,  
					maxHeight: me.maxTreeHeight,  
					shadow: false, 
					rootVisible:true,  
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
		if(record.data.code==''){
			//alert('您现在选择的是单位组，请选择上网单位进行查询');
			doSetFrameText(record.data.text);
			setGlobalGroupId(record.data.id);
			var me = this; 
			me.setValue(record.get('text')); 
			me.picker.hide();  
			me.inputEl.focus();  
			me.fireEvent('select', me, record);
			return;
		}
 //   	alert(record.data.code);
 //   	alert(record.data.text);
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
        servicecode=record.get('code');
        corpdid=record.get('id');
        dxtitle=record.get('text');
		//alert(servicecode); 
		//alert(dxtitle);
		doSetFrameText(dxtitle);	
		setGlobalUserName(record.data.code);
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


