#include <ESP8266WiFi.h>
#include "curtain_server.h"
#include "curtain_stepper.h"

cStepper curtains(5, 4, 2);
cServer server(80, &curtains);

void setup() {
  Serial.begin(115200);
  delay(100);
  server.begin();
}

void loop() {
  server.handle();
  curtains.update();
}
