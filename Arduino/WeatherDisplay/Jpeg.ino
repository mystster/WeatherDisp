#include <GxEPD.h>
#include <GxGDEW029T5/GxGDEW029T5.h> // 2.9" b/w IL0373
// #include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

// JPEG decoder library
#include "JPEGDecoder.h"

void drawJpeg(WiFiClient *client, uint32_t size)
{
    int b = JpegDec.decode(client, size, 0);
    jpegInfo();
    // Serial.printf("c size is %d\n", c);
    display.fillScreen(GxEPD_WHITE);

    display.setRotation(3);

    uint8 *pImg;
    int bx, by, x, y;
    uint16_t px;
    while (JpegDec.read())
    {
        pImg = JpegDec.pImage;

        for (by = 0; by < JpegDec.MCUHeight; by++)
        {

            for (bx = 0; bx < JpegDec.MCUWidth; bx++)
            {

                x = JpegDec.MCUx * JpegDec.MCUWidth + bx;
                y = JpegDec.MCUy * JpegDec.MCUHeight + by;

                if (x < JpegDec.width && y < JpegDec.height)
                {

                    if (JpegDec.comps == 1)
                    { // Grayscale
                        // char str[100];
                        // sprintf(str, "%d,%d,%u", x, y, pImg[0]);
                        // Serial.println(str);
                        display.drawPixel(x, y, pImg[0] > 240 ? 1 : 0);
                    }
                    else
                    { // RGB

                        // 0.3 * R + 0.59 * G + 0.11 * B

                        // px = pImg[0] * 77 + pImg[1] * 151 + pImg[2] * 28;
                        px = pImg[0] + pImg[1] + pImg[2] > 650 ? GxEPD_WHITE : GxEPD_BLACK;

                        display.drawPixel(x, y, px);
                        // display.drawPixel(x, y, pImg[0] > 1 ? GxEPD_WHITE : GxEPD_BLACK);
                        // if(pImg[0] == 0 && pImg[1] + pImg[2] > 0){
                        //    Serial.printf("#RGB,%d,%d,%u,%u,%u\n", x, y, pImg[0], pImg[1], pImg[2]);
                        //    ESP.wdtFeed();
                        // }
                    }
                }
                pImg += JpegDec.comps;
            }
        }
    }
}

//====================================================================================
//   Print information decoded from the Jpeg image
//====================================================================================
void jpegInfo()
{

    Serial.println("===============");
    Serial.println("JPEG image info");
    Serial.println("===============");
    Serial.print("Width      :");
    Serial.println(JpegDec.width);
    Serial.print("Height     :");
    Serial.println(JpegDec.height);
    Serial.print("Components :");
    Serial.println(JpegDec.comps);
    Serial.print("MCU / row  :");
    Serial.println(JpegDec.MCUSPerRow);
    Serial.print("MCU / col  :");
    Serial.println(JpegDec.MCUSPerCol);
    Serial.print("Scan type  :");
    Serial.println(JpegDec.scanType);
    Serial.print("MCU width  :");
    Serial.println(JpegDec.MCUWidth);
    Serial.print("MCU height :");
    Serial.println(JpegDec.MCUHeight);
    Serial.println("===============");
    Serial.println("");
}
