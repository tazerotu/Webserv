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
                <a href="/home" class="nav-link text-light">Home</a>
                <a href="/cookies.php" class="nav-link text-light">Cookies</a>
            </div>
        </nav>
    </header>

    <!-- Main Content -->
    <main class="container-fluid">
        <!-- Carousel -->
        <div id="cottageCarousel" class="carousel slide mb-5" data-bs-ride="carousel">
            <div class="carousel-inner">
                <div class="carousel-item active">
                    <img src="asset/1.jpg" class="d-block w-100" alt="Cottage View 1">
                </div>
                <div class="carousel-item">
                    <img src="asset/2.jpg" class="d-block w-100" alt="Cottage View 2">
                </div>
                <div class="carousel-item">
                    <img src="asset/3.jpg" class="d-block w-100" alt="Cottage View 3">
                </div>
                <div class="carousel-item">
                    <img src="asset/4.jpg" class="d-block w-100" alt="Cottage View 4">
                </div>
                <div class="carousel-item">
                    <img src="asset/5.jpg" class="d-block w-100" alt="Cottage View 5">
                </div>
                <div class="carousel-item">
                    <img src="asset/6.jpg" class="d-block w-100" alt="Cottage View 6">
                </div>
                <div class="carousel-item">
                    <img src="asset/7.jpg" class="d-block w-100" alt="Cottage View 7">
                </div>
                <div class="carousel-item">
                    <img src="asset/8.jpg" class="d-block w-100" alt="Cottage View 8">
                </div>
                <div class="carousel-item">
                    <img src="asset/9.jpg" class="d-block w-100" alt="Cottage View 9">
                </div>
                <div class="carousel-item">
                    <img src="asset/10.jpg" class="d-block w-100" alt="Cottage View 10">
                </div>
                <div class="carousel-item">
                    <img src="asset/11.jpg" class="d-block w-100" alt="Cottage View 11">
                </div>
                <div class="carousel-item">
                    <img src="asset/12.jpg" class="d-block w-100" alt="Cottage View 12">
                </div>
                <div class="carousel-item">
                    <img src="asset/13.jpg" class="d-block w-100" alt="Cottage View 13">
                </div>
                <div class="carousel-item">
                    <img src="asset/14.jpg" class="d-block w-100" alt="Cottage View 14">
                </div>
                <div class="carousel-item">
                    <img src="asset/15.jpg" class="d-block w-100" alt="Cottage View 15">
                </div>
                <div class="carousel-item">
                    <img src="asset/16.jpg" class="d-block w-100" alt="Cottage View 16">
                </div>
                <div class="carousel-item">
                    <img src="asset/17.jpg" class="d-block w-100" alt="Cottage View 17">
                </div>
                <div class="carousel-item">
                    <img src="asset/18.jpg" class="d-block w-100" alt="Cottage View 18">
                </div>
                <div class="carousel-item">
                    <img src="asset/19.jpg" class="d-block w-100" alt="Cottage View 19">
                </div>
                <div class="carousel-item">
                    <img src="asset/20.jpg" class="d-block w-100" alt="Cottage View 20">
                </div>
            </div>
            <button class="carousel-control-prev" type="button" data-bs-target="#cottageCarousel" data-bs-slide="prev" aria-label="Previous">
                <span class="carousel-control-prev-icon" aria-hidden="true"></span>
            </button>
            <button class="carousel-control-next" type="button" data-bs-target="#cottageCarousel" data-bs-slide="next" aria-label="Next">
                <span class="carousel-control-next-icon" aria-hidden="true"></span>
            </button>
        </div>

        <!-- Info Section -->
        <div class="container mb-5">
            <div class="row">
                <div class="col-lg-6">
                    <h2>About Our Cottage</h2>
                    <p>Welcome to our charming cottage nestled in the heart of the Alps. Experience mountain serenity with modern comfort and breathtaking views.</p>
                    <ul class="list-unstyled">
                        <li>✓ 4 Bedrooms</li>
                        <li>✓ Modern Amenities</li>
                        <li>✓ Stunning Mountain Views</li>
                    </ul>
                </div>
                <div class="col-lg-6">
                    <h2>Location</h2>
                    <div class="embed-responsive" style="position: relative; width: 100%; padding-bottom: 60%;">
                        <iframe src="https://www.google.com/maps/embed?pb=..." style="position: absolute; top: 0; left: 0; width: 100%; height: 100%; border: none;" allowfullscreen="" loading="lazy"></iframe>
                    </div>
                </div>
            </div>
        </div>
    </main>

    <!-- Footer -->
    <footer>
        <div class="container">
            <div class="row">
                <div class="col-md-4">
                    <h5>Contact Us</h5>
                    <p>Email: info@alpinecottage.com</p>
                    <p>Phone: +1 (234) 567-8900</p>
                </div>
                <div class="col-md-4">
                    <h5>Address</h5>
                    <p>123 Alpine Street<br>Mountain Valley, 12345</p>
                </div>
                <div class="col-md-4">
                    <h5>Disclaimer</h5>
                    <p><small>All content and images on this site were sourced from external sources for demonstration purposes.</small></p>
                </div>
            </div>
            <hr class="bg-light">
            <p class="text-center"><small>&copy; 2024 Alpine Cottage. All rights reserved.</small></p>
        </div>
    </footer>

    <script src="https://cdn.jsdelivr.net/npm/bootstrap@5.3.0/dist/js/bootstrap.bundle.min.js"></script>
</body>
</html>