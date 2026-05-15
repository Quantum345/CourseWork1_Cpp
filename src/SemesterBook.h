#pragma once
#include "RecordBook.h"
#include "MyContainer.h"
#include <iostream>

class SemesterBook
{
private:
    unsigned int year = 0;
    MyContainer<RecordBook> recordBooks{};
public:
    SemesterBook() = default;
    SemesterBook(
        unsigned int year,
        const RecordBook* recordBooks,
        size_t recordBooksAmount
    );
    SemesterBook(const SemesterBook& rb);
    SemesterBook(SemesterBook&& rb) noexcept;
    ~SemesterBook();

    SemesterBook& operator=(const SemesterBook&);
    SemesterBook& operator=(SemesterBook&&) noexcept;

    void setYear(unsigned int value);

    unsigned int getYear() const;
    MyContainer<RecordBook> getRecordBooks() const;
    MyContainer<RecordBook> getRetakingCourseStudents() const;
    // Returns average rating of all record books
    double getAverageRating() const;

    void add(const RecordBook& rb);
    void add(RecordBook&& rb);

    friend std::istream& operator>>(std::istream& is, SemesterBook& rb);
    friend std::ostream& operator<<(std::ostream& os, const SemesterBook& rb);
};

