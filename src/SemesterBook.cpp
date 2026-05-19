#include "SemesterBook.h"
#include <string>
#include "RecordBook.h"
#include "MyContainer.h"
#include <iostream>
#include <utility>

SemesterBook::SemesterBook(
    unsigned int year,
    const RecordBook* recordBooks,
    size_t recordBooksAmount
) : year(year),
recordBooks(recordBooks, recordBooksAmount) {}

SemesterBook::SemesterBook(const SemesterBook& sb)
    : year(sb.year),
    recordBooks(sb.recordBooks) {}

SemesterBook::SemesterBook(SemesterBook&& sb) noexcept
    : year(sb.year),
    recordBooks(std::move(sb.recordBooks)) {}

SemesterBook::~SemesterBook() = default;

SemesterBook& SemesterBook::operator=(const SemesterBook& sb) {
    if (this != &sb) {
        year = sb.year;
        recordBooks = sb.recordBooks;
    }
    return *this;
}

SemesterBook& SemesterBook::operator=(SemesterBook&& sb) noexcept {
    if (this != &sb) {
        year = sb.year;
        recordBooks = std::move(sb.recordBooks);
    }
    return *this;
}

void SemesterBook::setYear(unsigned int value) {
    year = value;
}

unsigned int SemesterBook::getYear() const {
    return year;
}

MyContainer<RecordBook> SemesterBook::getRecordBooks() const {
    return recordBooks;
}

MyContainer<RecordBook> SemesterBook::getRetakingCourseStudents() const {
    MyContainer<RecordBook> result{};
    for (auto const& rb : recordBooks)
        for (auto const& s : rb.getSubjects())
            if (s.getNationalGrade() <= 2) {
                result.add(rb);
                break;
            }
    return result;
}

double SemesterBook::getAverageRating() const {
    double sum = 0.0;
    for (auto const& rb : recordBooks)
        sum += rb.getRating();
    return sum / recordBooks.getSize();
}

void SemesterBook::add(const RecordBook& rb) {
    recordBooks.add(rb);
}

void SemesterBook::add(RecordBook&& rb) {
    recordBooks.add(std::move(rb));
}

std::istream& operator>>(std::istream& is, SemesterBook& sb) {
    std::string classTag;
    unsigned int year = 0;
    MyContainer<RecordBook> recordBooks{};

    if (!(is >> classTag >> year >> recordBooks)
        || classTag != "SemesterBook") {
        is.setstate(std::ios::failbit);
        return is;
    }

    sb.year = year;
    sb.recordBooks = std::move(recordBooks);
    return is;
}

std::ostream& operator<<(std::ostream& os, const SemesterBook& sb) {
    os << "SemesterBook "
        << sb.year << ' '
        << sb.recordBooks;
    return os;
}
