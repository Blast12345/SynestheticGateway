#pragma once

#include <algorithm>
#include <array>
#include <cstddef>
#include <esp_system.h>
#include "helpers/StringHelpers.h"

class MacAddress {
public:
    static constexpr size_t Octets = 6;

private:
    std::array<uint8_t, Octets> address;

public:
    explicit MacAddress(const std::array<uint8_t, Octets> &array) : address(array) {}

    explicit MacAddress(const uint8_t *bytes) {
        std::copy_n(bytes, Octets, address.begin());
    }

    static auto local() -> MacAddress {
        std::array<uint8_t, Octets> mac{};
        esp_read_mac(mac.data(), ESP_MAC_WIFI_STA);
        return MacAddress(mac);
    }

    constexpr auto toBytes() const noexcept -> const uint8_t * {
        return address.data();
    }

    auto toString() const -> String {
        std::array<String, Octets> parts;

        for (size_t i = 0; i < Octets; ++i) {
            const auto octet = address.at(i);
            parts.at(i) = StringHelpers::toHexString(octet);
        }

        return StringHelpers::join(parts, ":");
    }
};
