
 Ext.define('proauthRzAuthlog.view.list.oftentime', {
    extend: 'Ext.panel.Panel',
    alias: 'widget.oftentime',
    height:900,
     autoscroll:false,  
    initComponent: function(){
        Ext.apply(this,{   
           margins: '0 0 0 0',      
           frame: false,
           html: '<iframe src="/mac/infoquery/macterm/featureanalysis/oftentime.html" frameborder="no" style="width:100%;height:1000px;"></iframe>' 
    
        });

        this.callParent(arguments);
    }
});
 Ext.define('proauthRzAuthlog.view.list.oftenplace', {
    extend: 'Ext.panel.Panel',
    alias: 'widget.oftenplace',
    height:900,
     autoscroll:false,  
    initComponent: function(){
        Ext.apply(this,{   
           margins: '0 0 0 0',      
           frame: false,
           html: '<iframe src="/mac/infoquery/macterm/featureanalysis/oftenplace.html" frameborder="no" style="width:100%;height:1000px;"></iframe>' 
    
        });

        this.callParent(arguments);
    }
});
 Ext.define('proauthRzAuthlog.view.list.oftenstyle', {
    extend: 'Ext.panel.Panel',
    alias: 'widget.oftenstyle',
    height:900,
     autoscroll:false,  
    initComponent: function(){
        Ext.apply(this,{   
           margins: '0 0 0 0',      
           frame: false,
           html: '<iframe src="/mac/infoquery/macterm/featureanalysis/oftenplacetype.html" frameborder="no" style="width:100%;height:1000px;"></iframe>' 
    
        });

        this.callParent(arguments);
    }
});
Ext.define('proauthRzAuthlog.view.list.choseareacollect', {
    extend: 'Ext.panel.Panel',
    alias: 'widget.collect',
    height:900,
     autoscroll:false,  
    initComponent: function(){
        Ext.apply(this,{   
           margins: '0 0 0 0',      
           frame: false,
           html: '<iframe src="/mac/infoquery/mobileap/checkgraph/choseareacollect.html" frameborder="no" style="width:100%;height:1000px;"></iframe>' 
    
        });

        this.callParent(arguments);
    }
});
 Ext.define('proauthRzAuthlog.view.list.People', {
    extend: 'Ext.panel.Panel',
    alias: 'widget.people',
    height:900,
     autoscroll:false,  
    initComponent: function(){
        Ext.apply(this,{   
           margins: '0 0 0 0',      
           frame: false,
           html: '<iframe src="/mac/datamining/frecharact/terminalMACTop.html" frameborder="no" style="width:100%;height:1000px;"></iframe>' 
    
        });

        this.callParent(arguments);
    }
});






Ext.define('proauthRzAuthlog.view.list.List' ,{
    extend: 'Ext.form.FormPanel',
    alias : 'widget.authloglist',
    height:parent.grid_height,
    width:parent.grid_width,
    autoScroll: true,
    title:'',
    columnLines: true,
    initComponent: function() {
        
        var timeCombo = Ext.create('Ext.form.field.ComboBox', {
        labelWidth: 45,
        fieldLabel:'时间',
        width:160,
        valueField:'value',
        displayField: 'name',
        id:'times',
        name : 'times',      
        value:'0',
        allowBlank: true,
        forceSelection: false,
                                    store: Ext.create('Ext.data.Store', {
                                    fields : ['name', 'value'],
                                    data   : [
                                        {name : '今天',   value: '0'},
                                        {name : '最近三天',   value: '1'},
                                        {name : '最近七天',   value: '2'},
                                        {name : '自定义',   value: '3'}
                                                                
                                    ]
                                })
                                ,
        queryMode: 'local',
        typeAhead: true,
                listeners: {
                      'change':function(){
                      if(this.value==3){
                        var win = Ext.create('proauthRzAuthlog.view.common.AddWin',{
                        title: "时间选择",
                        field1: ""

                          });
                       win.showAt(340,20);

                      }

                        }
                    }
        });
         var placeCombo = Ext.create('Ext.form.field.ComboBox', {
        labelWidth: 60,
        fieldLabel:'统计对象',
        width:220,
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
                                        {name : '指定场所频繁出现终端',   value: '0'},
                                        {name : '指定MAC频繁出现时间段',   value: '1'},
                                        {name : '指定终端频繁出现场所',   value: '2'},
                                        {name : '指定场所人流密集时段',   value: '3'},
                                        {name : '指定终端出现的场所性质',   value: '4'}
                                                                
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
          
            items:['  ',
                 placeCombo
                ,'  ' ,
                 timeCombo
                ,' ',
               {
                 
                    iconCls:'question',
                    action:'help'
                },
                
                {
                    xtype:'textfield',
                    id:'keyword_mobileaccount',
                    width:180,
                    name:'keyword_mobileaccount',
                    style:'color:#7aa7d5',             
                    emptyText:'请输入场所名称或终端MAC',
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
                },' ',
                 
                
                {
                    text:'查询',
                    iconCls:'accept',
                    action:'keycx'
                },
                '-',
             {
			     	 	text:'导出',
			     	 	itemId: 'expbutton',
			     	 	iconCls:'exp',
			     	 	action:'exp'
			     	 },
                {xtype:'label', html:'<span id="titledx"></span>'}
            ]
        });
           
           var tabPanel = new Ext.TabPanel({
            //renderTo: "grid",
            frame: true,
            layout: 'form',
            id:'mytab',
            name:'mytab',
            activeTab: 0
          
        });
            var tabs_center=Ext.getCmp("mytab");
                     var grid;
                     var tab_title;
                    
                       /* grid=Ext.create(proauthRzAuthlog.view.list.oftentime);
                        tab_title='频繁出现的时间';
                     tabs_center.add({
            closable: true,
            autoScroll:false,
            iconCls: 'add16',          
            title: tab_title
            
                   }).show();*/
          
         this.items=[tabPanel];
        this.callParent(arguments);
    }
    
});
