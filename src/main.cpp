//
// A simple server implementation showing how to:
//  * serve static messages
//  * read GET and POST parameters
//  * handle missing pages / 404s
//

#include <Arduino.h>
#include <ESP8266WiFi.h>

#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <DNSServer.h>

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// my libs
#include "Gfx/Gfx.h"
Gfx* gfx = new Gfx();

#include <OneWire.h> 
#include <DallasTemperature.h>
//#define ONE_WIRE_BUS  
OneWire oneWire(D5); 
DallasTemperature sensors(&oneWire);


DNSServer dnsServer;
AsyncWebServer server(80);

//const char *ssid = "";
//const char *password = "";

//const char *PARAM_MESSAGE = "message";

//const uint16_t LED1_PIN = 13;
//const uint16_t LED2_PIN = 13;
float currentTemp = -99;
String currentTempStr = "-99";

const String baseHtml_begin = "<!DOCTYPE html><html lang=\"en\"><head><title>ESP LAN</title><meta name=\"viewport\" content=\"width=device-width, initial-scale=1, shrink-to-fit=no\"><link rel=\"stylesheet\" href=\"https://cdn.jsdelivr.net/npm/bootstrap@4.6.0/dist/css/bootstrap.min.css\" integrity=\"sha384-B0vP5xmATw1+K9KRQjQERJvTumQW0nPEzvF6L/Z6nronJ3oUOFUFpCjEUQouq2+l\" crossorigin=\"anonymous\"><head><body class=\"bg-dark text-light\"><div class=\"container bg-dark text-light\"><nav class=\"navbar navbar-expand-lg navbar-dark bg-dark\"><h2 class=\"navbar-brand\">ESP LAN</h2><button class=\"navbar-toggler\" type=\"button\" data-toggle=\"collapse\" data-target=\"#navbarNavAltMarkup\" aria-controls=\"navbarNavAltMarkup\" aria-expanded=\"false\" aria-label=\"Toggle navigation\"><span class=\"navbar-toggler-icon\"></span></button><div id=\"navbarNavAltMarkup\" class=\"collapse navbar-collapse\"><div class=\"navbar-nav\">\
    <a class=\"nav-link\" href=\"/\">Home</a></div></div></nav>";
const String baseHtml_end = "</div><script src=\"https://code.jquery.com/jquery-3.5.1.slim.min.js\" integrity=\"sha384-DfXdz2htPH0lsSSs5nCTpuj/zy4C+OGpamoFVy38MVBnE+IbbVYUew+OrCXaRkfj\" crossorigin=\"anonymous\"></script><script src=\"https://cdn.jsdelivr.net/npm/bootstrap@4.6.0/dist/js/bootstrap.bundle.min.js\" integrity=\"sha384-Piv4xVNRyMGpqkS2by6br4gNJ7DXjqk09RmUpJ8jgGtD7zP9yug3goQfGII0yAns\" crossorigin=\"anonymous\"></script></body>";


class CaptiveRequestHandler : public AsyncWebHandler {
public:
  CaptiveRequestHandler() {}
  virtual ~CaptiveRequestHandler() {}

  bool canHandle(AsyncWebServerRequest *request){
    //request->addInterestingHeader("ANY");
    return true;
  }

  void handleRequest(AsyncWebServerRequest *request) {
    request->send(200, "text/html", baseHtml_begin + "<h4>Current Temperature: " + currentTempStr + "</h4>" + baseHtml_end);
  }
};

// void notFound(AsyncWebServerRequest *request)
// {
//     request->send(404, "text/plain", "Not found");
// }

String ip;


void renderUi()
{
    gfx->write_clear();
    gfx->writeln("SSID: Thermometer");
    gfx->writeln(("IP: " + ip).c_str());
    gfx->writeln("---------------------");
    gfx->writeln(("Temperature C: " + currentTempStr).c_str());
}


void setup()
{
    //Serial.begin(9600);

    //pinMode(LED2_PIN, OUTPUT);

    //digitalWrite(LED2_PIN, HIGH);

    gfx->setup_lcd();

    //digitalWrite(LED2_PIN, LOW);

    // Init temp sensor
    gfx->writeln("Init sensors...");
    sensors.begin(); 


    
    gfx->writeln("Init WiFi...");
    WiFi.softAP("Thermometer");
    gfx->writeln("IP Address:");
    //ip = WiFi.localIP().toString();
    ip = WiFi.softAPIP().toString();
    gfx->writeln_s(&ip);

    gfx->writeln("Init DNS...");
    dnsServer.start(53, "*", WiFi.softAPIP());
    gfx->writeln("DNS started");


    gfx->writeln("Init HTTP server...");
    server.addHandler(new CaptiveRequestHandler()).setFilter(ON_AP_FILTER);//only when requested from AP

    // server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    //     request->send(200, "text/html", baseHtml_begin + "<h4>Current Temperature: " + *(new String(currentTemp)) + "</h4>" + baseHtml_end);
    // });
    //server.onNotFound(notFound);


    server.begin();
    gfx->writeln("Captive Portal Active");
    ////digitalWrite(LED1_PIN, LOW);
    //digitalWrite(LED1_PIN, HIGH);

    gfx->writeln("Startup Complete!");
    delay(2000);
}

void dns_wait_loop(u16_t wait_secs)
{
    for (u16_t i = 0; i < wait_secs; i++)
    {
        dnsServer.processNextRequest();
        delay(1000);
    }    
}

void loop()
{
    dns_wait_loop(5);

    // Main loop here...
        
    //Serial.print("Requesting temperatures..."); 
    sensors.requestTemperatures(); // Send the command to get temperature readings 
    //Serial.println("DONE"); 
    //Serial.print(sensors.getTempCByIndex(0)); // Why "byIndex"?  
    // You can have more than one DS18B20 on the same bus.  
    // 0 refers to the first IC on the wire 
    currentTemp = sensors.getTempCByIndex(0);
    currentTempStr.clear();
    currentTempStr.concat(currentTemp);

    renderUi();
    gfx->render();

}
