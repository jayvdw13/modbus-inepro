#include <ModbusMaster.h>
#define MAX485_DE     1 
#define MAX485_RE_NEG  22

// instantiate ModbusMaster object
ModbusMaster node;

void preTransmission()
{
  digitalWrite(MAX485_RE_NEG, 1);
  digitalWrite(MAX485_DE, 1);
}

void postTransmission()
{
  digitalWrite(MAX485_RE_NEG, 0);
  digitalWrite(MAX485_DE, 0);
}

void setup()
{
  pinMode(MAX485_RE_NEG, OUTPUT);
  pinMode(MAX485_DE, OUTPUT);
  // Init in receive mode
  digitalWrite(MAX485_RE_NEG, 0);
  digitalWrite(MAX485_DE, 0);

  // Modbus communication runs at 9600 baud
  Serial.begin(9600, SERIAL_8E1, 13, 3);  // standard config = 8N1 (8 bits, none parity, 1 stopbit) if else: 8E1 (8 bits, even parity, 1 stopbit)  Serial3.begin(9600, SERIAL_8E1) 
                        // Serial3 is port 14 & 15 @ mega 2560

  // Modbus slave ID 1
  node.begin(1, Serial);
  // Callbacks allow us to configure the RS485 transceiver correctly
  node.preTransmission(preTransmission);
  node.postTransmission(postTransmission);
}

bool state = true;

void loop()
{
  uint16_t result;
 
/* 
  // Read 2 registers starting at 0x4003)
  // for the INEPRO mod1 energy meter, these registers are data typed as:  Signed
  result = node.readHoldingRegisters(0x4003, 2);
  if (result == node.ku8MBSuccess)
  {
    Serial.print("Modbus ID: ");                   //modbus id
    Serial.println(node.getResponseBuffer(0x00));  // read register 4003
    Serial.print("Baud rate: ");                   // baud rate
    Serial.println(node.getResponseBuffer(0x01));  // read register 4004
   
  }

// Read 1 register starting at 0x400B)
  // for the INEPRO mod1 energy meter, these registers are data typed as:  Signed
 result = node.readHoldingRegisters(0x400B, 1);
  if (result == node.ku8MBSuccess)
  {
    Serial.print("Meter Amps: ");                   // max ampere of this meter
    Serial.println(node.getResponseBuffer(0x00));  // read register 400B
  }
 
*/ 
// Read 2 registers starting at 0x5000)
// for the INEPRO mod1 energy meter, these registers are data typed as:  float ABCD
// register length of 0x5000 is 2 so this is 2 times 16 bit = 32 bit
// so we are receiving 32-bit data in two parts of 16 bit via data[0] and data[1] 
// the data is part of the 32-bit binary32 (IEEE-754) standard
// to get the decimal number, we need to construct the data 
// decimalvalue = data[0] << 16 | data[1]   example: 0x43680000 + 0xD747 = 0x4368D747
/*
 result = node.readHoldingRegisters(0x5002, 2);
  if (result == node.ku8MBSuccess)
  {

Serial.println(node.getResponseBuffer(0x00));
Serial.println(node.getResponseBuffer(0x01));
     
  }
*/
 result = node.readHoldingRegisters(0x5002, 2);
  if (result == node.ku8MBSuccess)
  {

    float x;
unsigned long *p;
p = (unsigned long*)&x;
*p = (unsigned long)node.getResponseBuffer(0x00) <<16 | node.getResponseBuffer(0x01);

Serial.print("Voltage: ");    //voltage of the grid
Serial.println(x, 1); // 1 decimal   (x, 2)= x with 2 decimal
  } 

/*
 result = node.readHoldingRegisters(0x6000, 2);
  if (result == node.ku8MBSuccess)
  {

    float x;
unsigned long *p;

p = (unsigned long*)&x;

*p = (unsigned long)node.getResponseBuffer(0x00) <<16 | node.getResponseBuffer(0x01);

Serial.print("Current: ");      //total usage of kWh
Serial.print(x, 5); 
Serial.println(" kWh");
  }  

  result = node.readHoldingRegisters(0x5012, 2);
  if (result == node.ku8MBSuccess)
  {

    float x;
unsigned long *p;

p = (unsigned long*)&x;

*p = (unsigned long)node.getResponseBuffer(0x00) <<16 | node.getResponseBuffer(0x01);

Serial.print("Active Power: ");     // kW used at this moment
Serial.print(x, 5); 
Serial.println(" kW");
  }   
node.clearResponseBuffer();
*/
  delay(1000);  
}
