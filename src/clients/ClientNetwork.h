#pragma once

#include <esp_now.h>
#include "DebugSerial.h"
#include "clients/BroadcastPeer.h"
#include "clients/Peer.h"
#include "helpers/StringHelpers.h"
#include "misc/MacAddress.h"

class ClientNetwork {
    BroadcastPeer broadcastPeer;

public:
    ClientNetwork() {
        initializeEspNow();
        registerDataSentCallback();
        addPeer(broadcastPeer);
    }

    void broadcast(const String &message) const {
        const auto *data = StringHelpers::toBytes(message);
        const size_t len = message.length();

        const MacAddress macAddress = broadcastPeer.getMacAddress();
        const esp_err_t result = esp_now_send(macAddress.toBytes(), data, len);

        if (result == ESP_OK) {
            DebugSerial.println("Successfully broadcast: " + message);
        } else {
            const char *errorMessage = esp_err_to_name(result);
            DebugSerial.print("Failed to send message: " + String(errorMessage));
        }
    }

private:
    static void initializeEspNow() {
        const esp_err_t result = esp_now_init();

        if (result == ESP_OK) {
            DebugSerial.println("ESP-NOW initialized successfully.");
        } else {
            const char *errorMessage = esp_err_to_name(result);
            DebugSerial.println("Error initializing ESP-NOW: " + String(errorMessage));
        }
    }

    static void registerDataSentCallback() {
        esp_now_register_send_cb(OnDataSent);
    }

    static void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
        const MacAddress macAddress(mac_addr);

        if (status == ESP_NOW_SEND_SUCCESS) {
            DebugSerial.println("Data sent successfully to " + macAddress.toString());
        } else {
            DebugSerial.println("Data send failed to " + macAddress.toString());
        }
    }

    static void addPeer(const Peer peer) {
        const esp_now_peer_info_t peerInfo = peer.getInfo();
        const esp_err_t result = esp_now_add_peer(&peerInfo);

        if (result == ESP_OK) {
            DebugSerial.println("Peer added successfully");
        } else {
            const char *errorMessage = esp_err_to_name(result);
            DebugSerial.println("Failed to add peer: " + String(errorMessage));
        }
    }

};
