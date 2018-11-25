//Receives message from neighbouring buoy to determine RSSI value
//Sends ID and RSSI value to the buoy who sent the message

#include <XBee.h>
#include <SoftwareSerial.h>
#include <Printers.h>

XBeeWithCallbacks xbee;

uint8_t ssRX = 8;
uint8_t ssTX = 9;
SoftwareSerial nss(ssRX, ssTX);
uint8_t rssi;
uint8_t buoyID = 9;
uint8_t *data;
uint8_t length;

int analogPin = 1;
int val = 0;
int voltage1 = 0;
int voltage2 = 0;
float voltage;

uint8_t payload[] = {0, 0, 0, 0};

void receive16(Rx16Response& rx, uintptr_t) {
  val = analogRead(analogPin);
  voltage = val * (5.0 / 1024.0);
  voltage1 = (int) voltage;
  voltage2 = (voltage - voltage1)*100;
  // Create a reply packet
  // This directly reuses the rx data array, which is ok since the tx
  // packet is sent before any new response is received
  // Check for reveived messages
  if (xbee.getResponse().isAvailable()) {
  xbee.getResponse().getRx16Response(rx);
  rssi = rx.getRssi();

  payload[0] = buoyID;
  payload[1] = rssi;
  payload[2] = voltage1;
  payload[3] = voltage2;
  
  Serial.print(payload[0]);
  Serial.print("    ");
  Serial.print(payload[1]);
  Serial.print("    ");
  Serial.print(payload[2]);
  Serial.print("    ");
  Serial.println(payload[3]);


 // In real life we will use the address of the zigbee on the arduino on the masterbuoy
  Tx16Request tx = Tx16Request(rx.getRemoteAddress16(), payload, sizeof(payload));
 //Tx16Request tx = Tx16Request(0xFFFF, payload, sizeof(payload)); //used to send to the master buoy
  xbee.send(tx);
  //Serial.println("Sending Tx16Request");
  }
}


void setup() {
  Serial.begin(115200);

  nss.begin(9600);
  xbee.setSerial(nss);
  // Make sure that any errors are logged to Serial. The address of
  // Serial is first cast to Print*, since that's what the callback
  // expects, and then to uintptr_t to fit it inside the data parameter.
  xbee.onPacketError(printErrorCb, (uintptr_t)(Print*)&Serial);
  xbee.onTxStatusResponse(printErrorCb, (uintptr_t)(Print*)&Serial);
  xbee.onZBTxStatusResponse(printErrorCb, (uintptr_t)(Print*)&Serial);

  // These are called when an actual packet received
  xbee.onRx16Response(receive16);

}

void loop() {
  xbee.loop();
}
