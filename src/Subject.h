#pragma once
#include <iostream>
#include <string>

class Subject {
private:
    std::string name;
    unsigned int grade = 0;
public:
    Subject() = default;
    Subject(const std::string& name, unsigned int grade);
    Subject(const Subject& other);
    Subject(Subject&& other) noexcept;
    ~Subject() = default;

    Subject& operator=(const Subject& other);
    Subject& operator=(Subject&& other) noexcept;

    void setName(const std::string& name);
    void setGrade(unsigned int grade);

    std::string getName() const;
    unsigned int getGrade() const;
    std::string getEctsGrade() const;
    unsigned int getNationalGrade() const;

    friend std::istream& operator>>(std::istream& is, Subject& s);
    friend std::ostream& operator<<(std::ostream& os, const Subject& s);
};