#include <Device.h>
#include <RTC_DS3231_DST.h>

class Light : public Device {

  public:
    static const int DEVICE_STATE_WAKING_UP = 999;

    Light(RTC_DS3231_DST rtc) : Device("wakeup_light_01", "brown_bedroom", "wakeup_light") {
      _rtc = rtc;
    }

    bool loop() {
      if (_target_time_ms == 0) {
        return Device::loop();
      }

      uint32_t n = _rtc.now().unixtime();
      int br_pct = ((int)(n - (_target_time_ms - _target_delay_ms))) * 100 / (int)_target_delay_ms;
      br_pct = constrain(br_pct, 0, 100);

      if (get_state() == DEVICE_STATE_OFF && br_pct > 0) {
        set_state(DEVICE_STATE_WAKING_UP);
      }

      if (get_state() == DEVICE_STATE_WAKING_UP) {
        set_rgb(_target_rgb[0], _target_rgb[1], _target_rgb[2]);
        set_brightness(_target_br * br_pct / 100);
      }

      if (get_state() == DEVICE_STATE_WAKING_UP && br_pct >= 100) {
        _target_time_ms = 0;
        set_state(DEVICE_STATE_OK);
      }

      return Device::loop();
    }

    int get_brightness() {
      return _brightness;
    }

    void set_brightness(int brightness) {
      _changed = _changed || (_brightness != brightness);
      _brightness = brightness;
    }

    int get_r() {
      return _rgb[0];
    }

    int get_g() {
      return _rgb[1];
    }

    int get_b() {
      return _rgb[2];
    }

    void set_rgb(int r, int g, int b) {
      _changed = _changed || (_rgb[0] != r);
      _changed = _changed || (_rgb[1] != g);
      _changed = _changed || (_rgb[2] != b);

      _rgb[0] = r;
      _rgb[1] = g;
      _rgb[2] = b;
    }

    void set_target(int br, int r, int g, int b, uint32_t time_ms, uint32_t delay_ms) {
      _target_br = br;
      _target_rgb[0] = r;
      _target_rgb[1] = g;
      _target_rgb[2] = b;
      _target_time_ms = time_ms;
      _target_delay_ms = delay_ms;
    }

    void _add_state(JsonObject& root) {
      root["br"] = _brightness;
      root["r"] = _rgb[0];
      root["g"] = _rgb[1];
      root["b"] = _rgb[2];
    }

  private:
    RTC_DS3231_DST _rtc;

    int _target_br = 0;
    int _target_rgb[3];
    uint32_t _target_time_ms = 0;
    uint32_t _target_delay_ms = 0;

    int _brightness = 0;
    int _rgb[3];
};
