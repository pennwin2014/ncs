Ext.define('proauthRzAuthlog.view.list.List' ,{
    extend: 'Ext.grid.Panel',
    alias : 'widget.authloglist',
    store : 'List', 
    height:parent.grid_height,
    width:parent.grid_width,
    autoScroll: true,
    title:'',
    columnLines: true,
    initComponent: function() {
         var timeCombo = Ext.create('Ext.form.field.ComboBox', {
        labelWidth: 60,
        width:75,
        valueField:'value',
        displayField: 'name',
        id:'places',
        name : 'places',      
        value:'0',
        allowBlank: true,
        forceSelection: false,
                                    store: Ext.create('Ext.data.Store', {
                                    fields : ['name', 'value'],
                                    data   : [
                                        {name : '今天',   value: '0'},
                                        {name : '最近三天',   value: '1'},
                                        {name : '最近七天',   value: '2'}
                                                                
                                    ]
                                })
                                ,
        queryMode: 'local',
        typeAhead: true,
                listeners: {
                      'change':function(){
                      
                        }
                    }
        });

        this.tbar = Ext.create('Ext.Toolbar',{
           
            items:[
                timeCombo
                ,
                {
                    xtype:'textfield',
                    id:'keyword_mobileaccount',
                    width:180,
                    name:'keyword_bab',
                    style:'color:#7aa7d5',             
                    emptyText:'请在左侧选择场所范围',
                    enableKeyEvents:true,
                    listeners:{
                        'focus':function(){
                        },           
                        'keydown' : function(i,e){
                             var aae=e.getKey() ; 
                             if(aae==13){
                             }
                         }
                    }
                },
                {
                    text:'查询',
                    iconCls:'accept',
                    action:'keycx'
                },
                
                {xtype:'label', html:'<span id="titledx"></span>'}
            ]
        });
    
        this.columns = [];
     
        this.callParent(arguments);
    }
    
});
