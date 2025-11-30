#include <iostream>
#include "VDirectory.h"

VDirectory::VDirectory(std::string dirName, VDirectory *parentDir)
        : name(std::move(dirName)), parent(parentDir) {}

VDirectory::~VDirectory() {
    for (auto &pair: files) {
        // if file ref count reach 0
        if (pair.second->release()) {
            delete pair.second;
            // clean local dir (real dir)
            std::string fileName = "./" + pair.first;
            std::remove(fileName.c_str());
        }
    }
    files.clear();

    for (auto &pair: subDirs) {
        delete pair.second;
    }
    subDirs.clear();
}

std::string VDirectory::getName() const {
    return name;
}

VDirectory *VDirectory::getParent() const {
    return parent;
}

std::map<std::string, VDirectory *> VDirectory::getSubDirs() const {
    return subDirs;
}

std::string VDirectory::getFullPath() const {
    if (parent == nullptr) {
        return name + "/";
    } else {
        std::string parentPath = parent->getFullPath();
        return parentPath + name + "/";
    }
}

void VDirectory::addSubDir(VDirectory *subDir) {
    std::string dirName = subDir->getName();
    if (subDirs.find(dirName) != subDirs.end()) {
        throw FileSystemException("Directory already exists: " + dirName);
    }
    subDirs[dirName] = subDir;
}

VDirectory *VDirectory::getSubDir(const std::string &dirName) const {
    auto it = subDirs.find(dirName);
    if (it == subDirs.end()) {
        throw FileSystemException("No such directory: " + dirName);
    }
    return it->second;
}

void VDirectory::removeSubDir(const std::string &dirName) {
    auto it = subDirs.find(dirName);
    if (it == subDirs.end()) {
        throw FileSystemException("No such directory to remove: " + dirName);
    }
    delete it->second;
    subDirs.erase(it);
}

bool VDirectory::hasFile(const std::string &fileName) const {
    return (files.find(fileName) != files.end());
}

void VDirectory::addFile(const std::string &fileName, VFile *file) {
    if (hasFile(fileName)) {
        throw FileSystemException("File already exists: " + fileName);
    }
    files[fileName] = file;
}

VFile *VDirectory::getFile(const std::string &fileName) const {
    auto it = files.find(fileName);
    if (it == files.end()) {
        throw FileSystemException("No such file: " + fileName);
    }
    return it->second;
}

void VDirectory::removeFile(const std::string &fileName) {
    auto it = files.find(fileName);
    if (it == files.end()) {
        throw FileSystemException("No such file to remove: " + fileName);
    }
    if (it->second->release()) {
        delete it->second;
        // remove from local dir
        std::string removedFile = "./" + it->first;
        std::remove(removedFile.c_str());
    }
    files.erase(it);
}

std::vector<std::string> VDirectory::listContents() const {
    std::vector<std::string> contents;

    contents.reserve(subDirs.size());
    for (const auto &pair: subDirs) {
        contents.push_back(pair.first + "/");
    }

    for (const auto &pair: files) {
        contents.push_back(pair.first);
    }
    return contents;
}
