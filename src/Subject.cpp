#include "Subject.h"
#include <stdexcept>
#include <iomanip>
#include <iostream>
#include <string>
#include <utility>

void Subject::setName(const std::string& value) {
    this->name = value;
}

void Subject::setGrade(unsigned int value) {
    if (value > 100)
        throw std::invalid_argument("Invalid argument: the grade value must be in range from 0 to 100.");
    this->grade = value;
}

std::string Subject::getName() const {
    return this->name;
}

unsigned int Subject::getGrade() const {
    return this->grade;
}

std::istream& operator>>(std::istream& is, Subject& s) {
    std::string classTag;
    std::string name;
    unsigned int grade = 0;

    if (!(is >> classTag) || classTag != "Subject") {
        is.setstate(std::ios::failbit);
        return is;
    }

    if (!(is >> std::quoted(name)))
        return is;

    if (!(is >> grade)) {
        is.setstate(std::ios::failbit);
        return is;
    }

    s.name = std::move(name);
    s.grade = grade;
    return is;
}

std::ostream& operator<<(std::ostream& os, const Subject& s) {
    os << "Subject " << std::quoted(s.name) << ' ' << s.grade;
    return os;
}
