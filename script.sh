ARDUINO_PATH=$1
USB_DEV=$2

$ARDUINO_PATH/Arduino.app/Contents/Resources/Java/hardware/tools/avr/bin/avrdude -C$ARDUINO_PATH/Arduino.app/Contents/Resources/Java/hardware/tools/avr/etc/avrdude.conf -v -v -v -v -patmega328p -cstk500v2 -Pusb -e -Ulock:w:0x3F:m -Uefuse:w:0x05:m -Uhfuse:w:0xde:m -Ulfuse:w:0xff:m 

$ARDUINO_PATH/Arduino.app/Contents/Resources/Java/hardware/tools/avr/bin/avrdude -C$ARDUINO_PATH/Arduino.app/Contents/Resources/Java/hardware/tools/avr/etc/avrdude.conf -v -v -v -v -patmega328p -cstk500v2 -Pusb -Uflash:w:$ARDUINO_PATH/Arduino.app/Contents/Resources/Java/hardware/arduino/bootloaders/optiboot/optiboot_atmega328.hex:i -Ulock:w:0x0F:m 

$ARDUINO_PATH/Arduino.app/Contents/Resources/Java/hardware/tools/avr/bin/avrdude -C$ARDUINO_PATH/Arduino.app/Contents/Resources/Java/hardware/tools/avr/etc/avrdude.conf -v -v -v -v -patmega328p -carduino -P$USB_DEV -b115200 -D -Uflash:w:./hex/BadgeInit.cpp.hex:i 


sleep 5

$ARDUINO_PATH/Arduino.app/Contents/Resources/Java/hardware/tools/avr/bin/avrdude -C$ARDUINO_PATH/Arduino.app/Contents/Resources/Java/hardware/tools/avr/etc/avrdude.conf -v -v -v -v -patmega328p -carduino -P$USB_DEV -b115200 -D -Uflash:w:./hex/BadgeChallenge.cpp.hex:i 

tput bel
tput bel
tput bel
