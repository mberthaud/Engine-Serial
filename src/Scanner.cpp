#include <Scanner.hpp>
#include <algorithm>
#include <cctype>
#include <exception>
#include <functional>
#include <iostream>
#include <stop_token>

Scanner::Scanner(std::string port, std::function<void(std::span<const uint8_t>)> callback,
                 std::function<void(const char*, const size_t)> error_callback)
    : port_name(std::move(port)), serial_port(port_name), on_read(std::move(callback)),
      on_error(std::move(error_callback))
{
    std::cout << "opening scanner at port: " << port_name << '\n';
    uint32_t timeout_ms = 100;
    serial_port.setTimeout(serial_cpp::Timeout::simpleTimeout(timeout_ms));
    reader_thread = std::jthread([&](std::stop_token token) -> void { poll_scanner(token); });
}

void Scanner::poll_scanner(std::stop_token stop_token)
{
    std::vector<uint8_t> buf = std::vector<uint8_t>(1024);
    while (!stop_token.stop_requested())
    {
        try
        {
            size_t bytes = serial_port.available();
            if (bytes == 0)
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(5));
                continue;
            }

            if (bytes > buf.size())
            {
                buf.resize(bytes * 2);
            }

            size_t bytes_read = serial_port.read(buf.data(), bytes);
            if (bytes_read > 0)
            {
                std::span scanner_data(buf.data(), bytes_read);
                on_read(scanner_data);
            }
        }
        catch (const std::exception& e)
        {
            const char* what = e.what();
            size_t len = std::strlen(what);
            on_error(what, len);
            break;
        }
    }
}

void Scanner::stop()
{
    reader_thread.request_stop();
}

std::string Scanner::as_string(const std::span<const uint8_t> data)
{
    return {reinterpret_cast<const char*>(data.data()), data.size()};
}

Scanner::~Scanner()
{
    stop();
}

std::vector<std::string> Scanner::get_scanner_ports()
{
    //@NOTE: scanners report differently on windows vs unix
    // Windows reports: "USB\VID_152A&PID_880F&..."
    // macOS and Linux report "USB VID:PID=152a:880f ..."
    std::vector<std::string> ports;
    for (const serial_cpp::PortInfo& device : serial_cpp::list_ports())
    {
        std::string id = device.hardware_id;
        std::ranges::transform(id, id.begin(), [](unsigned char c) { return std::tolower(c); });
        if (id.find("vid_152a&pid_880f") != std::string::npos || id.find("vid:pid=152a:880f") != std::string::npos)
        {
            ports.push_back(device.port);
        }
    }
    return ports;
}
