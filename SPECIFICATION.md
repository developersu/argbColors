# Uh, umm... well... let's call it specification of the 048d:5711 Integrated Technology Express, Inc. GIGABYTE Device

**USB control transfer used**

(and USB HID a bit 🤓)

Endpoint: **0x00**, Direction: **OUT**
* bmRequestType: **0x21**
* bRequest: **9**
* wValue: **0x03cc**
* wIndex: **1 (0x0001)**
* wLength: **64**
* Data fragment: as described below. Trailing zeroes trimmed in this document however must be defined within actual data interaction. Therefore 'wLength: 64' is imperative'.

In libusb semantic:
```
unsigned char message[64] = { 0XCC, 0X60, }; // example 'cc60'
int returned_value = libusb_control_transfer(dev_handle, 0x21, 9, 0x03cc, 1, message, 64, TIMEOUT);
```

Endpoint: **0x80**, Direction: **IN**
* bmRequestType: **0xa1**
* bRequest: **1**
* wValue: **0x03cc**
* wIndex: **1 (0x0001)**
* wLength: **64**

In libusb semantic:
```
unsigned char buf[64] = {};
int returned_value = libusb_control_transfer(dev_handle, 0xa1, 1, 0x03cc, 1, buf, sizeof(buf), TIMEOUT);
```
**NOTE:** input values below marked with prefix "INPUT << "

----

Interaction could be split on three phases: Application initial flow, command(s) transmission, termination

### Application initial flow:
```
cc60             ↓↓↓↓↓↓↓↓ version in hex. I guess. At least looks like this.
INPUT << cc01000100001802000000034954353731312d47494741425954452056302e302e32342e320000000000000002000100020001000001020000c01157
cc60                             ↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑ ascii text: 'IT5711-GIGABYTE V0.0.24.2'
INPUT << cc01000100001802000000034954353731312d47494741425954452056302e302e32342e320000000000000002000100020001000001020000c01157
cc34
cc48
INPUT << cc
cc61
in? << cc000000020001000200010002000100020001
cc3c
cc3e
INPUT << cc
cc34
cc3d
cc3f
INPUT << cc
cc34
cc38
```

### Commands / directives to apply changes

*BGR abbreviation stands for Blue Green Red*

There are two types of commands: 
* synchronized - applies same setting to everything
* separate - adjusts settings separately

***Prefix-directives: commands sent before actual sequence of the command. Structure is always the same***
```
cc20
cc21
cc22
cc23
cc24
cc25
cc26
cc27
cc90
cc91
cc92
cc28ff07
```
After application initial flow sent and till termination (see below) any command could be applied randomly. One after one or in completely random sequence. 
Let's summarize: full command sequence is prefix-directives plus actual command sequence (as explained in sections below).

~~Butt~~But actually 'prefix-directives' always sent:
1. Before switching between 'separate' commands
2. Before switching from 'separate' command already applied to 'synchronized' command
3. Before switching from 'synchronized' command to 'separate'
4. Before initial application of the 'synchronized' command

Therefore no need to send 'prefix-directives' once switching between two 'synchronized' commands. Is it safe send it either way? Could be.

---
#### Turn off backlight - Applies to everything!
```
cc20ff070000000000000001ff
cc28ff07
```
EXAMPLE:
`cc20ff070000000000000001ff000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000`

---

#### Turn off backlight (user selects: channel)
```
  ↓↓↓↓↓↓-----------------------|
cc2204000000000000000001ff   ← 1   |‾‾|‾‾‾‾‾‾‾‾‾‾‾4|
cc2410000000000000000001ff   ← 2   | 6| |‾‾| ||||  |
cc2520000000000000000001ff   ← 3   |  |  ‾‾  ||||  |
cc2640000000000000000001ff   ← 4   |__|      |‾‾‾| |
cc2780000000000000000001ff   ← 5   |         |_1_| |
cc9100020000000000000001ff   ← 6   |___2_3_5_______|
cc28ff07
```
EXAMPLE:
`cc2204000000000000000001ff000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000`

---

#### Wave 1 - Applies to everything!
```
cc20ff070000000000000001ff ← turns off backlight
cc28ff07
cc220000000000000000000964000000000000000000260200000000000007
cc2804
```
EXAMPLE:
`cc220000000000000000000964000000000000000000260200000000000007000000000000000000000000000000000000000000000000000000000000000000`

---

#### Wave 2 - Applies to everything!
```
cc20ff070000000000000001ff ← turns off backlight
cc28ff07
cc220000000000000000000a640000000000000000007d0000000000000020
cc2804
```
EXAMPLE:
`cc220000000000000000000a640000000000000000007d0000000000000020000000000000000000000000000000000000000000000000000000000000000000`

---

