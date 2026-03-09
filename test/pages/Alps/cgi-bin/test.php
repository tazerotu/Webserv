    
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

<div class="container">
    <h1>PHP Connectivity Test</h1>

    <!-- 1. DYNAMIC CONTENT TEST -->
    <p>
        Status: <span class="badge success">PHP IS WORKING</span><br>
        Server Time: <strong><?php echo date('Y-m-d H:i:s'); ?></strong>
    </p>

    <p>If you see the raw PHP code (like <code>&lt;?php echo...</code>) above, your server is not passing the file to the PHP interpreter.</p>

    <!-- 2. REQUEST INFORMATION -->
    <h2>Request Details</h2>
    <p>This section checks if your web server is passing the correct CGI/Environment variables.</p>
    <ul>
        <li><strong>Method:</strong> <?php echo $_SERVER['REQUEST_METHOD'] ?? 'Unknown'; ?></li>
        <li><strong>URI:</strong> <?php echo $_SERVER['REQUEST_URI'] ?? 'Unknown'; ?></li>
        <li><strong>Protocol:</strong> <?php echo $_SERVER['SERVER_PROTOCOL'] ?? 'Unknown'; ?></li>
        <li><strong>User Agent:</strong> <?php echo $_SERVER['HTTP_USER_AGENT'] ?? 'Unknown'; ?></li>
    </ul>

    <!-- 3. INPUT TEST (GET/POST) -->
    <h2>Input Test</h2>
    <p>Try adding <code>?name=Test</code> to the URL, or submit the form below.</p>
    
    <form method="POST">
        <input type="text" name="test_input" placeholder="Type something...">
        <button type="submit">Send POST Request</button>
    </form>

    <?php if (!empty($_GET)): ?>
        <h3>$_GET Data Received:</h3>
        <pre><?php print_r($_GET); ?></pre>
    <?php endif; ?>

    <?php if (!empty($_POST)): ?>
        <h3>$_POST Data Received:</h3>
        <pre><?php print_r($_POST); ?></pre>
    <?php endif; ?>

    <!-- 4. SERVER INTERNALS (DEBUGGING) -->
    <h2>Server Environment ($_SERVER)</h2>
    <p>This is exactly what your web server sent to PHP:</p>
    <pre><?php print_r($_SERVER); ?></pre>

</div>

</body>
</html>

  
