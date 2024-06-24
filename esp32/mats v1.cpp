#include <WiFi.h>
#include <HTTPClient.h>
#include <HX711.h>

const char* ssid = "Woo";
const char* password = "Maths2016";
const char* serverName = "http://192.168.8.139:3000/data2";

const int NUM_LOADCELLS = 8;
const int NUM_READINGS = 1;

const int LOADCELL_DOUT[] = { 15, 4, 18, 21, 13, 14, 26, 33 };
const int LOADCELL_SCK[] = { 2, 5, 19, 22, 12, 27, 25, 32 };

const float CALIBRATION_FACTORS[] = {
  0.0000097037, 0.0000103885, 0.0000088292, 0.0000087307, 
  0.0000090520, 0.0000094489, 0.0000091757, 0.0000081239
};

const long TARE_VALUES[] = {
  96655, -106733, -354926, 752957, 
  53157, -155956, 102065, -432513
};

HX711 loadcells[NUM_LOADCELLS];

const int BUZZER_PIN = 23;
const float WEIGHT_DIFFERENCE_THRESHOLD = 6.0;

unsigned long previousMillis = 0;
const long interval = 500;

float leftMatWeight = 0;
float rightMatWeight = 0;

void initHX711(int index);
float readCalibratedValue(int index);

void setup() {
  Serial.begin(115200);
  delay(1000);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  for (int i = 0; i < NUM_LOADCELLS; i++) {
    initHX711(i);
  }

  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);
}

void loop() {
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    leftMatWeight = 0;
    for (int i = 0; i < 4; i++) {
      leftMatWeight += readCalibratedValue(i);
    }

    rightMatWeight = 0;
    for (int i = 4; i < 8; i++) {
      rightMatWeight += readCalibratedValue(i);
    }

    if (leftMatWeight < 0.1) {
      leftMatWeight = 0;
    }
    if (rightMatWeight < 0.1) {
      rightMatWeight = 0;
    }

    if (WiFi.status() == WL_CONNECTED) {
      HTTPClient http;
      http.begin(serverName);
      http.addHeader("Content-Type", "application/json");

      String jsonData = "{\"leftMatWeight\": " + String(leftMatWeight, 6) + 
                        ", \"rightMatWeight\": " + String(rightMatWeight, 6) + "}";

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
  }

  float weightDifference = abs(leftMatWeight - rightMatWeight);
  if (weightDifference > WEIGHT_DIFFERENCE_THRESHOLD) {
    digitalWrite(BUZZER_PIN, HIGH);
  } else {
    digitalWrite(BUZZER_PIN, LOW);
  }
}

void initHX711(int index) {
  loadcells[index].begin(LOADCELL_DOUT[index], LOADCELL_SCK[index]);
  loadcells[index].set_scale(CALIBRATION_FACTORS[index]);
  loadcells[index].tare(TARE_VALUES[index]);
}

float readCalibratedValue(int index) {
  long sum = 0;
  for (int i = 0; i < NUM_READINGS; i++) {
    sum += loadcells[index].read();
  }
  long average = sum / NUM_READINGS;
  float weight = (average - TARE_VALUES[index]) * CALIBRATION_FACTORS[index];
  return weight;
}
