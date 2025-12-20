#include <optional>
#include <WiFi.h>
#include "clients/ClientNetwork.h"
#include "server/ServerConnection.h"

constexpr unsigned DEBUG_DELAY = 5000;
constexpr unsigned WIFI_WARMUP = 1000;

// ServerSerial and DebugSerial
ServerConnection *serverConnection;
ClientNetwork *clientNetwork;

void setup() {
#ifdef DEBUG
    delay(DEBUG_DELAY);
#endif

    Serial.begin(BAUD_RATE);
    Serial.println("Baud rate set to: " + String(BAUD_RATE));
    serverConnection = new ServerConnection(Serial);

    WiFi.mode(WIFI_STA); // NOLINT

    /*
     * TODO: ESP-NOW will init successfully without this, but the peer fails to add without.
     * The delay also must be directly in the setup method; including it in the constructor does not seem to work.
     * This is very strange and my research has not yielded any results. Perhaps this is a bug in the ESP-NOW library?
    */
    delay(WIFI_WARMUP);

    clientNetwork = new ClientNetwork();

    Serial.println("Setup complete.");

    // Router class
    serverConnection->onGatewayIdentificationRequest([]() -> void {
        JsonDocument json;
        json["message-type"] = "gateway-identification-response";
        json["message-sender"] = "gateway";
        json["message-recipient"] = "server";

        serverConnection->sendMessage(json);
    });
}

void loop() {
    serverConnection->checkForIncomingMessages();
    // clients->checkForNewMessages();
}
