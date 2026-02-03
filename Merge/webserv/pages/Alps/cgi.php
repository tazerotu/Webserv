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
            <form action="" method="get" class="d-inline-block">
                <div class="mb-3">
                    <label for="inputData" class="form-label">Enter some data:</label>
                    <input type="text" class="form-control" id="inputData" name="inputData" required />
                </div>
                <button type="submit" class="btn btn-primary">Submit</button>
            </form>

            <?php
            if ($_SERVER['REQUEST_METHOD'] === 'GET' && !empty($_GET['inputData'])) {
                $input = htmlspecialchars($_GET['inputData'], ENT_QUOTES, 'UTF-8');
                echo "<div class=\"mt-3\"><p><strong>You entered:</strong> $input</p></div>";
            }
            ?>
        </div>
    </main>

    <script src="https://cdn.jsdelivr.net/npm/bootstrap@5.3.0/dist/js/bootstrap.bundle.min.js"></script>
</body>
</html>
