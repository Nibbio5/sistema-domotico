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

public:
  Terminal();
  ~Terminal();
  void setCommandPrompt(const std::vector<std::string> &args);
  void rmCommandPrompt(const std::string &arg);
  void showCommandPrompt(const std::string &arg);
  void resetCommandPrompt(const std::string &arg);

  void print(const char *str) { std::cout << str; }
  void print(int num) { std::cout << num; }
  void print(char ch) { std::cout << ch; }
  void print(double num) { std::cout << num; }
  void print(float num) { std::cout << num; }
  void print(bool b) { std::cout << b; }
  void println(const char *str) { std::cout << str << std::endl; }
  void println(int num) { std::cout << num << std::endl; }
  void println(char ch) { std::cout << ch << std::endl; }
  void println(double num) { std::cout << num << std::endl; }
  void println(float num) { std::cout << num << std::endl; }
  void println(bool b) { std::cout << b << std::endl; }
  void println() { std::cout << std::endl; }
  void clear() { std::cout << "\033[2J\033[1;1H"; }
};
#endif // #ifndef
