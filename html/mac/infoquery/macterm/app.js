

Ext.application({
  name: 'proauthRzAuthlog',
  appFolder: 'app',  
  controllers: [
  	'Procy'
  ],

  launch: function() {

    Ext.tip.QuickTipManager.init();
    Ext.create('Ext.container.Viewport', {
		//'hbox',
		layout: 'border',
		items: [
		{
			region:'north',
			xtype: 'myselfPanel',
			width:'100%',
			height:'100'
        },
        {
			region:'center',
			xtype: 'authloglist',
			width:'100%',
			height:'50%'
		}       
      ]
    });
  }
});