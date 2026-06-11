#pragma once

#include <Arduino.h>
#include <ArduinoJson.h>
#include <functional>
#include "DebugSerial.h"

using std::optional;
using std::nullopt;

class ServerConnection {
    HardwareSerial *serial;
    std::function<void(const JsonDocument &)> gatewayIdentificationRequestHandler;
    std::function<void(const JsonDocument &)> broadcastColorHandler;

public:
    explicit ServerConnection(HardwareSerial &serial) : serial(&serial) {
        serial.begin(USB_BAUD_RATE);
    }

    void checkForIncomingMessages() const {
        // Serial.println("Checking for incoming messages...");

        auto nextJson = checkForJson();

        if (!nextJson) {
            return;
        }

        const String method = nextJson.value()["method"];

        if (method == "gateway-identification-request" && gatewayIdentificationRequestHandler) {
            gatewayIdentificationRequestHandler(nextJson.value());
        }

        if (method == "broadcast-color" && broadcastColorHandler) {
            broadcastColorHandler(nextJson.value());
        }
    }

    void sendMessage(const JsonDocument &json) const {
        serializeJson(json, *serial);
        serial->write('\n');

        DebugSerial.println("Serial Write: " + json.as<String>());
    }

    void onGatewayIdentificationRequest(const std::function<void(const JsonDocument &)> &handler) {
        gatewayIdentificationRequestHandler = handler;
    }

    void onBroadcastColor(const std::function<void(const JsonDocument &)> &handler) {
        broadcastColorHandler = handler;
    }

private:
    auto checkForJson() const -> optional<JsonDocument> {
        auto nextLine = checkForNextLine();

        if (!nextLine) {
            return nullopt;
        }

        DebugSerial.println("Serial Receive: " + nextLine.value());

        JsonDocument json;
        deserializeJson(json, *nextLine);

        return json;
    }

    auto checkForNextLine() const -> optional<String> {
        if (serial->available() == 0) {
            return nullopt;
        }

        DebugSerial.println("Data available...");

        return serial->readStringUntil('\n');
    }

};
