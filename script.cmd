set USB_DEV=COM8

"%programfiles(x86)%\Arduino\hardware\tools\avr\bin\avrdude" -C"%programfiles(x86)%\Arduino\hardware\tools\avr\etc\avrdude.conf" -v -patmega328p -cstk500v2 -Pusb -e -Ulock:w:0x3F:m -Uefuse:w:0x05:m -Uhfuse:w:0xde:m -Ulfuse:w:0xff:m 
timeout 1
"%programfiles(x86)%\Arduino\hardware\tools\avr\bin\avrdude" -C"%programfiles(x86)%\Arduino\hardware\tools\avr\etc\avrdude.conf" -v -patmega328p -cstk500v2 -Pusb -Uflash:w:"%programfiles(x86)%\Arduino\hardware\arduino\bootloaders\optiboot\optiboot_atmega328.hex":i -Ulock:w:0x0F:m 
timeout 1
"%programfiles(x86)%\Arduino\hardware\tools\avr\bin\avrdude" -C"%programfiles(x86)%\Arduino\hardware\tools\avr\etc\avrdude.conf"  -v -patmega328p -carduino -P%USB_DEV% -b115200 -D -Uflash:w:BadgeInit.cpp.hex:i 


timeout 5

"%programfiles(x86)%\Arduino\hardware\tools\avr\bin\avrdude" -C"%programfiles(x86)%\Arduino\hardware\tools\avr\etc\avrdude.conf"  -v -patmega328p -carduino -P%USB_DEV% -b115200 -D -Uflash:w:BadgeChallenge.cpp.hex:i 

