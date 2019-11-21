/*  This is a Basic Example of How to control a Stewart Platform 
 *  using the StewartPlatform Library.
 *  
 *  Setting up the translation and rotation you will have the desired movements.
 *  
 *  Make sure you have all your servos conected, and
 *  remeber to configure yours servos Pins in Setup and yours
 *  platform values in Config.h
 */

 /*
  * TRANSLATION
  * z range we should use (0 +25)
  * x range we should use (-20 +20)
  * y range we should use (-20 +20)
  * 
  * ROTATION
  * z range we should use ()
  * x range we should use (-0.1 +0.1)
  * y range we should use (-0.1 +0.1)
  * 
  * IMU 
  * pitch range is (-45,-31) rest value = -38
  * roll range is (-49,-35) rest value = -42
  * yaw range is (-30,-22) rest value = -26
  * 
  */

#include "Config.h"
#include "StewartPlatform.h"
#include <Servo.h>
#include <SoftwareSerial.h>

StewartPlatform sp;
Servo servos[6];
float servosPosition[6];

point_t translation = {0,0,0}, rotation = {0,0,0};

int pitchSwitch = true;
int rollSwitch = true;
int yawSwitch = true;

void setup() {
  Serial.begin(115200);  

  /* MIN_SERVO_PULSE AND MAX_SERVO_PULSE are defined in CONFIG.H */
  /* Attach yours Servos Pins here */
  servos[0].attach(3, MIN_SERVO_PULSE, MAX_SERVO_PULSE);
  servos[1].attach(5, MIN_SERVO_PULSE, MAX_SERVO_PULSE);
  servos[2].attach(6, MIN_SERVO_PULSE, MAX_SERVO_PULSE);
  servos[3].attach(9, MIN_SERVO_PULSE, MAX_SERVO_PULSE);
  servos[4].attach(10, MIN_SERVO_PULSE, MAX_SERVO_PULSE);
  servos[5].attach(11, MIN_SERVO_PULSE, MAX_SERVO_PULSE);
}

float count = 0;
void loop() {  
  readIMU();

  sp.getServoPosition(translation, rotation, servosPosition);
  
  for(int i = 0; i < 6; i++){   
    servos[i].writeMicroseconds(servosPosition[i]);
  }
}

int count2 = 0;
float rawX = 0; float rawY = 0; float rawZ = 0;
void readIMU() {
  count2 ++;
  if (count2 % 50 == 0) { 
    rawX = analogRead(A0);
    rawY = analogRead(A1);
    rawZ = analogRead(A2);
    
    int scale = 3;
    float scaledX = mapf(rawX, 0, 1023, -scale, scale);
    float scaledY = mapf(rawY, 0, 1023, -scale, scale);
    float scaledZ = mapf(rawZ, 0, 1023, -scale, scale);
  
    float accelerationX = scaledX;
    float accelerationY = scaledY;
    float accelerationZ = scaledZ;

    // not sure if we followed this but useful: https://www.accuware.com/support/dragonfly-heading-pitch-yaw-roll/
    // pitch is if camera is pointing up/down
    // roll is + if camera is moving toward right, - if moving toward left
    // yaw is + if camera is pointing to East, - if pointing to west 

    float pitch = 180 * atan2(accelerationX, sqrt(accelerationY*accelerationY + accelerationZ*accelerationY))/PI;
    float roll = 180 * atan2 (accelerationY, sqrt(accelerationX*accelerationX + accelerationZ*accelerationZ))/PI;
    float yaw = 180 * atan2 (accelerationZ, sqrt(accelerationX*accelerationX + accelerationZ*accelerationZ))/PI;

    float pitchMap = mapf(pitch, -50,-25, -0.05, 0.05); // around y
    float rollMap = mapf(roll, -50,-25, -0.05, 0.05); // around x
    float yawMap = mapf(yaw, -30,-23, -0.05, 0.05); // around z

    if(pitchSwitch){
      rotation.y = pitchMap;
    }
    if(rollSwitch){
      rotation.x = rollMap;
    }
    if(yawSwitch){
      rotation.z = yawMap;
    }

    Serial.print("PITCH (around y) "); Serial.print(pitch); Serial.print(" "); Serial.println(pitchMap);
    Serial.print("ROLL (around x) "); Serial.print(roll); Serial.print(" "); Serial.println(rollMap);
    Serial.print("YAW (around z) "); Serial.print(yaw); Serial.print(" "); Serial.println(yawMap); 
    
    Serial.println();
  }
}

// Same functionality as Arduino's standard map function, except using floats
float mapf(float x, float in_min, float in_max, float out_min, float out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
