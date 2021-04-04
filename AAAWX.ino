/************************************************************************************************************************
**	@File: AAAWX.ino
**	@Authors: Limor Fried & Kevin Townsend for Adafruit Industries, Hans Huth, John Cape for Argent Data Systems, Inc.
**	Collected and rewritten by Todd Chevrier
**
**	@copyright Argent Data Systems, Inc. - All rights reserved
**	@copyright Adafruit Industries
**
**	Arduino Code for: NANO 33 IoT
**	Adafruit Industries BME280 Humidity, Temperature & Pressure Sensor -- Nano 33 Iot Pinout: SDA - A4/D18    SCL - A5/D19
**	Adafruit Industries GUVA Analog UV Light Sensor					   -- Nano 33 Iot Pinout: D5
**
**	Argent Data Systems Weather Station (Anemometer, Wind Vane and Rain Gauge) -- Nano 33 Iot Pinout: A=A7 WV=A6 RG=A3
**	The anemometer and the rain gauge each require a digital input pin that can be used as an interrupt
**	The wind vane requires an analog input pin.
**
**
 *************************************************************************************************************************/
#include <WiFiNINA.h>
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <ADSWeather.h>

Adafruit_BME280 bme; // I2C

#define ANEMOMETER_PIN A7
#define VANE_PIN A6
#define RAIN_PIN A3

#define CALC_INTERVAL 1000

char ssid[] = "xxx";  // wireless network name
char password[] = "xxxx$"; // wireless password
int status = WL_IDLE_STATUS;
WiFiClient client;

unsigned long delayTime;
unsigned long nextCalc;
unsigned long timer;

int windDir;
int windSpeed;
int rainAmmount;

int sensorExp[] = {66, 84, 93, 126, 184, 244, 287, 406, 461, 599, 630, 702, 785, 827, 886, 945};
float dirDeg[] = {112.5, 67.5, 90, 157.5, 135, 202.5, 180, 22.5, 45, 247.5, 225, 337.5, 0, 292.5, 315, 270};
char* dirCard[] = {"ESE", "ENE", "E", "SSE", "SE", "SSW", "S", "NNE", "NE", "WSW", "SW", "NNW", "N", "WNW", "NW", "W"};

int sensorMin[] = {63, 80, 89, 120, 175, 232, 273, 385, 438, 569, 613, 667, 746, 812, 869, 931};
int sensorMax[] = {69, 88, 98, 133, 194, 257, 301, 426, 484, 612, 661, 737, 811, 868, 930, 993};

int incoming = 0;
float angle = 0;

float t;
float h;
float dew;
float pres;
float r;
int uv;

ADSWeather ws1(RAIN_PIN, VANE_PIN, ANEMOMETER_PIN); //This should configure all pins correctly

String wdata = "";



/*************************************/
void setup() {
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);

  // check for the WiFi module:
  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Communication with WiFi module failed!");
    // don't continue
    while (true);
  }

  connectToAP();    // Connect to Wifi Access Point
  //printWifiStatus();

  Serial.println(F("BME280 test"));

  unsigned status;

  // default settings
  status = bme.begin();

  if (!status) {
    Serial.println("Could not find a valid BME280 sensor, check wiring, address, sensor ID!");
    Serial.print("SensorID was: 0x");
    Serial.println(bme.sensorID(), 16);
    Serial.print("        ID of 0xFF probably means a bad address, a BMP 180 or BMP 085\n");
    Serial.print("   ID of 0x56-0x58 represents a BMP 280,\n");
    Serial.print("        ID of 0x60 represents a BME 280.\n");
    Serial.print("        ID of 0x61 represents a BME 680.\n");
    while (1) delay(3);
  }

  attachInterrupt(digitalPinToInterrupt(RAIN_PIN), ws1.countRain, FALLING); //ws1.countRain is the ISR for the rain gauge.
  attachInterrupt(digitalPinToInterrupt(ANEMOMETER_PIN), ws1.countAnemometer, FALLING); //ws1.countAnemometer is the ISR for the anemometer.
  nextCalc = millis() + CALC_INTERVAL;

}

/*****************************************************************/
void printWifiStatus() {
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  IPAddress ip = WiFi.localIP(); // Device IP address
  Serial.print("IP Address: ");
  Serial.println(ip);
  Serial.println("\n");
}

