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
Light light = Light(rtc);
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

  //    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));

  FastLED.setBrightness(255);
  fill(CRGB::Green);
  FastLED.show();

  delay(300);

  light.set_brightness(255);
  light.set_rgb(255, 255, 0);
  light.set_wakeup_time(rtc.now().unixtime() + 17, 15);
  light.set_state(DEVICE_STATE_OFF);
}

void loop() {
  net.loop();

  light.set_wifi_state(net.get_state());
  light.set_wifi_strength(net.get_wifi_strength());

  bool state_changed = light.loop();

  if (state_changed) {
    apply_state();
    net.send(&light);
  }

  //  Serial.println(rtc.now().unixtime());
  //  delay(250);
}

void apply_state() {
  if (light.get_state() == DEVICE_STATE_OFF) {
    fill(CRGB::Black);
    FastLED.setBrightness(0);
  }

  if (light.get_state() == DEVICE_STATE_OK || light.get_state() == Light::DEVICE_STATE_WAKING_UP) {
    CRGB color = CRGB(light.get_r(), light.get_g(), light.get_b());
    fill(color);   
    FastLED.setBrightness(light.get_brightness());
  }

  FastLED.show();
}

void fill(CRGB color) {
  for (int i = 0; i < NUM_LEDS; i++) {
    _leds[i] = color;
  }
}

void on_cmd(Cmd cmd) {
  if (eq(cmd.cmd, CMD_STATE)) {
    light.invalidate();
    return;
  }

  if (eq(cmd.cmd, CMD_ON)) {
    light.set_state(DEVICE_STATE_OK);
    return;
  }

  if (eq(cmd.cmd, CMD_OFF)) {
    light.set_state(DEVICE_STATE_OFF);
    return;
  }

  if (eq(cmd.cmd, CMD_VALUE)) {
    light.set_state(DEVICE_STATE_OK);
    light.set_brightness(cmd.br);
    light.set_rgb(cmd.r, cmd.g, cmd.b);
    return;
  }

  if (eq(cmd.cmd, CMD_WAKEUP)) {
    light.set_wakeup_time(cmd.time_unix_s, cmd.delay_s);
    return;
  }
}

boolean eq(const char* a1, const char* a2) {
  return strcmp(a1, a2) == 0;
}
