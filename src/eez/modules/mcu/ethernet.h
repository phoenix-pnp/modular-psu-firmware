/*
 * EEZ Middleware
 * Copyright (C) 2015-present, Envox d.o.o.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

namespace eez {
namespace mcu {
namespace ethernet {

bool onSystemStateChanged();

class IPAddress {
private:
    union {
        uint8_t bytes[4]; // IPv4 address
        uint32_t dword;
    } _address;

public:
    operator uint32_t() const {
        return _address.dword;
    };
};

/// Arduino Ethernet object simulator
class EthernetModule {
  public:
    bool begin(uint8_t *mac, uint8_t *ipAddress = 0, uint8_t *dns = 0, uint8_t *gateway = 0,
               uint8_t *subnetMask = 0);
    uint8_t maintain();

    IPAddress localIP();
    IPAddress subnetMask();
    IPAddress gatewayIP();
    IPAddress dnsServerIP();
};

extern EthernetModule Ethernet;

class EthernetClient {
  public:
    EthernetClient();
    EthernetClient(bool valid);

    operator bool();
    bool operator==(EthernetClient &other) {
        return true;
    }

    bool connected();

    size_t available();
    size_t read(uint8_t *, size_t);
    size_t write(const char *data, size_t len);
    void flush();

    void stop();

  private:
    bool valid;
};

class EthernetServer {
  public:
    void init(int port);

    void begin();
    EthernetClient available();

  private:
    bool bind_result;
    int port;
    EthernetClient client;
};


class EthernetUDP {
public:
    uint8_t begin(uint16_t port);
    void stop();
    int beginPacket(const char *host, uint16_t port);
    size_t write(const uint8_t *buffer, size_t size);
    int endPacket();
    int read(unsigned char* buffer, size_t len);
    int parsePacket();
};
}
}
} // namespace eez::mcu::ethernet