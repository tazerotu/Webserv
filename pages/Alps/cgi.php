<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Cottage in the Alps</title>
    <link href="https://cdn.jsdelivr.net/npm/bootstrap@5.3.0/dist/css/bootstrap.min.css" rel="stylesheet">
    <style>
        body {
            display: flex;
            flex-direction: column;
            min-height: 100vh;
        }
        main {
            flex: 1;
        }
        .carousel-item img {
            height: 500px;
            object-fit: cover;
        }
        footer {
            background-color: #343a40;
            color: white;
            padding: 30px 0;
            margin-top: 50px;
        }
    </style>
</head>
<body>
    <!-- Header -->
    <header>
        <nav class="navbar navbar-expand-lg navbar-dark bg-dark">
            <div class="container-fluid">
                <span class="navbar-brand">🏔️ Alpine Cottage</span>
                <a href="/index.html" class="nav-link text-light">Home</a>
                <a href="/cgi.php" class="nav-link text-light">Test</a>
            </div>
        </nav>
    </header>

    <!-- Main Content -->
    <main class="container-fluid">
		<h1 class="text-center my-5">CGI Test Page</h1>
		<div class="text-center">
			<form action="/cgi-bin/test.cgi" method="get" class="d-inline-block">
				<div class="mb-3">
					<label for="inputData" class="form-label">Enter some data:</label>
					<input type="text" class="form-control" id="inputData" name="inputData" required>
				</div>
				<button type="submit" class="btn btn-primary">Submit</button>
			</form>
			<?php if ($_SERVER['REQUEST_METHOD'] === 'get' && isset($_GET['inputData'])): ?>
				<div class="mt-3">
					<p><strong>You entered:</strong> <?php echo htmlspecialchars($_GET['inputData']); ?></p>
				</div>
			<?php endif; ?>
		</div>
    </main>
    <script src="https://cdn.jsdelivr.net/npm/bootstrap@5.3.0/dist/js/bootstrap.bundle.min.js"></script>
</body>
</html>