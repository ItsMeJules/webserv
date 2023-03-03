<?php
  header('Content-Type: text/html; charset=UTF-8');
  header_remove('Content-Type');
?>

<?php
		if (isset($_POST['title']))
		{
			$cookie_name1 = "title";
			$cookie_value1 = $_POST['title'];
			setcookie($cookie_name1, $cookie_value1, time() + (86400 * 30), "/"); // 86400 = 1 day
		}
		if (isset($_POST['content']))
		{
			$cookie_name2 = "content";
			$cookie_value2 = $_POST['content'];
			setcookie($cookie_name2, $cookie_value2, time() + (86400 * 30), "/"); // 86400 = 1 day
		}
?>

<!DOCTYPE html>

<head>
    <title>POST</title>
</head>

<h1>Hello this is post request ! Voici les infos que tu as rentrees :)</h1>
<body style="text-align: center;">
    <p> Voila ton mot <?php
        if (isset($_POST["title"])) {
            echo htmlspecialchars($_POST["title"]);
        } else if (isset($COOKIE["title"])) {
            echo htmlspecialchars($_COOKIE["title"]);
        } else {
            echo "Jules";
        }
        ?>
    </p>
    <p> et la description: <?php
        if (isset($_POST["content"])) {
            echo htmlspecialchars($_POST["content"]);
        } else if (isset($COOKIE["content"])) {
            echo htmlspecialchars($_COOKIE["content"]);
        } else {
            echo "Peyron";
        }
        ?>
    </p>
    <?php if(isset($_GET["title"])) { ?>
        <h3>voila ton mot:</h3><p><?=$_GET["title"];?></p>
    <?php } ?>
    <?php if(isset($_GET["content"])) { ?>
        <h3>et la description :</h3> <p><?=$_GET["content"];?></p>
    <?php } ?>
</body>

</html>

