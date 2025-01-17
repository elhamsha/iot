# ESP8266 MQTT Humidity Sensor (HR202)

This project demonstrates how to use an ESP8266 microcontroller to read humidity data from an HR202 sensor and publish the readings to an MQTT broker. It provides a simple example of integrating IoT hardware and MQTT for environmental monitoring.

## Features
- Reads humidity data from the HR202 sensor.
- Converts analog sensor readings into humidity values using a linear calibration formula.
- Publishes humidity readings to an MQTT topic at regular intervals.
- Includes MQTT reconnection logic for stable communication.

## Prerequisites
- An ESP8266 board (e.g., NodeMCU, Wemos D1 Mini).
- HR202 humidity sensor.
- Arduino IDE with ESP8266 board support installed.
- Access to an MQTT broker (e.g., [broker.hivemq.com](https://www.hivemq.com)).

## Wiring Diagram
- **HR202 Sensor Connections:**
  - **Signal (OUT)**: Connect to the ESP8266's analog input pin (e.g., `A0`).
  - **VCC**: Connect to 3.3V.
  - **GND**: Connect to GND.

## Code Explanation
1. **Wi-Fi Connection**: Connects the ESP8266 to a Wi-Fi network using the provided SSID and password.
2. **MQTT Connection**: Publishes humidity readings to the MQTT topic `IoTJump_humidity`.
3. **Humidity Calculation**:
   - Reads the HR202 sensor's analog output.
   - Converts the raw ADC value into voltage using:
     \[
     \text{Voltage} = \text{Raw Reading} \times \frac{\text{HR202\_VCC}}{\text{HR202\_RESOLUTION}}
     \]
   - Converts voltage into humidity using a calibration formula.
   - The example formula assumes a linear relationship:
     \[
     \text{Humidity (\%)} = 60 + (\text{Voltage} - 0.5) \times 100
     \]
4. **Publishing Data**: Sends the calculated humidity to the MQTT topic as a formatted string.

## Usage
1. Clone this repository:
   ```bash
   git clone https://github.com/elhamsha/iot.git
