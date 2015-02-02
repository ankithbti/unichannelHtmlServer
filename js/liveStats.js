$(function () {

	//http://jsfiddle.net/gh/get/jquery/1.9.1/highslide-software/highcharts.com/tree/master/samples/stock/demo/lazy-loading/

    //$("#liveLatencyStats").hide();
    $("#historyLatencyStats").hide();
    //liveLatencyStats
    $("#enableLiveLatencyStats").click(function(){
        $("#historyLatencyStats").hide();
        $("#enableHistoryLatencyStats").removeClass( "active" );
        $("#enableLiveLatencyStats").addClass("active");
        $("#liveLatencyStats").show();

    });

    $("#enableHistoryLatencyStats").click(function(){
        $("#liveLatencyStats").hide();
        $("#enableLiveLatencyStats").removeClass("active");
        $("#enableHistoryLatencyStats").addClass( "active" );
        $("#historyLatencyStats").show();
    });

    Highcharts.setOptions({
        global : {
            useUTC : false
        }
    });

    // Create the chart
    $('#liveLatencyStats').highcharts('StockChart', {
        chart : {
            events : {
                load : function () {

                    // set up the updating of the chart each second
                    var series = this.series[0];
                    setInterval(function () {
                        var x = (new Date()).getTime(), // current time
                            y = Math.round(Math.random() * 100);
                        series.addPoint([x, y], true, true);
                    }, 1000);
                }
            }
        },

        rangeSelector: {
            buttons: [{
                count: 1,
                type: 'minute',
                text: '1M'
            }, {
                count: 5,
                type: 'minute',
                text: '5M'
            }, {
                type: 'all',
                text: 'All'
            }],
            inputEnabled: false,
            selected: 0
        },

        title : {
            text : 'Live Latency for ita2ice Feed'
        },

        exporting: {
            enabled: false
        },

        series : [{
            name : 'Latency',
            data : (function () {
                // generate an array of random data
                var data = [], time = (new Date()).getTime(), i;

                for (i = -999; i <= 0; i += 1) {
                    data.push([
                        time + i * 1000,
                        Math.round(Math.random() * 100)
                    ]);
                }
                return data;
            }())
        }]
    });

});
