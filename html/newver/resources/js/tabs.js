
Ext.require('Ext.tab.*');

Ext.onReady(function(){
    // basic tabs 1, built from existing content

    var tabs = Ext.createWidget('tabpanel', {
        renderTo: 'tabs1',
        width: 250,
        activeTab: 0,
        defaults :{
            bodyPadding: 10
        },
        items: [{
            title: 'µ¼º½Ê÷',
            loader: {
                    url: '/l.htm',
                    contentType: 'html',
                    contentEl:'script', 
  //                  autoLoad: true,
                    params: 'foo=123&bar=abc'
                },
                listeners: {
                    activate: function(tab) {
        
                        tab.loader.load();
                    }
                }
        },{
            contentEl:'markup', 
            title: 'µ¥Î»Ê÷',
             loader: {
                    url: '/11.htm',
                    contentType: 'html',
                    contentEl:'script',
//                    autoLoad: true,
                    params: ''
                }
            
        }]
    });
 

});
