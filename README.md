# AAAWX
I found there was no weather stations setup with this Arduino Nano 33 IoT that was setup for sending sensor data to a MYSQL server and Weather Underground. 
I felt compiled to answer this call.

Setup your own weather station page with Weather Underground at: https://www.wunderground.com/pws/overview

The .ino file references some libraries that you may need to update and if you get the AVR error, you will need to google search on how to fix the config to allow the library to function without error. It literally is a line where you need to add the nano 33 Iot to it.

**Requirements: A server that is a hardware server or a Virtual Machine (VM) with LAMP - Linux Apache MySQL PHP.
Weather measuring instruments:
[Argent's Wind / Rain Sensor Assembly, Located at https://www.argentdata.com/catalog/index.php?cPath=29   ]
[Adufruit's BME280 sensor].
[Arduino Nano 33 IoT, uses 5 volts]**

The php and the css files go into your Apache **/var/www/html** folder

**add.php is a required file as it is the main driver for sending the sensor data to the database and weather underground.**

***Adminer is a Compact Database Management system, if you don't want to use a full feature program like MYSQL Bench or MYSQL Manager.***
***WHEN you are ready to deploy this out in the field, you MUST comment out near line 85 where it typed as **while (!Serial);** If you don't comment it out , it will only run with the serial monitor open and the USB connected. ***
