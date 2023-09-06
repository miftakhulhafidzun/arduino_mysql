#include <ESP8266WiFi.h>

#define TRIGGER_PIN D1 // Define the trigger pin
#define ECHO_PIN D2    // Define the echo pin

char ssid[] = "POCO F4";     // Replace with your WiFi network name
const char* password = "1234lima"; // Replace with your WiFi password
const char* host = "192.168.97.108"; // Replace with your server address

void setup() {
  Serial.begin(115200);
  Serial.println("HC-SR04 Distance Sensor Output!");

  pinMode(TRIGGER_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  // Connect to Wi-Fi
  Serial.println();
  Serial.println("Connecting to " + String(ssid));
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  // Trigger the HC-SR04 sensor
  digitalWrite(TRIGGER_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIGGER_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGGER_PIN, LOW);

  // Measure the echo duration to calculate distance
  long duration = pulseIn(ECHO_PIN, HIGH);
  float distance = (duration / 2.0) * 0.0344; // Speed of sound in cm/us

  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  // Connect to the server and send the distance
  Serial.print("Connecting to ");
  Serial.println(host);

  WiFiClient client;
  const int httpPort = 8888;
  if (!client.connect(host, httpPort)) {
    Serial.println("Connection failed");
    return;
  }

  // Send the distance as a GET request
  client.print(String("GET /iot_project/connect.php?") +
               "distance_cm=" + String(distance) +
               " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "Connection: close\r\n\r\n");

  unsigned long timeout = millis();
  while (client.available() == 0) {
    if (millis() - timeout > 1000) {
      Serial.println(">>> Client Timeout !");
      client.stop();
      return;
    }
  }

  // Read and print the response from the server
  while (client.available()) {
    String line = client.readStringUntil('\r');
    Serial.print(line);
  }

  Serial.println();
  Serial.println("Closing connection");

  delay(6000); // Wait before taking another distance measurement
}
