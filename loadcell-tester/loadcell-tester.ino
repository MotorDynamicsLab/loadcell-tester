/**************************************************************************
This is a simple tool used for interfacing with a HX711 chip for testing 
load cells

Basic operation:
- Press knob to select a setting
- Turn knob to change selected setting
- Press kill button to start or stop motor

 $Copyright: Copyright (C) LDO Systems
 **************************************************************************/
#include "ControlPanel28.h" 

const int TFT_CS = 10;
const int TFT_RST = 9;
const int TFT_DC = 8;

const int BTN_ENC = 35;
const int BTN_ENC1 = 31;
const int BTN_ENC2 = 33;
const int KILL_PIN = 41;
const int BEEPER_PIN = 42;

ControlPanel28 panel = ControlPanel28(TFT_CS, TFT_DC, TFT_RST, BTN_ENC, BTN_ENC1, BTN_ENC2, KILL_PIN, BEEPER_PIN);

//UI related variables
const int settingLineHeight = 20;
const int settingFirstLinePos = 50;
const int settingLine0 = settingFirstLinePos;
const int settingLine1 = settingFirstLinePos + settingLineHeight;
const int settingLine2 = settingFirstLinePos + settingLineHeight * 2;

//App global variables
int loadcellVal = 0;
void killPressed();

void setup(void) {
  panel.init(killPressed);
  lcdSetup();
}

void loop() {
  bool isEncPressed = panel.checkEncBtn();
  int change = panel.checkRotaryEnc();

  updateDisplay();
}

void updateDisplay() {
  panel.tft.setTextSize(2);
  panel.tft.setTextColor(ST77XX_CYAN, ST77XX_BLACK);

  panel.tft.setCursor(190, settingLine0);
  panel.tft.print(loadcellVal, 1);
  panel.tft.print("            ");
}

void lcdSetup()
{
  panel.tft.setTextWrap(false);
  panel.tft.setCursor(0, 0);
  panel.tft.fillScreen(ST77XX_BLACK);
  panel.tft.setTextColor(ST77XX_CYAN);
  panel.tft.setTextSize(3);
  panel.tft.println("Loadcell Tester");
  panel.tft.println(" ");

  panel.tft.setTextSize(2);
  panel.tft.setTextColor(ST77XX_WHITE);
  panel.tft.setCursor(0, settingLine0);
  panel.tft.println("Loadcell Value: ");

  panel.tft.setCursor(0, 130);
  panel.tft.println("Press Kill to restart!");
  delay(500);
  updateDisplay();
}

void killPressed()
{
  RSTC->RSTC_CR = 0xA5000005; //reset
}
