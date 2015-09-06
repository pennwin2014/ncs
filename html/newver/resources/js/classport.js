/**
* @class ExtMVC.view.app.PortalDropZone
* @extends Ext.dd.DropTarget
* Internal class that manages drag/drop for {@link Ext.app.PortalPanel}.
*/
Ext.define('ExtMVC.view.app.PortalDropZone', {
    extend: 'Ext.dd.DropTarget',

    constructor: function (portal, cfg) {
        this.portal = portal;
        Ext.dd.ScrollManager.register(portal.body);
        ExtMVC.view.app.PortalDropZone.superclass.constructor.call(this, portal.body, cfg);
        portal.body.ddScrollConfig = this.ddScrollConfig;
    },

    ddScrollConfig: {
        vthresh: 50,
        hthresh: -1,
        animate: true,
        increment: 200
    },

    createEvent: function (dd, e, data, col, c, pos) {
        return {
            portal: this.portal,
            panel: data.panel,
            columnIndex: col,
            column: c,
            position: pos,
            data: data,
            source: dd,
            rawEvent: e,
            status: this.dropAllowed
        };
    },

    notifyOver: function (dd, e, data) {
        var xy = e.getXY(),
            portal = this.portal,
            proxy = dd.proxy;

        // case column widths
        if (!this.grid) {
            this.grid = this.getGrid();
        }

        // handle case scroll where scrollbars appear during drag
        var cw = portal.body.dom.clientWidth;
        if (!this.lastCW) {
            // set initial client width
            this.lastCW = cw;
        } else if (this.lastCW != cw) {
            // client width has changed, so refresh layout & grid calcs
            this.lastCW = cw;
            //portal.doLayout();
            this.grid = this.getGrid();
        }

        // determine column
        var colIndex = 0,
            colRight = 0,
            cols = this.grid.columnX,
            len = cols.length,
            cmatch = false;

        for (len; colIndex < len; colIndex++) {
            colRight = cols[colIndex].x + cols[colIndex].w;
            if (xy[0] < colRight) {
                cmatch = true;
                break;
            }
        }
        // no match, fix last index
        if (!cmatch) {
            colIndex--;
        }

        // find insert position
        var overPortlet, pos = 0,
            h = 0,
            match = false,
            overColumn = portal.items.getAt(colIndex),
            portlets = overColumn.items.items,
            overSelf = false;

        len = portlets.length;

        for (len; pos < len; pos++) {
            overPortlet = portlets[pos];
            h = overPortlet.el.getHeight();
            if (h === 0) {
                overSelf = true;
            } else if ((overPortlet.el.getY() + (h / 2)) > xy[1]) {
                match = true;
                break;
            }
        }

        pos = (match && overPortlet ? pos : overColumn.items.getCount()) + (overSelf ? -1 : 0);
        var overEvent = this.createEvent(dd, e, data, colIndex, overColumn, pos);

        if (portal.fireEvent('validatedrop', overEvent) !== false && portal.fireEvent('beforedragover', overEvent) !== false) {

            // make sure proxy width is fluid in different width columns
            proxy.getProxy().setWidth('auto');
            if (overPortlet) {
                dd.panelProxy.moveProxy(overPortlet.el.dom.parentNode, match ? overPortlet.el.dom : null);
            } else {
                dd.panelProxy.moveProxy(overColumn.el.dom, null);
            }

            this.lastPos = {
                c: overColumn,
                col: colIndex,
                p: overSelf || (match && overPortlet) ? pos : false
            };
            this.scrollPos = portal.body.getScroll();

            portal.fireEvent('dragover', overEvent);
            return overEvent.status;
        } else {
            return overEvent.status;
        }

    },

    notifyOut: function () {
        delete this.grid;
    },

    notifyDrop: function (dd, e, data) {
        delete this.grid;
        if (!this.lastPos) {
            return;
        }
        var c = this.lastPos.c,
            col = this.lastPos.col,
            pos = this.lastPos.p,
            panel = dd.panel,
            dropEvent = this.createEvent(dd, e, data, col, c, pos !== false ? pos : c.items.getCount());

        Ext.suspendLayouts();

        if (this.portal.fireEvent('validatedrop', dropEvent) !== false && this.portal.fireEvent('beforedrop', dropEvent) !== false) {

            // make sure panel is visible prior to inserting so that the layout doesn't ignore it
            panel.el.dom.style.display = '';

            if (pos !== false) {
                c.insert(pos, panel);
            } else {
                c.add(panel);
            }

            dd.proxy.hide();
            this.portal.fireEvent('drop', dropEvent);

            // scroll position is lost on drop, fix it
            var st = this.scrollPos.top;
            if (st) {
                var d = this.portal.body.dom;
                setTimeout(function () {
                    d.scrollTop = st;
                },
                10);
            }

        }

        Ext.resumeLayouts(true);

        delete this.lastPos;
        return true;
    },

    // internal cache of body and column coords
    getGrid: function () {
        var box = this.portal.body.getBox();
        box.columnX = [];
        this.portal.items.each(function (c) {
            box.columnX.push({
                x: c.el.getX(),
                w: c.el.getWidth()
            });
        });
        return box;
    },

    // unregister the dropzone from ScrollManager
    unreg: function () {
        Ext.dd.ScrollManager.unregister(this.portal.body);
        ExtMVC.view.app.PortalDropZone.superclass.unreg.call(this);
    }
});

