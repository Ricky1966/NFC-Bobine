#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include <Update.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <SPI.h>
#include <MFRC522.h>
#include "NfcAdapter.h"
#include "hsv_functions.h"
#include "page_home.h"
#include "page_login.h"
#include "page_ota.h"
#include "secrets.h"

#define CS_PIN 10

float col[3];
float hue = 0.0;
long lastMsg = 0;
char msg[50];
int value = 0;
int countMsg = 0;
char uid[30];
String uid_str ;
char tag_msg[100];
String tag_msg_str ;
char wrt_msg[30];
String wrt_msg_str ;

MFRC522 mfrc522(CS_PIN, UINT8_MAX); 
NfcAdapter nfc = NfcAdapter(&mfrc522);
WebServer server(80);
WiFiClient espClient;
PubSubClient client(espClient);

void setup(void) {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  Serial.println("");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  if (!MDNS.begin(host)) { //http://esp32.local
    Serial.println("Error setting up MDNS responder!");
    while (1) {
      delay(1000);
    }
  }
  Serial.println("mDNS responder started");
  
  server.on("/", HTTP_GET, []() {
    server.sendHeader("Connection", "close");
    server.send(200, "text/html", Page_Home()
    );
  });
  server.on("/login", HTTP_GET, []() {
    server.sendHeader("Connection", "close");
    server.send(200, "text/html", Page_Login());
  });
  server.on("/serverIndex", HTTP_GET, []() {
    server.sendHeader("Connection", "close");
    server.send(200, "text/html", Page_OTA());
  });
  server.on("/update", HTTP_POST, []() {
    server.sendHeader("Connection", "close");
    server.send(200, "text/plain", (Update.hasError()) ? "FAIL" : "OK");
    ESP.restart();
  }, []() {
    HTTPUpload& upload = server.upload();
    if (upload.status == UPLOAD_FILE_START) {
      Serial.printf("Update: %s\n", upload.filename.c_str());
      if (!Update.begin(UPDATE_SIZE_UNKNOWN)) { //start with max available size
        Update.printError(Serial);
      }
    } else if (upload.status == UPLOAD_FILE_WRITE) {
      /* flashing firmware to ESP*/
      if (Update.write(upload.buf, upload.currentSize) != upload.currentSize) {
        Update.printError(Serial);
      }
    } else if (upload.status == UPLOAD_FILE_END) {
      if (Update.end(true)) { //true to set the size to the current progress
        Serial.printf("Update Success: %u\nRebooting...\n", upload.totalSize);
      } else {
        Update.printError(Serial);
      }
    }
  });
  server.begin();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  SPI.begin();        // Init SPI bus
  mfrc522.PCD_Init(); // Init MFRC522
  nfc.begin();
}

void callback(char* topic, byte* message, unsigned int length) {
  countMsg +=1;
  Serial.print("Message arrived n° ");
  Serial.print(countMsg,DEC);
  //Serial.print(" on topic: ");
  //Serial.print(topic);
  //Serial.print(". Message: ");
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
      hue = 0.5;
      setColor(hsv2rgb(hue, 1.0, 1.0, col));
    } else if(messageTemp == "write"){
      Serial.println("write");
      if (tag_write("PLA")){ //inserire i dati per ora sono standard
        Serial.println("OK!!!");  
      } else {
        Serial.println("NO!!!");
      }
      hue = 0.8;
      setColor(hsv2rgb(hue, 1.0, 1.0, col));
    } else if(messageTemp == "clear"){  //will be implemented asap
      Serial.println("clear");
      hue = 0.3;
      setColor(hsv2rgb(hue, 1.0, 1.0, col));
    } else if(messageTemp == "format"){  //will be implemented asap
      Serial.println("format");
      hue = 0.4;
      setColor(hsv2rgb(hue, 1.0, 1.0, col));
    }  else if(messageTemp == "erase"){  //will be implemented asap
      Serial.println("erase");
      hue = 0.6;
      setColor(hsv2rgb(hue, 1.0, 1.0, col));
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
  if (nfc.tagPresent()){
    Serial.println("Reading NFC tag");
    NfcTag tag = nfc.read();
    //tag.print();
    //Serial.println(tag.getTagType());
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
      }
    }
    delay(500);
    return true;
  } else {
    delay(500);
    return false;
  }
}

bool tag_write(String material){
  if (nfc.tagPresent()) {
    Serial.println("Writing record to NFC tag");
    NdefMessage message = NdefMessage();
    wrt_msg_str = "material = "+ material;
    wrt_msg_str.toCharArray(wrt_msg, wrt_msg_str.length() + 1);
    Serial.println(wrt_msg_str);
    message.addTextRecord(wrt_msg); 
    message.addTextRecord("color = #aabbcc");
    message.addTextRecord("print_temp = 200");
    message.addTextRecord("bed_temp = 60");
    message.addTextRecord("f_layer_t = 205");
    message.addTextRecord("f_layer_bd = 65");
    message.addTextRecord("weight = 1000");
    message.addTextRecord("length = 32000");
    bool success = nfc.write(message);
    if (success) {
      Serial.println("Success!");   
      client.publish("rfid", "SUCCESS!");     
      delay(500);
      return true;
    } else {
      Serial.println("Write failed.");
      client.publish("rfid", "ERROR!");
      return false;
    }
  }
  delay(500);
}

void tag_format(){
  if (nfc.tagPresent()) {
    Serial.println("Formatting tag");
    bool success = nfc.format();
    if (success) {
      Serial.println("\tSuccess, tag formatted as NDEF.");
      delay(10000);
    } else {
      Serial.println("\tFormat failed, card may already be formatted.");
    }
  }
  delay(5000);
}

void tag_clear(){
  if (nfc.tagPresent()) {
    Serial.println("Cleaning tag");
    bool success = nfc.clean();
    if (success) {
      Serial.println("\tSuccess, tag restored to factory state.");
      delay(10000);
    } else {
      Serial.println("\tError, unable to clean tag.");
    }
  }
  delay(5000);
}

void tag_erase(){
  if (nfc.tagPresent()) {
    Serial.println("Erasing tag");
    bool success = nfc.erase();
    if (success) {
      Serial.println("\tSuccess, tag contains an empty record.");        
      delay(10000);
    } else {
      Serial.println("\tUnable to erase tag.");
    }
  }
  delay(5000);
}

void loop(void) {
  server.handleClient();
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}
