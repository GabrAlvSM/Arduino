#define DEBUG
#include <Wire.h>

const byte I2C_ADDR = 128;

/* Joystick */
const int PIN_X = A0;
const int PIN_Y = A1;

/* Zona morta (evita tremedeira) */
const int DEADZONE = 30;

/* Servos contínuos (ex: rodas) */
const byte SERVO_LEFT = 3;
const byte SERVO_RIGHT = 4;

/* Ajuste do neutro (IMPORTANTE!) */
const byte STOP_LEFT = 90;
const byte STOP_RIGHT = 90;

/* ========================= */

void setup() {
  Serial.begin(115200);
  Wire.begin();

  Serial.println("Controle por joystick iniciado");
}

/* ========================= */

void moveServo(byte servo, byte pos) {
  Wire.beginTransmission(I2C_ADDR);
  Wire.write(servo);
  Wire.write(pos);
  Wire.endTransmission();
}

/* ========================= */

int applyDeadzone(int value) {
  if (abs(value) < DEADZONE) return 0;
  return value;
}

/* ========================= */

void loop() {

  int x = analogRead(PIN_X) - 512;
  int y = analogRead(PIN_Y) - 512;

  x = applyDeadzone(x);
  y = applyDeadzone(y);

  // 🎮 mistura tipo tanque (diferencial)
  int left = y + x;
  int right = y - x;

  // limita
  left = constrain(left, -512, 512);
  right = constrain(right, -512, 512);

  // converte para servo contínuo
  byte leftCmd = map(left, -512, 512, 0, 180);
  byte rightCmd = map(right, -512, 512, 0, 180);

  // aplica neutro ajustado
  if (left == 0) leftCmd = STOP_LEFT;
  if (right == 0) rightCmd = STOP_RIGHT;

  moveServo(SERVO_LEFT, leftCmd);
  moveServo(SERVO_RIGHT, rightCmd);

#ifdef DEBUG
  Serial.print("X: "); Serial.print(x);
  Serial.print(" Y: "); Serial.print(y);
  Serial.print(" | L: "); Serial.print(leftCmd);
  Serial.print(" R: "); Serial.println(rightCmd);
#endif

  delay(20);
}