Ext.define('proauthApset.model.List', {
    extend: 'Ext.data.Model',
   //fields: ['gwid', 'mac', 'groupid', 'address', 'version','servicecode','plate','line','mapid','ssid','longitude','latitude','flags','corpname','servicename','aptype','fcode','radius','uptimestep','fname','radius']
    fields:['name','department','phone','email','sid','flags','stime','casename']
   });