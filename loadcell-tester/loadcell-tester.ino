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
#include "HX711.h"

const int TFT_CS = 10;
const int TFT_RST = 9;
const int TFT_DC = 8;

const int BTN_ENC = 35;
const int BTN_ENC1 = 31;
const int BTN_ENC2 = 33;
const int KILL_PIN = 41;
const int BEEPER_PIN = 42;

const int LOADCELL_DOUT_PIN = 20;
const int LOADCELL_SCK_PIN = 21;

ControlPanel28 panel = ControlPanel28(TFT_CS, TFT_DC, TFT_RST, BTN_ENC, BTN_ENC1, BTN_ENC2, KILL_PIN, BEEPER_PIN);

//UI related variables
const int settingLineHeight = 20;
const int settingFirstLinePos = 50;
const int settingLine0 = settingFirstLinePos;
const int settingLine1 = settingFirstLinePos + settingLineHeight;
const int settingLine2 = settingFirstLinePos + settingLineHeight * 2;

//App global variables
const unsigned long samplePeriodMs = 400;
float loadcellVal = 0;
unsigned long lastSampleMs = 0;
void killPressed();
HX711 scale;

void setup(void) {
  panel.init(killPressed);
  lcdSetup();
  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
  scale.set_scale(1.0f);
  scale.tare();
}

void loop() {
  bool isEncPressed = panel.checkEncBtn();
  //int change = panel.checkRotaryEnc();

  if (isEncPressed)
  {
    scale.tare();
  }

  sampleLoadcell();
}

void sampleLoadcell()
{
  unsigned long nowMillis = millis();
  if (nowMillis - lastSampleMs > samplePeriodMs)
  {
    lastSampleMs = nowMillis;
    loadcellVal = 0.001f * scale.get_value(1);
    updateDisplay();
  }
}

void updateDisplay() {
  panel.tft.setTextSize(4);
  if (loadcellVal > 10.0f)
  {
    panel.tft.setTextColor(ST77XX_GREEN, ST77XX_BLACK);
  }
  else if (loadcellVal < -10.0f)
  {
    panel.tft.setTextColor(ST77XX_RED, ST77XX_BLACK);
  }
  else
  {
    panel.tft.setTextColor(ST77XX_WHITE, ST77XX_BLACK);
  }

  panel.tft.setCursor(50, settingLine1 + 10);
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

  panel.tft.setCursor(0, 160);
  panel.tft.println("Press [Enc] to tare");
  panel.tft.setCursor(0, 160 + settingLineHeight);
  panel.tft.println("Press [Kill] to restart");
  delay(500);
  updateDisplay();
}

void killPressed()
{
  RSTC->RSTC_CR = 0xA5000005; //reset
}
