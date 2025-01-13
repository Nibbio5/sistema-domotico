/**
 * @file terminal.h
 * @author Francesco Dotoli (francesco.dotoli@studenti.unipd.it)
 * @brief
 * @date 2024-12-27
 *
 * @copyright Copyright (c) 2025
 *
 */

#ifndef TERMINAL_H
#define TERMINAL_H

#include <string>
#include <vector>
#include <cmath>
#include "domotics_system.h"
#include "device.h"


class Terminal {
  private:
    DomoticsSystem domotics_system_; ///< The domotics system.
    report::logs log_; ///< The log system.

    /**
     * @brief Sets the current time in the system.
     * @param arg The new time to set.
     */
    void setTimeCommandPrompt(const std::string &arg);

    /**
     * @brief Checks if a device is present in the system.
     * @param arg The name of the device.
     * @return The device if present, nullptr otherwise.
     */
    Device *isDevice(const std::string &device_name);

    /**
     * @brief Sets Device command prompt based on the arguments.
     * @param args The arguments of the command.
     */
    void setDeviceCommandPrompt(const std::vector<std::string> &args, Device *device);

    /**
     * @brief Shows the status of a single device.
     * @param device The device to show.
     * @param show_time The flag to show the time at the start.
     * @return The string of the device status.
     */
    std::string showOneDevice(const Device *device, const bool &isSigle = true);

    /**
     * @brief Sets command prompt based on the arguments.
     * @param args The arguments of the command.
     */
    void setCommandPrompt(const std::vector<std::string> &args);

    /**
     * @brief Removes the timer of a device.
     * @param arg The name of the device.
     */
    void rmCommandPrompt(const std::string &arg);

    /**
     * @brief Shows the status of system devices or one device ({showOneDevice}).
     * @param arg The arguments of the command.
     */
    void showCommandPrompt(const std::string &arg);

    /**
     * @brief Resets command prompt based on the arguments.
     * @param arg The arguments of the command.
     */
    void resetCommandPrompt(const std::string &arg);

    /**
     * @brief Sets the timer of a device.
     * @param device_name The name of the device.
     * @param start The start time.
     * @param stop The stop time.
     */
    void setDeviceTimer(const std::string &device_name, const std::string &start, const std::string &stop);

    /**
     * @brief Shows the help message.
     */
    void helpCommandPrompt();

  public:
    Terminal(double power_limit = 3.5);

    void run();
};

/**
 * @brief Helper function that splits a string into a vector of strings based on a delimiter.
 *
 * @param txt The string to split.
 * @param strs The vector of strings to store the result.
 * @param ch The delimiter character.
 * @return size_t The size of the vector of strings.
 */
static size_t split(const std::string &txt, std::vector<std::string> &strs, char ch);

/**
 * @brief Helper function that rounds a given double value to the desired precision
 *
 * @param value the double value to round
 * @param precision the decimal precision
 * @return double the rounded value
 */
static std::string roundTo(double value, int precision = 2);

/**
 * @brief Helper function that finds the command in a given string
 *
 * @param line the string to search the command in
 * @return std::string the command found
 */
std::string findCommand(std::string line);

/**
 * @brief Helper function that finds the device name in a given string
 *
 * @param line the string to search the device name in
 * @return std::string the device name found
 */
std::string findDeviceName(std::string line);

#endif // #ifndef