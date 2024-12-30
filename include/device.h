#ifndef DEVICE_H
#define DEVICE_H

#include <string>

class Device{
    protected:
        int id;
        bool isOn;

        int getId();
    
    public: 
        const std::string name;
        const double power;

        Device(std::string, double);
        
        ~Device();

        void switchOn() {isOn = true;}

        void switchOff() {isOn = false;}
};

#endif

/*
Domande:
1. La gestione dell'id unico a chi la facciamo fare? Pk se la faccio nella classe non posso mettere
    l'id const.
*/