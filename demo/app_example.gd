@tool
extends HtmlRect

signal test_signal

func _ready():
	test_signal.connect(func(): print("test_signal"))

func _on_dom_ready(_url):
	return {
		"bool": true,
		"int": 69,
		"float": 3.1415,
		"string": "Hello World!",
		"null": null,
		"callable": test_callable,
		"vector3": Vector3(1, 2, 3),
		"plane": Plane(1, 2, 3, 4),
		"quaternion": Quaternion(1, 2, 3, 4),
		"dictionary": {
			"test": "Hello World!",
			"nested_callable": test_nested_callable
		},
		"array": [1, 2, 3, 4, 5],
		"object": self,
		"signal": _signal_to_dictionary(test_signal)
	}

func test_callable(temp = ""):
	print("test_callable")
	print("temp: ", temp)
	if temp is Callable:
		var result = temp.call(1, 2, 3, test_nested_callable)
		print("result: ", result)

func test_nested_callable():
	print("test_nested_callable")
	return 420

func _signal_to_dictionary(p_signal: Signal):
	return {
		"connect": p_signal.connect,
		# "disconnect": p_signal.disconnect, # ALERT: Disconnect does not work yet.
		"emit": p_signal.emit,
		"get_connections": p_signal.get_connections,
		"get_name": p_signal.get_name,
		"get_object": p_signal.get_object,
		"get_object_id": p_signal.get_object_id,
		"has_connections": p_signal.has_connections,
		# "is_connected": p_signal.is_connected, # ALERT: Is_connected probably doesn't work. Same reason as above.
		"is_null": p_signal.is_null
	}
