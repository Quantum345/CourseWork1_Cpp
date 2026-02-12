#pragma once
#include "Book.h"
#include "Subject.h"
#include <vector>

class RecordBook :
    public Book
{
private:
    unsigned int course;
    std::vector<Subject> subjects;
};

