# ESP32 OTA UPDATE USING GITHUB AND TELEGRAM 

**The PROJECT is an OTA update for ESP32 from Github using update message from Telegram**

The client can update ESP32 by sending a message to telegram. After uploading the version number text file and bin file in github. 
The client send an update signal to the ESP32(receiver at any location) via telegram. 
When message from telegram is received by the ESP32, ESP32 check version number in github and if there is a version change then esp32 take the bin file from github 
location and update ESP32 with new firmware.

**cert.h -- The root CA certificate file.**

**fw.bin -- the new firmware bin file.**

**OTAUPDATETELEGRAM.ino -- program**


**How to create Telegram Bot for ESP32**

*The ESP32 will interact with the Telegram bot to receive and handle the messages, and send responses.
*Open Telegram and follow the next steps to create a Telegram Bot. First, search for “botfather” and click the BotFather.
*The following window should open and you’ll be prompted to click the start button.
*Type /newbot and follow the instructions to create your bot. Give it a name and username.
If your bot is successfully created, you’ll receive a message with a link to access the bot and the bot token. Save the bot token because you’ll need it so that the ESP32 can interact with the bot.
Anyone that knows your bot username can interact with it. To make sure that we ignore messages that are not from our Telegram account (or any authorized users), you can get your Telegram User ID. Then, when your telegram bot receives a message, the ESP can check whether the sender ID corresponds to your User ID and handle the message or ignore it.
In your Telegram account, search for “IDBot”.Start a conversation with that bot and type /getid. You will get a reply back with your user ID. Save that user ID.

Two Libraries required for Telegram bot : ArduinoJson Library & Universal Telegram Bot Library
