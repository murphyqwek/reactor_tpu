char inChar;
char inStr[255];
int i;
bool rec;

void setup() {
  Serial.begin(19200);
  Serial.println((char)21586);
  inStr[0]='0';
}

void loop() {
  if (Serial.available() > 0 ) 
    {
      //inStr = "";
      while (Serial.available() > 0)
    {
      inChar = Serial.read();
      inStr[i] += inChar;
      i++;
    }
    inStr[i] += '_';
    i++;
  }
  if (inStr[0] != '0') 
  {
    Serial.println((int)inStr[0]);
    Serial.println((int)inStr[1]);
    Serial.println(inStr);
  }
  delay(1000);
}
