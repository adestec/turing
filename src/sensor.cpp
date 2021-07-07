#include "sensor.h"
using namespace std;

Sensor::Sensor(){
    pin = 0;
    data = 0;
    type = 0;
    Serial.println("Creating Base Sensor");

}

Sensor::Sensor(int pin, float data, int type){
    this->pin = pin;
    this->data = data;
    this->type = type;
    Serial.println("Creating Base Sensor");
    
}

Sensor::Sensor(const Sensor &s){
        this->pin=s.pin;
        this->data=s.data;
        this->type=s.type;
        Serial.println("Creating Base Sensor");
}

Sensor::~Sensor(){
    Serial.println("Destructing base sensor");

}

int Sensor::getpin() const{
    return(pin);
}

float Sensor::getdata(){
    return(data);
}

int Sensor::gettype() const{
    return(type);
}

int Sensor::setpin(const int pin){
    this->pin=pin;
    
   return(1);
}

int Sensor::setdata(const float data){
    this->data=data;
 
   return(1);
}

int Sensor::settype(const int type){
    this->type=type;
    
   return(1);
}


thermalsensor::thermalsensor() : Sensor(){
    Serial.println("Creating Temperature Sensor");
}

thermalsensor::thermalsensor(int pin, float data, int type) : Sensor(pin,data,type){
    Serial.println("Creating Temperature Sensor");
    try{
        setwire(pin);
    } catch (const char* msg){
        Serial.println(msg);
        bad_alloc ba_exception;
        throw ba_exception;
    }
    try{
        setdalasdata();
    } catch (const char* msg){
        Serial.println(msg);
        bad_alloc ba_exception;
        throw ba_exception;
    }
    try{
        dalasbegin();
    } catch (const char* msg){
        Serial.println(msg);
        bad_alloc ba_exception;
        throw ba_exception;
    }
}

thermalsensor::~thermalsensor(){
    Serial.println("Destructing Temperature Sensor");

    deletewire();
    deletedalas();
}

void thermalsensor::settempmesh(int temp_mesh){
    if (temp_mesh!=0 && temp_mesh!=1){
        throw "Error in declaration of 'Temperature Sensor Mesh' in 'sensors.cpp->thermalsensor::settempmesh'\n <Invalid Mesh Value>";
    }
    else {
        this->temp_mesh=temp_mesh;
    }
}

int thermalsensor::gettempmesh() const{
    return temp_mesh;
}

void thermalsensor::setwire(const int input_pin){
    if(input_pin<0){
        throw "Error in declaration of 'Temperature Sensor' in 'sensors.cpp->thermalsensor::setwire'\n <Invalid Pin Value>";
    }
    try{
        OneWire *tempwire = new OneWire(input_pin);
        wire=tempwire;
    }
    catch (bad_alloc &ba){
        Serial.println("Error in allocation of 'Temperature Sensor' in 'sensors.cpp->thermalsensor::setwire'");
        Serial.print("<");
        Serial.print(ba.what());
        Serial.println(">");
    }
    
    Serial.println("Thermalwire Set");
}

void thermalsensor::deletedalas(){
    delete dalasdata;
}

void thermalsensor::deletewire(){
    delete wire;
}

OneWire thermalsensor::getwire() const{
    return (*wire);
}

void thermalsensor::setdalasdata(){
    if(wire==NULL){
        throw "Error in declaration of 'Temperature Sensor' in 'sensors.cpp->thermalsensor::setdalasdata'\n <Invalid Pointer WIRE Value>";
    }
    try{
        DallasTemperature *tempdalasdata = new DallasTemperature(wire);
        dalasdata=tempdalasdata;
    }
    catch (bad_alloc &ba){
        Serial.println("Error in allocation of 'Temperature Sensor' in 'sensors.cpp->thermalsensor::setdalasdata'");
        Serial.print("<");
        Serial.print(ba.what());
        Serial.println(">");
    }
}

DallasTemperature thermalsensor::getdalasdata() const{
    return(*dalasdata);
}

void thermalsensor::dalasbegin() const{
    
    if(dalasdata==NULL){
        throw "Error in declaration of 'Temperature Sensor' in 'sensors.cpp->thermalsensor::dalasbegin'\n <Invalid Pointer DALASDATA Value> <Can not Begin>";
    }
    else{
        dalasdata->begin();
        Serial.println("Temperature Sensor Started");
    }
}

