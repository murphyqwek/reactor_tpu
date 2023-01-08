//Для дебага
//#define DEBUG_ENABLE

#define STEPV 32
#define DIRV 30
#define ENV 28
#define SPDV A8

#define STEPH 27
#define DIRH 25
#define ENH 23
#define SPDH A9

#define STEPD 26
#define DIRD 24
#define EN_D 22
#define SPDD A10

#define START A1
#define read_st() analogRead(START) * 0.00488758553

//Pins
#define StartPin 9
#define LowMikPin 42
#define HighMikPin 43
#define down 46
#define up 47
#define left 44
#define right 45

#define horis 52
#define vert 53
#define switcher 29
//#define forward 26
//#define back 27

char inChar;
String inStr;
long inInt;
int buttonStart = 0;
int buttonHigh = 0;
int buttonLow = 0;
int buttonUp = 0;
int buttonDown = 0;
int buttonLeft = 0;
int buttonRight = 0;
int Switcher = 0;
int SpeedV = 0;
int SpeedH = 0;
long sintes_time = 5;
int rast = 0;
int cos1 = 0;
int cos2 = 0;
int all = 0;
int k = 0;
int l = 0;
int h = 0;
int rasst = 0;
int currentReal = 200;
int work_mode = 0;
int spddwn = 0;
int discr = 0;
bool brk = 0;
float st = read_st();
float contact = 2.66;
float AnodUp = 2.71;
float AnodDown = 2.68;
float aver_st = 0;
float cur_CC = 2.23;
uint32_t tmr = millis();
uint32_t t = micros();


void setup() {
  Serial.begin(115200);
  pinMode(STEPV, OUTPUT);
  pinMode(DIRV, OUTPUT);
  pinMode(ENV, OUTPUT);

  pinMode(STEPH, OUTPUT);
  pinMode(DIRH, OUTPUT);
  pinMode(ENH, OUTPUT);

  pinMode(STEPD, OUTPUT);
  pinMode(DIRD, OUTPUT);
  pinMode(EN_D, OUTPUT);
  pinMode(14, OUTPUT);

  digitalWrite(6, LOW);
  pinMode(horis, OUTPUT);
  pinMode(vert, OUTPUT);

  pinMode(StartPin, INPUT_PULLUP);
  pinMode(LowMikPin, INPUT_PULLUP);
  pinMode(HighMikPin, INPUT_PULLUP);
  pinMode(up, INPUT_PULLUP);
  pinMode(down, INPUT_PULLUP);
  pinMode(left, INPUT_PULLUP);
  pinMode(right, INPUT_PULLUP);
  pinMode(switcher, INPUT_PULLUP);

  sintes_time = 5;    //время синтеза в секундах
  currentReal = 200;  //ток на установке
  rast = 0;
  discr = 3;
  rasst = 50;
  spddwn = 1000;
  // contact = 2.65; //ток контакта, для 220А = 2.75; для 150А = 2.65; для 100А = 2.55; для 50А = 2.4;
  // AnodUp = 2.68; //отвод электрода для 220А = 2.8; для 150А = 2.68; для 100А = 2.6; для 50А = 2.41;
  // AnodDown = 2.55; //подача электрода для 220А = 2.6; для 150А = 2.55; для 100А = 2.45; для 50А = 2.35(нужно больше опытов!!!);
  switch (currentReal) {
    case 50:
      //2.4 2.41 2.35 woilok

      contact = 2.38;
      AnodUp = 2.41;
      AnodDown = 2.37;
      break;
    case 100:
      //2.55 2.6 2.45 woilok
      contact = 2.55;
      AnodUp = 2.6;
      AnodDown = 2.5;
      break;
    case 150:
      //2.5 2.55 2.52 woilok
      //2.65 2.68 2.6
      contact = 2.6;
      AnodUp = 2.58;
      AnodDown = 2.55;
      break;
    case 200:
      //2.6 2.7 2.65 woilok
      //2.66 2.71 2.68 new
      //2.7 2.75 2.72
      contact = 2.66;
      AnodUp = 2.71;
      AnodDown = 2.68;
      break;
  }
}

