#include <WiFi.h>

#include "DebugSerial.h"
#include "clients/ClientNetwork.h"
#include "misc/Version.h"
#include "server/ServerConnection.h"

constexpr unsigned DEBUG_DELAY = 5000;
constexpr unsigned WIFI_WARMUP = 1000;

HardwareSerial &USBSerial = Serial;
ServerConnection *serverConnection;
ClientNetwork *clientNetwork;

void setup() {
#ifdef DEBUG
    delay(DEBUG_DELAY);
#endif

    // TODO: Update readme with debug information
    DebugSerial.begin(DEBUG_BAUD_RATE);
    DebugSerial.println("Baud rate set to: " + String(DEBUG_BAUD_RATE));

    serverConnection = new ServerConnection(USBSerial);

    WiFi.mode(WIFI_STA); // NOLINT

    /*
     * TODO: ESP-NOW will init successfully without this, but the peer fails to add without.
     * The delay also must be directly in the setup method; including it in the constructor does not seem to work.
     * This is very strange and my research has not yielded any results. Perhaps this is a bug in the ESP-NOW library?
    */
    delay(WIFI_WARMUP);

    clientNetwork = new ClientNetwork();

    DebugSerial.println("Setup complete.");

    // Router class
    serverConnection->onGatewayIdentificationRequest([](const JsonDocument &request) -> void {
        DebugSerial.println("Gateway identification request received.");

        JsonDocument body;
        body["mac-address"] = MacAddress().toString();
        body["firmware-version"] = Version::toString();

        JsonDocument response;

        response["jsonrpc"] = "2.0";
        response["id"] = request["id"];
        response["result"] = body;

        serverConnection->sendMessage(response);
    });

    serverConnection->onBroadcastColor([](const JsonDocument &request) -> void {
        DebugSerial.println("Broadcast color received.");

        String message;
        serializeJson(request, message);

        clientNetwork->broadcast(message);
    });
}

void loop() {
    serverConnection->checkForIncomingMessages();
    // clients->checkForNewMessages();
}
