
Ext.define('MyDesktop.GdrzWindow', {
    extend: 'Ext.ux.desktop.Module',

    id:'gdrz-win',

    init : function(){
        this.launcher = {
            text: '归档日志',
            iconCls:'icon-grid'
        };
    },

    createWindow : function(){
        var desktop = this.app.getDesktop();
        var win = desktop.getWindow('main-gdrz');
//      var htmlvar='<iframe src="/v4/windows/main_frm.htm" style="width:1350px;height:950px;display:block;" FrameBorder=0 scrolling="yes">';
        var htmlvar='<iframe src="/pronline/Msg?FunName@ncsWebReplace&plate@windows/main_frm.htm&title@归档日志" style="width:1350px;height:950px;display:block;" FrameBorder=0 scrolling="yes">';
        if(!win){
            win = desktop.createWindow({
                id: 'main-gdrz',
                title:'归档日志',
                width:1024,
                height:700,
                autoScroll: true,
                iconCls: 'icon-grid',
                animCollapse:false,
                constrainHeader:true,
    //            layout: 'fit',
                 html:htmlvar

            });
        };
       win.on("beforeclose", function() {
 //      	alert('cccc');
  //     	alert(win.aaaa);
       })

  //      alert(win);
  //      win.dhsrc('/v4.ncs_gdrz_lab.htm','归档日志','gdrz');

        return win;
    }
});

