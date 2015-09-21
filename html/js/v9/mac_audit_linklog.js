
 Ext.define('ncViewer.mac_audit_linklog', {
    extend: 'Ext.panel.Panel',
    alias: 'widget.mac_audit_linklog',
    height:900,
    autoscroll:false,  
    initComponent: function(){
        Ext.apply(this,{   
        	margins: '0 0 0 0',      
        	frame: false,
			html: '<iframe id="lan_itm_audit_linklog_s" name="lan_itm_audit_linklog_s" src="/rzck/iplog/proauth_rzck_iplog.html" scrolling="no" width=100% height=900 FrameBorder=0 ></iframe>' 
    
        });
        this.callParent(arguments);
    }
});