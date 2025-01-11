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
#include <chrono>
#include <locale>
#include <string>

void report::logs::addLog(const report::message &newReport) {
    logList_.push_back(newReport);
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
    std::chrono::time_point now = std::chrono::system_clock::now();
    std::string logFileName = "../logs/log_" + std::to_string(now.time_since_epoch().count()) + ".txt";
    std::ofstream logFile(logFileName);
    for(auto &value : logList_) {
        logFile << value << std::endl;
        if(value.display()) {
            std::cout << value << std::endl;

        }
    }
    logList_.clear();
    logFile.close();
}
