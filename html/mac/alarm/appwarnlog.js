Ext.application({
  name: 'proauthMobileAccount',
  appFolder: 'appwarnlog',  
  controllers: [
  	'Procy'
  ],

  launch: function() {

    Ext.tip.QuickTipManager.init();
    Ext.create('Ext.container.Viewport', {
      layout: 'hbox',
      
  //    height: 730,
      
      items: [
        {xtype: 'warnProcyTree',width:'20%'},
        {xtype: 'mobileaccountlist',width:'80%'}
       
      ]
    });
  }
});