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

/**
 * @class message
 * @brief A class that represents a log message.
 *
 * The message class represents a log message with a timestamp and a string.
 */
class message {
  private:
    std::string messageString_; ///< The string of the message.
    Time messageTime_; ///< The time of the message.
    bool display_ ; ///< The display on terminal flag of the message.

  public:
    /**
     * @brief Default constructor for the message class.
     */
    message(const Time &currentTime, const std::string &messageString, const bool &display = true);

    /**
     * @brief Gets the time of the message.
     * @return The time of the message.
     */
    Time messageTime(void) const {
        return messageTime_;
    }

    /**
     * @brief Gets the string of the message.
     * @return The string of the message.
     */
    std::string messageString(void) const {
        return messageString_;
    }

    /**
     * @brief Gets the display flag of the message.
     * @return The display flag of the message.
     */
    bool display(void) const {
        return display_;
    }

    
    /**
     * @brief Overload of the << operator for the message class.
     * @param out The output stream.
     * @param msg The message to print.
     * @return The output stream.
     */
    friend std::ostream &operator<<(std::ostream &out, const message &msg) {
        if(!msg.display_) {
            return out << "[Command] " << msg.messageString_;;
        }
        return  out << "[" << msg.messageTime_ << "] " << msg.messageString_;
    }
};

/**
 * @class logs
 * @brief A class that represents a log system.
 *
 * The logs class represents a log system that stores log messages and displays them.
 */
class logs {
  private:
    static std::string log_file_name_;
    static std::vector<message> logList_;

    /**
     * @brief sorts the log messages by time
     * 
     * @param newMessage 
     */
    void insertionSort(message newMessage);

    /**
     * @brief Default constructor for the logs class.
     */
    logs();

  public:
    /**
     * @brief Gets the instance of the logs class.
     * @return The instance of the logs class.
     */
    static logs &getInstance() {
        static logs instance;
        return instance;
    }

    /**
     * @brief Adds a log message to the log system.
     * @param newMessage The new log message.
     */
    void addLog(const report::message &newMessage);

    /**
     * @brief Displays the log messages.
     */
    void displayLogs();
};
}