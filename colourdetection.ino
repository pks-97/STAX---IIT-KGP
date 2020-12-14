/*     Arduino Color Sensing Tutorial
 *      
 *  by Dejan Nedelkovski, www.HowToMechatronics.com
 *  
 */
 
#define S0 4
#define S1 5
#define S2 6
#define S3 7
#define sensorOut 8
int frequency_r = 0;
int frequency_g = 0;
int frequency_b = 0;


void setup() {
  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  pinMode(sensorOut, INPUT);
  pinMode(10,OUTPUT);
  pinMode(9,OUTPUT);
 pinMode(3,OUTPUT);
 pinMode(2,OUTPUT);
 pinMode(12,OUTPUT);
 pinMode(11,OUTPUT);
pinMode(13,OUTPUT);
  // Setting frequency-scaling to 20%
  digitalWrite(S0,HIGH);
  digitalWrite(S1,LOW);
  digitalWrite(13,LOW);
  Serial.begin(9600);
}
void loop() {
  // Setting red filtered photodiodes to be read
  digitalWrite(S2,LOW);
  digitalWrite(S3,LOW);
  // Reading the output frequency
  frequency_r = pulseIn(sensorOut, LOW);
  //Remaping the value of the frequency to the RGB Model of 0 to 255
  //frequency_r = map(frequency_r, 72,25,255,0);
  // Printing the value on the serial monitor
 Serial.print("R= ");//printing name
  Serial.print(frequency_r);//printing RED color frequency
  Serial.print("  ");
  delay(100);
  // Setting Green filtered photodiodes to be read
  digitalWrite(S2,HIGH);
  digitalWrite(S3,HIGH);
  // Reading the output frequency
  frequency_g = pulseIn(sensorOut, LOW);
  //Remaping the value of the frequency to the RGB Model of 0 to 255
  //frequency_g = map(frequency_g, 90,30,255,0);
  // Printing the value on the serial monitor
  Serial.print("G= ");//printing name
  Serial.print(frequency_g);//printing RED color frequency
  Serial.print("  ");
  delay(100);
  // Setting Blue filtered photodiodes to be read
  digitalWrite(S2,LOW);
  digitalWrite(S3,HIGH);
  // Reading the output frequency
  frequency_b = pulseIn(sensorOut, LOW);
  //Remaping the value of the frequency to the RGB Model of 0 to 255
 // frequency_b = map(frequency_b, 70,25,255,0);
  // Printing the value on the serial monitor
  Serial.print("B= ");//printing name
  Serial.print(frequency_b);//printing RED color frequency
  Serial.println("  ");
  delay(100);
  if(frequency_r-frequency_b>40 && 150<frequency_r < 200 && frequency_g > frequency_r && frequency_b < frequency_r )
    {
   Serial.println("this is blue");
   digitalWrite(13,HIGH);
   //digitalWrite(11,LOW);
   }
   else 
{  
  digitalWrite(13,LOW);
}
  if((frequency_g-frequency_b)>15 &&(frequency_r)<120 && 20<frequency_r && frequency_b > frequency_r && frequency_g>frequency_b) 
{
Serial.println("this is red");
digitalWrite(3,HIGH);
digitalWrite(2,LOW); 
}
  else 
  {digitalWrite(3,LOW);
  }

if(40>(frequency_g-frequency_b)  &&(frequency_r-frequency_b)<50 && frequency_r<230  && frequency_g >= frequency_r  && frequency_r >frequency_b)
{
  Serial.println("this is green");
  digitalWrite(10,HIGH);
  digitalWrite(9,LOW);
  }
else
{digitalWrite(10,LOW);
}
if((frequency_b-frequency_g)<20 && (frequency_g-frequency_r)<20 && frequency_g>frequency_r && frequency_g<frequency_b && frequency_r <80 && frequency_r>10)
{
  Serial.println("this is yellow");

  digitalWrite(12,HIGH);
  digitalWrite(11,LOW);
 }
else 
digitalWrite(12,LOW);

}





