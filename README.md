# SimpleFuckingTimer

<div align="center">
  <img src="assets/sft-logo-256.png" alt="SimpleFuckingTimer Logo" width="128" />
</div>


![Build Status](https://github.com/yetanotherf0rked/simplefuckingtimer/actions/workflows/build-deb.yml/badge.svg)
![Latest Release](https://img.shields.io/github/release/yetanotherf0rked/simplefuckingtimer.svg)

A minimal overlay timer for speedrunning with global hotkeys on Linux.

<div align="center">
  <img src="assets/demo.gif" alt="SimpleFuckingTimer Demo" width="128" />
</div>

## Features
- **Overlay Display:** Overlays the timer over all windows.
- **Global Hotkeys:** Use keys for start/split/reset, pause/unpause, and undo split.
- **Platform:** Designed primarily for Linux (initially Debian-based systems).

## Usage
- **INSERT:** Start, Stop, and Reset.
  - First press: Starts the timer.
  - Second press: Stops it and displays your time.
  - Third press: Resets the timer.
- **PAUSE:** Pause and Resume the timer.
- **HOME:** Undo Split.

## Install from Build
Install necessary packages:
~~~bash
sudo apt update
sudo apt install build-essential libsdl2-dev libsdl2-ttf-dev libx11-dev
~~~

Build then install:
~~~bash
make
sudo make install
~~~

## Install from .deb Package
Download the latest .deb file from the [Releases](https://github.com/yetanotherf0rked/simplefuckingtimer/releases) page.

Install the package using:
~~~bash
sudo dpkg -i simplefuckingtimer-YYYYMMDDhhmm-1_amd64.deb
~~~

If there are dependency issues, run:
~~~bash
sudo apt-get install -f
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

