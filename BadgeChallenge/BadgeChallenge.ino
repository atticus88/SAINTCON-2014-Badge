#include <Wire.h>
#include <EEPROM.h>
#include <IRremote.h>
#include <IRremoteInt.h>
#include <math.h>


#define THERMISTORPIN A1 
#define SERIESRESISTOR 10000
float vcc = 4.91;                       // only used for display purposes, if used
float pad = 9850;                       // balance/pad resistor value, set this to
float thermr = 10000;                   // thermistor nominal resistance

/* Initialize the irrecv part of the IRremote  library */
int RECV_PIN = A0;
IRrecv irrecv(RECV_PIN);

IRsend irsend; // send data to other badge
decode_results results; // This will store our IR received codes
uint16_t lastCode = 0; // This keeps track of the last code RX'd
String passcode = "";
uint16_t code1 = 0;
uint16_t code2 = 0;
int count = 0;
// ---- END OF IR RECV -----//

int incomingByte = 0;
boolean show = true;

int lvl1Address = 0;
int lvl2Address = 1;
int lvl3Address = 2;
int lvl4Address = 3;
int lvl5Address = 4;



int lvl1encode = 0x37;
int lvl2encode = 0x42;
int lvl3encode = 0x14;
int lvl4encode = 0x12;
int lvl5encode = 0x55;

// S7ZLKAAGAEJRML
// int lvl1encode = 0x37;
int lvl1message[] = {0x64, 0x0, 0x6D, 0x7B, 0x7C, 0x76, 0x76, 0x70, 0x76, 0x72, 0x7D, 0x65, 0x7A, 0x7B };
 
//CTQSQHFGBPBISE
//int lvl2encode = 0x42;
int lvl2message[] = {0x1, 0x16, 0x13, 0x11, 0x13, 0xA, 0x4, 0x5, 0x0, 0x12, 0x0, 0xB, 0x11, 0x7};

//T2HE8EZS24WQQM
//int lvl3encode = 0x14;
int lvl3message[] = {0x40, 0x26, 0x5C, 0x51, 0x2C, 0x51, 0x4E, 0x47, 0x26, 0x20, 0x43, 0x45, 0x45, 0x59};

//Q2VM1JUDZ9IYVT
//int lvl4encode = 0x12;
int lvl4message[] = {0x43, 0x20, 0x44, 0x5F, 0x23, 0x58, 0x47, 0x56, 0x48, 0x2B, 0x5B, 0x4B, 0x44, 0x46};

//ELJGK68VPX662U
//int lvl5encode = 0x55;
int lvl5message[] = {0x10, 0x19, 0x1F, 0x12, 0x1E, 0x63, 0x6D, 0x3, 0x5, 0xD, 0x63, 0x63, 0x67, 0x0};

int eepromMessage[] = {0x03, 0x13, 0x37};

void setup() {
  Serial.begin(115200);
  Wire.begin();
   analogWrite(6, 255);
   analogWrite(9, 255);
   analogWrite(10, 255);
   analogWrite(11, 255);

  /* FIRST RUN ONLY
  for (int i = 0; i < 6; i++) {
    EEPROM.write(i, 0);
  }
  //*/
}

void clearScreen() {
  Serial.write(27);       // ESC command
  Serial.print("[2J");    // clear screen command
  Serial.write(27);       // ESC
  Serial.print("[H");     // Set Cursor to Home
}

void mainMenu() {
    clearScreen();
    byte lvl1 = EEPROM.read(lvl1Address);
    byte lvl2 = EEPROM.read(lvl2Address);
    byte lvl3 = EEPROM.read(lvl3Address);
    byte lvl4 = EEPROM.read(lvl4Address);
    byte lvl5 = EEPROM.read(lvl5Address);

    Serial.println("+-------------------------------------+"); 
    Serial.println("|    SAINTCON 2014 Badge Challenge    |"); 
    Serial.println("+-------------------------------------+"); 
    Serial.println("|  1) Badge Challenge 100             |");

    if (lvl1 > 0) {
        Serial.println("|  2) Badge Challenge 200             |"); 
    }
    if (lvl2 > 0) {
        Serial.println("|  3) Badge Challenge 300             |"); 
    }
    if (lvl3 > 0) {
        Serial.println("|  4) Badge Challenge 400             |"); 
    }
    if (lvl4 > 0) {
        Serial.println("|  5) Badge Challenge 500             |"); 
    }

    Serial.println("|                                     |");
    Serial.println("|  9) Your Codes                      |");
    Serial.println("|  0) Credits                         |");
    Serial.println("+-------------------------------------+"); 
}

