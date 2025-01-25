# argb controls for 048d:5711 Integrated Technology Express, Inc. GIGABYTE Device 


![License](https://img.shields.io/badge/License-GPLv3-blue.svg) [![status-badge](https://ci.redrise.ru/api/badges/13/status.svg)](https://ci.redrise.ru/repos/13)

Reference hardware used: Z890 AORUS PRO ICE

### udev rules

To make application work once started by user, and not only from root:
```
# vim /etc/udev/rules.d/99-gigabyte.rules
SUBSYSTEM=="input", MODE="0666"
SUBSYSTEM=="usb", ATTRS{idVendor}=="048d", ATTRS{idProduct}=="5711", MODE="0666"
# udevadm control --reload-rules && udevadm trigger
```
### Research

See SPECIFICATION.md

### License
[![GNU GPLv3 Image](https://www.gnu.org/graphics/gplv3-127x51.png)](https://www.gnu.org/licenses/gpl-3.0.en.html)  

argbColors is a free software licensed under GPL v3.0 It is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY. [GNU General Public License](https://www.gnu.org/licenses/gpl.html) as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.


This application is in no way affiliated with GIGABYTE, Integrated Technology Express or any of its partners