//RoboDiceGoBrrrrrrr

//OLED Stuff
#include "DisplayControl.h"

#define PICO_DEFAULT_I2C_SDA_PIN 6
#define PICO_DEFAULT_I2C_SCL_PIN 7
#define RollButton 15
#define SplitButton 16
#define SwitchButton 17

int roll = 0;
int dVal = 0;
int screenPrint = 1;
int FirstRoll = 1;
int RollMode = 1;
int OutputMode = 0;
int iD1 = 0;
int iD2 = 0;
int pastRolls[3];

void setup() {

  // Set pinModes
  pinMode(RollButton, INPUT_PULLUP);
  pinMode(SplitButton, INPUT_PULLUP);
  pinMode(SwitchButton, INPUT_PULLUP);

  // Begin Serial Comunications
  Serial.begin(19200);

  //OLED Stuff
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
  }

  // BootScreen
  display.display();
  drawbitmap();
  RollModeSplash();
  FirstRollSplash();  //Pre-First Roll Splash
}

void loop() {
  int ButState, ButState1, ButState2, RollModeLast = 9;

  // Set Random Seed to Time in micro secinds since program start



  while (FirstRoll == 1) {
    int ButState = digitalRead(RollButton);
    if (ButState == 0) {
      randomSeed(micros());
      delay(30);
      FirstRoll = 0;
      roll = 1;
    }
    Serial.println(micros());
  }

  ButState = digitalRead(RollButton);
  if (ButState == 0) {
    roll = 1;
    delay(30);
  }

  ButState1 = digitalRead(SplitButton);
  if (ButState1 == 0 && RollMode == 1) {
    ChangeMode();
    delay(200);
  }

  ButState2 = digitalRead(SwitchButton);
  if (ButState2 == 0) {
    RollModeChange();
    delay(200);
    RollModeSplash();
  }

  switch (RollMode) {
    case 0:
      if (roll == 1) {
        dVal = random(1, 7);
        Serial.println(dVal);
        screenPrint = 1;
        roll = 0;
        OutputMode = 0;
        Rolling();
        Serial.println("1*6");
      }
      break;
    case 1:
      // Roll 2 D6
      if (roll == 1) {
        iD1 = random(1, 7);
        iD2 = random(1, 7);
        dVal = iD1 + iD2;
        roll = 0;
        Serial.println(dVal);
        screenPrint = 1;
        Rolling();
        Serial.println("2*6");
      }
      break;
    case 2:
      if (roll == 1) {
        dVal = random(1, 13);
        Serial.println(dVal);
        roll = 0;
        screenPrint = 1;
        OutputMode = 0;
        Rolling();
        Serial.println("1*12");
      }
      break;
    default:
      Serial.println("It's Broke");
      break;
  }


  screenOut();

  //screenOut();
}

void ChangeMode() {
  OutputMode += 1;
  if (OutputMode >= 2) OutputMode = 0;
  if (OutputMode == 0) {
    Serial.println("Single");
  }
  if (OutputMode == 1) {
    Serial.println("Dual");
  }
}

void RollModeChange() {
  RollMode += 1;
  if (RollMode >= 3) RollMode = 0;
  // if (RollMode == 0) {
  //   Serial.println("1*6");
  // }
  // if (RollMode == 1) {
  //   Serial.println("2*6");
  // }
  // if (RollMode == 2) {
  //   Serial.println("1*12");
  Serial.println(RollMode);
}


void Rolling() {
  if (screenPrint == 1) {
    display.clearDisplay();
    display.setTextSize(2);
    display.setCursor(0, 0);  // Start at top-left corner
    display.println("Rolling");
    display.display();
    delay(200);
    display.println("Rolling.");
    display.display();
    delay(200);
    display.println("Rolling..");
    display.display();
    delay(200);
    display.println("Rolling...");
    display.display();
    delay(200);
  }
}

void FirstRollSplash() {
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);  // Start at top-left corner
  display.println("Press To");
  display.println("Roll");
  display.display();
}

void screenOut() {
  if ((screenPrint == 1) && (OutputMode == 0)) {
    display.clearDisplay();
    display.setTextSize(8);
    display.setTextColor(SSD1306_WHITE);
    if (dVal <= 9) display.setCursor(40, 0);  // Start at top-left corner
    if (dVal >= 10) display.setCursor(15, 0);
    display.println(dVal);
    display.setCursor(0, 0);  // Start at top-left corner
    display.setTextSize(2);
    display.display();
  }

  if ((screenPrint == 1) && (OutputMode == 1)) {
    display.clearDisplay();
    display.setTextSize(8);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);  // Start at top-left corner
    display.println(iD1);
    display.setCursor(80, 0);
    display.print(iD2);
    display.setTextSize(4);
    display.setCursor(50, 18);
    display.print("+");
    display.display();
  }
}


void RollModeSplash() {
  display.clearDisplay();
  display.setTextSize(5);
  display.setTextColor(SSD1306_WHITE);
  if (RollMode == 0) {
    display.setCursor(0, 0);  // Start at top-left corner
    display.print("1*6");
  }
  if (RollMode == 1) {
    display.setCursor(0, 0);  // Start at top-left corner
    display.print("2*6");
  }
  if (RollMode == 2) {
    display.setCursor(0, 0);  // Start at top-left corner
    display.print("1*12");
  }
  display.display();
  delay(1000);
}