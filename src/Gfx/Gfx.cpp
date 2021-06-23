
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "./Logo/Logo.h"

#include "Gfx.h"

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
// The pins for I2C are defined by the Wire-library.
// On an arduino UNO:       A4(SDA), A5(SCL)
// On an arduino MEGA 2560: 20(SDA), 21(SCL)
// On an arduino LEONARDO:   2(SDA),  3(SCL), ...
#define OLED_RESET -1       // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

Logo* logo = new Logo();

uint16_t lineCount = 0;
bool updateDisplay = true;
String scr_line1;
String scr_line2;
String scr_line3;
String scr_line4;

Gfx::Gfx()
{
}

void Gfx::render()
{
    if (!updateDisplay)
        return;

    display.clearDisplay();
    display.setCursor(0, 0);

    // UI/display
    display.println(scr_line1);
    display.println(scr_line2);
    display.println(scr_line3);
    display.println(scr_line4);

    display.display();
    updateDisplay = false;
}

void Gfx::write_clear()
{
    scr_line1.clear();
    scr_line2.clear();
    scr_line3.clear();
    scr_line4.clear();
    updateDisplay = true;
}

void Gfx::writeln_s(String *txt)
{
    scr_line1 = scr_line2;
    scr_line2 = scr_line3;
    scr_line3 = scr_line4;
    scr_line4 = *txt;
    updateDisplay = true;
}

String write_buffer = *(new String());

void Gfx::writeln(const char *txt)
{
    write_buffer.clear();
    write_buffer.concat(txt);
    writeln_s(&write_buffer);
}

void Gfx::displayLogo()
{
    display.clearDisplay();

    uint8_t imgBuffer[logo->getByteLength()];
    logo->getBitmap(imgBuffer);

    display.drawBitmap(
        (display.width() - logo->getWidth()) / 2,
        (display.height() - logo->getHeight()) / 2,
        imgBuffer, logo->getWidth(), logo->getHeight(), 1);
    display.display();

    free(imgBuffer);
    delay(3000);
}

void Gfx::setup_lcd()
{

    // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
    if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS))
    {
        Serial.println(F("SSD1306 allocation failed"));
        for (;;)
            ; // Don't proceed, loop forever
    }

    // Show initial display buffer contents on the screen --
    // the library initializes this with an Adafruit splash screen.
    display.clearDisplay();
    displayLogo();
    display.clearDisplay();

    display.setTextSize(1);              // Normal 1:1 pixel scale
    display.setTextColor(SSD1306_WHITE); // Draw white text
    display.setCursor(0, 0);             // Start at top-left corner
}