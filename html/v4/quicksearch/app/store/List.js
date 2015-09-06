var store_sxxlog = Ext.create('Ext.data.Store',{pageSize: 17,fields: ['dispname', 'username', 'corpname','sip','mac','position','idno','idtype','stime','etime','uid','did'],autoLoad: false,id:'store_sxxlog',remoteSort: true,
  proxy: {type: 'ajax',url: '/pronline/Msg?FunName@ncsQuickSearch_sxx',reader: {type:'json',root: 'eimdata1',totalProperty: 'totalCount'},simpleSortMode: true
 },sorters:[{property: '',direction: 'DESC'}]});
 	
 store_sxxlog.on('beforeload', function (store, options) {
    Ext.apply(store.proxy.extraParams, store_sxxlog.new_params);
 });	
 
 var store_netidlog = Ext.create('Ext.data.Store',{pageSize: 17,fields: ['dispname', 'username', 'corpname','sip','mac','position','idno','idtype','stime','etime','nname','ntype','uid','did'],autoLoad: false,id:'store_netidlog',remoteSort: true,
  proxy: {type: 'ajax',url: '/pronline/Msg?FunName@ncsQuickSearch_netidlog',reader: {type:'json',root: 'eimdata1',totalProperty: 'totalCount'},simpleSortMode: true
 },sorters:[{property: '',direction: 'DESC'}]});
 	
 store_netidlog.on('beforeload', function (store, options) {
    Ext.apply(store.proxy.extraParams, store_netidlog.new_params);
 });
 
  var store_httplog = Ext.create('Ext.data.Store',{pageSize: 17,fields: ['dispname', 'username', 'corpname','sip','mac','position','idno','idtype','stime','etime','url','title','urltype','did','uid'],autoLoad: false,id:'store_httplog',remoteSort: true,
  proxy: {type: 'ajax',url: '/pronline/Msg?FunName@ncsQuickSearch_httplog',reader: {type:'json',root: 'eimdata1',totalProperty: 'totalCount'},simpleSortMode: true
 },sorters:[{property: '',direction: 'DESC'}]});
 	
 store_httplog.on('beforeload', function (store, options) {
    Ext.apply(store.proxy.extraParams, store_httplog.new_params);
 });
 
var store_searchlog = Ext.create('Ext.data.Store',{pageSize: 17,fields: ['dispname', 'username', 'corpname','sip','mac','position','idno','idtype','stime','etime','url','sname','svalue','did','uid'],autoLoad: false,id:'store_searchlog',remoteSort: true,
  proxy: {type: 'ajax',url: '/pronline/Msg?FunName@ncsQuickSearch_searchlog',reader: {type:'json',root: 'eimdata1',totalProperty: 'totalCount'},simpleSortMode: true
 },sorters:[{property: '',direction: 'DESC'}]});
 	
 store_searchlog.on('beforeload', function (store, options) {
    Ext.apply(store.proxy.extraParams, store_searchlog.new_params);
 });
 
 var store_bbslog = Ext.create('Ext.data.Store',{pageSize: 17,fields: ['dispname', 'username', 'corpname','sip','mac','position','idno','idtype','stime','etime','url','bname','subject','content','author','did','uid'],autoLoad: false,id:'store_bbslog',remoteSort: true,
  proxy: {type: 'ajax',url: '/pronline/Msg?FunName@ncsQuickSearch_bbslog',reader: {type:'json',root: 'eimdata1',totalProperty: 'totalCount'},simpleSortMode: true
 },sorters:[{property: '',direction: 'DESC'}]});
 	
 store_bbslog.on('beforeload', function (store, options) {
    Ext.apply(store.proxy.extraParams, store_bbslog.new_params);
 });
 
 var store_maillog = Ext.create('Ext.data.Store',{pageSize: 17,fields: ['dispname', 'username', 'corpname','sip','mac','position','idno','idtype','stime','etime','mfrom','mto','mailsub','mtype','author','did','uid'],autoLoad: false,id:'store_maillog',remoteSort: true,
  proxy: {type: 'ajax',url: '/pronline/Msg?FunName@ncsQuickSearch_maillog',reader: {type:'json',root: 'eimdata1',totalProperty: 'totalCount'},simpleSortMode: true
 },sorters:[{property: '',direction: 'DESC'}]});
 	
 store_maillog.on('beforeload', function (store, options) {
    Ext.apply(store.proxy.extraParams, store_maillog.new_params);
 });