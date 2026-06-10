#pragma once

#ifndef FIRMWARE_VERSION
#define FIRMWARE_VERSION "dev"
#endif

class Version {
public:
    static auto toString() -> String {
        return FIRMWARE_VERSION;
    }
};
