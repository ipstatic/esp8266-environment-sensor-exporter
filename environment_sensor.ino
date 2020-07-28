#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

#include "arduino_secrets.h"

#define BME_SCK 13
#define BME_MISO 12
#define BME_MOSI 11
#define BME_CS 10

#define SEALEVELPRESSURE_HPA (1013.25)

const char* ssid  = SECRET_SSID;
const char* pass  = SECRET_PSK;
const char* hstname = SECRET_HOSTNAME;
const char* sensor = SECRET_SENSOR;

Adafruit_BME280 bme; // I2C

unsigned long delayTime;

ESP8266WebServer server(8266);

void setup() {
  Serial.begin(115200);
  delay(10);
  Serial.println('\n');
  Serial.print("Sensor #");
  Serial.println(sensor);

  WiFi.hostname(hstname);
  WiFi.begin(ssid, pass);
  Serial.print("Connecting to ");
  Serial.print(ssid);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }

  Serial.println('\n');
  Serial.println("Connection established!");
  Serial.print("IP address:\t");
  Serial.println(WiFi.localIP());

  unsigned status;
  status = bme.begin(0x76, &Wire);
  //status = bme.begin();
  if (!status) {
    Serial.println("Could not find a valid BME280 sensor, check wiring, address, sensor ID!");
    while (1) delay(10);
  }

  server.on("/", handleRoot);
  server.on("/metrics", handleMetrics);
  server.onNotFound(handleNotFound);
  server.begin();
  Serial.println("HTTP server started");

  delayTime = 1000;
  Serial.println();
}

void loop() {
  server.handleClient();
  delay(delayTime);
}

void handleRoot() {
  server.send(200, "text/html", "<html><head><title>ESP8266 Environment Sensor Exporter</title></head><body><h1>ESP8266 Environment Sensor Exporter</h1><p><a href='/metrics'>Metrics</a></p></body></html>");
}

void handleMetrics() {
  server.send(200, "text/plain", collectSensor());
}

void handleNotFound(){
  server.send(404, "text/plain", "404: Not found");
}

String collectSensor() {
  Serial.println("sensor reading requested");
  String temp = String(bme.readTemperature());
  String pressure = String((bme.readPressure() / 100.0F));
  String rh = String(bme.readHumidity());
  Serial.println("Temperature: " + temp + " *C");
  Serial.println("Pressure: " + pressure + " hPa");
  Serial.println("RH: " + rh + "%");
  Serial.println();
  
  String out = "# HELP esp8266_temperature_celsius Current temperature in Celsius.\n";
  out += "# TYPE esp8266_temperature_celsius gauge\n";
  out += "esp8266_temperature_celsius{sensor=\"" + String(sensor) + "\"} " + temp + "\n";

  out += "# HELP esp8266_pressure_hpa Current pressure in hPa.\n";
  out += "# TYPE esp8266_pressure_hpa gauge\n";
  out += "esp8266_pressure_hpa{sensor=\"" + String(sensor) + "\"} " + pressure + "\n";
    
  out += "# HELP esp8266_relative_humidity Current relative humidity.\n";
  out += "# TYPE esp8266_relative_humidity gauge\n";
  out += "esp8266_relative_humidity{sensor=\"" + String(sensor) + "\"} " + rh + "\n";

  return out;
}
