#ifndef ADVANCE_OOP_HW1_TERMINAL_H
#define ADVANCE_OOP_HW1_TERMINAL_H

#include "VFileSystem.h"
#include <vector>

class Terminal {
private:
    VFileSystem fileSystem;
    bool running;

    static std::vector<std::string> parseCommand(const std::string& commandLine) ;

    void executeCommand(const std::vector<std::string>& tokens);

public:
    Terminal();

    void run();
};

#endif //ADVANCE_OOP_HW1_TERMINAL_H
