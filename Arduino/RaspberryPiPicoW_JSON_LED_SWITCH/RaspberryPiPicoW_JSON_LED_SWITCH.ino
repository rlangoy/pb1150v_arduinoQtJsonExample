#include "ArduinoJson.h"

void setup() {
  // Initialize the LED as an output
  pinMode(LED_BUILTIN, OUTPUT);
}

// Allocate the JSON document
JsonDocument doc;

void serialInputChecker(){
  if(Serial.available())
  {
      String input=Serial.readString();
      // Serial.println(input); //Debug
      deserializeJson(doc, input);

      if(doc.containsKey("userLedOn")) 
      { long userLedStatus= doc["userLedOn"];
        // Serial.println(userLedStatus); // Debug
        if(userLedStatus==1)
          digitalWrite(LED_BUILTIN, HIGH);
        if(userLedStatus==0)
          digitalWrite(LED_BUILTIN, LOW);
           
      }   
  }
}


void loop() {
  // put your main code here, to run repeatedly:
  if (BOOTSEL) {
      Serial.println(F("{\"SwitchBootSel\": 1}"));//'{ "SwitchBootSel": 1}'
      while (BOOTSEL) {
          delay(.01);
          serialInputChecker();  //check for serial port input
      }
      Serial.println(F("{\"SwitchBootSel\": 0}"));//'{ "SwitchBootSel": 0}'
  }
  
  serialInputChecker(); //check for serial port input

}
