/**
 定义策略tab
 */
 
Ext.define('ncViewer.ncAd',{
    extend: 'Ext.tab.Panel',

    initComponent : function(){
        Ext.apply(this,{
        layout : 'border',
        id:'lay_ad',
        margins: '0 0 0 0',
 //       frames:true,
        forceFit: true,
        activeTab: 0,        
         defaults :{
 
            bodyPadding: 0
 
        },
        items: [{
            title: '投放模板', 
            id:'adplate',           
            html: '<iframe  id="lay_ad_adplate" name="lay_ad_adplate" src="/ad/proauth_adplate.html" scrolling="no" width=100% height=900  FrameBorder=0 ></iframe>' 
        },
        {
            title: '投放内容',
            id:'adContent',            
            html: '<iframe  id="lay_ad_adContent" name="lay_ad_adContent" src="/v8/ad/nc_adplate_list.htm" scrolling="no" width=100% height=900  FrameBorder=0 ></iframe>' 
        }
        ]
          
        });
        this.callParent(arguments);
    }
})