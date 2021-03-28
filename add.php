<?php
   $dbhost = "localhost";
   $dbuser = "xxx";
   $dbpass = "xxxx";
   $dbname = "wx_data";
   $conn = mysqli_connect($dbhost, $dbuser, $dbpass, $dbname);

echo "DB TEST\n";

   if (! $conn ){
	die("Could not Connect: " . mysqli_error());
}

$sql = "INSERT INTO tblwx_data (dtime, ambient_temp, humidity, dew, air_pres, alt, wind_speed, wind_dir, wind_gust, rainfall, uv) VALUES (now(),'$_POST[temp1]','$_POST[hum1]','$_POST[dew1]','$_POST[pres1]','$_POST[alt1]','$_POST[windS1]','$_POST[windD1]', '$_POST[windG1]','$_POST[HRain1]','$_POST[uv1]')"; 

if (mysqli_query($conn,$sql)) {
	echo "New Record Created";
} else {
	echo "Error: " .$sql . "<br>" . mysqli_error($conn);
}
mysqli_close($conn);
?>
