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
		"signal_test": signal_test
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

func signal_test():
	test_signal.emit()
