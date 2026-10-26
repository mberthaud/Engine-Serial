#include <Scanner.hpp>
#include <iostream>
#include <ostream>
#include <serial_cpp/serial.h>
#include <span>
#include <string>

void some_user_func(span<const uint8_t> data)
{
    string x = Scanner::as_string(data);
    std::cout << "recv from scanner: " << x << std::endl;
}

int main()
{
    Scanner s("/dev/ttyACM0", some_user_func);
    while (true)
    {
    }
    return 0;
}
