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
    void showOneDevice(const std::string &arg);
    void setCommandPrompt(const std::vector<std::string> &args);
    void rmCommandPrompt(const std::string &arg);
    void showCommandPrompt(const std::string &arg);
    void resetCommandPrompt(const std::string &arg);

    void setDeviceTimer(const std::string &device_name, const std::string &start,
                        const std::string &stop);

  public:
    Terminal() = default;
    ~Terminal() = default;

};
#endif // #ifndef