void codes() {
    clearScreen();
    byte lvl1 = EEPROM.read(lvl1Address);
    byte lvl2 = EEPROM.read(lvl2Address);
    byte lvl3 = EEPROM.read(lvl3Address);
    byte lvl4 = EEPROM.read(lvl4Address);
    byte lvl5 = EEPROM.read(lvl5Address);

    Serial.println("+-------------------------------------+"); 
    Serial.println("| SAINTCON 2014 Hack the Badge Codes  |"); 
    Serial.println("+-------------------------------------+"); 
    if (lvl1 > 0) {
        Serial.print(F("HB100: "));
        for(int i = 0; i < 14; i++) {
            Serial.print(char(lvl1message[i] ^ lvl1encode));
        }
        Serial.println("");
    }

    if (lvl2 > 0) {
        Serial.print(F("HB200: "));
        for(int i = 0; i < 14; i++) {
            Serial.print(char(lvl2message[i] ^ lvl2encode));
        }
        Serial.println("");
    }

    if (lvl3 > 0) {
        Serial.print(F("HB300: "));
        for(int i = 0; i < 14; i++) {
            Serial.print(char(lvl3message[i] ^ lvl3encode));
        }
        Serial.println("");
    }

    if (lvl4 > 0) {
        Serial.print("HB400: ");
        for(int i = 0; i < 14; i++) {
            Serial.print(char(lvl4message[i] ^ lvl4encode));
        }
        Serial.println(F(""));
    }

    if (lvl5 > 0) {
        Serial.print(F("HB500: "));
        for(int i = 0; i < 14; i++) {
            Serial.print(char(lvl5message[i] ^ lvl5encode));
        }
        Serial.println(F(""));
    }

    delay(5000);
    mainMenu();
}

boolean lvl1Menu() {
    clearScreen();
    Serial.println("+-------------------------------------+"); 
    Serial.println("|  SAINTCON 2014 Hack the Badge  100  |"); 
    Serial.println("+-------------------------------------+"); 
    Serial.println("|          Checking Badge             |"); 
    Serial.println("+-------------------------------------+"); 
    delay(5000);
    return true;
}


// MENU SECTION
boolean lvl2Menu() {
    clearScreen();
    Serial.println("+-----------------------------------+"); 
    Serial.println("| SAINTCON 2014 Hack the Badge  200 |"); 
    Serial.println("+-----------------------------------+"); 
    Serial.println("|          Receiveing IR.....       |"); 
    Serial.println("+-----------------------------------+");

    irrecv.enableIRIn();
    while (true) {
        if (irrecv.decode(&results)) {
            uint16_t result = (results.value & 0xFFFF);

            if (result != 0XFFFF) {
                count++; 
                if (count > 15) {
                    count = 0;
                }
            }

            if (result == 0xC0FF) {
                return true;
            }

            irrecv.resume(); // Receive the next value
        }
    }
}

boolean lvl3Menu() {
    clearScreen();
    Serial.println("+-----------------------------------+"); 
    Serial.println("| SAINTCON 2014 Hack the Badge  300 |"); 
    Serial.println("+-----------------------------------+"); 
    Serial.println("|              Sending.....         |"); 
    Serial.println("+-----------------------------------+");

    for (int i=0; i < 5; i ++ ) {
        irsend.sendSony(0xDEAD, 16);
        delay(250);
        irsend.sendSony(0xC0DE, 16);
        delay(250);
    }

    // might need another while loop for receive
    delay(500);
    clearScreen();
    Serial.println("+-----------------------------------+"); 
    Serial.println("| SAINTCON 2014 Hack the Badge  300 |"); 
    Serial.println("+-----------------------------------+"); 
    Serial.println("|           Receiveing.....         |"); 
    Serial.println("+-----------------------------------+");

    irrecv.enableIRIn();
    while (true) {
        if (irrecv.decode(&results)) {
            uint16_t result = (results.value & 0xFFFF);

            if (result == 0xBEEF) {
                return true;
            }

            irrecv.resume(); // Receive the next value
        }
    }
}

