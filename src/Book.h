#pragma once
#include <iostream>
#include <string>
#include "UdcCode.h"

class Book {
private:
    UdcCode udcCode;
    std::string author;
    std::string name;
    std::string publishingHouse;
    unsigned int year{};
public:

    Book();

    Book(const UdcCode& udcCode,
         const std::string& author,
         const std::string& name,
         const std::string& publishingHouse,
         unsigned int year);

    Book(const Book& b);

    Book(Book&& b) noexcept;

    virtual ~Book();

    Book& operator=(const Book& b);

    Book& operator=(Book&& b) noexcept;

    Book& operator()(
        const UdcCode& udcCode,
        const std::string& author,
        const std::string& name,
        const std::string& publishingHouse,
        unsigned int year);

    void setUdcCode(const UdcCode& udc);
    void setAuthor(const std::string& author);
    void setName(const std::string& name);
    void setPublishingHouse(const std::string& publishingHouse);
    void setYear(unsigned int year);

    UdcCode getUdcCode() const;
    std::string getAuthor() const;
    std::string getName() const;
    std::string getPublishingHouse() const;
    unsigned int getYear() const;

    virtual void printId(std::ostream& os) const;
    bool matchesName(const std::string& substring) const;
    bool matchesAuthor(const std::string& a) const;
    bool matchesPublishingHouse(const std::string& ph) const;

    friend std::istream& operator>>(std::istream& is, Book& b);
    friend std::ostream& operator<<(std::ostream& os, const Book& b);
};