float thermalsensor::gettempc(){
    if(dalasdata==NULL){
        throw "Error in validation of 'Temperature Sensor' in 'sensors.cpp->thermalsensor::gettempc'\n <Invalid Pointer DALASDATA Value> <Can not Measure Data>";
        return(0);
    }
    
    dalasdata->requestTemperatures();
    this->temperature_C = dalasdata->getTempCByIndex(0);
    setdata(this->temperature_C);
    return(this->temperature_C);
}

float thermalsensor::getdata(){
    float a=0;
    try{
        a=gettempc();
    } catch (const char* msg){
        Serial.println(msg);
    }
    return(a);
}


soilmoisturesensor::soilmoisturesensor() : Sensor(){
    Serial.println("Creating Soil Moisture Sensor");
}

soilmoisturesensor::soilmoisturesensor(int pin, float data, int type) : Sensor(pin,data,type){
    Serial.println("Creating Soil Moisture Sensor");
}

soilmoisturesensor::~soilmoisturesensor(){
    Serial.println("Destructing Soil Moisture Sensor");

}

float soilmoisturesensor::getdata(){
    if(getpin()<=0){
        return(0);
    }
    moisture = analogRead(getpin());
    //Serial.println(moisture);
    //moisture=map(moisture, 3700, 1700, 0, 100);
    setdata(moisture);
    /*if(moisture>=100){
      //moisture=100;
    }*/
    return(moisture);
}


dhttemp::dhttemp () : Sensor() {
    Serial.println("Creating DHT Sensor");
}

dhttemp::dhttemp(int pin, float data, int type) : Sensor(pin,data,type){
    
    Serial.println("Creating DHT Sensor");
    try{
        dhtset(getpin());
    } catch (const char* msg){
        Serial.println(msg);
        bad_alloc ba_exception;
        throw ba_exception;
    }
    try{
        dhtbegin();
    } catch (const char* msg){
        Serial.println(msg);
        bad_alloc ba_exception;
        throw ba_exception;
    }

}

dhttemp::~dhttemp(){
    Serial.println("Destructing DHT Sensor");
    deletedhtdata();

}

void dhttemp::dhtset(int pin){
    if (pin<0){
        throw "Error in declaration of 'DHT Sensor' in 'sensors.cpp->dhttemp::dhtset'\n <Invalid Pin Value>";
    }
    try{
        DHT *dhtnode = new DHT(pin,DHT11);
        dhtdata=dhtnode;
    } catch (bad_alloc &ba) {
        Serial.println("Error in allocation of 'DHT Sensor' in 'sensors.cpp->dhttemp::dhtset'");
        Serial.print("<");
        Serial.print(ba.what());
        Serial.println(">");
    }

}

void dhttemp::dhtbegin(){
    if(dhtdata==NULL){
        throw "Error in declaration of 'DHT Senosr' in 'sensors.cpp->dhttemp::dhtbegin'\n <Invalid Pointer DHTDATA Value> <Can not Begin>";
    }
    dhtdata->begin();
}

float dhttemp::getdata(){
    float tempdata=0;
    try{
        tempdata=gettempandhumidity();
    } catch (const char* msg){
        Serial.println(msg);
    }
    return(tempdata);
}

void dhttemp::deletedhtdata(){
    delete dhtdata;
}

float dhttemp::gettempandhumidity(){
    if(dhtdata==NULL){
        throw "Error in validation of 'DHT Sensor' in 'sensors.cpp->dhttemp::getdata'\n <Invalid Pointer DHTDATA Value> <Can not Measure Data>";
        return(0);
    }
    float t=dhtdata->readTemperature();
    float h=dhtdata->readHumidity();
    if (isnan(t) || isnan(h)) {
        Serial.println("Failed to read from DHT sensor!");
        h=1;
        t=1;
    }
    if(h<=0){
        h=1;
    }
    if(h>=100){
        h=99;
    }
    if(t<=0){
        t=1;
    }
    if(t>=100){
        t=99;
    }
    setdata(h*1000+t*10);
    return(h*1000+t*10);
}

