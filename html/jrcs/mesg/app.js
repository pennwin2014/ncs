   
    Ext.define('State', {
        extend: 'Ext.data.Model',
        fields: [
            {type: 'string', name: 'groupid'},
            {type: 'string', name: 'groupname'}
        ]
    });
    var store_fac = Ext.create('Ext.data.Store', {
        model: 'State',
        remoteSort: true,
         proxy: {
            type: 'ajax',
            url: '/pronline/Msg?FunName@ncsWebFacComp_mesg',
            reader: {
                type:'json',
                root: 'corpclassdata'              
            },         
            simpleSortMode: true
        }
    });
    store_fac.load();

Ext.application({
  name: 'proauthMobileAccount',
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
        {xtype: 'mobileaccountlist',width:'100%'}
       
      ]
    });
  }
});