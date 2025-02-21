#include <WiFi.h>
#include "Base64.h"
#include "camera_pins.h"
#include "esp_camera.h"
#include "soc/soc.h"
#include "soc/rtc_cntl_reg.h"
#include <PubSubClient.h>

// WiFi and MQTT configuration – don't forget to update the credentials!
const char* ssid = "AP23";
const char* password = "ymyr3138";
const char* mqtt_server = "broker.mqttdashboard.com"; // Options: broker.hivemq.com, broker.emqx.io
const unsigned int mqtt_port = 1883;

#define MQTT_USER               ""
#define MQTT_PASSWORD           ""
#define MQTT_PUBLISH_TOPIC      "camera/elhamshaMQTTStream"
#define MQTT_SUBSCRIBE_TOPIC    "camera/elhamshaMQTTStream/get"

// WiFi and MQTT Clients
WiFiClient espClient;
PubSubClient client(espClient);

// Timing for periodic actions
unsigned long previousMillis = 0;
const long interval = 150;

// Initialize WiFi
void initWiFi() {
    WiFi.mode(WIFI_STA);
    Serial.print("Connecting to WiFi: ");
    Serial.println(ssid);

    for (int attempt = 0; attempt < 3; attempt++) {
        WiFi.begin(ssid, password);

        unsigned long startTime = millis();
        while (WiFi.status() != WL_CONNECTED && millis() - startTime < 5000) {
            delay(500);
            Serial.print(".");
        }

        if (WiFi.status() == WL_CONNECTED) {
            Serial.println("\nWiFi Connected. IP Address: ");
            Serial.println(WiFi.localIP());
            return;
        }

        Serial.println("\nWiFi connection failed. Retrying...");
    }

    Serial.println("Failed to connect to WiFi after 3 attempts. Restarting...");
    ESP.restart();
}

void setup() {
    WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0); // Disable brownout detector
    Serial.begin(115200);
    randomSeed(micros());

    initCamera();
    initWiFi();

    client.setServer(mqtt_server, mqtt_port);
    client.setCallback(callback);
    client.setBufferSize(2048); // Increase buffer size for large messages
}

// Main loop
void loop() {
    if (!client.connected()) {
        reconnect();
    }
    else {
        unsigned long currentMillis = millis();
        if (currentMillis - previousMillis >= interval) {
            previousMillis = currentMillis;
            String feedback = sendFrame();
            sendText(feedback);
        }
    }
    client.loop();
}

