#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

#define JoyStick_X_1 A0 // Analog Pin  X_1
#define JoyStick_X_2 A2 // Analog Pin  X_2
#define JoyStick_Y_1 A1 // Analog Pin  Y_1
#define JoyStick_Y_2 A3 // Analog Pin  Y_2

int pos0 = 102;
int pos180 = 512;

Adafruit_PWMServoDriver servos = Adafruit_PWMServoDriver(0x40);

void setup() {
    pinMode(JoyStick_X_1, INPUT);
    pinMode(JoyStick_Y_1, INPUT);
    pinMode(JoyStick_X_2, INPUT);
    pinMode(JoyStick_Y_2, INPUT);
    servos.begin();
    servos.setPWMFreq(50);
    Serial.begin(9600);
}

void setServo (uint8_t n_servo, int valorPotenciometro){
  int angulo = map(valorPotenciometro, -512, 512, 0, 180);
  int duty = map(angulo, 0, 180, pos0, pos180);
  servos.setPWM(n_servo, 0, duty);
}

void setServo2 (uint8_t n_servo, int valorPotenciometro){
  int angulo = map(valorPotenciometro, 512, -512, 0, 180);
  int duty = map(angulo, 0, 180, pos0, pos180);
  servos.setPWM(n_servo, 0, duty);
}


void loop() {
  int x_1;
  x_1 = analogRead(JoyStick_X_1);
  setServo(0, x_1);
  setServo2(1, x_1);
  delay(100);

}
