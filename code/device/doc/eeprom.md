# EEPROM

The EEPROM located on the board is used to store different things :

- Calibration and device info.
- Known icons, to fetch them faster than before.

Since the EEPROM is 64 kbits, it can store up to 60 icons. In practice, that's not the case,
arround 4 to 10 will be stored.

## System informations

System informations are also available, such as :

- Serial Number, to identify which devices are which (mostly in case of multiple systems on the same host).
- Data to know at which offsets are placed images.

## Calibration data

To ensure a smooth and regular experience, calibration data will be stored into the EEPROM, and used
when calculating the volume from the potentiometer. This include :

- An ADC gain error
- An ADC offset
- Per channel calibration :
  - A channel offset
  - A channel gain error

All of this done is to ensure the smallest differences between channels of the devices, of all devices.

## Icons

Each icon is 30 x 30 px, which means 900 bits of data (screens are black and white). Each of them is
stored on four consecutive pages.
