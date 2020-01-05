#ifndef CMD
#define CMD "cmd"

#define CMD_STATE "state"
#define CMD_ON "on"
#define CMD_OFF "off"
#define CMD_RESET "reset"
#define CMD_PAUSE "pause"
#define CMD_VALUE "value"

#include <ArduinoJson.h>

class Cmd {
   public:
    static Cmd parse(JsonObject& json);

    const char* name;
    const char* cmd;
    int value;
};

#endif