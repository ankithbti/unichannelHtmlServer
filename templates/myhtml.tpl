{{%AUTOESCAPE context="HTML"}}
<html>
	<head>
		<title>Dashboard</title>

		<link href="bootstrap/css/bootstrap.min.css" rel="stylesheet" media="screen">
		<link href="bootstrap/css/bootstrap-responsive.min.css" rel="stylesheet">
		<link href="dashboard.css" rel="stylesheet">
		<meta http-equiv="Content-Type" content="text/html;charset=ISO-8859-8"> 

	</head>
	<body>
		<div class="row dashboardheader">
			<div class="span4">
				<img src="images/Citi.png" width="80px"/>
			</div>
			<div class="span8">
				<h1>Real time Monitoring Dashboard [ POC ]</h1>
			</div>
		</div>
		<hr>
		<div class="container dashboard">

			<div class="row">
				<div class="span2 giveRightBorder">
					<ul class="nav nav-pills nav-stacked">
					  <li role="presentation" id="testStats" class="active"	><a href="#">Test Stats</a></li>
					</ul>
				</div>
				<div class="span9">
					<br>
					<div id="test" style="width:80%;"></div>
				</div>
			</div>
		</div>

		<script type="text/javascript" src="js/jquery.js"></script>
		<script type="text/javascript" src="bootstrap/js/bootstrap.min.js"></script>
		<script type="text/javascript" src="js/highstock.js"></script>
                <script type="text/javascript" src="js/exporting.js"></script>
                <script type="text/javascript">


                        $(function () {

                            Highcharts.setOptions({
                                global : {
                                    useUTC : false
                                }
                            });

                            function runUdate(series){

                                window.setInterval(function () {

                                    $.ajax
                                    ({
                                        dataType: 'json',
                                        type: "GET",
                                        url: "updateValues",
                                        data: "",
                                        success: function(msg)
                                        {
                                            if(msg.length > 0){
                                                series.addPoint(msg, true, true);
                                            }
                                        }
                                    });
                                }, {{UPDATEINTERVAL}});
                            }

                            $.ajax
                            ({
                                dataType: 'json',
                                type: "GET",
                                url: "giveMeValues",
                                data: "",
                                success: function(msg)
                                {

                                    // Create the chart
                                    $('#test').highcharts('StockChart', {

                                        chart : {
                                            events : {
                                                load : function () {
                                                    runUdate(this.series[0]); 
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
                                            data : msg
                                        }]

                                    });
                                }
                            });

                        });
                </script>

	</body>
</html>

