// Eros Menin

#include <iostream>
#include <string>

class SingeltonId {
    public:
        static SingeltonId& getInstance() {
            static SingeltonId instance;
            return instance;
        }

        int generateId() { return currentId++; }

    private:
        int currentId;

        // Private constructor to prevent direct instantiation
        SingeltonId() : currentId(0) {}

        // Copy constructor and assignment operator disabled
        SingeltonId(const SingeltonId&) = delete;
        SingeltonId& operator=(const SingeltonId&) = delete;
};