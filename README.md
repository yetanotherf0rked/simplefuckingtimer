# SimpleFuckingTimer

A minimal overlay timer for speedrunning with global hotkeys on Linux.

## Features
- **Overlay Display:** Overlays the timer over all windows.
- **Global Hotkeys:** Use keys for start/split/reset and pause/unpause.
- **Platform:** Designed primarily for Linux (initially Debian-based systems).

## Usage
- **INSERT:** Start, Stop, and Reset.
  - First press: Starts the timer.
  - Second press: Stops it and displays your time.
  - Third press: Resets the timer.
- **PAUSE:** Pause and Resume the timer.
- **HOME:** Undo Split.

## Install from Build
   Install necessary packages
   ~~~bash
   sudo apt update
   sudo apt install build-essential libsdl2-dev libsdl2-ttf-dev libx11-dev
   ~~~

   Build then install
   ~~~bash
   make
   sudo make install
   ~~~

## Running the Application
- **From the Command Line:**  
  Run:
  ~~~bash
  simplefuckingtimer
  ~~~
- **From the Desktop Environment:**  
  Look for "Simple Fucking Timer" in your application menu.

## Uninstalling the Application
To remove the application, run:
~~~bash
sudo make uninstall
~~~