void connectToAP() {
  // Try to connect to Wifi network
  while ( status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network
    status = WiFi.begin(ssid, password);

    digitalWrite(LED_BUILTIN, HIGH);
    // wait 1 second for connection:
    delay(1000);
    digitalWrite(LED_BUILTIN, LOW);
    delay(1000);
    digitalWrite(LED_BUILTIN, HIGH);
    // wait 1 second for connection:
    delay(1000);
    digitalWrite(LED_BUILTIN, LOW);
  }
  Serial.println("Connected...");
  delay(1000);                       // wait for a second

}

/***************************************************/
void loop() {

  Serial.print("\nTemperature = ");
  Serial.print(bme.readTemperature() * 1.8 + 32);
  Serial.println(" F");

  Serial.print("\nPressure = ");

  Serial.print(bme.readPressure() / 3386.39);
  Serial.println(" in.");

  Serial.print("\nHumidity = ");
  Serial.print(bme.readHumidity());
  Serial.println(" %");

  delay(delayTime);

  timer = millis();

  int rainAmmount;
  long windSpeed;
  long windDirection;
  int windGust;
  String windSpeed1;
  String windGust1;

  ws1.update(); //Call this every cycle in your main loop to update all the sensor values

  if (timer > nextCalc)
  {

    nextCalc = timer + CALC_INTERVAL;
    rainAmmount = ws1.getRain();
    windSpeed = ws1.getWindSpeed();
    windDirection = ws1.getWindDirection();
    windGust = ws1.getWindGust();

    //     windSpeed / 10 will give the interger component of the wind speed
    //     windSpeed % 10 will give the fractional component of the wind speed
    windSpeed1 = (windSpeed / 100) + String(".") + (windSpeed % 10);
    windGust1 = (windGust / 100) + String(".") + (windGust % 10);

    Serial.print("\nWind speed: ");
    Serial.print(windSpeed1);
    Serial.print("\nGusting at: ");
    Serial.print(windGust1);

    incoming = analogRead(windDir);
    for (int i = 0; i <= 15; i++) {
      if (incoming >= sensorMin[i] && incoming <= sensorMax[i]) {
        angle = dirDeg[i];
        break;
      }
    }
    Serial.print("\nWind Direction: ");
    Serial.print(angle);
    Serial.print(":");
    delay(25);

    Serial.println("");

    Serial.print("\nTotal Rain: ");
    Serial.println((float) rainAmmount / 1000);

  }


  //***Send weather DATA ***
  digitalWrite(LED_BUILTIN, HIGH);
  // wait 1 second for connection:
  delay(1000);

  h = (float) bme.readHumidity();
  t = (float) bme.readTemperature() * 1.8 + 32;
  pres = (float) ((bme.readPressure() / 3386.39) + 1.00) ;

  r = (float) rainAmmount / 1000;
  dew = (float) (t - ((100 - h) * .36));
  uv = (int) 0;

  wdata = String("temp1=") + t + String( "&hum1=") + h + String( "&dew1=") + dew + String("&pres1=") + pres + String("&windS1=") + windSpeed1 + String("&windD1=") + angle + String("&windG1=") + windGust1 + String("&HRain1=") + r + String("&uv1=") + uv;

  if (client.connect("xxx.xxx.xxx.xxx", 80)) { //Change IP Address to your server IP Address
    // Connect to SQL server
    Serial.println("connected...Sending data...");
    client.println("POST /add.php? HTTP/1.1");
    client.println("Host: xxx.xxx.xxx.xxx"); //Change IP Address to your server IP Address
    client.println("Content-Type: application/x-www-form-urlencoded");
    client.print("Content-Length: ");
    client.println(wdata.length());
    Serial.println(wdata.length());
    client.println();
    client.print(wdata);
    Serial.println(wdata);
  }
  else {
    // if you didn't get a connection to the server:
    Serial.println("connection failed");
  }
  if (client.connected()) {
    client.stop();  // DISCONNECT FROM THE SERVER
    Serial.println("DONE!");
    Serial.println("**********************>\n");
  }
  delay(1000);                       // wait for one second
  digitalWrite(LED_BUILTIN, LOW);
  delay(10000); // WAIT A TOTAL OF TEN SECONDS BEFORE SENDING AGAIN
}
