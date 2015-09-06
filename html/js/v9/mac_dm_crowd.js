
 Ext.define('ncViewer.mac_dm_crowd', {
    extend: 'Ext.panel.Panel',
    alias: 'widget.mac_dm_crowd',
    height:900,
    id:'dm_crowd',
     autoscroll:false,  
    initComponent: function(){
        Ext.apply(this,{   
           margins: '0 0 0 0',      
           frame: false,
           html: '<iframe id="lan_itm_dm_crowd_s" name="lan_itm_dm_crowd_s" src="/mac/datamining/crowd/crowd.htm" scrolling="no" width=100% height=900 FrameBorder=0 ></iframe>' 
    
        });

        this.callParent(arguments);
    }
});
 