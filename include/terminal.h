#ifndef TERMINAL_H
#define TERMINAL_H

#include "device.h"
#include "domotics_system.h"
#include <string>
#include <vector>

class Terminal {
  private:
    DomoticsSystem domotics_system;

    void setTimeCommandPrompt(const std::string &arg);
    Device *isDevice(const std::string &device_name);
    void setDeviceCommandPrompt(const std::vector<std::string> &args,
                                Device *device);
    std::string showOneDevice(const Device *device, const bool &show_time = true);
    void setCommandPrompt(const std::vector<std::string> &args);
    void rmCommandPrompt(const std::string &arg);
    void showCommandPrompt(const std::string &arg);
    void resetCommandPrompt(const std::string &arg);
    void setDeviceTimer(const std::string &device_name, const std::string &start,
                        const std::string &stop);
    void helpCommandPrompt();

    size_t split(const std::string &txt, std::vector<std::string> &strs, char ch);

  public:
    Terminal(double power_limit = 3.5);
    ~Terminal() = default;

    void run();
    static void log(const std::string &message);
};

#endif // #ifndef
