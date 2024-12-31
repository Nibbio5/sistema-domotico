#include "..\include\device.h"
#include "..\include\singleton_id.h"
#include <string>
#include <cmath>
#include <random>
#include <iostream>

Device::Device(std::string name, double power) : id{SingeltonId::getInstance().generateId()}, name(name), power(power), isOn{false} {}

Device::~Device() {}