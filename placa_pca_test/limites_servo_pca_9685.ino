#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

// Cria o objeto PCA9685 usando o endereço I2C padrão 0x40
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

// Definições dos parâmetros do servo
#define SERVOMIN  150 // Pulso mínimo (ajuste conforme necessário para seu servo)
#define SERVOMAX  600 // Pulso máximo (ajuste conforme necessário para seu servo)
#define NUM_SERVOS  16 // Número de servos conectados ao PCA9685

void setup() {
  
  pwm.begin(); // Inicia o PCA9685
  pwm.setPWMFreq(60); // Ajusta a frequência do PWM para 60 Hz (típico para servos)

  delay(10);
}

void loop() {
  // Move todos os servos para a posição máxima mais rapidamente
  for (uint16_t pulselen = SERVOMIN; pulselen < SERVOMAX; pulselen++) {
    for (int i = 0; i < NUM_SERVOS; i++) {
      pwm.setPWM(i, 0, pulselen);
    }
    delay(5); // Reduzido para tornar o movimento mais rápido
  }

  // Move todos os servos de volta para a posição mínima mais rapidamente
  for (uint16_t pulselen = SERVOMAX; pulselen > SERVOMIN; pulselen--) {
    for (int i = 0; i < NUM_SERVOS; i++) {
      pwm.setPWM(i, 0, pulselen);
    }
    delay(5); // Reduzido para tornar o movimento mais rápido
  }
}
