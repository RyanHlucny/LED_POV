# Getting started with Raspberry Pi Pico W using Arduino-Pico core and Platform.io

## Ryan Hlucny

A major part of the software development for this project was done in VS code using the Platformio extension. Setting up a computer to allow you to develop code for a Raspberry Pi Pico W using the Arduino-core from Earle Philhower isn't trivial and I've had issues setting it up with Windows 10 and Windows 11. The purpose of this document is to streamline the process for anyone else wanting to set up their development environment in the same way as I did for this project.

### Arduino-Pico

Arduino-Pico is a port of the RP2040 microcontroller (the microcontroller of the Pico and Pico W) to the Arduino environment. This allows for easy software development for the Pico in Arduino code, which makes available to you many of the pre-existing libraries and hardware that are traditionally used with Arduino. This is relatively easy to set up and the installation steps can be followed [here](https://arduino-pico.readthedocs.io/en/latest/install.html). I haven't run into any snags getting Arduino-Pico set up on my Pico boards and uploading sketches to the board through the (new) Arduino IDE (I haven't tried the old one).

However, developing real software in the Arduino IDE leaves quite a bit to be desired, so I opted for an embedded development environment extension to VS Code called Platformio.

### PlatformIO

[PlatformIO](https://platformio.org/) is an embedded software development ecosystem, which is installed super easily as a VS Code extension (one-click installation!). PlatformIO boasts support for over 40 platforms, 20+ frameworks, over 1500 boards, and 13,000+ libraries. I first used PlatformIO as an upgraded alternative to the Arduino IDE (as I was already familiar with VS Code), and I grew to liking it for its ability to develop a lot more professional, industry standard software. It also makes expanding to other embedded platforms much easier. When starting this project I wondered if the Raspberry Pi Pico W can be programmed as an Arduino using the Arduino-Pico core, then could it also be supported by PlatformIO?

The answer is, **YES!**

This, however, is where the difficulties begin with setting up the development environment, since the Arduino-Pico core integration into PlatformIO is still a work-in-progress. I found it to be in a satisfactory state for this project. For instructions on PlatformIO integration follow [this](https://arduino-pico.readthedocs.io/en/latest/platformio.html) link. Make sure you follow all steps in that tutorial.

*NOTE: I was unable to enable long paths on my Windows 10 operating system, but PlatformIO integration has worked just fine.
Windows 11 worked just fine for me.*

Your `platformio.ini` file should look something like the following for a Raspberry Pi Pico W:

```ini
[env:rpipicow]
platform = https://github.com/maxgerhardt/platform-raspberrypi.git
board = rpipicow    
framework = arduino
board_build.core = earlephilhower

```

(*Note* that this file will be different for a Raspberry Pi Pico and a Raspberry Pi Pico W)

### Windows Drivers

*__Warning:__ I had to perform the below steps for my both my Windows 11 and Windows 10 PC.*

Additionally, if you have troubles with uploading code to your Pico W, you may need to install the correct drivers for the Pico W device. You can find the solution to this problem [here](https://github.com/earlephilhower/arduino-pico/discussions/1591).

Install Zadig and you should be able to install the correct drivers found at the link above relatively easily. Then you should be able to upload code to your Pico W using Arduino-Pico core with VS Code and the PlatformIO development environment.
