# Code
This is the core of the VolumeMixer, since all of this stuff handle the interractions between the hardware and the computer.
 
The whole software is splitted in half : 
- The host part
- The device part

## Host
The host part, wrotten in Python ensure control of audio volumes in both Linux and Windows.\*
This part is responsible for the overall management of the tool, the device only respond to commands.

\*Actually, there is some functionnality variations between both, making the Linux version a bit more complete.

## Device
The device part is written in native C for STM32 targets. It's also divided in half :
- the mixerlib project
- the device(\_nucleo) project

### Mixerlib
This C++ library host most of the functions that does not depend on any hardware. This include commands parser,
or any data treatement / computations.

All of this stuff can be tested by running native UnitTests, which run on an x86_64 (or host) platform. They ensure
the code is really doing what we want.
This library can be compiled to any target or system that leave us linking against this library.

Even if the project is configured as C++, the entire functions that are exported to the user for the final application
are written in plain C.

### device(\_nucleo) project
This C project is the base support for the hardware. It only implement the lowest interractions with the hardware, 
which depends on the target. It can't be thus globalized.


