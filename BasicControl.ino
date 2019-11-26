#include "Config.h"
#include "StewartPlatform.h"
#include <Servo.h>
#include <SoftwareSerial.h>

StewartPlatform sp;
Servo servos[6];
float servosPosition[6];

// x y z
point_t translation = {0,0,0}, rotation = {0,0,0};

unsigned long startMillis = millis();

// raw values
int x_rest=333, x_min=270, x_max=400; // 270 400, 310 380
int y_rest=335, y_min=270, y_max=400;
int z_rest=420, z_min=400, z_max=420;
int range = 3;

// switch pins to turn off/on
const int x_switch = 2, y_switch = 4, z_switch = 7; 

float range_rotation_pitch = 0.3, range_rotation_roll = 0.3, range_rotation_yaw = 0.4;

void setup() {
  Serial.begin(115200);  
  
  servos[0].attach(3, MIN_SERVO_PULSE, MAX_SERVO_PULSE);
  servos[1].attach(5, MIN_SERVO_PULSE, MAX_SERVO_PULSE);
  servos[2].attach(6, MIN_SERVO_PULSE, MAX_SERVO_PULSE);
  servos[3].attach(9, MIN_SERVO_PULSE, MAX_SERVO_PULSE);
  servos[4].attach(10, MIN_SERVO_PULSE, MAX_SERVO_PULSE);
  servos[5].attach(11, MIN_SERVO_PULSE, MAX_SERVO_PULSE);
}

void loop() { 
  readIMU(); 
  sp.getServoPosition(translation, rotation, servosPosition);

  // moves the servos
  for(int i = 0; i < 6; i++){   
    servos[i].writeMicroseconds(servosPosition[i]);
  }
  delay(1500);
}

// Read the values from the IMU and change that to values to pass
// into the functions to move servos
void readIMU() {
  
  int rawX = analogRead(A0);
  int rawY = analogRead(A1);
  int rawZ = analogRead(A2);

  delay(100);
  if (false) {
    Serial.print("x "); Serial.println(rawX); 
    Serial.print("y "); Serial.println(rawY); 
    Serial.print("z "); Serial.println(rawZ);
    Serial.println();   
  }

  // change values to between 
  // -range <= value <= range
  float mappedX = mapf(rawX, x_min, x_max, -range, range);
  float mappedY = mapf(rawY, y_min, y_max, -range, range);
  float mappedZ = mapf(rawZ, z_min, z_max, -range, range);

  float deltaZ = 0.5*(mappedZ-5.08)*9.81*pow(0.1,2);

  float pitch = 180 * atan2(mappedX, sqrt(mappedY*mappedY + mappedZ*mappedZ))/PI;
  float roll = 180 * atan2 (mappedY, sqrt(mappedX*mappedX + mappedZ*mappedZ))/PI;
  float yaw = 180 * atan2 (mappedZ, sqrt(mappedX*mappedX + mappedZ*mappedZ))/PI;

  float mappedPitch = mapf(pitch, -100, 100, -range_rotation_pitch, range_rotation_pitch);
  float mappedRoll = mapf(roll, -100, 100, -range_rotation_roll, range_rotation_roll);
  float mappedYaw = mapf(pitch, 0, 50, -range_rotation_yaw, range_rotation_yaw);

  if (false) {
    Serial.print("PITCH (around y) "); Serial.print(pitch); Serial.print(" "); Serial.println(mappedPitch);
    Serial.print("ROLL (around x) "); Serial.print(roll); Serial.print(" "); Serial.println(mappedRoll);
    Serial.print("YAW (around z) "); Serial.print(yaw); Serial.print(" "); Serial.println(mappedYaw); 
    Serial.println();    
  }

  // set the rotation values
  int x_state = digitalRead(x_switch);
  int y_state = digitalRead(y_switch);
  int z_state = digitalRead(z_switch);

  if (x_state == 0) {
    Serial.print("Pitch(x) is on, pitch is: "); Serial.println(pitch);
    rotation.x = mappedPitch * 2;
  }
  
  if (y_state == 0) {
    Serial.print("Roll(x) is on, roll is: "); Serial.println(mappedRoll);
    rotation.y = -mappedRoll * 2;
  }

  if (z_state == 0) {
    Serial.println("Z translation is on");
    // do something
  }
}

// Same functionality as Arduino's standard map function, except using floats
float mapf(float x, float in_min, float in_max, float out_min, float out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
