#include <XBee.h>
#include <SoftwareSerial.h>

XBee xbee = XBee();
XBeeResponse response = XBeeResponse();

uint8_t ssRX = 8;
uint8_t ssTX = 9;
SoftwareSerial nss(ssRX, ssTX);

Rx16Response rx16 = Rx16Response();
Rx64Response rx64 = Rx64Response();

int statusLed = 13;

uint8_t option = 0;
uint8_t *data;
uint8_t length;
uint16_t zender;

void flashLed(int times, int wait) {
    
    for (int i = 0; i < times; i++) {
      digitalWrite(statusLed, HIGH);
      delay(wait);
      digitalWrite(statusLed, LOW);
      
      if (i + 1 < times) {
        delay(wait);
      }
    }
}

void setup() {
  pinMode(statusLed, OUTPUT);
  Serial.begin(9600);

  nss.begin(9600);
  xbee.setSerial(nss);

}

// continuously reads packets, looking for RX16 or RX64
void loop() {

    xbee.readPacket();
    
    if (xbee.getResponse().isAvailable()) {
      Serial.println("iets ontvangen");
      flashLed(1,100);
         
      if (xbee.getResponse().getApiId() == RX_16_RESPONSE || xbee.getResponse().getApiId() == RX_64_RESPONSE) {
       Serial.println("ontvangen bericht is rx bericht");
        
        if (xbee.getResponse().getApiId() == RX_16_RESPONSE) {
            Serial.println("rx16 bericht");
            xbee.getResponse().getRx16Response(rx16);
            Serial.print("RSSI = ");
            Serial.println( rx16.getRssi() );
        	option = rx16.getOption();
        	data = rx16.getData();
          length = rx16.getDataLength();
          zender = rx16.getRemoteAddress16();
        }// else {
//            Serial.println("rx64 bericht");
//            xbee.getResponse().getRx64Response(rx64);
//            option = rx64.getOption();
//        	  data = rx64.getData();
//            length = rx64.getDataLength();
//            zender = rx64.getRemoteAddress64();
//        }
        Serial.print("option: ");
        Serial.println(option);
        Serial.print("zender: ");
        Serial.println(zender);
        Serial.print(", data: ");
        for(int i = 0; i<length; i++)
          Serial.print((char)data[i]);
        Serial.println();

      

      } else {
      	Serial.println("geen rx bericht");    
      }
    } else if (xbee.getResponse().isError()) {
      Serial.println("Error reading packet.  Error code: ");  
      Serial.println(xbee.getResponse().getErrorCode());
    }


    
}
