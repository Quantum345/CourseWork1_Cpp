#pragma once
#include "Book.h"
#include "Subject.h"
#include <vector>

class RecordBook :
    public Book
{
private:
    unsigned int course;
    Subject* subjects;
    unsigned int subjectsAmount;
public:
    RecordBook();
    RecordBook(unsigned int course);
    RecordBook(const RecordBook& rb);
    RecordBook(RecordBook&& rb);
    ~RecordBook();

    RecordBook& operator=(const RecordBook&);
    RecordBook& operator=(RecordBook&&);

    void setCourse(unsigned int course);
    
    unsigned int getCourse();
    unsigned int getSubjectsAmount();
};

