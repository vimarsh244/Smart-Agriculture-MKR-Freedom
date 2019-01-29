//Code By: Vimarsh

//#define BLYNK_PRINT Serial //comment this out to save space and ram and time

#include "VidorGraphics.h"
#include "Vidor_GFX.h"

Vidor_GFX  vdgfx;

#include <Wire.h>
//#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

#define BME_SCK 13
#define BME_MISO 12
#define BME_MOSI 11
#define BME_CS 10

#define SEALEVELPRESSURE_HPA (1013.25)

Adafruit_BME280 bme; // I2C
//Adafruit_BME280 bme(BME_CS); // hardware SPI
//Adafruit_BME280 bme(BME_CS, BME_MOSI, BME_MISO, BME_SCK); // software SPI

#include <SPI.h>
#include <WiFiNINA.h>
#include <BlynkSimpleWiFiNINA.h>
BlynkTimer timer;

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "<Auth_Token>";

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "SSID";
char pass[] = "PSK";

void setup()
{
  // Debug console
  Serial.begin(115200);

  if (!FPGA.begin()) {
    Serial.println("Initialization failed!");
    while (1) {}
  }
  int version = FPGA.version();
  Serial.print("Vidor bitstream version: ");
  Serial.println(version, HEX);
  FPGA.printConfig();

  // Ok, so we know now that the FPGA contains the extended GPIO IP
  // "Please refer to the online documentation for the actual pin assignment"
  //Which has yet not come
  // I tried to configure FPGA pin 33 to be the analog input, but it has not been made any function like that yet for FPGA lib
  analogReadResolution(10);
  pinMode(A0, INPUT);
  // The same pin can be read by the SAMD processor :)

  bool status;
  // default settings
  // (you can also pass in a Wire library object like &Wire2)
  status = bme.begin();
  if (!status) {
    Serial.println("Could not find a valid BME280 sensor, check wiring!");
    while (1);
  }

  Blynk.begin(auth, ssid, pass);
  // You can also specify server:
  //Blynk.begin(auth, ssid, pass, "blynk-cloud.com", 80);
  //Blynk.begin(auth, ssid, pass, IPAddress(192,168,1,100), 8080);

  timer.setInterval(5000L, update_);
  //FPGA.end(); //don't strop FPGA, or HDMI out will not work
}

void loop()
{
  Blynk.run();
  timer.run();

}
void update_() {
  //FPGA.begin();
  int soil_data = analogRead(A0);
  //FPGA.end();

  soil_data = map(soil_data, 800, 0, 0, 100);
  // change this mapping data after you cliberate your soil moisture sensor
  float t = bme.readTemperature();
  float h = bme.readHumidity();
  float p = bme.readPressure() / 100.0f;


  Blynk.virtualWrite(V3, soil_data);
  Blynk.virtualWrite(V5, t);
  Blynk.virtualWrite(V6, h);
  Blynk.virtualWrite(V7, p);

  Serial.println("printing data: ");
  Serial.println("temp: ");
  Serial.println(t);
  Serial.println("C ; Humidity: ");
  Serial.println(h);
  Serial.println("% ; Pressure: ");
  Serial.println(p);
  Serial.println("soil Moisture: ");
  Serial.println(soil_data);
  Serial.println("Updating Screen");

  tv(t, h, p, soil_data); //calling update screen method
}
void tv(float temp, float h, float ap, int sm) {
  //FPGA.begin();

  vdgfx.fillRect(0, 0, 640, 480, vdgfx.Yellow ());

  vdgfx.text.setColor(vdgfx.Green());
  vdgfx.text.setAlpha(187);
  vdgfx.text.setSize(2);
  vdgfx.text.setCursor(15, 40);
  vdgfx.print("Smart Agriculture");

  vdgfx.text.setCursor(15, 80);
  vdgfx.print("Stats");

  vdgfx.text.setColor(vdgfx.Blue());
  vdgfx.text.setAlpha(255);
  vdgfx.text.setSize(1);
  vdgfx.text.setCursor(15, 140);
  vdgfx.print("Temperature: ");
  vdgfx.print(temp); vdgfx.print(" C");

  vdgfx.text.setCursor(15, 200);
  vdgfx.print("Humidity: ");
  vdgfx.print(h); vdgfx.print("%");

  vdgfx.text.setCursor(15, 260);
  vdgfx.print("Air Pressure: ");
  vdgfx.print(ap); vdgfx.print(" hPa");

  vdgfx.text.setCursor(15, 320);
  vdgfx.print("Soil Moisture: ");
  vdgfx.print(sm); vdgfx.print("%");

  vdgfx.text.setColor(vdgfx.Blue());
  vdgfx.text.setAlpha(120);
  vdgfx.text.setSize(1);
  vdgfx.text.setCursor(15, 400);
  vdgfx.print("By: Vimarsh");
}
//Code By: Vimarsh
