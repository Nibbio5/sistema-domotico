#include "../include/manual_device.h"
#include "../include/cp_device.h"
#include "../include/device.h"
#include "../include/time.h"
#include <string>
#include <iostream>
// remove the following include, ONLIY FOR THEST PURPOSE
#include <time.h>
#include <stdio.h>
#include <string>
#include "../include/domotics_system.h"

/*
 *   TODO
 *   time gestion
 *   input gestion
 *   output gestion
 * DomoticsSystem::getDevices ()  to finish after @eros_m has done the device classes
 * REMEMBER to run the file: go to the biuld folder. run cmake --build . and ./sistema-domotico.exe
 */

/* DOCUMENTATION
*  
*/
int main()
{
int aaa(Device a) {return 1;}

int main() {

    ManualDevice md {"cacca", 12.3, Time().fromString("12:44"), Time().fromString("12:44")};
    std::cout << md << "\n\n";
    
    CPDevice cpd {"merda", 1.2, std::chrono::duration_cast<std::chrono::seconds>(md.getStartTime() - md.getStopTime())};
    std::cout << cpd << "\n\n";

    try {
        Time time = Time::fromString("12:30");
        std::cout << time.getHour() << "  " << time.getMinute() << std::endl;
        std::cout << time << std::endl;
    } catch(const std::invalid_argument &e) {
        std::cerr << e.what() << std::endl;
    }
    return 0;
}


