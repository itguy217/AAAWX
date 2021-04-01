<?php
   $dbhost = "localhost";
   $dbuser = "XXXX"; //Database admin username
   $dbpass = "XXXX"; //Database admin password
   $dbname = "wx_data";
   $conn = mysqli_connect($dbhost, $dbuser, $dbpass, $dbname);


   if (! $conn ){
	die("Could not Connect: " . mysqli_error());
}

$sql = "INSERT INTO tblwx_data (dtime, tempf, humidity, dewptf, baromin, wind_speed, wind_dir, wind_gust, rainfall, UV) VALUES (now(),'$_POST[temp1]','$_POST[hum1]','$_POST[dew1]','$_POST[pres1]','$_POST[windS1]','$_POST[windD1]', '$_POST[windG1]','$_POST[HRain1]','$_POST[uv1]')"; 

if (mysqli_query($conn,$sql)) {
	echo "New Record Created";
} else {
	echo "Error: " .$sql . "<br>" . mysqli_error($conn);
}
mysqli_close($conn);

//*** Send data to Weather Underground ***//
$r = file_get_contents('https://rtupdate.wunderground.com/weatherstation/updateweatherstation.php?ID=XXXXX&PASSWORD=XXXXX&dateutc=now&tempf='.$_POST[temp1].'&humidity='.$_POST[hum1].'&dewptf='.$_POST[dew1].'&baromin='.$_POST[pres1].'&windspeedmph='.$_POST[windS1].'&windgustmph='.$_POST[windG1].'&windwinddir='.$_POST[windD1].'&rainin='.$_POST[HRain1].'&uv='.$_POST[uv1].'&software=DIY_ArduinoNANO33IoT&action=updateraw&realtime=1&rtfreq=10');
echo $r;
?>
