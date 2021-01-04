#include <Device.h>
#include <RTC_DS3231_DST.h>

class Light : public Device {

  public:
    static const int DEVICE_STATE_WAKING_UP = 9;

    Light(RTC_DS3231_DST rtc) : Device("wl01", "bb", "wl") {
      _rtc = rtc;
    }

    bool loop() {
      if (_target_time_ms == 0) {
        return Device::loop();
      }

      uint32_t n = _rtc.now().unixtime();
      byte br_pct = (n - (_target_time_ms - _target_delay_ms)) * 100 / _target_delay_ms;
      br_pct = constrain(br_pct, 0, 100);

      //            Serial.print("target = ");
      //            Serial.println(_target_time_ms);
      //            Serial.print("state = ");
      //            Serial.println(get_state());
      //            Serial.print("br_pct = ");
      //      Serial.println(br_pct);
      //      Serial.println("   ");

      if (get_state() == DEVICE_STATE_OFF && br_pct > 0) {
        set_state(DEVICE_STATE_WAKING_UP);
      }

      if (get_state() == DEVICE_STATE_WAKING_UP) {
        byte a = _get_a(_target_value);
        byte r = _get_r(_target_value);
        byte g = _get_g(_target_value);
        byte b = _get_b(_target_value);

        byte a_pct = a * br_pct / 100;

        uint32_t new_value = _set_argb(a_pct, r, g, b);
        set_value(new_value);
      }

      if (get_state() == DEVICE_STATE_WAKING_UP && br_pct >= 100) {
        _target_time_ms = 0;
        set_state(DEVICE_STATE_OK);
      }

      return Device::loop();
    }

    byte get_brightness() {
      return _get_a(get_value());
    }

    byte get_r() {
      return _get_r(get_value());
    }

    byte get_g() {
      return _get_g(get_value());
    }

    byte get_b() {
      return _get_b(get_value());
    }

    void set_rgb(byte r, byte g, byte b) {
      uint32_t new_value = _set_argb(get_brightness(), r, g, b);
      set_value(new_value);
    }

    void set_brightness(byte brightness) {
      byte br = constrain(brightness, 0, 255);
      uint32_t new_value = _set_argb(br, get_r(), get_g(), get_b());
      set_value(new_value);
    }

    uint32_t _set_argb(byte a, byte r, byte g, byte b) {
      uint32_t new_value = a;
      new_value = new_value << 8;

      new_value = new_value + r;
      new_value = new_value << 8;

      new_value = new_value + g;
      new_value = new_value << 8;

      new_value = new_value + b;

      return new_value;
    }

    void set_wakeup_time(uint32_t time_ms, uint32_t delay_ms) {
      _target_time_ms = time_ms;
      _target_delay_ms = delay_ms;
      _target_value = get_value();

      _changed = true;
    }

    void _add_state(JsonObject& root) {
//      root["time"] = _rtc.now().unixtime();
      //      root["r"] = _rgb[0];
      //      root["g"] = _rgb[1];
      //      root["b"] = _rgb[2];
      //      root["wakeup_br"] = _target_br;
      //      root["wakeup_r"] = _target_rgb[0];
      //      root["wakeup_g"] = _target_rgb[1];
      //      root["wakeup_b"] = _target_rgb[2];
      //      root["wakeup_g"] = _target_rgb[1];
      //      root["wakeup_b"] = _target_rgb[2];
      //      root["wakeup_time_ms"] = _target_time_ms;
      //      root["wakeup_delay_ms"] = _target_delay_ms;
    }

    byte _get_a(uint32_t value) {
      return (byte)((value & 0xff000000) >> 24);
    }

    byte _get_r(uint32_t value) {
      return (byte)((value & 0x00ff0000) >> 16);
    }

    byte _get_g(uint32_t value) {
      return (byte)((value & 0x0000ff00) >> 8);
    }

    byte _get_b(uint32_t value) {
      return (byte)(value & 0x000000ff);
    }

  private:
    RTC_DS3231_DST _rtc;

    uint32_t _target_value = 0;
    uint32_t _target_time_ms = 0;
    uint32_t _target_delay_ms = 0;
};
