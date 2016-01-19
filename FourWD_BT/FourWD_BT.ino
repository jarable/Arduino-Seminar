/*
m_Ctrl((Connects to a Bluetooth module and parses data sent from a device. Works with Arduino Uno and also tested to work
 with a Digispark (must remove all Serial.print)
 Expects string data to come as follows:
 sX=[xVal],Y=[yVal]e  - used for sending X/Y data. Example:  sX=75,Y=55e.  Handles +/-
 sC=[cVal]e - used for sending C (command button) data. Example: sC=1e
 
 https://play.google.com/store/apps/details?id=appinventor.ai_picoware.BTBotControlLite&hl=en
 
 */


#include <SoftwareSerial.h>
//for Digispark
//#define RxPin 3  //goes to TX pin on BT module
//#define TxPin 4 //goes to RX pin on BT module

//for testing on UNO
#define RxPin 9  //goes to TX pin on BT module
#define TxPin 8 //goes to RX pin on BT module

#define SOP 's'
#define EOP 'e'

#define left 0  // left drive motor
#define right 1  // right drive motor

#define FWD 0  // left drive motor
#define REV 1  // right drive motor
#define STP 2  // right drive motor
#define RGT 1 // right turn
#define LFT 0 // left turn


//-----------------
char inData[20];
byte index;
int xVal = 0, xAdj = 0, yVal = 0, yAdj = 0, cVal = 0;
int leftSpeed = 0, rightSpeed = 0;

bool started = false;
bool ended = false;
SoftwareSerial blueToothSerial(RxPin, TxPin); // RX, TX

int i;
int j;
int m[4][2] = {
  {
    4 , 5
  }
  , // motor 0 front left motor <right lead>, <left lead>
  {
    6, 7
  }
  ,// motor 1 front right motor <right lead>, <left lead>
  {
    10, 11
  }
  ,// motor 2 rear left  motor <right lead>, <left lead>
  {
    12, 13
  }
  // motor 3 rear right front motor <right lead>, <left lead>
  //PWM channels 3, 5*, 6*, 9, 10, and 11.
};
void setup()
{
  Serial.begin(9600);
  blueToothSerial.begin(9600);
  // Other stuff...
 // pinMode(RxPin, INPUT);
 // pinMode(TxPin, OUTPUT);
  // Setup motors
  int i;
  int j;

  for (i = 0; i <= 3; i++) {
    for (j = 0; j <= 3; j++) {
      pinMode(m[i][j], OUTPUT);
    }
  }
  pinMode(RxPin, INPUT);
  pinMode(TxPin, OUTPUT);

}
/*
 if (blueToothSerial.available() > 0){
 char choice = blueToothSerial.read();
 Serial.print("Val=");
 Serial.println(choice);
 }
 */

void loop()
{
  xVal = 0;
  yVal = 0;


  // Read all serial data available, as fast as possible
  //while(Serial.available() > 0)


  while (blueToothSerial.available() > 0)
  {

    //char inChar = Serial.read();
    char inChar = blueToothSerial.read();
    if (inChar == SOP)
    {
      index = 0;
      inData[index] = '\0';
      started = true;
      ended = false;
    }
    else if (inChar == EOP)
    {
      ended = true;
      break;
    }
    else
    {
      if (index < 19)
      {
        inData[index] = inChar;
        index++;
        inData[index] = '\0';
      }
    }
  }




  // We are here either because all pending serial
  // data has been read OR because an end of
  // packet marker arrived. Which is it?
  if (started && ended)
  {
    // The end of packet marker arrived. Process the packet
    char *name = strtok(inData, "=");
    while (name)
    {
      char *valToken = strtok(NULL, ",");
      if (valToken)
      {
        int val = atoi(valToken);
        if (strcmp(name, "X") == 0)
        {
          xVal = val;
          //digitalWrite(led1, HIGH);
        }
        else if (strcmp(name, "Y") == 0)
        {
          yVal = val;
          //digitalWrite(led2, HIGH);
        }
        else if (strcmp(name, "C") == 0)
        {
          cVal = val;
          //digitalWrite(led3, HIGH);
        }
        // More else if's go here if more features are added to BT Bot Control - Android App.
      }
      name = strtok(NULL, "=");
    }

    //digitalWrite(led1, LOW);
    //digitalWrite(led2, LOW);
    //testing.
    //** Note - debugging to the Serial Monitor will cause Serial.available to crap out and make it seem
    // like the device has stopped outputting BT data. The reason is that the Serial buffer overflowed due to
    //printing debuggin statements.  Turn debugging on only to verify you are getting data and test a few values

    //output values to Serial Monitor
    //Serial.print("X=");
    //Serial.print(xVal);
    //Serial.print("  Y=");
    //Serial.print(yVal);
    //Serial.print("  C=");
    //Serial.print(cVal);

    if ((xVal != 0) || (yVal != 0))
    {
      //allow for a state where the robot is not moving with small joystick movements.
      //if (((xVal < 5) && (xVal > -5)) && ((yVal < 5) && (yVal > -5))){ xVal=0; yVal=0;}

      driveMotors(xVal, yVal);
    }
    else {
      //turn motors off
      //m_Ctrl(left, 0, 0 );
      //m_Ctrl(right, 0, 0 );
      all_Go(255, STP);
    }


    //you can also process the commands here as well.

    // Reset for the next packet
    started = false;
    ended = false;
    index = 0;
    inData[index] = '\0';
  }

  xVal = 0, yVal = 0;

  //make sure motors are off with each iteration of the loop in the event that Bluetooth is cut off.
  //analogWrite(leftMtrSpdPin, 0);
  //analogWrite(rightMtrSpdPin, 0);
  //delay(50);
} //end main loop




