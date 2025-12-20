#pragma once

#include <Arduino.h>
#include <ArduinoJson.h>
#include <functional>

using std::optional;
using std::nullopt;

// TODO: Document
// JSON
// message-type
// message-sender
// message-recipient
// r, g, b

class ServerConnection {
    HardwareSerial *serial;
    std::function<void()> gatewayIdentificationRequestHandler;

public:
    explicit ServerConnection(HardwareSerial &serial) : serial(&serial) {}

    void checkForIncomingMessages() const {
        auto nextJson = checkForJson();

        if (!nextJson) {
            return;
        }

        // Serial.println("Message received from server: " + nextJson.value().as<String>());

        const String messageType = nextJson.value()["message-type"];

        if (messageType == "gateway-identification-request" && gatewayIdentificationRequestHandler) {
            gatewayIdentificationRequestHandler();
        }
    }

    void sendMessage(const JsonDocument &json) const {
        serializeJson(json, *serial);
        serial->write('\n');

        // Serial.println("Message sent to server: " + json.as<String>());
    }

    void onGatewayIdentificationRequest(const std::function<void()> &handler) {
        gatewayIdentificationRequestHandler = handler;
    }

private:
    auto checkForJson() const -> optional<JsonDocument> {
        auto nextLine = checkForNextLine();

        if (!nextLine) {
            return nullopt;
        }

        JsonDocument json;
        deserializeJson(json, *nextLine);

        return json;
    }

    auto checkForNextLine() const -> optional<String> {
        if (serial->available() == 0) {
            return nullopt;
        }

        return serial->readStringUntil('\n');
    }

};
