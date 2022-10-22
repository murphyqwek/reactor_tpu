#define STEPV 32
#define DIRV 30
#define STEPH 13
#define DIRH 12 
#define START A1 
#define SPDV A8

const int StartPin = 8; 
const int LowMikPin = 42; 
const int HighMikPin = 43; 
const int down = 48; 
const int up = 49; 
const int left = 24; 
const int right = 25;  

int buttonStart = 0; 
int buttonHigh = 0; 
int buttonLow = 0; 
int buttonUp = 0; 
int buttonDown = 0; 
int buttonLeft = 0; 
int buttonRight = 0;
int SpeedV = 0;
int rast=0; 
int cos1=0;
int all=0; 
int k = 0;
bool brk = 0;
int timer = 100;
uint32_t  tmr=millis();
float st=analogRead(START)*0.00488758553; 
float aver_st =0;
void setup() { 

  Serial.begin(115200); 
  pinMode(STEPV,OUTPUT);
  pinMode(DIRV,OUTPUT);
  pinMode(28,OUTPUT);
  digitalWrite(28,HIGH);
  pinMode(STEPH,OUTPUT);
  pinMode(DIRH,OUTPUT);
  pinMode(11,OUTPUT);
  pinMode(14,OUTPUT); 
  digitalWrite(6,LOW); 
  pinMode(StartPin, INPUT_PULLUP); 
  pinMode(LowMikPin, INPUT_PULLUP); 
  pinMode(HighMikPin, INPUT_PULLUP); 
  pinMode(up, INPUT_PULLUP); 
  pinMode(down, INPUT_PULLUP);
  pinMode(left, INPUT_PULLUP); 
  pinMode(right, INPUT_PULLUP);
} 

 void stepperV(unsigned int Step, int SpeedV, boolean Set)
{
  digitalWrite(DIRV, Set);
  for (int i=0; i<Step; i++){
    digitalWrite(STEPV,HIGH);
    delayMicroseconds(SpeedV);
    digitalWrite(STEPV,LOW);
    delayMicroseconds(SpeedV);
  }
  
}


void loop()
{ 
  brk = 0;
  buttonStart = digitalRead(StartPin); 
  buttonHigh = digitalRead(HighMikPin); 
  buttonLow = digitalRead(LowMikPin); 
  buttonUp = digitalRead(up); 
  buttonDown= digitalRead(down); 
  buttonHigh = digitalRead(HighMikPin);
  st=analogRead(START)*0.00488758553;
  //Serial.print("start = ");
  
  //if (timer < millis()-tmr)
   //{
    tmr=millis();
    //Serial.print("time =");
    //Serial.println(millis());
    //Serial.println(buttonStart);
    Serial.print("up = ");
    Serial.println(buttonUp);
    Serial.print("down = ");
    Serial.println(buttonDown);
    //stepperV(2,600,HIGH);
    //stepperV(2,600,LOW);
    aver_st = aver_st + 0.0243*millis()/timer;
    cos1++;
    /*if (cos1 == 5)
    {  
      aver_st = aver_st/5;
      Serial.print("aver_st = ");
      Serial.println(aver_st);
      cos1 = 0;
      aver_st =0;
    }
    Serial.print("time = ");
    Serial.print(millis()/1000);
    Serial.print("   tok = ");
    Serial.println(st);*/

   //}
}
