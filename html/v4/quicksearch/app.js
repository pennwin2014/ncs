Ext.application({
  name: 'quickResarch',
  appFolder: 'app',  
  controllers: [
  	'Procy'
  ],

  launch: function() {
  
    var twidth=parent.Ext.getCmp('layout_center').getWidth()-245;
    Ext.tip.QuickTipManager.init();
    Ext.create('Ext.container.Viewport', {
     layout: 'hbox',
     autoScroll: false,
 //     height: 730,
     
      items: [
      {xtype: 'left',width:'230'},
      {
               xtype: 'container',
               region: 'center',
    //           margin:'0 0 0 0',
               layout: 'vbox',
                items: [      
                {xtype: 'sxxlog',id:'view_sxxlog',width:twidth},
                {xtype: 'netidlog',id:'view_netidlog',width:twidth,hidden:true},
                {xtype: 'httplog',id:'view_httplog',width:twidth,hidden:true},
                {xtype: 'searchlog',id:'view_searchlog',width:twidth,hidden:true},
                {xtype: 'bbslog',id:'view_bbslog',width:twidth,hidden:true},
                {xtype: 'maillog',id:'view_maillog',width:twidth,hidden:true}
                
                ]
       }
      ]
    });
  }
});