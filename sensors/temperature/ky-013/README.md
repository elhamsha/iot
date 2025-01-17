# ESP8266 MQTT Temperature Sensor (KY-013)

This project demonstrates how to use an ESP8266 microcontroller to read temperature data from a KY-013 sensor and publish the readings to an MQTT broker. 

## Features
- Reads temperature data from a KY-013 sensor using the ESP8266's analog pin.
- Converts the sensor's analog output into voltage and calculates the corresponding temperature.
- Publishes the temperature data to a specified MQTT topic.

## Prerequisites
- An ESP8266 board (e.g., NodeMCU, Wemos D1 Mini).
- KY-013 temperature sensor.
- Arduino IDE with ESP8266 board support installed.
- Access to an MQTT broker (e.g., [test.mosquitto.org](https://test.mosquitto.org)).

## Wiring Diagram
- **KY-013 Sensor Connections:**
  - **Signal (S)**: Connect to ESP8266 pin D4 (or your chosen analog pin).
  - **VCC**: Connect to 3.3V.
  - **GND**: Connect to GND.

## Code Explanation
1. **Wi-Fi Connection**: Connects the ESP8266 to a Wi-Fi network using the provided SSID and password.
2. **MQTT Connection**: Publishes temperature readings to the MQTT topic `"your/mqtt/topic"`.
3. **Temperature Calculation**:
   - Reads the KY-013 sensor's analog output.
   - Converts the analog value into voltage and then calculates the temperature using the formula:
     \[
     \text{Temperature (°C)} = (\text{Voltage} - 0.5) \times 100
     \]
   - This assumes a linear relationship between voltage and temperature.
4. **Publishing Data**: Sends the calculated temperature to the MQTT topic as a string.

## Usage
1. Clone this repository:
   ```bash
   git clone https://github.com/elhamsha/iot.git
