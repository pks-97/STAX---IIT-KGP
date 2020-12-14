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
#define direction1motorB A3
#define direction2motorB A2
#define direction1motorA A0
#define direction2motorA A1
#define direction1motorC A4
#define direction2motorC A5
#define direction1motorD A6
#define direction2motorD A7
#define S0 24
#define S1 25
#define S2 26
#define S3 27
#define sensorOut 28

  
// sensors 0 through 7 are connected to digital pins 3 through 10, respectively
QTRSensorsRC qtrrc((unsigned char[]) {32, 33, 34, 35, 36,37,38,39 },
  NUM_SENSORS, TIMEOUT, EMITTER_PIN); 
QTRSensorsRC qtrrc1((unsigned char[]) {40},
  1, TIMEOUT, QTR_NO_EMITTER_PIN); 
QTRSensorsRC qtrrc2((unsigned char[]) {41 },
  1, TIMEOUT, QTR_NO_EMITTER_PIN); 

unsigned int sensorValues[NUM_SENSORS];
unsigned int sidesensorsL[1];
unsigned int sidesensorsR[1];
int c=1,k=0;
int r=0,v=1;
int b=0,y=0;
int j,detectedcolour[3];
char givenarrangement[]= {'R','B','G'};
int lastError = 0;
float Kp=.5;
float  Kd=0.5;
int rightBaseSpeed=200;
int leftBaseSpeed=200;
int rightMaxSpeed=225;
int leftMaxSpeed=225;
int SumLeft = 0;
int SumRight = 0;
int SumDifference = 0;
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

   //led pins
   pinMode(A11,OUTPUT);
   pinMode(A12,OUTPUT);
   pinMode(A13,OUTPUT);
   pinMode(A14,OUTPUT);
   pinMode(50,OUTPUT);
   digitalWrite(50,LOW);
        
  
  for (int i = 0; i < 400; i++)                         // make the calibration take about 10 seconds
  {
    qtrrc.calibrate();                                    // reads all sensors 10 times at 2500 us per read (i.e. ~25 ms per call)
     qtrrc1.calibrate();
     qtrrc2.calibrate();
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
  Serial.print(qtrrc1.calibratedMinimumOn[0]);
  Serial.print(' ');
  Serial.print(qtrrc2.calibratedMinimumOn[0]);
  Serial.println();
  
  Serial.print(qtrrc1.calibratedMaximumOn[0]);
  Serial.print(' ');
  Serial.print(qtrrc2.calibratedMaximumOn[0]);
  Serial.println();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////  MAIN CODE  ///////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////






void loop()
{
                                                                                          // read calibrated sensor values and obtain a measure of the line position from 0 to 5000
                                                                         // To get raw sensor values, call:
                                                                                        //  qtrrc.read(sensorValues); instead of unsigned int position = qtrrc.readLine(sensorValues);
 
   Follow();
   sensor_Values();
 for (int i=0;i<=7;i++)
{
  Serial.print(sensorValues[i]);
  Serial.print(' ');
  }
  Serial.print(' ');
Serial.print(sidesensorsL[0]);
Serial.print(' ');
Serial.println(sidesensorsR[0]); 
int m=sensorValues[0]+sensorValues[1]+sensorValues[2]+sensorValues[3]+sensorValues[4]+sensorValues[5];
int n=sensorValues[2]+sensorValues[3]+sensorValues[4]+sensorValues[5]+sensorValues[6]+sensorValues[7];
 if(sidesensorsL[0]==1&&sidesensorsR[0]==0     &&     c<=4)
{     if(c==1){
          Forward();
          delay(290);
        }
   Stop();delay(800);
          Serial.print("left turn");
        if(c>1){ 
        godetect();  
      }
       c=c+1;
     HardLeftx();
  }
else if(sidesensorsR[0]==1 && sidesensorsL[0]==0   &&  c<=4)
{              Stop();delay(800);
            Serial.print("right turn"); 
           if(c>1)
       {
       godetect();
       }
     c=c+1;
   HardRightx();
}
 
else if (sidesensorsL[0]==1 &&  sidesensorsR[0]==1)
{          Stop();delay(10000);
  
  
  /*
}
                                         ////////////SORTING//////////////
for(int i=0;i<3;i++){
        if(i==0){
            if(detectedcolour[i]==givenarrangement[2]){
                mainstackP(1);leftstackL(1);//st1[1]=in[2];
                //in[2]='0';
                //abcd(in,st1,st2);
                mainstackP(2);rightstackL(2);//st2[0]=in[1];
                //in[1]='0';
                //abcd(in,st1,st2);
                mainstackP(3);rightstackL(1);//st2[1]=in[0];
                //in[0]='0';
                //abcd(in,st1,st2);
            }
            else if(detectedcolour[i]==givenarrangement[1]){
                mainstackP(1);rightstackL(2);//st2[0]=in[2];
                //in[2]='0';
                //abcd(in,st1,st2);
                mainstackP(2);leftstackL(1);//st1[1]=in[1];
                //in[1]='0';
                //abcd(in,st1,st2);
                mainstackP(3);rightstackL(1);//st2[1]=in[0];
                //in[0]='0';
                //abcd(in,st1,st2);
            }
            else if(detectedcolour[i]==givenarrangement[0]){
                if(detectedcolour[1]!=givenarrangement[1]){
                    mainstackP(1);leftstackL(1);//st1[1]=in[2];
                    //in[2]='0';
                    //abcd(in,st1,st2);
                    mainstackP(2);rightstackL(1);//st2[1]=in[1];
                    //in[1]='0';
                    //abcd(in,st1,st2);
                    y=1;
                }
            }
            if(y==0){
                leftstackP(1);mainstackL(3);//in[0]=st1[1];
                //st1[1]='0';
                //abcd(in,st1,st2);
            }
        }
        if(i==1){
            if(detectedcolour[i]!=givenarrangement[i]){
                if(y==0){
                    if(detectedcolour[1]==givenarrangement[0]){
                        rightstackP(1);mainstackL(2);//in[1]=st2[1];
                        //st2[1]='0';
                        //abcd(in,st1,st2);
                        rightstackP(2);mainstackL(1);//in[2]=st2[0];
                        //st2[0]='0';
                        //abcd(in,st1,st2);
                    }
                    else{
                        rightstackP(1);leftstackL(1);//st1[1]=st2[1];
                        //st2[1]='0';
                        //abcd(in,st1,st2);
                        rightstackP(2);mainstackL(2);//in[1]=st2[0];
                        //st2[0]='0';
                        //abcd(in,st1,st2);
                        leftstackP(1);mainstackL(1);//in[2]=st1[1];
                        //st1[1]='0';
                        //abcd(in,st1,st2);
                    }
                }
                else{
                    leftstackP(1);mainstackL(2);//in[1]=st1[1];
                    //st1[1]='0';
                    //abcd(in,st1,st2);
                    rightstackP(1);mainstackL(1);//in[2]=st2[1];
                    //st2[1]='0';
                    //abcd(in,st1,st2);
                }
            }
        }

    }*/
 }











}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 /////////////////////////FUNCTIONS BELOW///////////////////////////////////////////////////////////////////////////////////////////////
 
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

 
 
 
 char* godetect(){
   
  Forward();
    delay(290);
     Stop();
   delay(1000);
   detectedcolour[b]= detect();
   b=b+1;
   Serial.println("detected");
   delay(800);
   //back();
   //delay(200);
    Stop();
    delay(200);
   }
 
///////////////////////////////////////////////////////////////////////////////////////////////////////////////  
                                                   //right jaake pick kr
   void rightstackP(int t)
  {
    int w=1;
     h:back();
        if(sidesensorsL[0]!=1 && sidesensorsR[0]!=1)
        {
          sensor_Values();
          goto h;  
          }
    sensor_Values();
      if(sidesensorsL[0]==1 && sidesensorsR[0]==1)
     { 
        rightturn();
        }
    
  k: sensor_Values(); 
   while((sidesensorsL[0]!=1 && sidesensorsR[0]!=1)){
    sensor_Values();
     Forward();}
     if(sidesensorsL[0]==1 && sidesensorsR[0]==1)
     {  
      Stop(); 
        if(w==t){
     pick();
        delay(2000);
         back();
        delay((t-1)*100);
        rotate();
        
        }
        if(w!=t)
       { 
        w=w+1;
        goto k;
       
       }       
       sensor_Values();
     while((sidesensorsL[0]!=1 && sidesensorsR[0]!=1)){
       sensor_Values();
        Forward();}
       
        if(sidesensorsL[0]==1 && sidesensorsR[0]==1){
         rightturn();
        Stop();
         }
      
     }
     }
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                                                                 //right jaake leave kar
  
  
  void rightstackL(int t)
  {
    int w=1;
      h:back();
        if(sidesensorsL[0]!=1 && sidesensorsR[0]!=1)
        {
          sensor_Values();
          goto h;  
          }
        sensor_Values(); 
      if(sidesensorsL[0]==1 && sidesensorsR[0]==1)
     { 
        rightturn();
        }
    
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
        delay((t-1)*100);
        rotate();
        
        }
        if(w!=t)
       { 
        w=w+1;
        goto k;
       
       }  
       sensor_Values();     
     while((sidesensorsL[0]!=1 && sidesensorsR[0]!=1)){
       sensor_Values();
        Forward();}
       
        if(sidesensorsL[0]==1 && sidesensorsR[0]==1)
        {
           rightturn();
        Stop();
         }
      
     }
     }
    
   ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////  
     
                                                                       //left jaaake leave kar
   
   void leftstackL(int t)
  {
        int w=1; 
        h:back();
        if(sidesensorsL[0]!=1 && sidesensorsR[0]!=1)
        {
          sensor_Values();
          goto h;  
          }
          sensor_Values();
        if(sidesensorsL[0]==1 && sidesensorsR[0]==1)
    { 
      leftturn();
        }
 
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
        delay((t-1)*100);
        rotate();
        
        }
        if(w!=t)
       { 
        w=w+1;
        goto k;
       
       }    
       sensor_Values();  
     while((sidesensorsL[0]!=1 && sidesensorsR[0]!=1)){
        sensor_Values();
        Forward();}
       
        if(sidesensorsL[0]==1 && sidesensorsR[0]==1){
         leftturn();
        Stop();
         }
      
     }
 }
 ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

                                                                   //left jaake pick kar
