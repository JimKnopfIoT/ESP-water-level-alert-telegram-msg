/* Arduino script for use with ESP01 (ESP8266). This script informs you when waterlevel raises and flooding your basement. */

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <ESP8266TelegramBOT.h>
#include <PubSubClient.h>

extern "C" {
#include "user_interface.h" //os_timer
}

#define inputPin 4
// #define D2 4


//////////////////////////  Telegram Config  //////////////////////////////////////////
// First create a bot with telegram's botfather and write down the bot settings. 
// Findout your own telegramID (this is the adminID to communicate with the bot).
// If you create a channel, findout the channelID (chatID).

#define botName "<botName>"  // for Bot use
#define botUserName "<botUserName>" // for Bot use
#define botToken "<like 123456789:AABBccDDeeFfgGHHiIjjKklLMmnNooPPqQrR" // for Bot use
//#define adminID "like 12345678" // your ID, use this ID if you want do talk directly to the bot
#define chatID "like -123456789" // channelID, use this if you want to talk to the bot via channel, (leading "-" needed)


/////////////////////////  Wifi Config  //////////////////////////////////////////
static char ssid[] = "<your-SSID";
static char password[] = "your-password";
static char hostname[] = "<your-hostname-for-ESP-01";



TelegramBOT bot(botToken, botName, botUserName);
// ---------------------------------------------------------------------------------------------------------------

#define NB_TRYWIFI    10

const char* deviceName = hostname;

void setup()
     {
     rst_info *rinfo = ESP.getResetInfoPtr();
     Serial.println(String("\nResetInfo.reason = ") + (*rinfo).reason + ": " + ESP.getResetReason()); 
     Serial.begin(115200);
     wifi_station_set_hostname(hostname);
     WiFi.begin(SSID, password);
     Serial.print("\n  Connecting to " + String(SSID) + " ");
     int _try = 0;
     // Wait until it is connected to the network
     while (WiFi.status() != WL_CONNECTED) 
           {
           delay(300);
           Serial.print(".");
           }
     delay(300);
     _try++;
     if ( _try >= NB_TRYWIFI ) 
        {
        Serial.println("Keine Verbindung mit dem WLAN Netzwerk möglich, gehe in den Tiefschlaf!");
        delay(60000);
        }
     Serial.println();       
     Serial.println("WiFi verbunden.");  
     //delay(300);
     Serial.println("IP-Adresse: " + WiFi.localIP().toString());
     Serial.println("Hostname:  " + String(hostname));
     //delay(300);
     bot.begin();          // initialize telegram bot
     //delay(300);
     //bot.sendMessage(chatID, "ESP01-Pumpensumpf online.", ""); // Bot <-> Channel
     Serial.println("ESP01-Pumpensumpf online.");
     Serial.println("- - - - - - - - - - - - - - - - - - -\n");
     //delay(300);
     
     Serial.print("Achtung! Wasserstand zu hoch!");
     bot.sendMessage(chatID, String(hostname) + ", " + "Achtung! Wasserstand zu hoch!", "");  // Bot <-> Channel        
     Serial.println();
     delay(200);
   
     ESP.deepSleep(0);
}

//////////////////////////////////////////////////////////////////
void loop()
{
 
}

//////////////////////////////////////////////////////////////////
