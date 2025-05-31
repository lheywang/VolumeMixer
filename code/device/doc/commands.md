# Commands

In this file, let's explore how the host computer and the device communicate.
This device only act as a slave, which mean that it will never send data by itself. It will only respond to a previous command.

There is five known commands :

- SHUTD : Shut down the device. Require to press the button to start it.
- RESET : Reset the device to the base state.
- DCONF : Set calibration data of the device.
- CONNC : Connect the host to the device (and thus, block any further connections).

- ASYNC : Sync applications that are known to the device with the apps that are currently in use.
- UICON : Update icons. This commands is generally sent after an ASYNC command that may returned wrong apps.
- SLPOS : Return the position of the sliders, in %.

Internally, theses commands are executed as a state machine, that execute the correct steps in the correct order.

## Protocol

```
START;<CMD_NAME>;<LEN>;<DATA>;<CRC32>;END
```

With :

- CMD_NAME : The command name, on 8 bytes of data. Can be
- LEN : The length of the data received, in bytes (char). Expressed as 3 integer (up to 999)
- DATA : Custom data passed. Always formatted as JSON
- CRC32 : Checksum of the whole message up to now (CRC32 and END excluded).

The command is borned with START and END tokens to clearly identify the
start and end of a said command.

The DATA is generally encoded as JSON data, which will be detailled in the few next paragraphs.

## Data exhange

For each command, this is the field you may find :

(All of the exhanges are seen as the host, so TX = command SENT, RX = command received).

### SHUTD

This command does not send or receive additionnal data.

### RESET

This command does not send or receive additionnal data.

### CONNC

This command is used to start a communication with an host and a device.
The device answer OOK or NOK depending on it's previous state.

### ASYNC

This command receive the list of applications that shall be displayed on the device and used as it.

TX : The host list the device that shall be used.

```javascript
{
    "apps" : [
        {"1"    : "master"},
        {"2"    : "discord.exe"},
        {"3"    : "firefox.exe"},
        {"4"    : "UNUSED"},
        {"5"    : "Valorant.exe"}
    ]
}
```

And the device respond the apps that are already known (and thus, displayed) with a match = true.
Otherwise a match = false will indicate that the app is not know of the device.

```javascript
{
    "match" : "false",
    "apps" : [
        {
            "1"     : "master",
            "match" : "true"
        },
        {
            "2"     : "discord.exe",
            "match" : "true"
        },
        {
            "3"     : "firefox.exe",
            "match" : "known"
        },
        {
            "4"     : "UNUSED",
            "match" : "true"
        },
        {
            "5"     : "Valorant.exe",
            "match" : "false"
        }
    ]
}
```

There's three state : true, known and false. If the app is not actually displayed (known or false), the device will start
to update the icon on the screen. The difference is that for "known", the device already know the icon in it's memory, and will thus
use the icon stored internally. Only the "false" shall trigger an UICON command.

### UICON

This command is used to uptate an icon (generally, this command follow an ASYNC command). It contain the app, the slider and the data of the icon.

```javascript
{
    "icon" : {
        "slider"    : "1",
        "app"       : "world-of-tank.exe",
        "icon"      : "IMGBEGINxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxIMGEND!",
        "store"     : true
    }
}
```

The device respond with the same header, and data as OOK to acknowledge the data, or not (NOK).

### SLPOS

This command is used to fetch the position of the sliders on the device. This is the most common command to be runned. There's no data sent alongside
of the command, but the device respond with the following data structure :

```javascript
{
    "sliders" : [
        {
            "1"     : "082",
            "1M"    : true
        },
                {
            "2"     : "029",
            "2M"    : false
        },
                {
            "3"     : "050",
            "3M"    : true
        },
                {
            "4"     : "100",
            "4M"    : true
        },
                {
            "5"     : "000",
            "5M"    : true
        }
    ]
}
```

For each slider, there's two field : n and nM. n refer to the slider, and it's value to the value of the volume (in %). nM refer to a boolean, which is mute or not.

### DCONF

This command configure the device. It shall not be run continuously by the host (but once at the manufacturing step).

It take some data in input :

```javascript
{
    "cal" : [
        {
            "1OFF"  : "+0.082",
            "1G"    : "1.01"
        },
                {
            "2OFF"  : "+0.029",
            "2G"    : "1.09"
        },
                {
            "3OFF"  : "+0.050",
            "3G"    : "1.00"
        },
                {
            "4OFF"  : "+0.000",
            "4G"    : "1.12"
        },
                {
            "5OFF"  : "-0.012",
            "5G"    : "0.98"
        }
    ],
    "gain" : "1.09",
    "offset": "0.012",
    "device": "TEST01",
}
```

The device respond with an OOK or NOK.
