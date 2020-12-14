#include <QTRSensors.h>
// This example is designed for use with eight QTR-1RC sensors or the eight sensors of a
// QTR-8RC module.  These reflectance sensors should be connected to digital inputs 3 to 10.
// The QTR-8RC's emitter control pin (LEDON) can optionally be connected to digital pin 2, 
// or you can leave it disconnected and change the EMITTER_PIN #define beHIGH from 2 to 
// QTR_NO_EMITTER_PIN.

// The setup phase of this example calibrates the sensor for ten seconds and turns on
// the LED built in to the Arduino on pin 13 while calibration is going on.
// During this phase, you should expose each reflectance sensor to the lightest and 
// darkest readings they will encounter.
// For example, if you are making a line folHIGHer, you should slide the sensors across the
// line during the calibration phase so that each sensor can get a reading of how dark the
// line is and how light the ground is.  Improper calibration will result in poor readings.
// If you want to skip the calibration phase, you can get the raw sensor readings
// (pulse times from 0 to 2500 us) by calling qtrrc.read(sensorValues) instead of
// qtrrc.readLine(sensorValues).

// The main loop of the example reads the calibrated sensor values and uses them to
// estimate the position of a line.  You can test this by taping a piece of 3/4" black
// electrical tape to a piece of white paper and sliding the sensor across it.  It
// prints the sensor values to the serial monitor as numbers from 0 (maximum reflectance) 
// to 1000 (minimum reflectance) folHIGHed by the estimated location of the line as a number
// from 0 to 5000.  1000 means the line is directly under sensor 1, 2000 means directly
// under sensor 2, etc.  0 means the line is directly under sensor 0 or was last seen by
// sensor 0 before being lost.  5000 means the line is directly under sensor 5 or was
// last seen by sensor 5 before being lost.


#define NUM_SENSORS   8  // number of sensors used
#define TIMEOUT       2500  // waits for 2500 microseconds for sensor outputs to go HIGH
#define EMITTER_PIN   31     // emitter is controlled by digital pin 2
#define enableA 11
#define enableB 12
#define direction1motorB A0//A0 to IN1 right motor
#define direction2motorB A1//A1 to IN2 right motor
#define direction1motorA A3//A2 to IN3 left motor
#define direction2motorA A2//A3 to IN4 left motor

// sensors 0 through 7 are connected to digital pins 3 through 10, respectively
QTRSensorsRC qtrrc((unsigned char[]) {32,33,34,35,36,37,38,39 },
  NUM_SENSORS, TIMEOUT, EMITTER_PIN); 
QTRSensorsRC qtrrc1((unsigned char[]) {40,41},
  2, TIMEOUT, QTR_NO_EMITTER_PIN); 
unsigned int sensorValues[NUM_SENSORS];
unsigned int sidesensors[2];
int c=1,k=0;
int r=0,v=1;
int j,detectedcolour[3];
int SumLeft = 0;
int SumRight = 0;
char givenarrangment[]= {'R','B','G'};
int SumDifference = 0;
int lastError = 0;
float Kp=.5;
float  Kd=0.5;
int rightBaseSpeed=175;
int leftBaseSpeed=175;
int rightMaxSpeed=200;
int leftMaxSpeed=200;
unsigned int position;
void setup()
{
  delay(500);
   pinMode(enableA, OUTPUT);
   pinMode(enableB, OUTPUT);
   pinMode(direction1motorB, OUTPUT);
   pinMode(direction2motorB, OUTPUT);
   pinMode(direction1motorA, OUTPUT);
   pinMode(direction2motorA, OUTPUT); 
 //Wire.begin();
// Wire.onReceive(receiveEvent); 
  //pinMode(13, OUTPUT);
  //digitalWrite(13, LOW);    // turn on Arduino's LED to indicate we are in calibration mode
  for (int i = 0; i < 400; i++)  // make the calibration take about 10 seconds
  {
    qtrrc.calibrate();       // reads all sensors 10 times at 2500 us per read (i.e. ~25 ms per call)
  //  qtrrc1.calibrate(); 
  }
 // digitalWrite(13, HIGH);     // turn off Arduino's LED to indicate we are through with calibration

  // print the calibration minimum values measured when emitters were on
  Serial.begin(9600);
/*  for (int i = 0; i < 2; i++)
  {
    Serial.print(qtrrc1.calibratedMinimumOn[i]);
    Serial.print(' ');
  }
  Serial.println();
 
  // print the calibration maximum values measured when emitters were on
  for (int i = 0; i <2; i++)
  {
    Serial.print(qtrrc1.calibratedMaximumOn[i]);
    Serial.print(' ');
  }
  Serial.println();
  Serial.println();
  delay(1000);

  */for (int i = 0; i < NUM_SENSORS; i++)
  {
    Serial.print(qtrrc.calibratedMinimumOn[i]);
    Serial.print(' ');
  }
  Serial.println();
 
  // print the calibration maximum values measured when emitters were on

 for (int i = 0; i < NUM_SENSORS; i++)
  {
    Serial.print(qtrrc.calibratedMaximumOn[i]);
    Serial.print(' ');
  }
  Serial.println();
  Serial.println();
  delay(1000);
}


