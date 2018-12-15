#include <Arduino.h>
#include "Config.h"

#include "AutoShutdown.h"
#include <ReButton.h>
#include <AZ3166WiFi.h>

const char* CONFIG_FIRMWARE_VERSION = "1.0";
static const char* SSID_PREFIX = "AZB-";

CONFIG_TYPE Config;

static void strncpy_w_zero(char* dest, const char* src, int destSize)
{
    strncpy(dest, src, destSize - 1);
    dest[destSize] = '\0';
}

static uint8_t CalcCheckSum(CONFIG_TYPE* config)
{
    uint8_t backup[2];
    memcpy(backup, config->CheckSum, sizeof (config->CheckSum));
    memset(config->CheckSum, 0, sizeof(config->CheckSum));

    uint8_t checksum = 0;
    const uint8_t* ptr = (const uint8_t*)config;
    for (int i = 0; i < (int)sizeof(*config); i++) checksum ^= *ptr++;

    memcpy(config->CheckSum, backup, sizeof(config->CheckSum));

    return checksum;
}

void ConfigResetFactorySettings()
{
	unsigned char macAddress[6];
	WiFi.macAddress(macAddress);
	char ssid[strlen(SSID_PREFIX) + 6 * 2 + 1];
	snprintf(ssid, sizeof(ssid), "%s%02x%02x%02x%02x%02x%02x", SSID_PREFIX, macAddress[0], macAddress[1], macAddress[2], macAddress[3], macAddress[4], macAddress[5]);

    memset(&Config, 0, sizeof(Config));

	Config.DisplayColorSingleClick    = { 0  , 0  , 255 };  // BLUE
	Config.DisplayColorDoubleClick    = { 0  , 255, 0   };  // LIME
    Config.DisplayColorTripleClick    = { 255, 0  , 255 };  // MAGENTA
	Config.DisplayColorLongPress      = { 255, 255, 0   };  // YELLOW
	Config.DisplayColorSuperLongPress = { 0  , 255, 255 };  // CYAN
    Config.DisplayColorUltraLongPress = { 255, 255, 255 };  // WHITE

    strncpy_w_zero(Config.Message1, "Single click", sizeof(Config.Message1));
    strncpy_w_zero(Config.Message2, "Double click", sizeof(Config.Message2));
    strncpy_w_zero(Config.Message3, "Triple click", sizeof(Config.Message3));
    strncpy_w_zero(Config.Message10, "Long press", sizeof(Config.Message10));
    strncpy_w_zero(Config.Message11, "Super long press", sizeof(Config.Message11));

    strncpy_w_zero(Config.CustomMessagePropertyName, "customMessageEnable", sizeof(Config.CustomMessagePropertyName));
    Config.CustomMessageEnable = false;
    strncpy_w_zero(Config.CustomMessageJson, "\"message\":\"Custom message\"", sizeof(Config.CustomMessageJson));

    strncpy_w_zero(Config.WiFiSSID, "", sizeof(Config.WiFiSSID));
    strncpy_w_zero(Config.WiFiPassword, "", sizeof(Config.WiFiPassword));
	strncpy_w_zero(Config.TimeServer, "", sizeof(Config.TimeServer));

	strncpy_w_zero(Config.ScopeId, "", sizeof(Config.ScopeId));
	strncpy_w_zero(Config.DeviceId, "", sizeof(Config.DeviceId));
	strncpy_w_zero(Config.SasKey, "", sizeof(Config.SasKey));

    strncpy_w_zero(Config.IoTHubConnectionString, "", sizeof(Config.IoTHubConnectionString));

    strncpy_w_zero(Config.MQTTServer, "", sizeof(Config.MQTTServer));
	strncpy_w_zero(Config.MQTTPort, "", sizeof(Config.MQTTPort));
	strncpy_w_zero(Config.MQTTTopic, "", sizeof(Config.MQTTTopic));
    strncpy_w_zero(Config.MQTTClient, "", sizeof(Config.MQTTClient));
	strncpy_w_zero(Config.MQTTUser, "", sizeof(Config.MQTTUser));
	strncpy_w_zero(Config.MQTTPassword, "", sizeof(Config.MQTTPassword));

	strncpy_w_zero(Config.APmodeSSID, ssid, sizeof(Config.APmodeSSID));
	strncpy_w_zero(Config.APmodePassword, "", sizeof(Config.APmodePassword));
}

void ConfigRead()
{
	ReButton::ReadConfig(&Config, sizeof(Config));

    uint8_t check_sum = CalcCheckSum(&Config);
	if (Config.CheckSum[0] != check_sum || Config.CheckSum[1] != (check_sum ^ 0xff)) ConfigResetFactorySettings();
}

void ConfigWrite()
{
    uint8_t check_sum = CalcCheckSum(&Config);
    Config.CheckSum[0] = check_sum;
    Config.CheckSum[1] = check_sum ^ 0xff;

	AutoShutdownSuspend();
	ReButton::WriteConfig(&Config, sizeof(Config));
	AutoShutdownResume();
}

void ConfigPrint()
{
    //Serial.printf("DisplayColorSingleClick = #%02X%02X%02X\n", Config.DisplayColorSingleClick.Red, Config.DisplayColorSingleClick.Green, Config.DisplayColorSingleClick.Blue);
    //Serial.printf("DisplayColorDoubleClick = #%02X%02X%02X\n", Config.DisplayColorDoubleClick.Red, Config.DisplayColorDoubleClick.Green, Config.DisplayColorDoubleClick.Blue);
    //Serial.printf("DisplayColorTripleClick = #%02X%02X%02X\n", Config.DisplayColorTripleClick.Red, Config.DisplayColorTripleClick.Green, Config.DisplayColorTripleClick.Blue);
    //Serial.printf("DisplayColorLongPress = #%02X%02X%02X\n", Config.DisplayColorLongPress.Red, Config.DisplayColorLongPress.Green, Config.DisplayColorLongPress.Blue);
    //Serial.printf("DisplayColorSuperLongPress = #%02X%02X%02X\n", Config.DisplayColorSuperLongPress.Red, Config.DisplayColorSuperLongPress.Green, Config.DisplayColorSuperLongPress.Blue);
    //Serial.printf("DisplayColorUltraLongPress = #%02X%02X%02X\n", Config.DisplayColorUltraLongPress.Red, Config.DisplayColorUltraLongPress.Green, Config.DisplayColorUltraLongPress.Blue);
    //Serial.printf("Message1 = %s\n", Config.Message1);
    //Serial.printf("Message2 = %s\n", Config.Message2);
    //Serial.printf("Message3 = %s\n", Config.Message3);
    //Serial.printf("Message10 = %s\n", Config.Message10);
    //Serial.printf("Message11 = %s\n", Config.Message11);
	Serial.printf("WiFiSSID = %s\n", Config.WiFiSSID);
    //Serial.printf("WiFiPassword = %s\n", Config.WiFiPassword);
	Serial.printf("TimeServer = %s\n", Config.TimeServer);
	//Serial.printf("IoTHubConnectionString = %s\n", Config.IoTHubConnectionString);
	Serial.printf("APmodeSSID = %s\n", Config.APmodeSSID);
	//Serial.printf("APmodePassword = %s\n", Config.APmodePassword);
}
