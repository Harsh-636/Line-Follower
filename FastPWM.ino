
void setup() 
{
  Serial.begin(9600);
  Serial.println(TCCR2A, BIN);        //display default values of registers
  Serial.println(TCCR2B, BIN);a
  
  pinMode(3, OUTPUT);                 //set pins 3 & 11 to output mode
  pinMode(11, OUTPUT);

  TCCR2A = 0;                         //reset the registers
  TCCR2B = 0;

  TCCR2A = 0b01010011;                //WGM00 & 01 are both 1, thus FastPWM
  TCCR2B = 0b00001001;                //WGM02 is 1

  OCR2A = 40;                         
  OCR2B = 50;
}

void loop()
{
  // put your main code here, to run repeatedly:
  //this higher frequency of pwm on pin 3 can be used for various applications.
}
