#pragma once
#include "Book.h"
#include "Subject.h"

class RecordBook :
    public Book
{
private:
    unsigned int academicYear;
    Subject* subjects;
    unsigned int subjectsAmount;
public:
    RecordBook();
    RecordBook(unsigned int academicYear);
    RecordBook(const RecordBook& rb);
    RecordBook(RecordBook&& rb);
    ~RecordBook();

    RecordBook& operator=(const RecordBook&);
    RecordBook& operator=(RecordBook&&);

    void setAcademicYear(unsigned int academicYear);
    
    unsigned int getAcademicYear();
    unsigned int getSubjectsAmount();
};

