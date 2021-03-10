# Introduction
We live in a digital world where everyone can communicate with one another. All you have to do is to connect to the internet. To make it possible, you need to have a router in your home or have data on your phone.  However, some of the remote areas in the world do not have access to the internet. They want to send a message to the neighboring town, but couldn’t afford the data. 
This pandemic is a good reflection of how mesh networking is essential to society. For instance, a teacher might send a message to his/her pupils who are living in a distant site.  
I am inspired by the project disaster radio. Some of the concepts are based on that project. ESP’s are powered by power banks instead of a solar panel. Also, I did not use the LoRa chip since the delivery date did not meet my deadline.
In this prototype, I will show how to set-up a mesh network for the group chat application.  There is still a lot of improvement in the project since the range is short. But, by adding more nodes, the coverage will be wider.

# Network Design
The topology I used is a star topology. The center nodes are a bridge to another network. Clients who are connected to the same local area do not need a bridge. However, if the client is located in the other local area, it requires a bridge (station nodes). 
In this prototype, I used six ESP 8266 modules as the nodes. The three nodes are access points (AP), and the other three nodes are the station point. One station node is enough for the AP1 to be able to connect to AP2. If you want to extend the coverage, multiple nodes are required.
 The advantage of ESP (8266/32) is that it can act as station point, access point, and both station/access point. The nodes are connected in a mesh network using a Painless Mesh library.
