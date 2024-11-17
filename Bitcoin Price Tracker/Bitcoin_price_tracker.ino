#include <SPI.h>
#include <TFT_eSPI.h>
#include <WiFi.h>
#include <ArduinoJson.h>
#include <PNGdec.h>
#include <HTTPClient.h>

#include "btc_logo.h"  // Include the Bitcoin logo image

#define MAX_IMAGE_WIDTH 64  // Maximum width of the image

PNG png;  // Create an instance of the PNG decoder
TFT_eSPI tft = TFT_eSPI();  // Create an instance of the TFT display

// Replace with your network credentials
const char* ssid = "your-ssid";
const char* password = "your-password";

// Function to connect to Wi-Fi
void connectToWiFi() {
  Serial.print("Connecting to Wi-Fi...");
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("Connected to Wi-Fi");
  Serial.println(WiFi.localIP());
}

// Function to retrieve the current Bitcoin price from the Binance API
float getBitcoinPrice(float& changePercentage) {
  HTTPClient http;

  // Make a GET request to the Binance API
  http.begin("https://api.binance.com/api/v3/ticker/24hr?symbol=BTCUSDT");

  // Send the request
  int httpResponseCode = http.GET();

  float price = 0.0;

  if (httpResponseCode == HTTP_CODE_OK) {
    // Parse the JSON response
    DynamicJsonDocument jsonBuffer(2048);
    DeserializationError error = deserializeJson(jsonBuffer, http.getString());

    if (error) {
      Serial.print("Failed to parse JSON: ");
      Serial.println(error.c_str());
    } else {
      // Extract the Bitcoin price and change percentage
      price = jsonBuffer["lastPrice"].as<float>();
      changePercentage = jsonBuffer["priceChangePercent"].as<float>();
    }
  } else {
    Serial.print("HTTP request failed with error code: ");
    Serial.println(httpResponseCode);
  }

  // Close the connection
  http.end();

  return price;
}

// Function to update the display with the Bitcoin price
void updateDisplay(float price, float changePercentage) {
  tft.fillScreen(TFT_BLACK);  // Clear the screen

  // Draw the Bitcoin logo
  int16_t rc = png.openFLASH((uint8_t *)pngegg, sizeof(pngegg), pngDraw);
  if (rc == PNG_SUCCESS) {
    tft.startWrite();
    uint32_t dt = millis();
    rc = png.decode(NULL, 0);
    tft.endWrite();
  }

  // Display the Bitcoin price

  int16_t textWidth = tft.textWidth("$" + String(price, 2));
  int16_t textHeight = tft.fontHeight(); // Double the font height
  int16_t x = tft.width()  / 2;
  int16_t y = (tft.height() - textHeight) / 2 + 16;  // Center vertically
  tft.setTextColor(TFT_WHITE);
  tft.drawCentreString("$" + String(price, 2),x,y,4);

  // Display 24-hour change percentage below the price
  // the price difference in a colored rectangle at the bottom of the screen
  int16_t rectHeight = 20;  // Adjust the height of the rectangle
  int16_t rectY = tft.height() - rectHeight - 8;
  int16_t rectColor = changePercentage >= 0 ? TFT_GREEN : TFT_RED;
  tft.fillRoundRect(20, rectY, tft.width()-40, rectHeight,8, rectColor);

  // Display the price difference percentage text
  tft.setTextColor(TFT_WHITE);
  tft.setTextSize(1);
  tft.setTextDatum(MC_DATUM);
  String priceDiffText = changePercentage >= 0 ? "+" : "";
  priceDiffText += String(changePercentage, 3) + "%";
  tft.drawString(priceDiffText, tft.width() / 2, rectY + rectHeight / 2);
}

// Callback function for PNG decoding and drawing
void pngDraw(PNGDRAW *pDraw) {
  uint16_t lineBuffer[MAX_IMAGE_WIDTH];
  png.getLineAsRGB565(pDraw, lineBuffer, PNG_RGB565_BIG_ENDIAN, 0xffffffff);
  tft.pushImage((tft.width() - MAX_IMAGE_WIDTH) / 2, 8 + pDraw->y, pDraw->iWidth, 1, lineBuffer);
}

void setup() {
  Serial.begin(115200);

  tft.init();
  tft.setRotation(4);

  connectToWiFi();
}

void loop() {
  static unsigned long previousMillis = 0;
  const unsigned long interval = 5000;  // Update every 5 seconds

  unsigned long currentMillis = millis();


  if (currentMillis - previousMillis >= interval) {
    float bitcoinPrice = 0.0;
    float bitcoinChangePercentage = 0.0;
    bitcoinPrice = getBitcoinPrice(bitcoinChangePercentage);
    updateDisplay(bitcoinPrice, bitcoinChangePercentage);
    previousMillis = currentMillis;
  }
}
