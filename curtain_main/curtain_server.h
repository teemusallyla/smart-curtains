#ifndef curtain_server
#define curtain_server

#include <ESP8266WiFi.h>
#include "Arduino.h"
#include "curtain_stepper.h"
#include "curtain_led.h"

class cServer {
    public:
        cServer(int http_port, cStepper* stpr);
        void begin();
        void handle();
    private:
        void send200(WiFiClient client, String body);
        void send404(WiFiClient client);
        WiFiServer _server;
        Led _led;
        cStepper* _stepper;
};

#endif
