#include <Scanner.hpp>
#include <exception>
#include <iostream>
#include <ostream>
#include <stop_token>
using namespace std;

Scanner::Scanner(string port, function<void(span<const uint8_t>)> callback)
    : port_name(std::move(port)), serial_port(port_name), on_read(std::move(callback))
{
    std::cout << "opening scanner at port: " << port_name << std::endl;
    uint32_t timeout_ms = 100;
    serial_port.setTimeout(serial_cpp::Timeout::simpleTimeout(timeout_ms));

    reader_thread = jthread([&](stop_token token) -> void { poll_scanner(token); });
}

void Scanner::poll_scanner(stop_token stop_token)
{
    vector<uint8_t> buf = vector<uint8_t>(1024);
    while (!stop_token.stop_requested())
    {
        if (serial_port.waitReadable())
        {
            size_t bytes = serial_port.available();
            if (bytes > buf.size())
            {
                buf.resize(bytes * 2);
            }
            size_t bytes_read = serial_port.read(buf.data(), bytes);
            if (bytes_read > 0)
            {
                std::span scanner_data(buf.data(), bytes_read);
                try
                {
                    on_read(scanner_data);
                }
                catch (const std::exception& e)
                {
                    std::cerr << e.what() << std::endl;
                }
            }
        }
    }
}

void Scanner::stop()
{
    reader_thread.request_stop();
}

string Scanner::as_string(const span<const uint8_t> data)
{
    return string(reinterpret_cast<const char*>(data.data()), data.size());
}

Scanner::~Scanner()
{
    stop();
}

void Scanner::get_scanner_ports()
{
    //@TODO: all the scanners have the same description and hardware id on linux so we could filter
    // this list down to only return to callers callers the ports we know are scanners.
    // I dont know if thats the case on other platforms so leaving this as as stub for now...
    std::vector<serial_cpp::PortInfo> devices_found = serial_cpp::list_ports();
    for (const serial_cpp::PortInfo& device : devices_found)
    {
        printf("(%s, %s, %s)\n", device.port.c_str(), device.description.c_str(), device.hardware_id.c_str());
    }
}
