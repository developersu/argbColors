# argb-colors is application for backlight adjusting. Created for VID:PID=048d:5711


[![status-badge](https://ci.redrise.ru/api/badges/13/status.svg)](https://ci.redrise.ru/repos/13)

Reference hardware used: Z890 AORUS PRO ICE, 048d:5711 Integrated Technology Express, Inc. GIGABYTE Device

Information regarding compatible motherboards:

* *[X870 AORUS ELITE WIFI7](https://github.com/developersu/argbColors/issues/2)*
* *[X870E AORUS MASTER](https://github.com/developersu/argbColors/issues/5#issuecomment-2762690239)*
* *[X870E AORUS PRO](https://github.com/developersu/argbColors/issues/3)*
* *[Z890 AORUS ELITE WIFI7](https://github.com/developersu/argbColors/issues/4)*
* Z890 AORUS PRO ICE

Does your motherboard work too? [Come on, tell me!](https://github.com/developersu/argbColors/issues/5)

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

### Requirements

* libusb

### Usage examples

```
Set color ff2f00:
$ argb-colors

Set one color:
$ argb-colors -c ff00ff

Set 'wave' effect:
$ argb-colors -s wave

Set: 
- one color 0000ff and default brightness for region 1, 
- cycle effect with brightness of 2 and intensity of 3 for region 2, 
- impulse effect with intensity of 0 and color of ff0000 for region 3, 
- flash effect with color ff00ff, default value of intensity and brightness of 1 for zone 4, 
- flash2 effect with all default values for zone 5,
- one color of default value with brightness of 3 for zone 5
$ argb-colors -e --c1=0000ff --z1=color --z2=cycle --i2=3 --b2=4 --z3=impulse --i3=0 --c3=ff0000 --z4=flash --c4=ff00ff --b4=1 --z5=flash2 --z6=color --i6=3 

Set impulse effect with color of af0f0f, intensity of 0 and brightness of 4
$ argb-colors -s impulse -i 0 -b 4 -c af0f0f

Get help and usage information:
$ argb-colors --help
```

##### Todo:
* Remove feature with '-v' option that occasionally causes segfault
* Set correct CI pipeline

### License
[![GNU GPLv3 Image](https://www.gnu.org/graphics/gplv3-127x51.png)](https://www.gnu.org/licenses/gpl-3.0.en.html)  

argb-colors is a free software licensed under GPL v3.0 It is distributed in the hope that it will be useful, but **WITHOUT ANY WARRANTY**. [GNU General Public License](https://www.gnu.org/licenses/gpl.html) as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.


This application is in no way affiliated with GIGABYTE, Integrated Technology Express or any of its partners