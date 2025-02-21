import time
import base64
import cv2 as cv
import paho.mqtt.client as mqtt
import threading

# MQTT Configuration
MQTT_BROKER = "broker.mqttdashboard.com"
MQTT_TOPIC = "camera/elhamshaMQTTStream"
RECONNECT_DELAY = 5  # Delay before reconnecting in case of failure

# Camera Configuration
CAMERA_INDEX = 0  # Default webcam (change if using an external camera)
FRAME_WIDTH = 320  # Adjust for resolution
FRAME_HEIGHT = 240
FPS_LIMIT = 15  # Limit FPS (Frames Per Second) to reduce CPU usage

# Initialize MQTT Client
client = mqtt.Client()

# Function to handle MQTT connection loss
def on_disconnect(client, userdata, rc):
    print(f"[ERROR] MQTT Disconnected! Reconnecting in {RECONNECT_DELAY} seconds...")
    time.sleep(RECONNECT_DELAY)
    try:
        client.reconnect()
        print("[INFO] Reconnected to MQTT Broker!")
    except Exception as e:
        print(f"[ERROR] Reconnection failed: {e}")

client.on_disconnect = on_disconnect

# Connect to MQTT Broker
try:
    client.connect(MQTT_BROKER)
except Exception as e:
    print(f"[ERROR] Failed to connect to MQTT Broker: {e}")
    exit(1)

# Function to Capture & Send Frames
def stream_video():
    cap = cv.VideoCapture(CAMERA_INDEX)

    if not cap.isOpened():
        print("[ERROR] Could not open webcam!")
        return

    cap.set(cv.CAP_PROP_FRAME_WIDTH, FRAME_WIDTH)
    cap.set(cv.CAP_PROP_FRAME_HEIGHT, FRAME_HEIGHT)

    print("[INFO] Streaming started... Press CTRL+C to stop.")

    try:
        while True:
            start_time = time.time()  # Start time for FPS calculation

            ret, frame = cap.read()
            if not ret:
                print("[ERROR] Failed to capture frame!")
                break

            # Resize frame
            frame = cv.resize(frame, (FRAME_WIDTH, FRAME_HEIGHT))

            # Encode frame to JPEG
            _, buffer = cv.imencode('.jpg', frame)

            # Convert to Base64
            jpg_as_text = base64.b64encode(buffer).decode("utf-8")

            # Publish to MQTT
            client.publish(MQTT_TOPIC, f"data:image/jpg;base64,{jpg_as_text}")

            # FPS Control
            elapsed_time = time.time() - start_time
            delay = max(1.0 / FPS_LIMIT - elapsed_time, 0)
            time.sleep(delay)

            # Print FPS
            fps = 1 / (time.time() - start_time)
            print(f"FPS: {fps:.2f}")

    except KeyboardInterrupt:
        print("\n[INFO] Stopping Stream...")

    finally:
        cap.release()
        client.disconnect()
        print("[INFO] Cleanup complete. Exiting.")

# Run the video streaming function in a separate thread
thread = threading.Thread(target=stream_video)
thread.start()
