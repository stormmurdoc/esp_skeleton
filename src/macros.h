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

#ifdef ESP_HOSTNAME
    #pragma message STR(ESP_HOSTNAME)
#else
    #warning "ESP_HOSTNAME NOT defined"
#endif


