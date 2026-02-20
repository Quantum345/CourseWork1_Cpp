#include "UdcCode.h"

UdcCode::UdcCode()
{}

UdcCode::UdcCode(const std::string& code) : code(code)
{}

UdcCode::UdcCode(const UdcCode& udcCode) : code(udcCode.code)
{}

UdcCode::UdcCode(UdcCode&& udcCode) noexcept : code(std::move(udcCode.code))
{}

UdcCode::~UdcCode()
{}

UdcCode& UdcCode::operator=(const UdcCode& udcCode)
{
    if (this != &udcCode)
    {
        this->code = udcCode.code;
    }
    return *this;
}

UdcCode& UdcCode::operator=(UdcCode&& udcCode) noexcept
{
    if (this != &udcCode)
    {
        this->code = std::move(udcCode.code);
    }
    return *this;
}

void UdcCode::setUdcCode(const std::string& code)
{
    this->code = code;
}

std::string UdcCode::getUdcCode()
{
    return this->code;
}

std::istream& operator>>(std::istream& is, UdcCode& udcCode)
{
    return is;
}

std::ostream& operator<<(std::ostream& os, const UdcCode& udcCode)
{
    return os;
}
