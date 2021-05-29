noise_detecor: Connected Noise Detector
======

Noise Detector is a simple device for lowering noise levels in the office or public places. It monitors noise level and alarms when it gets too loud. It is also connected to cloud for providing noise data for further analytics
It is easy to install and use in every environment.


### Hardware

Noise Detector is based on a MEMS microphone that measures intensity of noise and ESP32 microcontroller that monitors the noise level and sends measured results to cloud.
The user can adjust a threshold for the detection. The Noise Detector can indicate alarms if it is too loud using onboard buzzer and LED.

### Firmware

The firmware consists of a RTOS and applications for measuring and analysing of noise level as well as sending information to cloud. It connects to a cloud and provides noise data over MQTT.

### Usage

No software installation is required. Just turn on and connect to your WiFi Network. Immediately after connecting, the device starts to send the measured noise level 
to a cloud service enabling analytics for your working environment, even without invasive notifications.


---
