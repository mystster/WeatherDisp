// mapping suggestion from Waveshare SPI e-Paper to Wemos D1 mini
// BUSY -> D2, RST -> D4, DC -> D3, CS -> D8, CLK -> D5, DIN -> D7, GND -> GND, 3.3V -> 3.3V

// include library, include base class, make path known
#include <GxEPD.h>
#include <GxGDEW029T5/GxGDEW029T5.h> // 2.9" b/w IL0373
#include <Fonts/FreeMonoBold9pt7b.h>

#include <GxIO/GxIO_SPI/GxIO_SPI.h>
#include <GxIO/GxIO.h>

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

#include <Ambient.h>
#include <DHT.h>

// Please enter your sensitive data in the Secret tab or arduino_secrets.h(SECRET_xxxx defines)
#include "arduino_secret.h"

// Uncomment the type of sensor in use:
//#define DHTTYPE    DHT11     // DHT 11
#define DHTTYPE DHT22 // DHT 22 (AM2302)
//#define DHTTYPE    DHT21     // DHT 21 (AM2301)

#define DHTPIN D3

// for SPI pin definitions see e.g.:
// C:\Users\xxx\AppData\Local\Arduino15\packages\esp8266\hardware\esp8266\2.4.2\variants\generic\common.h

GxIO_Class io(SPI, /*CS=D8*/ D8, /*DC=D3*/ D4, /*RST=D4*/ D2); // arbitrary selection of D3(=0), D4(=2), selected for default of GxEPD_Class
GxEPD_Class display(io, /*RST=D4*/ D2, /*BUSY=D2*/ D1);        // default selection of D4(=2), D2(=4)
HTTPClient client;
BearSSL::WiFiClientSecure secure;
Ambient ambient;
WiFiClient ambientClient;
DHT dht(DHTPIN, DHTTYPE);

const uint32_t getWeatherInfoPeriod = 10 * 60 * 1000;
const uint32_t getCurrentTempPeriod = 1 * 60 * 1000;
const uint32_t writeCurrentTempPeriod = 5 * 60 * 1000;

struct {
  uint64_t getWeatherInfoJpeg;
  uint64_t getCurrentTemp;
  uint64_t writeCurrentTemp;
} lastExecDate;


void setup()
{
  Serial.begin(74880);
  Serial.println();
  Serial.println("setup");
  display.init(74880); // enable diagnostic output on Serial
  display.setFont(&FreeMonoBold9pt7b);
  display.setTextColor(GxEPD_BLACK);

  WiFi.begin(SECRET_SSID, SECRET_SSID_PASSWORD);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  Serial.print("Connected, IP address: ");
  Serial.println(WiFi.localIP());
  secure.setInsecure();

  ambient.begin(SECRET_AMBIENT_CHANNEL, SECRET_AMBIENT_WRITE, &ambientClient);
  dht.begin();

  Serial.println("Init data");
  lastExecDate.getCurrentTemp = 0;
  lastExecDate.getWeatherInfoJpeg = 0;

  Serial.println("setup done");
}

void loop()
{
  uint32_t now = millis();

  if(now < lastExecDate.getCurrentTemp || now < lastExecDate.getWeatherInfoJpeg ){
    // millis()がオーバーフローしたら
    lastExecDate.getCurrentTemp = 0;
    lastExecDate.getWeatherInfoJpeg = 0;
  }

  if (lastExecDate.getWeatherInfoJpeg == 0 || now - lastExecDate.getWeatherInfoJpeg > getWeatherInfoPeriod)
  {
    Serial.println("getWeatherInfoJpeg()");
    getWeatherInfoJpeg();
    lastExecDate.getWeatherInfoJpeg = now;
    Serial.println("writeCurrentTemp()");
    writeCurrentTemp();
    lastExecDate.writeCurrentTemp = now;
  }
  if (lastExecDate.getCurrentTemp == 0 || now - lastExecDate.getCurrentTemp > getCurrentTempPeriod)
  {
    Serial.println("getCurrentTemp()");
    getCurrentTemp();
    lastExecDate.getCurrentTemp = now;
  }
  if (lastExecDate.writeCurrentTemp == 0 || now - lastExecDate.writeCurrentTemp > writeCurrentTempPeriod)
  {
    Serial.println("writeCurrentTemp()");
    writeCurrentTemp();
    lastExecDate.writeCurrentTemp = now;
  }
}

void getWeatherInfoJpeg(){
  Serial.println("Http access");

httpaccess:
  client.begin(secure, SECRET_WEBSITE, 443, SECRET_WEBSITE_PATH, true);
  int16_t status = client.GET();

  Serial.println("Http get repuested");
  if (status == 200)
  {
    Serial.println("File GET");

    uint32_t size = client.getSize();
    Serial.printf("Memory size is %d\n", size);
    if (size)
    {
      drawJpeg(client.getStreamPtr(), size);
      display.update();
      display.powerDown();
      client.end();
    }
  }
  else
  {
    Serial.printf("status: %d\n", status);
    client.end();
    delay(1000 * 10);
    goto httpaccess;
  }
}

void getCurrentTemp()
{
  float t = dht.readTemperature();
  float h = dht.readHumidity();

  Serial.printf("temp:%f, humi:%f\n", t, h);

  ambient.set(1, t);
  ambient.set(2, h);
  ambient.send();
}

void writeCurrentTemp() {
    int posX = 180;
    int posY = 124;
    int boxW = 100;
    int boxH = 20;

    float t = dht.readTemperature();
    float h = dht.readHumidity();
    Serial.printf("temp:%.1f, humi:%.1f\n", t, h);

    display.setRotation(3);
    display.setCursor(posX, posY);
    display.fillRect(posX, posY - boxH , boxW, boxH + 2, GxEPD_WHITE);
    display.printf("%.1fC/%.0f%%", t, h);
    display.updateWindow(posX, posY - boxH, boxW, boxH + 2, true);
    display.powerDown();
}
