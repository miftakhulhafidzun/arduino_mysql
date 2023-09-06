<html>
<body>
<?php

$dbname = 'distance';
$dbuser = 'root';
$dbpass = 'root';
$dbhost = 'localhost';

$connect = @mysqli_connect($dbhost, $dbuser, $dbpass, $dbname);

if (!$connect) {
    echo "Error: " . mysqli_connect_error();
    exit();
}

echo "Connection Success!<br><br>";

// Assuming you're receiving distance data from the HC-SR04 sensor
$distance = $_GET["distance_cm"]; // Update the key to match your Arduino code

$query = "INSERT INTO distance (distance_cm) VALUES ('$distance')";
$result = mysqli_query($connect, $query);

if ($result) {
    echo "Insertion Success!<br>";
} else {
    echo "Insertion Failed: " . mysqli_error($connect) . "<br>";
}

mysqli_close($connect);

?>
</body>
</html>
