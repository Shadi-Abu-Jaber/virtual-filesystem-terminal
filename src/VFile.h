#ifndef ADVANCE_OOP_HW1_VFILE_H
#define ADVANCE_OOP_HW1_VFILE_H

#include <iostream>
#include <vector>

class FileSystemException : public std::exception {
private:
    std::string message;
public:
    explicit FileSystemException(std::string msg) : message(std::move(msg)) {}

    const char *what() const noexcept override {
        return message.c_str();
    }
};

class VFile {
private:
    std::fstream *file;
    std::string name;
    int refCount;
    std::string content;

    void isValidIndex(unsigned i) const;

public:
    explicit VFile(std::string name);

    ~VFile();

    char operator[](unsigned i) const;

    char &operator[](unsigned i);

    int getRefCount() const;

    unsigned getContentLen() const;

    void addRef();

    bool release();

    void ftouch() const;

    void cat() const;

    void wc() const;
};

#endif //ADVANCE_OOP_HW1_VFILE_H
