# Centrifugal Ring Positioner

The Centrifugal Ring Positioner (CRP) consists of a novel experimental setup for control engineering teaching. 
It has been developed through a master thesis project of the Department of Control Engineering and System Analysis of the Université libre de Bruxelles, Belgium.

The aim of this repository is to share this novel experimental setup and to provide all the required information and files to reproduce a CRP. 
This work is already reported in "A Project-Based Learning Approach for Building an Affordable Control Teaching Lab: the Centrifugal Ring Positioner", (date + revue).

<img src="https://user-images.githubusercontent.com/88432741/128845997-de1a78d8-3a76-4689-8fee-f60a324d0faf.jpg" width="300" height="300">

https://youtu.be/nnhnmA6dPzQ 

## Getting Started
This repository is divided in different folders
* Arduino: contains the Arduino files required to use the CRP
* CAD: contains the Solidworks files of the building pieces of the CRP
* Components: contains the datasheets of the components of the CRP
* Papers: contains the master thesis paper of the projet and the related published paper.

## Design 
Except the rod, all the pieces are 3D-printed. The CAD files are provided in the folder _CAD_.

## Components

* Microcontroller: Arduino DUE
* Motor: Maxon RE25 Precious Metal Brushes CLL 10W
* Motor driver: Escon 50/5 (+ 24V power supply)
* Ring position sensor: infrared Sharp GP2Y0A41SK0F
* Motor encoder:  Maxon HEDS 5540
* Slip ring: Senring H2056-0610
* Capacitors: 10 μF, 330 nF
