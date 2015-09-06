Ext.application({
  name: 'proauthRzAuthlog',
  appFolder: 'app',  
  controllers: [
  	'Procy'
  ],

  launch: function() {

    Ext.tip.QuickTipManager.init();
    Ext.create('Ext.container.Viewport', {
      //layout: 'hbox',
      height: 10,
      
      items: [
        {xtype: 'authloglist',width:'100%'}
       
      ]
    });
  }
});