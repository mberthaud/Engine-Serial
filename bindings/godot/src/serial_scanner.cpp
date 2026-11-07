#include "serial_scanner.h"

#include <cstring>
#include <exception>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/packed_byte_array.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

using namespace godot;

void SerialScanner::_bind_methods()
{
    ClassDB::bind_method(D_METHOD("set_port_name", "port_name"), &SerialScanner::set_port_name);
    ClassDB::bind_method(D_METHOD("get_port_name"), &SerialScanner::get_port_name);
    ClassDB::bind_method(D_METHOD("open"), &SerialScanner::open);
    ClassDB::bind_method(D_METHOD("close"), &SerialScanner::close);
    ClassDB::bind_method(D_METHOD("is_open"), &SerialScanner::is_open);
    ClassDB::bind_static_method(get_class_static(), D_METHOD("get_ports"), &SerialScanner::get_ports);

    ADD_PROPERTY(PropertyInfo(Variant::STRING, "port_name"), "set_port_name", "get_port_name");

    ADD_SIGNAL(MethodInfo("data_received", PropertyInfo(Variant::PACKED_BYTE_ARRAY, "data")));
    ADD_SIGNAL(MethodInfo("error", PropertyInfo(Variant::STRING, "message")));
}

void SerialScanner::set_port_name(const String& p_port_name)
{
    port_name = p_port_name;
}

String SerialScanner::get_port_name() const
{
    return port_name;
}

bool SerialScanner::open()
{
    close();
    try
    {
        scanner = std::make_unique<Scanner>(
            port_name.utf8().get_data(),

            [this](std::span<const uint8_t> data) -> void {
                PackedByteArray bytes;
                bytes.resize(static_cast<int64_t>(data.size()));
                std::memcpy(bytes.ptrw(), data.data(), data.size());
                call_deferred("emit_signal", "data_received", bytes);
            },

            [this](const char* what, const size_t len) -> void {
                call_deferred("emit_signal", "error", String::utf8(what, static_cast<int64_t>(len)));
            });
    }

    catch (const std::exception& e)
    {
        UtilityFunctions::push_error("SerialScanner: failed to open ", port_name, ": ", e.what());
        return false;
    }
    return true;
}

void SerialScanner::close()
{
    scanner.reset();
}

bool SerialScanner::is_open() const
{
    return scanner != nullptr;
}

PackedStringArray SerialScanner::get_ports()
{
    PackedStringArray ports;
    for (const std::string& port : Scanner::get_scanner_ports())
    {
        ports.push_back(String::utf8(port.c_str()));
    }
    return ports;
}

void SerialScanner::_exit_tree()
{
    close();
}
