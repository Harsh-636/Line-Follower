int th;

void setup() 
{
  pinMode(A0, INPUT);
  pinMode(13, OUTPUT);
}

void loop()
{
  int a;
  a = analogRead(A0);
  Serial.println(a);
  delay(300);

  if(a > th)
    digitalWrite(13, LOW);
  else
    digitalWrite(13, HIGH);
}
