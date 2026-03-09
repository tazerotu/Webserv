<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
	<link href="https://cdn.jsdelivr.net/npm/bootstrap@5.3.0/dist/css/bootstrap.min.css" rel="stylesheet">
    <title>Web Server Test</title>
    <style>
        body { font-family: sans-serif; padding: 20px; background: #f4f4f4; }
        .container { background: white; padding: 20px; border-radius: 8px; box-shadow: 0 2px 5px rgba(0,0,0,0.1); max-width: 800px; margin: auto; }
        h1 { color: #2c3e50; border-bottom: 2px solid #3498db; padding-bottom: 10px; }
        h2 { color: #e67e22; margin-top: 20px; }
        pre { background: #2d2d2d; color: #76e05e; padding: 15px; overflow-x: auto; border-radius: 4px; }
        .badge { display: inline-block; padding: 5px 10px; color: white; border-radius: 4px; font-weight: bold; }
        .success { background-color: #27ae60; }
    </style>
</head>

<header>
	<nav class="navbar navbar-expand-lg navbar-dark bg-dark">
		<div class="container-fluid">
		<!-- Left side: Brand and Home -->
		<div class="d-flex align-items-center">
			<a href="/" class="nav-link text-light ms-3">Home</a>
		</div>

		<!-- Right side: Upload and Test -->
		<div class="d-flex align-items-center ms-auto">
			<a href="/upload/upload.php" class="nav-link text-light me-3">Upload</a>
			<a href="/cgi-bin" class="nav-link text-light">Test</a>
		</div>
		</div>
	</nav>
	</header>

<body>

<?php
try {
    $test = 1 / 0;
    echo "<h3>Division by 0</h3>";
    echo "<pre>$test</pre>";
	} catch (DivisionByZeroError $e) {
		echo "<h3>Error caught</h3>";
		echo "<pre>" . $e->getMessage() . "</pre>";
}
?>

</body>
</html>
