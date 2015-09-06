
 Ext.define('ncViewer.mac_dm_sample', {
    extend: 'Ext.panel.Panel',
    alias: 'widget.mac_dm_sample',
    height:900,
     autoscroll:false,  
    initComponent: function(){
        Ext.apply(this,{   
           margins: '0 0 0 0',      
           frame: false,
           html: '<iframe id="lan_itm_dm_sample_s" name="lan_itm_dm_sample_s" src="/mac/datamining/sample/sample.htm" scrolling="no" width=100% height=900 FrameBorder=0 ></iframe>' 
    
        });

        this.callParent(arguments);
    }
});

