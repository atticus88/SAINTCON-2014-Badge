## Hackers Challenge: Hack the Badge

###Summary

100: Build Badge  
200: Receive IR  
300: Transmit IR  
400: Use Thermistor  
500: Use i2c EEPROM  

Badge will have a text based interface to give instructions and give back HC challenge board hashes. Each challenge builds on the next, and we will store challenge completed status in the eeprom of the uC.

After successfully completing each challenge, Hacker will be given both the code for the HC game board and instructions on how to get the parts required for their next challenge. 


### Testing

#### Serial Console
```
$ screen /dev/tty.usbmodem1451 115200
```

#### Security
The following command will dump the ROM from the Arduino and use strings on the output to make sure sencitive informaiton cannot be seen in plain text.

```
$ ./avrdude -C avrdude.conf -F -v -pm328p -carduino -P/dev/tty.usbmodem1451 -b115200 -D -Uflash:r:program.bin:r
$ strings program.bin
```