void leftstackP(int t)
  {
            int w=1;
        h:back();
        if(sidesensorsL[0]!=1 && sidesensorsR[0]!=1)
        {
          sensor_Values();
          goto h;  
          }
           sensor_Values();
        if(sidesensorsL[0]==1 && sidesensorsR[0]==1)
      { 
         leftturn();
         }
          k: sensor_Values(); 
          while((sidesensorsL[0]!=1 && sidesensorsR[0]!=1)){
        sensor_Values();
         Forward();}
           if(sidesensorsL[0]==1 && sidesensorsR[0]==1)
         { 
     
      Stop();
        if(w==t){
       pick();
        delay(4000);
         back();
        delay((t-1)*100);
        rotate();
      //  delay(1000);
        Stop();
        }
      else  if(w!=t)
       { 
        w=w+1;
        goto k;
       
       }      
          sensor_Values();
           while((sidesensorsL[0]!=1 && sidesensorsR[0]!=1)){
        sensor_Values();
        Forward();}
       
        if(sidesensorsL[0]==1 && sidesensorsR[0]==1){
         leftturn();
        Stop();
         }
      
     }
  }  

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  void mainstackP(int t)
  {
      int w=1;
          k: sensor_Values(); 
          while((sidesensorsL[0]!=1 && sidesensorsR[0]!=1)){
        sensor_Values();
         Forward();}
           if(sidesensorsL[0]==1 && sidesensorsR[0]==1)
         { 
     
      Stop();
                 if(w==t){
                  pick();
                     delay(4000);
                    back();
                    delay((t-1)*100);
                  Stop();
                     
                 }
                else if(w!=t)
                 { 
               w=w+1;
                goto k;
                }
         }
   }
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  void mainstackL(int t)
  {
      int w=1;
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
                  Stop();
                     
                 }
                else if(w!=t)
                 { 
               w=w+1;
                goto k;
                }
         }
   }


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 
  
  void rightturn()
  {
    
    
    digitalWrite(direction1motorA, HIGH);
    digitalWrite(direction2motorA, LOW);
    digitalWrite(direction2motorB, LOW);
    digitalWrite(direction1motorB, HIGH);
    analogWrite(enableA, 229);
    analogWrite(enableB,0);
   delay(1650);
    }
  
  void leftturn()
  {
    digitalWrite(direction1motorA, HIGH);
    digitalWrite(direction2motorA, LOW);
    digitalWrite(direction2motorB, LOW);
    digitalWrite(direction1motorB, HIGH);
    analogWrite(enableA, 0);
    analogWrite(enableB,225);
    delay(1650);
    } 
  
  void Forward(){
    digitalWrite(direction1motorA, HIGH);
    digitalWrite(direction2motorA, LOW);
    digitalWrite(direction1motorB, HIGH);
    digitalWrite(direction2motorB, LOW);
    analogWrite(enableA,200);
    analogWrite(enableB,225);
    //delay(3000);
}
  void back()
    {
  
    digitalWrite(direction2motorA, HIGH);
    digitalWrite(direction1motorA, LOW);
    digitalWrite(direction1motorB, LOW);
    digitalWrite(direction2motorB, HIGH); 
    analogWrite(enableA,200);
    analogWrite(enableB,225);
   
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
    analogWrite(enableA,230);
    analogWrite(enableB,255);
    //delay(300);
    
    } 
    void SlightRight(){
    digitalWrite(direction1motorA, HIGH);
    digitalWrite(direction2motorA, LOW);
    digitalWrite(direction1motorB, HIGH);
    digitalWrite(direction2motorB, LOW);
    analogWrite(enableB,175);
    analogWrite(enableA,200);
    //delay(100);
}

 void HardRight(){
    digitalWrite(direction1motorA, HIGH);
    digitalWrite(direction2motorA, LOW);
    digitalWrite(direction1motorB, HIGH);
    digitalWrite(direction2motorB, LOW);
    analogWrite(enableB,25);
    analogWrite(enableA,200);
    //delay(100);
 }

