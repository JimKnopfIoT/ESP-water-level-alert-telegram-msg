/* Arduino script for use with Lolin D1 mini ESP8266. This script informs you when waterlevel raises and flooding your basement. */

#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <ESP8266TelegramBOT.h>

extern "C"
{ 
    #include "user_interface.h" 
}

#define inputPin 4
// #define D2 4

#define NB_TRYWIFI    10

// Telegram Settings
// First create a bot with telegram's botfather and write down the bot settings. 
// Findout your own telegramID (this is the adminID to communicate with the bot).
// If you create a channel, findout the channelID (chatID).

#define botName "<botName>"  // for Bot use
#define botUserName "<botUserName>" // for Bot use
#define botToken "<like 123456789:AABBccDDeeFfgGHHiIjjKklLMmnNooPPqQrR" // for Bot use
//#define adminID "like 12345678" // your ID, use this ID if you want do talk directly to the bot
#define chatID "like -123456789" // channelID, use this if you want to talk to the bot via channel, (leading "-" needed)

// Wifi settings 
static char ssid[] = "<your-SSID";
static char password[] = "your-password";
static char hostname[] = "<your-hostname-for-ESP-01";



// ---------------------------------------------------------------------------------------------------------------

TelegramBOT bot(botToken, botName, botUserName);
void setup() {
  // Setup the pins
  pinMode(inputPin, INPUT);
  // Start serial com
  Serial.begin(115200);
  // Restart reason
  Serial.println(""); Serial.print("Reason for startup :");Serial.println(ESP.getResetReason());
  // Set the hostname (otherwise the controller will be named esp8266) and init the esp for WiFi use
  wifi_station_set_hostname(hostname);
  WiFi.begin(SSID, password);
  Serial.print("\n  Connecting to " + String(SSID) + " ");
  int _try = 0;
  // Wait until it is connected to the network
  while (WiFi.status() != WL_CONNECTED) {
        Serial.print(".");
        delay(500);
        _try++;
        if ( _try >= NB_TRYWIFI ) {
           Serial.println("Impossible to connect WiFi network, go to deep sleep");
           ESP.deepSleep(0, WAKE_RF_DEFAULT);
           delay(1000);
           }
        Serial.println("\n\nWiFi connected.");  
        delay(500);
        Serial.print("  IP address: " + WiFi.localIP().toString() + "\n");
        Serial.print("  Host name:  " + String(hostname) + "\n");
        Serial.print("- - - - - - - - - - - - - - - - - - -\n\n");
        delay(3000);
        // Start the bot and send a message to you, so you know the ESP has booted and is online.
        bot.begin();
        bot.sendMessage(adminID, "Lolin D1 mini from pump pit online.", "");
        Serial.print( "Lolin D1 mini from pump pit online. \n");
  }
  // Check if water contacts Pin 4 and Vcc and shorten it
        if (digitalRead(inputPin) == LOW) {
        bot.sendMessage(adminID, "Waterlevel OK", "");
        Serial.println("\n\nWaterlevel OK");  
        delay(5000);   // Halt for 5 sec
        }
    else
        if (digitalRead(inputPin) == HIGH) {
        bot.begin();
        bot.sendMessage(adminID, "Attention! Waterlevel too high! ", "");
        Serial.print( "Attention! Waterlevel too high! \n");
        delay(5000);   // Halt for 5 sec
        ESP.deepSleep(1800000000, WAKE_RF_DEFAULT); // wait 30 Minutes (60 Seconds * 30 Minutes = 1800 Sekunden)
        // ESP.deepSleep(60000000); // For testing purpose 60 Sekunden
        }
     
  ESP.deepSleep(0, WAKE_RF_DEFAULT);
  delay(300);
}

void loop()
{

}

/* 4. November 2018, connected via Pin D2 works perfect.Wakeup from deepsleep works. 
 */
