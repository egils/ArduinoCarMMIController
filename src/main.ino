#include <SoftwareSerial.h>
#include <SPI.h>
#include <mcp_can.h>

#define BLE_ENABLED false
#define CAN_BUS_ENABLED true

#define RING 0x428
#define DEVICE 0x436
#define AUDIO_SOURCE 0x661

#define DIS_TEXT_line1 0x261
#define DIS_TEXT_line2 0x263

unsigned char requestToJoin[6] = {0x16, 0x02, 0xC0, 0x04, 0x00, 0x00};
unsigned char okToJoin[6] = {0x16, 0x01, 0x00, 0x00, 0x00, 0x00};
unsigned char ack[6] = {0x08, 0x01, 0x00, 0x00, 0x00, 0x00};
unsigned char fmRadio[8] = {0x81, 0x01, 0x12, 0xA0, 0x00, 0x00, 0x00, 0x00};

unsigned char length = 0;
unsigned char buf[8];
char str[20];

MCP_CAN CAN(10);
SoftwareSerial BTSerial(5, 6); // RX | TX

String currentMessage;

void setup() {
  // Init BLuetooth (BLE) and debugging Serial
  Serial.begin(115200);

  if (BLE_ENABLED) {
    BTSerial.begin(115200);
  }

  // Init CANBUS
  if (CAN_BUS_ENABLED) {
    START_INIT:

    if(CAN_OK == CAN.begin(CAN_200KBPS))                   // init can bus : baudrate = 100k
    {
        Serial.println("CAN BUS Shield init ok!");
    }
    else
    {
        Serial.println("CAN BUS Shield init fail");
        Serial.println("Init CAN BUS Shield again");
        delay(100);
        goto START_INIT;
    }

    CAN.sendMsgBuf(DEVICE, 0, 6, requestToJoin);
    delay(100);
    CAN.sendMsgBuf(DEVICE, 0, 6, ack);
  }
}

void loop() {
  if (CAN_BUS_ENABLED) {
    CAN.sendMsgBuf(AUDIO_SOURCE, 0, 8, fmRadio);
    delay(100); // should report status only once in 100ms

    unsigned char DIS1Message[8] = {0x4c, 0x41, 0x42, 0x41, 0x53, 0x2c, 0x00, 0x00};
    CAN.sendMsgBuf(DIS_TEXT_line1, 0, 8, DIS1Message);

    unsigned char DIS2Message[8] = {0x50, 0x41, 0x53, 0x41, 0x55, 0x4c, 0x49, 0x21};
    CAN.sendMsgBuf(DIS_TEXT_line2, 0, 8, DIS2Message);

    // readCANMessage();
  }

  if (BLE_ENABLED) {
    handleBLEmessage();
  }

}

void handleBLEmessage() {
  if(BTSerial.available()) {
    currentMessage = BTSerial.readString();
  }

  if (currentMessage.length() > 0) {
    unsigned int bufSize = currentMessage.length() > 8 ? 8 : currentMessage.length();

    if (CAN_BUS_ENABLED) {
      unsigned char* buf;
      currentMessage.getBytes(buf, bufSize, 0);
      CAN.sendMsgBuf(DIS_TEXT_line1, 0, bufSize, buf);
    } else {
      Serial.write(currentMessage.begin(), bufSize);
    }

    currentMessage.remove(0, bufSize);

  }


  if(Serial.available()) {
    char message = Serial.read();

    BTSerial.write(message);
    Serial.write(message);
  }
}

void readCANMessage() {
  unsigned char buffer[8];
  INT32U canId = 0x000;

  if(CAN_MSGAVAIL == CAN.checkReceive()) {
    CAN.readMsgBuf(&length, buffer);

    canId = CAN.getCanId();

    Serial.print(canId, HEX); Serial.print('>');

    for(int i = 0; i<length; i++) {
      if (buffer[i]<16) {
        Serial.print('0');
      }

      Serial.print(buffer[i],HEX);

      if (i<length-1) {
        Serial.print(":");
      }
    }

    Serial.println();
  }
}
