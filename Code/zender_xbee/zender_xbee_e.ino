//Sends message to neighbouring buoy
//Receives ID and RSSI value of neighbouring buoy

#include <XBee.h>
#include <SoftwareSerial.h>

XBee xbee = XBee();

//uint8_t buoyID = 7;
uint8_t ssRX = 8;
uint8_t ssTX = 9;
SoftwareSerial nss(ssRX, ssTX);

uint8_t *data;
uint8_t length;

uint8_t payload[] = { 0, 0, 0, 0, 0, 0};
Tx16Request tx = Tx16Request(0x1611, payload, sizeof(payload));
TxStatusResponse txStatus = TxStatusResponse();

Rx16Response rx16 = Rx16Response();

void send16 () {
    payload[0] = 's';
    payload[1] = 't';
    payload[2] = 'a';
    payload[3] = 't';
    payload[4] = 'u';
    payload[5] = 's';
    
    xbee.send(tx);
    //Serial.println("Bericht verzonden");
 
    // after sending a tx request, we expect a status response
    // wait up to 5 seconds for the status response
    if (xbee.readPacket(5000)) {
        // got a response!
        //Serial.println("Ontvangen bericht");
    
       
       // should be a znet tx status              
      if (xbee.getResponse().getApiId() == TX_STATUS_RESPONSE) {
           //Serial.println("is status respons");
         xbee.getResponse().getTxStatusResponse(txStatus);
        
         // get the delivery status, the fifth byte
           if (txStatus.getStatus() == SUCCESS) {
                //Serial.println("status respons is success");
             
           } else {
              // the remote XBee did not receive our packet
              //Serial.println("status respons is niet succes, remote heeft bericht niet ontvangen");
           }
        }      
    } else if (xbee.getResponse().isError()) {
      //Serial.println("Error reading packet.  Error code: ");  
      //Serial.println(xbee.getResponse().getErrorCode());
    } else {
      //Serial.println("Error");
    }
    
    delay(1000);
}

// Check for reveived messages
// In real life this message will be received by the arduino on the master buoy
void recieve16(){
  xbee.readPacket();
  if (xbee.getResponse().isAvailable()) {
     xbee.getResponse().getRx16Response(rx16);
     data = rx16.getData();
     length = rx16.getDataLength();
     Serial.print("00");
     for(int i = 0; i<length; i++){
       Serial.print(data[i]);
     }
     Serial.println();
            
}
}

void setup() {
  nss.begin(9600);
  xbee.setSerial(nss);

  Serial.begin(115200);
}


void loop() {
send16();
recieve16();

}
