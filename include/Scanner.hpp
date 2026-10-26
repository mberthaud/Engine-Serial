#pragma once
#include "serial_cpp/serial.h"
#include <functional>
#include <span>
#include <string>
#include <thread>
using namespace std;
class Scanner
{
  public:
    const string port_name;
    static void get_scanner_ports();
    void stop();
    static string as_string(const span<const uint8_t> data);
    Scanner(string port_name, function<void(span<const uint8_t>)> callback);
    ~Scanner();

  private:
    void poll_scanner(stop_token token);
    serial_cpp::Serial serial_port;
    function<void(const span<uint8_t>)> on_read;
    jthread reader_thread;
};
