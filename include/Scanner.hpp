#pragma once
#include "serial_cpp/serial.h"
#include <functional>
#include <span>
#include <string>
#include <thread>
#include <vector>

class Scanner
{
  public:
    const std::string port_name;
    static std::vector<std::string> get_scanner_ports();
    void stop();
    static std::string as_string(const std::span<const uint8_t> data);
    Scanner(std::string port_name, std::function<void(std::span<const uint8_t>)> callback,
            std::function<void(const char*, const size_t)> error_callback);
    ~Scanner();

  private:
    void poll_scanner(std::stop_token token);
    serial_cpp::Serial serial_port;
    std::function<void(const std::span<uint8_t>)> on_read;
    std::function<void(const char*, const size_t)> on_error;
    std::jthread reader_thread;
};
