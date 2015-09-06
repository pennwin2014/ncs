Ext.define('Ext.ux.grid.plugin.RowEditing', {   
   extend: 'Ext.grid.plugin.RowEditing',   
   alias: 'plugin.ux.rowediting',    
      
   /**  
    * 是否添加记录在当前位置<br/>  
    * whether add record at current rowIndex.<br/>  
    * see {@link #cfg-addPosition}  
    * @cfg {Boolean}  
    */  
   addInPlace: false,   
   filed1:'',
   filed2:'',   
   filed3:'',
   filed4:'',
   filed5:'',
   filed6:'',   
   filed7:'',
   filed8:'',
   filed9:'',
   filed10:'',   
   filed11:'',
   filed12:'',
   addPosition: 0,   
      
   /**  
    * 是否点击触发事件,0代表不触发,1代表单击,2代表双击,默认为双击.<br/>  
    * The number of clicks on a grid required to display the editor (disable:0,click:1,dblclick:2)  
    * @cfg {Number}  
    */  
   clicksToEdit:2,   
      
   /**  
    * 是否在取消编辑的时候自动删除添加的记录  
    * if true, auto remove phantom record on cancel,default is true.  
    * @cfg {Boolean}  
    */  
   autoRecoverOnCancel: true,   
      
   /**  
    * adding flag  
    * @private  
    * @type Boolean  
    */  
   adding: false,   
   addflag:false,
   autoCancel:true,   
      
   /**  
    * when add record, hide error tooltip for the first time  
    * @private  
    * @type Boolean  
    */  
   hideTooltipOnAdd: true,   
      
   /**  
    * register canceledit event && relay canceledit event to grid  
    * @param {Ext.grid.Panel} grid  
    * @override  
    * @private  
    */  
   init:function(grid){   
       var me = this;   
       /**  
        * 取消编辑事件.<br/>  
        * Fires canceledit event.And will be relayEvents to Grid.<br/>  
        * @see {@link Ext.ux.grid.RowActions#event-beforeaction} <br/>  
        * @event canceledit  
        * @param {Object} context  
        */  
       me.addEvents('canceledit');
       me.callParent(arguments);   
       grid.addEvents('canceledit');   
       grid.relayEvents(me, ['canceledit']);   
   },   
      
   /**  
    * 提供默认的Editor配置  
    *      @example  
    *      {header:'手机',dataIndex:'phone',fieldType:'numberfield',field:{allowBlank:true}}  
    * provide default field config  
    * @param {String} fieldType 可选值:numberfield,checkboxfield,passwordField  
    * @return {Object}   
    * @protected  
    */  
   getFieldCfg: function(fieldType){   
        switch(fieldType){   
            case 'passwordField':   
                return {   
                    xtype: 'textfield',   
                    inputType: 'password',   
                    allowBlank:false  
                }   
            case 'numberfield':   
                return {   
                    xtype: 'numberfield',   
                    hideTrigger: true,   
                    keyNavEnabled: false,   
                    mouseWheelEnabled: false,   
                    allowBlank:false  
                }   
                   
            case 'checkboxfield':   
                return {   
                    xtype: 'checkboxfield',   
                    inputValue: 'true',   
                    uncheckedValue: 'false'  
                }  
           case 'treecomboxcheck': 
          
                return {   
                    xtype: 'treecomboxcheck',   
                    inputValue: 'true',   
                    uncheckedValue: 'false'  
                } 
                
                
                 
            }   
    },   
       
    /**  
     * Help to config field,just giving a fieldType and field as additional cfg.  
     * see {@link #getFieldCfg}  
     * @private  
     * @override  
     */  
    getEditor: function() {   
        var me = this;   
  
        if (!me.editor) {   
            Ext.each(me.grid.headerCt.getGridColumns(),function(item,index,allItems){   
                if(item.fieldType){   
                    item.field = Ext.applyIf(item.field||{},this.getFieldCfg(item.fieldType))   
                }   
            },this)   
            // keep a reference for custom editor..   
            me.editor = me.initEditor();   
        }   
        me.editor.editingPlugin = me   
        return me.editor;   
    },   
       
    /**  
     * if clicksToEdit===0 then mun the click/dblclick event  
     * @private  
     * @override  
     */  
    initEditTriggers: function(){   
        var me = this    
        var clickEvent = me.clicksToEdit === 1 ? 'click' : 'dblclick'  
        me.callParent(arguments);    
        if(me.clicksToEdit === 0){   
            me.mun(me.view, 'cell' + clickEvent, me.startEditByClick, me);    
        }   
    },   
       
    /**  
     * 添加记录  
     * add a record and start edit it (will not sync store)  
     * @param {Model/Object} data Data to initialize the Model's fields with <br/>  
     * @param {Object} config see {@link #cfg-addPosition}.   
     */  
    startAdd: function(data,config){   
        var me = this;   
        var cfg = Ext.apply({   
            addInPlace: this.addInPlace,   
            addPosition: this.addPosition,   
            colIndex: 0   
        },config)   
           
        //find the position   
        var position;   
        if(cfg.addInPlace){   
            var selected = me.grid.getSelectionModel().getSelection()   
            if(selected && selected.length>0){   
                position = me.grid.store.indexOf(selected[0])    
     //           console.log('a',position)   
                position += (cfg.addPosition<=0) ? 0: 1   
            }else{   
                position = 0   
            }   
        }else{   
            position = (cfg.addPosition==-1 ? me.grid.store.getCount() : cfg.addPosition) || 0   
        }   
           
        var record = data.isModel ? data : me.grid.store.model.create(data);   
        var autoSync = me.grid.store.autoSync;   
        me.grid.store.autoSync = false;   
        me.grid.store.insert(position, record);   
        me.grid.store.autoSync = autoSync;   
          
        me.adding = true  
        me.startEdit(position,cfg.colIndex);   
           
        //since autoCancel:true dont work for me   
        if(me.hideTooltipOnAdd && me.getEditor().hideToolTip){   
            me.getEditor().hideToolTip()   
        }   
    },   
       
          /**  
     * 添加记录  
     * add a record and start edit it (will not sync store)  
     * @param {Model/Object} data Data to initialize the Model's fields with <br/>  
     * @param {Object} config see {@link #cfg-addPosition}.   
     */  
    startAdd1: function(data,config){   
        var me = this;   
  
        me.adding = true  
        me.startEdit(0,0);   
           
     
    },  
       
       
    /**  
     * 编辑之前,自动取消编辑  
     * Modify: if is editing, cancel first.  
     * @private  
     * @override  
     */  
    startEdit: function(record, columnHeader) {   
        var me = this;   
        if(me.editing){   
            me.cancelEdit();    
        }  

        me.callParent(arguments);   
  //      Ext.getCmp('gpid').setValue(record.get('groupname'));
  //      temp_id=record.get('groupid');


    },   
       
    /**  
     * 修改adding的状态值  
     * Modify: set adding=false  
     * @private  
     * @override  
     */  
    completeEdit: function() {   
        var me = this;   
        if (me.editing && me.validateEdit()) {   
            me.editing = false;   
            me.fireEvent('edit', me.context);   
        }   
        me.adding = false 
       var dfiled1 ='me.context.record.data.'+me.filed1;
       var vfiled1='';
       if(me.filed1!=''){vfiled1=eval(dfiled1);}
       
  
       var dfiled2 ='me.context.record.data.'+me.filed2;
       var vfiled2='';    
      if(me.filed2!=''){vfiled2=eval(dfiled2);}
      var dfiled3 ='me.context.record.data.'+me.filed3;
      var vfiled3='';     
      if(me.filed3!=''){vfiled3=eval(dfiled3);}
        
      var dfiled4 ='me.context.record.data.'+me.filed4;
      var vfiled4='';     
      if(me.filed4!=''){vfiled4=eval(dfiled4);}

       var dfiled5 ='me.context.record.data.'+me.filed5;
       var vfiled5='';
       if(me.filed5!=''){vfiled5=eval(dfiled5);}
       
      
       
       var dfiled6 ='me.context.record.data.'+me.filed6;
       var vfiled6='';   
       
      
        
      if(me.filed6!=''){vfiled6=eval(dfiled6);}
      
      var dfiled7 ='me.context.record.data.'+me.filed7;
      var vfiled7='';     
      if(me.filed7!=''){vfiled7=eval(dfiled7);}
      var dfiled8 ='me.context.record.data.'+me.filed8;
      var vfiled8='';     
      if(me.filed8!=''){vfiled8=eval(dfiled8);}

       var dfiled9 ='me.context.record.data.'+me.filed9;
       var vfiled9='';
       if(me.filed9!=''){vfiled9=eval(dfiled9);}
       var dfiled10 ='me.context.record.data.'+me.filed10;
       var vfiled10='';    
      if(me.filed10!=''){vfiled10=eval(dfiled10);}
      var dfiled11 ='me.context.record.data.'+me.filed11;
      var vfiled11='';     
      if(me.filed11!=''){vfiled11=eval(dfiled11);}
      var dfiled12 ='me.context.record.data.'+me.filed12;
      var vfiled12='';     
      if(me.filed12!=''){vfiled12=eval(dfiled12);}

       me.grid.store.load({params: {update:"update",filed1:vfiled1,filed2:vfiled2,filed3:vfiled3,filed4:vfiled4,filed5:vfiled5,filed6:vfiled6,filed7:vfiled7,filed8:vfiled8,filed9:vfiled9,filed10:vfiled10,filed11:vfiled11,filed12:vfiled12}});

      
    },   
       
    /**  
     * 取消编辑  
     * 1.fireEvent 'canceledit'  
     * 2.when autoRecoverOnCancel is true, if record is phantom then remove it  
     * @private  
     * @override  
     */  
    cancelEdit: function(){

        var me = this; 
        if(me.editing){
          me.editing = false; 
        	me.getEditor().cancelEdit();   
        	if(me.adding){
             me.adding=false;
        		 me.context.record.store.remove(me.context.record);  
        	}
        }
/*
        if (me.editing) {
     
    
            me.getEditor().cancelEdit();   
            me.editing = false;   
            me.fireEvent('canceledit', me.context);   
     
             
            if (me.autoRecoverOnCancel){ 
            	if(m.adding){
           
            	   me.context.record.store.remove(me.context.record);  
            	 } 
            	 else{
        
            	}
  //               me.adding = false             	           	

                }else{   
                    //不需要reject,因为Editor没有更改record.   
                    //me.context.record.reject()   
                }   
            }  
            else{
            	 
            	 } 
            	 
  */
        }  


         
 
}) ;