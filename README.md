# 3-Segment-Robotic-Arm
A custom designed 3 segment robotic arm powered by ESP-32 microcontrollers communicating via ESP-NOW, a peer to peer wireless protocol. This project implements
FreeRTOS on both the master (transmitter) and slave (receiver) to coordinate concurrent tasks that include servo movement to position each segment, communication, and full arm rotation. 
This approach paired with 2 joysticks allows us to provide the user with a smooth, low latency control.

On the master (transmitter) side, FreeRTOS manages tasks for joystick readings from both joysticks and processing to successfully transfer control data to the reciever.
On the slave (receiver) side, FreeRTOS manages tasks for recieving transmitted data turning it into servo movement (postion & claw control), along with general rotation.


*created by: Bryan Diaz & Junior Ortega | Focus: Embedded Systems, Real time control system, Robotics*

# Technologies and Components
## Hardware Related
- ESP-32 (X2)
- SG90 Servo Motor (X2)
- MG666R 55G Servo Motor (X3) [General arm movement & rotation of arm]
- Power bank (5000 MaH)
- Joystick Module (X2) [controller]
- Push button [push down = claw close | at rest = claw open]
- Soldering of through hole components

## Software Related
- Arduino IDE
- Fusion 360 (Arm segments, Base of arm, claw (gears for open/close))
- C++ (Custom tasks for speed and steer control eliminating excess memory usage)
- FreeRTOS (Task managment)
- ESP-NOW (Sends data readings from controller to main esp)

# System Features
- Wireless communcation via ESP-NOW
- Real-time task scheduling using FreeRTOS
- Dual joystick arm control
- Openable/Closable claw driven by servo spinning custom formatted gears

# System Preview

<img src = "https://github.com/user-attachments/assets/5217a082-3014-4443-8a81-5766a3f5b5f2" width = "550" height = "550"/> 
<img src = "https://github.com/user-attachments/assets/54091036-61c1-4042-a63a-8d0bdc36c202" width = "550" height = "550"/> 

