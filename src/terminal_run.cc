/**
 * @file terminal_run.cc
 * @author Francesco Dotoli (francesco.dotoli@studenti.unipd.it)
 * @brief part of implementation of the terminal class
 * @date 2024-12-27
 *
 */
#include "../include/terminal.h"
#include <string>

void Terminal::run() {
    std::string line;
    std::vector<std::string> args;
    log_.displayLogs();

    while(true) {
        std::cout << "\n>> ";
        std::getline(std::cin, line);
        std::string command = findCommand(line);
        if(command == "exit") {
            break;
        }
        line.erase(0, command.length() + 1);
        std::string device_name = "";
        if(line.front() == '"') {
            device_name = findDeviceName(line);
            line.erase(0, device_name.length() + 3);
        }
        split(line, args, ' ');
        if(!device_name.empty()) {
            args.insert(args.begin(), device_name);
        }
        std::string arg = args.size() > 1 ? args.at(0) : "";
        try {
            log_.addLog(report::message(domotics_system_.getCurrentTime(), command, false));
            log_.addLog(report::message(domotics_system_.getCurrentTime(), "L'orario attuale è " + domotics_system_.getCurrentTime().getHourString() + ":" + domotics_system_.getCurrentTime().getMinuteString()));
            if(command == "set") {
                setCommandPrompt(args);
            } else if(command == "rm") {
                rmCommandPrompt(arg);
            } else if(command == "show") {
                showCommandPrompt(arg);
            } else if(command == "reset") {
                resetCommandPrompt(arg);
            } else if(command == "help") {
                helpCommandPrompt();
            } else {
                throw std::invalid_argument("Commando sbagliato. Scrivere 'help' per maggiori informazioni.");
            }
        } catch(const std::invalid_argument &e) {
            log_.addLog(report::message(domotics_system_.getCurrentTime(), std::string("[!Error!] ") + e.what()));
        }
        log_.displayLogs();
    }
}

size_t split(const std::string &txt, std::vector<std::string> &strs, char ch) {
    size_t pos = txt.find(ch);
    size_t initialPos = 0;
    strs.clear();

    // Decompose statement
    while(pos != std::string::npos) {
        strs.push_back(txt.substr(initialPos, pos - initialPos));
        initialPos = pos + 1;

        pos = txt.find(ch, initialPos);
    }

    // Add the last one
    strs.push_back(txt.substr(initialPos, std::min(pos, txt.size()) - initialPos + 1));

    return strs.size();
}

std::string findCommand(std::string line) {
    size_t pos = line.find(" ");
    if(pos == std::string::npos) {
        return line;
    }
    return line.substr(0, pos);;
};

std::string findDeviceName(std::string line) {
    size_t pos = line.find('"', 1);
    if(pos == std::string::npos) {
        throw std::invalid_argument("Nome del dispositivo non valido. Scrivere 'help' per maggiori informazioni.");
    }
    return line.substr(1, pos - 1);
};