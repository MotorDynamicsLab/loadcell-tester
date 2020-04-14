#ifndef _CONTROL_PANEL28_H_
#define _CONTROL_PANEL28_H_

#include <Adafruit_GFX.h> // Core graphics library
#include "LDO_ST7789.h"   // Hardware-specific library for ST7789

#define LDO_PANEL_W (240)
#define LDO_PANEL_H (320)

class ControlPanel28
{
  protected:
    const int bounceDelayMs = 50;
    static unsigned long lastKillPressMs;
    static bool killIsrEna;
    static void (*killPressFcn)();
    int8_t encBtnPin, encAPin, encBPin, killPin, beeperPin;
    int lastEncBtnState, lastEncAState, lastEncBState;
    
  public:
    LDO_ST7789 tft;
    
    ControlPanel28(int8_t cs, int8_t dc, int8_t rst, int8_t encBtn, int8_t encA, int8_t encB, int8_t kill, int8_t beeper);
    void init(void (*fcnPtr)());
    bool checkEncBtn();
    int checkRotaryEnc();
    static void checkKillBtn();
};


#endif // _CONTROL_PANEL28_H_
