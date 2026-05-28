/*
====================================================
 BRACO ROBOTICO BLUETOOTH - CONTINUO
====================================================
*/

#define DEBUG

#include <Wire.h>
#include <SoftwareSerial.h>

SoftwareSerial BT(10, 11);

const byte I2C_ADDR = 128;

/* =========================================
   SERVOS
========================================= */

const int SERVO_MIN[7] = {
  0,
  0,
  0,
  0,
  0,
  -100,
  0
};

const int SERVO_MAX[7] = {
  0,
  270,
  260,
  260,
  180,
  100,
  85
};

/* =========================================
   POSICOES
========================================= */

int currentPos[7] = {
  0,
  135,
  135,
  90,
  90,
  0,
  0
};

/* =========================================
   CONFIG
========================================= */

const int STEP_BIG = 6;
const int STEP_GRIP = 6;

const unsigned long MOVE_INTERVAL = 30;

/* =========================================
   CONTROLE CONTINUO
========================================= */

bool moving = false;

byte activeServo = 0;
int activeStep = 0;

unsigned long lastMove = 0;

/* =========================================
   SETUP
========================================= */

void setup() {

  Serial.begin(115200);

  BT.begin(9600);

  Wire.begin();

#ifdef DEBUG
  Serial.println("---Braco iniciado---");
  Serial.println("Movendo para posição inicial...");
#endif

  // POSIÇÃO INICIAL
  currentPos[1] = 90;
  currentPos[2] = 0;
  currentPos[3] = 0;
  currentPos[4] = 0;
  currentPos[6] = 0;

  sendAllServos();

  #ifdef DEBUG
  Serial.println("Braco pronto para uso");
  #endif
}

/* =========================================
   LOOP
========================================= */

void loop() {

  readBluetooth();

  processContinuousMovement();
}

/* =========================================
   LEITURA BLUETOOTH
========================================= */

void readBluetooth() {

  if (!BT.available()) return;

  String cmd = BT.readStringUntil('\n');

  cmd.trim();

#ifdef DEBUG
  Serial.print("CMD: ");
  Serial.println(cmd);
#endif

  processCommand(cmd);
}

/* =========================================
   PROCESSAMENTO
========================================= */

void processCommand(String cmd) {

  // BASE
  if (cmd == "B+") startMove(1, +STEP_BIG);
  else if (cmd == "B-") startMove(1, -STEP_BIG);

  // BASE VERTICAL
  else if (cmd == "U+") startMove(2, +STEP_BIG);
  else if (cmd == "U-") startMove(2, -STEP_BIG);

  // MEIO
  else if (cmd == "M+") startMove(3, +STEP_BIG);
  else if (cmd == "M-") startMove(3, -STEP_BIG);

  // TOPO
  else if (cmd == "T+") startMove(4, +STEP_BIG);
  else if (cmd == "T-") startMove(4, -STEP_BIG);

  // GARRA
  else if (cmd == "G+") startMove(6, +STEP_GRIP);
  else if (cmd == "G-") startMove(6, -STEP_GRIP);

  // ROTACAO CONTINUA
  else if (cmd == "R+") sendContinuousServo(5, +30);
  else if (cmd == "R-") sendContinuousServo(5, -30);

  // STOP GERAL
  else if (cmd == "STOP") {

    moving = false;

    stopContinuousServo();

#ifdef DEBUG
    Serial.println("STOP");
#endif
  }
}

/* =========================================
   INICIAR MOVIMENTO
========================================= */

void startMove(byte servo, int step) {

  activeServo = servo;

  activeStep = step;

  moving = true;
}

/* =========================================
   MOVIMENTO CONTINUO
========================================= */

void processContinuousMovement() {

  if (!moving) return;

  if (millis() - lastMove < MOVE_INTERVAL) return;

  lastMove = millis();

  incrementServo(activeServo, activeStep);
}

/* =========================================
   INCREMENTO
========================================= */

void incrementServo(byte servo, int step) {

  currentPos[servo] += step;

  currentPos[servo] = constrain(
    currentPos[servo],
    SERVO_MIN[servo],
    SERVO_MAX[servo]
  );

  sendServo(servo, currentPos[servo]);

#ifdef DEBUG
  Serial.print("Servo ");
  Serial.print(servo);

  Serial.print(" -> ");

  Serial.println(currentPos[servo]);
#endif
}

/* =========================================
   PARAR CONTINUO
========================================= */

void stopContinuousServo() {

  sendContinuousServo(5, 0);
}

/* =========================================
   ENVIAR TODOS
========================================= */

void sendAllServos() {

  for (byte i = 1; i <= 6; i++) {

    if (i == 5) continue;

    sendServo(i, currentPos[i]);
  }
}

/* =========================================
   CONVERSAO 270
========================================= */

byte convert270(int angulo) {

  angulo = constrain(angulo, 0, 270);

  return map(angulo, 0, 270, 1, 179);
}

/* =========================================
   CONVERSAO 180
========================================= */

byte convert180(int angulo) {

  angulo = constrain(angulo, 0, 180);

  return map(angulo, 0, 180, 1, 179);
}

/* =========================================
   CONTINUO
========================================= */

byte convertContinuous(int vel) {

  vel = constrain(vel, -100, 100);

  return map(vel, -100, 100, 0, 180);
}

/* =========================================
   ENVIO NORMAL
========================================= */

void sendServo(byte servo, int angulo) {

  byte pos;

  if (servo <= 3) {
    pos = convert270(angulo);
  }
  else {
    pos = convert180(angulo);
  }

  Wire.beginTransmission(I2C_ADDR);

  Wire.write(servo);
  Wire.write(pos);

  Wire.endTransmission();
}

/* =========================================
   ENVIO CONTINUO
========================================= */

void sendContinuousServo(byte servo, int vel) {

  byte pos = convertContinuous(vel);

  Wire.beginTransmission(I2C_ADDR);

  Wire.write(servo);
  Wire.write(pos);

  Wire.endTransmission();
}
