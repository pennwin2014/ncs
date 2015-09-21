
 Ext.define('ncViewer.mac_audit_onlinelog', {
    extend: 'Ext.panel.Panel',
    alias: 'widget.mac_audit_onlinelog',
    height:900,
    autoscroll:false,  
    initComponent: function(){
        Ext.apply(this,{   
        	margins: '0 0 0 0',      
        	frame: false,
			html: '<iframe id="lan_itm_audit_onlinelog_s" name="lan_itm_audit_onlinelog_s" src="/rzck/proauth_rzck_authlog.html" scrolling="no" width=100% height=900 FrameBorder=0 ></iframe>' 
    
        });
        this.callParent(arguments);
    }
});