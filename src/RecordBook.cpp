#include <iomanip>
#include <utility>
#include "RecordBook.h"

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
subjects(subjects, subjectsAmount) {}

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
subjects(subjects, subjectsAmount) {}

RecordBook::RecordBook(const RecordBook& rb)
    : Book(rb),
    student(rb.student),
    university(rb.university),
    startYear(rb.startYear),
    academicYear(rb.academicYear),
    subjects(rb.subjects) {}

RecordBook::RecordBook(RecordBook&& rb) noexcept
    : Book(std::move(rb)),
    student(std::move(rb.student)),
    university(std::move(rb.university)),
    startYear(rb.startYear),
    academicYear(rb.academicYear),
    subjects(std::move(rb.subjects)) {}

RecordBook::~RecordBook() = default;

RecordBook& RecordBook::operator=(const RecordBook& rb) {
    if (this != &rb) {
        student = rb.student;
        university = rb.university;
        startYear = rb.startYear;
        academicYear = rb.academicYear;
        subjects = rb.subjects;
    }
    return *this;
}

RecordBook& RecordBook::operator=(RecordBook&& rb) noexcept {
    if (this != &rb) {
        student = std::move(rb.student);
        university = std::move(rb.university);
        startYear = rb.startYear;
        academicYear = rb.academicYear;
        subjects = std::move(rb.subjects);
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
    return this->student;
}

std::string RecordBook::getUniversity() const {
    return this->university;
}

unsigned int RecordBook::getAcademicYear() const {
    return this->academicYear;
}

size_t RecordBook::getSubjectsAmount() const {
    return this->subjects.getSize();
}

MyContainer<Subject> RecordBook::getSubjects() const {
    return this->subjects;
}

float RecordBook::getRating() const {
    int sum = 0;
    for (const auto& s : subjects)
        sum += s.getGrade();
    return (float)sum / this->subjects.getSize();
}

int RecordBook::getSubjectGrade(const std::string& subjectName) const {
    for (const auto& s : subjects)
        if (subjectName == s.getName())
            return s.getGrade();
    return -1;
}

void RecordBook::printId(std::ostream& os) const {
    os << "Номер залікової книги: " << startYear << "-" << student;
}

void RecordBook::addSubject(const Subject& s) {
    subjects.add(s);
}

void RecordBook::addSubject(Subject&& s) {
    subjects.add(std::move(s));
}

std::istream& operator>>(std::istream& is, RecordBook& rb) {
    std::string classTag;
    std::string student;
    std::string university;
    unsigned int startYear = 0;
    unsigned int academicYear = 0;
    MyContainer<Subject> subjects;

    if (!(is >> classTag >> static_cast<Book&>(rb)
          >> std::quoted(student) >> std::quoted(university)
          >> startYear >> academicYear >> subjects)
        || classTag != "RecordBook") {
        is.setstate(std::ios::failbit);
        return is;
    }

    rb.student = std::move(student);
    rb.university = std::move(university);
    rb.startYear = startYear;
    rb.academicYear = academicYear;
    rb.subjects = std::move(subjects);
    return is;
}

std::ostream& operator<<(std::ostream& os, const RecordBook& rb) {
    os << "RecordBook " << static_cast<const Book&>(rb) << ' '
        << std::quoted(rb.student) << ' '
        << std::quoted(rb.university) << ' '
        << rb.startYear << ' '
        << rb.academicYear << ' '
        << rb.subjects;
    return os;
}
