#include <string>
#include "time.h"

static void log(const Time &currentTime, const std::string &message){
    std::cout << "[" << currentTime << "]" << message << std::endl;
}