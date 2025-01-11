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
    DomoticsSystem domotics_system_;
    report::logs log_;

    void setTimeCommandPrompt(const std::string &arg);
    Device *isDevice(const std::string &device_name);
    void setDeviceCommandPrompt(const std::vector<std::string> &args, Device *device);
    std::string showOneDevice(const Device *device, const bool &show_time = true);
    void setCommandPrompt(const std::vector<std::string> &args);
    void rmCommandPrompt(const std::string &arg);
    void showCommandPrompt(const std::string &arg);
    void resetCommandPrompt(const std::string &arg);
    void setDeviceTimer(const std::string &device_name, const std::string &start, const std::string &stop);
    void helpCommandPrompt();

    size_t split(const std::string &txt, std::vector<std::string> &strs, char ch);

  public:
    Terminal(double power_limit = 3.5);

    void run();
};

/**
 * @brief Helper function that rounds a given double value to the diresred precision
 *
 * @param value the double value to round
 * @param precision the decimal precision
 * @return double the rounded value
 */
static std::string roundTo(double value, int precision = 2);

#endif // #ifndef