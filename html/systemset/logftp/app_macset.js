 	 	
	 	    var store_logsave = Ext.create('Ext.data.Store',{pageSize: 40,fields: ['StartDb2Bcp', 'Db2BcpOutPath','Db2BcpStep','DB2BcpZipPass','Db2BcpScode','Db2BcpDcode','FtpKeep','FtpKeepPath','Db2BcpSendType','StartSendFtp','FtpTimeStep','FtpLocalPath','FtpFileKey','FtpRemotePath','FtpUser','FtpServer','FtpPass'],autoLoad: false,remoteSort: true,
        		proxy: {type: 'ajax',url: '/pronline/Msg?FunName@proauth_logftpparset',reader: {type:'json',root: 'eimdata',totalProperty: 'totalCount'},simpleSortMode: true
        		},sorters:[{property: '',direction: 'DESC'}]});
	 	
	 	
Ext.application({
  name: 'proauthApMacset',
  appFolder: 'app_macset',  
  controllers: [
  	'Procy'
  ],

  launch: function() {

    Ext.tip.QuickTipManager.init();
    Ext.create('Ext.container.Viewport', {
      layout: 'hbox',
      
  //    height: 730,
      
      items: [
        {xtype: 'apmacsetlist',width:'100%'}
       
      ]
    });
  }
});