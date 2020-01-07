#ifndef CMD
#define CMD "cmd"

#define CMD_STATE "state"
#define CMD_ON "on"
#define CMD_OFF "off"
#define CMD_RESET "reset"
#define CMD_PAUSE "pause"
#define CMD_VALUE "value"
#define CMD_WAKEUP "wakeup"

#include <ArduinoJson.h>

class Cmd {
   public:
    static Cmd parse(JsonObject& json);

    const char* name;
    const char* cmd;
    int value;
    int r;
    int g;
    int b;
    int br;
    uint32_t time_unix_s;
    uint32_t delay_s;
};

#endif