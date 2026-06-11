#pragma once

#include <Arduino.h>

#ifndef FIRMWARE_VERSION
#define FIRMWARE_VERSION "dev" // NOLINT(cppcoreguidelines-macro-usage)
#endif

constexpr auto kFirmwareVersion = FIRMWARE_VERSION;

#undef FIRMWARE_VERSION