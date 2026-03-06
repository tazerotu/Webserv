<?php
session_start();

$message = "";

if ($_SERVER['REQUEST_METHOD'] === 'POST') {

    // =========================
    // COOKIE ACTIONS
    // =========================

    if (isset($_POST['addCookie']) && !empty($_POST['inputData'])) {
        $value = htmlspecialchars($_POST['inputData'], ENT_QUOTES, 'UTF-8');
        setcookie("userInput", $value, time() + (7 * 24 * 60 * 60), "/");
        $message = "Cookie has been set!";
    }

    if (isset($_POST['readCookie'])) {
        if (isset($_COOKIE['userInput'])) {
            $message = "Cookie value: " . htmlspecialchars($_COOKIE['userInput'], ENT_QUOTES, 'UTF-8');
        } else {
            $message = "No cookie found.";
        }
    }

    // =========================
    // DELETE ALL
    // =========================

    if (isset($_POST['deleteAll'])) {
        setcookie("userInput", "", time() - 3600, "/");
        $message = "Cookie deleted.";
    }
}
?>

<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8" />
    <meta name="viewport" content="width=device-width, initial-scale=1" />
    <title>Cottage in the Alps</title>
    <link href="https://cdn.jsdelivr.net/npm/bootstrap@5.3.0/dist/css/bootstrap.min.css" rel="stylesheet" />
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
                <!-- Left side -->
                <div class="d-flex align-items-center">
                    <span class="navbar-brand mb-0">🏔️ Alpine Cottage</span>
                    <a href="/index.html" class="nav-link text-light ms-3">Home</a>
                </div>

                <!-- Right side -->
                <div class="d-flex align-items-center ms-auto">
                    <a href="/cgi.php" class="nav-link text-light">Test</a>
                </div>
            </div>
        </nav>
    </header>

    <!-- Main Content -->
    <main class="container-fluid">
        <h1 class="text-center my-5">CGI Test Page</h1>
        <div class="text-center">
			<form method="post" class="d-inline-block">
				<div class="mb-3">
					<label for="inputData" class="form-label">Enter some data:</label>
					<input type="text" class="form-control" id="inputData" name="inputData" />
				</div>

				<!-- Cookie Buttons -->
				<button type="submit" name="addCookie" class="btn btn-success m-1">
					Add Cookie
				</button>

				<button type="submit" name="readCookie" class="btn btn-primary m-1">
					Read Cookie
				</button>

				<!-- Delete -->
				<button type="submit" name="deleteAll" class="btn btn-danger m-1">
					Delete All
				</button>
			</form>

			<?php if (!empty($message)): ?>
				<div class="mt-3">
					<p><strong><?php echo $message; ?></strong></p>
				</div>
			<?php endif; ?>
		</div>
    </main>

    <script src="https://cdn.jsdelivr.net/npm/bootstrap@5.3.0/dist/js/bootstrap.bundle.min.js"></script>
</body>
</html>
