# NFC-Bobine
 NFC system for filament bobines.
 
This project is thinked for give a system to your 3D printer to estimate how much filament remain on the bobine, and if is enougth to printer the object we have selected.
I know, now, you are asking youself "why?"
Think that, you have started your print, with an half bobine, you think "it's enougth", but after a long time, we speak in tens of hours, your 3d printer stop the job, until you don't change the bobine, with a new one, 'cause you miss about 1 meter of filament. 
Now you'll say me" hey man, I could resume printing job!" I tell you this is a heavy and dangerous way, many thinghs could will be wrong, so you loose all your work, a lot of filament, and a lot of bad words!
So...
I've planing to build the project in three steps:
1) project an hardware using a microcontroller like Arduino o Raspberry, a RFID reader and writer for analize the Ntag215 and in future, a tft to watch all information about the bobine;
2) connect this hardware with Klipper via MQTT service ;
3) parsing a json of moonraker, or GCode file, to extracting some information about the object that will be printed.

At this moment I've write some code for new Arduino Nano Esp32. It's provide a WiFi connection, start a mini web server that allow you to update new firmware, via OTA, display information about you bobine, and the infos that are coming from Klipper.
Arduino, after eatablished cnnection with Mosquitto, an MQTT broker easy to install on Klipper side, publish and subscribe a topic to "speak" with Klipper.

TODO 

The most important part of this project is to parsing infos about the object to print like the amount of filament used, the temperatures (extruding and bed), type of material, and so on.
Those data are storing on a json file of Moonraker or in some points of Gcode. 
