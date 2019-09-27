#include "Servo.h"

class MyServo {
  private:
    Servo theServo;
    int m_angle = 0;
    int m_pin = 999;

  public:
    MyServo(int pin);

    // assume that we won't be pushing the servo beyond its limits
    void moveCW(int desiredAngle, int speedStep);
    void moveCCW(int desiredAngle, int speedStep);

    // getters
    int getAngle();
};

// ------------------------------------

MyServo servo1(9);

void setup() {}

void loop() {
  // something is a little f'ed up with this code but it can move the servos
  servo1.moveCW(90, 5);
  servo1.moveCCW(90, 5);
}

// ------------------------------------

MyServo::MyServo(int pin) {
  m_pin = pin;
  theServo.attach(m_pin);
}

void MyServo::moveCW(int desiredAngle, int speedStep) {
  for (int newAngle = m_angle; newAngle < desiredAngle; newAngle += speedStep) {                               
      theServo.write(newAngle);             
  }
  m_angle += desiredAngle;
}

void MyServo::moveCCW(int desiredAngle, int speedStep) {
  for (int newAngle = m_angle; newAngle >= desiredAngle; newAngle -= speedStep) {                               
      theServo.write(newAngle);         
  }
  m_angle -= desiredAngle;
}

int MyServo::getAngle() {
  return m_angle;
}
