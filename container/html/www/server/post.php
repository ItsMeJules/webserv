<?php
  header('Content-Type: text/html; charset=UTF-8');
  header_remove('Content-Type');
?>

<!DOCTYPE html>

<head>
    <title>POST</title>
</head>

<h1>Hello this is post request ! Voici les infos que tu as rentrees :)</h1>
<body style="text-align: center;">
    <?php if(isset($_GET["title"])) { ?>
        <h3>voila ton mot:</h3> <p><?=$_GET["title"];?></p>
    <?php } ?>
    <?php if(isset($_GET["content"])) { ?>
        <h3>et la description :</h3> <p><?=$_GET["content"];?></p>
    <?php } ?>
</body>

</html>

