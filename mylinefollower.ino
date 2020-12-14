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


#define NUM_SENSORS   8    // number of sensors used
#define TIMEOUT       2500  // waits for 2500 microseconds for sensor outputs to go HIGH
#define EMITTER_PIN   31     // emitter is controlled by digital pin 2
#define enableA 3
#define enableB 4
#define direction1motorB A0//A0 to IN1 right motor
#define direction2motorB A1//A1 to IN2 right motor
#define direction1motorA A3//A2 to IN3 left motor
#define direction2motorA A2//A3 to IN4 left motor
#define direction1motorC A4//A0 to IN1 right motor
#define direction2motorC A5//A1 to IN2 right motor
#define direction1motorD A6//A2 to IN3 left motor
#define direction2motorD A7
#define S0 24
#define S1 25
#define S2 26
#define S3 27
#define sensorOut 28

  
// sensors 0 through 7 are connected to digital pins 3 through 10, respectively
QTRSensorsRC qtrrc((unsigned char[]) {32, 33, 34, 35, 36,37,38,39 },
  NUM_SENSORS, TIMEOUT, EMITTER_PIN); 

unsigned int sensorValues[NUM_SENSORS];
int c=1,k=0;
int r=0,v=1;
int b=0;
int j,detectedcolour[3];
char givenarrangment[]= {'R','B','G'};
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
   //colour sensor
   pinMode(S0, OUTPUT);
   pinMode(S1, OUTPUT);
   pinMode(S2, OUTPUT);
   pinMode(S3, OUTPUT);
   pinMode(sensorOut,OUTPUT);
 //motor pins
   pinMode(enableA, OUTPUT);
   pinMode(enableB, OUTPUT);
   pinMode(direction1motorB, OUTPUT);
   pinMode(direction2motorB, OUTPUT);
   pinMode(direction1motorA, OUTPUT);
   pinMode(direction2motorA, OUTPUT); 
   pinMode(direction1motorC, OUTPUT);
   pinMode(direction2motorC, OUTPUT);
   pinMode(direction1motorD, OUTPUT);
   pinMode(direction2motorD, OUTPUT); 

//pololu pins
   pinMode(32,OUTPUT);
   pinMode(33,OUTPUT);
   pinMode(34,OUTPUT);
   pinMode(35,OUTPUT);
   pinMode(36,OUTPUT);
   pinMode(37,OUTPUT);
   pinMode(38,OUTPUT);
   pinMode(39,OUTPUT);
   pinMode(31,OUTPUT);
 
   //led pins
   pinMode(22,OUTPUT);
   pinMode(23,OUTPUT);
   pinMode(29,OUTPUT);
   pinMode(30,OUTPUT);
        
  
  for (int i = 0; i < 400; i++)                         // make the calibration take about 10 seconds
  {
    qtrrc.calibrate();                                    // reads all sensors 10 times at 2500 us per read (i.e. ~25 ms per call)
  }
   
                                                    // print the calibration minimum values measured when emitters were on
  Serial.begin(9600);
  for (int i = 0; i < NUM_SENSORS; i++)
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
   Forward();                               //works for a small time for multiple times
  
 if(sidesensorsL[0]==1 && sensorValues[3]==1 &&sidesensorsR[0]==0)
{
   
          Serial.print("left turn");
        if(c<=4 && c>1){ 
        godetect();  
      }
       c=c+1;
      leftturn();
  }
else if(sidesensorsR[0]==1 && sensorValues[5]==1 && sidesensorsL[0]==0)
{
            Serial.print("right turn"); 
           if(c<=4 && c>1)
       {
       godetect();
       }
     c=c+1;
    rightturn();
}
 
