// Eros Menin

#include "../include/singleton_id.h"

int SingletonId::generateId() { return current_id_++; }

SingletonId::SingletonId() : current_id_(0) {}