/**
* @class ExtMVC.view.portlet.Portlet
* @extends Ext.panel.Panel
* A {@link Ext.panel.Panel Panel} class that is managed by {@link Ext.app.PortalPanel}.
*/
Ext.define('ExtMVC.view.app.Portlet', {
    extend: 'Ext.panel.Panel',
    alias: 'widget.portlet',
    layout: 'fit',
    anchor: '100%',
    frame: true,
    closable: true,
    collapsible: true,
    animCollapse: true,
    draggable: {
        moveOnDrag: false
    },
    cls: 'x-portlet',

    // Override Panel's default doClose to provide a custom fade out effect
    // when a portlet is removed from the portal
    doClose: function () {
        if (!this.closing) {
            this.closing = true;
            this.el.animate({
                opacity: 0,
                callback: function () {
                    this.fireEvent('close', this);
                    this[this.closeAction]();
                },
                scope: this
            });
        }
    }
});



/**
* @class Ext.app.PortalColumn
* @extends Ext.container.Container
* A layout column class used internally be {@link Ext.app.PortalPanel}.
*/
Ext.define('ExtMVC.view.app.PortalColumn', {
    extend: 'Ext.container.Container',
    alias: 'widget.portalcolumn',

    requires: ['ExtMVC.view.app.Portlet'],

    layout: 'anchor',
    defaultType: 'portlet',
    cls: 'x-portal-column'

    // This is a class so that it could be easily extended
    // if necessary to provide additional behavior.
});



/**
* @class Ext.app.PortalPanel
* @extends Ext.panel.Panel
* A {@link Ext.panel.Panel Panel} class used for providing drag-drop-enabled portal layouts.
*/
Ext.define('ExtMVC.view.app.PortalPanel', {
    extend: 'Ext.panel.Panel',
    alias: 'widget.portalpanel',

    requires: ['ExtMVC.view.app.PortalColumn'],

    cls: 'x-portal',
    bodyCls: 'x-portal-body',
    defaultType: 'portalcolumn',
    autoScroll: true,

    initComponent: function () {
        var me = this;

        // Implement a Container beforeLayout call from the layout to this Container
        this.layout = {
            type: 'column'
        };
        this.callParent();

        this.addEvents({
            validatedrop: true,
            beforedragover: true,
            dragover: true,
            beforedrop: true,
            drop: true
        });
        this.on('drop', this.doLayout, this);
    },

    // Set columnWidth, and set first and last column classes to allow exact CSS targeting.
    beforeLayout: function () {
        var items = this.layout.getLayoutItems(),
            len = items.length,
            i = 0,
            item;

        for (; i < len; i++) {
            item = items[i];
            item.columnWidth = 1 / len;
            item.removeCls(['x-portal-column-first', 'x-portal-column-last']);
        }
        items[0].addCls('x-portal-column-first');
        items[len - 1].addCls('x-portal-column-last');
        return this.callParent(arguments);
    },

    // private
    initEvents: function () {
        this.callParent();
        this.dd = Ext.create('ExtMVC.view.app.PortalDropZone', this, this.dropConfig);
    },

    // private
    beforeDestroy: function () {
        if (this.dd) {
            this.dd.unreg();
        }
        this.callParent();
    }
});


///////////////////////////////////////////////////////////////////////////////////////


Portal.jsÄÚÈÝ

Ext.define('ExtMVC.view.portal.PortletPanel', {
    extend: 'ExtMVC.view.app.PortalPanel',    
    
    alias: 'widget.portletpanel',

    //uses: ['ExtMVC.util.Constants'],
    
    getTools: function(){
        return [{
            xtype: 'tool',
            type: 'gear',
            handler: function(e, target, panelHeader, tool){
                var portlet = panelHeader.ownerCt;
                portlet.setLoading('Loading...');
                Ext.defer(function() {
                    portlet.setLoading(false);
                }, 2000);
            }
        }];
    },
    
    initComponent: function() {
        
        Ext.apply(this, {

            items: [{
                id: 'col-1',
                items: [{
                    id: 'portlet-1',
                    title: 'Grid Portlet',
                    tools: this.getTools(),
                    //items: Ext.create('ExtMVC.view.portlet.GridPortlet')
                    html:'aaa'
                },{
                    id: 'portlet-2',
                    title: 'Portlet 2',
                    tools: this.getTools(),
                    //html: ExtMVC.util.Constants.shortBogusMarkup
                    html:'bbb'
                }]
            },{
                id: 'col-2',
                items: [{
                    id: 'portlet-3',
                    title: 'Portlet 3',
                    tools: this.getTools(),
                    //html: ExtMVC.util.Constants.bogusMarkup
                    html:'ccc'
                }]
            },{
                id: 'col-3',
                items: [{
                    id: 'portlet-4',
                    title: 'Stock Portlet',
                    tools: this.getTools(),
                    //items: Ext.create('ExtMVC.view.portlet.ChartPortlet')
                    html:'ddd'
                }]
            }]
            
        });
                
        this.callParent(arguments);
    }
});





Ext.define('ExtMVC.view.Viewport', {

    extend: 'Ext.container.Viewport',
   
    uses: [
        'ExtMVC.view.app.PortalPanel', 
        'ExtMVC.view.app.PortalColumn',
        'ExtMVC.view.portal.PortletPanel',
    ],

    initComponent: function(){

        Ext.apply(this, {
            id: 'app-viewport',
            layout: {
                type: 'border',
                padding: '0 5 5 5'
            },
           items: [/* {
                id: 'app-header',
                xtype: 'box',
                region: 'north',
                height: 40,
                html: 'Ext Portal'
            },*/{
                xtype: 'container',
                region: 'center',
                layout: 'border',
                items: [{
                    id: 'app-portal',
                    xtype: 'portletpanel',
                    region: 'center',
                }]
            }]
        });
        this.callParent(arguments);
    }
});
