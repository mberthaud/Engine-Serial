#include <Scanner.hpp>
#include <iostream>
#include <serial_cpp/serial.h>
#include <span>
#include <string>

void some_user_func(std::span<const uint8_t> data)
{
    std::string x = Scanner::as_string(data);
    std::cout << "recv from scanner: " << x << '\n';
}
void some_user_error_func(const char* what, size_t len)
{
    std::string x = std::string(what);
    std::cout << "error from scanner: " << x << '\n';
}

int main()
{
    std::vector<std::string> ports = Scanner::get_scanner_ports();
    if (ports.empty())
    {
        std::cout << "no scanner found\n";
        return 1;
    }
    Scanner s(ports[0], some_user_func, some_user_error_func);
    while (true)
    {
    }
    return 0;
}
