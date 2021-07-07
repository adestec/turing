#ifndef SENSOR_H
#define SENSOR_H

#include <iostream>
#include <string>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <Arduino.h>
#include "DHT.h"
#include <string>
#include <exception>
using namespace std;

class Sensor{
    
    public:

    Sensor();
    Sensor(int, float, int);
    Sensor(const Sensor &);
    virtual ~Sensor();

    int getpin() const;
    virtual float getdata();
    int gettype() const;
    int setpin(const int);
    int settype(const int);
    int setdata(const float);

    private:

    int pin;
    mutable float data;
    int type;

};

class thermalsensor : public Sensor{
    
    public:

    thermalsensor();
    thermalsensor(int, float, int);
    ~thermalsensor();

    void settempmesh(const int);
    int gettempmesh() const;
    virtual float getdata();

    private:

    void setwire(const int);
    OneWire getwire() const;
    void setdalasdata();
    DallasTemperature getdalasdata() const; 
    void dalasbegin() const;
    float gettempc();
    void deletewire();
    void deletedalas();

    OneWire* wire=NULL;
    DallasTemperature* dalasdata=NULL;
    int temp_mesh=0;
    float temperature_C;

};

class soilmoisturesensor : public Sensor{

    public:

    soilmoisturesensor();
    soilmoisturesensor(int,float,int);
    ~soilmoisturesensor();

    virtual float getdata();

    private:

    float moisture;

};

class dhttemp : public Sensor{

    public:

    dhttemp();
    dhttemp(int,float,int);
    ~dhttemp();

    virtual float getdata();
    
    private:

    void dhtbegin();
    void dhtset(int);
    float gettempandhumidity();
    void deletedhtdata();

    DHT *dhtdata;
    float temp;
    float humi;

};

#endif