/*********
  Rui Santos
  Complete instructions at https://RandomNerdTutorials.com/esp32-wi-fi-manager-asyncwebserver/
  
  Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files.
  The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
*********/

#include <Arduino.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <AsyncTCP.h>
#include <AsyncElegantOTA.h>
#include <LittleFS.h>
#include <Preferences.h>
#include <PubSubClient.h>
#include <MFRC522.h>
#include <NfcAdapter.h>
#include <Preferences.h>
#include <ArduinoJson.h>
// Create AsyncWebServer object on port 80
#define CS_PIN 5
#define SS_PIN  5  // ESP32 pin GPIO5 
#define RST_PIN 27 // ESP32 pin GPIO27 
const char* mqtt_server = "192.168.1.101";
const char* mqtt_user = "arduino_nano_esp32";
const char* mqtt_passwd = "arduino_nano_esp32";
const char* host = "esp32";
// Search for parameter in HTTP POST request
const char* PARAM_INPUT_1 = "ssid";
const char* PARAM_INPUT_2 = "pass";
const char* PARAM_INPUT_3 = "ip";
const char* PARAM_INPUT_4 = "gateway";
// File paths to save input values permanently
const char* ssidPath = "/ssid.txt";
const char* passPath = "/pass.txt";
const char* ipPath = "/ip.txt";
const char* gatewayPath = "/gateway.txt";

//Variables to save values from HTML form
String ssid;
String pass;
String ip;
String gateway;
// Timer variables
unsigned long previousMillis = 0;
const long interval = 10000;  // interval to wait for Wi-Fi connection (milliseconds)

// which function is called?
String functionCalled;
int value = 0;
int countMsg = 0;
char uid[30];
String uid_str ;
char tag_msg[100];
String tag_msg_str ;
char wrt_msg[30];
String wrt_msg_str ;
String mat_type,mat_color,spool_lenght,spool_weigth,temp_bed,temp_ext,t_fl_b,t_fl_e;

AsyncWebServer server(80);
Preferences preferences;
IPAddress localIP;
//IPAddress localIP(192, 168, 1, 200); // hardcoded
// Set your Gateway IP address
IPAddress localGateway;
//IPAddress localGateway(192, 168, 1, 1); //hardcoded
IPAddress subnet(255, 255, 0, 0);
//MFRC522 mfrc522(CS_PIN, UINT8_MAX);
MFRC522 mfrc522(SS_PIN, RST_PIN);
NfcAdapter nfc = NfcAdapter(&mfrc522);
WiFiClient espClient;
PubSubClient client(espClient);

// aggiunte 06/09/2023
void callback(char* topic, byte* message, unsigned int length);
void reconnect();
bool tag_read();
bool tag_write();
void tag_erase();
void tag_clear();
void tag_format();
// fine aggiunte

// Initialize LittleFS
void initLittleFS() {
  if (!LittleFS.begin(true)) {
    Serial.println("An error has occurred while mounting LittleFS");
  }
  Serial.println("LittleFS mounted successfully");
}

// Read File from LittleFS
void readPref(){
  ssid = preferences.getString("ssid", "");
  pass = preferences.getString("pass", "");
  ip = preferences.getString("ip","");
  gateway = preferences.getString("gateway","");
}
/*
String readFile(fs::FS &fs, const char * path){
  Serial.printf("Reading file: %s\r\n", path);

  File file = fs.open(path);
  if(!file || file.isDirectory()){
    Serial.println("- failed to open file for reading");
    return String();
  }
  
  String fileContent;
  while(file.available()){
    fileContent = file.readStringUntil('\n');
    break;     
  }
  return fileContent;
}
*/
// Write file to LittleFS
void writePref(const char * pref, const char * message){
  preferences.putString(pref,message);
  Serial.print("Preference :");
  Serial.print(pref);
  Serial.println(" setted!");
}
/*
void writeFile(fs::FS &fs, const char * path, const char * message){
  Serial.printf("Writing file: %s\r\n", path);

  File file = fs.open(path, FILE_WRITE);
  if(!file){
    Serial.println("- failed to open file for writing");
    return;
  }
  if(file.print(message)){
    Serial.println("- file written");
  } else {
    Serial.println("- write failed");
  }
}
*/
// Initialize WiFi
bool initWiFi() {
  if(ssid=="" || ip==""){
    Serial.println("Undefined SSID or IP address.");
    return false;
  }

  WiFi.mode(WIFI_STA);
  localIP.fromString(ip.c_str());
  localGateway.fromString(gateway.c_str());


  if (!WiFi.config(localIP, localGateway, subnet)){
    Serial.println("STA Failed to configure");
    return false;
  }
  WiFi.begin(ssid.c_str(), pass.c_str());
  Serial.println("Connecting to WiFi...");

  unsigned long currentMillis = millis();
  previousMillis = currentMillis;

  while(WiFi.status() != WL_CONNECTED) {
    currentMillis = millis();
    if (currentMillis - previousMillis >= interval) {
      Serial.println("Failed to connect.");
      return false;
    }
  }

  Serial.println(WiFi.localIP());
  return true;
}

