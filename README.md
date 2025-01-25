# argb controls for 048d:5711 Integrated Technology Express, Inc. GIGABYTE Device 


![License](https://img.shields.io/badge/License-GPLv3-blue.svg) ![status-badge](https://ci.redrise.ru/api/badges/13/status.svg)](https://ci.redrise.ru/repos/13)

Reference hardware used: Z890 AORUS PRO ICE

### udev rules

To make application work once started by user, and not only from root:
```
# vim /etc/udev/rules.d/99-gigabyte.rules
SUBSYSTEM=="input", MODE="0666"
SUBSYSTEM=="usb", ATTRS{idVendor}=="048d", ATTRS{idProduct}=="5711", MODE="0666"
# udevadm control --reload-rules && udevadm trigger
```

### License
GPLv3 or higher

### Research

See SPECIFICATION.md

This application is in no way affiliated with GIGABYTE, Integrated Technology Express or any of its partners