<?php
session_start();

if (!isset($_SESSION['counter'])) {
    $_SESSION['counter'] = 1;
} else {
    $_SESSION['counter']++;
}

echo "Session ID: " . session_id() . "<br>";
echo "Counter: " . $_SESSION['counter'];
?>