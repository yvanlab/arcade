#include <Arduino.h>
#include <Wire.h>
#include "Adafruit_MCP23017.h"

// Basic pin reading and pullup test for the MCP23017 I/O expander
// public domain!

// Connect pin #12 of the expander to Analog 5 (i2c clock)
// Connect pin #13 of the expander to Analog 4 (i2c data)
// Connect pins #15, 16 and 17 of the expander to ground (address selection)
// Connect pin #9 of the expander to 5V (power)
// Connect pin #10 of the expander to ground (common ground)
// Connect pin #18 through a ~10kohm resistor to 5V (reset pin, active low)

// Output #0 is on pin 21 so connect an LED or whatever from that to ground

Adafruit_MCP23017 mcp;
  
void setup() {  
  Serial.begin(115200);
  delay(500);
  Serial.println("start debuging");
  mcp.begin();      // use default address 0
  for (int i=0;i<16;i++) {
    mcp.pinMode(i, OUTPUT);
  }
}


// flip the pin #0 up and down

void loop() {
  delay(1000);
  mcp.digitalWrite(0, HIGH);
  delay (250);
  mcp.digitalWrite(1, HIGH);
  Serial.println("ON");

  delay(1000);
    mcp.digitalWrite(0, LOW);
  delay (250);
  mcp.digitalWrite(1, LOW);

    Serial.println("OFF");
}