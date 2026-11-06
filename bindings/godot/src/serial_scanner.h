#pragma once

#include "Scanner.hpp"

#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/variant/packed_string_array.hpp>
#include <godot_cpp/variant/string.hpp>
#include <memory>

namespace godot
{

class SerialScanner : public Node
{
    GDCLASS(SerialScanner, Node)

  public:
    void set_port_name(const String& p_port_name);
    String get_port_name() const;

    bool open();
    void close();
    bool is_open() const;

    static PackedStringArray get_ports();

    void _exit_tree() override;

  protected:
    static void _bind_methods();

  private:
    String port_name;
    std::unique_ptr<Scanner> scanner;
};

} // namespace godot
