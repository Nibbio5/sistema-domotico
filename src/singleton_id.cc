/**
 * @file singleton_id.cc
 * @author Eros Menin (eros.menin@studenti.unipd.it)
 * @brief Implementation of SingletonId class.
 * @date 2025-01-13
 * 
 */

#include "../include/singleton_id.h"

int SingletonId::generateId() { return current_id_++; }

SingletonId::SingletonId() : current_id_(0) {}