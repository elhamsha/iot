# Live MQTT Camera Streaming

This Python script captures frames from a webcam, encodes them in Base64, and publishes them to an MQTT topic. The frames can then be received and displayed by an MQTT subscriber.

## Features
- Captures frames from a webcam in real-time
- Resizes frames to optimize performance
- Encodes images in Base64 format for MQTT transmission
- Publishes frames to an MQTT broker
- Prints FPS (frames per second) to monitor performance

## Requirements
Ensure you have the following installed:

```sh
pip install opencv-python paho-mqtt
```

## Usage
1. Update the MQTT broker details in the script:
   ```python
   MQTT_BROKER = "broker.mqttdashboard.com"
   MQTT_SEND = "camera/elhamshaMQTTStream"
   ```

2. Run the script:
   ```sh
   python webcam_mqtt_publisher.py
   ```

## Configuration
- Modify the `frame` resolution in the script to adjust image quality and performance:
  ```python
  frame = cv.resize(frame, (240, 190))
  ```
- Adjust the sleep time to control frame rate:
  ```python
  time.sleep(0.06)
  ```

## Notes
- Ensure your MQTT broker is running and accessible.
- You may need to adjust the frame size and sleep time depending on your network and hardware.
- To view the live camera feed, use the HTML file in the live_stream_viewer folder.

## License
This project is licensed under the MIT License.

