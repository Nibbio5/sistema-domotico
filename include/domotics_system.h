/**
 * @file domotics_system.h
 * @brief Header file for the DomoticsSystem class.
 *
 * This file contains the declaration of the DomoticsSystem class, which manages
 * a domotics system by controlling various devices. It includes methods for
 * managing device schedules, power load, and device statuses.
 *
 * @version 1.0
 * @date 2023-10-05
 *
 * @author Daniele Piccolo
 *
 * @note This file is part of the Domotics System project.
 */

#ifndef DOMOTICS_SYSTEM_H
#define DOMOTICS_SYSTEM_H

#include <functional>
#include <string>
#include <map>
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <memory>
#include <algorithm>
#include "device.h"
#include "time.h"
#include "manual_device.h"
#include "cp_device.h"
#include "log.h"

/**
 * @class DomoticsSystem
 * @brief A class that manages the domotics system by controlling the devices.
 *
 * The DomoticsSystem class provides methods to manage a collection of devices,
 * including scheduling, power load balancing, and status management.
 */
class DomoticsSystem {
  private:
    std::vector<Device*> all_devices; ///< All devices in the system.
    std::vector<Device*> active_devices; ///< Currently active devices.
    const double KPowerLimit; ///< Maximum power limit for the system.
    double powerLoad; ///< Current power load of the system.
    Time currentTime; ///< Current time in the system.
    report::logs log; ///< List of log messages.

    /**
     * @brief Checks the schedule of devices and updates their status.
     */
    void checkSchedule();

    void checkScheduleCpDevice(CPDevice *cpDevice, const Time *startTime, const Time *lastActivationTime, double &powerLoad);

    /**
     * @brief Sets the devices based on the current schedule.
     */
    void setDevices();

    /**
     * @brief Orders devices by their start time.
     */
    void orderByStartTime();

  public:
    /**
     * @brief Default constructor for DomoticsSystem.
     */
    DomoticsSystem();

    /**
     * @brief Constructor for DomoticsSystem with a specified power limit.
     * @param power The power limit for the system.
     */
    DomoticsSystem(double power);

    /**
     * @brief Gets the list of all devices.
     * @return A vector of pointers to all devices.
     */
    std::vector<Device*> getDevices() const;

    /**
     * @brief Gets the vector of all devices.
     * @return A vector of pointers to all devices.
     */
    std::vector<Device *> getDevicesVector() const;

    /**
     * @brief Displays the list of all devices.
     */
    void showDevices() const;

    /**
     * @brief Gets the current time in the system.
     * @return The current time.
     */
    Time getCurrentTime() const;

    /**
     * @brief Gets the index of a device.
     * @param device The name of the device.
     * @param prova A boolean flag.
     * @return The index of the device.
     */
    int getIndex(std::string device, bool prova) const;

    /**
     * @brief Checks if a device is present in the system.
     * @param device The name of the device.
     * @return True if the device is present, false otherwise.
     */
    bool isPresent(std::string device) const;

    /**
     * @brief Gets the current power load of the system.
     * @return The current power load.
     */
    double getPowerLoad() const;

    /**
     * @brief Sets the current time in the system.
     * @param newTime The new time to set.
     */
    void setCurrentTime(const Time& newTime);

    /**
     * @brief Sets the start and stop time for a device.
     * @param device The name of the device.
     * @param start The start time.
     * @param stop The stop time.
     */
    void setDeviceTime(const std::string& device, const Time& start, const Time& stop);

    /**
     * @brief Changes the status of a device.
     * @param status The new status of the device.
     * @param device The name of the device.
     */
    void changeDeviceStatus(bool status, std::string device);

    /**
     * @brief Removes the timer for a device.
     * @param device The name of the device.
     */
    void removeDeviceTimer(std::string device);

    /**
     * @brief Balances the power load in the system.
     * @param last The name of the last device.
     */
    void balancePower(std::string last);

    /**
     * @brief Displays the list of active devices.
     */
    void showActiveDevices() const;

    // DEBUG section

    /**
     * @brief Resets the current time in the system.
     */
    void resetTime();

    /**
     * @brief Resets the timers for all devices.
     */
    void resetTimers();

    /**
     * @brief Resets all settings in the system.
     */
    void resetAll();

    /**
     * @brief Overloads the << operator to output the system status.
     * @param out The output stream.
     * @param sys The DomoticsSystem object.
     * @return The output stream.
     */
    friend std::ostream &operator<<(std::ostream &out, const DomoticsSystem &sys);
};

void checkScheduleManualDevice(ManualDevice *manualDevice, Time currentTime, const Time *startTime, const Time *lastActivationTime, double &powerLoad, report::logs &logList);

#endif // #ifndef