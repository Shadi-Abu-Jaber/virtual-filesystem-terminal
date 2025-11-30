#include <iostream>
#include <sstream>
#include "Terminal.h"

Terminal::Terminal() : running(true) {}

std::vector<std::string> Terminal::parseCommand(const std::string &commandLine) {
    std::vector<std::string> tokens;
    std::stringstream ss(commandLine);
    std::string token;

    while (ss >> token) {
        tokens.push_back(token);
    }
    return tokens;
}

void Terminal::executeCommand(const std::vector<std::string> &tokens) {
    if (tokens.empty()) {
        return;
    }

    try {
        const std::string &command = tokens[0];

        if (command == "read") {
            if (tokens.size() != 3) {
                throw FileSystemException("Usage: read <FileName> <Position>");
            }
            int i = std::stoi(tokens[2]);
            fileSystem.readFile(tokens[1], i);
        } else if (command == "write") {
            if (tokens.size() != 4 || tokens[3].length() != 1) {
                throw FileSystemException("Usage: write <FileName> <Position> <Character>");
            }
            int i = std::stoi(tokens[2]);
            char character = tokens[3][0];
            fileSystem.writeFile(tokens[1], i, character);
        } else if (command == "touch") {
            if (tokens.size() != 2) {
                throw FileSystemException("Usage: touch <FileName>");
            }
            fileSystem.touchFile(tokens[1]);
        } else if (command == "copy") {
            if (tokens.size() != 3) {
                throw FileSystemException("Usage: copy <Source_FileName> <Target_FileName>");
            }
            fileSystem.copyFile(tokens[1], tokens[2]);
        } else if (command == "remove") {
            if (tokens.size() != 2) {
                throw FileSystemException("Usage: remove <FileName>");
            }
            fileSystem.removeFile(tokens[1]);
        } else if (command == "move") {
            if (tokens.size() != 3) {
                throw FileSystemException("Usage: move <Source_FileName> <Target_FileName>");
            }
            fileSystem.moveFile(tokens[1], tokens[2]);
        } else if (command == "cat") {
            if (tokens.size() != 2) {
                throw FileSystemException("Usage: cat <FileName>");
            }
            fileSystem.catFile(tokens[1]);
        } else if (command == "wc") {
            if (tokens.size() != 2) {
                throw FileSystemException("Usage: wc <FileName>");
            }
            fileSystem.wcFile(tokens[1]);
        } else if (command == "ln") {
            if (tokens.size() != 3) {
                throw FileSystemException("Usage: ln <Target_FileName> <LinkName>");
            }
            fileSystem.ln(tokens[1], tokens[2]);
        } else if (command == "mkdir") {
            if (tokens.size() != 2) {
                throw FileSystemException("Usage: mkdir <DirName>");
            }
            fileSystem.mkdir(tokens[1]);
        } else if (command == "chdir") {
            if (tokens.size() != 2) {
                throw FileSystemException("Usage: chdir <DirName>");
            }
            fileSystem.chdir(tokens[1]);
        } else if (command == "rmdir") {
            if (tokens.size() != 2) {
                throw FileSystemException("Usage: rmdir <DirName>");
            }
            fileSystem.rmdir(tokens[1]);
        } else if (command == "ls") {
            if (tokens.size() != 2) {
                throw FileSystemException("Usage: ls <DirName>");
            }
            fileSystem.ls(tokens[1]);
        } else if (command == "lproot") {
            if (tokens.size() != 1) {
                throw FileSystemException("Usage: lproot");
            }
            fileSystem.lproot();
        } else if (command == "pwd") {
            if (tokens.size() != 1) {
                throw FileSystemException("Usage: pwd");
            }
            fileSystem.pwd();
        } else if (command == "exit") {
            if (tokens.size() != 1) {
                throw FileSystemException("Usage: exit");
            }
            running = false;
        } else {
            throw FileSystemException("Unknown command: " + command);
        }
    } catch (const std::exception &e) {
        std::cerr << "ERROR: " << e.what() << std::endl;
    }
}

void Terminal::run() {
    std::string commandLine;
    while (running) {
        std::cout << "$ ";
        std::getline(std::cin, commandLine);
        std::vector<std::string> tokens = parseCommand(commandLine);
        executeCommand(tokens);
    }
}
