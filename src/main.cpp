/******************************************************//**
 * @file main.cpp
 * @author Gabriele Baldi (gabriele.baldi.92@gmail.com)
 * @brief 
 * 
 * Manage the controller main and the action derived 
 * from the comunication with othier nterfaces, including 
 * IA software
 * 
 * @version 0.1
 * @date 2021-12-16
 **********************************************************/

#include "main.h"

void setup()
{
  Serial.begin(115200);
  while(!Serial){}
  Serial1.begin(9600);
  Serial1.setTimeout(10000);
  initEthernet();
  initTBClient();
}


void loop()
{
  // If the connection goes down try to reconnect
  if(Ethernet.linkStatus() != 1 || !mqtt_client.connected())
  {
    reconnect();
  }
  mqtt_client.loop();

  selectAction();
}

/**
 * Other Functions
 */
/**
 * @brief 
 * Select the action to be made, based on the 
 * incoming data from the serial communication
 */
void selectAction()
{
  // Copy inputJson into outputJson and decides which action has to be taken
  if(inputJson.isNull())
  {
    return;
  }
  else if(inputJson.size() < 2)
  {
    Serial.println();
    Serial.println("ERROR");
    return;
  }
  String message;
  String key = String((const char*)inputJson["key"]);
  String value = String((const char*)inputJson["value"]);
  outputJson["key"] = inputJson["key"];
  outputJson["value"] = inputJson["value"];
  // Actions to be done before the ON input from Thingsboard
  if(key.equals("IASoftware") && flag_start == false)
  {
    if(value.equals("ON"))
    {
      // Controllo componenti ed invio messaggio
      Serial.println("\nComponents OK");
    }
  }
  // Actions of the overall process(After that the ON button has been pressed)
  // IA Software messages
  else if(key.equals("IASoftware") && flag_start == true)
  {
    if(value.equals("READY"))
    {
      // Microtome activation //
      // Vacuum activation //
      outputJson["key"] = "ATRMController";
      outputJson["code"] = "12003";
      outputJson["detail"] = "Vacuum Cleaner ON";
      httpRequest(outputJson);
      delay(500);
      // Send serial message to Jetson//
    }
    else if(value.equals("STOP"))
    {
      // Microtome de-activation //
      delay(1000);
      // Vacuum de-activation //
      outputJson["key"] = "ATRMController";
      outputJson["code"] = "12004";
      outputJson["detail"] = "Vacuum cleaner OFF";
      httpRequest(outputJson);
      delay(500);
      outputJson["key"] = "ATRMController";
      outputJson["code"] = "12005";
      outputJson["detail"] = "Trimming stopper";
      httpRequest(outputJson);
      delay(500);
      // Sending holder to home position //
      Serial.println("Holder in home position");
      Serial.println("Waiting for block removal...");
      // Send stop input to Thingsboard //
      mqtt_client.publish("v1/devices/me/attributes", "{\"param\":\"false\"}");
      Serial.println("Block removed");
    }
    else if(value.equals("WARNING")){}
    else if(value.equals("ERROR")){}
    // Make sure that the buffer is empty
    while(Serial1.available()){Serial1.read();}
  }

  else if(key.equals("ATRMController") && flag_start == true)
  {
    if(value.equals("START"))
    {
      Serial.println("Turning Lights ON"); 
      delay(1000);
      Serial.println("Light Intensity OK");
      delay(1000);      
      Serial.println("Holder in Home position");
      delay(1000);
      Serial.println("Waiting for block insertion...");
      delay(1000);
      ///////////////////////////////////////////////////////////////////////////////////
      // INPECO Interface waiting cycle
      //unsigned long time = 0;
      //unsigned long time_now = 0;
      //time = millis();
      // while(!blockArrived){ time_now = millis(); if(time_now-time >= 10000){WARNING}}
      // if(digitalRead == OK) 
      // Controllo sensori
      // IF ERROR
      //Gestire errore {gestione errore; return;}
      ///////////////////////////////////////////////////////////////////////////////////      
      Serial.println("Block inserted");
      delay(1000);
      Serial.println("Moving holder towards the blade...");
      delay(1000);
      Serial.println("Holder in position");
      delay(1000);
      ////////////////////////////////////
      // Controllo sensori da inserire
      ///////////////////////////////////
      outputJson["key"] = "ATRMController";
      outputJson["value"] = "CheckID";
      outputJson["blockCode"] = inputJson["blockCode"];
      serializeJson(outputJson,message);
      outgoingSerial(message);
      delay(500);
    }
    else if(value.equals("WARNING")){}
    else if(value.equals("ERROR")){}    
  }
  inputJson.clear();
  outputJson.clear(); 
}

/**
 * @brief 
 * Tries to reconnect to Ethernet and Thingsboard
 */
void reconnect() 
{
  Serial.println("Not connected");
  // Loop until we're reconnected, both to Ethernet and then Thingsboard server
  while(!mqtt_client.connected()) 
  {
    if(!Ethernet.linkStatus())
    {
      Serial.println("Connecting to Ethernet");
      initEthernet();
    }
      initTBClient();
  }
}
