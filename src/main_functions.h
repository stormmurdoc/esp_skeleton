/* some debug infos */
void printDevInfos()
{
   	Serial.begin(115200);
	Serial.println(">> Booting");
	Serial.print(">> git rev: ");
    Serial.println(GIT_REV);
	Serial.print(">> ESP_HOSTNAME: ");
    Serial.println(ESP_HOSTNAME);
    Serial.print(">> MQTT_SERVER: ");
    Serial.println(MQTT_SERVER);
    Serial.print(">> MQTT_PORT: ");
    Serial.println(MQTT_PORT);
    Serial.print(">> WIFI SSID: ");
    Serial.println(WIFI_SSID);
    Serial.print(">> NTP SERVER: ");
    Serial.println(NTP_SERVER);
    Serial.print(">> UPDATE INTERVAL (ms): ");
    Serial.println(UPDATE_INTERVAL);


}

/*
 * init WiFi
 *
*/
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

/*
 * Over the air updates
 *
*/
void setup_ota()
{
    ArduinoOTA.setPort(8266);
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
	Serial.print(">> Ready");
	Serial.print(" - My IP address: ");
	Serial.println(WiFi.localIP());
}

void FlashStatusLED(){

    int i = 0;
    while(i<10){
        if(digitalRead(LED_BUILTIN) == HIGH){
            digitalWrite(LED_BUILTIN, LOW);
        } else {
            digitalWrite(LED_BUILTIN, HIGH);
        }
        delay(30);
        i = i + 1;
    }
    digitalWrite(LED_BUILTIN, LOW);
}
// Telemetrie data
void sendTelemetrie(long now){

    // send telemetrie data
    if (now - lastMsg > UPDATE_INTERVAL) {
        lastMsg = now;
        long rssi = WiFi.RSSI();
        itoa(rssi,tmp,10);
        client.publish(MQTT_TOPIC_RSSI, tmp);
        //client.publish(MQTT_TOPIC_RSSI, tmp);
        int ip = WiFi.localIP();
        itoa(ip,tmp,10);
        client.publish(MQTT_TOPIC_IP, tmp);
    }
}
