#include <Servo.h>

int ledVerm = 10;
int ledVerd = 11;
int ang = 0;

Servo motor;

void setup() {
  //put your setup code here, to run once:

pinMode(ledVerm,OUTPUT);

pinMode(ledVerd, OUTPUT);

motor.attach(8);

// Serial.begin(9600);
}

void loop() {
  //put your setup code here, to run in loop:

digitalWrite(ledVerm, HIGH); // porta 10 LED VERMELHO

motor.write(90); //MOTOR GIRA A 90° (ABERTO)

digitalWrite(ledVerm, LOW); // porta 10 LED VERMELHO

//ABERTO

digitalWrite(ledVerd, HIGH); //porta 11 LED VERDE

delay(7000)

digitalWrite(ledVerm, HIGH); // porta 10 LED VERMELHO
delay(500)
digitalWrite(ledVerm, LOW); // porta 10 LED VERMELHO
delay(500)
digitalWrite(ledVerm, HIGH); // porta 10 LED VERMELHO

delay(50) //MOTOR ESPERA 500ms (0,5 s) (ABERTO)
digitalWrite(ledVerd, LOW); //porta 11 LED VERDE

delay(500)
motor.write(0) //MOTOR GIRA A 0° (FECHADO)




// //porta 11 LED VERDE
// digitalWrite(ledVerd, HIGH);
// delay(400);
// digitalWrite(ledVerd, LOW);
// delay(200);

// for(ang = 0; ang <= 90; ang+=3)
//   {
//     motor.write(ang);
//     delay(8);
//   }

//   delay(1000);

//   for(ang >= 90; ang > 0; ang+-3)
//   {
//     motor.write(ang);
//     delay(8);
//   }

//   delay(1000);

//       {
//     motor.write(ang);
//     delay(8);
//   }

}