boolean lvl4Menu() {
    float temp; 
    while (true) {
        clearScreen();
        temp=Thermistor(analogRead(THERMISTORPIN));

        Serial.println("+-----------------------------------+"); 
        Serial.println("| SAINTCON 2014 Hack the Badge  400 |"); 
        Serial.println("+-----------------------------------+"); 
        Serial.print("|              TEMP: ");
        Serial.print(temp, 1);
        Serial.println(" *F        |"); 
        Serial.println("+-----------------------------------+");

        if (temp >= 88.0 && temp <= 100.0) {
            return true;
        }

        delay(250);
    }
}

float Thermistor(int RawADC) {
    long Resistance;  
    float Temp;  // Dual-Purpose variable to save space.
    Resistance=pad*((1024.0 / RawADC) - 1); 
    Temp = log(Resistance); // Saving the Log(resistance) so not to calculate  it 4 times later
    Temp = 1 / (0.001129148 + (0.000234125 * Temp) + (0.0000000876741 * Temp * Temp * Temp));
    Temp = Temp - 273.15;  // Convert Kelvin to Celsius                      
    Temp = (Temp * 9.0)/ 5.0 + 32.0;                  // Convert to Fahrenheit
    return Temp;                                      // Return the Temperature
}

boolean lvl5Menu() {
    clearScreen();
    Serial.println("+-----------------------------------+"); 
    Serial.println("| SAINTCON 2014 Hack the Badge  500 |"); 
    Serial.println("+-----------------------------------+");
    Serial.println("|        Writing EEPROM.....       |"); 
    Serial.println("+-----------------------------------+");

    for(int i = 0; i < 124; i++) {
        eeprom_i2c_write(B01010000, i, eepromMessage[i%3]);
        delay(10);
    }

    clearScreen();

    Serial.println("+-----------------------------------+"); 
    Serial.println("| SAINTCON 2014 Hack the Badge  500 |"); 
    Serial.println("+-----------------------------------+");
    Serial.println("|        Verifying EEPROM.....      |"); 
    Serial.println("+-----------------------------------+");
    
    int total = 0;
    for(int i = 0; i < 3; i++) {
        byte r = eeprom_i2c_read(B01010000, i);
        total += r;
        Serial.print(r, HEX);
    }

    delay(1000);
    if (total == 77 ) {
        return true;
    } else {
        return false;
    }
}

void eeprom_i2c_write(byte address, byte from_addr, byte data) {
    Wire.beginTransmission(address);
    Wire.write(from_addr);
    Wire.write(data);
    Wire.endTransmission();
}

byte eeprom_i2c_read(int address, int from_addr) {
    Wire.beginTransmission(address);
    Wire.write(from_addr);
    Wire.endTransmission();

    Wire.requestFrom(address, 1);
    if(Wire.available()) {
        return Wire.read();
    } else {
        return 0xFF;
    }
}

////Success Messages
void lvl1Success() {
    clearScreen();
    Serial.println(F("+-------------------------------------+")); 
    Serial.println(F("|  SAINTCON 2014 Hack the Badge  100  |")); 
    Serial.println(F("+-------------------------------------+")); 
    Serial.print("|  Code: ");  

    for(int i = 0; i < 14; i++) {
        Serial.print(char(lvl1message[i] ^ lvl1encode));
    }

    Serial.println(F("               |")); 
    Serial.println(F("+-------------------------------------+"));

    EEPROM.write(lvl1Address, random(1,256));
    delay(5000);
    mainMenu();
}

void lvl2Success() {
    clearScreen();
    Serial.println("+-----------------------------------+"); 
    Serial.println("| SAINTCON 2014 Hack the Badge  200 |"); 
    Serial.println("+-----------------------------------+"); 
    Serial.print("|  Code: ");  

    for(int i = 0; i < 14; i++) {
        Serial.print(char(lvl2message[i] ^ lvl2encode));
    }

    Serial.println(F("             |")); 
    Serial.println(F("+-----------------------------------+"));

    EEPROM.write(lvl2Address, random(1,256));
    delay(5000);
    mainMenu();
}

