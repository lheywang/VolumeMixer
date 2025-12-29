# Code
This is the core of the VolumeMixer, since all of this stuff handle the interractions between the hardware and the computer.
 
The whole software is splitted in half : 
- The host part
- The device part

## Host
The host part, wrotten in Python ensure control of audio volumes in both Linux and Windows.\*
This part is responsible for the overall management of the tool, the device only respond to commands.

> \*Actually, there is some functionnality variations between both, making the Linux version a bit more complete.

## Device
The device part is written in native C for STM32 targets.
It was wrote using STM32CubeIDE 1.19.

The project is named "board", and contain all the needed code.

The project is based arround a folder named "lib", which contain a lot of code that is platform agnostic.
It could be easily reused accross a lot of different devices !

There's one large file, fsm/fsm.c - .h that are the "heart" of the device, and handle all of the necessary actions.

All of the remaining code is basically setup, drivers for some devices and interrupts routines.




