# Create a moisure sensor with ESP32. 


## Goal

* Create a moisture sensor for a flower pot
* with ESP32
* with WiFi
* Send an e-mail when the flower pot is too dry.


## Overview

We start creating development environment.

1. Prepare tools
* Install VS code
* Add Platform IO extention to VS code
* Create a new project

2. Code to measure threshould (without e-mail part)
* Write measure_threshold.cpp
* Build and upload the code to ESP32
* Measure the threshould

3. Test deep sleep
* Write mosture_deepsleep.cpp
* With deep sleep function
* Measure voltage decline
* Build and upload the code

4. E-mail part
* Create new gmail account for ESP32
* Credential?
* Write the code again with e-mail part
* Build and upload the code
* Test it


---
## Prepare development environment

### VS code
[Instruction](https://code.visualstudio.com/download)

### PlatformIO
This is to replace Arduino IDE.
[Instruction](https://platformio.org/install/ide?install=vscode)

Create a new project.
[Create a new project](./images/platformio_newproject.png)

Your directoy should now look like
```
.
├── LICENSE
├── README.md
├── include
│   └── README
├── lib
│   └── README
├── platformio.ini
├── src
│   └── main.cpp
└── test
    └── README
```
We will work with ```main.cpp```.


---
## Code to measure threshold

### Prepare hardware


---
# END