void SlightLeft(){
    digitalWrite(direction1motorA, HIGH);
    digitalWrite(direction2motorA, LOW);
    digitalWrite(direction1motorB, HIGH);
    digitalWrite(direction2motorB, LOW);
    analogWrite(enableB,225);
    analogWrite(enableA,150);
    //delay(100);
}
void HardLeft(){
    digitalWrite(direction1motorA, HIGH);
    digitalWrite(direction2motorA, LOW);
    digitalWrite(direction1motorB, HIGH);
    digitalWrite(direction2motorB, LOW);
    analogWrite(enableB,200);
    analogWrite(enableA,25);
    //delay(100);
}
void HardRightx(){
    digitalWrite(direction1motorA, HIGH);
    digitalWrite(direction2motorA, LOW);
    digitalWrite(direction1motorB, LOW);
    digitalWrite(direction2motorB, HIGH);
    analogWrite(enableB,175);
    analogWrite(enableA,200);
    delay(1150);
}
 void HardLeftx(){
    digitalWrite(direction1motorA, LOW);
    digitalWrite(direction2motorA, HIGH);
    digitalWrite(direction1motorB, HIGH);
    digitalWrite(direction2motorB, LOW);
    analogWrite(enableB,200);
    analogWrite(enableA,150);
    delay(1000);
 }