void lvl3Success() {
    clearScreen();
    Serial.println("+-----------------------------------+"); 
    Serial.println("| SAINTCON 2014 Hack the Badge  300 |"); 
    Serial.println("+-----------------------------------+"); 
    Serial.print("|  Code: ");  

    for(int i = 0; i < 14; i++) {
        Serial.print(char(lvl3message[i] ^ lvl3encode));
    }

    Serial.println(F("             |")); 
    Serial.println(F("+-----------------------------------+"));

    EEPROM.write(lvl3Address, random(1,256));
    delay(5000);
    mainMenu();
}

void lvl4Success() {
    clearScreen();
    Serial.println("+-----------------------------------+"); 
    Serial.println("| SAINTCON 2014 Hack the Badge  400 |"); 
    Serial.println("+-----------------------------------+"); 
    Serial.print("|  Code: ");  

    for(int i = 0; i < 14; i++) {
        Serial.print(char(lvl4message[i] ^ lvl4encode));
    }

    Serial.println(F("             |")); 
    Serial.println(F("+-----------------------------------+"));

    EEPROM.write(lvl4Address, random(1,256));
    delay(5000);
    mainMenu();
}

void lvl5Success() {
    clearScreen();
    Serial.println(F("+-----------------------------------+")); 
    Serial.println("| SAINTCON 2014 Hack the Badge  500 |"); 
    Serial.println(F("+-----------------------------------+")); 
    Serial.print("|  Code: ");  

    for(int i = 0; i < 14; i++) {
        Serial.print(char(lvl5message[i] ^ lvl5encode));
    }

    Serial.println(F("             |")); 
    Serial.println(F("+-----------------------------------+"));

    EEPROM.write(lvl5Address, random(1,256));
    delay(5000);
    mainMenu();
}

// MAIN LOOP
void loop() {
    if (show) {
        mainMenu();
        show = false;
    }

    if (Serial.available() > 0) {
        incomingByte = Serial.read();
        byte lvl1 = EEPROM.read(lvl1Address);
        byte lvl2 = EEPROM.read(lvl2Address);
        byte lvl3 = EEPROM.read(lvl3Address);
        byte lvl4 = EEPROM.read(lvl4Address);


        if (incomingByte == 49) { // 1
            boolean result = lvl1Menu();
            if (result) {
                lvl1Success();
            }
        }

        if (incomingByte == 50 && lvl1 > 0) { // 1
            boolean result = lvl2Menu();
            if (result) {
                lvl2Success();
            }
        }

        if (incomingByte == 51 && lvl2 > 0) { // 1
            boolean result = lvl3Menu();
            if (result) {
                lvl3Success();
            }
        }

        if (incomingByte == 52 && lvl3 > 0) { // 1
            boolean result = lvl4Menu();
            if (result) {
                lvl4Success();
            }
        }

        if (incomingByte == 53 && lvl4 > 0) { // 1
            boolean result = lvl5Menu();
            if (result) {
                lvl5Success();
            } else {
                mainMenu(); 
            }
        }

        if (incomingByte == 57) { // 0
            codes();
        }

        if (incomingByte == 48) { // 0
            credits();
        }
    }
}

void credits() {
    clearScreen();
    Serial.println(F("+-------------------------------------+")); 
    Serial.println(F("| SAINTCON 2014 Hack the Badge Team   |")); 
    Serial.println(F("+-------------------------------------+")); 
    Serial.println(F("|  Luke Jenkins                       |"));
    Serial.println(F("|  Klint Holmes                       |"));
    Serial.println(F("|  Matt Lorimer                       |"));
    Serial.println(F("|  Jonathan Karras                    |"));
    Serial.println(F("|                                     |"));
    Serial.println(F("| Thanks to Troy Jessup and the rest  |"));
    Serial.println(F("|   of the SAINTCON 2014 Committee!   |"));
    Serial.println(F("+-------------------------------------+"));
    delay(5000);
    mainMenu();
}