// Replaces placeholder with LED state value
String processor(const String& var) {
  if(var == "STATE") {
    //inserire chiamate a funzioni if functionCalled == write....
    if (functionCalled == "READ"){
      Serial.println("Chiamata a READ");
    } else if (functionCalled == "WRITE"){
      Serial.println("Chiamata a WRITE");
    } else if (functionCalled == "CLEAR"){
      Serial.println("Chiamata a CLEAR");
    } else if (functionCalled == "ERASE"){
      Serial.println("Chiamata a ERASE");
    } else if (functionCalled == "FORMAT"){
      Serial.println("Chiamata a FORMAT");
    }
    return functionCalled;
  }
  if(var == "UID"){
    Serial.print("UID : ");
    Serial.println(uid_str);
    return uid_str;
  }
  //SEN MAT LEN WEI TBED TEXT TFLB TFLE
   //mat_type,mat_color,spool_lenght,spool_weigth,temp_bed,temp_ext,t_fl_b,t_fl_e;
  
  if(var == "SEN"){
    Serial.println("1");
    return "1";
  }
  if(var == "MAT"){
    Serial.println(mat_type);
    return mat_type;
  }
  if(var == "COL"){
    Serial.println(mat_color);
    return mat_color;
  }
  if(var == "LEN"){
    Serial.println(spool_lenght);
    return spool_lenght;
  }
  if(var == "WEI"){
    Serial.println(spool_weigth);
    return spool_weigth;
  }
  if(var == "TBED"){
    Serial.println(temp_bed);
    return temp_bed;
  }
  if(var == "TEXT"){
    Serial.println(temp_ext);
    return temp_ext;
  }
  if(var == "TFLB"){
    Serial.println(t_fl_b);
    return t_fl_b;
  }
  if(var == "TFLE"){
    Serial.println(t_fl_e);
    return t_fl_e;
  }
  
  return String();
}

void notFound(AsyncWebServerRequest *request)
{
  request->send(404, "application/json", "{\"message\":\"Not found\"}");
}

