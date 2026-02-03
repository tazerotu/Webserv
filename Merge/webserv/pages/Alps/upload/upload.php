<!DOCTYPE html>
<html>
<head>
    <title>Upload File</title>
</head>
<body>
    <h2>Upload a File</h2>
	.jpg, .jpeg, .png, .gif, .pdf only <br><br>
    <form action="upload.php" method="POST" enctype="multipart/form-data">
        <input type="file" name="uploadedFile" required>
        <br><br>
        <input type="submit" name="uploadBtn" value="Upload">
        <?php
	if (isset($_POST['uploadBtn']) && isset($_FILES['uploadedFile'])) {
		$fileName = $_FILES['uploadedFile']['name'];
		$fileTmpName = $_FILES['uploadedFile']['tmp_name'];
		$fileSize = $_FILES['uploadedFile']['size'];
		$fileError = $_FILES['uploadedFile']['error'];
		$fileType = $_FILES['uploadedFile']['type'];

		// Optional: Limit file types
		$allowed = ['jpg', 'jpeg', 'png', 'gif', 'pdf', 'txt'];
		$fileExt = strtolower(pathinfo($fileName, PATHINFO_EXTENSION));

		if (in_array($fileExt, $allowed)) {
			if ($fileError === 0) {
				if ($fileSize < 5 * 1024 * 1024) { // 5MB limit
					$uploadDir = 'uploads/';
					if (!is_dir($uploadDir)) {
						mkdir($uploadDir, 0777, true); // Create folder if not exists
					}

					$newFileName = uniqid('', true) . "." . $fileExt;
					$uploadPath = $uploadDir . $newFileName;

					if (move_uploaded_file($fileTmpName, $uploadPath)) {
						echo "File uploaded successfully: $newFileName";
					} else {
						echo "Failed to move uploaded file.";
					}
				} else {
					echo "File is too large. Max 5MB allowed.";
				}
			} else {
				echo "Error during file upload: $fileError";
			}
		} else {
			echo "Invalid file type. Allowed types: " . implode(', ', $allowed);
		}
	} else {
		echo "No file uploaded.";
	}
	?>
    </form>
	
</body>
</html>
