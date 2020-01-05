#include "Cmd.h"

Cmd Cmd::parse(JsonObject& json) {
    Cmd cmd = Cmd();

    cmd.name = json["name"];
    cmd.cmd = json["cmd"];
    cmd.value = json["value"];

    return cmd;
};