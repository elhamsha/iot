# ESP32-CAM MQTT Streaming

This project enables an **ESP32-CAM** to stream video over **MQTT**, allowing real-time video transmission to an MQTT broker and connected clients.

## 📁 Project Structure


## 🚀 Features

- Live video streaming from **ESP32-CAM** via **MQTT**
- Base64 encoding for efficient data transmission
- Configurable camera pins for different ESP32-CAM models
- Lightweight and optimized for embedded systems

## 📦 Requirements

- **ESP32-CAM** module
- **Arduino IDE** with ESP32 board support
- **MQTT Broker** (e.g., Mosquitto, broker.mqttdashboard.com)
- Libraries:
  - `PubSubClient` (for MQTT)
  - `WiFi` (for network connectivity)

## 🔧 Setup & Usage

1. Install the required **ESP32 board support** in the Arduino IDE.
2. Add the necessary **libraries** via the Arduino Library Manager.
3. Configure **WiFi credentials** and **MQTT broker details** in `esp32cam_mqtt_streaming.ino`.
4. Compile and upload the sketch to your **ESP32-CAM**.
5. Run an MQTT client to receive and display the video stream.

## 📺 Viewing the Stream

Use the provided **HTML client** (`live_stream_viewer/index.html`) to view the live stream from your MQTT broker.

## 🛠 Configuration

Modify `camera_pins.h` to match your **ESP32-CAM** model's pin configuration.

## 📜 License

This project is open-source and available under the **MIT License**.

---

🔗 *Contributions and improvements are welcome!*
