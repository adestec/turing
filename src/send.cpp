#include "send.h"
#include "sensor.h"

using namespace std;

#define WIFI_AP_NAME        "HackerSpace"
#define WIFI_PASSWORD       ""
#define TOKEN               "tokena"
#define THINGSBOARD_SERVER  "demo.thingsboard.io"

WiFiClient espClient;
ThingsBoard tb(espClient);

int status = WL_IDLE_STATUS;
bool subscribed = false;

class Sensor;

vector<Sensor*> sensors;

int sensorQuanity = 5;

float   availablePorts[6]  =   {0,  32,26,2,4,12},
        sensorTypesUsed[6] =   {0,  0,0,0,0,0},
        logsensor[6]       =   {0,  0,0,0,0,0},
        sensorData[6]      =   {0,  0,0,0,0,0};

#define COUNT_OF(x) ((sizeof(x)/sizeof(0[x])) / ((size_t)(!(sizeof(x) % sizeof(0[x])))))

void InitWiFi()
{
  Serial.println("Connecting to AP ...");
  // attempt to connect to WiFi network

  WiFi.begin(WIFI_AP_NAME, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected to AP");
}

void connect(){
  if (WiFi.status() != WL_CONNECTED) {
    reconnect();
    return;
  }
  // Reconnect to ThingsBoard, if needed
  if (!tb.connected()) {
    subscribed = false;

    // Connect to the ThingsBoard
    Serial.print("Connecting to: ");
    Serial.print(THINGSBOARD_SERVER);
    Serial.print(" with token ");
    Serial.println(TOKEN);
    if (!tb.connect(THINGSBOARD_SERVER, TOKEN)) {
      Serial.println("Failed to connect");
      return;
    }
  }
}

void reconnect() {
  // Loop until we're reconnected
  status = WiFi.status();
  if ( status != WL_CONNECTED) {
    WiFi.begin(WIFI_AP_NAME, WIFI_PASSWORD);
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }
    Serial.println("Connected to AP");
  }
}

void setvector(vector<Sensor*>& sensors){
    int type_input=0;
    
    //32.26.2.4.12
    //1-Soil moisture sensor
    //2-Soil thermal sensor
    //3- DHT sensor
    if(sensors.empty()==1){
        for(int i=1; i<=sensorQuanity; i++){
            type_input=sensorTypesUsed[i];

            if(type_input==0){
                try{
                    sensors.push_back(new Sensor(0,0,0));
                } catch (...){
                    Serial.println("Error in allocation of 'Sensor' in 'send.cpp->setvector'\n <Can not Create 'Sensor' in Vector 'Sensors'");
                }
            }
            if(type_input==1){
                try{
                    sensors.push_back(new soilmoisturesensor(availablePorts[i], 0, type_input));
                } catch (...){
                    Serial.println("Error in allocation of 'Soil Moisture Sensor' in 'send.cpp->setvector'\n <Can not Create 'Soil Moisture Sensor' in Vector 'Sensors'");
                }
            }
            if(type_input==2){
                //Serial.println("creating");
                try{
                    sensors.push_back(new thermalsensor(availablePorts[i], 0, type_input));
                } catch (...){
                    Serial.println("Error in allocation of 'Temperature Sensor' in 'send.cpp->setvector'\n <Can not Create 'Temperature Sensor' in Vector 'Sensors'");
                }
            }
            if(type_input==3){
                try{
                    sensors.push_back(new dhttemp(availablePorts[i], 0, type_input));
                } catch (...){
                    Serial.println("Error in allocation of 'DHT Sensor' in 'send.cpp->setvector'\n <Can not Create 'DHT Sensor' in Vector 'Sensors'");
                }
            }

            logsensor[i]=type_input;
            
        }
    }   
    
    else{
        for(int i=1; i<=sensorQuanity; i++){
            if((logsensor[i])!=(sensorTypesUsed[i])){
                type_input=sensorTypesUsed[i];

                if(type_input==0){
                    //sensors.erase((sensors.begin()+i)-1);
                    delete sensors[i-1];
                    try{
                        sensors[i-1]=new Sensor(0,0,0);
                    } catch (...){
                    Serial.println("Error in allocation of 'Sensor' in 'send.cpp->setvector'\n <Can not Create 'Sensor' in Vector 'Sensors'");
                    }
                }
                if(type_input==1){   
                    delete sensors[i-1];
                    try{
                        sensors[i-1]=new soilmoisturesensor(availablePorts[i], 0, type_input);
                    } catch (...){
                    Serial.println("Error in allocation of 'Soil Moisture Sensor' in 'send.cpp->setvector'\n <Can not Create 'Soil Moisture Sensor' in Vector 'Sensors'");
                    }
                }
                if(type_input==2){
                    delete sensors[i-1];
                    try{
                        sensors[i-1]=new thermalsensor(availablePorts[i], 0, type_input);
                    } catch (...){
                    Serial.println("Error in allocation of 'Temperature Sensor' in 'send.cpp->setvector'\n <Can not Create 'Temperature Sensor' in Vector 'Sensors'");
                    }
                }
                if(type_input==3){
                    delete sensors[i-1];
                    try{
                        sensors[i-1]=new dhttemp(availablePorts[i], 0, type_input);
                    } catch (...){
                    Serial.println("Error in allocation of 'DHT Sensor' in 'send.cpp->setvector'\n <Can not Create 'DHT Sensor' in Vector 'Sensors'");
                    }
                }

                logsensor[i]=type_input;
            }
        }
    }
    
}

