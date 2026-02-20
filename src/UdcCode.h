#pragma once
#include <string>

class UdcCode
{
private:
    std::string code;
public:
    UdcCode();
    UdcCode(const std::string& code);
    UdcCode(const UdcCode& udcCode);
    UdcCode(UdcCode&& udcCode);
    ~UdcCode();

    UdcCode& operator=(const UdcCode& udcCode);
    UdcCode& operator=(UdcCode&& udcCode);

    void setUdcCode(const std::string& code);

    std::string getUdcCode();

    friend std::istream& operator>>(std::istream& is, UdcCode& udcCode);
    friend std::ostream& operator<<(std::ostream& os, const UdcCode& udcCode);
};

