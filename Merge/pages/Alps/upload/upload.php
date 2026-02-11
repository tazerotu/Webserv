<!DOCTYPE html>
<html>
<head>
    <title>Upload File</title>
	<link href="https://cdn.jsdelivr.net/npm/bootstrap@5.3.0/dist/css/bootstrap.min.css" rel="stylesheet">
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
			<a href="/cgi-bin/test.php" class="nav-link text-light">Test</a>
		</div>
		</div>
	</nav>
	</header>

<body>
    <h2>Upload a File</h2>
    .jpg, .jpeg, .png, .gif, .pdf only <br><br>

    <?php
    $uploadDir = 'uploads/';

    // Create the folder if it doesn't exist
    if (!is_dir($uploadDir)) {
        mkdir($uploadDir, 0777, true);
    }

    // Handle file deletion
    if (isset($_POST['deleteFile'])) {
        $fileToDelete = basename($_POST['deleteFile']); // sanitize input
        $filePath = $uploadDir . $fileToDelete;

        if (file_exists($filePath)) {
            unlink($filePath);
            echo "<p style='color:green;'>Deleted file: $fileToDelete</p>";
        } else {
            echo "<p style='color:red;'>File not found!</p>";
        }
    }

    // Handle file upload
    if (isset($_POST['uploadBtn']) && isset($_FILES['uploadedFile'])) {
        $fileName = $_FILES['uploadedFile']['name'];
        $fileTmpName = $_FILES['uploadedFile']['tmp_name'];
        $fileSize = $_FILES['uploadedFile']['size'];
        $fileError = $_FILES['uploadedFile']['error'];
        $fileType = $_FILES['uploadedFile']['type'];

        $allowed = ['jpg', 'jpeg', 'png', 'gif', 'pdf', 'txt'];
        $fileExt = strtolower(pathinfo($fileName, PATHINFO_EXTENSION));

        if (in_array($fileExt, $allowed)) {
            if ($fileError === 0) {
                if ($fileSize < 5 * 1024 * 1024) { // 5MB
                    $newFileName = uniqid('', true) . "." . $fileExt;
                    $uploadPath = $uploadDir . $newFileName;

                    if (move_uploaded_file($fileTmpName, $uploadPath)) {
                        echo "<p style='color:green;'>File uploaded successfully: $newFileName</p>";
                    } else {
                        echo "<p style='color:red;'>Failed to move uploaded file.</p>";
                    }
                } else {
                    echo "<p style='color:red;'>File is too large. Max 5MB allowed.</p>";
                }
            } else {
                echo "<p style='color:red;'>Error during file upload: $fileError</p>";
            }
        } else {
            echo "<p style='color:red;'>Invalid file type. Allowed types: " . implode(', ', $allowed) . "</p>";
        }
    }

    // List files in the folder
    $files = array_diff(scandir($uploadDir), ['.', '..']);
	?>

	<form action="" method="POST" enctype="multipart/form-data">
        <input type="file" name="uploadedFile" required>
        <br><br>
        <input type="submit" name="uploadBtn" value="Upload">
    </form>

	<br><br>

	<?php
    if (!empty($files)) {
        echo "<h3>Uploaded Files</h3>";
        echo "<table border='1' cellpadding='5'>";
        echo "<tr><th>File Name</th><th>Action</th></tr>";
        foreach ($files as $file) {
            echo "<tr>";
            echo "<td>$file</td>";
            echo "<td>
                    <form method='POST' style='margin:0;'>
                        <input type='hidden' name='deleteFile' value='$file'>
                        <input type='submit' value='Delete' onclick='return confirm(\"Are you sure?\")'>
                    </form>
                  </td>";
            echo "</tr>";
        }
        echo "</table>";
    } else {
        echo "<p>No files uploaded yet.</p>";
    }
    ?>

    

</body>
</html>
