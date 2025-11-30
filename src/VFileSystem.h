#ifndef ADVANCE_OOP_HW1_VFILESYSTEM_H
#define ADVANCE_OOP_HW1_VFILESYSTEM_H

#include "VFile.h"
#include "VDirectory.h"
#include <vector>

class VFileSystem {
private:
    VDirectory *rootDirectory;
    VDirectory *currentDirectory;

    static std::vector<std::string> parsePath(const std::string &path);

    static std::string getFileName(const std::string &path);

    VDirectory *findDir(const std::string &path) const;

    VFile *getFile(const std::string &path) const;

    VFile *createFile(const std::string &path);

    static bool isSubDirOf(VDirectory *potentialChild, VDirectory *potentialParent);

    void collectDirs(std::map<std::string, VDirectory*>& allDirs, VDirectory* dir);

public:
    VFileSystem();

    ~VFileSystem();

    void readFile(const std::string &filePath, int i) const;

    void writeFile(const std::string &filePath, int i, char c);

    void touchFile(const std::string &filePath);

    void catFile(const std::string &filePath) const;

    void wcFile(const std::string &filePath) const;

    void removeFile(const std::string &filePath);

    void moveFile(const std::string &source, const std::string &target);

    void copyFile(const std::string &source, const std::string &target);

    void ln(const std::string &target, const std::string &linkName);

    void mkdir(const std::string &dirPath);

    void chdir(const std::string &dirPath);

    void rmdir(const std::string &dirPath);

    void ls(const std::string &dirPath) const;

    void lproot();

    void pwd() const;

};

#endif //ADVANCE_OOP_HW1_VFILESYSTEM_H