void setup() {
  // Serial port for debugging purposes
  Serial.begin(115200);
  preferences.begin("nfc-bobine", false);
  initLittleFS();
 
  // Load values saved in LittleFS
  //ssid = readFile(LittleFS, ssidPath);
  ssid = preferences.getString("ssid","");
  //pass = readFile(LittleFS, passPath);
  pass = preferences.getString("pass","");
  //ip = readFile(LittleFS, ipPath);
  ip = preferences.getString("ip","");
  //gateway = readFile (LittleFS, gatewayPath);
  gateway =  preferences.getString("gateway","");
  //Serial.println(ssid);
  //Serial.println(pass);
  Serial.println(ip);
  //Serial.println(gateway);

  if(initWiFi()) {

    // Route for root / web page
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
      request->send(LittleFS, "/index.html", "text/html", false, processor);
    });
    server.serveStatic("/", LittleFS, "/").setDefaultFile("/index.html");
    
    // Route to set READ
    server.on("/read", HTTP_GET, [](AsyncWebServerRequest *request) {
      functionCalled = "READ";
      tag_read();
      request->send(LittleFS, "/index.html", "text/html", false, processor);
    });

    // Route to set WRITE
    server.on("/write", HTTP_GET, [](AsyncWebServerRequest *request) {
      functionCalled = "WRITE";
      request->send(LittleFS, "/index.html", "text/html", false, processor);
    });
    // Route to set FORMAT
    server.on("/format", HTTP_GET, [](AsyncWebServerRequest *request) {
      functionCalled = "FORMAT";
      request->send(LittleFS, "/index.html", "text/html", false, processor);
    });

    // Route to set ERASE
    server.on("/erase", HTTP_GET, [](AsyncWebServerRequest *request) {
      functionCalled = "ERASE";
      request->send(LittleFS, "/index.html", "text/html", false, processor);
    });

    // Route to set CLEAR
    server.on("/clear", HTTP_GET, [](AsyncWebServerRequest *request) {
      functionCalled = "CLEAR";
      request->send(LittleFS, "/index.html", "text/html", false, processor);
    });

    // Route JSON request
    server.on("/json", HTTP_GET, [](AsyncWebServerRequest *request){
      tag_read();
      StaticJsonDocument<300> data;
      data["Sensor"] = 1;
      data["UID"] = uid_str;
      data["Material"] = mat_type ;
      data["Color"] = mat_color ;
      data["Lenght"] = spool_lenght ;
      data["Weigth"] = spool_weigth ;
      data["T_Bed"] = temp_bed ;
      data["T_Ext"] = temp_ext ;
      data["T_fl_Bed"] = t_fl_b ;
      data["T_fl_Ext"] = t_fl_e ;
      String response;
      serializeJson(data, response);
      request->send(200, "application/json", response);
    });
    server.onNotFound(notFound);
    AsyncElegantOTA.begin(&server);
    server.begin();
  }
  else {
    // Connect to Wi-Fi network with SSID and password
    Serial.println("Setting AP (Access Point)");
    // NULL sets an open Access Point
    WiFi.softAP("ESP-WIFI-MANAGER", NULL);

    IPAddress IP = WiFi.softAPIP();
    Serial.print("AP IP address: ");
    Serial.println(IP); 

    // Web Server Root URL
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
      request->send(LittleFS, "/wifimanager.html", "text/html");
    });
    
    server.serveStatic("/", LittleFS, "/").setDefaultFile("/wifmanager.html");
    
    server.on("/", HTTP_POST, [](AsyncWebServerRequest *request) {
      int params = request->params();
      for(int i=0;i<params;i++){
        AsyncWebParameter* p = request->getParam(i);
        if(p->isPost()){
          // HTTP POST ssid value
          if (p->name() == PARAM_INPUT_1) {
            ssid = p->value().c_str();
            Serial.print("SSID set to: ");
            Serial.println(ssid);
            // Write file to save value
            writePref("ssid",ssid.c_str());
            //writeFile(LittleFS, ssidPath, ssid.c_str());
          }
          // HTTP POST pass value
          if (p->name() == PARAM_INPUT_2) {
            pass = p->value().c_str();
            Serial.print("Password set to: ");
            Serial.println(pass);
            // Write file to save value
            writePref("pass",pass.c_str());
            //writeFile(LittleFS, passPath, pass.c_str());
          }
          // HTTP POST ip value
          if (p->name() == PARAM_INPUT_3) {
            ip = p->value().c_str();
            Serial.print("IP Address set to: ");
            Serial.println(ip);
            // Write file to save value
            writePref("ip", ip.c_str());
            //writeFile(LittleFS, ipPath, ip.c_str());
          }
          // HTTP POST gateway value
          if (p->name() == PARAM_INPUT_4) {
            gateway = p->value().c_str();
            Serial.print("Gateway set to: ");
            Serial.println(gateway);
            // Write file to save value
            writePref("gateway",gateway.c_str());
            //writeFile(LittleFS, gatewayPath, gateway.c_str());
          }
          //Serial.printf("POST[%s]: %s\n", p->name().c_str(), p->value().c_str());
        }
      }
      request->send(200, "text/plain", "Done. ESP will restart, connect to your router and go to IP address: " + ip);
      delay(3000);
      ESP.restart();
    });
    server.begin();
  }
  // aggiunte 06/09/2023
  server.begin();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  SPI.begin();        // Init SPI bus
  mfrc522.PCD_Init(); // Init MFRC522
  nfc.begin();
  // fine aggiunta
}

