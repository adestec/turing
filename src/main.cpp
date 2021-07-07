#include "send.h"
#include "sensor.h"
using namespace std;

int testcounter=0;
void test();

void setup(){
    Serial.begin(115200);
    
    InitWiFi();
    ///Set the vector that contains the sensors 
    setvector(sensors);
}

void loop( ){

    delay(1500);
    test();
    connect();

    ///Verify any changes in port configuration and store the sensor data
    validate(sensors);
    ///Send sensor data to broker
    send();


    tb.loop();
}

void test(){
    testcounter+=1;
    if(testcounter==3){
        sensorTypesUsed[0]=sensorTypesUsed[0]+1;
        sensorTypesUsed[1]=1;
        sensorTypesUsed[2]=2;
        sensorTypesUsed[3]=3;
        sensorTypesUsed[4]=3;
    }

}











