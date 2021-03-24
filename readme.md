[![pipeline status](https://github.com/khorram-dev/komport/blob/master/komport/resource/png/32-apps-komport.png)](https://github.com/khorram-dev/komport)

## Welcome to _Komport_

Komport is a graphical serialport and socket terminal
And is a tool for monitoring system serial ports.
This program monitors, displays, logs and analyzes all serial ports, including RS232/422/485.

[![pipeline status](https://github.com/khorram-dev/komport/blob/master/komport/resource/png/btn_donate_LG.webp)](https://www.paypal.com/donate?hosted_button_id=VG6P3S4ELVXJ2)

### Features:

*   used KDE Frameworks
*   Color Scheme
*   serialport and socket
*   plugins (List command, Log view)


### Build instructions

Linux
----------------------------------------------------------------------------------------
*  cmake . -DCMAKE_BUILD_TYPE=Release
*  make -j4
*  make install


Mingw
----------------------------------------------------------------------------------------
*  x86_64-w64-mingw32-cmake . -DCMAKE_BUILD_TYPE=Release
*  x86_64-w64-mingw32-make -j4
*  x86_64-w64-mingw32-make install DESTDIR=path-output


#### Requirements for Building:

*   kde >= 5.15, Qt >= 5.4, CMake >= 3
*   the qt5 development packages including QSerialport
*   Since C++C11 features are used a gcc supporting these is needed too

#### Release for windows 64bit
https://github.com/khorram-dev/komport/releases

#### for Arch Linux
https://gist.github.com/khorram-dev/faa4059104b1bfb85aa16ffe8c17e7f9

### Screenshot

![Screenshots](https://github.com/khorram-dev/komport/blob/master/doc/screenshot_1.png)

