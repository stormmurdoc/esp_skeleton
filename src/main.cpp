/***************************************************************************
  ESP Skeleton
 ***************************************************************************/

/* include stuff */
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <ESP8266WiFi.h>
#include <ArduinoOTA.h>
#include <PubSubClient.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
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
WiFiUDP ntpUDP;

// pio lib install "arduino-libraries/NTPClient"
// europe.pool.ntp.org

NTPClient timeClient(ntpUDP,"192.168.1.1", 3600, 60000);

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
char MQTT_TOPIC_CMND[20];


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

    Serial.print("[");
    Serial.print(timeClient.getFormattedTime());
    Serial.print("]");
    Serial.print(" >> Message arrived on topic: ");
    Serial.print(topic);
    Serial.print(". Message: ");

    String messageTemp;
    char* LED_STATE;
    char* CMND;

    for (int i = 0; i < length; i++) {
      Serial.print((char)message[i]);
      messageTemp += (char)message[i];
    }
    Serial.println();

    // Feel free to add more if statements to control more GPIOs with MQTT

    // If a message is received on the topic MQTT_TOPIC/output, you check if the message is either "on" or "off".
    // Changes the output state according to the message
    Serial.print(String(topic));
    Serial.print("=");
    Serial.println(String(MQTT_TOPIC_CMND));
    if (String(topic) == String(MQTT_TOPIC_CMND)) {
      Serial.print(">> cmnd: ");
      if(messageTemp == "set_on"){
        Serial.println(">> set_on");
        digitalWrite(ledPin, HIGH);
        lastLEDState=HIGH;
        CMND="on";
      }
      else if(messageTemp == "set_off"){
        Serial.println(">> set_off");
        digitalWrite(ledPin, LOW);
        lastLEDState=LOW;
        CMND="off";
      }
      else if(messageTemp == "toggle"){
        Serial.print(">> toggle: last state: ");
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
      if(lastLEDState == 1 ) {
          LED_STATE="on";
      }
      else {
          LED_STATE="off";
      }
      client.publish(MQTT_TOPIC_OUT, LED_STATE);

    }

}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
      Serial.print(">> Attempting MQTT connection...");
      // Attempt to connect
      if (client.connect("ESP8266Client")) {
        Serial.println(">> connected");
        // Subscribe
        Serial.print(">> subcribing ");
        Serial.println(cMQTT_TOPIC);
        client.subscribe(cMQTT_TOPIC);
      } else {
        Serial.print("failed, rc=");
        Serial.print(client.state());
        Serial.println(" try again in 5 seconds");
        // Wait 5 seconds before retrying
        delay(5000);
      }
  }
}

// Telemetrie data
void sendTelemetrie(){

        long rssi = WiFi.RSSI();
        itoa(rssi,tmp,10);
        client.publish(MQTT_TOPIC_RSSI, tmp);
        if(digitalRead(LED_BUILTIN) == HIGH){
            digitalWrite(LED_BUILTIN, LOW);
        } else {
            digitalWrite(LED_BUILTIN, HIGH);
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
    // MQTT define topics
    snprintf(cMQTT_TOPIC,sizeof cMQTT_TOPIC,"%s%s",MQTT_TOPIC, "/#");
    snprintf(MQTT_TOPIC_OUT,sizeof MQTT_TOPIC_OUT,"%s%s",MQTT_TOPIC, "/output");
    snprintf(MQTT_TOPIC_RSSI,sizeof MQTT_TOPIC_RSSI,"%s%s",MQTT_TOPIC, "/rssi");
    snprintf(MQTT_TOPIC_CMND,sizeof MQTT_TOPIC_CMND,"%s%s",MQTT_TOPIC, "/cmnd");

    // subscribe
    client.subscribe(cMQTT_TOPIC);
    client.setCallback(callback);

    // NTP time sync
    timeClient.begin();

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
    timeClient.update();
    // handle over the air updates
    ArduinoOTA.handle();

    // send telemetrie data
    if (now - lastMsg > 5000) {
        lastMsg = now;
        sendTelemetrie();

    }
}
