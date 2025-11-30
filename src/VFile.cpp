#include <iostream>
#include <sstream>
#include <fstream>
#include "VFile.h"

VFile::VFile(std::string name) : file(new std::fstream()), name(std::move(name)), refCount(1) {
    ftouch();
}

VFile::~VFile() {
    delete file;
}

char VFile::operator[](unsigned i) const {
    isValidIndex(i);
    return content[i];
}

char &VFile::operator[](unsigned i) {
    isValidIndex(i);
    if (i == content.length()) {
        content.push_back('\0');
    }
    return content[i];
}

int VFile::getRefCount() const {
    return refCount;
}

unsigned VFile::getContentLen() const {
    return content.length();
}

void VFile::addRef() {
    refCount++;
}

bool VFile::release() {
    refCount--;
    return (refCount <= 0);
}

void VFile::ftouch() const {
    std::ofstream touched;
    touched.open(name);
    touched.flush();
    touched.close();
}

void VFile::cat() const {
    std::cout << content << std::endl;
}

void VFile::wc() const {
    unsigned chars = content.length();

    unsigned lines = 0;
    for (char c : content) {
        if (c == '\n') {
            lines++;
        }
    }

    std::istringstream iss(content);
    std::string word;
    unsigned words = 0;
    while (iss >> word) {
        words++;
    }

    std::cout << lines << " " << words << " " << chars << std::endl;
}

void VFile::isValidIndex(unsigned i) const {
    if (i > content.length()) {
        throw FileSystemException("Index out of bounds: " + std::to_string(i));
    }
}
