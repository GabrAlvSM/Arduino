#define DEBUG

#include <Wire.h>
#include <SoftwareSerial.h>

/* ========================= */
/* Bluetooth HC-05           */
/* RX, TX                    */
/* ========================= */

SoftwareSerial BT(10, 11);

/* ========================= */

const byte I2C_ADDR = 128;

/* ========================= */
/* Servo 270°                */
/* ========================= */

const int SERVO_MIN = 0;
const int SERVO_MAX = 270;

const int OUT_MIN = 1;
const int OUT_MAX = 179;

/* ========================= */
/* Motores contínuos         */
/* ========================= */

const byte MOTOR_LEFT  = 4;
const byte MOTOR_RIGHT = 5;

/* ========================= */

void setup() {

  Serial.begin(115200);
  BT.begin(9600);

  Wire.begin();

  Serial.println("LU9685 Bluetooth Controller");
}

/* ========================= */

bool isContinuousServo(byte servo) {
  return (servo >= 4 && servo <= 6);
}

/* ========================= */

byte convertPosition(int angulo) {

  angulo = constrain(
    angulo,
    SERVO_MIN,
    SERVO_MAX
  );

  return map(
    angulo,
    SERVO_MIN,
    SERVO_MAX,
    OUT_MIN,
    OUT_MAX
  );
}

/* ========================= */

byte convertContinuous(int vel) {

  vel = constrain(vel, -100, 100);

  return map(
    vel,
    -100,
    100,
    0,
    180
  );
}

/* ========================= */
/* PROTOCOLO LU9685          */
/* ========================= */

void moveServo(byte servo, byte pos) {

  Wire.beginTransmission(I2C_ADDR);

  Wire.write(servo);
  Wire.write(pos);

  Wire.endTransmission();

#ifdef DEBUG

  Serial.print("I2C -> Servo ");
  Serial.print(servo);

  Serial.print(" Pos ");
  Serial.println(pos);

#endif
}

/* ========================= */

void setContinuous(byte servo, int vel) {

  byte pos = convertContinuous(vel);

  moveServo(servo, pos);
}

/* ========================= */

void stopMotors() {

  setContinuous(MOTOR_LEFT, 0);
  setContinuous(MOTOR_RIGHT, 0);
}

/* ========================= */

void forward() {

  setContinuous(MOTOR_LEFT, 100);
  setContinuous(MOTOR_RIGHT, 100);
}

/* ========================= */

void backward() {

  setContinuous(MOTOR_LEFT, -100);
  setContinuous(MOTOR_RIGHT, -100);
}

/* ========================= */

void left() {

  setContinuous(MOTOR_LEFT, -100);
  setContinuous(MOTOR_RIGHT, 100);
}

/* ========================= */

void right() {

  setContinuous(MOTOR_LEFT, 100);
  setContinuous(MOTOR_RIGHT, -100);
}

/* ========================= */

void processSerial(Stream &port) {

  if (!port.available()) return;

  /* ========================= */
  /* COMANDOS DIRECIONAIS      */
  /* ========================= */

  char c = port.peek();

  if (
    c == 'F' ||
    c == 'B' ||
    c == 'L' ||
    c == 'R' ||
    c == 'S'
  ) {

    c = port.read();

    switch (c) {

      case 'F':
        forward();
        break;

      case 'B':
        backward();
        break;

      case 'L':
        left();
        break;

      case 'R':
        right();
        break;

      case 'S':
        stopMotors();
        break;
    }

#ifdef DEBUG
    Serial.print("CMD: ");
    Serial.println(c);
#endif

    return;
  }

  /* ========================= */
  /* MODO ORIGINAL             */
  /* ========================= */

  byte servo = port.parseInt();
  int valor  = port.parseInt();

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

  port.println("ok");
}

/* ========================= */

void loop() {

  processSerial(BT);

  processSerial(Serial);
}
