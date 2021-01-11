/* Note: Please do not edit!
 * All custom settings could be set in the platformio.ini file
 * */
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

#ifdef NTP_SERVER
    #pragma message STR(NTP_SERVER)
#else
    #warning "NTP_SERVER NOT defined"
#endif

#ifdef UPDATE_INTERVAL
    #pragma message STR(UPDATE_INTERVAL)
#else
    #warning "UPDATE_INTERVAL NOT defined"
#endif

char* clientId = ESP_HOSTNAME;
char cMQTT_TOPIC[20];
char MQTT_TOPIC_RSSI[20];
char MQTT_TOPIC_OUT[20];
char MQTT_TOPIC_CMND[20];
char MQTT_TOPIC_IP[20];

