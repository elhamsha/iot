#include <ESP8266WiFi.h>
#include <PubSubClient.h>

// Replace with your WiFi credentials
const char* ssid = "111"; 
const char* password = "111";

// MQTT broker settings
const char* mqttServer = "test.mosquitto.org";

// Temperature sensor settings
const int sensorPin = D4; // Define the pin where the KY-013 analog output is connected

WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  Serial.begin(115200);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  client.setServer(mqttServer, 1883);
  client.setCallback(callback);

  while (!client.connected()) {
    if (client.connect("ESP8266Client" )) {
      Serial.println("Connected to MQTT broker");
    } else {
      Serial.print("Failed, rc=");
      Serial.print(client.state());
      Serial.println(" Retrying in 5 seconds...");
      delay(5000);
    }
  }
}

void loop() {
  delay(2000);  // Adjust the delay as needed
  float voltage = analogRead(sensorPin) * (3.3 / 1024.0); // Convert analog reading to voltage
  float temperature = (voltage - 0.5) * 100.0; // Assuming a linear relationship between voltage and temperature

  Serial.print("Voltage: ");
  Serial.print(voltage);
  Serial.println(" V");

  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println(" °C");

  // Publish temperature to MQTT topic
  char tempStr[6];
  dtostrf(temperature, 4, 1, tempStr);
  client.publish("your/mqtt/topic", tempStr);

  client.loop();
}

void callback(char* topic, byte* payload, unsigned int length) {
  // Handle MQTT messages if needed
}

