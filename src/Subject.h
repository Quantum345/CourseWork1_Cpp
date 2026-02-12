#pragma once
#include <string>
class Subject
{
private:
    std::string name;
    unsigned int grade;
public:
    Subject() = default;
    Subject(const std::string& name, unsigned int grade): name(name), grade(grade)
    {}
    ~Subject() = default;

    void setName(const std::string& name);
    void setGrade(unsigned int grade);

    std::string getName() const;
    unsigned int getGrade() const;
};