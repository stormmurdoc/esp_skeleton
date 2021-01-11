/***************************************************************************
  ESP Skeleton

  A template for ESP8266 devices
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
#include <main_macros.h>

WiFiClient espClient;
PubSubClient client(espClient);
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP,NTP_SERVER, 3600, 60000);

// pio lib install "arduino-libraries/NTPClient"
// europe.pool.ntp.org

long lastMsg = 0;
char msg[50];
int value = 0;
char tmp[50];
char time_value[20];
int lastLEDState = 0;

// LED Pin
const int ledPin = D7;

#include <main_functions.h>

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
    //FlashStatusLED();
    // Feel free to add more if statements to control more GPIOs with MQTT

    // If a message is received on the topic MQTT_TOPIC/cmnd,
    // you check if the message is either "on" or "off".
    // Changes the output state according to the message

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
      Serial.println(">> Attempting MQTT connection...");
      Serial.println(client.state());
      /* setup mqtt_server */
      if (client.connect(clientId , ESP_HOSTNAME, MQTT_PASSWORD)) {
          Serial.print(">> MQTT connected to ");
          Serial.print(MQTT_SERVER);
          Serial.print(":");
          Serial.println(MQTT_PORT);
          Serial.print(">> subcribing ");
          Serial.println(cMQTT_TOPIC);
          client.subscribe(cMQTT_TOPIC);
      } else {
          // Attempt to connect
          Serial.print("failed, rc=");
          Serial.print(client.state());
          Serial.println(" try again in 5 seconds");
          // Wait 5 seconds before retrying
          delay(5000);
      }
  }
}

/*
 * Setup procedure
*/
void setup()
{
    // MQTT define topics
    snprintf(cMQTT_TOPIC,sizeof cMQTT_TOPIC,"%s%s",MQTT_TOPIC, "/#");
    snprintf(MQTT_TOPIC_OUT,sizeof MQTT_TOPIC_OUT,"%s%s",MQTT_TOPIC, "/output");
    snprintf(MQTT_TOPIC_RSSI,sizeof MQTT_TOPIC_RSSI,"%s%s",MQTT_TOPIC, "/rssi");
    snprintf(MQTT_TOPIC_CMND,sizeof MQTT_TOPIC_CMND,"%s%s",MQTT_TOPIC, "/cmnd");

    client.setServer(MQTT_SERVER, MQTT_PORT);

    printDevInfos();
    setup_wifi();
    setup_ota();
    reconnect();

    // NTP time sync
    timeClient.begin();

    // initialize digital pin LED_BUILTIN as an output.
    pinMode(LED_BUILTIN, OUTPUT);
    pinMode(ledPin, OUTPUT);
}

/*
 *  Main loop
 */
void loop()
{
    long now = millis();
    reconnect();
    client.loop();
    timeClient.update();
    ArduinoOTA.handle();
    sendTelemetrie(now);
}
