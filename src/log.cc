/**
 * @file log.cc
 * @author Francesco Dotoli (francesco.dotoli@studenti.unipd.it)
 * @brief implementation of the log classes methods
 * @date 2025-01-11
 *
 * @copyright Copyright (c) 2025
 *
 */

#include "../include/log.h"
#include <vector>

std::string report::logs::log_file_name_;
std::vector<report::message> report::logs::logList_;

report::message::message(const Time &currentTime, const std::string &messageString, const bool &display) :
    messageTime_{currentTime},
    messageString_{messageString},
    display_(display) {}

report::logs::logs() {
    std::chrono::time_point now = std::chrono::system_clock::now();
    std::string localTime_ = std::to_string(now.time_since_epoch().count());
    log_file_name_ = "../logs/log_" + localTime_  + ".txt";
}

void report::logs::addLog(const report::message &newMessage) {
    insertionSort(newMessage);
}

void report::logs::insertionSort(report::message newMessage) {
    if(logList_.empty()) {
        logList_.push_back(newMessage);
        return;
    }
    for(auto it = logList_.begin(); it != logList_.end(); ++it) {
        if(newMessage.messageTime() < it->messageTime()) {
            logList_.insert(it, newMessage);
            return;
        }
    }
    logList_.push_back(newMessage);
}

void report::logs::displayLogs() {
    bool multipleLogs = logList_.size() > 1;
    std::string command = multipleLogs ? (logList_.end() - 2)->messageString() : "" ;
    if(command == "reset time" || command == "reset all") {
        std::vector<report::message> tmpList;
        tmpList.insert(tmpList.end(), (logList_.end() - 2), logList_.end());
        logList_.insert(logList_.begin(), tmpList.begin(), tmpList.end());
        logList_.erase(logList_.end() - 2, logList_.end());
    }
    std::ofstream logFile(log_file_name_, std::ios::app);
    for(auto &value : logList_) {
        logFile << value << std::endl;
        if(value.display()) {
            std::cout << value << std::endl;
        }
    }
    logList_.clear();
    logFile.close();
}
