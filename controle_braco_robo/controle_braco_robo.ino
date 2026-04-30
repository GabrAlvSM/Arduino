#define DEBUG
#include <Wire.h>

const byte I2C_ADDR = 128;

/* Servo 270° */
const int SERVO_MIN = 0;
const int SERVO_MAX = 270;
const int OUT_MIN = 1;
const int OUT_MAX = 179;

void setup() {
  Serial.begin(115200);
  Wire.begin();

  Serial.println("Controle misto: servos + continuo");
  Serial.println("Formato:");
  Serial.println("Servo normal: 0 180");
  Serial.println("Continuo: 4 -100 (velocidade)");
}

/* ========================= */

bool isContinuousServo(byte servo) {
  return (servo >= 4 && servo <= 6); // ajuste aqui
}

/* ========================= */

byte convertPosition(int angulo) {
  angulo = constrain(angulo, SERVO_MIN, SERVO_MAX);
  return map(angulo, SERVO_MIN, SERVO_MAX, OUT_MIN, OUT_MAX);
}

byte convertContinuous(int vel) {
  vel = constrain(vel, -100, 100);
  return map(vel, -100, 100, 0, 180);
}

/* ========================= */

void moveServo(byte servo, byte pos) {
  Wire.beginTransmission(I2C_ADDR);
  Wire.write(servo);
  Wire.write(pos);
  Wire.endTransmission();
}

/* ========================= */

void loop() {

  if (Serial.available()) {

    byte servo = Serial.parseInt();
    int valor = Serial.parseInt();

    byte pos;

    if (isContinuousServo(servo)) {
      pos = convertContinuous(valor);

#ifdef DEBUG
      Serial.print("[CONT] ");
#endif

    } else {
      pos = convertPosition(valor);

#ifdef DEBUG
      Serial.print("[POS] ");
#endif
    }

    moveServo(servo, pos);

#ifdef DEBUG
    Serial.print("Servo: ");
    Serial.print(servo);
    Serial.print(" Valor: ");
    Serial.print(valor);
    Serial.print(" -> ");
    Serial.println(pos);
#endif

  if (isContinuousServo(servo)) {
      delay(500);

      pos = convertContinuous(0);

      moveServo(servo, pos);
    }

    Serial.println("ok");
  }
}