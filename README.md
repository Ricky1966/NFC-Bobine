# NFC-Bobine
 NFC system for filament bobines.
 
This project is thinked for give a system to your 3D printer to estimate how much filament remain on the bobine, and if is enougth to printer the object we have selected.
I've planing to switch the project in three steps:
1) project an hardware using a microcontroller like Arduino o Raspberry, a RFID reader and writer for analize the Ntag215 and in future, a tft to watch all information about the bobine;
2) connect this hardware with Klipper via MQTT service ;
3) will be analized moonraker's json or GCode file to extracting some information about the object will be printed.

