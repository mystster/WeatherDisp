// mapping suggestion from Waveshare SPI e-Paper to Wemos D1 mini
// BUSY -> D2, RST -> D4, DC -> D3, CS -> D8, CLK -> D5, DIN -> D7, GND -> GND, 3.3V -> 3.3V

// include library, include base class, make path known
#include <GxEPD.h>
#include <GxGDEW029T5/GxGDEW029T5.h> // 2.9" b/w IL0373

#include <GxIO/GxIO_SPI/GxIO_SPI.h>
#include <GxIO/GxIO.h>

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

#include <time.h>
// Please enter your sensitive data in the Secret tab or arduino_secrets.h(SECRET_xxxx defines)
#include "arduino_secret.h"

#define JST 9*3600


// for SPI pin definitions see e.g.:
// C:\Users\xxx\AppData\Local\Arduino15\packages\esp8266\hardware\esp8266\2.4.2\variants\generic\common.h

GxIO_Class io(SPI, /*CS=D8*/ D8, /*DC=D3*/ D3, /*RST=D4*/ D4); // arbitrary selection of D3(=0), D4(=2), selected for default of GxEPD_Class
GxEPD_Class display(io, /*RST=D4*/ D4, /*BUSY=D2*/ D2);        // default selection of D4(=2), D2(=4)
HTTPClient client;
BearSSL::WiFiClientSecure secure;

const int32_t getWeatherInfoPeriod = 7;
const int32_t getCurrentTempPeriod = 10;
const int16_t marginSleepTime = 30;
const uint32_t USER_DATA_ADDR = 66;

struct {
  time_t getWeatherInfoJpeg;
  time_t getCurrentTemp;
} lastExecDate;


void setup()
{
  Serial.begin(74880);
  Serial.println();
  Serial.println("setup");

  display.init(74880); // enable diagnostic output on Serial

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

  Serial.print("Time syncing");
  configTzTime("JST-9", "ntp.nict.jp", "ntp.jst.mfeed.ad.jp");
  while (time(NULL) <= 100000){
    delay(500);
    Serial.print(".");
  }
  Serial.println("done");

  Serial.println("determine wake reason");
  if (ESP.getResetReason() == "Deep-Sleep Wake"){
    Serial.println("wakeup from deepSleep");
    ESP.rtcUserMemoryRead(USER_DATA_ADDR, (uint32_t *)&lastExecDate, sizeof(lastExecDate));
  }else{
    Serial.println("normal wakeup. Init data");
    lastExecDate.getCurrentTemp = 0;
    lastExecDate.getWeatherInfoJpeg = 0;
  }

  time_t now = time(NULL);
  tm* tm = localtime(&now);
  Serial.printf("Now: %02d:%02d \n", tm->tm_hour, tm->tm_min);

  Serial.println("setup done");

  if(now - lastExecDate.getWeatherInfoJpeg > getWeatherInfoPeriod - marginSleepTime){
    // getWeatherInfoJpeg();
    Serial.println("getWeatherInfoJpeg()");
    lastExecDate.getWeatherInfoJpeg = now;
  }
  if(now - lastExecDate.getCurrentTemp > getCurrentTempPeriod - marginSleepTime){
    //TODO: 現在の気温を取得する
    Serial.println("getCurrentTemp()");
    lastExecDate.getCurrentTemp = now;
  }
  ESP.rtcUserMemoryWrite(USER_DATA_ADDR, (uint32_t *)&lastExecDate, sizeof(lastExecDate));

  int32_t sleepTimeSec = min(lastExecDate.getWeatherInfoJpeg + getWeatherInfoPeriod, lastExecDate.getCurrentTemp + getCurrentTempPeriod) - now;
  Serial.printf("deep sleep %d sec\n", sleepTimeSec);
  ESP.deepSleep(sleepTimeSec * 1000000 / 0.977, RF_DEFAULT);
}

void loop()
{
  delay(1000);
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