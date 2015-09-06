/*
 * elLvl1: element for containing 1st level menu
 * elLvl2: element for containing 2st level menu
 * navBar: the current position
 * menuData: 
 *
 *
 */
PgMenu = function (config) {
	Ext.apply(this, config);
	this.init();

	//PgMenu.target = config.target;
	//PgMenu.navBar = config.navBar;
}
PgMenu.jump = function (url, node) {
	// jump
	if (PgMenu.target) {
		PgMenu.target.location.href = url;
	}
	// fill navigation bar
	if (node != '' && PgMenu.navBar) {
		PgMenu.navBar.innerHTML = '';
		PgMenu.__createNavSpan(node);
		node = node.owner;
		if (node.onClick) {
			PgMenu.__createNavSep();
			PgMenu.__createNavSpan(node);
			node = node.owner;
			if (node.onClick) {
			PgMenu.__createNavSep();
				PgMenu.__createNavSpan(node);
			}
		}
	}
}
PgMenu.__createNavSep = function () {
	Ext.fly(PgMenu.navBar).insertFirst({
		tag: 'span',
		cls: 'pgmenu-nav-sep',
		html: '>>'
	});
}
PgMenu.__createNavSpan = function (node) {
	Ext.fly(PgMenu.navBar).insertFirst({
		tag: 'span',
		cls: 'pgmenu-nav-span',
		html: node.menuData.name
	})/*.on('click', node.onClick, node)*/;
}

PgMenu.prototype = {
	init: function () {

		this.children = [];
		for (var i = 0; i < this.menuData.length; ++i) {
			this.children[this.children.length] = new PgMenu.Lvl1Node({
				owner: this,
				elLvl1: this.elLvl1,
				elLvl2: this.elLvl2,
				menuData: this.menuData[i]
			});

			if (!this.defaultNode)
				this.defaultNode = this.children[0];
		}
		this.click(this.defaultNode);
	},

	click: function (node) {
		for (var i = 0; i < this.children.length; ++i)
			if (node != this.children[i])
				this.children[i].off();
		node.on();
	},

	jump: function (arr, url) {
		if (!arr instanceof Array) {
			return;
		}
		var f1 = 0, f2 = 0, f3 = 0, c='';
		var l1 = arr[0], l2 = arr[1], l3 = arr[2];
		
		if (l1 !== null && typeof(this.children[l1])!='undefined') {
			f1 = this.children[l1];
			if (l2 !== null && typeof(f1.children[l2])!='undefined') {
				f2 = f1.children[l2];
				if (l3 !== null && typeof(f2.children[l3])!='undefined') {
					f3 = c = f2.children[l3];
				}
			}
		}
		
		if (f1 != 0 && f2 != 0 && f3 != 0) {
			for (var i = 0; i < this.children.length; ++i) {
				this.children[i].off();
			}
			f1.expand();
			f2.on(true, true);
			f3.on(true);
		}

		if (url) {
			PgMenu.jump(url, c);
		}
	}
}

/*
 * owner: owner
 * elLvl1: container holding 1st lvl menu
 * elLvl2: container holding 2st lvl menu
 * menuData: array
 */
PgMenu.Lvl1Node = function (config) {
	Ext.apply(this, config);
	this.init();
}
PgMenu.Lvl1Node.prototype = {
	init: function () {
		if (this.menuData.isDefault)
			this.owner.defaultNode = this;

		// 1st lvl button
		this.button = Ext.fly(this.elLvl1).createChild({
			tag: 'div',
			cls: 'divleft1-button' + ' ' + (this.menuData.hidden ? 'pgmenu-hidden' : ''),
			children: [{
				tag: 'img',
				src: this.menuData.offImg
			}]
		}).dom;
		Ext.fly(this.button).on('click', this.onClick, this);
		this.img = this.button.firstChild;

		// 2nd lvl menu set
		this.set = Ext.fly(this.elLvl2).createChild({
			tag: 'ul',
			cls: 'divleft2-set',
			style: 'display: none;'
		}).dom;

		this.children = [];
		if (this.menuData.children) {
			for (var i = 0; i < this.menuData.children.length; ++i) {
				this.children[this.children.length] = new PgMenu.Lvl2Node({
					owner: this,
					el: this.set,
					menuData: this.menuData.children[i]
				});
			}

			if (!this.defaultNode)
				this.defaultNode = this.children[0];
		}
	},

	onClick: function () {
		this.owner.click(this);

	},

	expand: function () {
		this.expanded = true;
		this.img.src = this.menuData.onImg;
		this.set.style.display = '';
	},

	collapse: function () {
		this.expanded = false;
		this.img.src = this.menuData.offImg;
		this.set.style.display = 'none';
	},

	on: function (shallow) {
		this.expand();
		if (this.defaultNode) {
			if (!shallow) {
				this.click(this.defaultNode);
			}
		} else {
		}
	},

	off: function () {
		this.collapse();

		for (var i = 0; i < this.children.length; ++i)
			this.children[i].off();
	},

	click: function (node, shallow) {
		for (var i = 0; i < this.children.length; ++i)
			if (node != this.children[i])
				this.children[i].off();
		node.on(shallow || false);
	}
};


/*
 * owner: owner
 * el: container
 * menuData: {name,url,children*}
 * location: location to manipulate
 */
