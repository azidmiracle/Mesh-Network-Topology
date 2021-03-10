/*
 This project is a webserver hosted by the esp8266 which act both sration and access point.
Then thes AP is connected to the mesh network whateverYouLike.
In order for the client to access the server, it should connect to the wifi network of the AP.
The IP address will be displayed in the OLED.
Type the IP address to the browser.

The client aide (HTML,Javascript and CSS) is saved in a "data" folder.
To upload it in the ESP8266, click Tools -> ESP8266 Sketch Data Upload.
 
*/
#include "painlessMesh.h"

#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiClient.h>

#include <FS.h>   // Include the SPIFFS library
#include <SPI.h> 
#include <ESPAsyncWebServer.h>
#include <WebSocketsServer.h>

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define   MESH_PREFIX     "whateverYouLike"
#define   MESH_PASSWORD   "somethingSneaky"
#define   MESH_PORT       5555

#ifndef STASSID
#define STASSID "AP8266_3"
#define STAPSK  "12345678"
#endif
#define   STATION_PORT     5555

const char* ssid = STASSID;
const char* password = STAPSK;


const int http_port = 80;
const int ws_port = 1337;

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels



// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// TCP server at port 81 will respond to HTTP requests

// Globals
AsyncWebServer server(http_port);
WebSocketsServer webSocket = WebSocketsServer(ws_port);

/*--FUNCTIONS FOR PAINLESS MESH*/
painlessMesh  mesh;
void InitializeMesh(){  
//mesh.setDebugMsgTypes( ERROR | MESH_STATUS | CONNECTION | SYNC | COMMUNICATION | GENERAL | MSG_TYPES | REMOTE ); // all types on
  mesh.setDebugMsgTypes( ERROR | STARTUP );  // set before init() so that you can see startup messages

  mesh.init( MESH_PREFIX, MESH_PASSWORD, MESH_PORT );
  mesh.onReceive(&receivedCallback);
  mesh.onNewConnection(&newConnectionCallback);
  mesh.onChangedConnections(&changedConnectionCallback);
  mesh.onNodeTimeAdjusted(&nodeTimeAdjustedCallback);
  }
// User stub
void sendMessage() ; // Prototype so PlatformIO doesn't complain

void sendMessage_from_Client(String payload) {
  //String msg = "Hello from node root";
  //msg += mesh.getNodeId();
  mesh.sendBroadcast( payload );
  //taskSendMessage.setInterval( random( TASK_SECOND * 1, TASK_SECOND * 5 ));
}

void receivedCallback( uint32_t from, String &msg ) {
  //Serial.printf("startHere: Received from %u msg=%s\n", from, msg.c_str());
  //create websocket event
  //webSocket.begin();//i adde this line
  webSocket.broadcastTXT(msg.c_str()); //when the message is receive broadcast to other users connected to it
}

void newConnectionCallback(uint32_t nodeId) {
    //Serial.printf("--> startHere: New Connection, nodeId = %u\n", nodeId);
}

void changedConnectionCallback() {
  //Serial.printf("Changed connections\n");
  //when the connection change, open the websocket
  // Start WebSocket server and assign callback
  //webSocket.begin();
 // webSocket.onEvent(onWebSocketEvent);
}

void nodeTimeAdjustedCallback(int32_t offset) {
    //Serial.printf("Adjusted time %u. Offset = %d\n", mesh.getNodeTime(),offset);

    //add this everytime the connection closed
    webSocket.loop();
}

/*END--FUNCTIONS FOR PAINLESS MESH*/


/*--FUNCTIONS FOR WEBSOCKET*/
// Callback: receiving any WebSocket message
void onWebSocketEvent(uint8_t client_num,
                      WStype_t type,
                      uint8_t * payload,
                      size_t length) {

  // Figure out the type of WebSocket event
  switch(type) {

     //Client has disconnected
    case WStype_DISCONNECTED:
      //Serial.printf("[%u] Disconnected!\n", client_num);          
      break;

    // New client has connected
    case WStype_CONNECTED:
      {
        IPAddress ip = webSocket.remoteIP(client_num);
        //Serial.printf("[%u] Connection from ", client_num);
        //Serial.println(ip.toString());
    
      }
      break;

    // Handle text messages from client
    case WStype_TEXT:

      // Print out raw message
      //Serial.printf("[%u] Received text: %s\n", client_num, payload);

      //SEND THIS DATA
     // send data to all connected clients in a specific IP
      webSocket.broadcastTXT(payload, length);
      
      //send to other nodes server connected in the mesh network   
      sendMessage_from_Client((char*)payload);
      
      break;

    // For everything else: do nothing
    case WStype_BIN:
    case WStype_ERROR:
    case WStype_FRAGMENT_TEXT_START:
    case WStype_FRAGMENT_BIN_START:
    case WStype_FRAGMENT:
    case WStype_FRAGMENT_FIN:
    default:
      break;
  }
}

