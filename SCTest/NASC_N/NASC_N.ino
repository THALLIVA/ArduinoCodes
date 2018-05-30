//#include <SoftwareSerial.h>
//SoftwareSerial sw(14,12);
//SoftwareSerial sw2(4,16);
#include <ESP8266WiFi.h>

#include <ArduinoJson.h>
StaticJsonBuffer<100> jsonBuffer;

/*
const char* ssid = "MiW";
const char* password = "9870533756";
const char* mqtt_server = "192.168.31.98";
*/
const char* ssid = "sakec";
const char* password = "34567890";
const char* mqtt_server = "192.168.43.229";
WiFiClient espClient;

String value;

void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}


#include <PubSubClient.h>
PubSubClient client(espClient);

void callback(char* topic, byte* payload, unsigned int length) {
  digitalWrite(5,HIGH);
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("ESP8266Client")) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish("outTopic", "hello world");
      // ... and resubscribe
      client.subscribe("fan");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(9600);
  Serial.println("Starting...");
    Serial.println("Setting wifi...");
  //swSer.begin(9600);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  //Serial1.begin(9600);
 //sw.begin(9600);
 //sw2.begin(9600);
 pinMode(5, OUTPUT);
}

void loop() {

  if (!client.connected()) {
    reconnect();
  }
  Serial.println(Serial.available());
  if(Serial.available() > 0)
  {

    value = Serial.readString();
    Serial.print(value);
    char val[100];
    value.toCharArray(val,100);
    JsonObject& root = jsonBuffer.parseObject(val);
    client.publish(root["topic"], root["value"]);

//    if(root["value"] > 25)  {
//      digitalWrite(5,HIGH);
//      client.publish("FAN", "FAN ON");
//      } //turn relay ON
//  
//  else {
//    client.publish("FAN", "FAN OFF");
//    digitalWrite(5,LOW);
//    } //turn relay OFF
  
  //delay(2500);
      
  }

  client.loop();

  /*
  if(sw.available() > 0)
  {
    value = sw.readString();
    sw.print(value);
    char val[10];
    value.toCharArray(val,10);
    client.publish("outTopic2", val);
  }

  if(sw2.available() > 0)
  {
    value = sw2.readString();
    sw2.print(value);
    char val[10];
    value.toCharArray(val,10);
    client.publish("outTopic1", val);
  }
  */
}
