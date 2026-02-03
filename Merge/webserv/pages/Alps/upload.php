<?php
// pages/Alps/upload.php
$target_dir = "uploads/";
$target_file = $target_dir . basename($_FILES["fileToUpload"]["name"]);
$uploadOk = 1;

// Check if file was actually sent
if(isset($_FILES["fileToUpload"])) {
    if (move_uploaded_file($_FILES["fileToUpload"]["tmp_name"], $target_file)) {
        echo "The file ". htmlspecialchars( basename( $_FILES["fileToUpload"]["name"])). " has been uploaded.";
    } else {
        http_response_code(500);
        echo "Sorry, there was an error uploading your file.";
    }
} else {
    http_response_code(400);
    echo "No file received.";
}
?>