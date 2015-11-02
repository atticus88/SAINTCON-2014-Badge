#include <IRremote.h>

int RECV_PIN = A0;
int count = 0;
IRrecv irrecv(RECV_PIN);
IRsend irsend;
String passcode = "";
decode_results results;
boolean locked = false;
unsigned long lastSend = 0;

void setup() {
    Serial.begin(115200);
    irrecv.enableIRIn();
    Serial.println("Booted");
    analogWrite(6, 255);
    analogWrite(9, 255);
    analogWrite(10, 255);
    analogWrite(11, 255);
}

void loop() {
    // HB 300
    if (irrecv.decode(&results)) {
        uint16_t result = (results.value & 0xFFFF);

        if (result != 0xFFFF) {
            count++; 
            if (count > 5) {
                passcode = "";
                count = 0;
            }
        }

        //Serial.println(result, HEX);
        if (result == 0xDEAD) {
            passcode = String(result, HEX);
            count = 0;
        }

        if (result == 0xC0DE) {
            passcode += String(result, HEX);
            count = 0;
        }

        //Serial.println(passcode);
        if (passcode == "deadc0de" && result != 0xFFFF) {
            passcode = ""; 
            delay(2000);

            for (int i =0; i < 10; i++ ) {
                irsend.sendSony(0xBEEF, 16);
                delay(100);
            }

            irrecv.enableIRIn();
        }

        irrecv.resume(); // Receive the next value
    } 
    
    // HB 200
    if (millis() - 10000 > lastSend) {
        lastSend = millis();
        irsend.sendSony(0xC0FF, 16);
        delay(100);
        irrecv.enableIRIn();
    }
}

