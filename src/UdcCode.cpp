#include <iomanip>
#include <utility>
#include "UdcCode.h"

UdcCode::UdcCode() = default;

UdcCode::UdcCode(const std::string& code) : code(code) {}

UdcCode::UdcCode(const UdcCode& udcCode) : code(udcCode.code) {}

UdcCode::UdcCode(UdcCode&& udcCode) noexcept : code(std::move(udcCode.code)) {}

UdcCode::~UdcCode() = default;

UdcCode& UdcCode::operator=(const UdcCode& udcCode) {
    if (this != &udcCode)
        this->code = udcCode.code;
    return *this;
}

UdcCode& UdcCode::operator=(UdcCode&& udcCode) noexcept {
    if (this != &udcCode)
        this->code = std::move(udcCode.code);
    return *this;
}

void UdcCode::setUdcCode(const std::string& code) {
    this->code = code;
}

std::string UdcCode::getUdcCode() const {
    return this->code;
}

std::istream& operator>>(std::istream& is, UdcCode& udcCode) {
    std::string classTag;
    std::string code;

    if (!(is >> classTag >> std::quoted(code))
        || classTag != "UdcCode") {
        is.setstate(std::ios::failbit);
        return is;
    }

    udcCode.setUdcCode(code);
    return is;
}

std::ostream& operator<<(std::ostream& os, const UdcCode& udcCode) {
    os << "UdcCode " << std::quoted(udcCode.code);
    return os;
}
