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
    message(const Time &currentTime, const std::string &messageString, const bool &display = true);

    Time messageTime(void) const {
        return messageTime_;
    }

    std::string messageString(void) const {
        return messageString_;
    }

    bool display(void) const {
        return display_;
    }

    friend std::ostream &operator<<(std::ostream &out, const message &msg) {
        if(!msg.display_) {
            return out << "[Command] " << msg.messageString_;;
        }
        return  out << "[" << msg.messageTime_ << "] " << msg.messageString_;
    }
};
class logs {
  private:
    static std::string log_file_name_;
    static std::vector<message> logList_;

    void insertionSort(message newMessage);

    logs();

  public:
    static logs &getInstance() {
        static logs instance;
        return instance;
    }

    void addLog(const report::message &newMessage);
    void displayLogs();
};
}