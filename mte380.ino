#include "Servo.h"

void moveAll(int s1, int s2, int s3, int s4, int s5, int s6);

// joystick
int jsx = 0;
int jsy = 0;  
int b1 = 100;
int b2 = 400;
int b3 = 600;
int b4 = 900; 

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
int pos1 = 102;
int pos2 = 102; 
int pos3 = 109;
int pos4 = 109;
int pos5 = 109;
int pos6 = 109;

// to tilt platform by 1 degree in forward/back
// pos is forward
double forBack[] = {2.796, 2.519, 0.405, -3.122, -2.723, 1.082};

// to tilt platform by 1 degree in left/right
// positive is left
double leftRight[] = {2.008, -2.011, -3.399, -1.155, 1.156, 3.405};

// positive is forward/left
double forLeftBackRight[] = {1.283, 3.343, 2.308, -1.915, -3.119, -1.024};

// positive is forward/right
double forRightBackLeft[] = {3.34, 1.231, -1.023, -3.122, -1.912, 2.312};

// ------------------------------------

void setup() {
  Serial.begin(9600);
  
  servo1.attach(3);
  servo2.attach(5);
  servo3.attach(6);
  servo4.attach(9);
  servo5.attach(10);
  servo6.attach(11);

  moveAll(pos1, pos2, pos3, pos4, pos5, pos6);
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
  double count = 0;
  // 0: no change | positive: right/up | negative: left/down 
  double x = 0, y = 0, frbl = 0, flbr = 0;

  Serial.print("jsx: "); Serial.print(jsx); Serial.print(" jsy: "); Serial.println(jsy); 
  
  // LEFT
  if (!moved) {
    while (jsx < b1 && jsy > b2 && jsy < b3) {
      Serial.println("left");
      x = -1;
      moveAll(pos1 + leftRight[0]*x*count, pos2 + leftRight[1]*x*count, pos3 + leftRight[2]*x*count,
              pos4 + leftRight[3]*x*count, pos5 + leftRight[4]*x*count, pos6 + leftRight[5]*x*count);
      delay(100);
      count++;
      jsx = analogRead(A0);
      jsy = analogRead(A1);
      moved = true;
    }
  }

  // RIGHT
  if (!moved) {
    while (jsx > b4 && jsy > b2 && jsy < b3) {
      Serial.println("right");
      x = 1;
      moveAll(pos1 + leftRight[0]*x*count, pos2 + leftRight[1]*x*count, pos3 + leftRight[2]*x*count,
              pos4 + leftRight[3]*x*count, pos5 + leftRight[4]*x*count, pos6 + leftRight[5]*x*count);
      delay(100);
      count++;
      jsx = analogRead(A0);
      jsy = analogRead(A1);
      moved = true;
    } 
  }
  
  // FORWARD
  if (!moved) {
    while (jsy < b1 && jsx > b2 && jsx < b3) {
      Serial.println("forward");
      y = -1;
      moveAll(pos1 + forBack[0]*y*count, pos2 + forBack[1]*y*count, pos3 + forBack[2]*y*count,
              pos4 + forBack[3]*y*count, pos5 + forBack[4]*y*count, pos6 + forBack[5]*y*count);
      delay(100);
      count++;
      jsy = analogRead(A1);
      jsx = analogRead(A0);
      moved = true;
    } 
  }

  // BACKWARD
  if (!moved) {
    while (jsy > b4 && jsx > b2 && jsx < b3) {
      Serial.println("backward");
      y = 1;
      moveAll(pos1 + forBack[0]*y*count, pos2 + forBack[1]*y*count, pos3 + forBack[2]*y*count,
              pos4 + forBack[3]*y*count, pos5 + forBack[4]*y*count, pos6 + forBack[5]*y*count);
      delay(100);
      count++;
      jsy = analogRead(A1);
      jsx = analogRead(A0);
      moved = true;
    }
  }

  // foward right
  if (!moved) {
    while (jsx < b2 && jsy < b2) {
      Serial.println("forward right");
      frbl = -1;
      int mult = frbl*count;
      moveAll(pos1 + forRightBackLeft[0]*mult, pos2 + forRightBackLeft[1]*mult, pos3 + forRightBackLeft[2]*mult,
              pos4 + forRightBackLeft[3]*mult, pos5 + forRightBackLeft[4]*mult, pos6 + forRightBackLeft[5]*mult);
      delay(100);
      count++;
      jsx = analogRead(A0);
      jsy = analogRead(A1);
      moved = true;
    }
  }

  // backward left
  if (!moved) {
    while (jsx > b3 && jsy > b3) {
      Serial.println("backward left");
      frbl = 1;
      int mult = frbl*count;
      moveAll(pos1 + forRightBackLeft[0]*mult, pos2 + forRightBackLeft[1]*mult, pos3 + forRightBackLeft[2]*mult,
              pos4 + forRightBackLeft[3]*mult, pos5 + forRightBackLeft[4]*mult, pos6 + forRightBackLeft[5]*mult);
      delay(100);
      count++;
      jsx = analogRead(A0);
      jsy = analogRead(A1);
      moved = true;
    }
  }

  // forward left
  if (!moved) {
    while (jsx > b3 && jsy < b2) {
      Serial.println("forward left");
      flbr = 1; 
      int mult = flbr*count;
      moveAll(pos1 + forLeftBackRight[0]*mult, pos2 + forLeftBackRight[1]*mult, pos3 + forLeftBackRight[2]*mult,
              pos4 + forLeftBackRight[3]*mult, pos5 + forLeftBackRight[4]*mult, pos6 + forLeftBackRight[5]*mult); 
      delay(100);
      count++;
      jsx = analogRead(A0);
      jsy = analogRead(A1);
      moved = true;
    }
  }

  // backward right
  if (!moved) {
    while (jsx < b2 && jsy > b3) {
      Serial.println("backward right");
      flbr = -1; 
      int mult = flbr*count;
      moveAll(pos1 + forLeftBackRight[0]*mult, pos2 + forLeftBackRight[1]*mult, pos3 + forLeftBackRight[2]*mult,
              pos4 + forLeftBackRight[3]*mult, pos5 + forLeftBackRight[4]*mult, pos6 + forLeftBackRight[5]*mult); 
      delay(100);
      count++;
      jsx = analogRead(A0);
      jsy = analogRead(A1);
      moved = true;
    }
  }

  // ------------------------------------------
  // move it back to original plaform position
  if (moved) {
    if (x != 0) {
      while (count >= 0) {
        moveAll(pos1 + leftRight[0]*x*count, pos2 + leftRight[1]*x*count, pos3 + leftRight[2]*x*count, 
                pos4 + leftRight[3]*x*count, pos5 + leftRight[4]*x*count, pos6 + leftRight[5]*x*count);
        delay(100);
        count--;
      }
    }

    else if (y != 0) {
      while (count >= 0) {
        moveAll(pos1 + forBack[0]*y*count, pos2 + forBack[1]*y*count, pos3 + forBack[2]*y*count, 
                pos4 + forBack[3]*y*count, pos5 + forBack[4]*y*count, pos6 + forBack[5]*y*count); 
        delay(100);
        count--;
      }
    }
    
    else if (flbr != 0) {
      while (count >= 0) {
        moveAll(pos1 + forLeftBackRight[0]*flbr*count, pos2 + forLeftBackRight[1]*flbr*count, pos3 + forLeftBackRight[2]*flbr*count,
                pos4 + forLeftBackRight[3]*flbr*count, pos5 + forLeftBackRight[4]*flbr*count, pos6 + forLeftBackRight[5]*flbr*count); 
        delay(100);
        count--;
      }
    }
    
    else if (frbl != 0) {
      while (count >= 0) {
        moveAll(pos1 + forRightBackLeft[0]*frbl*count, pos2 + forRightBackLeft[1]*frbl*count, pos3 + forRightBackLeft[2]*frbl*count,
                pos4 + forRightBackLeft[3]*frbl*count, pos5 + forRightBackLeft[4]*frbl*count, pos6 + forRightBackLeft[5]*frbl*count);
        delay(100);
        count--;
      }
    }

    delay(500);

    // set them all back to initial position
    moveAll(pos1, pos2, pos3, pos4, pos5, pos6);
    delay(500);
  }
}

void moveAll(int s1, int s2, int s3, int s4, int s5, int s6) {
    servo1.write(s1);
    servo2.write(s2);
    servo3.write(s3);
    servo4.write(s4);
    servo5.write(s5);
    servo6.write(s6);
}
