float kp = 0.099; //kp == 0.038,0.089&0.00058,0.086&0.00064 , 0.2 & 0.002 (0.0398)(0.098,0.000098)
float ki = 0.001; // ki = 0.001
float kd = 0.90;  //0.0033   (kp =0.098 and kd = 0.00125)
int error;
int proportional,derivative,integral = 0,last_proportional = 0;
unsigned int pos;

int k = 0;
int ir[] = {A2, A3, 10, 8, 11, A5, A0, A1};
int ir_sensor[8] , a[8];
int val , value;
int base_speed = 200;
int mot[] = {3,5,6,9};
int ledArray[] = {7,2,4};
#include<NewPing.h>
int trigPin = 13;
int echoPin = 12;
int maxdistance = 400;
NewPing sonar(trigPin, echoPin, maxdistance);

int readline();
void set_motors();
void sensor_reading();
void Stop();
void checkColour();
void uturn2();
void measure_distance();

int avgread, thres = 70;
void setup()
{
  Serial.begin(115200);
  for(int i = 0 ; i < 8 ; i++)
  {
    pinMode(ir[i],INPUT);
  }
  for(int i = 0 ; i < 4 ; i++)
  {
    pinMode(mot[i],OUTPUT);
  }
  pinMode(7,OUTPUT);
  pinMode(4,OUTPUT);
  pinMode(2,OUTPUT);
  
  for(int i = 0 ; i < 3 ; i++)
    digitalWrite(ledArray[i], HIGH);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

void loop()
{   
    delay(25);
    Serial.print("Distance : ");
    int x = sonar.ping_cm();
    Serial.print(x);
    Serial.println();
     
    for(int i = 0 ; i < 3 ; i++)
      digitalWrite(ledArray[i], LOW);
    if(x < 9)
    {
    if(x > 0)
    {
      Stop();
      checkColour();
      uturn2();
    }
    }
    
//    int x = analogRead(A4);
//    if(x > thres)
//    {
//      Stop();
//      checkColour();
//      uturn2();
//    }
      
    sensor_reading();
    pos = readline();
    value = a[0]*1 +  a[1]*2 + a[2]*4 +  a[3]*8 +  a[4]*16 +  a[5]*32 + a[6]*64 + a[7]*128 ;
    proportional = pos - 2500;
    derivative = proportional - last_proportional;
    integral = integral + proportional;
    last_proportional = proportional;

    error = kp*proportional + ki*integral + kd*derivative;
   Serial.println(error);
    if(error > base_speed)
       error = base_speed;
    else if(error < -base_speed)
       error = (-1)*base_speed;

    if(error < 0)     //TILT LEFT
        set_motors(error + base_speed , base_speed);  
    else
        set_motors(base_speed,base_speed - error);
    if((value == 255) || (a[0] && a[7] == 1) || (a[2] && a[3] && a[4] && a[5] == 1))
    {
        do
     {
       sensor_reading();
       forward();
       k++;
       Serial.print("Checking for T, k = ");
       Serial.println(k);
         if(k > 700)
         {
          for(int i = 0; i < 4; i++)
          {
            analogWrite(mot[i], 0);
          }
//          digitalWrite(ledArray[0], LOW);
//          delay(2000);
//          digitalWrite(ledArray[0], HIGH);
//          digitalWrite(ledArray[1], LOW);
//          delay(2000);
             int order[3] = {1, 0, 2};
//          digitalWrite(ledArray[1], HIGH);
//          digitalWrite(ledArray[2], LOW);
//          digitalWrite(ledArray[1], LOW);
//          delay(2000);
//          digitalWrite(ledArray[2], HIGH);
//          digitalWrite(ledArray[1], HIGH);
            for(int i = 0; i < 3; i++)
            {
              digitalWrite(ledArray[i], HIGH);
            }
            for(int i = 0 ; i < 3 ; i++)
            {
              if(order[i] == 0)
          {
            digitalWrite(ledArray[0], LOW);
            delay(2000);
            digitalWrite(ledArray[0], HIGH);
          }
             if(order[i] == 1)
          {
            digitalWrite(ledArray[1], LOW);
            delay(2000);
            digitalWrite(ledArray[1], HIGH);
          }
             if(order[i] == 2)
          {
            digitalWrite(ledArray[0], LOW);
            digitalWrite(ledArray[1], LOW);
            delay(2000);
            digitalWrite(ledArray[0], HIGH);
            digitalWrite(ledArray[1], HIGH);
          }
            }
            delay(4000);
         }
     }
     while(a[0] == 1 || a[7] == 1);
     Serial.println("a[0] && a[7],f");
    if(a[3] == 1 || a[4] == 1)
    {
       left_node();
       Serial.println("left node of +");
    }
    else if(a[2] == 0 || a[3] == 0)
    {
       left();
//        right();
       Serial.println("left of T");
    }
     
    }
    else if(a[0] == 1 && a[7] == 0 )//delay needs to be used for forward
    {
     do
     {
       sensor_reading();
       forward();
       Serial.println("a[0]");
     }
     while(a[0] != 0);
     Serial.println("a[0],f");
    if(a[3] == 1 || a[4] == 1)
    {
       left_node();
       Serial.println("left node");
    }
    else
    {
       left();
       Serial.println("left");
    }
    }
    else if((a[7] == 1) && (a[0] == 0))
    {
      sensor_reading();
//      if ( a[7]==1 && a[0] == 0 && a[1] == 0 )
//      {
     do
     {
      Serial.println("a[7]");
      sensor_reading();
      forward();
     
     }
     while(a[7] != 0);
     if(a[2] || a[3] == 0)
       right();
       Serial.println("right");
    }
    if((value == 0) || (a[0] && a[1] && a[2] && a[3] && a[4] && a[5] && a[6] && a[7] == 0))
     uturn();
     
}

void sensor_reading()
{
  for(int i = 0 ; i < 8 ; i++)
  {
    a[i] = digitalRead(ir[i]);
    Serial.print(a[i]);
    Serial.print(" ");
  }
  Serial.println();
}


void set_motors(int a , int b)
{
  analogWrite(mot[0] , a);
  analogWrite(mot[1] , 0);
  analogWrite(mot[2] , b);
  analogWrite(mot[3] , 0);
}

int readline()
{
  sensor_reading();
  val = (a[1]*0 + a[2]*1000 + a[3]*2000+ a[4]*3000 + a[5]*4000 + a[6]*5000)/(a[1] + a[2] + a[3] + a[4] + a[5] + a[6]);
  return val;  
}

void left()
{
  while(a[2] != 1 )
  {
    sensor_reading();
    analogWrite(mot[0] , 0);
    analogWrite(mot[1] , 60);
    analogWrite(mot[2] , 180);
    analogWrite(mot[3] , 0);
  }
}

void right()
{
  while(a[5] != 1)
  {
    sensor_reading();
    analogWrite(mot[0] , 180);
    analogWrite(mot[1] , 0);
    analogWrite(mot[2] , 0);
    analogWrite(mot[3] , 60);
  }
}

void left_node()
{
  while(a[3] != 0)
  {
    sensor_reading();
    analogWrite(mot[0] , 0);
//     analogWrite(mot[1] , 0);
//    analogWrite(mot[2] , 0);
    analogWrite(mot[1] , 60);
    analogWrite(mot[2] , 180);
    analogWrite(mot[3] , 0);
  }

  while(a[3] != 1)
  {
    sensor_reading();
    analogWrite(mot[0] , 0);
    analogWrite(mot[1] , 60);
    analogWrite(mot[2] , 180);
    analogWrite(mot[3] , 0);
  }
 
}

void uturn()
{
  while(a[3] != 1)
  {
    sensor_reading();
    analogWrite(mot[0] , 90);
    analogWrite(mot[1] , 0);
    analogWrite(mot[2] , 0);
    analogWrite(mot[3] , 90);
  }
//  forward();
}

void uturn2()
{
//  while(a[0] != 0)
//  {
//    sensor_reading();
    analogWrite(mot[1] , 120);
    analogWrite(mot[2] , 0);
    analogWrite(mot[3] , 120);

    delay(500);
    
    analogWrite(mot[0] , 0);
    analogWrite(mot[1] , 90);
    analogWrite(mot[2] , 90);
    analogWrite(mot[3] , 0);
    delay(1000);
}
void forward()
{
    sensor_reading();
    analogWrite(mot[0] , 120);
    analogWrite(mot[1] , 0);
    analogWrite(mot[2] , 120);
    analogWrite(mot[3] , 0);
}

void Stop()
{
    sensor_reading();
    analogWrite(mot[0] , 0);
    analogWrite(mot[1] , 0);
    analogWrite(mot[2] , 0);
    analogWrite(mot[3] , 0);
    delay(2000);
    k = 0;
}

void checkColour()
{ 
  for(int i = 0 ; i < 3 ; i++)
  {
    digitalWrite(ledArray[i], HIGH);
  }
  for(int i = 0 ; i < 3 ; i++)
  {
    digitalWrite(ledArray[i],LOW);
    delay(300);
    digitalWrite(ledArray[i],HIGH);
    delay(50);
  }
}

//void measure_distance()
//{
//  digitalWrite(trigPin, LOW);
//delayMicroseconds(2);
//// Sets the trigPin on HIGH state for 10 micro seconds
//digitalWrite(trigPin, HIGH);
//delayMicroseconds(10);
//digitalWrite(trigPin, LOW);
//// Reads the echoPin, returns the sound wave travel time in microseconds
//duration = pulseIn(echoPin, HIGH);
//// Calculating the distance
//distance= duration*0.034/2;
//// Prints the distance on the Serial Monitor
//Serial.print("Distance: ");
//Serial.println(distance);
//}
