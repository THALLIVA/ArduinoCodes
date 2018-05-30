#include <PubSubClient.h>
#include<DHT.h> 
#include <DHT_U.h>//#include<dht.11>
#include<ESP8266WiFi.h>

#include <Adafruit_ADS1015.h>
#include <Wire.h> 
#define DHTTYPE DHT11

Adafruit_ADS1115 ads;
//ads.setGain(GAIN_ONE); 




const char* ssid = "DarshShah" ;
const char* password = "23894085Dks";
const char* mqtt_server = "192.168.0.103";
int DHTPin = 13; //GPIO 13 for DHT sensor D7

int LDRPin = A0; //A0 for Photo Resistor
int LDRValue = 0; // variable to store the value coming from the photosensor
int soilMoisture;
int val=0;

DHT dht(DHTPin,DHTTYPE);
WiFiClient server;
PubSubClient client(server);
//Functions that will be used..
void setup_wifi();
void callback(char* topic, byte* payload, unsigned int length);
void reconnect();
void getandsendDHTvalues();
void getLightIntensity();

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  ads.setGain(GAIN_ONE); 
  Wire.begin();
  ads.begin();
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);


}
void setup_wifi(){
  delay(10);
  Serial.println();
  Serial.print("Connecting to");
  Serial.println(ssid);

  WiFi.begin(ssid,password);
  while(WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");

    }
  Serial.println("");
  Serial.println("WifiConnected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  }

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");

  for(int i=0; i< length ; i++){
    Serial.println((char)payload[i]);
    }
Serial.println();

  }

void reconnect(){
while(!client.connected()){
  Serial.println("Attempting MQTT connection..");
  if(client.connect("ESP8266CLient")){
    client.subscribe("inTopic");
  }

  Serial.print("failed,rc=");
  Serial.print(client.state());
  Serial.println("Try Again In 5 Sec");
  delay(5000);
        }
      }


//1.DHT
void  getandsendDHTvalues()
{
    int chk = dht.read(DHTPin);
    float t = dht.readTemperature();
    float h = dht.readHumidity();
    if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
    }
    Serial.print("Humidity");
    Serial.print(h);
    Serial.print("\t");
    Serial.print("Temperature");
    Serial.println(t);
    Serial.print("*C");

    String temperature = String(t);
    String humidity = String(h);

    Serial.print( "Sending temperature and humidity : [" );
    Serial.print( temperature ); Serial.print( "," );
    Serial.print( humidity );
    Serial.print( "]   -> " );

    String payload1 = "{";
    payload1 += "\"temperature\":"; payload1 += temperature; 
    payload1 += ",";
    payload1 += "\"humidity\":"; payload1 += humidity;
    payload1 += "}";

    char atrributes1[100];
    payload1.toCharArray(atrributes1,100);
    client.publish("getTemperature",atrributes1);
    Serial.println(atrributes1);
  }



//2.PHOTORESISTOR 
void getandsendLightIntensity()
{
 LDRValue = 1000 - analogRead(LDRPin);
 
 String LDRString = String(LDRValue);
 Serial.print( "Sending LDRValues: [" );
 Serial.print( LDRString );
 Serial.print( "]   -> " );

 String payload2 = "{";
 payload2 += "\"Luminosity\":"; payload2 += LDRString; 
 payload2 += "}";

 char atrributes2[100];
 payload2.toCharArray(atrributes2,100);
 client.publish("getLightIntensity",atrributes2);
 Serial.println(atrributes2);
}

//3.SoilMositure 
void getandsendsoilMositure()
{
  
  soilMoisture = ads.readADC_SingleEnded(0);  
  Serial.print("Soil Moisture Average Value: ");   
  Serial.print(soilMoisture);  
  float voltage1  = (soilMoisture * 0.125)/1000;  
  Serial.print("Soil Moisture Voltage Output: ");   
  Serial.println(voltage1, 6);
  Serial.println(""); 
  
  String SoilMoistureString = String(soilMoisture);
  Serial.print( "Sending SoilMoistureValues : [" );
  Serial.print( SoilMoistureString );
  Serial.print( "]   -> " );

  String payload3 = "{";
  payload3 += "\"Soil Moisture: \":"; payload3 += SoilMoistureString ; 
  payload3 += "}";

  char atrributes3[100];
  payload3.toCharArray(atrributes3,100);
  client.publish("getSoilMoisture",atrributes3);
  Serial.println(atrributes3);
  delay(2000);  
}
 
  

  
 



/*  
4.AirQuality MQ135
  void getandsendAirQuality()
  {
  
  
  }

*/



void loop() {
  // put your main code here, to run repeatedly:

  if(!client.connected()){
    reconnect();
    
  }
  client.loop();

  getandsendDHTvalues();
  getandsendLightIntensity();
  getandsendsoilMositure();
  delay(5000);
}

