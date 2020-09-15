#include "DHT.h"
#define DHTPIN 2     
#define DHTTYPE DHT22
HardwareSerial & simcom = Serial1;
//Tx Rx simcom di Serial1 mega
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600);
  dht.begin();
  simcomInit();
}

void loop() {  
  delay(2000);
  //"AT+HTTPPARA=\"URL\",\"http://ptsv2.com/t/yo2h1-1600138268/post?hum=70&&temp=33\"",3000);
  String alamat = "AT+HTTPPARA=\"URL\",\"http://ptsv2.com/t/yo2h1-1600138268/post?";
  String hu = "hum";
  String sam= "=";
  String te = "&&temp";
  String ku= "\"";
  //String data = nama + String(umur);
 
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  float f = dht.readTemperature(true);
  
  String lengkap = alamat + hu + sam + String (h) + te + sam + String (t) + ku ;
   
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  float hif = dht.computeHeatIndex(f, h);
  float hic = dht.computeHeatIndex(t, h, false);
  sendCmd("AT",300);
  sendCmd("AT+CGATT=1",5000);
  sendCmd("AT+SAPBR=3,1,\"CONTYPE\",\"GPRS\"",3000);
  sendCmd("AT+SAPBR=3,1,\"APN\",\"internet\"",3000);
  sendCmd("AT+SAPBR=2,1",3000);
  sendCmd("AT+SAPBR=1,1",3000);
  sendCmd("AT+HTTPINIT",3000);
  sendCmd("AT+HTTPPARA=\"CID\",1",3000);
  sendCmd(lengkap,1000);   
  sendCmd("AT+HTTPACTION=0",3000);
  sendCmd("AT+HTTPREAD",3000);
  sendCmd("AT+HTTPTERM",3000);
  
  Serial.println("Connection Established");
}

void simcomInit(){
  Serial.println("Opening MODEM Communication");
  simcom.begin(9600);
  Serial.println("Modem SETTED!");
}

String sendCmd (String command , const int timeout){
  String response = "";
  simcom.println(command);
  long int time = millis();
  int i = 0;
  while ( (time+timeout ) > millis()){
    while (simcom.available()){
      char c = simcom.read();
      response +=c;
    }
  }
  Serial.println(response);
  return response;
}
