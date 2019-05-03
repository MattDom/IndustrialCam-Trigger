# IndustrialCam-Trigger

This is a package for triggering and synchronizing industrial cameras by [Point Grey Research](https://www.flir.com/) (now FLIR) in a stereo-setup. Please read this readme carfully before usage.  

In principle every camera that provides a hardware trigger can be used with this package. But, the pin assignments/timings/etc. may vary.

| WARNING: Incorrect pin assignments can cause expensive hardware damage! There is no warranty for nothing at all! |
| --- |

## Requirements
The below listed hard- and software items are what is required to run everything without changes. For other µCs or not supported camera models some modifications are necessary.

### Hardware
- µC: Arduino nano (ver. 3.0, ATmega 328p) 
- and a socket for it
- Hirose HR10 6-pin female
- Cable: LiFYDY 7x0,1mm<sup>2</sup>
- Rocker switch (if you plan to use the 3D-printed housing, consider [this switch](https://www.digikey.com/product-detail/en/zf-electronics/SRB22A2FBBNN/CH755-ND/446021))
- pushbutton (for the printed housing use a button like [this](https://www.digikey.de/product-detail/de/e-switch/TL1105BF160Q/EG1833-ND/1358))
- female 9-pin sub-d
- male 9-pin sub-d
- good old vga-cable (length depends on you needs)
- 7 LEDs (3mm diameter)
- some prototyping pcb

### Software
There is no need for additional software or SDKs. This code was developed and originally compiled with Atmel Studio 7.0 (gcc 5.4.0).   
For adjusting and saving the neccesary options, the [Flycapture-SDK](https://www.flir.de/products/flycapture-sdk/) by FLIR may be a good choice. But, this is not mandatory for using this package. However, for using the cameras with [ROS](https://www.ros.org/) another possible configuration will be provided later.

# Usage

In general there are two modes to run the triggered stereo-system. At first there is a single-shot mode, which trigger both cameras simultaniously for one frame. The second one is the loop-mode. This is the *normal* mode, that runs continously with 10 Hz. Actually this frame rate it fix, but not *un-editable*. If the single-shot mode is not needed, just connect __PD2__ and __PD3__ to ground to avoid strange effects.

## Single-Shot

The single-shot mode is choosen with the rocker switch, attached on __PD3__. Once the single-shot mode is activated the next frame it triggered by pushing the pushbutton linked to __PD2__.

## Loop-mode

The loop-mode is executed with 10 Hz by pulling __PD3__ to ground. The state of __PD2__ has no effect in this state.

# Synchronization
## Expectations
## Preparing
## Doing

# Hardware
## 3D-prints
## PCB

# Additional notes
Some additional informations for using this package.
## ROS
With ROS, the usage of the _Flycatpure_-SDK is not needed for this package. For example use (this driver-node)[https://github.com/ros-drivers/pointgrey_camera_driver]. The only mandatory parameter is listed below.

```C
# Trigger parameters
gen.add("enable_trigger",       bool_t,   SensorLevels.RECONFIGURE_RUNNING,   "Enable the external triggering mode.",                                                        False)
```
However, this package was designed for using the following parameters in the ROS launchfile.

## Upcoming features
* Changing frame rate individuals
* Displaying the actual synchronization-offset with sevseg-modules
* Trigger both cameras separately and save the correct delays in the eeprom
