#include "Cmd.h"

Cmd Cmd::parse(JsonObject& json) {
    Cmd cmd = Cmd();

    cmd.name = json["name"];
    cmd.cmd = json["cmd"];
    cmd.value = json["value"];
    cmd.br = json["br"];
    cmd.r = json["r"];
    cmd.g = json["g"];
    cmd.b = json["b"];

    return cmd;
};