void loop() {
}
// aggiunte 06/09/2023
void callback(char* topic, byte* message, unsigned int length) {
  countMsg +=1;
  Serial.print("Message arrived n° ");
  Serial.print(countMsg,DEC);
  String messageTemp;
  
  for (int i = 0; i < length; i++) {
    //Serial.print((char)message[i]);
    messageTemp += (char)message[i];
  }
  //Serial.println();
  if (String(topic) == "rfid") {
    //Serial.print("Changing output to ");
    if(messageTemp == "read"){
      Serial.println("read");
      if (tag_read()){
        Serial.println("OK!!!");  
      } else {
        Serial.println("NO!!!");
      }
    } else if(messageTemp == "write"){
      Serial.println("write");
      if (tag_write()){ //inserire i dati per ora sono standard
        Serial.println("OK!!!");  
      } else {
        Serial.println("NO!!!");
      }
    } else if(messageTemp == "clear"){  //will be implemented asap
      Serial.println("clear");
    } else if(messageTemp == "format"){  //will be implemented asap
      Serial.println("format");
    }  else if(messageTemp == "erase"){  //will be implemented asap
      Serial.println("erase");
    }
  }
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("Arduino_nano_esp32", mqtt_user, mqtt_passwd)) {
      Serial.println("connected");
      // Subscribe
      client.subscribe("rfid");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 10 seconds");
      // Wait 10 seconds before retrying
      delay(10000);
    }
  }
} 

bool tag_read(){
  Serial.println("TAG.......");
  if (nfc.tagPresent()){
    Serial.println("Reading NFC tag");
    NfcTag tag = nfc.read();
    Serial.print("UID: ");
    uid_str = tag.getUidString(); //
    uid_str.toCharArray(uid, uid_str.length() + 1); //packaging up the data to publish to mqtt whoa...
    Serial.println(uid_str);
    if (tag.hasNdefMessage()){ 
      NdefMessage message = tag.getNdefMessage();
      Serial.print("This NFC Tag contains an NDEF Message with ");
      Serial.print(message.getRecordCount());
      Serial.println(" NDEF Records");

      client.publish("rfid", uid);
      int recordCount = message.getRecordCount();
      for (int i = 0; i < recordCount; i++){
        Serial.print("NDEF Record ");
        Serial.println(i+1);
        NdefRecord record = message.getRecord(i);
        // NdefRecord record = message[i]; // alternate syntax
        int payloadLength = record.getPayloadLength();
        const byte *payload = record.getPayload();
        String tag_msg_str = "";
        for (int c = 3; c < payloadLength; c++) {
          tag_msg_str += (char)payload[c];
        }
        Serial.print("  Payload (as String): ");                   
        tag_msg_str.toCharArray(tag_msg, tag_msg_str.length() + 1);
        client.publish("rfid", tag_msg);
        Serial.println(tag_msg_str);
        //mat_type,mat_color,spool_lenght,spool_weigth,temp_bed,temp_ext,t_fl_b,t_fl_e;
        switch (i) {
          case 0:
            mat_type = tag_msg_str;
            break;
          case 1:
            mat_color = tag_msg_str;
            break;
          case 2:
            spool_lenght = tag_msg_str;
            break;
          case 3:
            spool_weigth = tag_msg_str;
            break;
          case 4:
            temp_bed = tag_msg_str; 
            break;
          case 5:
            temp_ext = tag_msg_str;
            break;
          case 6:
            t_fl_b = tag_msg_str;
            break;
          case 7:
            t_fl_e = tag_msg_str;
            break;
          default :
            break;
        }
        if(i == 0){
          
        }
      }
    }
    delay(500);
    return true;
  } else {
    delay(500);
    return false;
  }
}

bool tag_write(){
  return true;
}

void tag_format(){
}

void tag_clear(){
}

void tag_erase(){
}
// fine aggiunta