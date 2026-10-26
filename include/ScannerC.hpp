#include "Scanner.hpp"
#if defined(_WIN32)
#define SCANNER_API __declspec(dllexport)
#else
#define SCANNER_API __attribute__((visibility("default")))
#endif
extern "C"
{
    typedef void (*ScannerCallback)(const uint8_t*, size_t);
    SCANNER_API void* scanner_create(const char* str, ScannerCallback callback);
    SCANNER_API void scanner_destroy(Scanner* scanner);
    SCANNER_API void get_scanner_ports_c();
}
