/***************************************************************************
  ESP Skeleton
 ***************************************************************************/

/* include stuff */
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <PubSubClient.h>
#include <time.h>
#include <macros.h>
#include <functions.h>

#define ST(A) #A
#define STR(A) ST(A)

#ifdef OTA_PASSWORD
    #pragma message STR(OTA_PASSWORD)
#else
    #warning "OTA_PASSWORD NOT defined"
#endif

#ifdef WIFI_SSID
    #pragma message STR(WIFI_SSID)
#else
    #warning "WIFI_SSID NOT defined"
#endif

#ifdef WIFI_PASSWORD
    #pragma message STR(WIFI_PASSWORD)
#else
    #warning "WIFI_PASSWORD NOT defined"
#endif

#ifdef MQTT_SERVER
    #pragma message STR(MQTT_SERVER)
#else
    #warning "MQTT_SERVER NOT defined"
#endif

#ifdef MQTT_PASSWORD
    #pragma message STR(MQTT_PASSWORD)
#else
    #warning "MQTT_PASSWORD NOT defined"
#endif

#ifdef MQTT_PORT
    #pragma message STR(MQTT_PORT)
#else
    #warning "MQTT_PORT NOT defined"
#endif

#ifdef MQTT_TOPIC
    #pragma message STR(MQTT_TOPIC)
#else
    #warning "MQTT_TOPIC NOT defined"
#endif

#ifdef ESP_HOSTNAME
    #pragma message STR(ESP_HOSTNAME)
#else
    #warning "ESP_HOSTNAME NOT defined"
#endif


WiFiClient espClient;
PubSubClient client(espClient);
char* clientId = ESP_HOSTNAME;
long lastMsg = 0;
char msg[50];
int value = 0;
int lastLEDState = 0;
char tmp[50];
char time_value[20];
char cMQTT_TOPIC[20];
char MQTT_TOPIC_RSSI[20];
char MQTT_TOPIC_OUT[20];


// LED Pin
const int ledPin = D7;

void setup_wifi()
{
	WiFi.mode(WIFI_STA);
	WiFi.begin(WIFI_SSID,WIFI_PASSWORD);

	while (WiFi.waitForConnectResult() != WL_CONNECTED) {
		Serial.print("Connection to ");
        Serial.print(WIFI_SSID);
        Serial.println(" failed! Rebooting...");
		delay(5000);
		ESP.restart();
	}
}

void setup_ota()
{
    // ArduinoOTA.setPort(8266);
	ArduinoOTA.setHostname(ESP_HOSTNAME);
	ArduinoOTA.setPassword(OTA_PASSWORD);
	ArduinoOTA.onStart([]()
	{
		String type;
		if (ArduinoOTA.getCommand() == U_FLASH) {
			type = "sketch";
		}
		else {
		    type = "filesystem"; // U_FS
		}
		// NOTE: if updating FS this would be the place to unmount FS using FS.end()
		Serial.println("Start updating " + type);
	});

	ArduinoOTA.onEnd([]() {
		Serial.println("\nEnd");
	});

	ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
		Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
	});

	ArduinoOTA.onError([](ota_error_t error)
	{
		Serial.printf("Error[%u]: ", error);
		if (error == OTA_AUTH_ERROR) {
			Serial.println("Auth Failed");
		}
		else if (error == OTA_BEGIN_ERROR) {
			Serial.println("Begin Failed");
		}
		else if (error == OTA_CONNECT_ERROR) {
			Serial.println("Connect Failed");
		}
		else if (error == OTA_RECEIVE_ERROR)
		{
			Serial.println("Receive Failed");
		}
		else if (error == OTA_END_ERROR) {
			Serial.println("End Failed");
		} });

	ArduinoOTA.begin();
	Serial.println(">> Ready");
	Serial.print(">> IP address: ");
	Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* message, unsigned int length) {
  Serial.print(">> Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  String messageTemp;

  for (int i = 0; i < length; i++) {
    Serial.print((char)message[i]);
    messageTemp += (char)message[i];
  }
  Serial.println();

  // Feel free to add more if statements to control more GPIOs with MQTT

  // If a message is received on the topic MQTT_TOPIC/output, you check if the message is either "on" or "off".
  // Changes the output state according to the message
  Serial.print(String(MQTT_TOPIC_OUT));
  Serial.print(" = ");
  Serial.println(String(topic));
  if (String(topic) == String(MQTT_TOPIC_OUT)) {
    Serial.print("Changing output to ");
    if(messageTemp == "on"){
      Serial.println("on");
      digitalWrite(ledPin, HIGH);
      lastLEDState=HIGH;
    }
    else if(messageTemp == "off"){
      Serial.println("off");
      digitalWrite(ledPin, LOW);
      lastLEDState=LOW;
    }
    else if(messageTemp == "toggle"){
      Serial.print("toggle: last state: ");
      Serial.println(lastLEDState);
      if(lastLEDState == HIGH){
          digitalWrite(ledPin, LOW);
          lastLEDState=LOW;
      }
      else {
          digitalWrite(ledPin, HIGH);
          lastLEDState=HIGH;
      }
    }
    else if(messageTemp == "reboot"){
      Serial.print("reboot");
      ESP.restart();
    }

  }
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
      Serial.print("Attempting MQTT connection...");
      // Attempt to connect
      if (client.connect("ESP8266Client")) {
        Serial.println("connected");
        // Subscribe
         Serial.println(cMQTT_TOPIC);
         client.subscribe(cMQTT_TOPIC);
        client.subscribe("esp32/out");
      } else {
        Serial.print("failed, rc=");
        Serial.print(client.state());
        Serial.println(" try again in 5 seconds");
        // Wait 5 seconds before retrying
        delay(5000);
      }
  }
}
void setup()
{
    printDevInfos();
    setup_wifi();
    setup_ota();

    /* setup mqtt_server */
    client.setServer(MQTT_SERVER, MQTT_PORT);
    if (client.connect(clientId , ESP_HOSTNAME, MQTT_PASSWORD)) {
        Serial.println(">> MQTT connected");
    }
    // Subscribe
    snprintf(cMQTT_TOPIC,sizeof cMQTT_TOPIC,"%s%s",MQTT_TOPIC, "/#");
    snprintf(MQTT_TOPIC_OUT,sizeof MQTT_TOPIC_OUT,"%s%s",MQTT_TOPIC, "/output");
    Serial.print("subscribe ");
    Serial.println(cMQTT_TOPIC);

    client.subscribe(cMQTT_TOPIC);
    client.setCallback(callback);

    // initialize digital pin LED_BUILTIN as an output.
    pinMode(LED_BUILTIN, OUTPUT);
    pinMode(ledPin, OUTPUT);
}


/* main proc */
void loop()
{
    long now = millis();

    if (!client.connected()) {
        reconnect();
    }
    client.loop();

    ArduinoOTA.handle();                // handle over the air updates

    if (now - lastMsg > 1000) {
        lastMsg = now;
        // Convert the value to a char array
        char tempString[32];
        dtostrf(lastMsg, 1, 2, tempString);

        snprintf(MQTT_TOPIC_RSSI,sizeof MQTT_TOPIC_RSSI,"%s%s",MQTT_TOPIC, "/rssi");
        long rssi = WiFi.RSSI();
        itoa(rssi,tmp,10);
        client.publish(MQTT_TOPIC_RSSI, tmp);
        if(digitalRead(LED_BUILTIN) == HIGH){
            digitalWrite(LED_BUILTIN, LOW);
        } else {
            digitalWrite(LED_BUILTIN, HIGH);
        }
    }

}
