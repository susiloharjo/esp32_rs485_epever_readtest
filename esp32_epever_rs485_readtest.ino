/*
            
        MAX485         NodeMCU 
            DI              TX
            DE              D2
            RE              D1
            RO              RX
            VCC             3V3 !!!
            GND             GND
            
        EpEver RJ45                        MAX485      NodeMCU
        pin1  +7.5 V       org-wht                       Vin
        pin8  GND          brn                           GND
        pin6  RS-485-A     grn               A
        pin4  RS-485-B     blu               B
   
 
*/



#include <ModbusMaster.h>


#define MAX485_DE       2
#define MAX485_RE       1

#define PANEL_VOLTS     0x00
#define PANEL_AMPS      0x01
#define PANEL_POWER_L   0x02
#define PANEL_POWER_H   0x03
#define BATT_VOLTS      0x04
#define BATT_AMPS       0x05
#define BATT_POWER_L    0x06
#define BATT_POWER_H    0x07

// instantiate ModbusMaster object
ModbusMaster node;

//#include <SoftwareSerial.h>

//SoftwareSerial mySerial(10, 11); // RX, TX

void setup()
{
  Serial.begin(57600);
  while (!Serial) {
    ;
  }
  Serial.println("Read Test!");

  pinMode(MAX485_RE, OUTPUT);
  pinMode(MAX485_DE, OUTPUT);
  // Init in receive mode
  digitalWrite(MAX485_RE, 0);
  digitalWrite(MAX485_DE, 0);

  // Modbus at 115200 baud
  Serial1.begin(115200);

  // EPEver Device ID 1
  node.begin(1, Serial1);

  // Callbacks
  node.preTransmission(preTransmission);
  node.postTransmission(postTransmission);
}

void loop()
{
  uint8_t result;

  // Read 8 registers starting at 0x3100)
  node.clearResponseBuffer();
  result = node.readInputRegisters(0x3100, 8);

  if (result == node.ku8MBSuccess)
  {
    Serial.print("VPanel: ");
    Serial.println(node.getResponseBuffer(PANEL_VOLTS)/100.0f);
    Serial.print("IPanel: ");
    Serial.println(node.getResponseBuffer(PANEL_AMPS)/100.0f);
    Serial.print("PPanel: ");
    Serial.println((node.getResponseBuffer(PANEL_POWER_L) +
                    node.getResponseBuffer(PANEL_POWER_H) << 16)/100.0f);
   
    Serial.print("VBatt: ");
    Serial.println(node.getResponseBuffer(BATT_VOLTS)/100.0f);
    Serial.print("Ibatt: ");
    Serial.println(node.getResponseBuffer(BATT_AMPS)/100.0f);
    Serial.print("PBatt: ");
    Serial.println((node.getResponseBuffer(BATT_POWER_L) +
                    node.getResponseBuffer(BATT_POWER_H) << 16)/100.0f);                   
    Serial.println();
    Serial.println();
  } else {
    Serial.print("Miss read, ret val:");
    Serial.println(result);
  }

  delay(2000);
}

void preTransmission()
{
  digitalWrite(MAX485_RE, 1);
  digitalWrite(MAX485_DE, 1);
}

void postTransmission()
{
  digitalWrite(MAX485_RE, 0);
  digitalWrite(MAX485_DE, 0);
}
