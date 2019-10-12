# Low-cost ICS Testbed - Remote IO

![RemoteIO](../../images/pcb_lowres.png)

## PCB Components

Setting up the PCBs for the two remote IOs is an easy two step process. First you order the PCB and the components, as
described below, and then it takes some basic soldering skills to put it together. This is the most basic, cheap and fun
way to do it. Yet there [alternatives][0.0], should that not appeal to you.

[0.0]: (##alternatives)

# List of PCB Components

| Label        | Name                                         | Amount | Description           |
|--------------|----------------------------------------------|--------|-----------------------|
C1             | 0603 100nF Capacitor                         | 1      | Optional              |
D1-D4          | 0603 LED                                     | 4      |                       |
C1             | ATECC508A OIC-8_3.9x4.9mm_P1.27mm	          | 1      | Optional              |
J1             | PinHeader_1x04_P2.54mm_Horizontal            | 1      |                       |
P1             | Socket_Arduino_Uno:Socket_Strip_Arduino_1x08 | 1      |                       |
P2             | Socket_Arduino_Uno:Socket_Strip_Arduino_1x06 | 1      |                       |
P3             | Socket_Arduino_Uno:Socket_Strip_Arduino_1x10 | 1      |                       |
P4             | Socket_Arduino_Uno:Socket_Strip_Arduino_1x08 | 1      |                       |
P9	           | Pin_Headers:Pin_Header_Straight_1x10 2.54mm  | 1      |                       |
Q1, Q3         | BSS138 Package_TO_SOT_SMD:SOT-23             | 2      |                       |
Q2, Q4         | BSS83P Package_TO_SOT_SMD:SOT-23             | 2      |                       |
R1-R10         | 0R Resistor_SMD:R_0603_1608Metric            | 10     |                       |
R21-R24        | 1k Resistor_SMD:R_0603_1608Metric            | 4      |                       |
R11-R14, R19, R20, R27-R42, R45, R46, R48-R50, R52 | 10 Resistor_SMD:R_0603_1608Metric | 28  | |
R47, R51       | 2k2 Resistor_SMD:R_0603_1608Metric           | 2      |                       |


## PCB Ordering

For our LICSTER prototypes we ordered the PCB at [JLCPCB][1.0]. They are fast and cheap, though any
other PCB producer will do just as well. In the folder [Fabrication][1.1] you find all the files
needed to order the PCB. In the case of JLCPCB, you need only zip them together and upload them to [upload][1.2].
After you uploaded the files, you can choose the color of the PCB, while all other functional requirements are already
set. Please note, that at JLCPCB the minimum amount of PCBs you can order is five.

[1.0]: (https://jlcpcb.com/)
[1.1]: (hardware/Fabrication)
[1.2]: (https://jlcpcb.com/quote#/?orderType=1&stencilWidth=100&stencilLength=100&stencilCounts=5&stencilLayer=2&stencilPly=1.6&steelmeshSellingPriceRecordNum=A8256537-5522-491C-965C-646F5842AEC9&purchaseNumber=)

## Placing Components

The folder [schematics][2.0] contains a schema for the PCB which tells you where the components need to
be placed. Soldering the components onto the PCB requires basic soldering skills and an off-the-shelf soldering iron.

[2.0]: (hardware/Schematic)

## Alternatives

Should you, for any reason whatsoever, do not want to use the open remote IO solution provided here, you can easily exchange
it with most any other remote IO component on the market that is able to communicate via Modbus/TCP, such as [Remote IO][3.0].

[3.0]: (https://amzn.to/2B4KCLH)
