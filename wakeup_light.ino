#define LED D5
#define FASTLED_ESP8266_RAW_PIN_ORDER

#include <FastLED.h>
#include <Wire.h>
#include <RTC_DS3231_DST.h>

#include "Light.cpp"
#include <Homenet.h>

#define LED_TYPE WS2812B
#define COLOR_ORDER GRB
#define NUM_LEDS 58
#define LED_PIN D8

RTC_DS3231_DST rtc;
Light light = Light();
Homenet net = Homenet(light.get_name());

CRGB _leds[NUM_LEDS];

int c = 0;
int s = 8;
CRGB colors[] = {CRGB::White, CRGB::Red, CRGB::Yellow, CRGB::Green, CRGB::Purple, CRGB::Cyan, CRGB::Orange, CRGB::Blue};


void setup() {
  Serial.begin(115200);
  Wire.begin();

  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(_leds, NUM_LEDS).setCorrection(TypicalLEDStrip).setDither(0);

  net.setup(on_cmd);
  
//  rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));

  FastLED.setBrightness(255);
  fill(CRGB::Green);
  FastLED.show();
  
  delay(200);

  light.set_brightness(255);
  light.set_rgb(255, 0, 255);
}

void loop() {
  net.loop();

  light.set_wifi_state(net.get_state());
  light.set_wifi_strength(net.get_wifi_strength());

  bool state_changed = light.loop();

  if (state_changed) {
    apply_state();
    net.send(light);
  }
}

void apply_state() {
  CRGB color = CRGB(light.get_r(), light.get_g(), light.get_b());
  
  fill(color);
  FastLED.setBrightness(light.get_brightness());

  FastLED.show();
}

void fill(CRGB color) {
  for (int i = 0; i < NUM_LEDS; i++) {
    _leds[i] = color;
  }
}

void on_cmd(Cmd cmd) {
  
}