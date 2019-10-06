#include "Servo.h"

void moveAll(int s1, int s2, int s3, int s4, int s5, int s6);

// joystick
int jsx = 0;
int jsy = 0;  
int bound1 = 300;
int bound2 = 800;
int pin = 0;

// servos
Servo servo1;
Servo servo2;
Servo servo3;
Servo servo4;
Servo servo5;
Servo servo6;

// positions
// 0 position is when it can't turn CW anymore
// tilt is for 5 degrees
// flat, forward, backward, left, right 
int pos1[] = {102, 88, 116, 93, 113};
int pos2[] = {102, 92, 117, 113, 93};
int pos3[] = {109, 104, 109, 127, 93};
int pos4[] = {109, 128, 97, 116, 104};
int pos5[] = {109, 123, 96, 104, 116};
int pos6[] = {109, 107, 118, 93, 127};

// to tilt platform by 1 degree in forward/back
// pos is forward
double forBack[] = {2.796, 2.519, 0.405, -3.122, -2.723, 1.082};

// to tilt platform by 1 degree in left/right
// positive is 
double leftRight[] = {2.008, -2.011, -3.399, -1.155, 1.156, 3.405};

// ------------------------------------

void setup() {
  Serial.begin(9600);
  
  servo1.attach(3);
  servo2.attach(5);
  servo3.attach(6);
  servo4.attach(9);
  servo5.attach(10);
  servo6.attach(11);
  
  servo1.write(pos1[0]);
  servo2.write(pos2[0]);
  servo3.write(pos3[0]);
  servo4.write(pos4[0]);
  servo5.write(pos5[0]);
  servo6.write(pos6[0]);
 }

void loop() {
  // read the pins on the joystick
  // home position for joystick is at 511 (1023/2)
  jsx = analogRead(A0);
  delay(100);
  jsy = analogRead(A1);
  delay(100);

  // we are just considering moving the joystick in 4 directions
  // so don't move on the diagonals
  bool moved = false;
  double count=0;
  double x = 0, y = 0; // 0: no change | positive: right/up | negative: left/down

  // LEFT
  if (!moved) {
    while (jsx < bound1) {
      x = -0.5;
      servo1.write(pos1[0] + leftRight[0]*x*count);
      servo2.write(pos2[0] + leftRight[1]*x*count);
      servo3.write(pos3[0] + leftRight[2]*x*count);
      servo4.write(pos4[0] + leftRight[3]*x*count);
      servo5.write(pos5[0] + leftRight[4]*x*count);
      servo6.write(pos6[0] + leftRight[5]*x*count);
  
      delay(100);
      count++;
      jsx = analogRead(A0);
      moved = true;
    }
  }

  // RIGHT
  if (!moved) {
    while (jsx > bound2) {
      x = 0.5;
      servo1.write(pos1[0] + leftRight[0]*x*count);
      servo2.write(pos2[0] + leftRight[1]*x*count);
      servo3.write(pos3[0] + leftRight[2]*x*count);
      servo4.write(pos4[0] + leftRight[3]*x*count);
      servo5.write(pos5[0] + leftRight[4]*x*count);
      servo6.write(pos6[0] + leftRight[5]*x*count);
  
      delay(100);
      count++;
      jsx = analogRead(A0);
      jsy = analogRead(A1);
      moved = true;
    } 
  }
  
  // DOWN
  if (!moved) {
    while (jsy < bound1) {
      y = -0.5;
      servo1.write(pos1[0] + forBack[0]*y*count);
      servo2.write(pos2[0] + forBack[1]*y*count);
      servo3.write(pos3[0] + forBack[2]*y*count);
      servo4.write(pos4[0] + forBack[3]*y*count);
      servo5.write(pos5[0] + forBack[4]*y*count);
      servo6.write(pos6[0] + forBack[5]*y*count);
  
      delay(100);
      count++;
      jsy = analogRead(A1);
      moved = true;
    } 
  }

  // UP
  bool diagonal = false; 
  if (!moved) {
    while (jsy > bound2) {
      Serial.println("Moving up");
      if (jsx < bound1 || jsx > bound2) {
        Serial.println("Diagonal cut to x");
        diagonal = true;
        break;
      }
      
      y = 0.5;
      servo1.write(pos1[0] + forBack[0]*y*count);
      servo2.write(pos2[0] + forBack[1]*y*count);
      servo3.write(pos3[0] + forBack[2]*y*count);
      servo4.write(pos4[0] + forBack[3]*y*count);
      servo5.write(pos5[0] + forBack[4]*y*count);
      servo6.write(pos6[0] + forBack[5]*y*count);
      
      delay(100);
      count++;
      jsy = analogRead(A1);
      jsx = analogRead(A0);
      moved = true;
    }
    
    if (diagonal) {
      
    }
    
  }

  // -------- for jerking -----------
  
  double f = 3;
  bool fed = false;
  pin = digitalRead(2);
  delay(100);
  
  if (!moved && pin) {
    Serial.println("Jerk the platform");
    servo1.write(pos1[0] + leftRight[0]*f);
    servo2.write(pos2[0] + leftRight[1]*f);
    servo3.write(pos3[0] + leftRight[2]*f);
    servo4.write(pos4[0] + leftRight[3]*f);
    servo5.write(pos5[0] + leftRight[4]*f);
    servo6.write(pos6[0] + leftRight[5]*f);
    fed = true;
  }

  delay(500);

  if (fed) {
    Serial.println("Move the platform back from jerking");
    servo1.write(pos1[0]);
    servo2.write(pos2[0]);
    servo3.write(pos3[0]);
    servo4.write(pos4[0]);
    servo5.write(pos5[0]);
    servo6.write(pos6[0]);
  }

  // -------- end jerking -----------
  
  // move it back to original plaform position
  if (moved) {
    if (x != 0) {
      while (count >= 0) {
        servo1.write(pos1[0] + leftRight[0]*x*count);
        servo2.write(pos2[0] + leftRight[1]*x*count);
        servo3.write(pos3[0] + leftRight[2]*x*count);
        servo4.write(pos4[0] + leftRight[3]*x*count);
        servo5.write(pos5[0] + leftRight[4]*x*count);
        servo6.write(pos6[0] + leftRight[5]*x*count);
        delay(100);
        count--;
      }
    }

    else {
      while (count >= 0) {
        servo1.write(pos1[0] + forBack[0]*y*count);
        servo2.write(pos2[0] + forBack[1]*y*count);
        servo3.write(pos3[0] + forBack[2]*y*count);
        servo4.write(pos4[0] + forBack[3]*y*count);
        servo5.write(pos5[0] + forBack[4]*y*count);
        servo6.write(pos6[0] + forBack[5]*y*count);
        delay(100);
        count--;
      }
    }

    delay(500);

    // set them all back to initial position
    servo1.write(pos1[0]);
    servo2.write(pos2[0]);
    servo3.write(pos3[0]);
    servo4.write(pos4[0]);
    servo5.write(pos5[0]);
    servo6.write(pos6[0]);
    delay(500);
  }
}
