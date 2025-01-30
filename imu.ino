#include "Arduino_BMI270_BMM150.h"

float accX, accY, accZ;
float gyroX, gyroY, gyroZ;
float degreesX = 0, degreesY = 0, degreesZ = 0;
unsigned long lastTime;

void setup() {
  Serial.begin(9600);
  while (!Serial);
  Serial.println("Started");

  if (!IMU.begin()) {
    Serial.println("Failed to initialize IMU!");
    while (1);
  }

  lastTime = millis(); // Inicializa o tempo
}

void loop() {
  unsigned long currentTime = millis();
  float deltaTime = (currentTime - lastTime) / 1000.0; // Converte para segundos
  lastTime = currentTime;

  if (IMU.accelerationAvailable() && IMU.gyroscopeAvailable()) {
    IMU.readAcceleration(accX, accY, accZ);
    IMU.readGyroscope(gyroX, gyroY, gyroZ);

    // Roll e Pitch pelo acelerômetro (faixa de -180° a 180°)
    float roll = atan2(accY, sqrt(accX * accX + accZ * accZ)) * 180 / PI;
    float pitch = atan2(accX, sqrt(accY * accY + accZ * accZ)) * 180 / PI;

    // Yaw pelo giroscópio (integração ao longo do tempo)
    degreesX += gyroX;
    degreesY += gyroY;
    degreesZ += gyroZ;

    // Exibir os valores de Roll, Pitch e Yaw
    Serial.print("Roll (X): "); Serial.print(degreesX); Serial.print("° ");
    Serial.print("Pitch (Y): "); Serial.print(degreesY); Serial.print("° ");
    Serial.print("Yaw (Z): "); Serial.print(degreesZ); Serial.println("°");

    delay(100); // Pequeno delay para leitura
  }
}
