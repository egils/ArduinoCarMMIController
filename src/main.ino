#include <SoftwareSerial.h>
#include <SPI.h>
#include <mcp_can.h>

#define BLE_ENABLED true
#define CAN_BUS_ENABLED true

#define DIS_TEXT_line1 0x261
#define DIS_TEXT_line2 0x263

MCP_CAN CAN(10);
SoftwareSerial BTSerial(5, 6); // RX | TX

String currentMessage;

void setup() {
  // Init BLuetooth (BLE) and debugging Serial
  Serial.begin(9600);

  if (BLE_ENABLED) {
    BTSerial.begin(9600);
  }

  // Init CANBUS
  if (CAN_BUS_ENABLED) {
    unsigned int status;

    do {
      status = CAN.begin(CAN_500KBPS);
      delay(100);
    } while (CAN_OK == status);

    Serial.println("* CANBUS init successful!");
  }
}

void loop() {
  if (BLE_ENABLED) {
    handleBLEmessages();
  }

  if (CAN_BUS_ENABLED) {
    readAllCANMessages();
  }
}

void handleBLEmessages() {
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

    delay(1000);
    currentMessage.remove(0, bufSize);
  }


  if(Serial.available()) {
    char message = Serial.read();

    BTSerial.write(message);
    Serial.write(message);
  }
}

void readAllCANMessages() {
  unsigned char length = 0;
  unsigned char buf[8];

  if(CAN_MSGAVAIL == CAN.checkReceive()) {
    CAN.readMsgBuf(&length, buf);

    for(int i = 0; i<length; i++) {
      Serial.write(buf[i]);
      Serial.write("\t");
    }
    Serial.println();
  }
}
