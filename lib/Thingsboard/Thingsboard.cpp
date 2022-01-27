/******************************************************//**
 * @file Thingsboard.cpp
 * @author Gabriele Baldi (you@domain.com)
 * @brief 
 * 
 * Manage the initialization of the Thingsboard client
 * and the messaging with the interface(Start button)
 * 
 * @version 0.1
 * @date 2021-12-17
 **********************************************************/

/**
 * Header
 */
#include "Thingsboard.h"

/**
 * Variables definition
 */
bool flag_start = false;
// Broker credentials (Thingsboard)
IPAddress MQTT_BROKER(192,168,1,178);
PubSubClient mqtt_client(client);

/**
 * @brief 
 * Initialize thingsboard client and subscripe to necessary topics
 */
void initTBClient()
{
  mqtt_client.setServer(MQTT_BROKER,1883);
  mqtt_client.setCallback(rpcCallback);
  Serial.print("Connecting to ThingsBoard node ...");
  // Attempt to connect (clientId, username, password)
  if (mqtt_client.connect("ATRM controller","ATRM1010", NULL) ) 
  {
    Serial.println( "[DONE]" );
    // Subscribing to receive RPC requests
    mqtt_client.subscribe("v1/devices/me/rpc/request/+");
    mqtt_client.subscribe("v1/devices/me/rpc/response/+");
  } 
  else 
  {
    Serial.print( "[FAILED] [ rc = " );
    Serial.print( mqtt_client.state() );
    Serial.println( " : retrying in 5 seconds]" );
    // Wait 5 seconds before retrying
    delay(5000);
  }
}

/**
 * @brief 
 * Manage the start of the process by pressing
 * the relative interface button
 * @param topic // topic in which the client is listening for start
 * @param payload // content of the message to be listened in the topic
 * @param length // length of the payload
 */
void rpcCallback(char* topic, byte* payload, unsigned int length)
{
  // Receiving the payload and copying it into a json object
  char json[length + 1];
  strncpy (json, (char*)payload, length);
  json[length] = '\0';
  StaticJsonDocument<200> data;
  deserializeJson(data,json);
  ////////////////////////////////
  Serial.println("Topic: ");
  Serial.println(topic);
  Serial.println("Message: ");
  Serial.println(json);
  ////////////////////////////////
  // Reading the 'method' field of the json and, consequently, choosing an action
  String methodName = String((const char*)data["method"]);
  if (methodName.equals("setValue")) 
  {
    bool param = data["params"];
    if (param)
    {
      flag_start = true;
      delay(500);
      Serial.println("Debug1");
      inputJson["key"] = "ATRMController";
      inputJson["value"] = "START";
      inputJson["blockCode"] = String(random(10000,20000));
      delay(1000);
    }
    else if(!param)
    {
      Serial.println("Debug2");
      flag_start = false;
    }
    else 
    {
      Serial.println("Error sending ON/OFF command!");
    }
  }
  return;
}