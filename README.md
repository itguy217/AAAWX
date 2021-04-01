# AAAWX
I found there was no weather stations setup with this Arduino Nano 33 IoT that was setup for sending sensor data to a MYSQL server and Weather Underground. 
I felt compiled to answer this call.

The .ino file references some libraries that you may need to update and if you get the AVR error, you will need to google search on how to fix the config to allow the library to function without error. It literally is a line where you need to add the nano 33 Iot to it.

***Requirements: A server that is a hardware server or a Virtual Machine (VM) with LAMP - Linux Apache MySQL PHP.
Weather measuring instruments:
[Argent's Wind / Rain Sensor Assembly, Located at https://www.argentdata.com/catalog/index.php?cPath=29   ]
[Adufruit's BME280 sensor].
[Arduino Nano 33 IoT, uses 5 volts]
The php and the css files go into your Apache /var/www/html folder***

***Adminer is a Compact Database Management system, if you don't want to use a full feature program like MYSQL Bench or MYSQL Manager.***
