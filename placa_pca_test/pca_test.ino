#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

#define SERVO_MIN 150
#define SERVO_MAX 550
#define FREQ 50

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

int porta = 0;
int val;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  pwm.begin();
  pwm.setPWMFreq(FREQ);  //Hz
}

void loop() {

  Serial.println("Defina o motor a ser utilizado: 0-1-2-3-4-5"); // 6 motores
  if (Serial.available() > 0) {
    porta = Serial.read();  // captura porta do motor
    Serial.println("Porta definida: " + porta);
  }

  Serial.println("Digite o valor do angulo: 0-180");
  if (Serial.available() > 0) {
    val = Serial.read();

    if (val >= 0 && val <= 180) {
      pwm.setPWM(porta, 0, angleToPulse(val));
    } 
    else {
      Serial.println("Valor fora dos limites permitidos!");
    }
  }

  delay(1000);

}  // fim do loop

int angleToPulse(int ang) {  //converte o angulo em um pulso definido no limite MIN e MAX
  int pulse = map(ang, 0, 150, SERVO_MIN, SERVO_MAX);
  return pulse;
}