#include <Device.h>

class Light : public Device {

  public:
    Light() : Device("wakeup_light_01", "brown_bedroom", "wakeup_light") { }


    int get_brightness() {
      return _brightness;
    }

    void set_brightness(int brightness) {
      _changed = _brightness != brightness;
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
    _rgb[0] = r;
    _rgb[1] = g;
    _rgb[2] = b;

    _changed = true;
  }
  
  private:
    int _brightness = 0;
    int _rgb[3];
};
