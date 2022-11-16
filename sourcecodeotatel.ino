#include <WiFi.h>
#include <HTTPClient.h>
#include <HTTPUpdate.h>
#include <WiFiClientSecure.h>
#include "cert.h"
#include <ArduinoJson.h>
#include <UniversalTelegramBot.h>

const char * ssid = "";//wifi name
const char * wifiPassword = "";//wifi password

int status = WL_IDLE_STATUS;
#define BOTtoken "55174733:AAEiK3fDEz8xtz4COuikT0M" //Telegram BOT token 
#define CHAT_ID "6590068"//Chat ID from telegram for authorized user
#ifdef ESP8266
  X509List cert(TELEGRAM_CERTIFICATE_ROOT);
#endif
WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);

int botRequestDelay = 1000;
unsigned long lastTimeBotRan;

String FirmwareVer = {
  "5.0"
};
#define URL_fw_Version "https://raw.githubusercontent.com/Abhijithmuralidharan21/ESP32/Master/version.txt"  ///Github URL for getting firmware version.
#define URL_fw_Bin "https://raw.githubusercontent.com/Abhijithmuralidharan21/ESP32/Master/fw.bin"          ///Github URL link for the "bin" file.


void setup() {
    Serial.print("Active Firmware Version:");
    Serial.println(FirmwareVer);
    Serial.begin(115200);
    #ifdef ESP8266
    configTime(0, 0, "pool.ntp.org");      // get UTC time via NTP
    client.setTrustAnchors(&cert); // Add root certificate for api.telegram.org
    #endif

    WiFi.begin(ssid, wifiPassword);
    #ifdef ESP32
    client.setCACert(TELEGRAM_CERTIFICATE_ROOT); // Add root certificate for api.telegram.org
    #endif

    Serial.print("Connecting to WiFi");
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");

        int i = 0;
        if (i == 10) {
            ESP.restart();
        }
        i++;
    }
    Serial.println("Connected To Wifi");
}
void handleNewMessages(int numNewMessages) {
  Serial.println("handleNewMessages");
  Serial.println(String(numNewMessages));

  for (int i=0; i<numNewMessages; i++) {
    // Chat id of the requester
    String chat_id = String(bot.messages[i].chat_id);
    if (chat_id != CHAT_ID){
      bot.sendMessage(chat_id, "Unauthorized user", "");
      continue;
    }
    
    // Print the received message
    String text = bot.messages[i].text;
    Serial.println(text);

    String from_name = bot.messages[i].from_name;

    if (text == "/start") {
      String welcome = "Welcome to ESP32 controlling, " + from_name + ".\n";
      welcome += "Use the following commands to control your outputs.\n\n";
      welcome += "/control to controlling \n";
      welcome += "/update to update the firmware \n";
      
      bot.sendMessage(chat_id, welcome, "");
    }

    if (text == "/control") {
      bot.sendMessage(chat_id, "output details", "");
      
    }
    
    if (text == "/update") {
      bot.sendMessage(chat_id, "Checking for new firmware \n Current firmware version " + FirmwareVer,"");
      if (FirmwareVersionCheck()) {
         bot.sendMessage(chat_id, "Latest Firmware Version is available.....Updating......"); 
         firmwareUpdate();
      }
       else{
         bot.sendMessage(chat_id, " No updates Device is latest version");
        }
    }      
  }
}
void loop() {

    delay(1000);
    Serial.print(" Active Firmware Version:");
    Serial.println(FirmwareVer);

    if (WiFi.status() != WL_CONNECTED) {
        reconnect();
    }
    if (millis() > lastTimeBotRan + botRequestDelay)  {
      int numNewMessages = bot.getUpdates(bot.last_message_received + 1);

     while(numNewMessages) {
       Serial.println("got response");
       handleNewMessages(numNewMessages);
       numNewMessages = bot.getUpdates(bot.last_message_received + 1);
      }
    lastTimeBotRan = millis();
  }
}
void reconnect() {
    int i = 0;
    // Loop until we're reconnected
    status = WiFi.status();
    if (status != WL_CONNECTED) {
        WiFi.begin(ssid, wifiPassword);
        while (WiFi.status() != WL_CONNECTED) {
            delay(500);
            Serial.print(".");
            if (i == 10) {
                ESP.restart();
            }
            i++;
        }
        Serial.println("Connected to AP");
    }
}


void firmwareUpdate(void) {
  WiFiClientSecure client;
  client.setCACert(rootCACertificate);
  //httpUpdate.setLedPin(LED_BUILTIN, LOW);
  t_httpUpdate_return ret = httpUpdate.update(client, URL_fw_Bin);

  switch (ret) {
  case HTTP_UPDATE_FAILED:
    Serial.printf("HTTP_UPDATE_FAILD Error (%d): %s\n", httpUpdate.getLastError(), httpUpdate.getLastErrorString().c_str());
    break;

  case HTTP_UPDATE_NO_UPDATES:
    Serial.println("HTTP_UPDATE_NO_UPDATES");
    break;

  case HTTP_UPDATE_OK:
    Serial.println("HTTP_UPDATE_OK");
    break;
  }
}
int FirmwareVersionCheck(void) {
  String payload;
  int httpCode;
  String fwurl = "";
  fwurl += URL_fw_Version;
  fwurl += "?";
  fwurl += String(rand());
  Serial.println(fwurl);
  WiFiClientSecure * client = new WiFiClientSecure;

  if (client) 
  {
    client -> setCACert(rootCACertificate);

    // Add a scoping block for HTTPClient https to make sure it is destroyed before WiFiClientSecure *client is 
    HTTPClient https;

    if (https.begin( * client, fwurl)) 
    { // HTTPS      
      Serial.print("[HTTPS] GET...\n");
      // start connection and send HTTP header
      delay(100);
      httpCode = https.GET();
      delay(100);
      if (httpCode == HTTP_CODE_OK) // if version received
      {
        payload = https.getString(); // save received version
      } else {
        Serial.print("error in downloading version file:");
        Serial.println(httpCode);
      }
      https.end();
    }
    delete client;
  }
      
  if (httpCode == HTTP_CODE_OK) // if version received
  {
    payload.trim();
    if (payload.equals(FirmwareVer)) {
      Serial.printf("\nDevice already on latest firmware version:%s\n", FirmwareVer);
      return 0;
    } 
    else 
    {
      Serial.println(payload);
      Serial.println("New firmware detected");
      return 1;
    }
  } 
  return 0;  
}
