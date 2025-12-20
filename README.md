# Synesthetic Gateway

The gateway is a component of the [Synesthetic Light Organ](https://github.com/Blast12345/Synesthetic). Its job is to
facilitate communication between the server
and the clients.

## Overview

The gateway receives colors from the server over the serial port. The colors are then routed to the clients using the
ESP-NOW protocol.

The gateway is necessary because the ESP-NOW protocol is only supported on ESP devices, so the server (i.e. a computer)
cannot communicate directly with the clients.

<img src="plantuml/overview.svg" alt="Overview">

#### Why not Wi-Fi?

_Note: I had Wi-Fi communication implemented previously, but I could only scale to a few clients before causing issues._

* Wi-Fi credentials must be manually updated on each client device, which is cumbersome.
* A poor connection and/or congested network can cause significant delays in the delivery of colors.
* Sending a high volume of small UDP packets congests the network, potentially causing disruptions to other devices.
* Having a dependency on Wi-Fi means the system is not as portable.

### Testing

Because the server sends data over the serial port, you cannot run the server and the serial monitor at the same time.
To test that the gateway is properly handling messages, I recommend sending them through command line.

Unix systems (Linux, macOS):

1. Open the serial monitor that corresponds to the gateway.
    1. The PlatformIO plugin in your IDE should provide this
       functionality.
2. Send a message to the gateway.
    1. Open a terminal.
    2. List all serial outputs: `ls /dev/cu*`
    3. Note the device that corresponds to the gateway (e.g., `/dev/cu.usbserial-0001`).
    4. Send a message: `echo "test1" > /dev/cu.usbserial-0001`
    5. Verify that the message appears in the serial monitor. This confirms that the gateway is receiving serial
       messages.
3. Verify that the client is receiving the message.
    1. Open the serial monitor for the client.
    2. Send a message to the gateway.
    3. Verify that the message appears in client's serial monitor. This confirms that the client is receiving ESP-NOW
       messages.

## To-do list

- Set up automated linting on PR
    - PIO Check
    - [CLion inspections](https://www.jetbrains.com/help/clion/command-line-code-inspector.html)
- Set up testing suite
- Set up automated testing on PR
