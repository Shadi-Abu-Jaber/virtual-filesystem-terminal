#ifndef ADVANCE_OOP_HW1_VDIRECTORY_H
#define ADVANCE_OOP_HW1_VDIRECTORY_H

#include "VFile.h"
#include <map>
#include <vector>

class VDirectory {
private:
    std::string name;
    VDirectory *parent;
    std::map<std::string, VDirectory *> subDirs;
    std::map<std::string, VFile *> files;

public:
    explicit VDirectory(std::string dirName, VDirectory *parentDir = nullptr);

    ~VDirectory();

    std::string getName() const;

    VDirectory *getParent() const;

    std::map<std::string, VDirectory *> getSubDirs() const;

    std::string getFullPath() const;

    void addSubDir(VDirectory *subDir);

    VDirectory *getSubDir(const std::string &dirName) const;

    void removeSubDir(const std::string &dirName);

    void addFile(const std::string &fileName, VFile *file);

    VFile *getFile(const std::string &fileName) const;

    bool hasFile(const std::string &fileName) const;

    void removeFile(const std::string &fileName);

    std::vector<std::string> listContents() const;

};

#endif //ADVANCE_OOP_HW1_VDIRECTORY_H
