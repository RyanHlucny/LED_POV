# HW06 - Task 3 - Interrupts

## Ryan Hlucny

### Task Overview

This project has specific timing requirements for it to work. Anytime when dealing with microcontrollers and precision timing is required, the solution is some kind of interrupt. In our case, I will be using timer interrupts to refresh the LED pixel color data buffer at a higher frequency than the microcontroller's main code loop runs. This will be necessary to get the maximum refresh rate possible and achieve persistence-of-vision.

### Timing

Persistence-of-vision is the phenomenon that allows a flickering object to appear static if the flickering occurs at a high enough frequency. This frequency is known as the **flicker-fusion threshold**. The flicker-fusion threshold is determined by many characteristics. The basic idea is that the flicker-fusion threshold needs to be high enough such that a receptor (our eyes) can no longer sense changes in an object. For our LED Spinner, multiple physical characteristics are changing all the time. Rotation angle $\theta$, angular velocity $\omega$, and the LED colors. Also, though imperceptibly fast, the red, green, and blue micro-LEDs within each LED flicker on and off really fast to achieve the desired color. This technique is known as PWM, and is being used to control the relative brightness of each micro-LED. For example, setting the RGB of a single led to,

| `Red` | `Green` | `Blue` |
| :---: | :---: | :---: |
| 127 | 0 | 255 |

yields a different duty cycle, or "on-time", for each micro-LED. With this color command, the red led has a duty cycle of ~50%, green 0%, and blue 100%.

So there are a lot of visible moving/changing features on the device. In order for persistence-of-vision to hold for a still image, we need all of these changing features to be above the flicker-fusion threshold. For the PWM of the LEDs, that's not an issue (otherwise you would see the flickering of the micro-LEDs while it's running). The greater challenge comes in making each "pixel" on the virtual "screen" appear constant. The challenging part with this is that because the LEDs themselves are spinning, they need to rotate $2\pi$ rad and update its pixel color faster than the flicker-fusion threshold. This is also challenging because the single LED strip needs to do this at every $\Delta\theta$ angle division as it rotates. So how fast does an LED strip need to update in order to achieve this? And how fast does the spinner need to spin?

### The Math

The flicker-fusion threshold for most human adults is 15 Hz, which is a surprisingly low number considering most of our computer screens and digital devices have screens have well above 60 Hz refresh rate. In our math, let's parameterize the refresh rate $f$, angle $\theta$, angular velocity $\omega$, and time $t$.

Assume $\omega=c$ to be constant (steady-state) for computing maximum operating conditions.

Then, $\theta(t) = \omega t$, and the equation of motion for an LED on the blade is $$r_i e^{j\theta(t)} = r_i e^{ j  \omega t}$$

where $r_i$ is the radius of the $i_{th}$ LED.

The refresh rate for a single LED (ie, the frequency of rotation, in Hz) is then $$f = \frac{\omega}{2\pi} = \frac{c}{2\pi}$$

So we simply need to pick $\omega$ such that $f$ is above the flicker-fusion threshold. However, there is another constraint. How fast can the microcontroller set the LEDs colors?

Each SPI transaction starts with a 32-bit start frame, followed by $n$ 32-bit LED frames, and ends with a 32-bit end frame. Thus the total number of bits to transfer in an SPI transaction is $$B = 64 + 32n$$ where n is the total number of LEDs in the strip. The SPI frequency $f_{SPI}$ is 1:1 with the bit rate, $T_b$, since SPI is a synchronous communication method where 1 bit is transfered each clock cycle. That is $T_b = f_{SPI}$. Which means the maximum transactions per second can be given by $$\frac{T_b}{B} = \frac{f_{SPI}}{B} \space\space\space transmissions/sec$$

Now there is going to be some amount of quantization error no matter what, because we are mapping rectilinear image pixels to a polar pixel space, and the polar space doesn't perfectly map to rectangular grid locations. The higher the resolution of the original rectangular image, the lower the quantization error. However, there may be additional quantization loss in making the LUT. The amount of space on a microcontroller is quite limited, so we can only store so much pixel data in the LUT. This is the motivation for limiting the resolution of the image. Only in a polar space, that limiting comes in the form of adjusting the number of angle divisions in the circle and the number of radius divisions along the radius. We denote the angle interval $\Delta\theta$, and the radius interval $\Delta r$. Fortunately, $\Delta r$ is constant and is exactly the distance between each subsequent LED, making our calculations simpler. However, $\Delta\theta$ changes based on $\omega$. For this reason, we will select $\Delta\theta$ to be some sufficiently small number associated with the normal operating conditions of the machine.

Now it would be useful to know the relationship between the SPI frqeuency, image resolution, and angular velocity of the machine. With $\Delta\theta$ intervals on the circle, that correlates to $$D = \frac{2\pi}{\Delta\theta} \space\space\space divisions$$ Each divion represents a whole new array of pixel information to read into each LED strip. Since each LED strip has its own independent SPI interface, we will only discuss one LED strip. This means the number of divisions is the number of transmissions required per rotation $T_r$: $$T_r = D = \frac{2\pi}{\Delta\theta} \space\space\space \frac{transmissions}{revolution}$$