PgMenu.Lvl2Node = function (config) {
	Ext.apply(this, config);
	this.init();
}
PgMenu.Lvl2Node.prototype = {
	init: function () {
		if (this.menuData.isDefault)
			this.owner.defaultNode = this;

		this.li = Ext.fly(this.el).createChild({
			tag: 'li',
			cls: 'divleft2-li' + ' ' + (this.menuData.hidden ? 'pgmenu-hidden' : ''),
			children: [{
					tag: 'div',
					cls: 'divleft2-div',
					children: [{
							tag: 'img',
							src: Ext.BLANK_IMAGE_URL,
							cls: this.menuData.children ? 'divleft2-img-collapsed' : 'divleft2-img-grey'
						}, {
							tag: 'span',
							cls: 'divleft2-span',
							html: this.menuData.name
				}]}, {
					tag: 'ul',
					cls: 'divleft2-ul',
					style: 'display: none;'
		}]}).dom;
		this.div = this.li.firstChild;
		this.img = this.div.firstChild;
		this.span = this.div.childNodes[1];
		this.ul = this.li.childNodes[1];

		Ext.fly(this.div).on('mouseover', this.onOver, this);
		Ext.fly(this.div).on('mouseout', this.onOut, this);
		this.expanded = false;
		this.children = [];
		if (this.menuData.children) {
			Ext.fly(this.img).on('click', this.onImgClick, this);

			for (var i = 0; this.menuData.children && i < this.menuData.children.length; ++i) {
				this.children[this.children.length] = new PgMenu.Lvl3Node({
					owner: this,
					el: this.ul,
					menuData: this.menuData.children[i]
				});

			if (!this.defaultNode)
				this.defaultNode = this.children[0];
			}
		}
		Ext.fly(this.div).on('click', this.onClick, this);

	},

	onImgClick: function (e) {
		e.stopPropagation(  );
		this.toggle();
	},

	toggle: function () {
		if (this.expanded) {
			this.collapse();
		} else {
			this.expand();
		}
	},

	collapse: function () {
		this.expanded = false;
		Ext.fly(this.img).removeClass('divleft2-img-expanded');
		this.ul.style.display = 'none';
	},

	expand: function () {
		if (!this.children.length)
			return;
		this.expanded = true;
		Ext.fly(this.img).addClass('divleft2-img-expanded');
		this.ul.style.display = '';
	},

	onOver: function () {
		Ext.fly(this.div).addClass('divleft2-div-over');
	},

	onOut: function () {
		Ext.fly(this.div).removeClass('divleft2-div-over');
	},

	onClick: function (e) {
		this.owner.click(this);
		e.stopPropagation();
	},

	on: function (shallow, nojump) {
		this.expand();
		if (this.defaultNode) {
			if (!shallow)
				this.click(this.defaultNode);
		} else {
			if (!nojump)
				PgMenu.jump(this.menuData.url, this);
			Ext.fly(this.div).addClass('divleft2-div-on');
		}
		if (this.menuData.help)
			PgMenu.helper.setHelp(this.menuData.help);
	},

	off: function () {
		this.collapse();
		Ext.fly(this.div).removeClass('divleft2-div-on');

		for (var i = 0; i < this.children.length; ++i)
			this.children[i].off();
	},

	click: function (node, shallow) {
		for (var i = 0; i < this.children.length; ++i)
			if (node != this.children[i])
				this.children[i].off();
		node.on();

		this.owner.click(this, true);
	}
};


/*
 * el: container
 * menuData: {name, url, icon}
 */
PgMenu.Lvl3Node = function (config) {
	Ext.apply(this, config);
	this.init();
}
PgMenu.Lvl3Node.prototype = {
	defaultIcon : '/images/new/filefold.gif',

	init: function () {
		if (this.menuData.isDefault)
			this.owner.defaultNode = this;

		this.li = Ext.fly(this.el).createChild({
			tag: 'li',
			cls: 'pgmenu-lvl3-li' + ' ' + (this.menuData.hidden ? 'pgmenu-hidden' : ''),
			children: [{
					tag: 'div',
					cls: 'pgmenu-lvl3-div',
					children: [{
							tag: 'img',
							cls: 'pgmenu-lvl3-img',
							src: this.menuData.icon || this.defaultIcon
						}, {
							tag: 'span',
							cls: 'pgmenu-lvl3-span',
							html: this.menuData.name
		}]}]}).dom;
		this.div = this.li.firstChild;
		this.img = this.div.childNodes[0];
		this.span = this.div.childNodes[1];

		Ext.fly(this.div).on('mouseover', this.onOver, this);
		Ext.fly(this.div).on('mouseout', this.onOut, this);
		Ext.fly(this.div).on('click', this.onClick, this);
	},

	onClick: function () {
		this.owner.click(this);
	},

	on: function (nojump) {
		Ext.fly(this.div).addClass('pgmenu-lvl3-div-on');
		if (!nojump)
			PgMenu.jump(this.menuData.url, this);
		if (this.menuData.help)
			PgMenu.helper.setHelp(this.menuData.help);
	},

	off: function () {
		Ext.fly(this.div).removeClass('pgmenu-lvl3-div-on');
	},

	onOver: function () {
		Ext.fly(this.div).addClass('pgmenu-lvl3-div-over');
	},

	onOut: function () {
		Ext.fly(this.div).removeClass('pgmenu-lvl3-div-over');
	}
};