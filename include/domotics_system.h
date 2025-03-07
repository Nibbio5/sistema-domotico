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
    std::vector<Device*> switchedDevices; ///< Devices that have been switched on manually.
    const double KPowerLimit; ///< Maximum power limit for the system.
    double powerLoad; ///< Current power load of the system.
    Time currentTime; ///< Current time in the system.
    report::logs log; ///< List of log messages.

    /**
     * @brief Checks the schedule of devices and updates their status.
     */
    void checkSchedule();

    /**
     * @brief Checks the schedule of a CPDevice.
     * @param cpDevice The CPDevice to check.
     * @param nowTime The current time.
     * @param startTime The start time of the device.
     * @return True if the device is scheduled to be on, false otherwise.
     */
    bool checkScheduleCpDevice(CPDevice *cpDevice, const Time nowTime, const Time *startTime);

    /**
     * @brief Checks the schedule of a ManualDevice.
     * @param manualDevice The ManualDevice to check.
     * @param nowTime The current time.
     * @param startTime The start time of the device.
     * @param stopTime The stop time of the device.
     * @return True if the device is scheduled to be on, false otherwise.
     */
    bool checkScheduleManualDevice(ManualDevice *manualDevice,const Time nowTime, const Time *startTime, const Time *stopTime);

    /**
     * @brief Sets the devices based on a json file parsing
     * @note The file must be in the assets folder and named devices.json
     */
    void setDevices();

    /**
     * @brief Orders devices by their start time.
     */
    void orderByStartTime();

    /**
     * @brief Ends the day by turning off all devices.
     */
    void endDay();

  public:
    /**
     * @brief Default constructor for DomoticsSystem.
     */
    DomoticsSystem();

    /**
     * @brief Destructor for DomoticsSystem.
     */
    ~DomoticsSystem();

    /**
     * @brief Constructor for DomoticsSystem with a specified power limit.
     * @param power The power limit for the system.
     */
    DomoticsSystem(const double& power);

    /**
     * @brief Gets the list of all devices.
     * @return A vector of pointers to all devices.
     */
    std::vector<Device*> getDevices() const;

    /**
     * @brief Displays the list of all devices.
     */
    void showDevices() const;

    /**
     * @brief Gets the current time in the system.
     * @return The current time.
     */
    const Time& getCurrentTime() const;

    /**
     * @brief Gets the index of a device.
     * @param device The name of the device.
     * @param isActive A boolean flag.
     * @return The index of the device.
     */
    int getIndex(const std::string& device, bool isActive) const;

    /**
     * @brief Gets the current power load of the system.
     * @return The current power load.
     */
    const double& getPowerLoad() const;

    /**
     * @brief Sets the current time in the system.
     * @param newTime The new time to set.
     */
    void setCurrentTime(const Time& newTime);

    /**
     * @brief Sets the start for a device, if the device is a manual device it also sets stop time.
     * @param device The name of the device.
     * @param start The start time.
     * @param stop The stop time.
     */
    void setDeviceTime(const std::string& device, const Time& start, const Time& stop);

    /**
     * @brief Changes the status of a device by turning it on or off.
     * @param status The new status of the device.
     * @param device The name of the device.
     */
    void changeDeviceStatus(bool status,const std::string& device);

    /**
     * @brief Removes the timer for a device.
     * @param device The name of the device.
     */
    void removeDeviceTimer(std::string device);

    /**
     * @brief Balances the power load in the system, by turning off the needed devices to respect the power limit.
     * the devices are turned off in the inverse order of activation.
     * @param last The name of the last device.
     */
    void balancePower(const std::string& last, const Time &nowTime);

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

#endif // #ifndef