// Callback: send homepage
void onIndexRequest(AsyncWebServerRequest *request) {
  IPAddress remote_ip = request->client()->remoteIP();
  //Serial.println("[" + remote_ip.toString() +
                 // "] HTTP GET request of " + request->url());
  request->send(SPIFFS, "/index.html", "text/html");
}

// Callback: send style sheet
void onCSSRequest(AsyncWebServerRequest *request) {
  IPAddress remote_ip = request->client()->remoteIP();
  Serial.println("[" + remote_ip.toString() +
                  "] HTTP GET request of " + request->url());
  request->send(SPIFFS, "/style.css", "text/css");
}

// Callback: send 404 if requested file does not exist
void onPageNotFound(AsyncWebServerRequest *request) {
  IPAddress remote_ip = request->client()->remoteIP();
  //Serial.println("[" + remote_ip.toString() +
                 // "] HTTP GET request of " + request->url());
  request->send(404, "text/plain", "Not found");
}

void connectToServer(){
   // On HTTP request for root, provide index.html file
  server.on("/", HTTP_GET, onIndexRequest);

  // On HTTP request for style sheet, provide style.css
  server.on("/style.css", HTTP_GET, onCSSRequest);

  // Handle requests for pages that do not exist
  server.onNotFound(onPageNotFound);

  // Start web server
  server.begin();
  }

void connectToWebsocket(){
    // Start WebSocket server and assign callback
  webSocket.begin();
  webSocket.onEvent(onWebSocketEvent);
  
  }  

void initializeMDNS(){
   // Set up mDNS responder:
  // - first argument is the domain name, in this example
  //   the fully-qualified domain name is "esp8266.local"
  // - second argument is the IP address to advertise
  //   we send our IP address on the WiFi network
  if (!MDNS.begin("esp8266")) {
    //Serial.println("Error setting up MDNS responder!");
    while (1) {
      delay(1000);
    }
  }
  //Serial.println("mDNS responder started");
  // Start TCP (HTTP) server
  //Serial.println("TCP server started");

  // Add service to MDNS-SD
  MDNS.addService("http", "tcp", http_port);
  
  }


/*END EHERE--FUNCTIONS FOR WEBSOCKET*/

/*START: FUNCTION FOR THE OLED*/
void InitializeToLED(){
 if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    //Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  delay(2000);
  display.clearDisplay();
  display.setTextSize(1.8);
  display.setTextColor(WHITE);
  display.setCursor(0, 10);
  
  }

void displayString(String str){
  display.println(str);
  display.display(); 
}
/*END HERE --START: FUNCTION FOR THE OLED*/


/*FUNCTIONS FOR THE SPIFFS*/

void InitializeSPIFFS(){

    // Make sure we can read the file system
  if( !SPIFFS.begin()){
    //Serial.println("Error mounting SPIFFS");
    while(1);
  }
   SPI.begin();
  
  }
/*END--FUNCTIONS FOR THE SPIFFS*/

/*Functions for connecting to wfifi */

void InitializeWIFI(){
  // Connect to Wi-Fi network with SSID and password
  //Serial.print("Setting AP (Access Point)…");
  // Remove the password parameter, if you want the AP (Access Point) to be open
  WiFi.softAP(ssid, password);

  IPAddress IP = WiFi.softAPIP();
  //Serial.print("AP IP address: ");
  displayString("IP address: ");
  display.println(IP);

  displayString("Connect to ");
  displayString(ssid);
  
  //display.println(WiFi.localIP());
  //display.display(); 
  }

/*START OF THE MAIN PROGRAM*/
void setup(void) {
  Serial.begin(115200);

  InitializeToLED();
  InitializeSPIFFS();
  InitializeMesh();
  InitializeWIFI();
  connectToServer();
  connectToWebsocket();
  initializeMDNS();
 
}

void loop(void) {
  mesh.update();
  webSocket.loop();
}
