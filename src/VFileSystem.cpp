#include <iostream>
#include <sstream>
#include <fstream>
#include "VFileSystem.h"

VFileSystem::VFileSystem() {
    rootDirectory = new VDirectory("V");
    currentDirectory = rootDirectory;
}

VFileSystem::~VFileSystem() {
    delete rootDirectory;
    rootDirectory = nullptr;
    currentDirectory = nullptr;
}

std::vector<std::string> VFileSystem::parsePath(const std::string &path) {
    std::vector<std::string> tokens;
    std::stringstream ss(path);
    std::string token;

    if (path.substr(0, 2) != "V/") {
        throw FileSystemException("All paths must be absolute and start with V/");
    }

    ss.ignore(2);
    while (std::getline(ss, token, '/')) {
        if (!token.empty()) {
            tokens.push_back(token);
        } else {
            throw FileSystemException("Wrong Path syntax: " + path);
        }
    }

    return tokens;
}

std::string VFileSystem::getFileName(const std::string &path) {
    size_t pos = path.find_last_of('/');
    if (pos == std::string::npos) {
        throw FileSystemException("All paths must be absolute and start with V/");
    }
    return path.substr(pos + 1);
}

VDirectory *VFileSystem::findDir(const std::string &path) const {
    size_t pos = path.find_last_of('/');
    if (pos == std::string::npos) {
        throw FileSystemException("All paths must be absolute and start with V/");
    }

    std::string DirPath = path.substr(0, pos + 1);
    std::vector<std::string> dirs = parsePath(DirPath);
    VDirectory *current = rootDirectory;

    for (const auto &dir: dirs) {
        current = current->getSubDir(dir);
    }
    return current;
}

VFile *VFileSystem::getFile(const std::string &path) const {
    std::string fileName = getFileName(path);
    VDirectory *dir = findDir(path);
    return dir->getFile(fileName);
}

VFile *VFileSystem::createFile(const std::string &path) {
    std::string fileName = getFileName(path);
    VDirectory *dir = findDir(path);

    auto *file = new VFile(fileName);
    dir->addFile(fileName, file);
    return file;
}

void VFileSystem::readFile(const std::string &filePath, int i) const {
    try {
        const VFile *file = getFile(filePath);
        std::cout << (*file)[i] << std::endl;
    } catch (const FileSystemException &) {
        // file not found in virtual, so it is real file in local dir
        std::ifstream realFile("./" + filePath);
        realFile.seekg(i);
        char c;
        realFile.get(c);
        std::cout << c << std::endl;
        realFile.close();
    }
}

void VFileSystem::writeFile(const std::string &filePath, int i, char c) {
    try {
        VFile *file = getFile(filePath);
        (*file)[i] = c;
    } catch (const FileSystemException &) {
        std::fstream realFile("./" + filePath);
        realFile.seekp(i);
        realFile.put(c);
        realFile.close();
    }
}

void VFileSystem::touchFile(const std::string &filePath) {
    try {
        auto file = getFile(filePath);
        file->ftouch();
    } catch (const FileSystemException &) {
        // file not found, creat new one
        createFile(filePath);
    }
}

void VFileSystem::copyFile(const std::string &source, const std::string &target) {
    std::string content;
    try {
        VFile *sourceFile = getFile(source);
        for (std::size_t i = 0; i < sourceFile->getContentLen(); i++) {
            content += (*sourceFile)[i];
        }
    } catch (const FileSystemException &) {
        // src is not found in virtual system, so it is real file in the local dir
        std::string src = "./" + source;
        std::ifstream realFile(src);
        if (!realFile) {
            throw FileSystemException("Source file not found: " + source);
        }

        std::string line;
        while (std::getline(realFile, line)) {
            content += line + '\n';
        }
        realFile.close();
    }

    VFile *targetFile;
    try {
        targetFile = getFile(target);
    } catch (const FileSystemException &) {
        // target file not found, creat it
        targetFile = createFile(target);
    }

    for (std::size_t i = 0; i < content.length(); i++) {
        (*targetFile)[i] = content[i];
    }
}

void VFileSystem::removeFile(const std::string &filePath) {
    std::string fileName = getFileName(filePath);
    VDirectory *dir = findDir(filePath);
    dir->removeFile(fileName);
}

void VFileSystem::moveFile(const std::string &source, const std::string &target) {
    copyFile(source, target);
    removeFile(source);
}

void VFileSystem::catFile(const std::string &filePath) const {
    try {
        VFile *file = getFile(filePath);
        file->cat();
    } catch (const FileSystemException &) {
        std::ifstream file("./" + filePath);
        std::string line;
        while (std::getline(file, line)) {
            std::cout << line << '\n';
        }
        file.close();
    }
}

