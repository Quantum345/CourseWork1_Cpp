#pragma once
#include "Book.h"
#include "Subject.h"
#include "MyContainer.h"
#include <iostream>
#include <string>

class RecordBook :
    public Book {
private:
    std::string student;
    std::string university;
    unsigned int startYear = 0;
    unsigned int academicYear = 0;
    MyContainer<Subject> subjects{};
public:
    RecordBook() = default;
    RecordBook(
        const std::string& student,
        const std::string& university,
        unsigned int startYear,
        unsigned int academicYear,
        const Subject* subjects,
        size_t subjectsAmount
    );
    RecordBook(
        const Book& b,
        const std::string& student,
        const std::string& university,
        unsigned int startYear,
        unsigned int academicYear,
        const Subject* subjects,
        size_t subjectsAmount
    );
    RecordBook(const RecordBook& rb);
    RecordBook(RecordBook&& rb) noexcept;
    ~RecordBook() override;

    RecordBook& operator=(const RecordBook&);
    RecordBook& operator=(RecordBook&&) noexcept;

    void setStudent(const std::string& fullName);
    void setUniversity(const std::string& name);
    void setStartYear(unsigned int value);
    void setAcademicYear(unsigned int value);

    std::string getStudent() const;
    std::string getUniversity() const;
    unsigned int getAcademicYear() const;
    size_t getSubjectsAmount() const;
    MyContainer<Subject> getSubjects() const;
    float getRating() const;
    int getSubjectGrade(const std::string& subjectName) const;

    void addSubject(const Subject&);
    void addSubject(Subject&&);

    friend std::istream& operator>>(std::istream& is, RecordBook& rb);
    friend std::ostream& operator<<(std::ostream& os, const RecordBook& rb);
};

