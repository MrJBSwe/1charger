///////////////////////////////////////////////////
//
// 1Charger ver 0.95 by JB
// http://hackaday.io/project/1936-1Charger
//
///////////////////////////////////////////////////

This project is heavily inspired by
http://flashlightwiki.com/AVR_Drivers


Build:

cd C:\1Charger\src

make clean
make


Flash:
cd C:\1Charger\flash
avrdude -p t861 -c usbasp -u -Uflash:w:1CHG.hex:a -Ueeprom:w:1CHG.eep:a

pre bulit with CALIBRATION
avrdude -p t861 -c usbasp -u -Uflash:w:1CHG-CALIB.hex:a -Ueeprom:w:1CHG.eep:a
