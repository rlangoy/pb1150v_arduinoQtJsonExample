#include "ArduinoJson.h"

const int potVccPin =  27;             // the VCC input pin of the Potmeter
const int pot1OutputVoltagePin = 26;     // the output voltage pin from Potmeter

void setup() {
  // Initialize the LED as an output
  pinMode(LED_BUILTIN, OUTPUT);
  
  // initialize Potmeter VCC pin as an HIGH output 
  pinMode(potVccPin, OUTPUT);
  digitalWrite(potVccPin, HIGH);

  pinMode(pot1OutputVoltagePin,INPUT);
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

int _pot1OutputVoltagePin=-1;

void analogInpuPot1tChecker(){
  // Read analog value
  int currentPot1OutputAnalogValue=analogRead(pot1OutputVoltagePin);

  //Chek if analog value has changed by more than delta (due to noise)
  int delta=24;
  //if( _pot1OutputVoltagePin != currentPot1OutputAnalogValue)
  if (( _pot1OutputVoltagePin + delta < currentPot1OutputAnalogValue)  ||  (_pot1OutputVoltagePin - delta > currentPot1OutputAnalogValue)   ) 
  {   //Update old value
      _pot1OutputVoltagePin=currentPot1OutputAnalogValue;      
      //Send new JSON message
     Serial.print(F("{\"Pot1Value\":")); //1}"));//'{ "SwitchBootSel": 1}'     
     Serial.print(currentPot1OutputAnalogValue);
     Serial.println(F("}"));
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
  
  analogInpuPot1tChecker();  // check if pot1 has changed it's value
  serialInputChecker();      // check for serial port input

}
