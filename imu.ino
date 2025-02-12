#include "Arduino_BMI270_BMM150.h"  // Biblioteca para o sensor BMI270 e BMM150

float accX, accY, accZ;
float gyroX, gyroY, gyroZ;
float mx, my, mz; 
float roll, pitch, yaw; 
unsigned long lastTime;

void setup() {
  Serial.begin(9600);
  while (!Serial);
  Serial.println("Started");

  if (!IMU.begin()) {
    Serial.println("Failed to initialize IMU!");
    while (1);
  }

  lastTime = millis();
}

void loop() {
  unsigned long currentTime = millis();
  float deltaTime = (currentTime - lastTime) / 1000.0; // segundos
  lastTime = currentTime;

  if (IMU.accelerationAvailable() && IMU.gyroscopeAvailable() && IMU.magneticFieldAvailable()) {
    IMU.readAcceleration(accX, accY, accZ);
    IMU.readGyroscope(gyroX, gyroY, gyroZ);
    IMU.readMagneticField(mx, my, mz);
    roll = atan2(accY, accZ) * 180 / PI;
    pitch = -atan2(-accX, sqrt(accY * accY + accZ * accZ)) * 180 / PI;
    yaw +=  gyroZ * deltaTime;


    // Exibe os valores de Roll, Pitch e Yaw
    Serial.print("Roll (X): "); Serial.print(roll); Serial.print("° ");
    Serial.print("Pitch (Y): "); Serial.print(pitch); Serial.print("° ");
    Serial.print("Yaw (Z): "); Serial.print(yaw); Serial.println("°");

    delay(100);
  }
}
