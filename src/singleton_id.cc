// Eros Menin

#include "../include/singleton_id.h"
#include <iostream>
#include <string>

int SingeltonId::generateId() { return currentId++; }

SingeltonId::SingeltonId() : currentId(0) {}