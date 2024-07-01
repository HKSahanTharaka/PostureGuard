#include <Wire.h>
#include <MPU6050.h>

MPU6050 mpu;

// Thresholds for detecting sudden movements or changes in posture
const float movementThreshold = 2.0;  // Example threshold for detecting sudden movements (m/s^2)
const float tiltThreshold = 0.5;      // Example threshold for detecting tilt (m/s^2)

void setup() {
  Wire.begin();
  Serial.begin(115200);
  
  Serial.println("Initializing MPU6050...");
  if (!mpu.begin(MPU6050_SCALE_2000DPS, MPU6050_RANGE_2G)) {
    Serial.println("Could not find a valid MPU6050 sensor, check wiring!");
    while (1);
  }
}

void loop() {
  Vector normAccel = mpu.readNormalizeAccel();
  
  // Convert normalized values to m/s^2
  float accelX = normAccel.XAxis * 9.81;
  float accelY = normAccel.YAxis * 9.81;
  float accelZ = normAccel.ZAxis * 9.81;
  
  // Calculate magnitude of acceleration
  float accelMagnitude = sqrt(accelX * accelX + accelY * accelY + accelZ * accelZ);
  
  // Detect sudden movements
  if (accelMagnitude > movementThreshold) {
    Serial.println("Sudden movement detected!");
  }
  
  // Detect tilt (for example, leaning forward or backward)
  if (abs(accelX) > tiltThreshold) {
    Serial.println("Tilt detected!");
    if (accelX > 0) {
      Serial.println("Leaning forward");
    } else {
      Serial.println("Leaning backward");
    }
  }
  
  // Print acceleration values
  Serial.print("X Acceleration (m/s^2): ");
  Serial.print(accelX);
  Serial.print(" Y Acceleration (m/s^2): ");
  Serial.print(accelY);
  Serial.print(" Z Acceleration (m/s^2): ");
  Serial.println(accelZ);
  
  delay(100);
}
