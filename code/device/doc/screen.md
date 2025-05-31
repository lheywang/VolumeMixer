# Screen

The screen onboard is used to prompt to the user some cool infos, such as the icon of the app currently, the volume or a MUTE flag.

## Layout

With dimensions such as 128 x 32 px, the screen isn't that big.

The layout is the following :

| Pixels     | Function       | Details                                                       |
| ---------- | -------------- | ------------------------------------------------------------- |
| 0-6 px     | BLANK          |                                                               |
| 7-23 px    | Volume in %    | Two chars, 16 x 16 px each                                    |
| 24-35 px   | BLANK          |                                                               |
| 36-58 px   | Speaker logo   | Show the actual volume (graphically).                         |
| 59- 70 px  | BLANK          |                                                               |
| 71-79 px   | MUTE indicator | Four char, 8 x 8 px each (MUTE only). Blank if not mute.      |
| 80-91 px   | BLANK          |                                                               |
| 92-121 px  | App Icon       | 30 x 30 px icon (image treatement is done on the host device) |
| 122-128 px | BLANK          |                                                               |

[Font source](https://lexus2k.github.io/ssd1306/ssd1306__fonts_8c_source.html)
