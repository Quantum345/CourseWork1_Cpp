#pragma once
#include <string>
#include <iostream>

class Subject {
private:
    std::string name;
    unsigned int grade = 0;
public:
    Subject() = default;
    Subject(const std::string& name, unsigned int grade) : name(name), grade(grade) {}
    ~Subject() = default;

    void setName(const std::string& name);
    void setGrade(unsigned int grade);

    std::string getName() const;
    unsigned int getGrade() const;

    friend std::istream& operator>>(std::istream& is, Subject& s);
    friend std::ostream& operator<<(std::ostream& os, const Subject& s);
};