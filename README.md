# 🎛️ VolumeMixer

VolumeMixer is an open-source physical volume controller that allows you to manage specific application and system volumes directly from your desktop. Featuring an OLED screen and a touch/rotary interface, it displays the icon of the application currently being controlled, offering a seamless user experience.

## ✨ Features

- Precise Control: Adjust the volume of each application independently or the global (Master) volume.
- Visual Feedback: OLED screen (SSD1306) displaying the application icon, and volume level.
- Multi-platform: Compatible with Windows and Linux.
- Plug & Play: Communication via USB (CDC Serial interface).
- Custom Hardware: PCB designed in KiCad powered by an STM32F3 microcontroller.
- Extensible: Host software written in Python, easy to modify and extend.

## 📂 Project Structure

The project is organized into three main parts:

.<br>
├── 📂 code<br>
│   ├── 📂 board       # Firmware for the STM32 microcontroller (C/C++)<br>
│   └── 📂 host        # Host software running on the PC (Python)<br>
├── 📂 data            # Graphic assets (Fonts, Bitmap icons)<br>
├── 📂 docs            # Technical documentation (Protocols, Hardware)<br>
└── 📂 pcb             # Electronic design files (KiCad)<br>

## 🛠️ Hardware

The core of the system is based on an STM32 microcontroller. Manufacturing files can be found in the pcb/ folder.

- MCU: STM32F302C8Tx
- Display: I2C OLED Screen
- Inputs: Potentiometers / Touch areas (TSC)
- Connectivity: USB Type-C.

## 🚀 Installation and Usage
### 1. Firmware (Board)

The microcontroller code is developed using STM32CubeIDE 2.0.0 / STM32CubeMX 

1. Open the project located in code/board/ with STM32CubeIDE.
2. Compile the project.
3. Flash the binary onto your STM32 board.

### 2. Host Software (PC)

The host software acts as a bridge between your operating system's audio mixer and the USB device. It uses Python and Poetry for dependency management.

Prerequisites: Python 3.x and Poetry installed.
Bash

Go to the host folder
```sh
cd code/host
```

Install dependencies
```sh
poetry install
```

Run the program
```sh
python -m poetry run .\host\main.py
```

> [!WARNING]
> Note: On Linux, ensure you have permission to access the serial port (usually by adding your user to the dialout group).
### 3. Configuration

You can configure the behavior of the host software via the .toml files located in code/host/host/config/:

 - [win.toml](https://github.com/lheywang/VolumeMixer/blob/main/code/host/host/config/win.toml) for Windows.
 - [linux.toml](https://github.com/lheywang/VolumeMixer/blob/main/code/host/host/config/linux.toml) for Linux.

## 📚 Technical Documentation

For developers who want to contribute or understand the internal workings:

  - [Communication Protocol](https://github.com/lheywang/VolumeMixer/blob/main/docs/commands.md)
  - [Screen & Buffer Management](https://github.com/lheywang/VolumeMixer/blob/main/docs/screen.m)
  - [Finite State Machine (FSM)](https://github.com/lheywang/VolumeMixer/blob/main/docs/statemachine.m)
  - [EEPROM Management](https://github.com/lheywang/VolumeMixer/blob/main/docs/eeprom.m)
