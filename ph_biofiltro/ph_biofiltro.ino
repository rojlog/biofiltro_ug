
//PH
#include <Wire.h>
#include <ThingSpeak.h>
#include <ESP8266WiFi.h>
 
const char* ssid     = "SSSSSSSS";
const char* password = "DDDDDDDD";

// ThingSpeak information
char thingSpeakAddress[] = "api.thingspeak.com";
unsigned long channelID = YOUR_CHANNEL_ID;
char* readAPIKey = "YOUR_READ_APIKEY";
char* writeAPIKey = "YOUR_WRITE_APIKEY";

unsigned int dataFieldOne = 1;      // Field to write temperature data
WiFiClient client;

float calibration_value = 21.908 - 0.08;
int phval = 0; 
unsigned long int avgval; 
int buffer_arr[10],temp;
int pH = A0;   //Se asigna el pin A0 analogico como pin para el pH
 
float ph_act;

///////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////

float readTSData( long TSChannel,unsigned int TSField ){
    
  float data =  ThingSpeak.readFloatField( TSChannel, TSField, readAPIKey );
  Serial.println( " Data read from ThingSpeak: " + String( data, 9 ) );
  return data;

}


int writeTSData( long TSChannel, unsigned int TSField, float data ){
  int  writeSuccess = ThingSpeak.writeField( TSChannel, TSField, data, writeAPIKey ); // Write the data to the channel
  if ( writeSuccess ){
    
    Serial.println( String(data) + " written to Thingspeak." );
    }
    
    return writeSuccess;
}


int write2TSData( long TSChannel, unsigned int TSField1, float field1Data ){

  ThingSpeak.setField( TSField1, field1Data );
   
  int writeSuccess = ThingSpeak.writeFields( TSChannel, writeAPIKey );
  return writeSuccess;
}


////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////


int connectWiFi(){
    WiFi.begin( ssid, password );
    while (WiFi.status() != WL_CONNECTED) {      
        delay(2500);
        Serial.println("Connecting to WiFi");
    }
    
    Serial.println( "Connected" );
    ThingSpeak.begin( client );
    Serial.println("Llegamos hasta este punto....");
}



 
void setup() {
  Serial.begin(9600);
  Serial.println("Start");
  connectWiFi();
  Wire.begin();

}
 
void loop() {
  //PH
 for(int i=0;i<10;i++) { 
  buffer_arr[i]=analogRead(pH);
  delay(10);
 }
 
 float avgvalue=0;
 for(int i=0;i<10;i++)
  avgvalue+=buffer_arr[i];
 float volt=(float)avgvalue*5.0/1023/10; 
 float ph_act = -5.7746 * volt + calibration_value;

// write2TSData( channelID , dataFieldOne , fahrenheitTemperature , dataFieldTwo , celsiusTemperature , dataFieldThree , millis() );      // Write the temperature in F, C, and time since starting.
 write2TSData( channelID , dataFieldOne , ph_act );      // Write the temperature in F, C, and time since starting.

 
  //IMPRIME TEMPERATURA

 Serial.print("pH Val= ");
 Serial.println(ph_act);

 delay(1000);
}