void loop()
{
  // read calibrated sensor values and obtain a measure of the line position from 0 to 5000
  // To get raw sensor values, call:
  //  qtrrc.read(sensorValues); instead of unsigned int position = qtrrc.readLine(sensorValues);
 sensor_Values();
   Forward(position); //works for a small time for multiple times
for (int i=0;i<=7;i++)
{
  Serial.print(sensorValues[i]);
  }
  Serial.print(' ');
Serial.print(sidesensors[0]);
Serial.print(' ');
Serial.println(sidesensors[1]);
   if(sidesensors[0]==1 && sensorValues[3]==1 && sidesensors[1]==0)
{
   
          Serial.print("left turn");
      /*  if(c<=4 && c>1){ 
        godetect();  
      }
       c=c+1;*/
      leftturn();
  }
else if(sidesensors[1]==1 && sensorValues[4]==1 && sidesensors[0]==0)
{
            Serial.print("right turn"); 
         /*  if(c<=4 && c>1)
       {
       godetect();
       }
     c=c+1;*/
    rightturn();
}
 /*
else if (sidesensors[0]==1 &&  sidesensors[1]==1)
{
      if(k>1)
      {
      pick();
       delay(4000);
     if(givenarrangment[v]==detectedcolour[v])
     {
    
      
      
      rightstack(v);
      } 
    else
    leftstack(v);
    }
   v=v+1;
    k=k+1;
 }
*/

}




 void Forward( int position){ 
   
  Serial.print("Forward"); 
   int error=position-3500;
 Serial.println(error);
 int motorSpeed = Kp * error + Kd * (error - lastError);
  lastError = error;

  int rightMotorSpeed = rightBaseSpeed - motorSpeed;
  int leftMotorSpeed = leftBaseSpeed + motorSpeed;
  
    if (rightMotorSpeed > rightMaxSpeed ) rightMotorSpeed = rightMaxSpeed; // prevent the motor from going beyond max speed
  if (leftMotorSpeed > leftMaxSpeed ) leftMotorSpeed = leftMaxSpeed; // prevent the motor from going beyond max speed
  if (rightMotorSpeed < 0) rightMotorSpeed = 0; // keep the motor speed positive
  if (leftMotorSpeed < 0) leftMotorSpeed = 0; // keep the motor speed positive
  
 
    digitalWrite(direction1motorA, HIGH);
    digitalWrite(direction2motorA, LOW);
    digitalWrite(direction2motorB, LOW);
    digitalWrite(direction1motorB, HIGH);   
    analogWrite(enableA,rightMotorSpeed);
    analogWrite(enableB,leftMotorSpeed);
    
}
 void sensor_Values()
 {
   unsigned int position = qtrrc.readLine(sensorValues);
 
     qtrrc1.read(sidesensors);
/* for (int i=0;i<=1;i++)
 {
   if(sidesensors[i]<600)
   {
    sidesensors[i]=1;
    
   }
   else 
   sidesensors[i]=0;

  
  }*/
 for (int i=0;i<=7;i++)
 {
   if(sensorValues[i]<600)
   {
    sensorValues[i]=1;
    
   }
   else 
   sensorValues[i]=0;
  
  }
  
  }
  void rightturn()
  {
    
    
    digitalWrite(direction1motorA, HIGH);
    digitalWrite(direction2motorA, LOW);
    digitalWrite(direction2motorB, LOW);
    digitalWrite(direction1motorB, HIGH);
    analogWrite(enableA, 0);
    analogWrite(enableB,255);
   // delay(1000);
    }
  
  void leftturn()
  {
    digitalWrite(direction1motorA, HIGH);
    digitalWrite(direction2motorA, LOW);
    digitalWrite(direction2motorB, LOW);
    digitalWrite(direction1motorB, HIGH);
    analogWrite(enableA, 255);
    analogWrite(enableB,0);
    //delay(1000);
    } 
  
  
