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
    Scanner::get_scanner_ports();
    Scanner s("/dev/cu.usbmodem00000000050C1", some_user_func, some_user_error_func);
    while (true)
    {
    }
    return 0;
}
