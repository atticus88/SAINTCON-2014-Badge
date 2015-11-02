#include <EEPROM.h>

void setup () {
   analogWrite(6, 255);
   analogWrite(9, 255);
  //* FIRST RUN ONLY
  for (int i = 0; i < 6; i++) {
    EEPROM.write(i, 0);
  }
  //*/
  analogWrite(10, 255);
  analogWrite(11, 255);
}

void loop() {
  analogWrite(6, 0);
   analogWrite(9, 0);
   analogWrite(10, 0);
   analogWrite(11, 0);
   delay(1000);
    analogWrite(6, 255);
   analogWrite(9, 255);
   analogWrite(10, 255);
   analogWrite(11, 255);
   delay(1000);
}
