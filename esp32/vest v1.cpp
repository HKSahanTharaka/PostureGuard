#include <WiFi.h>
#include <HTTPClient.h>

// WiFi credentials
const char* ssid = "Woo";
const char* password = "Maths2016";
const char* serverName = "http://192.168.8.139:3000/data1";

// Pins
const int pulsePin = 35; // Analog pin for reading heart rate sensor data
const int flexPin1 = 33;
const int flexPin2 = 34;
const int vibrationMotorPin = 13;

// Flex sensor ranges (adjust these based on your sensors)
const int flexRange1Min = 260000;
const int flexRange1Max = 270000;
const int flexRange2Min = 260000;
const int flexRange2Max = 270000;

// Variables for sensor readings
int sensorValue = 0;
int heartRate = 0;
int flexValue1 = 0;
int flexValue2 = 0;
int adjustedFlexValue1 = 0;
int adjustedFlexValue2 = 0;

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  pinMode(vibrationMotorPin, OUTPUT);
}

void loop() {
  // Read heart rate sensor
  sensorValue = analogRead(pulsePin);
  heartRate = map(sensorValue, 0, 4095, 40, 180); // Adjust the range based on your sensor

  // Read flex sensors
  flexValue1 = analogRead(flexPin1);
  flexValue2 = analogRead(flexPin2);
  adjustedFlexValue1 = flexValue1 * 100;
  adjustedFlexValue2 = flexValue2 * 100;

  // Control vibration motor based on flex sensor values
  if (adjustedFlexValue1 < flexRange1Min || adjustedFlexValue1 > flexRange1Max ||
      adjustedFlexValue2 < flexRange2Min || adjustedFlexValue2 > flexRange2Max) {
    digitalWrite(vibrationMotorPin, HIGH);
  } else {
    digitalWrite(vibrationMotorPin, LOW);
  }

  // Send data to the server
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(serverName);
    http.addHeader("Content-Type", "application/json");

    String jsonData = "{\"heartRate\": " + String(heartRate) + 
                      ", \"flexSensor1\": " + String(adjustedFlexValue1) + 
                      ", \"flexSensor2\": " + String(adjustedFlexValue2) + "}";

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

  // Print heart rate to serial monitor for debugging
  Serial.print("Heart Rate: ");
  Serial.println(heartRate);

  delay(1000); // Send data every second
}
