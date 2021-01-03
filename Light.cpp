#include <Device.h>
#include <RTC_DS3231_DST.h>

class Light : public Device {

  public:
    static const int DEVICE_STATE_WAKING_UP = 9;

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

//      Serial.print("target = ");
//      Serial.println(_target_time_ms);
//      Serial.print("state = ");
//      Serial.println(get_state());
//      Serial.print("br_pct = ");
//      Serial.println(br_pct);
//      Serial.println("   ");

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

    byte get_brightness() {
      return (byte)((get_value() & 0xff000000) >> 24);
    }

    byte get_r() {
      return (byte)((get_value() & 0x00ff0000) >> 16);
    }

    byte get_g() {
      return (byte)((get_value() & 0x0000ff00) >> 8);
    }

    byte get_b() {
      return (byte)(get_value() & 0x000000ff);
    }

    void set_rgb(byte r, byte g, byte b) {
      _set_br_rgb(get_brightness(), r, g, b);
    }

    void set_brightness(byte brightness) {
      byte br = constrain(brightness, 0, 255);
      _set_br_rgb(br, get_r(), get_g(), get_b());
    }

    void _set_br_rgb(byte br, byte r, byte g, byte b) {
      int new_value = br;
      new_value = new_value << 8;

      new_value = new_value + r;
      new_value = new_value << 8;

      new_value = new_value + g;
      new_value = new_value << 8;

      new_value = new_value + b;

      set_value(new_value);
    }

    void set_wakeup_time(uint32_t time_ms, uint32_t delay_ms) {
      _target_time_ms = time_ms;
      _target_delay_ms = delay_ms;
      _target_rgb[0] = get_r();
      _target_rgb[1] = get_g();
      _target_rgb[2] = get_b();
      _target_br = get_brightness();
      _changed = true;
    }

    void _add_state(JsonObject& root) {
      //      root["time_unix_s"] = _rtc.now().unixtime();
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

  private:
    RTC_DS3231_DST _rtc;

    byte _target_br = 0;
    byte _target_rgb[3];
    uint32_t _target_time_ms = 0;
    uint32_t _target_delay_ms = 0;
};
