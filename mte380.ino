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

MyServo servo1(11);
int joyPin1 = 7;
int joyPin2 = 8;
int jsval1 = 0;
int jsval2 = 0;  

// servo to sweep 180
Servo servo9;
int pos = 0;

void setup() {
  Serial.begin(9600);
//  servo9.attach(9); // servo to sweep 180
 }

void loop() {
  // something is a little f'ed up with this code but it can move the servos
  jsval1 = analogRead(joyPin1);
  delay(100);
  jsval2 = analogRead(joyPin2);

  Serial.println(jsval1);
  

//  // servo to sweep 180
//  for(pos = 0; pos < 180; pos += 1) {
//    servo9.write(pos);
//  }
//  delay(2000);
//  for(pos = 180; pos>=1; pos -= 1) {
//    servo9.write(pos);
//  } 
//  delay(1000); 
  
  servo1.moveCCW(90, 5);
  delay(5000); // need this
  servo1.moveCW(90, 5);
  delay(1000); // need this
}

// ------------------------------------

MyServo::MyServo(int pin) {
  m_pin = pin;
  theServo.attach(m_pin);
}

void MyServo::moveCCW(int desiredAngle, int speedStep) {
  for (m_angle = 0; m_angle <= desiredAngle; m_angle += speedStep) {
    theServo.write(m_angle);
  }
//  Serial.println("Move CW and now m_angle is");
//  Serial.println(m_angle);
}

void MyServo::moveCW(int desiredAngle, int speedStep) {
  for (m_angle = desiredAngle; m_angle >= 0; m_angle -= speedStep) {
    theServo.write(m_angle);
  }
//  Serial.println("Move CCW and now m_angle is");
//  Serial.println(m_angle);
}

int MyServo::getAngle() {
  return m_angle;
}
