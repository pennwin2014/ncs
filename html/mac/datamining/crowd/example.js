var mymap;

var BMapExt;
var container;
var myChart;
function refresh(param) {
  
    //if (param in mygeo)
    {
            var tmpstore = Ext.ComponentQuery.query('grid')[0].getStore();
            tmpstore.removeAll();
            var i = 0;
            var tmpdataBig2 = [];
            for (i = 0; i < tmpdataBig1.length; i++)
                if (tmpdataBig1[i][0].name.match(param) ) {
                    tmpdataBig2.push(tmpdataBig1[i]);
                    tmpstore.add([{ start: tmpdataBig1[i][0].name, stop: tmpdataBig1[i][1].name, number: tmpdataBig1[i][1].value }]);
                }
            for (i = 0; i < tmpdataBig1.length; i++)
                if ( tmpdataBig1[i][1].name.match(param)) {
                    tmpdataBig2.push(tmpdataBig1[i]);
                    tmpstore.add([{ start: tmpdataBig1[i][0].name, stop: tmpdataBig1[i][1].name, number: tmpdataBig1[i][1].value }]);
                }
            var allOverlay = mymap.getOverlays();
            for (var i = 0; i < allOverlay.length - 1; i++) {
                mymap.removeOverlay(allOverlay[i]);
            }
            var pointss = [];
            for (var i = 0; i < tmpdata2.length; i++) {
                if (tmpdata2[i].name in mygeo && parseInt(mygeo[tmpdata2[i].name][0]) >= -180 && parseInt(mygeo[tmpdata2[i].name][0]) <= 180 && parseInt(mygeo[tmpdata2[i].name][1]) >= -90 && parseInt(mygeo[tmpdata2[i].name][1]) <= 90) {
                    var point = new BMap.Point(mygeo[tmpdata2[i].name][0], mygeo[tmpdata2[i].name][1]);
                    pointss.push(point);
                    var opts = {
                        position: point,
                        offset: new BMap.Size(-20, 0)
                    }
                    var label = new BMap.Label(tmpdata2[i].name + '<br /><b><font size=3 color="red">' + tmpdata2[i].value + '</font></b>人', opts);  // 创建文本标注对象
                    label.setStyle({
                        color: "black",
                        fontSize: "1",
                        height: "35px",
                        border: 'none',
                        borderRadius: '3px',
                        backgroundColor: 'rgba(0,177,241,0.7)',
                        // lineHeight: "20px",
                        fontFamily: "微软雅黑"
                    });
                    mymap.addOverlay(label);
                }
            }

            for (var i = 0; i < tmpdataBig2.length; i++) {
                if (tmpdataBig2[i][0].name in mygeo && parseInt(mygeo[tmpdataBig2[i][0].name][0]) >= -180 && parseInt(mygeo[tmpdataBig2[i][0].name][0]) <= 180 && parseInt(mygeo[tmpdataBig2[i][0].name][1]) >= -90 && parseInt(mygeo[tmpdataBig2[i][0].name][1]) <= 90 && tmpdataBig2[i][1].name in mygeo && parseInt(mygeo[tmpdataBig2[i][1].name][0]) >= -180 && parseInt(mygeo[tmpdataBig2[i][1].name][0]) <= 180 && parseInt(mygeo[tmpdataBig2[i][1].name][1]) >= -90 && parseInt(mygeo[tmpdataBig2[i][1].name][1]) <= 90) {
                    var x1 = mygeo[tmpdataBig2[i][0].name][0];
                    var x2 = mygeo[tmpdataBig2[i][1].name][0];
                    var y1 = mygeo[tmpdataBig2[i][0].name][1];
                    var y2 = mygeo[tmpdataBig2[i][1].name][1];
                    var x = (x1 + x2 - 0.24 * y2 + 0.24 * y1) / 2;
                    var y = (y1 + y2 + 0.24 * x2 - 0.24 * x1) / 2;
                    var point = new BMap.Point(x, y);

                    var opts = {
                        position: point,
                        offset: new BMap.Size(0, 0)
                    }
                    var label = new BMap.Label(tmpdataBig2[i][1].value, opts);  // 创建文本标注对象
                    label.setStyle({
                        color: "black",
                        fontSize: "1",
                        height: "18px",
                        border: 'none',
                        borderRadius: '5px',

                        backgroundColor: 'rgba(0,255,0,0.7)',
                        // lineHeight: "20px",
                        fontFamily: "微软雅黑"
                    });
                    mymap.addOverlay(label);
                }
            }

            option = {
                color: ['red'],

                tooltip: {
                    trigger: 'item'
                },
                
                toolbox: {
                    show: false,
                    orient: 'vertical',
                    x: 'right',
                    y: 'center',
                    feature: {
                        mark: { show: true },
                        dataView: { show: true, readOnly: false },
                        restore: { show: true },
                        saveAsImage: { show: true }
                    }
                },
                dataRange: {
                    //  orient:'horizontal',
                    min: 0,
                    max: 200,
                    range: {
                        start: 0,
                        end: 100
                    },
                    x: 'right',

                    calculable: true,
                    color: ['#360802', '#ff7302', '#f3a604'],
                    textStyle: {
                        color: '#000'
                    }
                }, credits: {
                    enabled: false
                },


                series: [
                    {
                        name: '各场所当前人数',
                        type: 'map',
                        mapType: 'none',
                        data: [],
                        geoCoord: mygeo,
                        markPoint: {
                            //symbol: 'circle',
                            normal: {
                                label: { show: false }
                            },
                            symbolSize: 10,
                            effect: {
                                show: false,
                                shadowBlur: 0
                            },
                            tooltip: {
                                trigger: 'item',
                                formatter: '{b} 当前{c}人'
                            },
                            itemStyle: {

                                normal: {
                                    color: 'red',
                                    label: { show: false }
                                },
                                emphasis: {
                                    label: { show: false }
                                }
                            },
                            data: tmpdata2

                        }

                    },
                    {
                        name: '各场所流动人数',
                        type: 'map',
                        mapType: 'none',
                        data: [],
                        geoCoord: [],
                        markLine: {
                            //symbol: ['none', 'circle'],
                            smooth: true,
                            effect: {
                                show: true,
                                scaleSize: 1,
                                period: 30,
                                color: '#fff',
                                shadowBlur: 10
                            },
                            tooltip: {
                                trigger: 'item',
                                formatter: '{b}:  {c}人'
                            },
                            itemStyle: {
                                normal: {
                                    borderWidth: 1,
                                    label: {
                                        show: true,
                                        position: 'outer'
                                    },
                                    lineStyle: {
                                        type: 'solid',
                                        shadowBlur: 10
                                    }
                                }
                            },
                            data: tmpdataBig2

                        }
                       
                    }
                ]
            };
            myChart.showLoading();
            setTimeout(needrefresh, 500);
            BMapExt.setOption(option, true);

            Ext.getCmp('keywords').setValue(param);
        }    
}
(function () {
    require.config({
        paths: {
            echarts: '/js/echarts/build/dist/'
        },
        packages: [
            {
                name: 'BMap',
                location: '/js/echarts/extension/BMap/src',
                main: 'main'
            }
        ]
    });

    require(
    [
        'echarts',
        'BMap',
        'echarts/chart/map'
    ],
    function (echarts, BMapExtension) {
       

        // 初始化地图
        BMapExt = new BMapExtension($('#main')[0], BMap, echarts,{
            enableMapClick: false
        });
        mymap = BMapExt.getMap();
        container = BMapExt.getEchartsContainer();
        var top_left_control = new BMap.ScaleControl({ anchor: BMAP_ANCHOR_TOP_LEFT });// 左上角，添加比例尺
        var top_left_navigation = new BMap.NavigationControl();
        var top_right_navigation = new BMap.NavigationControl({ anchor: BMAP_ANCHOR_TOP_RIGHT, type: BMAP_NAVIGATION_CONTROL_SMALL });
       // mymap.addControl(top_left_control);
       // mymap.addControl(top_left_navigation);
        mymap.addControl(top_right_navigation);
        mymap.centerAndZoom('上海');
        mymap.disableScrollWheelZoom();
        mymap.disableDoubleClickZoom();
       
       
        mymap.setMapStyle({
            styleJson: [
          {
              "featureType": "land",
              "elementType": "geometry",
              "stylers": {
                  "color": "#e7f7fc"
              }
          },
          {
              "featureType": "water",
              "elementType": "all",
              "stylers": {
                  "color": "#96b5d6"
              }
          },
          {
              "featureType": "green",
              "elementType": "all",
              "stylers": {
                  "color": "#b0d3dd"
              }
          },
          {
              "featureType": "highway",
              "elementType": "geometry.fill",
              "stylers": {
                  "color": "#a6cfcf"
              }
          },
          {
              "featureType": "highway",
              "elementType": "geometry.stroke",
              "stylers": {
                  "color": "#7dabb3"
              }
          },
          {
              "featureType": "arterial",
              "elementType": "geometry.fill",
              "stylers": {
                  "color": "#e7f7fc"
              }
          },
          {
              "featureType": "arterial",
              "elementType": "geometry.stroke",
              "stylers": {
                  "color": "#b0d5d4"
              }
          },
          {
              "featureType": "local",
              "elementType": "labels.text.fill",
              "stylers": {
                  "color": "#7a959a"
              }
          },
          {
              "featureType": "local",
              "elementType": "labels.text.stroke",
              "stylers": {
                  "color": "#d6e4e5"
              }
          },
          {
              "featureType": "arterial",
              "elementType": "labels.text.fill",
              "stylers": {
                  "color": "#374a46"
              }
          },
          {
              "featureType": "highway",
              "elementType": "labels.text.fill",
              "stylers": {
                  "color": "#374a46"
              }
          },
          {
              "featureType": "highway",
              "elementType": "labels.text.stroke",
              "stylers": {
                  "color": "#e9eeed"
              }
          },
          {
              "featureType": "label",
              "elementType": "labels.text.stroke",
              "stylers": {
                  "color": "#eeeeee"
              }
          },
          {
              "featureType": "railway",
              "elementType": "labels",
              "stylers": {}
          },
          {
              "featureType": "highway",
              "elementType": "labels",
              "stylers": {
                  "visibility": "off"
              }
          },
          {
              "featureType": "subway",
              "elementType": "all",
              "stylers": {
                  "lightness": -20,
                  "saturation": -61
              }
          },
          {
              "featureType": "label",
              "elementType": "labels.text.fill",
              "stylers": {
                  "color": "#000000"
              }
          }
            ]
        });
      
         option = {};
        myChart = BMapExt.initECharts(container);
        window.onresize = myChart.onresize;
        BMapExt.setOption(option);
        myChart.showLoading();
        setTimeout(needrefresh, 500);
        var ecConfig = require('echarts/config');
        function eConsole(param) {    
            if (param.type == 'click') {
                refresh(param.name);     
            }
        }
      
        myChart.on(ecConfig.EVENT.CLICK, eConsole);
        myChart.on(ecConfig.EVENT.DBLCLICK, eConsole);
        //myChart.on(ecConfig.EVENT.HOVER, eConsole);
        myChart.on(ecConfig.EVENT.DATA_ZOOM, eConsole);
        myChart.on(ecConfig.EVENT.LEGEND_SELECTED, eConsole);
        myChart.on(ecConfig.EVENT.MAGIC_TYPE_CHANGED, eConsole);
        myChart.on(ecConfig.EVENT.DATA_VIEW_CHANGED, eConsole);
    }
);
})();