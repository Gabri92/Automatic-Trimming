/******************************************************//**
 * @file main.h
 * @author Gabriele Baldi (you@domain.com)
 * @brief 
 * 
  * Manage the controller main and the action derived 
 * from the comunication with othier interfaces, including 
 * IA software - Header file
 * 
 * @version 0.1
 * @date 2021-12-17
 **********************************************************/

#ifndef MAIN_H
#define MAIN_H

/**
 * Libraries
 */
#include <Arduino.h>
#include "FastAPI.h"
#include "Thingsboard.h"
#include "Serial.h"

/**
 * Variables declaration
 */
// Counter variables
extern bool count;
extern unsigned long counter;
extern bool startCount;
// Timing variables
extern unsigned long counterMillis;
extern unsigned long previousMillis;

/**
 * Functions declaration
 */
void selectAction();
void reconnect();

#endif