void VFileSystem::wcFile(const std::string &filePath) const {
    try {
        VFile *file = getFile(filePath);
        file->wc();
    } catch (const FileSystemException &) {
        std::ifstream realFile("./" + filePath);
        unsigned lines = 0, words = 0, chars = 0;

        std::string line;
        while (std::getline(realFile, line)) {
            ++lines;
            chars += line.length() + 1;

            std::istringstream iss(line);
            std::string word;
            while (iss >> word) {
                words++;
            }

        }
        std::cout << lines << " " << words << " " << chars << std::endl;
        realFile.close();
    }
}

void VFileSystem::ln(const std::string &target, const std::string &linkName) {
    std::string linkFileName = getFileName(linkName);
    VDirectory *linkDir = findDir(linkName);

    if (linkDir->hasFile(linkFileName)) {
        throw FileSystemException("Link already exists: " + linkName);
    }

    try {
        std::string targetFileName = getFileName(target);
        VDirectory *targetDir = findDir(target);

        if (targetDir->hasFile(targetFileName)) {
            VFile *targetFile = targetDir->getFile(targetFileName);
            linkDir->addFile(linkFileName, targetFile);
            targetFile->addRef();
            return;
        }
    } catch (const FileSystemException &) {
        // src is not found in virtual system, so it is real file in the local dir
        std::string src = "./" + target;
        std::ifstream realFile(src);
        if (!realFile) {
            throw FileSystemException("Target file not found: " + src);
        }

        std::string content;
        std::string line;
        while (std::getline(realFile, line)) {
            content += line + '\n';
        }
        realFile.close();

        auto *newFile = new VFile(linkFileName);
        try {
            for (std::size_t i = 0; i < content.length(); i++) {
                (*newFile)[i] = content[i];
            }
            linkDir->addFile(linkFileName, newFile);
        } catch (...) {
            // free memory if adding file failed
            delete newFile;
            throw;
        }
    }
}

void VFileSystem::mkdir(const std::string &dirPath) {
    if (dirPath.back() != '/') {
        throw FileSystemException("Directory name must end with /");
    }

    std::vector<std::string> path = parsePath(dirPath);
    if (path.empty()) {
        throw FileSystemException("Invalid directory name");
    }

    VDirectory *parentDir = rootDirectory;
    for (std::size_t i = 0; i < path.size() - 1; ++i) {
        parentDir = parentDir->getSubDir(path[i]);
    }

    std::string newDirName = path.back();
    auto *newDir = new VDirectory(newDirName, parentDir);
    try {
        parentDir->addSubDir(newDir);
    } catch (...) {
        // free memory if adding dir failed
        delete newDir;
        throw;
    }
}

void VFileSystem::chdir(const std::string &dirPath) {
    VDirectory *dir = findDir(dirPath);
    currentDirectory = dir;
}

void VFileSystem::rmdir(const std::string &dirPath) {
    if (dirPath == "V/") {
        throw FileSystemException("Cannot remove root directory");
    }

    std::string dirName = getFileName(dirPath.substr(0, dirPath.length() - 1));
    VDirectory *parentDir = findDir(dirPath.substr(0, dirPath.length() - 1));
    VDirectory *dirToRemove = parentDir->getSubDir(dirName);

    // if current dir will be removed or is a subDir of the dir to be removed
    if (currentDirectory == dirToRemove || isSubDirOf(currentDirectory, dirToRemove)) {
        currentDirectory = parentDir;
    }

    parentDir->removeSubDir(dirName);
}

bool VFileSystem::isSubDirOf(VDirectory *potentialChild, VDirectory *potentialParent) {
    if (potentialChild == potentialParent) {
        return true;
    }

    VDirectory *parent = potentialChild->getParent();
    while (parent != nullptr) {
        if (parent == potentialParent) {
            return true;
        }
        parent = parent->getParent();
    }
    return false;
}

void VFileSystem::ls(const std::string &dirPath) const {
    VDirectory *dir = findDir(dirPath);
    std::vector<std::string> contents = dir->listContents();

    std::cout << dir->getFullPath() << ":" << std::endl;
    for (const auto &item: contents) {
        std::cout << item << std::endl;
    }
}

void VFileSystem::pwd() const {
    std::cout << currentDirectory->getFullPath() << std::endl;
}

void VFileSystem::lproot() {
    std::map<std::string, VDirectory *> allDirs;
    collectDirs(allDirs, rootDirectory);

    for (const auto &pair: allDirs) {
        const std::string &dirPath = pair.first;
        VDirectory *dir = pair.second;

        std::cout << dirPath << ":" << std::endl;
        for (const auto &fileName: dir->listContents()) {
            if (fileName.back() != '/') {
                VFile *file = dir->getFile(fileName);
                std::cout << fileName << " " << file->getRefCount() << std::endl;
            }
        }
        std::cout << std::endl;
    }
}

void VFileSystem::collectDirs(std::map<std::string, VDirectory *> &allDirs, VDirectory *dir) {
    std::string path = dir->getFullPath();
    allDirs[path] = dir;

    for (const auto &pair: dir->getSubDirs()) {
        collectDirs(allDirs, pair.second);
    }
}
