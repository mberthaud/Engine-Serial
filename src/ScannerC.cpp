#include "ScannerC.hpp"
#include <span>

void* scanner_create(const char* str, ScannerCallback callback)
{
    if (callback == NULL)
    {
        return NULL;
    }

    int len = std::strlen(str);
    string port = string(str, len);
    Scanner* scanner = new Scanner(port, [=](span<const uint8_t> data) -> void { callback(data.data(), data.size()); });
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
