{{%AUTOESCAPE context="HTML"}}
<html>
    <head>
        <title>{{TITLE}}</title>
        {{#CSS}}
            <link href="{{CSSFILE}}" rel="stylesheet" media="screen">
        {{/CSS}}
    </head>
    <body>
        <header>
            <h3>{{HEADING}}</h3>
        </header>
        <div class="container">
            <div class="row">
                <div class="span6">
                    Left Section
                </div>
                <div class="span4">
                    Right Section
                </div>
            </div>
        </div>

        <footer>
            {{FOOTER}}
        </footer>
    </body>
</html>