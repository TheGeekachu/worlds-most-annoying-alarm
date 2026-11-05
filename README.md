# annoying-alarm

## Description

This is the World's Most Annoying Alarm, made with Onshape, LibrePCB, and a lot of pain. It is an alarm clock designed to blare like a siren in the morning and display insults on the screen every few seconds till you wake up. It has 4 buttons, one for turning off the alarm, one for insulting you during the day if you feel like it, and two unprogrammed ones. All firmware is open source, so you can adjust the insults, and changes will be continually made to the official firmware. Has a TFT screen for displaying insults and all 4 buttons are high-quality mechanical keyboard buttons that are very satisfying to play with.

## Features and Parts:

* Full-color TFT to display insults
* Very loud buzzer to wake you up.
* USB-C powered ESP32 microcontroller for running the firmware
* Well-designed aesthetically pleasing case for when you get made at it and throw it off the balcony
* 4 very clacky ASMR keys for pressing and stuff
* Several custom-made premium insults in the official firmware
* Overall, one very annoying alarm clock.
* 3D printable case so you can make a replacement if you have a very high balcony
* Full BOM not including case material in the production files.

## Issues and Frustrations I've had:

* Continual review process and logging hours
* Figuring out differences between KiCAD and LibrePCB
* Firmware, it's been a while since I've used Arduino for something this complicated, might switch the firmware to CircuitPython.
* LibrePCB not having pre-installed footprints that I need, so going hunting in the official KiCAD repo on GitLab.
* Neither Libre nor KiCAD having the 3D models, so going on GrabCAD for models.
* Schoolwork while doing all this stuff.
* Writing this README. (The Hardest Part!!!!!!)

## Devlogs

**Devlog 1**

This is a [LibrePCB](https://librepcb.org) project!
So anyway, yeah, I made this in LibrePCB instead of KiCAD, not sure if that's against the rules for BLARE. If you want to see my outputs n' stuff, go to the output/v1 folder and you'll find gerber files, pdf schematics, and so on and so forth. I don't know if I can put the 3D printable files for the rest of the project INSIDE this one, but we'll find out. So, yeah. This is my first commit, and the PCB is finished. YAY!!!!

**Devlog 2**

Okay, so I finished the CAD work as well, I'm putting the CAD file in the root folder. Onshape gave me a lot of issues, but I perservered, and I'm done here. So, you should see a STEP file in the root folder for the CAD of the final thing with all parts included. Stay on the creature trail!

**Devlog 3**

Finished firmware, CAD is in CAD folder, firmware is in Firmware folder, and the actual Gerber files and PCB stuff is in the PCB folder. The alarm stops with a press of the left key, and the clock insults you with the second key. Every time it BLAREs, it also insults you. Apart from that, it's a normal clock that displays the time. FIrmware is still a WIP, I need to code the other keys, but thanks for being here!

## Pictures! 
![alt text](FinishedCADforMostAnnoyingAlarmClock.png)
![alt text](AnnoyingAlarmFIrmware.png)
![alt text](AnnoyingAlarm3d1.png)
![alt text](AnnoyingAlarm3d2.png)
![alt text](AnnoyingAlarmScheme.png)

## License

No license set.
