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
}
