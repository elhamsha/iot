# ESP8266 MQTT LED Control

This project demonstrates how to use an ESP8266 microcontroller to connect to a Wi-Fi network and an MQTT broker to control an LED remotely. It showcases a simple IoT application for MQTT communication.

## Features
- Connects to a Wi-Fi network.
- Subscribes to an MQTT topic to receive messages.
- Controls an LED based on MQTT messages (`ON`/`OFF` commands).

## Prerequisites
- An ESP8266 board (e.g., NodeMCU, Wemos D1 Mini).
- Arduino IDE installed with ESP8266 board support.
- Access to an MQTT broker (e.g., [test.mosquitto.org](https://test.mosquitto.org)).

## Wiring Diagram
- Connect an LED to GPIO pin D4 (with a suitable resistor, e.g., 220Ω).

## Code Explanation
The `esp8266_mqtt_led.ino` file:
1. Connects to Wi-Fi using the provided credentials.
2. Subscribes to the MQTT topic `IoTJump_topic`.
3. Turns the LED **on** when the message `"ON"` is received.
4. Turns the LED **off** when the message `"OFF"` is received.

## Usage
1. Clone this repository:
   ```bash
   git clone https://github.com/elhamsha/iot.git

