#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

const char* ssid = "Woo";
const char* password = "Maths2016";
const char* serverName = "http://192.168.8.139:3000/data1";
const char* configServerName = "http://192.168.8.139:3000/config1";

const int pulsePin = 35;
const int flexPin1 = 33;
const int flexPin2 = 34;
const int vibrationMotorPin = 13;

Adafruit_MPU6050 mpu;

int flexRange1Min = 260000;
int flexRange1Max = 270000;
int flexRange2Min = 260000;
int flexRange2Max = 270000;

int sensorValue = 0;
int heartRate = 0;
int flexValue1 = 0;
int flexValue2 = 0;
int adjustedFlexValue1 = 0;
int adjustedFlexValue2 = 0;
float linearAcceleration = 0.0;

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  pinMode(vibrationMotorPin, OUTPUT);

  Wire.begin();
  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }

  mpu.setAccelerometerRange(MPU6050_RANGE_16_G);
  mpu.setGyroRange(MPU6050_RANGE_250_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
  delay(100);
}

void loop() {
  sensorValue = analogRead(pulsePin);
  heartRate = map(sensorValue, 0, 4095, 60, 100);

  flexValue1 = analogRead(flexPin1);
  flexValue2 = analogRead(flexPin2);
  adjustedFlexValue1 = flexValue1 * 100;
  adjustedFlexValue2 = flexValue2 * 100;

  if (adjustedFlexValue1 < flexRange1Min || adjustedFlexValue1 > flexRange1Max ||
      adjustedFlexValue2 < flexRange2Min || adjustedFlexValue2 > flexRange2Max) {
    digitalWrite(vibrationMotorPin, HIGH);
  } else {
    digitalWrite(vibrationMotorPin, LOW);
  }

  sensors_event_t accel, gyro, temp;
  mpu.getEvent(&accel, &gyro, &temp);

  float a_total = sqrt(accel.acceleration.x * accel.acceleration.x +
                       accel.acceleration.y * accel.acceleration.y +
                       accel.acceleration.z * accel.acceleration.z);

  linearAcceleration = a_total - 9.8;

  if (abs(linearAcceleration) <= 0.2) {
    linearAcceleration = 0.0;
  }

  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(serverName);
    http.addHeader("Content-Type", "application/json");

    String jsonData = "{\"heartRate\": " + String(heartRate) + 
                      ", \"flexSensor1\": " + String(adjustedFlexValue1) + 
                      ", \"flexSensor2\": " + String(adjustedFlexValue2) + 
                      ", \"linearAcceleration\": " + String(linearAcceleration, 2) + "}";

    Serial.print("Sending data: ");
    Serial.println(jsonData);

    int httpResponseCode = http.POST(jsonData);

    if (httpResponseCode > 0) {
      String response = http.getString();
      Serial.println(httpResponseCode);
      Serial.println(response);
    } else {
      Serial.println("Error on sending POST");
    }
    http.end();
  }

  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(configServerName);
    int httpResponseCode = http.GET();

    if (httpResponseCode > 0) {
      String response = http.getString();
      Serial.println("Received config: " + response);
      DynamicJsonDocument doc(1024);
      deserializeJson(doc, response);
      flexRange1Min = doc["flexRange1Min"];
      flexRange1Max = doc["flexRange1Max"];
      flexRange2Min = doc["flexRange2Min"];
      flexRange2Max = doc["flexRange2Max"];
    } else {
      Serial.println("Error on getting config");
    }
    http.end();
  }

  delay(100);
}
