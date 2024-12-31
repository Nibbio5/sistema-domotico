#ifndef DEVICE_H
#define DEVICE_H

#include <string>

class Device{
    public: 
        const int id;
        const std::string name;
        const double power;

        Device(std::string, double);
        
        ~Device();

        int isDeviceOn() const {return isOn;}

        void switchOn() {isOn = true;}

        void switchOff() {isOn = false;}
    
    protected:
        bool isOn;

        // Copy constructor and assignment operator disabled
        Device(const Device&) = delete;
        Device& operator=(const Device&) = delete;
};

#endif

/*
Domande:
1. La gestione dell'id unico a chi la facciamo fare? Pk se la faccio nella classe non posso mettere
    l'id const.
*/