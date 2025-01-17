#include <ESP8266WiFi.h>
#include <PubSubClient.h>

// WiFi credentials
const char* ssid = "111"; 
const char* password = "111";

const char* mqttServer = "broker.hivemq.com"; 
//const char* mqttServer = "test.mosquitto.org";
const int mqttPort = 1883;

const char* humidity_topic = "IoTJump_humidity";  // MQTT topic to publish humidity data

const int HR202_PIN = A0;  // GPIO pin connected to HR202 sensor OUT
const float HR202_VCC = 3.3;  // Voltage supplied to the HR202 sensor (adjust as needed)
const float HR202_RESOLUTION = 1023.0;  // ADC resolution (for analogRead)

WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  Serial.begin(115200);
  delay(10);

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  Serial.println("Connected to WiFi");

  // Set MQTT server and callback function
  client.setServer(mqttServer, mqttPort);
  client.setCallback(callback);
}

void callback(char* topic, byte* payload, unsigned int length) {
  // Handle incoming MQTT messages (if needed)
}

void reconnect() {
  // Loop until we're reconnected to the MQTT server
  while (!client.connected()) {
    Serial.println("Attempting MQTT connection...");

    // Attempt to connect
    if (client.connect("ESP8266Client")) {
      Serial.println("Connected to MQTT server");
    } else {
      Serial.print("Failed, rc=");
      Serial.print(client.state());
      Serial.println(" Retrying in 5 seconds...");
      delay(5000);
    }
  }
}

float readHR202Humidity() {
  // Read humidity data from HR202
  int rawReading = analogRead(HR202_PIN);
  float voltage = (float)rawReading * (HR202_VCC / HR202_RESOLUTION);

  // Add your calibration formula here to convert voltage to humidity
  // Example: 
  float humidity = 60 + (voltage - 0.5) * 100.0; // Assuming a simple linear relationship

  return humidity;
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }

  float humidity = readHR202Humidity();

  char payload[16]; // Adjust buffer size as needed
  snprintf(payload, sizeof(payload), "%.2f", humidity);
  client.publish(humidity_topic, payload);

  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.println("%");

  delay(10000); // Wait for 10 seconds before the next reading
}
