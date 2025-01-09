#include "../include/terminal.h"
#include <iostream>
#include <string>

void Terminal::run() {
    std::string command;
    std::vector<std::string> args;
    std::cout << "\n\t\tBenvenuto a casa ;)\n" << std::endl;
    while(true) {
        std::cout << "\n>> ";
        std::getline(std::cin, command);
        if(command == "exit") {
            break;
        }
        split(command, args, ' ');
        std::string arg = args.size() > 1 ? args.at(1) : "";
        try {
            if(args[0] == "set") {
                args.erase(args.begin());
                setCommandPrompt(args);
            } else if(args[0] == "rm") {
                rmCommandPrompt(arg);
            } else if(args[0] == "show") {
                showCommandPrompt(arg);
            } else if(args[0] == "reset") {
                resetCommandPrompt(arg);
            } else if(args[0] == "help") {
                helpCommandPrompt();
            } else {
                throw std::invalid_argument(
                    "Invalid command provided. Type 'help' for more information.");
            }
        } catch(const std::invalid_argument &e) {
            std::cerr << "[!Error!]" << e.what() << std::endl;
        }
    }
}

void Terminal::log(const std::string &message) {
    std::cout << message << std::endl;
}

size_t Terminal::split(const std::string &txt, std::vector<std::string> &strs, char ch) {
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