//Функция шагового двигателя
void stepperV(unsigned int Step, int SpeedV, boolean Set) {
  static bool de_lay = 1;
  digitalWrite(ENV, HIGH);
  //delayMicroseconds(500);
  digitalWrite(DIRV, Set);
  for (int i = 0; i < Step; i++) {
    if (micros() - t >= SpeedV) {
      t = micros();
      digitalWrite(STEPV, de_lay);
      de_lay = !de_lay;
      //delayMicroseconds(SpeedV);
      //digitalWrite(STEPV,LOW);
      //delayMicroseconds(SpeedV);
    }
  }
  digitalWrite(STEPV, LOW);
  //digitalWrite(ENV,LOW);
}

void stepperH(unsigned int Step, int SpeedH, boolean Set) {
  static bool de_lay = 1;
  digitalWrite(ENH, HIGH);
  //delayMicroseconds(500);
  digitalWrite(DIRH, Set);
  for (int i = 0; i < Step; i++) {
    if (micros() - t >= SpeedH) {
      t = micros();
      digitalWrite(STEPH, de_lay);
      de_lay = !de_lay;
      //delayMicroseconds(SpeedV);
      //digitalWrite(STEPV,LOW);
      //delayMicroseconds(SpeedV);
    }
  }
  digitalWrite(STEPH, LOW);
  //digitalWrite(ENV,LOW);
}

void stepperD(unsigned int Step, int SpeedH, boolean Set) {
  static bool de_lay = 1;
  digitalWrite(EN_D, HIGH);
  //delayMicroseconds(500);
  digitalWrite(DIRD, Set);
  for (int i = 0; i < Step; i++) {
    if (micros() - t >= SpeedH) {
      t = micros();
      digitalWrite(STEPD, de_lay);
      de_lay = !de_lay;
      //delayMicroseconds(SpeedV);
      //digitalWrite(STEPV,LOW);
      //delayMicroseconds(SpeedV);
    }
  }
  digitalWrite(STEPD, LOW);
  //digitalWrite(ENV,LOW);
}



//Отвод до контрольной точки (верхний микрик)
void step_KP()
{
  cos1 = 0;
  while (cos1 < 3)
  {
    buttonHigh = digitalRead(HighMikPin);
    if (buttonHigh == LOW)
      cos1 += 1;
    stepperV(1, 300, HIGH);
  }
  cos1 = 0;
}

//Функция инициализации дуги
void duga_init(int time_init = 1000) {
  while (cos1 < 3) {
    buttonLow = digitalRead(LowMikPin);
    stepperV(1, spddwn, LOW);
    rast = rast + 1;
    if (st < contact && cos1 > 0) {
      cos1 = 0;
    }
    if (st > cur_CC + 0.14) {
      //spddwn = 10000;
    }
    if (st > contact) {
      cos1++;
    }

    buttonUp = digitalRead(up);
    if (Serial.available() > 0) inChar = Serial.read();

    if (buttonUp == 0) cos2 += 1;
    if (buttonUp == 1 && cos2 > 0) cos2 = 0;

    if (cos2 == 5 || inChar == 'd') {
      brk = 1;
      inChar = 'a';
      Serial.println(buttonUp);
      Serial.println(inChar);
      Serial.println("end");
      break;
    }
    st = read_st();


#ifdef DEBUG_ENABLE
    Serial.println(buttonUp);
#endif

    delay(1);
  }
  Serial.println("Contact");
  delay(time_init);         //выдержка контакта в мс
  stepperV(50, 600, HIGH);  //где 11 число шагов, 1 шаг=0,44
}

