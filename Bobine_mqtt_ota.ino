#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include <Update.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <SPI.h>
#include <MFRC522.h>
#include "homepage.h"
#include "serverpage.h"
#include "loginpage.h"
#include "secrets.h"
#include "NfcAdapter.h"

#define CS_PIN 10

MFRC522 mfrc522(CS_PIN, UINT8_MAX); // Create MFRC522 instance

NfcAdapter nfc = NfcAdapter(&mfrc522);

int redPin = 14;
int greenPin = 15;
int bluePin = 16;

float col[3];
float hue = 0.0;

WebServer server(80);

WiFiClient espClient;
PubSubClient client(espClient);

long lastMsg = 0;
char msg[50];
int value = 0;

void setup(void) {
  Serial.begin(115200);

  // Connect to WiFi network
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  /*use mdns for host name resolution*/
  if (!MDNS.begin(host)) { //http://esp32.local
    Serial.println("Error setting up MDNS responder!");
    while (1) {
      delay(1000);
    }
  }
  Serial.println("mDNS responder started");
  /*return index page which is stored in serverIndex */
  server.on("/", HTTP_GET, []() {
    server.sendHeader("Connection", "close");
    server.send(200, "text/html", homeIndex);
  });
  server.on("/login", HTTP_GET, []() {
    server.sendHeader("Connection", "close");
    server.send(200, "text/html", loginIndex);
  });
  server.on("/serverIndex", HTTP_GET, []() {
    server.sendHeader("Connection", "close");
    server.send(200, "text/html", serverIndex);
  });
  /*handling uploading firmware file */
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
  /*
   * MQTT
   */
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  /*
   * NTAG
   */
   SPI.begin();        // Init SPI bus
   mfrc522.PCD_Init(); // Init MFRC522
   nfc.begin();
}

void callback(char* topic, byte* message, unsigned int length) {
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  String messageTemp;
  
  for (int i = 0; i < length; i++) {
    Serial.print((char)message[i]);
    messageTemp += (char)message[i];
  }
  Serial.println();
  if (String(topic) == "rfid") {
    Serial.print("Changing output to ");
    if(messageTemp == "clear"){
      Serial.println("clear");
      hue = 0.3;
      setColor(hsv2rgb(hue, 1.0, 1.0, col));
    } else if(messageTemp == "format"){
      Serial.println("format");
      hue = 0.4;
      setColor(hsv2rgb(hue, 1.0, 1.0, col));
    } else if(messageTemp == "read"){
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
      hue = 0.6;
      tag_write();
      setColor(hsv2rgb(hue, 1.0, 1.0, col));
    } else if(messageTemp == "erase"){
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
    if (client.connect("ESP8266Client", mqtt_user, mqtt_passwd)) {
      Serial.println("connected");
      // Subscribe
      client.subscribe("rfid");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 10 seconds before retrying
      delay(10000);
    }
  }
} 

bool tag_read(){
  if (nfc.tagPresent()){
    Serial.println("Reading NFC tag\n");
    NfcTag tag = nfc.read();
    Serial.println(tag.getTagType());
    Serial.print("UID: ");Serial.println(tag.getUidString());
    if (tag.hasNdefMessage()) // every tag won't have a message
        { NdefMessage message = tag.getNdefMessage();
          Serial.print("\nThis NFC Tag contains an NDEF Message with ");
          Serial.print(message.getRecordCount());
          Serial.println(" NDEF Record");
          client.publish("rfid", "01 01 01 01 01 01 01");
    }
    delay(5000);
    return true;
  } else {
    delay(5000);
    return false;
  }
}

void tag_write(){
  if (nfc.tagPresent()) {
    Serial.println("Writing record to NFC tag");
    NdefMessage message = NdefMessage();
    message.addTextRecord("PLA,#aabbcc,200,60,205,65,1000,32000"); //material,color,T° print,T° bed,T° first layer, T° bed first layer,weigth, length
    bool success = nfc.write(message);
    if (success) {
      Serial.println("\tSuccess. Try reading this tag with your phone.");        
      delay(10000);
    } else {
      Serial.println("\tWrite failed.");
    }
  }
  delay(5000);
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

void setColor(float *rgb) {
  analogWrite(redPin, (int)((1.0 - rgb[0]) * 255));
  analogWrite(greenPin, (int)((1.0 - rgb[1]) * 255));
  analogWrite(bluePin, (int)((1.0 - rgb[2]) * 255));  
}

// HSV->RGB conversion based on GLSL version
// expects hsv channels defined in 0.0 .. 1.0 interval
float fract(float x) { return x - int(x); }

float mix(float a, float b, float t) { return a + (b - a) * t; }

float step(float e, float x) { return x < e ? 0.0 : 1.0; }

float* hsv2rgb(float h, float s, float b, float* rgb) {
  rgb[0] = b * mix(1.0, constrain(abs(fract(h + 1.0) * 6.0 - 3.0) - 1.0, 0.0, 1.0), s);
  rgb[1] = b * mix(1.0, constrain(abs(fract(h + 0.6666666) * 6.0 - 3.0) - 1.0, 0.0, 1.0), s);
  rgb[2] = b * mix(1.0, constrain(abs(fract(h + 0.3333333) * 6.0 - 3.0) - 1.0, 0.0, 1.0), s);
  return rgb;
}

float* rgb2hsv(float r, float g, float b, float* hsv) {
  float s = step(b, g);
  float px = mix(b, g, s);
  float py = mix(g, b, s);
  float pz = mix(-1.0, 0.0, s);
  float pw = mix(0.6666666, -0.3333333, s);
  s = step(px, r);
  float qx = mix(px, r, s);
  float qz = mix(pw, pz, s);
  float qw = mix(r, px, s);
  float d = qx - min(qw, py);
  hsv[0] = abs(qz + (qw - py) / (6.0 * d + 1e-10));
  hsv[1] = d / (qx + 1e-10);
  hsv[2] = qx;
  return hsv;
}

void loop(void) {
  server.handleClient();
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  /*
  long now = millis();
  if (now - lastMsg > 10000) {
    lastMsg = now;
    hue = 0.5;
    setColor(hsv2rgb(hue, 1.0, 1.0, col));

    Serial.print("Message sended : ");
    Serial.println("CIAO");
    client.publish("esp32/", "CIAO");
    delay(1000);

    hue = 0.0;
    setColor(hsv2rgb(hue, 1.0, 1.0, col));
  }

  Serial.println("Reading NFC tag\n");
  if (nfc.tagPresent()){
    Serial.println("Reading NFC tag\n");
    NfcTag tag = nfc.read();
    tag.print();
    Serial.println(tag.getTagType());
    Serial.print("UID: ");Serial.println(tag.getUidString());
  }

  */  
  delay(5000);
}
