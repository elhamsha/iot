#include <ESP8266WiFi.h>
#include <PubSubClient.h>

// WiFi credentials
const char* ssid = "111"; 
const char* password = "111";

// MQTT broker
const char* mqttServer = "test.mosquitto.org";
const int mqttPort = 1883;
const char* mqttTopic = "IoTJump_topic";

WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
    pinMode(D4, OUTPUT);
    digitalWrite(D4, HIGH);

    Serial.begin(115200);
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.println("Connecting to WiFi...");
    }
    
    Serial.println("Connected to WiFi");
    client.setServer(mqttServer, mqttPort);
    client.setCallback(callback);
}

void callback(char* topic, byte* payload, unsigned int length) {
    String message = "";
    for (int i = 0; i < length; i++) {
        message += (char)payload[i];
    }
    Serial.println("Message received: --->  " + message);
    if (message == "ON") {
        digitalWrite(D4, LOW);
    } else if (message == "OFF") {
        digitalWrite(LED_BUILTIN, HIGH);
    }
}

void reconnect() {
    while (!client.connected()) {
        if (client.connect("ESP8266Client")) {
            client.subscribe(mqttTopic);
        } else {
            delay(5000);
        }
    }
}

void loop() {
    if (!client.connected()) {
        reconnect();
    }
    client.loop();
}

