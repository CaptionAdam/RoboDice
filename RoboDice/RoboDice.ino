//RoboDiceGoBrrrrrrr
// Now Using SeedStudio Xaio ESP32-C6

//OLED Stuff
#include "DisplayControl.h"

#define StartingMode 1



#define RollButton D8
#define SplitButton D7
#define SwitchButton D9

int roll = 0;
int dVal = 0;
int screenPrint = 1;
int FirstRoll = 1;
int RollMode = StartingMode;
int OutputMode = 0;
int iD1 = 0;
int iD2 = 0;
int pastRolls[3];

void setup() {
  int ButState = digitalRead(RollButton);

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
  // DrawBootScreen();
  drawbitmap();


  while (FirstRoll == 1) {
    FirstRollSplash();  //Pre-First Roll Splash

    ButState = digitalRead(RollButton);
    if (ButState == 0) {
      randomSeed(micros());
      delay(30);
      FirstRoll = 0;
      // roll = 1;
    }
  }
  RollModeSplash();
}

void loop() {
  int ButState, ButState1, ButState2, RollModeLast = 9;

  // Set Random Seed to Time in micro secinds since program start


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

  if (roll == 1) {
    switch (RollMode) {
      case 0:
        // Roll 1 D6
        dVal = random(1, 7);
        Serial.println(dVal);
        OutputMode = 0;
        Serial.println("1*6");
        break;

      case 1:
        // Roll 2 D2
        if( dVal == 0) OutputMode = 0;
        iD1 = random(1, 7);
        iD2 = random(1, 7);
        dVal = iD1 + iD2;
        Serial.println(dVal);
        Serial.println("2*6");
        break;

      case 2:
        // Roll 1 D12
        dVal = random(1, 13);
        Serial.println(dVal);
        OutputMode = 0;
        Serial.println("1*12");
        break;

      case 3:
        // Roll 1 D4
        dVal = random(1, 5);
        Serial.println(dVal);
        OutputMode = 0;
        Serial.println("1*4");
        break;

      case 4:
        // Roll 1 D4 for NESW
        dVal = random(1, 5);
        Serial.println(dVal);
        OutputMode = 2;
        Serial.println("NESW");
        break;

      default:
        Serial.println("It's Broke");
        break;
    }
    roll = 0;
    if (dVal |= 0) screenPrint = 1;
    Rolling();
  }

  if (dVal |= 0) screenOut();

  // screenOut();
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
  dVal = 0;
  RollMode += 1;
  if (RollMode >= 5) RollMode = 0;
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
  display.println("Set Seed");
  display.println(micros());
  display.display();
}

void screenOut() {
  display.clearDisplay();
  display.setTextSize(8);
  display.setTextColor(SSD1306_WHITE);
  switch (OutputMode) {
    case 0:
      // Single Number
      if (dVal <= 9) display.setCursor(40, 0);  //
      if (dVal >= 10) display.setCursor(15, 0);
      display.println(dVal);
      break;
    case 1:
      // 2 Single Digit
      display.setCursor(0, 0);  // Start at top-left corner
      display.println(iD1);
      display.setCursor(80, 0);
      display.print(iD2);
      display.setTextSize(4);
      display.setCursor(50, 18);
      display.print("+");
      break;
    case 2:
      // DIRECTIONAL
      display.setCursor(40, 0);
      if (dVal == 1) display.println("N");
      if (dVal == 2) display.println("E");
      if (dVal == 3) display.println("S");
      if (dVal == 4) display.println("W");
      break;
    default:
      break;
  }
  display.display();
}


void RollModeSplash() {
  display.clearDisplay();
  display.setTextSize(5);
  display.setTextColor(SSD1306_WHITE);
  switch (RollMode) {
    case 0:
      display.setCursor(0, 0);  // Start at top-left corner
      display.print("1*6");
      break;
    case 1:
      display.setCursor(0, 0);  // Start at top-left corner
      display.print("2*6");
      break;
    case 2:
      display.setCursor(0, 0);  // Start at top-left corner
      display.print("1*12");
      break;
    case 3:
      display.setCursor(0, 0);  // Start at top-left corner
      display.print("1*4");
      break;
    case 4:
      display.setCursor(0, 0);  // Start at top-left corner
      display.print("NESW");
      break;
    default:
      break;
  }

  display.display();
  delay(100);
}