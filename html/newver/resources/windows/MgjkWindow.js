
Ext.define('MyDesktop.MgjkWindow', {
    extend: 'Ext.ux.desktop.Module',

   

    id:'mgjk-win',

    init : function(){
        this.launcher = {
            text: 'Ãô¸Ð¼à¿Ø',
            iconCls:'icon-grid'
        };
    },

    createWindow : function(){
        var desktop = this.app.getDesktop();
        var win = desktop.getWindow('mgjk-win');
        var htmlvar='<iframe src="/pronline/Msg?FunName@ncsWebReplace&plate@windows/main_frm.htm&title@Ãô¸Ð¼à¿Ø" style="width:1350px;height:950px;display:block;" FrameBorder=0 scrolling="yes">';
        if(!win){
            win = desktop.createWindow({
                id: 'mgjk-win',
                title:'Ãô¸Ð¼à¿Ø',
                width:1024,
                height:700,
                autoScroll: true,
                iconCls: 'icon-grid',
                animCollapse:false,
                constrainHeader:true,
    //            layout: 'fit',
                 html:htmlvar

            });
        }
        return win;
    }
});

