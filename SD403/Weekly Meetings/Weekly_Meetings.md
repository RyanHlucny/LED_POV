# Weekly Meetings - LED_POV

## SD_403_Sp24_06 LED Spinner

This document shows our weekly meetings and progress throughout the semester.

### Overview

- [Feb 9, 2024](#feb-9-2024)
- [Feb 15, 2024](#feb-15-2024)
- [Feb 22, 2024](#feb-22-2024)
- [Feb 29, 2024](#feb-29-2024)
- [Mar 14, 2024](#mar-14-2024)
- [Mar 21, 2024](#mar-21-2024)
- [Mar 28, 2024](#mar-28-2024)
- [Apr 5, 2024](#apr-5-2024)
- [Apr 11, 2024](#apr-11-2024)
- [Apr 18, 2024](#apr-18-2024)

### Feb 9, 2024

![feb-9-2024-meeting](/SD403/Media/Images/02_09_2024_Meeting.jpg)

### Feb 15, 2024

![feb-15-2024-meeting-1](/SD403/Media/Images/02_15_2024_Meeting_1.jpg)
![feb-15-2024-meeting-2](/SD403/Media/Images/02_15_2024_Meeting_2.jpg)

### Feb 22, 2024

![feb-22-2024-meeting](/SD403/Media/Images/02_22_2024_Meeting.jpg)

https://github.com/RyanHlucny/LED_POV/assets/31110301/b2b64ef9-6dbf-48fa-892e-a6407bdd5961

MATLAB pixel transformation of a cartesian style pixel grid to polar style. This is necessary since the pixels will be spinning on the rotating platform. I will use the MATLAB program to generate a look-up-table (LUT) which will reduce the computational load for the Pico, and will only have to look-up pixel colors instead of performing the image transform itself. MATLAB is useful!

### Feb 29, 2024

No Pictures from our meeting this week.

### Mar 14, 2024

https://github.com/RyanHlucny/LED_POV/assets/31110301/247b1501-0b91-44f0-bc66-efd0bf0dff2f

### Mar 21, 2024

![mar-21-2024-meeting](/SD403/Media/Images/03_21_2024_Meeting.jpg)

![power](/SD403/Media/Images/PowerElectronics.jpg)
Assembled Power electronics for the first time.

![firstleds](/SD403/Media/Images/FirstLeds.jpg)
First time getting Dotstar LEDs to run from our Raspberry Pi Pico W using arduino-core and Platform.io.

![fullStrip](/SD403/Media/Images/FullStrip.jpg)
Got the full strip to light solid while experimenting with SPI clock frequency. With this long of a strip (144 LEDs) we found the colors would start to fizzle out by the end of the strip if running too high of a frequency. Very stable at ~15 MHz, which is roughly half of the maximum frequency for the Dotstar LED chip.

![aluminumHub](/SD403/Media/Images/AluminumHub.jpg)
CAD preview for the aluminum center hub.

![jankyPower](/SD403/Media/Images/JankyPower.jpg)
Janky power setup for testing power supplies.

### Mar 28, 2024 Spring Break

![parts](/SD403/Media/Images/PARTS.jpg)
PARTS!

![moreParts](/SD403/Media/Images/MoreParts.jpg)
Assembling the blades.

![partsAgain](/SD403/Media/Images/PartsAgain.jpg)
Attaching the DC motor to the frame with the 3D printed pulley.

![puttingPartsTogether](/SD403/Media/Images/PuttingPartsTogether.jpg)
Front view.

![frameWelding](/SD403/Media/Images/FrameWelding.jpg)
Welding the frame.

![frameWelding2](/SD403/Media/Images/FrameWelding2.jpg)
Cleaning up the welds.

![hubBroach](/SD403/Media/Images/HubBroach.jpg)
Setting up to broach the keyway into the aluminum hub.

![broachOp1](/SD403/Media/Images/BroachOp1.jpg)
Broaching...

![broachOp2](/SD403/Media/Images/BroachOp2.jpg)

![broachOp3](/SD403/Media/Images/BroachOp3.jpg)

![broachOp4](/SD403/Media/Images/BroachOp4.jpg)

![broachOp5](/SD403/Media/Images/BroachOp5.jpg)
Finished product!

![tigWelding](/SD403/Media/Images/JankyTigWelding.jpg)
Janky tig welding setup for the blades. I added a second clamp on the side closer to the camera and the blades still buckled under all the heat from tig. Tig welding is hard!

![welder](/SD403/Media/Images/TigWelder.jpg)
Miller Welder used.

![millingChannel](/SD403/Media/Images/MillingTheChannel1.jpg)
Milling the channel.

https://github.com/RyanHlucny/LED_POV/assets/31110301/78458494-9d27-485f-92d1-7b921a663872

Milling operation video. This is done on a Bridgeport manual upright end mill.

![fitmentTest](/SD403/Media/Images/FitmentTest.jpg)
Fitting the blade in the channel

![dualBladeFitmentTest](/SD403/Media/Images/DualBladeFitment.jpg)
Blades fit nice!

![bearings](/SD403/Media/Images/BearingsNice.jpg)
Took us a while but realized the original bearing we were going to use was a self-aligning bearing. In other words, it would not hold up to any torques placed on the shaft. It's not meant to be used as a solo bearing solution - it requires another bearing to keep the shaft true. So we found these 1 inch ID steel ball bearings.

![bearingBlock](/SD403/Media/Images/BearingBlockCAD.jpg)
So off to the races we went designing a new bearing block to hold the bearings. This design has a groove snapring to lock the bearings in.

![bearingBlockNice](/SD403/Media/Images/BearingBlockNice.jpg)
Printed with Matte Black Overture PLA (on a prusa i3 MK3S+) for prototype.

![topCut](/SD403/Media/Images/TopCutsection.jpg)
Here's a cut section of the "head" of the project, including the new bearing block.

![electronicsCore](/SD403/Media/Images/ElectronicsCore.jpg)
At the same time, we were designing the electronics core which would go on the other end of the shaft. The orange piece here is a mount for the IMU. The "cages" are for holding 21700 battery cells.

![electronicsCorePrint](/SD403/Media/Images/ElectronicsCorePrint.jpg)
3D Printed.

### The Next Day

![bearingBlockAssembled](/SD403/Media/Images/BearingBlockAssembled.jpg)
Bearing block assembled with snapring to hold it togehter.

![frameWithBearingBlock](/SD403/Media/Images/FrameWIthBearingBlock.jpg)
Frame with bearing block mounted.

![shaftWithKey](/SD403/Media/Images/ShaftWithKey.jpg)
Shaft with Key and bore hole for wires to pass through to the LED blades.

![fullASM](/SD403/Media/Images/fullASM.jpg)
Full assembly (minus electronics).

![cusectionTop](/SD403/Media/Images/CutsectionTopASM.jpg)
Cutsection of the top ASM

### Apr 5, 2024

![apr-5-2024-meeting](/SD403/Media/Images/04_05_2024_Meeting.jpg)

### Apr 11, 2024

![apr-11-2024-meeting-1](/SD403/Media/Images/04_11_2024_Meeting_1.jpg)
![apr-11-2024-meeting-2](/SD403/Media/Images/04_11_2024_Meeting_2.jpg)
![apr-11-2024-meeting-3](/SD403/Media/Images/04_11_2024_Meeting_3.jpg)
![apr-11-2024-meeting-4](/SD403/Media/Images/04_11_2024_Meeting_4.jpg)
![apr-11-2024-meeting-5](/SD403/Media/Images/04_11_2024_Meeting_5.jpg)

https://github.com/RyanHlucny/LED_POV/assets/31110301/fd784f13-caec-4af9-b8e1-2b49033294a1

First test running with the COTS slipring. All the electronics are spinning with the machine here, and it's getting power from a slip ring device.

### Apr 18, 2024

![Apr-18-2024-meeting-1](/SD403/Media/Images/04_18_2024_Meeting_1.jpg)
![Apr-18-2024-meeting-2](./SD403/Media/Images/04_18_2024_Meeting_2.jpg)
