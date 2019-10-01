# Programmable Logic Controller (PLC)
This section will briefly describe what a PLC is and it is a key component for production environments and then focus on
the setup of the Soft-PLC used for this testbed and the program that it executes. This page is structured as follows:

1. [PLC Basics](#plc-basics)
    1. [What is a PLC?](#what-is-a-plc?)
    2. [What is a Soft-PLC?](#what-is-a-soft-plc?)
2. [The PLC as the heart of our testbed](#the-plc-as-the-heart-of-our-testbed)
    1. [Setup](#setup)
    2. [Program](#program)

## PLC Basics
As the successor of traditional relay circuits, which are basically switches that can be toggled electrically, the PLC
has become indispensable for our industrialized world. They control production plants as well as critical
infrastructures such as water and power supply. The PLC is the interface between the virtual world of software and the
physical world of signals and circuits. The fact that is is deployed almost everywhere makes it the Achilles tendon of
many machines, plants and infrastructures.

### What is a PLC?
Typically, a PLC is designed to endure even in rough industrial environments. It has a small form factor so it can be
mounted on top-hat-rails. A PLC consists out of a CPU, memory and said inputs and outputs. Further it has a series of
digital and or analogue inputs and outputs that are controlled by its internal logic, i.e. the implemented program.
These programs are very low level an run close to the actual hardware. Without the need for a full-fledged operating
system, a PLC does not requires less computing power. Especially the lack of a scheduler, a central component of
operating systems that splits the processing power between all processes, allows PLCs to meet industrial real-time
requirements.

### What is a Soft-PLC?
A regular PLC is software and hardware molded together inseparably. A Soft-PLC is the software of a PLC on its own, which
then can be installed on various hardware platforms. Depending on the underlying hardware, the Soft-PLC has more or
fewer inputs and outputs, computing power or other interfaces. This makes PLCs much more flexible and more affordable
due to open-source software and inexpensive hardware of one's choosing. 

## The PLC as heart of the testbed
One of the fundamental requirements for this project is to keep all components open-source and affordable. Out of these
reasons we decided to use a Raspberry Pi as hardware platform to host the Soft-PLC
[OpenPLC](https://openplcproject.com). 

### Setup

### Program
![Alt text](./list-network.png)
