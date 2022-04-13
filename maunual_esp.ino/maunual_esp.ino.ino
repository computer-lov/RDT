// wifi libraries
#include "WiFi.h"
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"
​
// network information
#define WLAN_SSID "team_15"
#define WLAN_PASS "lunabots"
#define target "192.168.0.101"
#define topicname "controller/loco"
​
// Globals
WiFiClient client;
Adafruit_MQTT_Client mqtt(&client, target, 1883, "client", "");
​
// Feeds
Adafruit_MQTT_Subscribe locomotion(&mqtt, "controller/loco");
Adafruit_MQTT_Subscribe excavation(&mqtt, "controller/exca");
Adafruit_MQTT_Subscribe emergStop(&mqtt, "controller/stop");
​
void setup()
{
  Serial.begin(115200);
  Serial1.begin(115200);
  delay(10);
  Serial.println("begin setup");
  // Connect to WiFi access point.
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(WLAN_SSID);
  WiFi.begin(WLAN_SSID, WLAN_PASS);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.print("WiFi connected. ");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.print("Attempting to subscribe at: ");
  Serial.println(target);
  // Setup MQTT subscription for onoff feed.
  mqtt.subscribe(&locomotion);
  mqtt.subscribe(&excavation);
  mqtt.subscribe(&emergStop);
}
​
void MQTT_connect();
int locoPower = 1;
int excaPower = 1;
​
bool augerToggle = 0;
bool bucketToggle = 0;
bool shovelToggle = 0;
​
void loop()
{
  // Ensure the connection to the MQTT server is alive
  MQTT_connect(); // see function definition for more information
  Adafruit_MQTT_Subscribe *subscription;
  while ((subscription = mqtt.readSubscription(5000)))
  {
    if (subscription == &locomotion) {
      // Read incoming data
      int command = String(((char *)locomotion.lastread)).toInt();
​
      // Splitting bits into a list
      bool data[6];
      // data[i] == 1 means key on, data[i] == 0 means key off
      for (int i = 5; i >= 0; i--) {
        data[i] = command & 1;
        command = command >> 1; // rightshift 1
      }
​
      // Variables used for decoding
      char idDecode = getLocoDecode(data);
​
      // Adjusting power if either Q or E are pressed
      if (data[4] && locoPower < 4) { // Q
        locoPower++;
      } else if (data[5] && locoPower > 0) { // E
        locoPower--;
      }
​
      Serial.println();
      String str = String(locoPower);
      char powerDecode = str.charAt(0);
      writeToSerial1(idDecode, powerDecode);
​
    } else if (subscription == &excavation) {
      // Read incoming data
      int command = String(((char *)excavation.lastread)).toInt();
​
      bool data[10];
      for(int i = 9; i >= 0; i--) {
        data[i] = command & 1;
        command = command >> 1;
      }
​
      // Variables used for decoding
      char idDecode = getExcaDecode(data);
      char secondNibble;
      if(isToggle(idDecode)) {
        if(idDecode == 'E') {
          secondNibble = augerToggle;
        }
        else if(idDecode == '2') {
          secondNibble = bucketToggle;
        }
        else if(idDecode == '7') {
          secondNibble = shovelToggle;
        }
      } else {
        // Adjusting power if either T or G are pressed
        if (data[6] && excaPower < 4) { // T
          excaPower++;
        } else if (data[7] && excaPower > 0) { // G
          excaPower--;
        }
        secondNibble = excaPower;
      }
​
      Serial.println();
      String str = String(secondNibble);
      char secondDecode = str.charAt(0);
      writeToSerial1(idDecode, secondDecode);
    } else if(subscription == &emergStop) {
      // Read incoming data
      int command = String(((char *)excavation.lastread)).toInt();
      bool data;
      data = command & 1;
      if(data) {
        Serial.println("EMERGENCY STOP");
        writeToSerial1('9', '0');
      }
    }
  }
  // ping the server to keep the mqtt connection alive
  // NOT required if you are publishing once every KEEPALIVE seconds
  // We need this cause we're only subscribing so we'd lose the connection
  // otherwise
  if (!mqtt.ping()) mqtt.disconnect();
}
​
char getLocoDecode(bool data[]) {
  // Checking each key and adjusting vars
  if (data[0]) {        // W
    Serial.print("Move Forward ");
    return 'A';
  } else if (data[1]) { // S
    Serial.print("Move Backward ");
    return 'B'; 
  } else if (data[2]) { // A
    Serial.print("Move Turn left ");
    return 'C';
  } else if (data[3]) { // D
    Serial.print("Move Turn right ");
    return 'D';
  } else { // WASD NOT PRESSED
    Serial.print("Stop Non-Toggles ");
    locoPower = 1;
    return '1';
  }
}
​
char getExcaDecode(bool data[]) {
  if (data[0]) {          // R 
    Serial.print("Move Auger Up ");
    return '3';
  } else if (data[1]) {   // F
    Serial.print("Move Auger Down ");
    return '4';
  } else if (data[2]) {   // B
    Serial.print("Shovel CCW ");
    return '5';
  } else if (data[3]) {   // N
    Serial.print("Shovel STOP ");
    shovelToggle = 0;
    return '7';
  } else if (data[4]) {   // M
    Serial.print("Shovel CW ");
    return '6';
  } else if (data[5]) {   // V
    Serial.print("Shovel SHAKE ");
    shovelToggle = 1;
    return '7';
  } else if (data[8]) {   // Z
    bucketToggle = !bucketToggle;
    Serial.print("Toggle Bucket: ");
    Serial.print(bucketToggle);
    return '2';
  } else if (data[9]) {   // X
    augerToggle = !augerToggle;
    Serial.print("Toggle Auger: ");
    Serial.print(augerToggle);
    return 'E';
  } else {
    Serial.print("Stop Non-Toggles ");
    excaPower = 1;
    return '1';
  }
}
​
bool isToggle(char id) {
  return id == 'E' || id == '2' || id == '7';
}
​
void writeToSerial1(char id, char speed) {
  // Using Serial for comms requires some ASCII shifting (Subtract 0x37 for letters, 0x30 for numbers)
  unsigned char commands1[2] = {id, speed};
  unsigned char id1;
  if (commands1[0] == '1') id1 = (commands1[0] - 0x30) << 4;
  else id1 = (commands1[0] - 0x37) << 4;
  unsigned char speed1 = (commands1[1] - 0x30);
  unsigned char com = id1 | speed1;
​
  // Debug prints
  Serial.println();
  Serial.print(id1, BIN);
  Serial.print(" ");
  Serial.print(speed1, BIN);
  Serial.print(" ");
  Serial.print(id1 | speed1, BIN);
  Serial.print(" ");
  Serial.println(com, HEX);
​
  // Writing to teensy
  Serial1.write(com);
}
​
// Function to connect and reconnect as necessary to the MQTT server.
// Should be called in the loop function and it will take care if connecting.
void MQTT_connect()
{
  int8_t ret;
  // Stop if already connected.
  if (mqtt.connected()) return;
​
  Serial.print("Connecting to MQTT... ");
  uint8_t retries = 5;
  while ((ret = mqtt.connect()) != 0)
  { // connect will return 0 for connected
    Serial.println(mqtt.connectErrorString(ret));
    Serial.println("Retrying MQTT connection in 5 seconds...");
    mqtt.disconnect();
    delay(5000); // wait 5 seconds
    // Infinitely try to connect to broker (TESTING)
//    retries--;
//    if (retries == 0)
//    { // basically die and wait for WDT to reset me
//      while (1)
//        ;
//    }
  }
  Serial.println("MQTT Connected!");
}
