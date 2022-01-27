# Automatic-Trimming

This code manages the controller of a platform used for automatic cutting of clinical tissues. It is composed basically of the main, in which the process is executed as a finite state machine, and three dedicated libraries. Into the libraries it can be found the following functionalities:

Managing of the communication between the controller and a FastApi server
Managing of serial communication with a vision system
Communication between the controller and a thingsboard interface, via MQTT
Control of the platform's hardware
Hardware: Arduino mkr Wifi 1010
IDE: Visual Studio Code

N.B. = The software development is still ongoing, due to the lack of part of the hardware. In particular, the control part is currently missing.
