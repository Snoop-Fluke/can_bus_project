#include <Arduino.h>

#include <SPI.h>
#include <mcp2515.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>


int tempPin = 0;
int Buzzer = 8;
float temp;

MCP2515 mcp2515(53);
#define SLAVE 1
// #define MASTER 1

#define TEMP_ 0xF4
#define GAS_0 0xF1
#define GAS_1 0xF2
#define GAS_2 0xF3

#ifdef MASTER
LiquidCrystal_I2C lcd(0x3F, 16, 2);
struct can_frame canMsg1;
void canbus_init()
{
  mcp2515.reset();                 //reset can bus
  mcp2515.setBitrate(CAN_125KBPS); //setBitrate
  mcp2515.setNormalMode();         //set mode
  Serial.println("Example: Write to CAN");
}
int send_canbus(can_frame *const me, float tmp, long id)
{
  static float f1, f2;
  static int i1, i2;
  static char xx[10];

  f1 = floor(tmp);
  f2 = tmp - f1;
  i1 = (int)f1;
  i2 = (int)100 * f2;
  // static int tmp_ = tmp * 100;
  // static int g0_ = g0 * 100;
  // static int g1_ = g1 * 100;
  // static int g2_ = g2 * 100;
  me->can_id = id;  //start
  me->can_dlc = 2;  //num of data
  me->data[0] = i1; //data1
  me->data[1] = i2; //data2
  // me->data[1] = g1_;             //data3
  // me->data[1] = g2_;             //data4
  mcp2515.sendMessage(&canMsg1); //submit dtaa
  Serial.println("Can_bus_send");
  return true;
}

// char c_o_n_v_e_r_t__f_l_o_a_t_c_h_a_r(float valor)
// {
//   float f1, f2;
//   int i1, i2;
//   char xx[10];

//   f1 = floor(valor);
//   f2 = valor - f1;
//   i1 = (int)f1;
//   i2 = (int)100 * f2;
//   sprintf(xx, "%d.%d", i1, i2);
//   printf("%s", xx);
//   return (xx);
// }

void setup()
{

  Serial.begin(115200);
  canbus_init();
  Serial.println("set_conbus_ok");
  pinMode(8, OUTPUT);
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Wecome Poject");
  lcd.setCursor(0, 1);
  lcd.print("By KEI & BENZ");
  delay(3000);
  lcd.clear();
}

void loop()
{
  temp = analogRead(A0);
  temp = temp * 0.48828125;
  float vol_0;
  float vol_1;
  float vol_2;
  int sensorValue_0 = analogRead(A1);
  int sensorValue_1 = analogRead(A3);
  int sensorValue_2 = analogRead(A4);
  vol_0 = (float)(sensorValue_0 / 1024 * 5.0) * 20;
  vol_1 = (float)(sensorValue_1 / 1024 * 5.0) * 20;
  vol_2 = (float)(sensorValue_2 / 1024 * 5.0) * 20;

  lcd.setCursor(0, 1);
  lcd.print("T=");
  lcd.print(temp);
  Serial.print("TEMPRATURE = ");
  Serial.print(temp);
  Serial.println();
  send_canbus(&canMsg1, temp, TEMP_); //send can bus

  lcd.setCursor(0, 0);
  lcd.print("Gas_0=");
  lcd.print(vol_0);
  lcd.print("%");
  lcd.print("      ");
  Serial.print("Gas_0 = ");
  Serial.print(vol_0);
  Serial.println("%");
  send_canbus(&canMsg1, vol_0, GAS_0); //send can bus
  delay(1500);

  lcd.setCursor(0, 0);
  lcd.print("Gas_1=");
  lcd.print(vol_1);
  lcd.print("%");
  lcd.print("      ");
  Serial.print("Gas_1 = ");
  Serial.print(vol_1);
  Serial.println("%");
   send_canbus(&canMsg1, vol_1, GAS_1); //send can bus
  delay(1500);
  
  lcd.setCursor(0, 0);
  lcd.print("Gas_2=");
  lcd.print(vol_2);
  lcd.print("%");
  lcd.print("      ");
  Serial.print("Gas_2 = ");
  Serial.print(vol_2);
  Serial.println("%");
  send_canbus(&canMsg1, vol_2, GAS_2); //send can bus
  delay(1500);

  if (vol_0 > 20 && vol_1 > 20 && vol_2 > 20)

  {
    digitalWrite(Buzzer, LOW);
  }
  else
  {
    digitalWrite(Buzzer, HIGH);
  }

  if (temp > 50)
  {
    digitalWrite(Buzzer, LOW);
  }
  else
  {
    digitalWrite(Buzzer, HIGH);
  }
}

#else

LiquidCrystal_I2C lcd(0x27, 16, 2);
struct can_frame canMsg;

static inline char *get_data(int x, int z) //ภาครับข้อมูล
{

  char *str = (char *)malloc(5 * sizeof(char *));
  sprintf(str, "%d.%d", x, z);
  return (str);
  free(str);
}

void setup()
{
  Serial.begin(115200);
  mcp2515.reset();
  mcp2515.setBitrate(CAN_125KBPS);
  mcp2515.setNormalMode();
  Serial.println("------- CAN Read ----------");
  Serial.println("ID  DLC   DATA");

  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Wecome Poject");
  lcd.setCursor(0, 1);
  lcd.print("By KEI & BENZ");
  delay(3000);
  lcd.clear();
  Serial.println("Wecome Poject");
}

void loop()
{
  if (mcp2515.readMessage(&canMsg) == MCP2515::ERROR_OK)
  {
    Serial.print(canMsg.can_id, HEX); // print ID
    Serial.print(" ");
    Serial.print(canMsg.can_dlc, HEX); // print DLC
    Serial.print(" ");
    int i1 = canMsg.data[0];
    int i2 = canMsg.data[1];
    switch (canMsg.can_id)
    {
      char *data;
    case TEMP_:

      Serial.print("temp__ = ");
      data = get_data(i1, i2);
      Serial.println(data);

      lcd.setCursor(0, 1);
      lcd.print("T=");
      lcd.print(data);

      free(data);
      break;

    case GAS_0:

      Serial.print("gas0__ = ");
      data = get_data(i1, i2);
      Serial.println(data);

      lcd.setCursor(0, 0);
      lcd.print("Gas_0=");
      lcd.print(data);
      lcd.print("%");
      lcd.print("      ");

      free(data);
      break;
    case GAS_1:

      Serial.print("gas1__ = ");
      data = get_data(i1, i2);
      Serial.println(data);

      lcd.setCursor(0, 0);
      lcd.print("Gas_1=");
      lcd.print(data);
      lcd.print("%");
      lcd.print("      ");

      free(data);
      break;

    case GAS_2:

      Serial.print("gas2__ = ");
      data = get_data(i1, i2);
      Serial.println(data);

      lcd.setCursor(0, 0);
      lcd.print("Gas_2=");
      lcd.print(data);
      lcd.print("%");
      lcd.print("      ");

      free(data);
      break;
    default:
      Serial.println("NO_ID");
      break;
    }
    Serial.println();
  }
}
#endif
