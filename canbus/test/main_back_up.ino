#include <Arduino.h>

#include <SPI.h>
#include <mcp2515.h>

MCP2515 mcp2515(53);
// #define SLAVE 1
#define MASTER 1

#ifdef MASTER
struct can_frame canMsg1;
struct can_frame canMsg2;

void canbus_init()
{
  mcp2515.reset(); //reset can bus
  mcp2515.setBitrate(CAN_125KBPS);//setBitrate
  mcp2515.setNormalMode();//set mode
  Serial.println("Example: Write to CAN");
}
int send_canbus()
{
  canMsg1.can_id = 0x0F6;        //start
  canMsg1.can_dlc = 2;           //num of data
  canMsg1.data[0] = 0x8E;        //data1
  canMsg1.data[1] = 0x87;        //data2
                                 // canMsg1.data[2] = 0x32;
                                 // canMsg1.data[3] = 0xFA;
                                 // canMsg1.data[4] = 0x26;
                                 // canMsg1.data[5] = 0x8E;
                                 // canMsg1.data[6] = 0xBE;
                                 // canMsg1.data[7] = 0x86;
  mcp2515.sendMessage(&canMsg1); //submit dtaa
  return true;
}

void setup()
{
  // canMsg1.can_id = 0x0F6;
  // canMsg1.can_dlc = 2;
  // canMsg1.data[0] = 0x8E;
  // canMsg1.data[1] = 0x87;
  // canMsg1.data[2] = 0x32;
  // canMsg1.data[3] = 0xFA;
  // canMsg1.data[4] = 0x26;
  // canMsg1.data[5] = 0x8E;
  // canMsg1.data[6] = 0xBE;
  // canMsg1.data[7] = 0x86;

  while (!Serial)
    ;
  Serial.begin(115200);

  // mcp2515.reset(); //reset can bus
  // mcp2515.setBitrate(CAN_125KBPS);//setBitrate
  // mcp2515.setNormalMode();//set mode

  // Serial.println("Example: Write to CAN");
}

void loop()
{
  // mcp2515.sendMessage(&canMsg1);
  // mcp2515.sendMessage(&canMsg2);
  delay(1000);
  Serial.println("Messages sent");

  delay(100);
}

#else

struct can_frame canMsg;

void setup()
{
  Serial.begin(115200);

  mcp2515.reset();
  mcp2515.setBitrate(CAN_125KBPS);
  mcp2515.setNormalMode();

  Serial.println("------- CAN Read ----------");
  Serial.println("ID  DLC   DATA");
}

void loop()
{
  if (mcp2515.readMessage(&canMsg) == MCP2515::ERROR_OK)
  {
    Serial.print(canMsg.can_id, HEX); // print ID
    Serial.print(" ");
    Serial.print(canMsg.can_dlc, HEX); // print DLC
    Serial.print(" ");
    int x = canMsg.data[0];
    int y = canMsg.data[1];

    // for (int i = 0; i<canMsg.can_dlc; i++)  {  // print the data
    //   Serial.print(canMsg.data[i],HEX);
    //   Serial.print(" ");
    // }
    Serial.print("x = ");
    Serial.println(x);
    Serial.print("y = ");
    Serial.println(y);
    // Serial.printf("x = %d , y= %d \n",x,y);
    Serial.println();
  }
}
#endif
