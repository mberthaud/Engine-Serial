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

char** get_scanner_ports_c(size_t* count)
{
    std::vector<std::string> ports = Scanner::get_scanner_ports();
    char** result = new char*[ports.size()];
    for (size_t i = 0; i < ports.size(); i++)
    {
        result[i] = new char[ports[i].size() + 1];
        std::memcpy(result[i], ports[i].c_str(), ports[i].size() + 1);
    }
    *count = ports.size();
    return result;
}

void free_scanner_ports_c(char** ports, size_t count)
{
    for (size_t i = 0; i < count; i++)
    {
        delete[] ports[i];
    }
    delete[] ports;
}
