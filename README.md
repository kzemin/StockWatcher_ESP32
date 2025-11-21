# 📈 STOCK WATCHER – ESP32-S3 + TFT 160x80 + Finnhub API
This proyect updates on screen the real-time prices for stocks and crypto, using an ESP32-S3, a TFT 160×80 display ST7735 and the Finnhub API.

It uses the maximum amount of requests allowed by Finnhub (1 request/second) without exceding the rate-limit.

## 🔧 Hardware needed

- ESP32-S3 (any model with WiFi).

- Display TFT 160×80 SPI ST7735.

- Jumpers / Dupont cables.

- USB 5V power source.

## 📚 Libraries

Before compiling you need to install the following libraries:

✔ TFT_eSPI (Bodmer)

✔ ArduinoJson

✔ WiFi.h (included with ESP32)

✔ HTTPClient.h (included with ESP32)

IMPORTANT:
Your board should be configured with the ESP32 Board Package on the IDE.

## ⚙️ REQUIRED CONFIGURATION (GIGA IMPORTANT)

TFT_eSPI needs configurating Arduino/libraries/TFT_eSPI/User_Setup.h

Inserting this #define code on User_Setup.h you must:

```cpp
#define USER_SETUP_ID 201

#define ST7735_DRIVER
#define ST7735_GREENTAB160x80
#define CGRAM_OFFSET
#define TFT_RGB_ORDER TFT_RGB

#define TFT_WIDTH  80
#define TFT_HEIGHT 160

#define TFT_MOSI 35
#define TFT_SCLK 36
#define TFT_CS   -1
#define TFT_DC    2
#define TFT_RST   4
#define TFT_MISO -1

#define LOAD_GLCD
#define LOAD_FONT2
#define LOAD_FONT4
#define LOAD_GFXFF

#define SMOOTH_FONT

#define SPI_FREQUENCY  20000000
#define SPI_READ_FREQUENCY  20000000
```
## Known issues (Work in progress)

The ticker symbol for BTC is not getting formatted correctly.
