#include "../include/terminal.h"
#include "../include/time.h"

Terminal::Terminal() {}

Terminal::~Terminal() {}

void Terminal::setCommandPrompt(const std::vector<std::string> &args) {
  if (args.size() < 2) {
    throw std::invalid_argument(
        "Invalid arguments provided. Type 'help' for more information.");
  }

  if (args[0] == "time") {
    setTimeCommandPrompt(args.at(1));
  }
}

void setTimeCommandPrompt(const std::string &arg) {
  if (arg.empty()) {
    throw std::invalid_argument(
        "Invalid arguments provided. Type 'help' for more information.");
  }

  Time time = Time::fromString(arg);

  // TODO:call the time setter
}
