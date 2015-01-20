### DISCLAIMER
### This is an example Makefile and it MUST be configured to suit your needs.
### For detailled explanations about all the avalaible options,
### please refer to https://github.com/sudar/Arduino-Makefile/blob/master/arduino-mk-vars.md
### Original project where this Makefile comes from: https://github.com/WeAreLeka/Bare-Arduino-Project

###
### Because avrdude doesn't understand the ~ for home directory, define the current working directory
### and use it instead.
###
CURRENT_DIR = $(shell pwd)

### PROJECT_DIR
### This is the path to where you have created/cloned your project
### EJR - sadly, "avrdude" doesn't understand the "~" shorthand for home directory, so /home/eric has to be used...BOO!!!
###
PROJECT_DIR       = $(CURRENT_DIR)

### ARDUINO_DIR
### Path to the Arduino application and ressources directory.
### On OS X:
### ARDUINO_DIR       = /Applications/Arduino.app/Contents/Resources/Java
### or on Linux: (remove the one you don't want)
ARDUINO_DIR       = /usr/share/arduino

### ARDMK_DIR
### Path to the Arduino-Makefile directory.
ARDMK_DIR         = $(ARDUINO_DIR)

### USER_LIB_PATH
### Path to where the your project's libraries are stored.
USER_LIB_PATH    :=  $(PROJECT_DIR)/lib

### BOARD_TAG
### It must be set to the board you are currently using. (i.e uno, mega2560, etc.)
### BOARD_TAG         = uno
BOARD_TAG         = pro5v328

### MONITOR_BAUDRATE
### It must be set to Serial baudrate value you are using.
### MONITOR_BAUDRATE  = 115200
MONITOR_BAUDRATE  = 38400

### AVR_TOOLS_DIR
### Path to the AVR tools directory such as avr-gcc, avr-g++, etc.
### On OS X with `homebrew`:
### AVR_TOOLS_DIR     = /usr/local
### or on Linux: (remove the one you don't want)
AVR_TOOLS_DIR     = /usr

### AVRDDUDE
### Path to avrdude directory.
### On OS X with `homebrew`:
### AVRDDUDE          = /usr/local/bin/avrdude
### or on Linux: (remove the one you don't want)
AVRDDUDE          = /usr/bin/avrdude

### CFLAGS_STD
### Set the C standard to be used during compilation.
### Documentation (https://github.com/WeAreLeka/Arduino-Makefile/blob/std-flags/arduino-mk-vars.md#cflags_std)
CFLAGS_STD        = -std=gnu11

### CXXFLAGS_STD
### Set the C++ standard to be used during compilation.
### Documentation (https://github.com/WeAreLeka/Arduino-Makefile/blob/std-flags/arduino-mk-vars.md#cxxflags_std)
CXXFLAGS_STD      = -std=gnu++11

### CXXFLAGS
### Flags you might want to set for debugging purpose. Comment to stop.
### EJR ** Tue Jan 20 14:30:51 2015 **
###	- added -Wno-write-strings 'cause I don't care about the automatic string conversion (remove to see what I mean)
###	- added -Wno-endif-labels 'cause we use them all of the time!
###	- added -Wno-variadic-macros 'cause Arduino.h uses them!
###	- added -Wno-comment 'cause we don't really care about multi-line comments (not normally a problem)

CXXFLAGS         += -pedantic -Wall -Wextra -Wno-write-strings -Wno-endif-labels -Wno-variadic-macros -Wno-comment

### MONITOR_PORT
### The port your board is connected to. Using an '*' tries all the ports and finds the right one.
MONITOR_PORT      = /dev/ttyUSB*

### CURRENT_DIR
### Do not touch - used for binaries path
CURRENT_DIR       = $(shell basename $(CURDIR))

### OBJDIR
### This is were you put the binaries you just compile using 'make'
OBJDIR            = $(PROJECT_DIR)/bin/$(BOARD_TAG)/$(CURRENT_DIR)

### path to Arduino.mk, inside the ARDMK_DIR, don't touch.
include $(ARDMK_DIR)/Arduino.mk
