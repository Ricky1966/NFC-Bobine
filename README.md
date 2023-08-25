# NFC-Bobine
 NFC system for filament bobines.
 
This project has been created to give your 3D printer a system to estimate how much filament is left on the spool, and if it is enough to print the object we have selected.
I know, now, you are asking youself "why?"
Think that, you have started your print, with an half bobine, you think "it's enough", but after a long time, we talk in tens of hours, your 3d printer stops working, until you change the spool with a new one, because you are missing about 1 meter of filament. 
Now you'll say to me: "hey man, I could just resume the print!" This is a bad solution because many things could go wrong, risking losing all your work, a lot of filaments and a lot of bad words!
So...
I'm planning to build this project following these three steps:
1) create the hardware using a microcontroller like a Arduino or Raspberry, a RFID reader and writer to analize the Ntag215 and in the future, a tft to read all the information about the spool;
2) connect this hardware with Klipper via MQTT service ;
3) parsing a json of moonraker, or GCode file, to extract some information about the object that will be printed.

At this moment I've written some code for new Arduino Nano Esp32. It provides a WiFi connection, starts a mini web server that allows you to update the firmware, via OTA, display information about the spool and the infos that are coming from Klipper.
Arduino, after eatablishing connection with Mosquitto, an MQTT broker easy to install on Klipper side, publish and subscribe a topic to "speak" with Klipper.

TODO 

The most important part of this project is to parsing infos about the object to print like the amount of filament used, the temperatures (extruding and bed), type of material, and so on.
Those data are storing on a json file of Moonraker or in some points of Gcode. 
I've writed some Python lines, and now I'm able to find oit the infos I need, on GCode produced with Slic3r and some of it's forks.
Next step will be to use the json of Moonraker, but at a first aigth on it, seems there aren't all the infos.
After I will dedicate myself to merge all parts together.

NOTE :
edit file NdefMessage_h
line #define MAX_NDEF_RECORDS 4
must be change to 8
