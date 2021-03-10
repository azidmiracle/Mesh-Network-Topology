# Introduction
We live in a digital world where everyone can communicate with one another. All you have to do is to connect to the internet. To make it possible, you need to have a router in your home or have data on your phone.  However, some of the remote areas in the world do not have access to the internet. They want to send a message to the neighboring town, but couldn’t afford the data. 
This pandemic is a good reflection of how mesh networking is essential to society. For instance, a teacher might send a message to his/her pupils who are living in a distant site.  
I am inspired by the project disaster radio. Some of the concepts are based on that project. ESP’s are powered by power banks instead of a solar panel. Also, I did not use the LoRa chip since the delivery date did not meet my deadline.
In this prototype, I will show how to set-up a mesh network for the group chat application.  There is still a lot of improvement in the project since the range is short. But, by adding more nodes, the coverage will be wider.

# Network Design
The topology I used is a star topology. The center nodes are a bridge to another network. Clients who are connected to the same local area do not need a bridge. However, if the client is located in the other local area, it requires a bridge (station nodes). 
In this prototype, I used six ESP 8266 modules as the nodes. The three nodes are access points (AP), and the other three nodes are the station point. One station node is enough for the AP1 to be able to connect to AP2. If you want to extend the coverage, multiple nodes are required.
 The advantage of ESP (8266/32) is that it can act as station point, access point, and both station/access point. The nodes are connected in a mesh network using a Painless Mesh library.

![Topology](/images/mesh_1.JPG)

The application is served by the access point modules. The web socket protocol is used so that the server and clients can communicate simultaneously.  The diagram below shows how the server is connected to the client device.

![Full-duplex](/images/fullduplex.JPG)

# Requirements
## Hardware

![Hardware](/images/hardware.JPG)

## Software
### IDE
![IDE](/images/ide.JPG)
### Libraries
1.	Painless Mesh - https://gitlab.com/painlessMesh/painlessMesh
2.	ESPAsyncWebServer - https://github.com/me-no-dev/ESPAsyncWebServer
3.	WebSocketsServer - https://github.com/Links2004/arduinoWebSockets
4.	Adafruit_GFX - https://github.com/adafruit/Adafruit-GFX-Library
5.	Adafruit_SS  - https://github.com/adafruit/Adafruit_SSD1306 

# Client-side (Software)
First, I will explain the codes for the Access Points. The OLED should be connected to the ESP 8266 AP first. A complete guide on how to connect to the ESP is this link https://randomnerdtutorials.com/esp8266-0-96-inch-oled-display-with-arduino-ide/.

### Interface
Make sure to save the html,js, and css inside the data folder.
![Interface](/images/interface1.JPG)

This is how the interface looks like. For instruction on how to use it, please refer to the user guide manual.

![Interface](/images/interface2.JPG)

### Database

* Database name: ChatDB
* Table name: history
* Attributes: id (auto-increment), name (string), myMessage (string) and dateSend (Date)

### System Flowchart
![system](/images/datadesign.JPG)

# Server-side (Firmware)
There two codes for the firmware. The first code basic is a standard code by painlessMesh library. I reused the code and upload to the three station nodes. The second code is the Web_Server wherein I do some of the hardcoding. This code is uploaded to the three access point nodes.

## System Flow Chart
Before we start the setup() and loop() code, we need to declare the libraries used and the variables.

![system](/images/systemflow1.JPG)
![system](/images/systemflow2.JPG)
![system](/images/system3.JPG)



