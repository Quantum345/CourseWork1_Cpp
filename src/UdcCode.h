#pragma once
#include <iostream>
#include <string>

class UdcCode {
private:
    std::string code;
public:
    UdcCode();
    explicit UdcCode(const std::string& code);
    UdcCode(const UdcCode& udcCode);
    UdcCode(UdcCode&& udcCode) noexcept;
    ~UdcCode();

    UdcCode& operator=(const UdcCode& udcCode);
    UdcCode& operator=(UdcCode&& udcCode) noexcept;

    void setCode(const std::string& code);

    std::string getCode() const;

    friend std::istream& operator>>(std::istream& is, UdcCode& udcCode);
    friend std::ostream& operator<<(std::ostream& os, const UdcCode& udcCode);
};
