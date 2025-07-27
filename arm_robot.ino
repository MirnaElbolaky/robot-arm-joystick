#include <Stepper.h>
#include <Servo.h>

// === Servo (Gripper) Setup ===
const int pin_servo = 13;           // Servo pin for gripper
Servo servo;
int pulso_min = 600;                // Minimum pulse width
int pulso_max = 2500;               // Maximum pulse width
int angulo = 90;                    // Initial gripper angle
int gripperState = 0;               // 0 = closed, 1 = open

// === Joystick Setup ===
const int joy_pin_A1 = A0;          // Analog pin for joystick movement
const int joy_button_A1 = 12;       // Digital pin for joystick button
const int X_pin = 1;                // Analog X-axis pin
const int Y_pin = 3;                // Analog Y-axis pin

// === Stepper Motors Setup ===
const int stepsPerRevolution = 2048;
const int rolePerMinute = 15;

// Base motor
Stepper myStepperB(stepsPerRevolution, 2, 4, 3, 5);
// Arm motor
Stepper myStepperR(stepsPerRevolution, 7, 9, 8, 10);

void setup() {
  myStepperB.setSpeed(rolePerMinute);
  myStepperR.setSpeed(rolePerMinute);

  pinMode(pin_servo, OUTPUT);
  servo.attach(pin_servo, pulso_min, pulso_max);

  pinMode(joy_button_A1, INPUT);
}

void loop() {
  // === Gripper Control with Joystick Y ===
  int val_A1 = analogRead(joy_pin_A1);
  angulo = map(val_A1, 0, 1023, 0, 180);
  servo.write(angulo);

  // === Toggle gripper state using button ===
  if (digitalRead(joy_button_A1) == HIGH) {
    if (!gripperState) {
      gripperState = 1 - gripperState;
      angulo = (gripperState == 1) ? 0 : 180;
      servo.write(angulo);
    }
  } else {
    gripperState = 0;
  }

  delay(15);  // Debounce / smoothing delay

  // === X-axis Movement (Base Motor) ===
  int a = analogRead(X_pin);
  if (a > 400 && a < 520) {
    for (int i = 2; i < 6; i++) digitalWrite(i, LOW);
  } else if (a < 400) {
    myStepperB.step(-30);  // Move left
  } else if (a > 530) {
    myStepperB.step(30);   // Move right
  }

  // === Y-axis Movement (Arm Motor) ===
  int b = analogRead(Y_pin);
  if (b > 400 && b < 520) {
    for (int i = 2; i < 6; i++) digitalWrite(i, LOW);
  } else if (b < 400) {
    myStepperR.step(-30);  // Move down
  } else if (b > 530) {
    myStepperR.step(30);   // Move up
  }
}
