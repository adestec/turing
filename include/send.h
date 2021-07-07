#ifndef SEND_H
#define SEND_H

#include "ThingsBoard.h"
#include <WiFi.h>
#include <Wire.h>
#include <vector>
#include <sstream>
using namespace std;

class Sensor;

extern vector<Sensor*> sensors;

extern int sensorQuanity;

extern float sensorData[6], availablePorts[6], sensorTypesUsed[6], logsensor[6];

extern ThingsBoard tb;

void reconnect();

void InitWiFi();

void setvector(vector<Sensor*>& sensors);

void validate(vector<Sensor*>&sensors);

void send();

void connect();


#endif