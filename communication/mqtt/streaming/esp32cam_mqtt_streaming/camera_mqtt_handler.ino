void initCamera() {
    camera_config_t cameraConfig;
    cameraConfig.ledc_channel = LEDC_CHANNEL_0;
    cameraConfig.ledc_timer = LEDC_TIMER_0;
    cameraConfig.pin_d0 = Y2_GPIO_NUM;
    cameraConfig.pin_d1 = Y3_GPIO_NUM;
    cameraConfig.pin_d2 = Y4_GPIO_NUM;
    cameraConfig.pin_d3 = Y5_GPIO_NUM;
    cameraConfig.pin_d4 = Y6_GPIO_NUM;
    cameraConfig.pin_d5 = Y7_GPIO_NUM;
    cameraConfig.pin_d6 = Y8_GPIO_NUM;
    cameraConfig.pin_d7 = Y9_GPIO_NUM;
    cameraConfig.pin_xclk = XCLK_GPIO_NUM;
    cameraConfig.pin_pclk = PCLK_GPIO_NUM;
    cameraConfig.pin_vsync = VSYNC_GPIO_NUM;
    cameraConfig.pin_href = HREF_GPIO_NUM;
    cameraConfig.pin_sscb_sda = SIOD_GPIO_NUM;
    cameraConfig.pin_sscb_scl = SIOC_GPIO_NUM;
    cameraConfig.pin_pwdn = PWDN_GPIO_NUM;
    cameraConfig.pin_reset = RESET_GPIO_NUM;
    cameraConfig.xclk_freq_hz = 20000000;
    cameraConfig.pixel_format = PIXFORMAT_JPEG;

    if (psramFound()) { 
        cameraConfig.frame_size = FRAMESIZE_UXGA;
        cameraConfig.jpeg_quality = 10;
        cameraConfig.fb_count = 2;
    } else {
        cameraConfig.frame_size = FRAMESIZE_SVGA;
        cameraConfig.jpeg_quality = 12;
        cameraConfig.fb_count = 1;
    }

    esp_err_t err = esp_camera_init(&cameraConfig);
    if (err != ESP_OK) {
        Serial.printf("Camera init failed with error 0x%x \n", err);
        ESP.restart();
    }

    sensor_t *s = esp_camera_sensor_get();
    s->set_framesize(s, FRAMESIZE_CIF); // Adjust frame size as needed, QVGA(320x240), CIF(400x296)    
}

// MQTT Callback
void callback(char* topic, byte* payload, unsigned int length) {
    // Print the received topic and message payload
    Serial.print("[Topic: ");
    Serial.print(topic);
    Serial.print("] Message: ");

    String receivedMessage = "";
    for (unsigned int i = 0; i < length; i++) {
        receivedMessage += (char)payload[i];
    }
    Serial.println("Received MQTT message: " + receivedMessage);
}

// Reconnect to MQTT
void reconnect() {
    // Loop until we're reconnected
    while (!client.connected()) {
        String clientId = "ESP32-" + String(random(0xffff), HEX); // A random client ID
        Serial.println("Attempting MQTT connection...");
        if (client.connect(clientId.c_str(), MQTT_USER, MQTT_PASSWORD)) {
            Serial.println("MQTT connected!");
            client.subscribe(MQTT_SUBSCRIBE_TOPIC);
        } else {
            Serial.print("MQTT connection failed, rc=");
            Serial.println(client.state());
            delay(2000);
        }
    }
}

String sendFrame() {
    Serial.println("Sending frame...");
    camera_fb_t *fb = esp_camera_fb_get();
    if (!fb) {
        String returnString = "Camera capture failed!";
        Serial.println(returnString);
        return returnString;
    }

    // Encode the frame in Base64
    char *input = (char *)fb->buf;
    char output[base64_enc_len(3)];
    String imageFile = "data:image/jpeg;base64,";
    for (int i = 0; i < fb->len; i++) {
        base64_encode(output, (input++), 3);
        if (i % 3 == 0) imageFile += String(output);
    }
    int fbLen = imageFile.length();

    String clientId = "ESP32-";
    clientId += String(random(0xffff), HEX);
    if (client.connect(clientId.c_str(), MQTT_USER, MQTT_PASSWORD)) {
        client.beginPublish(MQTT_PUBLISH_TOPIC, fbLen, true);

        String str = "";
        for (size_t n = 0; n < fbLen; n = n + 2048) {
            if (n + 2048 < fbLen) {
                str = imageFile.substring(n, n + 2048);
                client.write((uint8_t*)str.c_str(), 2048);
            }
            else if (fbLen % 2048 > 0) {
                size_t remainder = fbLen % 2048;
                str = imageFile.substring(n, n + remainder);
                client.write((uint8_t*)str.c_str(), remainder);
            }
        }

        client.endPublish();
        esp_camera_fb_return(fb); // Return frame buffer early to free memory
        return "";
    }
    esp_camera_fb_return(fb); // Return frame buffer early to free memory
    return "Failed process, rc=" + client.state();
}

void sendText(const String& text) {
    // Generate a unique client ID for the session
    char clientId[20];
    snprintf(clientId, sizeof(clientId), "ESP32-%04X", random(0xFFFF));

    // Attempt to connect to the MQTT broker and publish the text
    if (client.connect(clientId, MQTT_USER, MQTT_PASSWORD)) {
        client.publish(MQTT_PUBLISH_TOPIC, text.c_str());
        Serial.println("Text sent successfully!");
    } else {
        // Handle failure to send text
        Serial.print("Failed to send text, rc=");
        Serial.println(client.state());
    }
}
