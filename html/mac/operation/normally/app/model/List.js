Ext.define('proauthApset.model.List', {
    extend: 'Ext.data.Model',
	fields: ['flags','ftype'
	,'slink','stel','smail','mac',
	'authflag','ssid','security','servicequlity',
	'ISPname','servicetype',
	'ISPphone',
	'ISPemail',
	'note',
	'gtype'],
        idProperty: 'ssid'
});