void validate(vector<Sensor*>&sensors){
    if(sensorData[0]!=sensorTypesUsed[0]){
        Serial.println("\nChange in Vector of Sensors");
        setvector(sensors);
        Serial.println();
        sensorData[0]=sensorTypesUsed[0];
    }
    else{
        for(int i=1; i<=sensorQuanity;i++){
            if(sensors[i-1]->getpin()!=0){
                sensorData[i]=sensors[i-1]->getdata();
                //Serial.println(sensorData[i]);
                

            }
       }
    }

}

void send(){
    for(int i=1;i<sensorQuanity;i++){
        ///Readings and printing of sensors data
        ///1-  Soil Moisture
        ///2 - Soil Temperature
        ///3 - Air Humidity and Temperature
        if(sensorTypesUsed[i]==1){
            string message="Umidade do solo em porta ";
            ostringstream used_port;
            used_port<<i;
            message += used_port.str();
            const char * c = message.c_str();
            sensorData[i]=map(sensorData[i], 3700, 1700, 0, 100);
            if(sensorData[i]>100){
            sensorData[i]=100;
            }
            if(sensorData[i]<=0){
            sensorData[i]=0;
            }
            tb.sendTelemetryFloat(c, sensorData[i]);
            Serial.print(c);
            Serial.print(" = ");
            Serial.println(sensorData[i]);
        }

        if(sensorTypesUsed[i]==2){
            string message="Temperatura do solo em porta ";
            ostringstream used_port;
            used_port<<i;
            message += used_port.str();
            const char * c = message.c_str();
            tb.sendTelemetryFloat(c, sensorData[i]);
            Serial.print(c);
            Serial.print(" = ");
            Serial.println(sensorData[i]);
        }

        if(sensorTypesUsed[i]==3){
            string message="Umidade do ar em porta ";
            ostringstream used_port;
            used_port<<i;
            message += used_port.str();
            const char * ch = message.c_str();
            float temp_humidity=sensorData[i]/1000;
            tb.sendTelemetryFloat(ch, temp_humidity);
            Serial.print(ch);
            Serial.print(" = ");
            Serial.println(temp_humidity);

            message="Temperatura do ar em porta ";
            message += used_port.str();
            const char * ct = message.c_str();
            float temp_temperature=(float(int((sensorData[i]))%1000))/10;
            tb.sendTelemetryFloat(ct, temp_temperature);
            Serial.print(ct);
            Serial.print(" = ");
            Serial.println(temp_temperature);
        }
    }
    Serial.println();
}

