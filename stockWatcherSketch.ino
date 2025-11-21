#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <SPI.h>

#define TFT_CS    -1 
#define TFT_RST   4
#define TFT_DC    2
#define TFT_MOSI  35  
#define TFT_SCLK  36  

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

// WI-FI
const char* ssid     = "Tu wifi";
const char* password = "Tu contraseña";

// API
String apiKey = "Tu API Key";

// STOCKS
String stocks[] = {"TSLA", "BINANCE:BTCUSDT", "VIX.VN", "SPY", "GLD"};
int totalStocks = sizeof(stocks) / sizeof(stocks[0]);

#define TFT_BLACK   ST77XX_BLACK
#define TFT_WHITE   ST77XX_WHITE
#define TFT_RED     ST77XX_RED
#define TFT_GREEN   ST77XX_GREEN
#define TFT_BLUE    ST77XX_BLUE
#define TFT_CYAN    ST77XX_CYAN
#define TFT_MAGENTA ST77XX_MAGENTA
#define TFT_YELLOW  ST77XX_YELLOW

// FUNCS
void wifiConnect() {
  tft.fillScreen(TFT_BLACK);
  tft.setCursor(0, 0);
  tft.setTextSize(1);
  tft.setTextColor(TFT_WHITE);
  tft.println("Conectando WiFi...");

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(300);
    tft.print(".");
  }

  tft.println("\nWiFi OK!");
  delay(1000);
}

void showStock(String symbol) {
  String url = "https://finnhub.io/api/v1/quote?symbol=" + symbol + "&token=" + apiKey;

  HTTPClient http;
  http.begin(url);
  int code = http.GET();

  tft.fillScreen(TFT_BLACK);
  tft.setCursor(0, 0);
  tft.setTextColor(TFT_WHITE);

  if (code > 0) {
    String json = http.getString();

    DynamicJsonDocument doc(2048);
    DeserializationError error = deserializeJson(doc, json);

    if (error) {
      tft.println("JSON ERROR");
      Serial.print("JSON Error: ");
      Serial.println(error.c_str());
      http.end();
      delay(3000);
      return;
    }

    float price = doc["c"]; // precio actual
    float prev  = doc["pc"]; // cierre previo
    float diff  = ((price - prev) / prev) * 100.0;

    // formateo ya que el ticker original es muy grande
    String displayName = symbol;
    if (symbol == "BINANCE:BTCUSDT") {
      displayName = "BTC";
    }

    // Titulo
    tft.setTextSize(3);
    tft.println(symbol);
    
    tft.println();

    // Precio
    tft.setTextSize(2);
    tft.setTextColor(TFT_WHITE);
    tft.println("Price:");
    tft.print("$");
    tft.println(price, 2);

    tft.println(); 

    // Porcentajes
    tft.setTextSize(2);
    tft.println("Change:");
    
    if (diff >= 0) {
      tft.setTextColor(TFT_GREEN);
      tft.print("+");
    } else {
      tft.setTextColor(TFT_RED);
    }
    
    tft.print(diff, 2);
    tft.println("%");

    // Debug
    Serial.print(symbol);
    Serial.print(" - $");
    Serial.print(price, 2);
    Serial.print(" (");
    Serial.print(diff, 2);
    Serial.println("%)");

  } else {
    tft.setTextSize(2);
    tft.setTextColor(TFT_RED);
    tft.println("HTTP ERROR");
    tft.print("Code: ");
    tft.println(code);
    
    Serial.print("HTTP Error: ");
    Serial.println(code);
  }

  http.end();
  delay(3500); 
}

// SETUP
void setup() {
  Serial.begin(115200);
  delay(1000);
  
  Serial.println("\n=== STOCK WATCHER INICIANDO ===");
  
  // Inicializa SPI
  SPI.begin(TFT_SCLK, -1, TFT_MOSI, -1);
  Serial.println("SPI iniciado");
  
  // Inicializa display
  tft.initR(INITR_MINI160x80);
  tft.setRotation(1);
  Serial.println("Display inicializado");
  
  // Welcome
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_CYAN);
  tft.setTextSize(2);
  tft.setCursor(10, 20);
  tft.println("STOCK");
  tft.setCursor(10, 40);
  tft.println("WATCHER");
  delay(2000);
  
  wifiConnect();
  
  Serial.println("=== SETUP COMPLETO ===");
}

// LOOP
void loop() {
  for (int i = 0; i < totalStocks; i++) {
    showStock(stocks[i]);
  }
  
  tft.fillScreen(TFT_BLACK);
  tft.setTextSize(2);
  tft.setTextColor(TFT_CYAN);
  tft.setCursor(10, 30);
  tft.println("Actualizando...");
  delay(1500);
}