Ext.application({
  name: 'proauthMac',
  appFolder: 'app',  
  controllers: [
  	'Procy'
  ],

  launch: function() {

    Ext.tip.QuickTipManager.init();
    Ext.create('Ext.container.Viewport', {
      layout: 'hbox',
      
  //    height: 730,
      
      items: [
        {xtype: 'maclist',width:'100%'}
       
      ]
    });
  }
});