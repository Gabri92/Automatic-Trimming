/******************************************************//**
 * @file FastAPI.cpp
 * @author Gabriele Baldi (you@domain.com)
 * @brief 
 * 
 * Manage the Ethernet connection and the commmunication
 * with the FastAPI server
 * 
 * @version 0.1
 * @date 2021-12-17
**********************************************************/

/**
 * Header
 */
#include "FastAPI.h"

/**
 * Variables definition
 */
// Ethernet client
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
EthernetClient client;
// Server's credentials
int HTTP_PORT = 8092;
IPAddress HOST_NAME(192,168,1,178);
// Method
String HTTP_METHOD = "GET";
String PATH_NAME = "/msgs_det/";
String ACCESS_TOKEN = "ATRM1010";

/**
 * @brief 
 * Connect the client to the network
 */
void initEthernet()
{
    Ethernet.begin(mac);
    Serial.println("Connecting to Ethernet");
    while (!Ethernet.linkStatus()) {
      delay(500);
      Serial.print(".");
    }
    Serial.println("Connected to Ethernet network");    
}

/**
 * @brief 
 * Manage GET requests to FastAPI Server
 * @param message // Query string
 */
void httpRequest(String message)
{
  String queryString = "?ACCESS_TOKEN=" + ACCESS_TOKEN + "&msg_data=" + message; 
  // Connect to the server and sent the query string within a GET request
  if(client.connect(HOST_NAME, HTTP_PORT))
  {
    Serial.println("\nConnected to the server");
    // Sending HTTP Request
    client.println(HTTP_METHOD + " " + PATH_NAME + queryString + " HTTP/1.1");
    client.println("Host: " + String(HOST_NAME));;
    client.println("Connection: close");
    client.println(); // end HTTP header
    while(client.connected())
    {
      if(client.available())
      {
        char c = client.read();
        Serial.print(c);
      }
    }
    // The server's disconnected, stop the client
    client.stop();
    Serial.println();
    Serial.println("Disconnected");
  } 
  else 
  {
    Serial.println("Connection failed");
  }      
  delay(2000);
}
