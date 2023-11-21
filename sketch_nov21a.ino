  // Mole Mole by Musashinodenpa Prototypers 2009

  #include <LiquidCrystal.h>
  #include <MsTimer2.h>

  #define BUTTON 14
  #define JOYSTICK 1
  #define MAX_DURATION 30
  #define MAX_INTERVAL 80
  #define MIN_INTERVAL 30
  #define APPEAR 1
  #define PAUSE 2
  #define DISAPPEAR 3

  // RS, Enable, D4, D5, D6, D7
  LiquidCrystal lcd(2,3,4,5,6,7);
  // for microfan CLCD-Booster
  //LiquidCrystal lcd(4,3,3,14,15,16,17);

  struct Mole {
    unsigned char pos;
    unsigned char phase;
    unsigned char pause;
    unsigned char anim;
  };
  volatile Mole mole = {0, 0, 0, 0};

  volatile unsigned char hammerPos;
  volatile unsigned int score;
  volatile unsigned char life = 3;

  byte bitmap[8][8] = {
    {
      B00000, // hole
      B00000,
      B00000,
      B00000,
      B00000,
      B10001,
      B01110
    }, {
      B00000, // mole 1
      B00000,
      B00000,
      B00000,
      B00000,
      B11111,
      B01110
    }, {
      B00000, // mole 2
      B00000,
      B00000,
      B00000,
      B01110,
      B11111,
      B01110
    }, {
      B00000, // mole 3
      B00000,
      B00000,
      B01110,
      B11111,
      B10101,
      B01110
    }, {
      B00000, // mole 4
      B00000,
      B01110,
      B11111,
      B10101,
      B11111,
      B01110
    }, {
      B00000, // mole 5
      B01110,
      B11111,
      B10101,
      B11111,
      B11111,
      B01110
    }, {
      B01110, // mole 6
      B11111,
      B10101,
      B11111,
      B11111,
      B11111,
      B01110
    }, {
      B11100, // hammer
      B11100,
      B11111,
      B11111,
      B11100,
      B11100,
      B00000
    }
  };

  void setup() {
    Serial.begin(9600);

    lcd.begin(16, 2);

    for(char i = 0; i < 8; i++) {
      lcd.createChar(i, bitmap[i]);
    }

    pinMode(17, OUTPUT);
    digitalWrite(17, HIGH);

    digitalWrite(BUTTON, HIGH); // pullup

    MsTimer2::set(50, refresh);
    MsTimer2::start();

    Serial.println("initialized.");
  }


  void loop() {
    hammerPos = analogRead(JOYSTICK) / 114;
    if(digitalRead(BUTTON) == LOW) {
      if(hammerPos == mole.pos && mole.phase > 0) {
        hit();
      } else {
        MsTimer2::stop();
        swing(hammerPos);
        delay(100);
        MsTimer2::start();
      }
    }
  }

  void swing(char pos) {
    lcd.setCursor(pos,0);
    lcd.print(" ");
    lcd.setCursor(pos,1);
    lcd.write((uint8_t)0);
  }

  void hit() {
    MsTimer2::stop();
    swing(hammerPos);
    lcd.setCursor(mole.pos + 1,1);
    lcd.print("HIT!");
    numbers(life, ++score);
    mole.phase = 0;
    mole.anim = 0;
    delay(200);
    MsTimer2::start();
  }

  void refresh() {
    static char duration = MAX_DURATION, interval = MAX_INTERVAL;
    static unsigned int clock;

    if(mole.phase == 0 && clock++ > interval) {
      if(--interval < MIN_INTERVAL) interval = MIN_INTERVAL;
      if(--duration < 1) duration = 1;
      clock = 0;
      mole.pos = random(0,5) * 2;
      mole.phase = APPEAR;
    }

    lcd.clear();

    lcd.setCursor(hammerPos,0);
    lcd.write(7);

    switch(mole.phase) {
    case APPEAR:
      if(++mole.anim == 6) {
        mole.pause = duration;
        mole.phase = 2;
      }
      break;

    case PAUSE:
      if(--mole.pause == 0) {
        mole.phase = 3;
      }
      break;

    case DISAPPEAR:
      if(--mole.anim == 0) {
        mole.phase = 0;
        miss();
      }
      break;

    default:
      ;
    }

    for(char i=0; i<5; i++) {
      lcd.setCursor(i*2,1);
      lcd.write(static_cast<uint8_t>(0));
    }
    numbers(life, score);

    lcd.setCursor(mole.pos,1);
    lcd.write(mole.anim);
  }

  void numbers(unsigned char l, unsigned int s) {
    lcd.setCursor(13,0);
    lcd.print(l, DEC);
    lcd.setCursor(13,1);
    lcd.print(s, DEC);
  }

  void miss() {
    if(--life == 0) {
      MsTimer2::stop();
      lcd.setCursor(0,0);
      lcd.print("GAME OVER!     ");
      for(;;){}
    }
  }