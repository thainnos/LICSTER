# Low-cost ICS Testbed - Remote IO

![RemoteIO](../../images/pcb_lowres.png)

## PCB Components

Following comes a list with all the components on the PCB. Detailed instructions will come soon.

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

We order our PCBs at [JLCPCB](https://jlcpcb.com/), because they are mostly fast in production and also very cheap, but any PCB producer will be fine.
The LICSTER Github contains a folder called ["Fabrication"](hardware/Fabrication), where all necessary files are located for the PCB producer.
So basically, you just create a zip file from this folder and [upload](https://jlcpcb.com/quote#/?orderType=1&stencilWidth=100&stencilLength=100&stencilCounts=5&stencilLayer=2&stencilPly=1.6&steelmeshSellingPriceRecordNum=A8256537-5522-491C-965C-646F5842AEC9&purchaseNumber=) it.
You can choose a color of the PCB and all other settings are already fine and you can proceed with the checkout (At JLPCP you will get a minimum of 5pcs).

## PCB Manufacturing

In the [schematics folder](hardware/Schematic) of the LICSTER Git, you will find a plan, at which location each component must be placed on the PCB.
The Soldering requires basic skills and a solderin iron.

## Other Possible Solutions

In case, the usage of open remote IO solution is not possible any other Modbus/TCP Remote IO can be used, like these [Remote IO](https://amzn.to/2B4KCLH).
