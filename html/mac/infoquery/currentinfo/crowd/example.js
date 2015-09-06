
var BMapExt;
var map;
var container;
var myChart
(function () {
    require.config({
        paths: {
            echarts: '/js/echarts/doc/example/www/js'
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
        $('#main').css({
            height:$('body').height(),
            width: $('body').width()
        });

        // 初始化地图
        BMapExt = new BMapExtension($('#main')[0], BMap, echarts,{
            enableMapClick: false
        });
        map = BMapExt.getMap();
        container = BMapExt.getEchartsContainer();


        var point = new BMap.Point(121.4648, 31.2091);
        map.centerAndZoom(point, 11);
        map.enableScrollWheelZoom(true);
        // 地图自定义样式
        map.setMapStyle({
            styleJson: [
                  {
                      "featureType": "water",
                      "elementType": "all",
                      "stylers": {
                          "color": "#044161"
                      }
                  },
                  {
                      "featureType": "land",
                      "elementType": "all",
                      "stylers": {
                          "color": "#004981"
                      }
                  },
                  {
                      "featureType": "boundary",
                      "elementType": "geometry",
                      "stylers": {
                          "color": "#064f85"
                      }
                  },
                  {
                      "featureType": "railway",
                      "elementType": "all",
                      "stylers": {
                          "visibility": "off"
                      }
                  },
                  {
                      "featureType": "highway",
                      "elementType": "geometry",
                      "stylers": {
                          "color": "#004981"
                      }
                  },
                  {
                      "featureType": "highway",
                      "elementType": "geometry.fill",
                      "stylers": {
                          "color": "#005b96",
                          "lightness": 1
                      }
                  },
                  {
                      "featureType": "highway",
                      "elementType": "labels",
                      "stylers": {
                          "visibility": "off"
                      }
                  },
                  {
                      "featureType": "arterial",
                      "elementType": "geometry",
                      "stylers": {
                          "color": "#004981"
                      }
                  },
                  {
                      "featureType": "arterial",
                      "elementType": "geometry.fill",
                      "stylers": {
                          "color": "#00508b"
                      }
                  },
                  {
                      "featureType": "poi",
                      "elementType": "all",
                      "stylers": {
                          "visibility": "off"
                      }
                  },
                  {
                      "featureType": "green",
                      "elementType": "all",
                      "stylers": {
                          "color": "#056197",
                          "visibility": "off"
                      }
                  },
                  {
                      "featureType": "subway",
                      "elementType": "all",
                      "stylers": {
                          "visibility": "off"
                      }
                  },
                  {
                      "featureType": "manmade",
                      "elementType": "all",
                      "stylers": {
                          "visibility": "off"
                      }
                  },
                  {
                      "featureType": "local",
                      "elementType": "all",
                      "stylers": {
                          "visibility": "off"
                      }
                  },
                  {
                      "featureType": "arterial",
                      "elementType": "labels",
                      "stylers": {
                          "visibility": "off"
                      }
                  },
                  {
                      "featureType": "boundary",
                      "elementType": "geometry.fill",
                      "stylers": {
                          "color": "#029fd4"
                      }
                  },
                  {
                      "featureType": "building",
                      "elementType": "all",
                      "stylers": {
                          "color": "#1a5787"
                      }
                  },
                  {
                      "featureType": "label",
                      "elementType": "all",
                      "stylers": {
                          "visibility": "off"
                      }
                  }
            ]
        });

         option = {};
        myChart = BMapExt.initECharts(container);
        window.onresize = myChart.onresize;
        BMapExt.setOption(option);
    }
);
})();