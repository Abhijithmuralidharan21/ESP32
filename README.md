# ESP32 OTA UPDATE USING GITHUB AND TELEGRAM 

The PROJECT is an OTA update for ESP32 from Github using telegram message

The client can update ESP32 by sending a message to telegram. After uploading the version number text file and bin file in github. 
The client send an update signal to the ESP32(receiver at any location) via telegram. 
When message from telegram is received by the ESP32, ESP32 check version number in github and if there is a version change then esp32 take the bin file from github 
location and update ESP32 with new firmware.




