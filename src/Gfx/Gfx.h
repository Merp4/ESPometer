/*
  Gfx.h
*/
#ifndef Gfx_h
#define Gfx_h

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

class Gfx
{
public:
    Gfx();
    void render();
    void write_clear();
    void writeln_s(String *txt);
    void writeln(const char *txt);
    void setup_lcd();

private:
    int _pin;
    void displayLogo();
};

#endif