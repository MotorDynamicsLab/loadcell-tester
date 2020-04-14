#ifndef _LDO_ST7789H_
#define _LDO_ST7789H_

#include "Adafruit_ST77xx.h"

/// Subclass of ST77XX type display for ST7789 TFT Driver
class LDO_ST7789 : public Adafruit_ST77xx {
  public:
    LDO_ST7789(int8_t cs, int8_t dc, int8_t mosi, int8_t sclk,
      int8_t rst = -1);
    LDO_ST7789(int8_t cs, int8_t dc, int8_t rst);
#if !defined(ESP8266)
    LDO_ST7789(SPIClass *spiClass, int8_t cs, int8_t dc, int8_t rst);
#endif // end !ESP8266

    void setRotation(uint8_t m);
    void init(uint16_t width, uint16_t height);
};

#endif // LDO_ST7789