void driveMotors(int xVal, int yVal)
{
  //both motors forward full speed
  //digitalWrite(MotorADir, HIGH);   //forward
  //digitalWrite(MotorBDir, LOW);
  //analogWrite(MotorASpeed, 100); //100 will be the lowest speed to start at.
  //analogWrite(MotorBSpeed, 100);

  float xPct = 1.0;

  //xAdj = map(abs(xVal), 0, 100, 50, 255);  //start motor speed of 50 is as low as these motors will work.  Your motors may be different
  //yAdj = map(abs(yVal), 0, 100, 50, 255);  //start motor speed of 50 is as low as these motors will work.  Your motors may be different

  int baseVal = 0;

  switch (cVal)
  {
    case 1:
      baseVal = 0;
      break;
    case 2:
      baseVal = 20;
      break;
    case 3:
      baseVal = 50;
      break;
  }

  xAdj = map(abs(xVal), 0, 100, baseVal, 255);
  yAdj = map(abs(yVal), 0, 100, baseVal, 255);

  //xPct = map(abs(xAdj), 0, 255, 1, 0);
  xPct = mapf((float)abs(xAdj), (float)0, (float)255, (float)1, (float)0);


  //Serial.print("  xadj: "); printDouble(xAdj,10);
  //Serial.print("  yadj: "); printDouble(yAdj,10);
  //Serial.print("  yVal: "); printDouble(yVal,10);
  //Serial.print("  xAdj: "); printDouble(xAdj,10);
  //Serial.print("  xVal: "); printDouble(xVal,10);
  //Serial.print("  x%: "); printDouble(xPct,10);
  //drive based on quadrant


  if (((xVal > -20) && (xVal < 20)) && (((yVal <= 100) && (yVal > 60)) || ((yVal >= -100) && (yVal < -60))))
  {
    if  ((yVal <= 100) && (yVal > 60))    //1:  joystick full fwd.  Both motors full speed ahead.
    {
      //m_Ctrl(left,255,FWD );
      //m_Ctrl(right,255,FWD );
      all_Go(255, FWD);
      //Serial.print(" - Full FWD");
    }
    else if ((yVal >= -100) && (yVal < -60))  //2:  joystick full rev. Both motors full speed rev.
    {
      // m_Ctrl(left,255,REV );
      // m_Ctrl(right,255,REV );
      all_Go(255, REV);

      //Serial.print(" - Full REV");
    }

  }
  else if  (((yVal > -20) && (yVal < 20)) && (((xVal <= 100) && (xVal > 60)) || ((xVal >= -100) && (xVal < -60))))
  {
    if ((xVal <= 100) && (xVal > 60))  //3:  Fast turn right. Rt Mtr full fwd. Left Mtr full rev.
    {
      //m_Ctrl(left,150,FWD );
      //m_Ctrl(right,150,REV );
      turn(255, RGT);
      //Serial.print(" - Full RT");
    }
    else if ((xVal >= -100) && (xVal < -60))  //4: Fast turn left: Left Mtr full fwd. Rt Mtr full rev.
    {
      //m_Ctrl(left,150,REV );
      //m_Ctrl(right,150,FWD );
      turn(255, LFT);
      //Serial.print(" - Full LF");
    }
  }

  // if not extreme corners, do normal movements.
  else if (yVal > 0)  //going forward
  {
    //robot driving fwd - now determine left and right.
    m_Fwd(left);
    m_Fwd(right);
    if (xVal < -15) //going left  - leave room for going straight
    {
      m_Spd(left, yAdj * xPct);    //drive the motor
      m_Spd(right, yAdj);  //drive the motor
      //Serial.print("FwLF: "); printDouble(xPct,10);
    }
    else if (xVal > 15)
    { //going right
      m_Spd(left, yAdj);    //drive the motor
      m_Spd(right, yAdj  * xPct);  //drive the motor
      //Serial.print("FwRT: "); printDouble(xPct,10);
    }
    else  //x is beteen -15 and 15 - assume going straight
    {
      m_Spd(left, yAdj);    //drive the motor
      m_Spd(right, yAdj);   //drive the motor
      //Serial.print("Normal FWD: ");
    }
  }
  else   //y val is < 0 - reversing
  {
    //robot driving backwards - now determine left and right.
    m_Rev(left);
    m_Rev(right);

    if (xVal < -15) //going left  - leave room for going straight
    {
      m_Spd(left, yAdj * xPct);    //drive the motor
      m_Spd(right, yAdj);  //drive the motor
      //Serial.print("RevLF: "); printDouble(xPct,10);
    }
    else if (xVal > 15)
    {
      m_Spd(left, yAdj);    //drive the motor
      m_Spd(right, yAdj  * xPct);  //drive the motor
      //Serial.print("RevRT: "); printDouble(xPct,10);
    }
    else  //x is beteen -15 and 15 - assume going straight
    {
      m_Spd(left, yAdj);    //drive the motor
      m_Spd(right, yAdj);   //drive the motor
      //Serial.print("NormalRev: ");
    }

  }
  //slight delay
  delay(50);
  //Serial.println();
  leftSpeed = 0, rightSpeed = 0, xAdj = 0, yAdj = 0;
}


//void printDouble( double val, unsigned int precision){
// prints val with number of decimal places determine by precision
// NOTE: precision is 1 followed by the number of zeros for the desired number of decimial places
// example: printDouble( 3.1415, 100); // prints 3.14 (two decimal places)

//    Serial.print (int(val));  //prints the int part
//    Serial.print("."); // print the decimal point
//    unsigned int frac;
//    if(val >= 0)
//        frac = (val - int(val)) * precision;
//    else
//        frac = (int(val)- val ) * precision;
//    Serial.println(frac,DEC);
//}

//use to map values to float.
float mapf (float x, float in_min, float in_max, float out_min, float out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}




