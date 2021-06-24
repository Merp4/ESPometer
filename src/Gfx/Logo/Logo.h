/*
  Logo.h
*/
#ifndef Logo_h
#define Logo_h

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_Gfx.h>
#include <Adafruit_SSD1306.h>

class Logo
{
public:
    Logo(int meh);
    int16_t getWidth();
    int16_t getHeight();
    uint16_t getByteLength();
    void getBitmap(uint8_t *buffer);

private:
    int16_t _width;
    int16_t _height;
};

#endif