#include <WiFiNINA.h>
#include "ThingSpeak.h"
#include "DHT.h"

// Wi-Fi credentials
char ssid[] = "NETGEAR21";   
char pass[] = "69699696";   
int keyIndex = 0;            
WiFiClient client;

// ThingSpeak settings
unsigned long myChannelNumber = 2467247; // Correct data type for the Channel ID
const char * myWriteAPIKey = "EDT7XGO6P5XWNIBU"; // Replace with your actual ThingSpeak Write API Key

// DHT22 sensor settings
#define DHTPIN 2          // Digital pin connected to the DHT22
#define DHTTYPE DHT22     // DHT 22 (AM2302)
DHT dht(DHTPIN, DHTTYPE); // Initialize DHT sensor

void setup() {
  Serial.begin(115200);      
  while (!Serial) {
    ; // Wait for serial port to connect
  }
  
  dht.begin();              // Initialize the DHT sensor
  ThingSpeak.begin(client); // Initialize ThingSpeak 
}

void loop() {
  // Check Wi-Fi connection
  if(WiFi.status() != WL_CONNECTED){
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    while(WiFi.status() != WL_CONNECTED){
      WiFi.begin(ssid, pass);  
      Serial.print(".");
      delay(5000);     
    } 
    Serial.println("\nConnected.");
  }

  // Reading temperature and humidity
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();

  // Check if any reads failed
  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  // Set ThingSpeak fields
  ThingSpeak.setField(1, temperature);
  ThingSpeak.setField(2, humidity);

  // Write to ThingSpeak
  int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
  if(x == 200){
    Serial.println("Channel update successful.");
  }
  else{
    Serial.println("Problem updating channel. HTTP error code " + String(x));
  }
  
  delay(5000); // ThingSpeak API rate limit (15s minimum)
}
