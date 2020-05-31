#include <ESP8266WiFi.h>
#include "Arduino.h"
#include "curtain_server.h"
#include "curtain_led.h"

cServer::cServer(int http_port, cStepper* stpr)
    : _led(0), _server(http_port) {
        _stepper = stpr;
    }

void cServer::begin() {
    const char* ssid = "";
    const char* password = "";

    Serial.print("\n\nConnecting to ");
    Serial.println(ssid);

    WiFi.begin(ssid, password);
  
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
        _led.toggle();
    }

    Serial.println("\nWiFi connected");  
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
    _server.begin();
}

void cServer::send200(WiFiClient client, String body) {
    client.print("HTTP/1.1 200 OK\r\n");
    client.print("Access-Control-Allow-Origin: *\r\n");
    client.print("\r\n");
    client.println(body);
}

void cServer::send404(WiFiClient client) {
  client.print("HTTP/1.1 404 Not Found\r\n");
  client.print("Access-Control-Allow-Origin: *\r\n");
  client.print("\r\n");
}

void cServer::handle(){
    WiFiClient client = _server.available();
    if (client) {
        if (client.connected()) {
            Serial.println("Connected to client");
            while (!client.available());
            String line = client.readStringUntil('\r');
            Serial.println(line);
    
            while (client.available()) client.read();

            String path = line.substring(
                line.indexOf(' ') + 1,
                line.lastIndexOf(' ')
            );
            String major_path = path, minor_path = "";
            if (path.indexOf('/', 1) != -1) {
                major_path = path.substring(
                    0,
                    path.indexOf('/', 1)
                );
                minor_path = path.substring(
                    path.indexOf('/', 1)
                );
            }
            Serial.print("Major path: ");
            Serial.println(major_path);
            Serial.print("Minor path: ");
            Serial.println(minor_path);

            if (path.equals("/on")){
                _led.turnOn();
                send200(client, "LED is on!");
            } else if (path.equals("/off")){
                _led.turnOff();
                send200(client, "LED is off!");
            } else if (path.equals("/toggle")){
                _led.toggle();
                String statusText = _led.status == LOW ? "ON" : "OFF";
                String msg = "LED toggled " + statusText;
                send200(client, msg);
            } else if (major_path.equals("/rotate")) {
                float deg = minor_path.substring(1).toFloat();
                if (deg != 0) _stepper->rotateDeg(deg, 0.3);
                send200(client, "Rotated");
            } else if (major_path.equals("/setTarget")) {
                int target = minor_path.substring(1).toInt();
                target = _stepper->setTargetPos(target);
                send200(client, "Targeting " + String(target));
            } else if (major_path.equals("/setPercentage")) {
                int target = minor_path.substring(1).toInt();
                int min = _stepper->_min_pos;
                int max = _stepper->_max_pos;
                target = (max - min) * target / 100 + min;
                target = _stepper->setTargetPos(target);
                send200(client, "Targeting " + String(target));
            } else if (path.equals("/close")) {
                _stepper->_target_pos = _stepper->_max_pos;
                send200(client, "Closing the curtains");
            } else if (path.equals("/open")) {
                _stepper->_target_pos = _stepper->_min_pos;
                send200(client, "Opening the curtains");
            } else if (path.equals("/stop")) {
                int pos = _stepper->_cur_pos;
                _stepper->_target_pos = pos;
                send200(client, "Stopping the curtains at position " + String(pos));
            } else if (major_path.equals("/setMax")) {
                int max = minor_path.substring(1).toInt();
                _stepper->_max_pos = max;
                send200(client, "Setting the maximum position to " + String(max));
            } else if (major_path.equals("/setMin")) {
                int min = minor_path.substring(1).toInt();
                _stepper->_min_pos = min;
                send200(client, "Setting the minimum position to " + String(min));
            } else if (path.equals("/setThisMax")) {
                int max = _stepper->_cur_pos;
                _stepper->_max_pos = max;
                send200(client, "Setting the maximum position to " + String(max));
            } else if (path.equals("/setThisMin")) {
                int min = _stepper->_cur_pos;
                _stepper->_min_pos = min;
                send200(client, "Setting the minimum position to " + String(min));
            } else if (major_path.equals("/setSpeed")) {
                int spd = minor_path.substring(1).toInt();
                spd = _stepper->setSpeed(spd);
                send200(client, "Setting the speed to " + String(spd));
            } else if (path.equals("/getSpeed")) {
                int spd = _stepper->getSpeed();
                send200(client, String(spd));
            } else if (path.equals("/getPos")) {
                int pos = _stepper->_cur_pos;
                send200(client, String(pos));
            } else if (path.equals("/getMax")) {
                int max = _stepper->_max_pos;
                send200(client, String(max));
            } else if (path.equals("/getMin")) {
                int min = _stepper->_min_pos;
                send200(client, String(min));
            } else if (path.equals("/getAll")) {
                String resp = String(_stepper->_cur_pos) + ",";
                resp += String(_stepper->_min_pos) + ",";
                resp += String(_stepper->_max_pos) + ",";
                resp += String(_stepper->getSpeed());
                send200(client, resp);
            } else if (path.equals("/enable")) {
                _stepper->enable();
                send200(client, "Stepper enabled");
            } else if (path.equals("/disable")) {
                _stepper->disable();
                send200(client, "Stepper disabled");
            } else {
                send404(client);
            }
            Serial.println("Response sent");
        }
        client.stop();
        Serial.println("client stopped");
    }
}
