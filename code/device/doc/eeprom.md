# EEPROM

The EEPROM located on the board is used to store different things :

- Calibration and device info.
- Known icons, to fetch them faster than before.

Since the EEPROM is 64 kbits, it can store up to 60 icons. In practice, that's not the case,
arround 4 to 10 will be stored.

## System informations

System informations are also available, such as :

- Serial Number\*, to identify which devices are which (mostly in case of multiple systems on the same host).
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

## Data emplacement

On the eeprom, the data is stored in the following pages :

### Calibration data :

This data is stored on page 0, with the following placement :

| Byte  | Variable | Description                            |
| ----- | -------- | -------------------------------------- |
| 0-1   | MARKER   | Constant to 0xbeef                     |
| 2-9   | ID       | Device SN                              |
| 10-11 | ADC_OFF  | ADC offset (in raw code, 0-4096)       |
| 12-13 | CH1_OFF  | ADC offset for channel 1 (raw code)    |
| 14-15 | CH2_OFF  | ADC offset for channel 2 (raw code)    |
| 16-17 | CH3_OFF  | ADC offset for channel 3 (raw code)    |
| 18-19 | CH4_OFF  | ADC offset for channel 4 (raw code)    |
| 20-21 | CH5_OFF  | ADC offset for channel 5 (raw code)    |
| 22    | ADC_GAIN | ADC Gain error (in signed Q1.6 format) |
| 23    | CH1_G    | ADC Gain error for channel 1 (S Q1.6)  |
| 24    | CH2_G    | ADC Gain error for channel 2 (S Q1.6)  |
| 25    | CH3_G    | ADC Gain error for channel 3 (S Q1.6)  |
| 26    | CH4_G    | ADC Gain error for channel 4 (S Q1.6)  |
| 27    | CH5_G    | ADC Gain error for channel 5 (S Q1.6)  |
| 28-31 | CHKSM    | CRC32 of the config                    |

### Address reference

This data refer to the EEPROM addresses used to store each image. Each line refer to an HASH (HASH of the APP Name) and an ADDRESS on 12 bits.
This is stored on 2 pages (2 and 3).

| Byte  | Variable | Description                    |
| ----- | -------- | ------------------------------ |
| 0-3   | IMG1     | HASH:ADDR for image 1          |
| 4-7   | IMG2     | HASH:ADDR for image 2          |
| 8-11  | IMG3     | HASH:ADDR for image 3          |
| 12-15 | IMG4     | HASH:ADDR for image 4          |
| 16-19 | IMG5     | HASH:ADDR for image 5          |
| 20-23 | IMG6     | HASH:ADDR for image 6          |
| 24-27 | IMG7     | HASH:ADDR for image 7          |
| 28-31 | IMG8     | HASH:ADDR for image 8          |
| 32-35 | IMG9     | HASH:ADDR for image 9          |
| 36-39 | IMG10    | HASH:ADDR for image 10         |
| 40-43 | IMG11    | HASH:ADDR for image 11         |
| 44-47 | IMG12    | HASH:ADDR for image 12         |
| 48-51 | IMG13    | HASH:ADDR for image 13         |
| 52-55 | IMG14    | HASH:ADDR for image 14         |
| 56-59 | IMG15    | HASH:ADDR for image 15         |
| 60-63 | CHKSM    | CRC32 for the addresses config |

### Images

Then, we found images, splitted on four pages each.
Images are written randomly on the EEPROM address space to get a near constant wear levelling of the cells. Thus, addresses are always multiple
of 128 (size of an image).
First image start on byte 64, then 192, then 320...

There is about 63 possibles icons on the EEPROM, but only 15 are used, leaving 75% of the chip for wear levelling usage.

---

\* The serial number is expressed as the following form, on 8 bytes : YYMDHXDY
With :

- YY the year (20 and 24)
- M the month (0-12)
- D the day (0-31)
- H is constant to 'H'
- X is the hardware revision (0-...)
- D is constant to 'D'
- Y the device on that day (0-...)
