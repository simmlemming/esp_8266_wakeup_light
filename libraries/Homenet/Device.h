#ifndef STATE
#define STATE "state"

#include <ArduinoJson.h>

#define DEVICE_STATE_OFF 0
#define DEVICE_STATE_OK 1
#define DEVICE_STATE_INIT 2
#define DEVICE_STATE_ERROR 3
#define DEVICE_STATE_ALARM 4
#define DEVICE_STATE_ALARM_PENDING 5
#define DEVICE_STATE_PAUSED 6

class Device {
   private:
    char* _name;
    char* _room;
    char* _type;
    long _wifi_strength;
    int _wifi_state;
    int _state;
    volatile int _value;

   protected:
    bool _changed;
    virtual void _add_state(JsonObject& root);

   public:
    Device(char* name, char* room, char* type);
    bool loop();
    void invalidate();

    char* to_json();

    void set_wifi_strength(long strength);
    void set_state(int state);
    void set_value(int value);
    void set_wifi_state(int state);

    long get_wifi_strength();
    int get_state();
    int get_value();
    char* get_name();
    char* get_room();
    char* get_type();
    int get_wifi_state();
};

#endif