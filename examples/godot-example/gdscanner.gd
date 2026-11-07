class_name ScannerTest
extends Node
@export var PORT :=""
var scanner:SerialScanner

# Called when the node enters the scene tree for the first time.
func _ready() -> void:
	if PORT.is_empty():
		var ports := SerialScanner.get_ports()
		if ports.is_empty():
			print("no scanner found")
			return
		PORT = ports[0]

	scanner = SerialScanner.new();
	add_child(scanner);
	scanner.port_name=PORT;
	scanner.data_received.connect(_on_data_recv)
	scanner.open();
	
func _on_data_recv(data:PackedByteArray) -> void:
	print("recv from scanner: " ,data.get_string_from_utf8())

# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta: float) -> void:
	pass
