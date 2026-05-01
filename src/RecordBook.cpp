#include "RecordBook.h"
#include "MyContainer.h"
#include "Book.h"
#include "Subject.h"
#include "UdcCode.h"
#include <iostream>
#include <iomanip>
#include <string>
#include <algorithm>

RecordBook::RecordBook(
    const std::string& student,
    const std::string& university,
    unsigned int startYear,
    unsigned int academicYear,
    const Subject* subjects,
    size_t subjectsAmount
) : Book(UdcCode(), "", "Залікова книга", "", 0),
student(student),
university(university),
startYear(startYear),
academicYear(academicYear),
subjects(new Subject[subjectsAmount]),
subjectsAmount(subjectsAmount) {
    std::copy(subjects, subjects + subjectsAmount, this->subjects);
    iter = new MyContainer<Subject>(this->subjects, this->subjectsAmount);
}

RecordBook::RecordBook(
    const Book& b,
    const std::string& student,
    const std::string& university,
    unsigned int startYear,
    unsigned int academicYear,
    const Subject* subjects,
    size_t subjectsAmount
) : Book(b),
student(student),
university(university),
startYear(startYear),
academicYear(academicYear),
subjects(new Subject[subjectsAmount]),
subjectsAmount(subjectsAmount) {
    std::copy(subjects, subjects + subjectsAmount, this->subjects);
    iter = new MyContainer<Subject>(this->subjects, this->subjectsAmount);
}

RecordBook::RecordBook(const RecordBook& rb)
    : Book(rb),
    student(rb.student),
    university(rb.university),
    startYear(rb.startYear),
    academicYear(rb.academicYear),
    subjects(new Subject[rb.subjectsAmount]),
    subjectsAmount(rb.subjectsAmount) {
    std::copy(rb.subjects, rb.subjects + rb.subjectsAmount, this->subjects);
    iter = new MyContainer<Subject>(this->subjects, this->subjectsAmount);
}

RecordBook::RecordBook(RecordBook&& rb) noexcept
    : Book(rb),
    academicYear(rb.academicYear),
    subjects(rb.subjects),
    subjectsAmount(rb.subjectsAmount),
    iter(rb.iter) {
    rb.subjects = nullptr;
    rb.subjectsAmount = 0;
    rb.iter = nullptr;
}

RecordBook::~RecordBook() {
    delete this->iter;
    delete[] subjects;
}

RecordBook& RecordBook::operator=(const RecordBook& rb) {
    if (this != &rb) {
        delete this->iter;
        delete[] subjects;

        academicYear = rb.academicYear;
        subjects = new Subject[rb.subjectsAmount];
        subjectsAmount = rb.subjectsAmount;
        std::copy(rb.subjects, rb.subjects + rb.subjectsAmount, this->subjects);
        iter = new MyContainer<Subject>(this->subjects, this->subjectsAmount);
    }
    return *this;
}

RecordBook& RecordBook::operator=(RecordBook&& rb) noexcept {
    if (this != &rb) {
        delete this->iter;
        delete[] subjects;

        academicYear = rb.academicYear;
        subjects = rb.subjects;
        subjectsAmount = rb.subjectsAmount;
        iter = rb.iter;

        rb.subjects = nullptr;
        rb.subjectsAmount = 0;
        rb.iter = nullptr;
    }
    return *this;
}

void RecordBook::setStudent(const std::string& fullName) {
    this->student = fullName;
}

void RecordBook::setUniversity(const std::string& name) {
    this->university = name;
}

void RecordBook::setStartYear(unsigned int value) {
    this->startYear = value;
}

void RecordBook::setAcademicYear(unsigned int academicYear) {
    this->academicYear = academicYear;
}

std::string RecordBook::getStudent() const {
    return std::string();
}

std::string RecordBook::getUniversity() const {
    return std::string();
}

unsigned int RecordBook::getAcademicYear() const {
    return this->academicYear;
}

size_t RecordBook::getSubjectsAmount() const {
    return this->subjectsAmount;
}

Subject* RecordBook::getSubjects() const {
    return this->subjects;
}

float RecordBook::getRating() const {
    int sum = 0;
    for (const auto& s : *iter)
        sum += s.getGrade();
    return (float)sum / this->subjectsAmount;
}

int RecordBook::getSubjectGrade(std::string subjectName) const {
    for (const auto& s : *iter)
        if (subjectName == s.getName())
            return s.getGrade();
    return -1;
}

std::istream& operator>>(std::istream& is, RecordBook& rb) {
    std::string classTag;
    std::string student;
    std::string university;
    unsigned int startYear = 0;
    unsigned int academicYear = 0;
    size_t subjectsAmount = 0;

    if (!(is >> classTag) || classTag != "RecordBook") {
        is.setstate(std::ios::failbit);
        return is;
    }

    if (!(is >> static_cast<Book&>(rb)))
        return is;

    if (!(is >> std::quoted(student)))
        return is;

    if (!(is >> std::quoted(university)))
        return is;

    if (!(is >> startYear))
        return is;

    if (!(is >> academicYear))
        return is;

    if (!(is >> subjectsAmount))
        return is;

    Subject* newSubjects = nullptr;
    if (subjectsAmount > 0)
        newSubjects = new Subject[subjectsAmount];

    for (size_t i = 0; i < subjectsAmount; ++i) {
        if (!(is >> newSubjects[i])) {
            delete[] newSubjects;
            is.setstate(std::ios::failbit);
            return is;
        }
    }

    delete[] rb.subjects;
    delete rb.iter;
    rb.student = student;
    rb.university = university;
    rb.academicYear = startYear;
    rb.academicYear = academicYear;
    rb.subjects = newSubjects;
    rb.subjectsAmount = subjectsAmount;
    if (rb.subjects == nullptr || rb.subjectsAmount == 0)
        rb.iter = nullptr;
    else
        rb.iter = new MyContainer<Subject>(rb.subjects, rb.subjectsAmount);
    return is;
}

std::ostream& operator<<(std::ostream& os, const RecordBook& rb) {
    os << "RecordBook " << static_cast<const Book&>(rb) << ' '
        << std::quoted(rb.student) << ' '
        << std::quoted(rb.university) << ' '
        << rb.startYear << ' '
        << rb.academicYear << ' '
        << rb.subjectsAmount;
    if (rb.subjectsAmount == 0 || rb.iter == nullptr)
        return os;
    for (auto const& s : *rb.iter)
        os << ' ' << s;
    return os;
}
