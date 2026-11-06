extends Node

signal scan_detected(scan_string)

var current_string : String

func _input(input_event: InputEvent) -> void:
	# Discard events that aren't keyboard inputs
	if !(input_event is InputEventKey):
		return;
		
	if !input_event.pressed:
		return
	
	if input_event.get_keycode() == 4194309:
		FlushCurrentString();
		return;	
	
	current_string += OS.get_keycode_string(input_event.get_key_label_with_modifiers())
	
func FlushCurrentString():
	print("Scan detected: " + current_string)
	scan_detected.emit(current_string)
	current_string = ""
	
