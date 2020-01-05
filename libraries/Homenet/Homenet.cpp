#include "Homenet.h"
#include "Const.h"

#define DEBUG true

Homenet* Homenet::instance = 0;

Homenet::Homenet(char* mqtt_device_name) {
    Homenet::instance = this;
    
    _wifi = WiFiClient();
    _mqtt = PubSubClient(_wifi);
    _wifi_connecting = false;
    _mqtt_device_name = mqtt_device_name;
}

void Homenet::setup(void (*on_cmd)(Cmd cmd)) {
    _on_cmd = on_cmd;
    _mqtt.setServer(mqtt_server, 1883);
    _mqtt.setCallback(Homenet::_on_new_message_handler);
}

void Homenet::_on_new_message_handler(char* topic, uint8_t* payload, unsigned int length) {
    instance->_on_new_message(topic, payload, length);
}

void Homenet::_on_new_message(char* topic, uint8_t* payload, unsigned int length) {
    DynamicJsonBuffer jsonBuffer;
    JsonObject& message = jsonBuffer.parseObject(payload);

    if (!message.success()) {
        _mqtt.publish(debugTopic, _mqtt_device_name);
        _mqtt.publish(debugTopic, "cannot parse message");
#if DEBUG
        Serial.println("cannot parse message");
#endif
        return;
    }

#if DEBUG
    char jsonMessageBuffer[1024];
    message.printTo(jsonMessageBuffer, sizeof(jsonMessageBuffer));
    Serial.print("<-- ");
    Serial.println(jsonMessageBuffer);
#endif

    Cmd cmd = Cmd::parse(message);

    if (strcmp(cmd.name, _mqtt_device_name) != 0 && strcmp(cmd.name, "all") != 0) {
        return;
    }
    
    _on_cmd(cmd);
}

void Homenet::loop() {
    _setup_wifi();
    if (WiFi.status() == WL_CONNECTED) {
        _setup_mqtt();
    }

    _mqtt.loop();
}

void Homenet::send(Device device) {
    char* json = device.to_json();
    _mqtt.publish(outTopic, json);

#if DEBUG
    Serial.print("--> ");
    Serial.println(json);
#endif
}

bool Homenet::_setup_wifi() {
    bool connected = WiFi.status() == WL_CONNECTED;
    bool error = WiFi.status() == WL_CONNECT_FAILED;

    if (connected || error) {
        _wifi_connecting = false;
    }

    if (connected) {
        return true;
    }

    if (!_wifi_connecting) {
        _wifi_connecting = true;
        WiFi.begin(ssid, ssid_password);
    }

    return false;
}

void Homenet::_setup_mqtt() {
    bool connected = _mqtt.connected();

    if (connected) {
        return;
    }

    if (_mqtt.connect(_mqtt_device_name)) {
        _mqtt.subscribe(inTopic);
    }
}

int Homenet::get_state() {
    if (_mqtt.connected()) {
        return STATE_CONNECTED;
    }

    if (_wifi_connecting) {
        return STATE_CONNECTING;
    }

    return STATE_UNKNOWN;
}

long Homenet::get_wifi_strength() {
    return WiFi.RSSI();
}