void syntes(long sint_time = 1000)
{
  tmr = millis();
  l = 0;
  h = 0;
  cos1 = 0;
  while (millis() - tmr < sint_time)
  {
    st = read_st();
    aver_st = aver_st + st;
    cos1++;
    Serial.print("time_left = ");
    Serial.print((millis() - tmr));
    Serial.print("    tok = ");
    Serial.println(st);
    if (cos1 == discr) {
      aver_st = aver_st / discr;
      Serial.print("    aver_tok = ");
      Serial.print(aver_st);
      if (aver_st < AnodDown)  //подача электрода
      {
        stepperV(1, 600, LOW);
        l++;
        Serial.println("     step low");
      } else if (aver_st > AnodUp)  //отвод электрода
      {
        stepperV(1, 600, HIGH);
        h++;
        Serial.println("     step high");
      }
      cos1 = 0;
      aver_st = 0;
    }

    //delay(10);
    if (Serial.available() > 0)
      inChar = Serial.read();
    if (st < 1 || inChar == 'd') {
      inChar = 'a';
      break;
    }
    buttonStart = digitalRead(StartPin);
    if (buttonStart == 0) {
      buttonStart = digitalRead(StartPin);
      buttonUp = digitalRead(up);
      if (buttonStart == 0 && buttonUp == 0)
      {
        brk = 1;
        break;
      }
    }
  }
#ifdef DEBUG_ENABLE
  Serial.println("Sintes finish");
  Serial.print("high =");
  Serial.print(h);
  Serial.print("     low =");
  Serial.println(l);
#endif
  h = 0;
  l = 0;
}

void cut_duga()
{
  cos2 = 0;
  while (cos2 != 5)
  {
    st = read_st();
    buttonHigh = digitalRead(HighMikPin);
    if (buttonHigh == 0 || st < cur_CC + 0.1) cos2 += 1;
    if (buttonHigh == 1  && cos2 > 0 && st > cur_CC + 0.3) cos2 = 0;
    stepperV(1, 350, HIGH);
  }
}

bool impulse_mode(int n = 1) {
  duga_init();
  if (check_brk() == true) return 1;
  syntes();
  if (check_brk() == true) return 1;
  for (int i = 0; i < n; i++) {
    cut_duga();
    duga_init(100);
    if (check_brk() == true) return 1;
    syntes();
    if (check_brk() == true) return 1;

  }
  return 0;
}

bool duga_mode()
{
  duga_init();
#ifdef DEBUG_ENABLE
  Serial.println("Sintes start");
#endif
  if (check_brk() == true) return 1;
  syntes(sintes_time);
  if (check_brk() == true) return 1;
  return 0;
}


bool check_brk() {
  buttonUp = digitalRead(up);
  if (buttonUp == 0 || brk == 1) {
    Serial.println("end");
    return 1;
  }
  return 0;
}

void check_switch() {
  Switcher = digitalRead(switcher);
  if (Switcher == 1)  {
    digitalWrite(vert, LOW);
    digitalWrite(horis, HIGH);
  }
  else
  {
    digitalWrite(vert, HIGH);
    digitalWrite(horis, LOW);
  }
}



