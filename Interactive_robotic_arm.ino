#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

#define JoyStick_X_1 A0 // Analog Pin  X_1
#define JoyStick_X_2 A2 // Analog Pin  X_2
#define JoyStick_Y_1 A1 // Analog Pin  Y_1
#define JoyStick_Y_2 A3 // Analog Pin  Y_2

int pos0 = 102;
int pos180 = 512;
int pos = 0;

Adafruit_PWMServoDriver servos = Adafruit_PWMServoDriver(0x40);

// Valores alvo dos servos
int targetServo0 = pos0;
int targetServo1 = pos0;
int targetServo2 = pos0;
int targetServo3 = pos0;

// Constantes para suavização
const float smoothingFactor = 0.3; // Fator de suavização (ajustável)
int smoothedX1 = 0;
int smoothedX2 = 0;
int smoothedY1 = 0;

void setup() {
    pinMode(JoyStick_X_1, INPUT);
    pinMode(JoyStick_Y_1, INPUT);
    pinMode(JoyStick_X_2, INPUT);
    pinMode(JoyStick_Y_2, INPUT);
    
    servos.begin();
    servos.setOscillatorFrequency(27000000);
    servos.setPWMFreq(50);
    Serial.begin(9600);
    inicialServoPosition();
}

void inicialServoPosition () {
    servos.setPWM(0, 0, pos0);
    servos.setPWM(1, 0, pos0);
    servos.setPWM(2, 0, pos0);
    servos.setPWM(3, 0, pos0);
}

void updateServos() {
    // Suavização dos valores dos joysticks
    int rawX1 = analogRead(JoyStick_X_1);
    int rawX2 = analogRead(JoyStick_X_2);
    int rawY1 = analogRead(JoyStick_Y_1);
    
    smoothedX1 = (int)(smoothedX1 * (1.0 - smoothingFactor) + rawX1 * smoothingFactor);
    smoothedX2 = (int)(smoothedX2 * (1.0 - smoothingFactor) + rawX2 * smoothingFactor);
    smoothedY1 = (int)(smoothedY1 * (1.0 - smoothingFactor) + rawY1 * smoothingFactor);
    
    // Mapeamento dos valores suavizados para ângulos dos servos
    int angleServo0 = map(smoothedY1, 0, 1023, 0, 180);
    int angleServo1 = map(smoothedX2, 0, 1023, 0, 180);
    int angleServo2 = map(smoothedX1, 0, 1023, 0, 180);
    int angleServo3 = map(smoothedX1, 0, 1023, 0, 180);
    
    // Definir valores alvo dos servos com suavização
    targetServo0 = (int)(targetServo0 * (1.0 - smoothingFactor) + angleServo0 * smoothingFactor);
    targetServo1 = (int)(targetServo1 * (1.0 - smoothingFactor) + angleServo1 * smoothingFactor);
    targetServo2 = (int)(targetServo2 * (1.0 - smoothingFactor) + angleServo2 * smoothingFactor);
    targetServo3 = (int)(targetServo3 * (1.0 - smoothingFactor) + angleServo3 * smoothingFactor);
    
    // Enviar valores alvo para os servos
    servos.setPWM(0, 0, map(targetServo0, 0, 180, pos0, pos180));
    servos.setPWM(1, 0, map(targetServo1, 0, 180, pos0, pos180));
    servos.setPWM(2, 0, map(targetServo2, 0, 180, pos0, pos180));
    servos.setPWM(3, 0, map(targetServo3, 0, 180, pos0, pos180));
}

void loop() {
    updateServos();
    delay(100); // Adicione um pequeno atraso para evitar atualizações muito frequentes
}
