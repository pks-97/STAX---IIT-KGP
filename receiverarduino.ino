#include <Wire.h>
int i=0;
char c, colour[4];
void setup() {
  Wire.begin(8);                // join i2c bus with address #8
  Wire.onReceive(receiveEvent); // register event
  Serial.begin(9600);           // start serial for output
#define S0 4
#define S1 5
#define S2 6
#define S3 7
#define sensorOut 8

  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  pinMode(A0, OUTPUT);
  pinMode(A1, OUTPUT);
  pinMode(A2, OUTPUT);
  pinMode(A3, OUTPUT);
   pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(sensorOut, INPUT);
  
  // Setting frequency-scaling to 20%
  digitalWrite(S0,HIGH);
  digitalWrite(S1,LOW);
  
  //Serial.begin(9600);
}

void loop() {
  delay(100);
}
// function that executes whenever data is received from master
// this function is registered as an event, see setup()
void receiveEvent(int howMany) {
 while (1 < Wire.available()) { // loop through all but the last
  char t = Wire.read(); // receive byte as a character
  
   //digitalWrite(10,LOW);
   // Serial.print(c);         // print the character
  }
   c = Wire.read();    // receive byte as an integer
  Serial.println(c);         // print the integer
if (c=='D')
{
  
   digitalWrite(10,HIGH);
   digitalWrite(11,LOW);
 colour[i]=detect();
 i=i+1;
}

if(c=='P')
{
  pick();
 
  }

if(c=='L')
{
  leave();
}
}
/*     Arduino Color Sensing Tutorial
 *      
 *  by Dejan Nedelkovski, www.HowToMechatronics.com
 *  
 */
 void pick()
 {
  digitalWrite(A0,HIGH);
  digitalWrite(A1,LOW);
  delay(600); 
  digitalWrite(A0,LOW);
  delay(1000);
  digitalWrite(A2,HIGH);
  digitalWrite(A3,LOW);
delay(150);
  digitalWrite(A2,LOW);
//delay(2000);
 }

void leave()
{
  digitalWrite(A3,HIGH);
  digitalWrite(A2,LOW);
  delay(150);
  digitalWrite(A3,LOW);
delay(1000);
digitalWrite(A1,HIGH);
 digitalWrite(A0,LOW); 
  delay(600);
  digitalWrite(A1,LOW);
//delay(1000);
  
  }
char detect() 
{
 
  int frequency_r ,frequency_b,frequency_g;// Setting red filtered photodiodes to be read
  digitalWrite(S2,LOW);
  digitalWrite(S3,LOW);
  // Reading the output frequency
  frequency_r = pulseIn(sensorOut, LOW);
  // Printing the value on the serial monitor
  /*Serial.print("R= ");//printing name
  Serial.print(frequency);//printing RED color frequency
  Serial.print("  ");
  delay(100);*/
  // Setting Green filtered photodiodes to be read
  digitalWrite(S2,HIGH);
  digitalWrite(S3,HIGH);
  // Reading the output frequency
  frequency_g = pulseIn(sensorOut, LOW);
  // Printing the value on the serial monitor
  /*Serial.print("G= ");//printing name
  Serial.print(frequency);//printing RED color frequency
  Serial.print("  ");
  delay(100);*/
  // Setting Blue filtered photodiodes to be read
  digitalWrite(S2,LOW);
  digitalWrite(S3,HIGH);
  // Reading the output frequency
  frequency_b = pulseIn(sensorOut, LOW);
  // Printing the value on the serial monitor
 /* Serial.print("B= ");//printing name
  Serial.print(frequency);//printing RED color frequency
  Serial.println("  ");
  delay(100);*/ if(frequency_r-frequency_b>30 && 150<frequency_r < 200 && frequency_g > frequency_r && frequency_b < frequency_r )
    {
   //Serial.println("this is blue");

   digitalWrite(9,HIGH);
  delay(1000);
  digitalWrite(9,LOW);
return 'B';  
   } if((frequency_g-frequency_b)>15 &&(frequency_r)<190 && 20<frequency_r && frequency_b > frequency_r && frequency_g>frequency_b) 
{
Serial.println("this is red");
digitalWrite(10,HIGH);
digitalWrite(11,LOW);
delay(1000);
digitalWrite(10,LOW);
return 'R'; 
}
  if(40>abs(frequency_g-frequency_b)   && frequency_r<180  && 40>abs(frequency_g- frequency_r) && 80< frequency_r && 80< frequency_g && frequency_g<180&& 80< frequency_b && frequency_b<180)
{
  //Serial.println("this is green");
  digitalWrite(11,HIGH);
delay(1000);
digitalWrite(11,LOW);
return 'G';
  }
if((frequency_b-frequency_g)<50 && (frequency_g-frequency_r)<50 && frequency_g>frequency_r && frequency_g<frequency_b && frequency_r>10)
{
  //Serial.println("this is yellow");

  digitalWrite(12,HIGH);
  delay(1000);
  digitalWrite(12,LOW);
  return 'Y';
 }
}
