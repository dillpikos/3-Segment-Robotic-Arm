# 3-Segment-Robotic-Arm
A custom deisgned 3 segment robotic arm powered by ESP-32 microcontrollers communicating via ESP-NOW, a peer to peer wireless protocol. This project implements
FreeRTOS on both the master (transmitter) and slave (receiver) to cordinate concurent tasks that include servo movement and postioning of each segment,communication, and arm rotation. 
This approach pair with 2 joysticks allows us to provide the user with a smooth, low latency control.


*created by: Bryan Diaz & Junior Ortega | Focus: Embedded Systems, Wireless communication, Real time control system*

# Technologies and Components
## Hardware Related
- ESP-32 X 2
- SG90 Servo Motor X 2
- MG666R 55G Servo Motor X 2
- 28BYJ-48 Stepper Motor (rotation of arm)
- Power bank (5000 MaH)
- Joystick Module X 2 (controller)
- Soldering of through hole components

## Software Related
- Arduino IDE
- Fusion 360 (Arm segments, Base of arm, claw (gears for open/close))
- C++ (Custom tasks for speed and steer control eliminating excess memory usage)
- FreeRTOS (Task managment)
- ESP-NOW (...)

# System Features
- Wireless communcation via ESP-NOW
- Real-time task scheduling using FreeRTOS
- Joystick controlled navigation

# System Preview
