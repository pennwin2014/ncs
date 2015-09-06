
 Ext.define('ncViewer.proauthApgl', {
    extend: 'Ext.panel.Panel',
    alias: 'widget.proauthApgl',
    height:900,
     autoscroll:false,  
    initComponent: function(){
        Ext.apply(this,{   
        	 margins: '0 0 0 0',      
        	 frame: false,
           html: '<iframe id="lan_apgl_s" name="lan_apgl_s" src="/dwgl/apset/proauth_apset.htm" scrolling="no" width=100% height=900 FrameBorder=0 ></iframe>' 
    
        });

        this.callParent(arguments);
    }
});