//////////////////////////////////////////////////////////////////////////////////////////////////
    
 void Follow(){ 
   position = qtrrc.readLine(sensorValues);
    SumLeft =(sensorValues[0] + sensorValues[1] + sensorValues[2]+sensorValues[3]);
  SumRight = (sensorValues[4] + sensorValues[5] + sensorValues[6]+sensorValues[7]);
  SumDifference = (SumLeft - SumRight);
 Serial.print(' ');
 Serial.print(SumDifference); 
  Serial.print('\t');
  Serial.print(SumLeft);
  Serial.print('\t');
  Serial.print(SumRight);
  Serial.print('\t');
  for(int i=0;i<8;i++){
    Serial.print(sensorValues[i]);
    Serial.print(' ');
  }
  
  if(abs(SumDifference) < 700){
    Forward();
    Serial.print("Forward");
  
  }
  else if((SumDifference > 700) && (SumDifference < 1500)){
    SlightRight();
    Serial.print("SlightRight");
  }
  else if(SumDifference >= 1500){
    HardRight();
    Serial.print("Hard Right");
  }
  
 else if((SumDifference < -700) && (SumDifference > -1500)){
    SlightLeft();
    Serial.print("SlightLeft");
  }
  else if((SumDifference) <= -1500){
    HardLeft();
    Serial.print("Hard Left");
  }
      
    
}

 ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

 
 void sensor_Values()
 {
   position = qtrrc.readLine(sensorValues);
  qtrrc1.read(sidesensorsL);
     qtrrc2.read(sidesensorsR);
for (int i=0;i<=7;i++)
 {
   if(sensorValues[i]<600)
    sensorValues[i]=1;
   else 
   sensorValues[i]=0;

  }
   if(sidesensorsL[0]<600)
    sidesensorsL[0]=1;
   else 
   sidesensorsL[0]=0;

    if(sidesensorsR[0]<600)
   sidesensorsR[0]=1;
   else 
  sidesensorsR[0]=0;
  
  }
 ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  
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
  
   digitalWrite(A14,HIGH);
  delay(1000);
  digitalWrite(A14,LOW);
return 'B';  
   } if((frequency_g-frequency_b)>15 &&(frequency_r)<190 && 20<frequency_r && frequency_b > frequency_r && frequency_g>frequency_b) 
{
Serial.println("this is red");
digitalWrite(A13,HIGH);
delay(1000);
digitalWrite(A13,LOW);
return 'R'; 
}
  if(40>abs(frequency_g-frequency_b)   && frequency_r<180  && 40>abs(frequency_g- frequency_r) && 80< frequency_r && 80< frequency_g && frequency_g<180&& 80< frequency_b && frequency_b<180)
{
  //Serial.println("this is green");
  digitalWrite(A11,HIGH);
delay(1000);
digitalWrite(A11,LOW);
return 'G';
  }
if((frequency_b-frequency_g)<50 && (frequency_g-frequency_r)<50 && frequency_g>frequency_r && frequency_g<frequency_b && frequency_r>10)
{
  

digitalWrite(A12,HIGH);
  delay(1000);
  digitalWrite(A12,LOW);
  return 'Y';
 }
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
