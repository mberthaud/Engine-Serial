#include "ScannerC.hpp"
#include "Scanner.hpp"
#include <span>

void* scanner_create(const char* str, ScannerDataCallback data_callback, ScannerErrorCallback error_callback)
{
    if (data_callback == nullptr)
    {
        return nullptr;
    }
    if (error_callback == nullptr)
    {
        return nullptr;
    }

    int len = std::strlen(str);
    std::string port = std::string(str, len);
    Scanner* scanner = nullptr;
    try
    {
        scanner = new Scanner(
            port, [=](std::span<const uint8_t> data) -> void { data_callback(data.data(), data.size()); },
            [=](const char* what, const size_t len) -> void { error_callback(what, len); });
    }
    catch (std::exception& e)
    {
    }
    return scanner;
}

void scanner_destroy(Scanner* scanner)
{
    delete scanner;
}

void get_scanner_ports_c()
{
    Scanner::get_scanner_ports();
}
