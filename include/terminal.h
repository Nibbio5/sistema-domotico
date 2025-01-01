#ifndef TERMINAL_H
#define TERMINAL_H

#include <iostream>
#include <string>
#include <vector>
#include "device.h"
#include "domotics_system.h" // Include the header file for DomoticsSystem

class Terminal {
private:
  DomoticsSystem domotics_system;

  void setTimeCommandPrompt(const std::string &arg);
  Device* isDevice(const std::string &device_name);
  void setDeviceCommandPrompt(const std::vector<std::string> &args, Device *device);
public:
  Terminal();
  ~Terminal();
  void setCommandPrompt(const std::vector<std::string> &args);
  void rmCommandPrompt(const std::string &arg);
  void showCommandPrompt(const std::string &arg);
  void resetCommandPrompt(const std::string &arg);
  void print(const char *str);
  void print(int num);
  void print(char ch);
  void print(double num);
  void print(float num);
  void print(bool b);
  void println(const char *str);
  void println(int num);
  void println(char ch);
  void println(double num);
  void println(float num);
  void println(bool b);
  void println();
  void clear();
};
#endif // #ifndef
