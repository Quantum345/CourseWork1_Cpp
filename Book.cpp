#include "Book.h"

Book::Book(const UdcCode& udcCode, const std::string& author, const std::string& name, const std::string& publishingHouse, unsigned int year)
	: udcCode(udcCode),
	author(author),
	name(name),
	publishingHouse(publishingHouse),
	year(year)
{}

Book::Book(const Book& b)
{}

Book::Book(Book && b)
{}

Book::~Book()
{}

Book& Book::operator=(const Book & b)
{
	return *this;
}

Book& Book::operator=(Book&& b) noexcept
{
	return *this;
}

Book& Book::operator()(
	const UdcCode& udcCode,
	const std::string& author,
	const std::string& name,
	const std::string& publishingHouse,
	unsigned int year)
{
	this->setUdcCode(udcCode);
	this->setAuthor(author);
	this->setName(name);
	this->setPublishingHouse(publishingHouse);
	this->setYear(year);
}

void Book::setUdcCode(const UdcCode& udc)
{
	this->udcCode = &udc;
}

void Book::setAuthor(const std::string& author)
{
	this->author = author;
}

void Book::setName(const std::string& name)
{
	this->name = name;
}

void Book::setPublishingHouse(const std::string& publishingHouse)
{
	this->publishingHouse = publishingHouse;
}

void Book::setYear(unsigned int year)
{
	this->year = year;
}

UdcCode Book::getUdcCode() const
{
	return this->udcCode;
}

std::string Book::getAuthor() const
{
	return this->author;
}

std::string Book::getName() const
{
	return this->name;
}

std::string Book::getPublishingHouse() const
{
	return this->publishingHouse;
}

unsigned int Book::getYear() const
{
	return this->year;
}

std::istream& operator>>(std::istream& is, Book& b)
{
	return is;
}

std::ostream& operator<<(std::ostream& os, const Book& b)
{
	return os;
}
