#include <XBee.h>
#include <SoftwareSerial.h>

XBee xbee = XBee();
XBeeResponse response = XBeeResponse();

uint8_t ssRX = 8;
uint8_t ssTX = 9;
SoftwareSerial nss(ssRX, ssTX);

uint8_t payload[] = {0, 0, 0};

Rx16Response rx16 = Rx16Response();
//3500 nog veranderen in MY value van master
Tx16Request tx = Tx16Request(0x3500, payload, sizeof(payload));

int boeiID = 1611; //MY value van de ontvangende xbee (had dit eig liever automatisch uitgelezen van de xbee, maar heb voorlopig nog niet gevonden hoe)
int rssi = 0;
uint8_t batterij = 100; //dit is zogezegd het batterijpercentage dat nog beschikbaar is, zal in dit programma dus niet veranderen
bool ontvangen = false;

void setup() {
  Serial.begin(9600);

  nss.begin(9600);
  xbee.setSerial(nss);
}

void loop() {

    //controleren op berichten
    xbee.readPacket();
    
    if (xbee.getResponse().isAvailable()) {
     
      xbee.getResponse().getRx16Response(rx16);
       rssi = rx16.getRssi();
                  
            Serial.print(boeiID);
            Serial.print("    ");
            Serial.print(rssi);
            Serial.print("    ");
            Serial.println(batterij);
            
            ontvangen = true;

      }
    
   
    else if (xbee.getResponse().isError()) {
      Serial.println("Error");  

    }
      if (ontvangen){
      //waarden sturen naar master
      payload[0] = boeiID;
      payload[1] = rssi;
      payload[2] = batterij;
      xbee.send(tx);
//      Serial.println("verzonden");
      ontvangen = false;
}
}
