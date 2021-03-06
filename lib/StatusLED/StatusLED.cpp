/**
  MQTT433gateway - MQTT 433.92 MHz radio gateway utilizing ESPiLight
  Project home: https://github.com/puuu/MQTT433gateway/

  The MIT License (MIT)

  Copyright (c) 2018 Puuu

  Permission is hereby granted, free of charge, to any person
  obtaining a copy of this software and associated documentation files
  (the "Software"), to deal in the Software without restriction,
  including without limitation the rights to use, copy, modify, merge,
  publish, distribute, sublicense, and/or sell copies of the Software,
  and to permit persons to whom the Software is furnished to do so,
  subject to the following conditions:

  The above copyright notice and this permission notice shall be
  included in all copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
  NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
  BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
  ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
  CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.
*/

#include <Arduino.h>

#include <LED.h>

#include "StatusLED.h"

StatusLED::StatusLED(uint8_t led_pin, bool activeHigh)
    : led(nullptr), beatLED(nullptr), _state(normalOperation) {
  if (activeHigh) {
    led = new LED(led_pin, true);
  } else {
    led = new LEDOpenDrain(led_pin);
  }
  beatLED = new HeartbeatFlashing(*led);
};

StatusLED::~StatusLED() {
  delete beatLED;
  delete led;
}

void StatusLED::loop() {
  if (beatLED && getState() == normalOperation) {
    beatLED->loop();
  }
}

void StatusLED::setState(StatusLED::States state) {
  if (state == getState()) {
    return;
  }
  _state = state;
  switch (state) {
    case wifiConnect:
    case startup:
      if (beatLED) beatLED->flash(500);
      break;
    case wifimanager:
      if (beatLED) beatLED->flash(100);
      break;
    case requireConfiguration:
      if (beatLED) beatLED->flash(1000);
      break;
    case normalOperation:
      // heartbeat with loop()
      if (beatLED) beatLED->off();
      break;
    case ota:
      if (beatLED) beatLED->on();
      break;
  }
}
