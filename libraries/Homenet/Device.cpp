#include "Device.h"

Device::Device(char* name, char* room, char* type) {
    _name = name;
    _room = room;
    _type = type;
}

bool Device::loop() {
    bool c = _changed;
    _changed = false;
    return c;
}

int Device::get_state() { return _state; }

int Device::get_value() { return _value; }

char* Device::get_name() {
    return _name;
}

char* Device::get_room() {
    return _room;
}

char* Device::get_type() {
    return _type;
}

long Device::get_wifi_strength() { return _wifi_strength; }

int Device::get_wifi_state() { return _wifi_state; }

void Device::set_wifi_state(int state) { 
    _changed = _wifi_state != state;
    _wifi_state = state;
}

void Device::set_state(int state) {
    _changed = _state != state;
    _state = state;
}

void Device::set_value(int value) {
    _changed = value != value;
    _value = value;
}

void Device::set_wifi_strength(long strength) {
    _changed = _changed || abs(_wifi_strength - strength) > 3;
    _wifi_strength = strength;
}

char* Device::to_json() {
    DynamicJsonBuffer jsonBuffer;
    char jsonMessageBuffer[1024];

    JsonObject& root = jsonBuffer.createObject();
    root["name"] = get_name();
    root["room"] = get_room();
    root["type"] = get_type();
    root["signal"] = get_wifi_strength();
    root["value"] = get_value();
    root["state"] = get_state();

    root.printTo(jsonMessageBuffer, sizeof(jsonMessageBuffer));
    return jsonMessageBuffer;
}