else if (sidesensorsL[0]==1 &&  sidesensorsR[0]==1)
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











}


 //functions below
 
 char* godetect(){
     analogWrite(enableA,rightMaxSpeed);
    analogWrite(enableB,leftMaxSpeed);
    delay(200);
    analogWrite(enableA,0);
    analogWrite(enableB,0);
   delay(200);
   detectedcolour[b]= detect();
   b=b+1;
   back();
   delay(200);
 return colour;
   }
 
  
  void rightstack(int t)
  {
    int w=1;
     h: back();
sensor_Values(); 
      if(sidesensorsL[0]==1 && sidesensorsR[0]==1)
     { 
        rightturn();
        }
    else 
    goto h;
  k: sensor_Values(); 
   while((sidesensorsL[0]!=1 && sidesensorsR[0]!=1)){
    sensor_Values();
     Forward();}
     if(sidesensorsL[0]==1 && sidesensorsR[0]==1)
     {  
      Stop(); 
        if(w==t){
     leave();
        delay(4000);
         back();
        delay(t*100);
        rotate();
        
        }
        if(w!=t)
       { 
        w=w+1;
        goto k;
       
       }       
     while((sidesensorsL[0]!=1 && sidesensorsR[0]!=1)){
       sensor_Values();
        Forward();}
       
        if(sidesensorsL[0]==1 && sidesensorsR[0]==1){
         rightturn();
         }
      
     }
     }
    
     
     
     
   
   void leftstack(int t)
  {
  h:   back();
    if(sidesensorsL[0]==1 && sidesensorsR[0]==1)
    { 
      leftturn();
        }
    else
    goto h;
     k: sensor_Values(); 
   while((sidesensorsL[0]!=1 && sidesensorsR[0]!=1)){
    sensor_Values();
     Forward();}
     if(sidesensorsL[0]==1 && sidesensorsR[0]==1)
     { 
      int w=1;
      Stop();
        if(w==t){
       leave();
        delay(4000);
         back();
        delay(t*100);
        rotate();
        
        }
        if(w!=t)
       { 
        w=w+1;
        goto k;
       
       }      
     while((sidesensorsL[0]!=1 && sidesensorsR[0]!=1)){
        sensor_Values();
        Forward();}
       
        if(sidesensorsL[0]==1 && sidesensorsR[0]==1){
         leftturn();
         }
      
     }
    }
  
 void R2L()
 {
 
   h:   back();
    if(sidesensorsL[0]==1 && sidesensorsR[0]==1)
    { 
      rightturn();
        }
    else
    goto h;
      k: sensor_Values(); 
   while((sidesensorsL[0]!=1 && sidesensorsR[0]!=1)){
    sensor_Values();
     Forward();}
     if(sidesensorsL[0]==1 && sidesensorsR[0]==1)
     {  
      Stop(); 
        if(w==t){
     pick();
        delay(500);
         back();
        delay(t*100);
        rotate();
        
        }
        if(w!=t)
       { 
        w=w+1;
        goto k; 
 
  
  }
    Forward();
  
  
  }
  void rightturn()
  {
    
    
    digitalWrite(direction1motorA, HIGH);
    digitalWrite(direction2motorA, LOW);
    digitalWrite(direction2motorB, LOW);
    digitalWrite(direction1motorB, HIGH);
    analogWrite(enableA, 0);
    analogWrite(enableB,255);
    delay(1000);
    }
  
  void leftturn()
  {
    digitalWrite(direction1motorA, HIGH);
    digitalWrite(direction2motorA, LOW);
    digitalWrite(direction2motorB, LOW);
    digitalWrite(direction1motorB, HIGH);
    analogWrite(enableA, 255);
    analogWrite(enableB,0);
    delay(1000);
    } 
  
  
  void back()
    {
  
    digitalWrite(direction2motorA, HIGH);
    digitalWrite(direction1motorA, LOW);
    digitalWrite(direction1motorB, LOW);
    digitalWrite(direction2motorB, HIGH); 
    analogWrite(enableA,175);
    analogWrite(enableB,175);
    }
    void Stop()
    {
  
    digitalWrite(direction1motorA, HIGH);
    digitalWrite(direction2motorA, LOW);
    digitalWrite(direction2motorB, LOW);
    digitalWrite(direction1motorB, HIGH); 
    analogWrite(enableA,0);
    analogWrite(enableB,0);
    }
 
   void rotate()
   {
    
      digitalWrite(direction1motorA, HIGH);
    digitalWrite(direction2motorA, LOW);
    digitalWrite(direction1motorB, LOW);
    digitalWrite(direction2motorB, HIGH); 
    analogWrite(enableA,255);
    analogWrite(enableB,255);
    
    
    } 
 void Forward(){ 
   
   int error=position-3500;
 Serial.println(error);
 int motorSpeed = Kp * error + Kd * (error - lastError);
  lastError = error;

  int rightMotorSpeed = rightBaseSpeed - motorSpeed;
  int leftMotorSpeed = leftBaseSpeed + motorSpeed;
  
    if (rightMotorSpeed > rightMaxSpeed ) rightMotorSpeed = rightMaxSpeed;                   // prevent the motor from going beyond max speed
  if (leftMotorSpeed > leftMaxSpeed ) leftMotorSpeed = leftMaxSpeed;                         // prevent the motor from going beyond max speed
  if (rightMotorSpeed < 0) rightMotorSpeed = 0;                                              // keep the motor speed positive
  if (leftMotorSpeed < 0) leftMotorSpeed = 0;                                                // keep the motor speed positive
  
 
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
  qtrrc.read(sensorValues);
 for (int i=0;i<=5;i++)
 {
   if(sensorValues[i]<600)
   {
    sensorValues[i]=1;
    
   }
   else 
   sensorValues[i]=0;

  }
  
  }
   void pick()
 {
  digitalWrite(A4,HIGH);
  digitalWrite(A5,LOW);
  delay(600); 
  digitalWrite(A4,LOW);
  delay(1000);
  digitalWrite(A6,HIGH);
  digitalWrite(A7,LOW);
delay(150);
  digitalWrite(A6,LOW);
 }

void leave()
{
  digitalWrite(A7,HIGH);
  digitalWrite(A6,LOW);
  delay(150);
  digitalWrite(A7,LOW);
delay(1000);
digitalWrite(A5,HIGH);
 digitalWrite(A4,LOW); 
  delay(600);
  digitalWrite(A5,LOW);

  
  }
char detect() 
{
 
  int frequency_r ,frequency_b,frequency_g;                        // Setting red filtered photodiodes to be read
  digitalWrite(S2,LOW);
  digitalWrite(S3,LOW);
                                  
  frequency_r = pulseIn(sensorOut, LOW);
 
  digitalWrite(S2,HIGH);
  digitalWrite(S3,HIGH);
                          
  frequency_g = pulseIn(sensorOut, LOW);
 
  digitalWrite(S2,LOW);
  digitalWrite(S3,HIGH);

  frequency_b = pulseIn(sensorOut, LOW);
  if(frequency_r-frequency_b>30 && 150<frequency_r < 200 && frequency_g > frequency_r && frequency_b < frequency_r )
    {
  
   digitalWrite(22,HIGH);
  delay(1000);
  digitalWrite(22,LOW);
return 'B';  
   } if((frequency_g-frequency_b)>15 &&(frequency_r)<190 && 20<frequency_r && frequency_b > frequency_r && frequency_g>frequency_b) 
{
Serial.println("this is red");
digitalWrite(23,HIGH);
delay(1000);
digitalWrite(23,LOW);
return 'R'; 
}
  if(40>abs(frequency_g-frequency_b)   && frequency_r<180  && 40>abs(frequency_g- frequency_r) && 80< frequency_r && 80< frequency_g && frequency_g<180&& 80< frequency_b && frequency_b<180)
{
  //Serial.println("this is green");
  digitalWrite(29,HIGH);
delay(1000);
digitalWrite(29,LOW);
return 'G';
  }
if((frequency_b-frequency_g)<50 && (frequency_g-frequency_r)<50 && frequency_g>frequency_r && frequency_g<frequency_b && frequency_r>10)
{
  

digitalWrite(30,HIGH);
  delay(1000);
  digitalWrite(30,LOW);
  return 'Y';
 }
}
