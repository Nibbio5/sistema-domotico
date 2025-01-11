/**
 * @file log.h
 * @author Francesco Dotoli (francesco.dotoli@studenti.unipd.it)
 * @brief classes for the log system
 * @date 2025-01-11
 *
 * @copyright Copyright (c) 2025
 *
 */

#include <string>
#include <vector>
#include <chrono>
#include <fstream>
#include "time.h"

namespace report {
class message {
  private:
    std::string messageString_;
    Time messageTime_;
    bool display_ ;

  public:
    message(const Time &currentTime, const std::string &messageString, const bool &display = true) : messageTime_{currentTime}, messageString_{messageString}, display_(display) {}

    Time messageTime(void) const {
        return messageTime_;
    }

    bool display(void) const {
        return display_;
    }

    friend std::ostream &operator<<(std::ostream &out, const message &msg) {
        out << "[" << msg.messageTime_ << "] " << msg.messageString_;
        return out;
    }
};
class logs {
  private:
    std::vector<report::message> logList_;

    void insertionSort(report::message newMessage);

  public:
    logs() = default;

    void addLog(const report::message &newReport);
    void displayLogs();
    void setFirstLogToDisplay(report::message *logToDisplay);
    report::message *getFirstLogToDisplay();
};
}

// static void report(const Time &currentTime, const std::string &message) {
//     std::cout << "[" << currentTime << "]" << message << std::endl;
// }