This implies the number of "frames," or full revolutions, the microcontroller can drive in one second is

$$f = \frac{f_{SPI}}{BT_r} \space\space\space revolutions/sec$$

Hence the maximum frame rate that the microcontroller can drive, in terms of $f_{SPI},\space n, \space and \space \Delta\theta$ is $$f=\frac{f_{SPI}}{BT_r}=\frac{f_{SPI}}{B \left(\frac{2\pi}{\Delta\theta} \right)}=f_{SPI}\frac{\Delta\theta}{2\pi B }=f_{SPI}\frac{\Delta\theta}{2\pi \left(64+32n\right) }$$

The maximum SPI frequency I found to be reliable for a full strip was $f_{SPI} = 15.625$ MHz, and the number of LEDs per strip will be 53. Substituting these numbers and iterating on $f$ we get the following table:

| $f_{SPI}$ (MHz) | $n$ |  $f$ Hz  |  $\Delta\theta$ (rad)  |
| :---: | :---: | :---: | :---: |
|  15.625  |  53  |  5.0000  |  0.0035  |
|  15.625  |  53  |  10.0000  |  0.0071  |
|  15.625  |  53  |  15.0000  |  0.0106  |
|  15.625  |  53  |  20.0000  |  0.0142  |
|  15.625  |  53  |  25.0000  |  0.0177  |
|  15.625  |  53  |  30.0000  |  0.0212  |
|  15.625  |  53  |  35.0000  |  0.0248  |
|  15.625  |  53  |  40.0000  |  0.0283  |
|  15.625  |  53  |  45.0000  |  0.0318  |
|  15.625  |  53  |  50.0000  |  0.0354  |
|  15.625  |  53  |  55.0000  |  0.0389  |
|  15.625  |  53  |  60.0000  |  0.0425  |

Since the LUT is a static entity, we must select the $\Delta\theta$ in advance. Therefore, we choose $\Delta\Theta$ based on the desired refresh rate, $f$ that we want to see, and based on how much memory the Pico W has. The lower $f$ is, the smaller $\Delta\theta$, which results in a larger LUT (smaller $\Delta\theta$ implies more divions per rotation, which means more data stored in the LUT).

The LUT consists of a 3D array consisting of radius $r$ (led index) along the first dimension, angle $\theta$ along the second dimension, and the color data (blue, green, red) as unsigned 8-bit integers of type `uint8_t`. One 8-bit number is 1 byte. In our matrix, there would be $3n \left(\frac{2\pi}{\Delta\theta}\right)$ 8-bit integers; however, our device has 2 blades. We decided to offset the LEDs such that the two diametrically opposed blades interleave with each other. This effectively doubles the radius resolution. It also, however, doubles the size of our LUT. So, the LUT actually will have $n_2 = 2n$. So the size of the LUT is $$3n_2 \left(\frac{2\pi}{\Delta\theta}\right) = 3 \cdot 2n \left(\frac{2\pi}{\Delta\theta}\right) = 6n \left(\frac{2\pi}{\Delta\theta}\right) = \frac{12n\pi}{\Delta\theta} \space\space\space bytes$$

Using the values of $f$ and $\Delta\theta$ from the previous table, we have the following LUT sizes:

|  $\Delta\theta$ (rad)  |  LUT size (bytes)  |
| :---: | :---: |
|  0.0035  |  564631  |
|  0.0071  |  282316  |
|  0.0106  |  188211  |
|  0.0142  |  141158  |
|  0.0177  |  112927  |
|  0.0212  |  94106  |
|  0.0248  |  80662  |
|  0.0283  |  70579  |
|  0.0318  |  62737  |
|  0.0354  |  56464  |
|  0.0389  |  51331  |
|  0.0425  |  47053  |

The Pico W has a flash program memory of 2 MB, so any of these should fit. We will go with a dq of 0.0035 to give us the largest LUT we could need for the operating conditions we will see. With $f = 5$, the device will be spinning at 300 RPM (note that this is below the typical **flicker-fusion-threshold** for adults, though it would be fast enough for a proof-of-concept). which will be easily attainable by our hardware and is a good benchmark speed. Anything faster and it will start "skipping" $\Delta\theta$ steps, which is completely fine and will likely be unnoticeable.

### Interrupts

Now that we got all the preliminary calculations done, we need to set up interrupts to command the LEDs. The above calculations assumed that each LED command would follow immediately after the previous command (no dead-zone), which requires a specific frequency of sending SPI transmissions. I will call this transmission frequency $f_{transmission}$.

We calculated above the number of transmissions per second $$ f_{transmission} = \frac{f_{SPI}}{B} = \frac{f_{SPI}}{64+32n} \space\space\space transmissions/sec$$ In this context, $n = 53$, since each led strip has its own SPI interface. With $f_{SPI} = 15.625$ MHz, $$f_{transmission} = \frac{15.625 \cdot 10^6}{64+32\cdot53}$$   =  This is the desired frequency of our interrupt to be able to send LED command SPI transmissions seamlessly, one to the next.



The data sheet of the [SK9822](https://cdn-shop.adafruit.com/product-files/2343/SK9822_SHIJI.pdf) gives a handy formula for computing 


