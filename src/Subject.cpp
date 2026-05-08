#include "Subject.h"
#include <stdexcept>
#include <iomanip>
#include <iostream>
#include <string>
#include <utility>


Subject::Subject(const std::string& name, unsigned int grade)
    : name(name), grade(grade) {}

Subject::Subject(const Subject& other)
    : name(other.name), grade(other.grade) {}

Subject::Subject(Subject&& other) noexcept
    : name(std::move(other.name)), grade(other.grade) {
    other.grade = 0;
}

Subject& Subject::operator=(const Subject& other) {
    if (this != &other) {
        name = other.name;
        grade = other.grade;
    }
    return *this;
}

Subject& Subject::operator=(Subject&& other) noexcept {
    if (this != &other) {
        name = std::move(other.name);
        grade = other.grade;
    }
    return *this;
}

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

std::string Subject::getEctsGrade() const {
    if (grade >= 90) return "A";
    if (grade >= 83) return "B";
    if (grade >= 75) return "C";
    if (grade >= 68) return "D";
    if (grade >= 50) return "E";
    if (grade >= 26) return "FX";
    return "F";
}

unsigned int Subject::getNationalGrade() const {
    if (grade >= 90) return 5;
    if (grade >= 75) return 4;
    if (grade >= 50) return 3;
    if (grade >= 25) return 2;
    return 1;
}

std::istream& operator>>(std::istream& is, Subject& s) {
    std::string classTag;
    std::string name;
    unsigned int grade = 0;

    if (!(is >> classTag >> std::quoted(name) >> grade)
        || classTag != "Subject") {
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
