[![pipeline status](https://gitub.com/khorram-dev/komport/badges/master/komport.svg)](https://gitlab.com/khorram-dev/komport/commits/master)

## Welcome to _Komport_

Komport is a graphical serialport and socket terminal
And is a tool for monitoring system serial ports.
This program monitors, displays, logs and analyzes all serial ports, including RS232/422/485.


### Features:

*   used Kde GUI
*   Color Scheme
*   serialport and socket
*   plugins (List Command)


### Build instructions

Linux

cmake . -DCMAKE_BUILD_TYPE=Release -DBUILD_TESTING=OFF
make -j4
make install

Mingw

x86_64-w64-mingw32-cmake . -DCMAKE_BUILD_TYPE=Release -DBUILD_TESTING=OFF
x86_64-w64-mingw32-make -j4
x86_64-w64-mingw32-make install DESTDIR=path-output

package builder for Arch Linux
makepkg -si


#### Requirements for Building:

*   Komport 0.1 +: kde >= 5.15, Qt >= 5.4, CMake >= 3
*   on linux look for the qt5 development packages including QSerialport
*   Since C++C11 features are used a gcc supporting these is needed too


### Screenshot

![Screenshots](komport.png)

