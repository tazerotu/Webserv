<?php
session_start();

// Handle actions
if (isset($_GET['set_session'])) {
    $_SESSION['test_session'] = "Session created at " . date("H:i:s");
}

if (isset($_GET['delete_session'])) {
    unset($_SESSION['test_session']);
}

if (isset($_GET['set_cookie'])) {
    setcookie("test_cookie", "Cookie created at " . date("H:i:s"), time() + 3600);
}

if (isset($_GET['delete_cookie'])) {
    setcookie("test_cookie", "", time() - 3600);
}
?>

<!DOCTYPE html>
<html>
<head>
    <title>PHP Cookie & Session Test</title>
    <style>
        body { font-family: Arial; margin: 40px; }
        button { padding:10px; margin:5px; }
        .box { border:1px solid #ccc; padding:15px; margin-top:20px; }
    </style>
</head>
<body>

<h1>PHP Cookie & Session Tester</h1>

<h2>Actions</h2>

<a href="?set_session=1"><button>Set Session</button></a>
<a href="?delete_session=1"><button>Delete Session</button></a>

<a href="?set_cookie=1"><button>Set Cookie</button></a>
<a href="?delete_cookie=1"><button>Delete Cookie</button></a>

<a href="?"><button>Refresh</button></a>

<div class="box">
<h2>Session Data</h2>
<?php
if (isset($_SESSION['test_session'])) {
    echo "Session value: " . $_SESSION['test_session'];
} else {
    echo "No session value set.";
}
?>
</div>

<div class="box">
<h2>Cookie Data</h2>
<?php
if (isset($_COOKIE['test_cookie'])) {
    echo "Cookie value: " . $_COOKIE['test_cookie'];
} else {
    echo "No cookie value set.";
}
?>
</div>

<div class="box">
<h2>Debug</h2>
<pre>
SESSION:
<?php print_r($_SESSION); ?>

COOKIE:
<?php print_r($_COOKIE); ?>
</pre>
</div>

</body>
</html>