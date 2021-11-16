# Centrifugal Ring Positioner

The Centrifugal Ring Positioner (CRP) consists of a novel experimental setup for control engineering teaching. 
It has been developed through a master thesis project in the Department of Control Engineering and System Analysis of the Université libre de Bruxelles, Belgium (https://saas.ulb.ac.be/).
The CRP aims at illustrating several concepts of closed-loop control (system identification, model validation, PID control...) within a significant
multidisciplinary context. The setup is mainly composed by 3D-printed pieces. Hence, it is easy-to-build, easily reproducible and affordable.

The aim of this repository is to share this novel experimental setup and to provide all the required information and files to reproduce a CRP. 

The CRP is an unstable and nonlinear system, consisting of a ring able to slide on a rod thanks to the balance between gravity and centrifugal force. The control of the system aims at stabilizing the ring at any fixed position on the rod.

<p align="center"><img src="https://user-images.githubusercontent.com/88432741/128845997-de1a78d8-3a76-4689-8fee-f60a324d0faf.jpg" width="300" height="300">

A video of the CRP under PID cascade control can be found on the following link:
https://youtu.be/nnhnmA6dPzQ 

## Getting Started
This repository is divided in different folders:
* CAD: contains the Solidworks and STL files of the building pieces of the CRP
* Codes (Arduino & MATLAB): contains the Arduino and MATLAB files used to simulate and control the CRP
* Components: contains the datasheets of the components of the CRP
* Papers: contains the master thesis paper of the projet and the related published paper
* Teaching material: contains the teaching material related to the use of the CRP in our department.

## Components
* Microcontroller: Arduino DUE
* Motor: Maxon RE25 Precious Metal Brushes CLL 10W
* Motor driver: Escon 50/5 (+ 24V power supply)
* Ring position sensor: infrared Sharp GP2Y0A41SK0F
* Motor encoder:  Maxon HEDS 5540
* Slip ring: Senring H2056-0610
* Capacitors: 10 μF, 330 nF.
  
## Building pieces
Except the rod, all the pieces are 3D-printed. The CAD files can be found in the folder _CAD_.
 
## Softwares
The following softwares have been used for building and controlling the CRP. Alternatives can be used.
* Solidworks: CAD design of the building pieces
* MATLAB/Simulink: controllers design and simulation
* Arduino IDE: Arduino programming.

**All the construction plans and the software are released under license CC BY-NC-SA 4.0 (for the purpose of this license academic and training usage are to be considered non-commercial)**
