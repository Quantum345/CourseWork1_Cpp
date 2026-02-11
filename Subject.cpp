#include "Subject.h"
#include <stdexcept>

void Subject::setName(const std::string& name)
{
    this->name = name;
}

void Subject::setGrade(unsigned int grade)
{
    if (grade < 0 || grade > 100)
        throw std::invalid_argument("Invalid argument: the grade value must be in range from 0 to 100.");
    this->grade = grade;
}

std::string Subject::getName() const
{
    return this->name;
}

unsigned int Subject::getGrade() const
{
    return this->grade;
}
