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

#define JST 9*3600


// for SPI pin definitions see e.g.:
// C:\Users\xxx\AppData\Local\Arduino15\packages\esp8266\hardware\esp8266\2.4.2\variants\generic\common.h

GxIO_Class io(SPI, /*CS=D8*/ D8, /*DC=D3*/ D3, /*RST=D4*/ D4); // arbitrary selection of D3(=0), D4(=2), selected for default of GxEPD_Class
GxEPD_Class display(io, /*RST=D4*/ D4, /*BUSY=D2*/ D2);        // default selection of D4(=2), D2(=4)
HTTPClient client;
BearSSL::WiFiClientSecure secure;

const uint32_t SleepTime = 10 * 60 * 1000;
uint32_t lastExecTime = 0;

void setup()
{
  Serial.begin(115200);
  Serial.println();
  Serial.println("setup");

  display.init(115200); // enable diagnostic output on Serial

  WiFi.begin("ssid", "password");

  // TODO:Wifiがつながらないことがある
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
  configTime(JST, 0, "ntp.nict.jp", "ntp.jst.mfeed.ad.jp");
  while (time(NULL) <= 100000){
    delay(500);
    Serial.print(".");
  }
  Serial.println("done");
  time_t t = time(NULL);
  tm* tm = localtime(&t);
  Serial.printf("Now: %02d:%02d ", tm->tm_hour, tm->tm_min);

  Serial.println("setup done");

}

void loop()
{
  Serial.printf("loop millis:%u\n", millis());
  if(abs(millis() - lastExecTime) > SleepTime){
    lastExecTime = millis();
    getWeatherInfoJpeg();
  }
  delay(10*1000);
}

void getWeatherInfoJpeg(){
  Serial.println("Http access");

httpaccess:
  client.begin(secure, "test.example.local", 443, "/path", true);
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
    goto httpaccess;
  }
}