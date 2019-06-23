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
- good old vga-cable (length depends on your needs)
- 7 LEDs (3mm diameter)
- some prototyping pcb
- DIP-switch (switches, see fig. below)

<img src="https://user-images.githubusercontent.com/24255135/59977029-88a6a380-95cc-11e9-8c05-96a98cf860fd.jpg" width="500">

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
This package serves two purposes. The primariy usage is of course to trigger two cameras simultaneously. But, who can say if a simultaneous triggering results in a simultaneous begin of integration? That's why the second purpose is the synchronization check.

## Expectations
In this package I tried a new way (to the best of my knowlege). The process aims explicit for the **begin of the integration process**. Suppose one uses a fixed shutter time, then there are two uncertanties left (for the first approch). Once the simultaneous beginnig of integration and second the length of the integration time, or in other words the end of integration.

## Preparing
Preparing for checking the synchronization is really fast. Just connect the cameras with the trigger and open up two instances of the flycap viewer. Next step is to connect the LED-module via a VGA-cable (or correctwise, a 9-pin D-Sub cable) with the trigger module. At that point you are ready to take off.

## Doing
That's the point were it get's a bit tricky. There are three modes provided. The first one is activated by pulling the first DIP-switch. In the default config, four LEDs should be visible in both cameras. If not, shift the LEDs with DIP 3-6 (O-bits in the table below). Each LED is on for 10 ms and according to this each gets shifted by 10 ms in this mode.

![offset-table](https://user-images.githubusercontent.com/24255135/59977159-133bd280-95ce-11e9-99dd-0d942e94d626.png)

Now, try to shift the LEDs in a way where all four LEDs are visible and with the next shift to the left in both cameras the leftmost LED disappears simultaneously. This ensures, that the cameras are synchronized at least with a precision of 10 ms. Repeat this step for the next two modes. A precision from nearly 1 ms could be reached in different test. But a bit practise is required.

The same procedure can be executed for the end of integration, if the shutter time is fix. For a variable shutter time it makes no sense at all.

# Hardware
The BOM is provided in a chapter above. The mounting is still work in progress so hopefully a better solution will follow and a good instruction also.

The below schematic shows how to wire everything up. Consider 1 kOhm for both resistors. It's not listet in the BOM, because in the future it will be changed to an internal pull-up. So, still work in progress.
![image](https://user-images.githubusercontent.com/24255135/59977597-42a10e00-95d3-11e9-81fc-4f5e8e109a76.png)

## 3D-prints
The housing for the trigger works fine, but it would be much better with a non-prototyping pcb. 

You can print it without any supports and with 5% infill. Recommended is PETG, but it also works with PLA. ABS is untested so far.

## PCB
A printed circuit board will follow on occasion and will greatly simplify installation ...

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
* check synchronization automatically (very faaaar future)

## Credits
I'm happy if you can make use of this software and even more if you can use it in your academic work. If you do so, feel free to cite this by [![DOI](https://zenodo.org/badge/183741394.svg)](https://zenodo.org/badge/latestdoi/183741394). At least this software it free and there is no need to cite it.
