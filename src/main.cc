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
    time_t rawtime;
    struct tm * timeinfo;
    char buffer [80];

    time(&rawtime); /* get current time; same as: timer = time(NULL)  */
    timeinfo = localtime(&rawtime);

    strftime(buffer, 80, "Now it's %T.", timeinfo);
    puts(buffer);

    DomoticsSystem domotic;
    domotic.setDevices();
    std::cout << domotic.getDevices() << std::endl;
    return 0;
}


