$(function () {

	$.ajax
        ({
        		dataType: 'json',
                type: "GET",
                url: "giveMeValues",
                data: "start={{START}}&end={{END}}",
                success: function(msg)
                {

                        // Create the chart
        				$('#test').highcharts('StockChart', {

        					title : {
				                text : 'Latency History Stats'
				            },

				            

				            series : [{
				                name : 'Latency',
				                data : msg,
				                type : 'areaspline',
				                threshold : null,
				                tooltip : {
				                    valueDecimals : 2
				                },
				                fillColor : {
				                    linearGradient : {
				                        x1: 0,
				                        y1: 0,
				                        x2: 0,
				                        y2: 1
				                    },
				                    stops : [
				                        [0, Highcharts.getOptions().colors[0]],
				                        [1, Highcharts.Color(Highcharts.getOptions().colors[0]).setOpacity(0).get('rgba')]
				                    ]
				                }
				            }]

        				});
                }
        });

	/*
    $.getJSON('test.php?', function (data) {

    	alert(data);

        // Create the chart
        $('#test').highcharts('StockChart', {


            rangeSelector : {
                selected : 0
            },

            title : {
                text : 'Latency History Stats'
            },

            series : [{
                name : 'Latency',
                data : data,
                type : 'areaspline',
                threshold : null,
                tooltip : {
                    valueDecimals : 2
                },
                fillColor : {
                    linearGradient : {
                        x1: 0,
                        y1: 0,
                        x2: 0,
                        y2: 1
                    },
                    stops : [
                        [0, Highcharts.getOptions().colors[0]],
                        [1, Highcharts.Color(Highcharts.getOptions().colors[0]).setOpacity(0).get('rgba')]
                    ]
                }
            }]
        });
    }).always(function(data) {
    	alert(data);
    });

	*/

});