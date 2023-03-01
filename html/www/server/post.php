<!DOCTYPE html>

<head>
    <title>POST</title>
</head>


<h1>Hello this is post request !</h1>
<body style="text-align: center;">
    <?php if(isset($_GET["title"])) { ?>
        <h3>title:</h3> <p><?=$_GET["title"];?></p>
    <?php } ?>
    <?php if(isset($_GET["content"])) { ?>
        <h3>content:</h3> <p><?=$_GET["content"];?></p>
    <?php } ?>
</body>


</html>
