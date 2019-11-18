/*  This is a Basic Example of How to control a Stewart Platform 
 *  using the StewartPlatform Library.
 *  
 *  Setting up the translation and rotation you will have the desired movements.
 *  
 *  Make sure you have all your servos conected, and
 *  remeber to configure yours servos Pins in Setup and yours
 *  platform values in Config.h
 */

#include "Config.h"
#include "StewartPlatform.h"
#include <Servo.h>
#include <SoftwareSerial.h>

StewartPlatform sp;
Servo servos[6];
float servosPosition[6];

point_t translation = {0,0,0}, rotation = {0.1,0,0};

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
  
  count = count + 1;
  if(count == 100){
    translation.z = 2;
  }
  
  if(count == 500){
    translation.z = -2;
    count = 0;
  }

  sp.getServoPosition(translation, rotation, servosPosition);
  
  for(int i = 0; i < 6; i++){   
    servos[i].writeMicroseconds(servosPosition[i]);
    
    if (count == 500) {
      Serial.print("position of servo "); Serial.print(i); Serial.print(": ");
      Serial.println(servosPosition[i]); 
    }
  }

  delay(5);
}

int count2 = 0;
float rawX = 0; float rawY = 0; float rawZ = 0;
void readIMU(){
  // Read IMU from A0, A1, A2
  count2 = count2 + 1;
  
  if (count2 % 10 == 0){
    //tempbool = false;
     rawX = analogRead(A0);
     rawY = analogRead(A1);
     rawZ = analogRead(A2);

     
     int scale = 3;
     float scaledX = mapf(rawX, 0, 1023, -scale, scale);
     float scaledY = mapf(rawY, 0, 1023, -scale, scale);
     float scaledZ = mapf(rawZ, 0, 1023, -scale, scale);

     // Print out raw X,Y,Z accelerometer readings
//    Serial.print("Y: "); Serial.println(rawY);
  
    // Print out scaled X,Y,Z accelerometer readings
    //Serial.print("Y: "); Serial.print(scaledY); Serial.println(" g");
  
    float accelerationX = scaledX;
    float accelerationY = scaledY;
    float accelerationZ = scaledZ;
    
    float pitch = 180 * atan (accelerationX/sqrt(accelerationY*accelerationY + accelerationZ*accelerationZ))/PI;
    float roll = 180 * atan (accelerationY/sqrt(accelerationX*accelerationX + accelerationZ*accelerationZ))/PI;
    float yaw = 180 * atan (accelerationZ/sqrt(accelerationX*accelerationX + accelerationZ*accelerationZ))/PI;
  
    float rotX = pitch/360;
    
    if (abs(float((rotX-rotation.x)/rotation.x)) < 0.05){
//      rotX = rotation.x;
  }
  
  if(rotX < 0.1 && rotX > -0.1){
//    rotation.x = rotX/2;
//    Serial.println(rotX/2);
  }
  
  }
}

// Same functionality as Arduino's standard map function, except using floats
float mapf(float x, float in_min, float in_max, float out_min, float out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