#### Individually set static color (user selects: color, brightness)
```
                            ↓↓↓↓↓↓ → Color as BRG
cc2204000000000000000001__00______ 
cc2410000000000000000001__00______
cc2520000000000000000001__00______
cc2640000000000000000001__00______
cc2780000000000000000001__00______
cc9100020000000000000001__00______
cc28ff07                ↑↑
                        ff → Brightness max
                        b3               -1
                        80               -2
                        4d               -3
                        1a               -4
```
EXAMPLE:
`cc2204000000000000000001ff00ffffff0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000`

---

#### Synchronized static color (user selects: color) - Applies to everything!
```
                            ↓↓↓↓↓↓ - Color as BRG
cc20ff070000000000000001ff00______
cc28ff07
```
EXAMPLE:
`cc20ff070000000000000001ff00ffffff0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000`

---

#### Impulse (user selects: color, intensity)
```
                            ↓↓↓↓↓↓ - Color as BRG
cc22040000000000000000026400______0000000000____________00000001
cc24100000000000000000026400______0000000000____________00000001
cc25200000000000000000026400______0000000000____________00000001
cc26400000000000000000026400______0000000000____________00000001
cc27800000000000000000026400______0000000000____________00000001
cc91000200000000000000026400______0000000000____________00000001
cc28ff07                                    ↑↑↑↑↑↑↑↑↑↑↑↑
                                            f401f401fa00 → Intensity max 
                                            bc02bc025e01             - 1
                                            84038403c201             - 2
                                            b004b004f401             - 3
                                            400640062003             - 4
```
EXAMPLE:
`cc220400000000000000000264000040fe0000000000b004b004f401000000010000000000000000000000000000000000000000000000000000000000000000`

---

#### Flash (user selects: frequency, color and brightness)
```
                            ↓↓↓↓↓↓ - Color BGR
cc2204000000000000000003__00______000000000064006400____0000000101
cc2410000000000000000003__00______000000000064006400____0000000101
cc2520000000000000000003__00______000000000064006400____0000000101
cc2640000000000000000003__00______000000000064006400____0000000101
cc2780000000000000000003__00______000000000064006400____0000000101
cc9100020000000000000003__00______000000000064006400____0000000101
cc28ff07                ↑↑                          ↑↑↑↑                  
                        ff → Brightness max         2003 → Frequency max      
                        b3              -1          b004               -1
                        80              -2          4006               -2
                        4d              -3          d007               -3
                        1a              -4          6009               -4
```
EXAMPLE:
`cc2204000000000000000003ff00ffffff0000000000640064002003000000010100000000000000000000000000000000000000000000000000000000000000`

---

#### Flash-double (user selects: brightness, color and frequency)
```
                            ↓↓↓↓↓↓ - Color BRG
cc2204000000000000000003__00______000000000064006400____0000000102
cc2410000000000000000003__00______000000000064006400____0000000102
cc2520000000000000000003__00______000000000064006400____0000000102
cc2640000000000000000003__00______000000000064006400____0000000102
cc2780000000000000000003__00______000000000064006400____0000000102
cc9100020000000000000003__00______000000000064006400____0000000102
cc28ff07                ↑↑                          ↑↑↑↑                       
                        ff → Brightness max         e803 → Frequency max
                        b3               -1         7805              -1
                        80               -2         0807              -2
                        4d               -3         9808              -3
                        1a               -4         280a              -4
```
EXAMPLE:
`cc2204000000000000000003ff00fd00fe000000000064006400e803000000010200000000000000000000000000000000000000000000000000000000000000`

---

#### Cycle (user selects: intensity and brightness)
```
                            ↓↓↓↓↓↓- Old values; doesn't make any difference
cc2204000000000000000004__00fd00fe0000000000________0000000007
cc2410000000000000000004__00fd00fe0000000000________0000000007
cc2520000000000000000004__00fd00fe0000000000________0000000007
cc2640000000000000000004__00fd00fe0000000000________0000000007
cc2780000000000000000004__00fd00fe0000000000________0000000007
cc9100020000000000000004__00fd00fe0000000000________0000000007
cc28ff07                ↑↑                  ↑↑↑↑↑↑↑↑
                        ff → Bright. max    9001c800 → intensity max
                        b3            -1    58029001              -1
                        80            -2    2602c201              -2
                        4d            -3    5203ee02              -3
                        1a            -4    7805b004              -4
```
EXAMPLE:
`cc91000200000000000000041a00f0f0f000000000007805b0040000000007000000000000000000000000000000000000000000000000000000000000000000`

---
### Termination
[repeat last sequence for some reason. or don't. who knows]
```
cc4701
```

This work is licensed under a  [Creative Commons Attribution 4.0 International License](https://creativecommons.org/licenses/by/4.0/)