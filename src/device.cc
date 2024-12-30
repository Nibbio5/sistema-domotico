#include "..\include\device.h"
#include <string>
#include <cmath>
#include <random>

Device::Device(std::string name, double power) : name(name), power(power) {
    id = Device::getId();
    isOn = false;
}

Device::~Device(){}

int Device::getId(){
    static std::random_device rd;                           // Generates a random hardware seed
    static std::mt19937 gen(rd());                          // Mersenne Twister generator
    std::uniform_real_distribution<int> dist(0, INT_MAX);   // Uniform distribution [1, 100]
    return dist(gen);
}