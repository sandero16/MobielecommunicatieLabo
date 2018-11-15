#include <XBee.h>
#include <SoftwareSerial.h>

XBee xbee = XBee();
 
uint8_t ssRX = 8;
uint8_t ssTX = 9;
SoftwareSerial nss(ssRX, ssTX);

uint8_t payload[] = { 0, 0, 0, 0, 0, 0};

//master MY value=3005
//slave MY value=1611
Tx16Request tx = Tx16Request(0x1611, payload, sizeof(payload));

TxStatusResponse txStatus = TxStatusResponse();

void setup() {
  nss.begin(9600);
  xbee.setSerial(nss);

  Serial.begin(9600);
}

void loop() {

    payload[0] = 's';
    payload[1] = 't';
    payload[2] = 'a';
    payload[3] = 't';
    payload[4] = 'u';
    payload[5] = 's';
    
    xbee.send(tx);
    Serial.println("Bericht verzonden");
 
    // after sending a tx request, we expect a status response
    // wait up to 5 seconds for the status response
    if (xbee.readPacket(5000)) {
        // got a response!
        Serial.println("Ontvangen bericht");
    
       
       // should be a znet tx status            	
    	if (xbee.getResponse().getApiId() == TX_STATUS_RESPONSE) {
           Serial.println("is status respons");
    	   xbee.getResponse().getTxStatusResponse(txStatus);
    		
    	   // get the delivery status, the fifth byte
           if (txStatus.getStatus() == SUCCESS) {
                Serial.println("status respons is success");
             
           } else {
            	// the remote XBee did not receive our packet. is it powered on?
             	Serial.println("status respons is niet succes, remote heeft bericht niet ontvangen");
           }
        }      
    } else if (xbee.getResponse().isError()) {
      Serial.println("Error reading packet.  Error code: ");  
      Serial.println(xbee.getResponse().getErrorCode());
    } else {
      Serial.println("Error");
    }
    
    delay(1000);
}
