#include <WiFi.h>
#include "DebugSerial.h"
#include "clients/ClientNetwork.h"
#include "misc/Version.h"
#include "server/ServerConnection.h"

HardwareSerial &USBSerial = Serial;
ServerConnection *serverConnection;
ClientNetwork *clientNetwork;

// cppcheck-suppress unusedFunction
void setup() {
    DebugSerial.begin(DEBUG_BAUD_RATE);
    delay(WARMUP_DELAY_MS);
    DebugSerial.println("Debug baud rate set to: " + String(DEBUG_BAUD_RATE));

    serverConnection = new ServerConnection(USBSerial);

    WiFiClass::mode(WIFI_STA);

    clientNetwork = new ClientNetwork();

    DebugSerial.println("Setup complete.");

    // Router class
    serverConnection->onGatewayIdentificationRequest([](const JsonDocument &request) -> void {
        DebugSerial.println("Gateway identification request received.");

        JsonDocument body;
        body["mac-address"] = MacAddress::local().toString();
        body["firmware-version"] = kFirmwareVersion;

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

// cppcheck-suppress unusedFunction
void loop() {
    serverConnection->checkForIncomingMessages();
    // clients->checkForNewMessages();
}
