#include <iomanip>
#include <utility>
#include "Book.h"

Book::Book() = default;

Book::Book(
    const UdcCode& udcCode,
    const std::string& author,
    const std::string& name,
    const std::string& publishingHouse,
    unsigned int year
) : udcCode(udcCode),
author(author),
name(name),
publishingHouse(publishingHouse),
year(year) {}

Book::Book(const Book& b)
    : udcCode(b.udcCode),
    author(b.author),
    name(b.name),
    publishingHouse(b.publishingHouse),
    year(b.year) {}

Book::Book(Book&& b) noexcept
    : udcCode(std::move(b.udcCode)),
    author(std::move(b.author)),
    name(std::move(b.name)),
    publishingHouse(std::move(b.publishingHouse)),
    year(b.year) {}

Book::~Book() = default;

Book& Book::operator=(const Book& b) {
    if (this != &b) {
        udcCode = b.udcCode;
        author = b.author;
        name = b.name;
        publishingHouse = b.publishingHouse;
        year = b.year;
    }
    return *this;
}

Book& Book::operator=(Book&& b) noexcept {
    if (this != &b) {
        udcCode = std::move(b.udcCode);
        author = std::move(b.author);
        name = std::move(b.name);
        publishingHouse = std::move(b.publishingHouse);
        year = b.year;
    }
    return *this;
}

Book& Book::operator()(
    const UdcCode& udcCode,
    const std::string& author,
    const std::string& name,
    const std::string& publishingHouse,
    unsigned int year
    ) {
    this->setUdcCode(udcCode);
    this->setAuthor(author);
    this->setName(name);
    this->setPublishingHouse(publishingHouse);
    this->setYear(year);
    return *this;
}

void Book::setUdcCode(const UdcCode& udc) {
    this->udcCode = udc;
}

void Book::setAuthor(const std::string& author) {
    this->author = author;
}

void Book::setName(const std::string& name) {
    this->name = name;
}

void Book::setPublishingHouse(const std::string& publishingHouse) {
    this->publishingHouse = publishingHouse;
}

void Book::setYear(unsigned int year) {
    this->year = year;
}

UdcCode Book::getUdcCode() const {
    return this->udcCode;
}

std::string Book::getAuthor() const {
    return this->author;
}

std::string Book::getName() const {
    return this->name;
}

std::string Book::getPublishingHouse() const {
    return this->publishingHouse;
}

unsigned int Book::getYear() const {
    return this->year;
}

void Book::printId(std::ostream& os) const {
    os << "УДК " << getUdcCode().getCode();
}

bool Book::matchesName(const std::string& substring) const {
    return name.find(substring) != std::string::npos;
}

bool Book::matchesAuthor(const std::string& substring) const {
    return author.find(substring) != std::string::npos;
}

bool Book::matchesPublishingHouse(const std::string& substring) const {
    return publishingHouse.find(substring) != std::string::npos;
}

std::istream& operator>>(std::istream& is, Book& b) {
    std::string classTag;
    std::string author;
    std::string name;
    std::string publishingHouse;
    unsigned int year = 0;

    if (!(is >> classTag >> b.udcCode
          >> std::quoted(author) >> std::quoted(name)
          >> std::quoted(publishingHouse) >> year)
        || classTag != "Book") {
        is.setstate(std::ios::failbit);
        return is;
    }

    b.author = std::move(author);
    b.name = std::move(name);
    b.publishingHouse = std::move(publishingHouse);
    b.setYear(year);
    return is;
}

std::ostream& operator<<(std::ostream& os, const Book& b) {
    os << "Book " << b.udcCode << ' '
        << std::quoted(b.author) << ' '
        << std::quoted(b.name) << ' '
        << std::quoted(b.publishingHouse) << ' '
        << b.year;
    return os;
}