void loop() {
  brk = 0;
  buttonStart = digitalRead(StartPin);
  buttonHigh = digitalRead(HighMikPin);
  buttonLow = digitalRead(LowMikPin);
  st = read_st();
  while (st < 2)
  {
    st = read_st();
    
#ifdef DEBUG_ENABLE
    Serial.println("end");
#endif

    delay(500);
  }

  Serial.println(st);
  cur_CC = st;
  cos1 = 0;
  cos2 = 0;
  spddwn = 1000;

#ifdef DEBUG_ENABLE
  Serial.println(buttonStart + buttonHigh + buttonLow);
  Serial.println("Hand on");
#endif

  sintes_time = sintes_time * 1000 - 1000;

  while (inChar != 'e' && buttonStart == 1)
  {
    if (Serial.available() > 0) inChar = Serial.read();

#ifdef DEBUG_ENABLE
    Serial.print(inChar);
#endif

    if (Serial.available() > 0 && inChar != 'e') {
      //inChar = Serial.read().toInt();
      inChar = Serial.read();

#ifdef DEBUG_ENABLE
      Serial.println("мы отправили");
#endif

      while (inChar != 'n')
      {
        if (Serial.available() > 0) {
#ifdef DEBUG_ENABLE
          Serial.println(inChar);
#endif

          inStr += inChar;
          inChar = Serial.read();

#ifdef DEBUG_ENABLE
          Serial.println(inStr);
#endif
        }
      }

      inInt = inStr.toInt();
      inStr += inChar;

      //Отправляем на комп время синтеза
      Serial.println("Time");
      sintes_time = inInt * 1000;
      Serial.println(sintes_time);

      if (Serial.available() > 0) {
        inChar = Serial.read();
        inStr = inChar;

        currentReal = inStr.toInt();
        inChar = Serial.read();

        inStr = inChar;
        work_mode = inStr.toInt();

        //Отправляем на компьютер напряжение и режим синтеза
        Serial.println("Current");
        Serial.println(currentReal);
        Serial.println("Sintesys mode");
        Serial.println(work_mode);

        //Что-то связанное с режимом синтеза и напряжением TODO: переместить в отдельную функцию
        switch (currentReal) {
          case 0:
            switch (work_mode) {
              case 0:
                contact = cur_CC + 0.15;  //2.23
                AnodUp = cur_CC + 0.18;
                AnodDown = cur_CC + 0.14;
                break;
              case 1:
                contact = cur_CC + 0.17;
                AnodUp = cur_CC + 0.18;
                AnodDown = cur_CC + 0.12;
                break;
            }
            break;
          case 1:
            switch (work_mode) {
              case 0:
                contact = cur_CC + 0.32;
                AnodUp = cur_CC + 0.37;
                AnodDown = cur_CC + 0.27;
                break;
              case 1:
                contact = cur_CC + 0.32;
                AnodUp = cur_CC + 0.37;
                AnodDown = cur_CC + 0.22;
                break;
            }
            break;
          case 2:
            switch (work_mode) {
              case 0:
                contact = cur_CC + 0.37;  //2.23
                AnodUp = cur_CC + 0.35;
                AnodDown = cur_CC + 0.32;
                break;
              case 1:
                contact = cur_CC + 0.27;
                AnodUp = cur_CC + 0.32;
                AnodDown = cur_CC + 0.29;
                break;
            }
            break;
          case 3:
            switch (work_mode) {
              case 0:
                contact = cur_CC + 0.45;
                AnodUp = cur_CC + 0.45;
                AnodDown = cur_CC + 0.42;
                break;
              case 1:
                contact = cur_CC + 0.37;
                AnodUp = cur_CC + 0.47;
                AnodDown = cur_CC + 0.42;
                break;
            }
            break;
        }

        //Отправляем параметры напряжений
        Serial.println("Param");
        Serial.println(contact);
        Serial.println(AnodUp);
        Serial.println(AnodDown);
      }
    }

    /*if (Serial.available() > 0 && inChar != 'e')
      {
      //inChar = Serial.read().toInt();
      inChar = Serial.read();
      //Serial.println("мы отправили");
      while (inChar != '\n')
      {
        if (Serial.available() > 0)
        {
          //Serial.println(inChar);
          inStr += inChar;
          inChar = Serial.read();
          //Serial.println(inStr);
        }
      }
      inInt = inStr.toInt();
      inStr += inChar;
      Serial.println("мы получили");
      sintes_time = inInt*1000;
      Serial.println(inStr);
      }*/

    //Считываем значения с кнопок и скорость?
    SpeedV = map(analogRead(SPDV), 0, 1023, 250, 5000);
    SpeedH = map(analogRead(SPDH), 0, 1023, 250, 5000);
    buttonStart = digitalRead(StartPin);
    buttonUp = digitalRead(up);
    buttonDown = digitalRead(down);
    buttonLeft = digitalRead(left);
    buttonRight = digitalRead(right);
    buttonHigh = digitalRead(HighMikPin);
    check_switch();
    k = 0;
    //ручное управление
    while (buttonUp == 0 && buttonDown == 1 && buttonHigh == 1 && Switcher == 1) {
      if (k == 0) {
        stepperV(4, SpeedV, HIGH);
        SpeedV = map(analogRead(SPDV), 0, 1023, 250, 5000);
      }
      buttonUp = digitalRead(up);
      buttonDown = digitalRead(down);
      buttonHigh = digitalRead(HighMikPin);
      k = k + 1;
      while (k == 300 && buttonUp == 0 && buttonDown == 1 && buttonHigh == 1 && Switcher == 1) {
        stepperV(1, SpeedV, HIGH);
        SpeedV = map(analogRead(SPDV), 0, 1023, 250, 5000);
        buttonUp = digitalRead(up);
        buttonDown = digitalRead(down);
        buttonHigh = digitalRead(HighMikPin);
      }
      delay(1);
    }
    while (buttonUp == 1 && buttonDown == 0 && Switcher == 1) {
      if (k == 0) {
        stepperV(4, SpeedV, LOW);
        SpeedV = map(analogRead(SPDV), 0, 1023, 250, 5000);
      }
      buttonUp = digitalRead(up);
      buttonDown = digitalRead(down);
      k = k + 1;
      while (k == 300 && buttonUp == 1 && buttonDown == 0 && Switcher == 1) {
        stepperV(1, SpeedV, LOW);
        SpeedV = map(analogRead(SPDV), 0, 1023, 250, 5000);
        buttonUp = digitalRead(up);
        buttonDown = digitalRead(down);
      }
      delay(1);
    }

    while (buttonLeft == 0 && buttonRight == 1 && Switcher == 0) {
      if (k == 0) {
        stepperH(4, SpeedH, HIGH);
        SpeedH = map(analogRead(SPDH), 0, 1023, 250, 5000);
      }
      buttonLeft = digitalRead(left);
      buttonRight = digitalRead(right);
      k = k + 1;
      while (k == 300 && buttonLeft == 0 && buttonRight == 1 && Switcher == 0) {
        stepperH(1, SpeedH, HIGH);
        SpeedH = map(analogRead(SPDH), 0, 1023, 250, 5000);
        buttonLeft = digitalRead(left);
        buttonRight = digitalRead(right);
      }
      delay(1);
    }
    while (buttonLeft == 1 && buttonRight == 0 && Switcher == 0) {
      if (k == 0) {
        stepperH(4, SpeedH, LOW);
        SpeedH = map(analogRead(SPDH), 0, 1023, 250, 5000);
      }
      buttonLeft = digitalRead(left);
      buttonRight = digitalRead(right);
      k = k + 1;
      while (k == 300 && buttonLeft == 1 && buttonRight == 0 && Switcher == 0) {
        stepperH(1, SpeedH, LOW);
        SpeedH = map(analogRead(SPDH), 0, 1023, 250, 5000);
        buttonLeft = digitalRead(left);
        buttonRight = digitalRead(right);
      }
      delay(1);
    }

    while (buttonUp == 0 && buttonDown == 1 && Switcher == 0) {
      if (k == 0) {
        stepperD(4, SpeedV, HIGH);
        SpeedV = map(analogRead(SPDV), 0, 1023, 250, 5000);
      }
      buttonUp = digitalRead(up);
      buttonDown = digitalRead(down);
      k = k + 1;
      while (k == 300 && buttonUp == 0 && buttonDown == 1 && Switcher == 0) {
        stepperD(1, SpeedV, HIGH);
        SpeedV = map(analogRead(SPDV), 0, 1023, 250, 5000);
        buttonUp = digitalRead(up);
        buttonDown = digitalRead(down);
      }
      delay(1);
    }
    while (buttonUp == 1 && buttonDown == 0 && Switcher == 0) {
      if (k == 0) {
        stepperD(4, SpeedV, LOW);
        SpeedV = map(analogRead(SPDV), 0, 1023, 250, 5000);
      }
      buttonUp = digitalRead(up);
      buttonDown = digitalRead(down);
      k = k + 1;
      while (k == 300 && buttonUp == 1 && buttonDown == 0 && Switcher == 0) {
        stepperD(1, SpeedV, LOW);
        SpeedV = map(analogRead(SPDV), 0, 1023, 250, 5000);
        buttonUp = digitalRead(up);
        buttonDown = digitalRead(down);
      }
      delay(1);
    }

    

    delay(100);
    inStr = "";
    inInt = 0;
  }

#ifdef DEBUG_ENABLE
  Serial.println("Hand off");
#endif

  if (Serial.available() > 0) inChar = Serial.read();
  buttonStart = digitalRead(StartPin);

#ifdef DEBUG_ENABLE
  Serial.println(inChar);
#endif

  while (inChar == 's' || buttonStart == 0)
  {

#ifdef DEBUG_ENABLE
    Serial.println("start");
#endif

    inChar = 'a';
    step_KP();
    delay(1000);
    //TODO: реализовать функцию выбора режима работы реактора
    //if (impulse_mode(4) == true) break;
    if (duga_mode() == true) break;
    buttonStart = digitalRead(StartPin);
    //Отвод анода наверх
  }
  step_KP();
  stepperV(100, 600, LOW);
  Serial.println("end");
  delay(10000);
}
