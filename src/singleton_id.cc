// Eros Menin

#include "../include/singleton_id.h"
#include <iostream>
#include <string>

int SingletonId::generateId() { return currentId++; }

SingletonId::SingletonId() : currentId(0) {}