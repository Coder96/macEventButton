// From https://github.com/jeena/esp8266-button

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <PubSubClient.h>

#define ssid "myssid"
#define password "mypass"
#define mqtt_server "example.com"
#define mqtt_port 1883
#define mqtt_user "user"
#define mqtt_password "passwd"
#define topic "macEvent"
#define topic_content "toggle"

WiFiClient espClient;
PubSubClient client(espClient);

//const int powerLatch = 5;
#define powerLatch 2
#define statusLed 0

void setup(void)
{
	
  Serial.begin(9600);

	// Turn gpoi pin on to keep the power on
	pinMode(powerLatch, OUTPUT);
	digitalWrite(powerLatch, HIGH);

  // Connect to WiFi network
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  client.setServer(mqtt_server, mqtt_port);
}

void mqttConnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("ESP8266ButtonClient", mqtt_user, mqtt_password)) {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void loop(void)
{
  if (!client.connected()) {
    mqttConnect();
  }
  client.loop();
  client.publish(topic, topic_content, true);
  client.disconnect();
  Serial.println("Done with client");
  
	// Turn off pin for power circuit.
  digitalWrite(powerLatch, LOW);

	//
  // Sleep basically forever
  ESP.deepSleep(999999999*999999999U, WAKE_NO_RFCAL);
  
  
  
}


